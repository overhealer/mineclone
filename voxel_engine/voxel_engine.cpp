#include "voxel_engine.h"

int WIDTH = 480;
int HEIGHT = 320;

float vertices[] = 
{
		-0.01f,-0.01f,
		0.01f, 0.01f,

	   -0.01f, 0.01f,
		0.01f,-0.01f,
};

int attrs[] = {
	 2, 0
};

int main()
{
	Window::initialize(WIDTH, HEIGHT, "Cool window");
	Events::initialize();

	Shader* shader = load_shader("main.glslv", "main.glslf");
	if (shader == nullptr)
	{
		std::cerr << "FAILED TO LOAD BASE SHADERS" << std::endl;
		Window::terminate();
		return -1;
	}

	Shader* crosshairShader = load_shader("crosshair.glslv", "crosshair.glslf");
	if (crosshairShader == nullptr) 
	{
		std::cerr << "FAILED TO LOAD CROSSHAIR SHADER" << std::endl;
		Window::terminate();
		return 1;
	}

	Texture* texture = load_texture("Textures/block.png");
	if (texture == nullptr)
	{
		std::cerr << "failed to load texture" << std::endl;
		delete shader;
		Window::terminate();
		return -1;
	}

	Chunks* chunks = new Chunks(8, 1, 8);
	Mesh** meshes = new Mesh*[chunks->volume];
	for (size_t i = 0; i < chunks->volume; i++)
		meshes[i] = nullptr;
	VoxelRenderer renderer(1024*1024*8);

	glClearColor(0.1, 0.1, 0.1, 1);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Mesh* crosshair = new Mesh(vertices, 4, attrs);
	Camera* camera = new Camera(glm::vec3(0, 0, 20), glm::radians(90.0f));

	glm::mat4 model(1.0f);

	//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
	//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));	

	//time system
	float lastTime = glfwGetTime();
	float delta = 0.0f;

	float camX = 0.0f;
	float camY = 0.0f;

	float speed = 5;

	//main loop
	while (!Window::isShouldClose())
	{
		float currentTime = glfwGetTime();
		delta = currentTime - lastTime;
		lastTime = currentTime;

		if (Events::justPressed(GLFW_KEY_ESCAPE))
		{
			Window::shouldClose(true);
		}

		if (Events::justPressed(GLFW_KEY_TAB))
		{
			Events::toggleCursor();
		}

		if (Events::justPressed(GLFW_KEY_X))
		{
			Window::setRandomScreenColor();
		}

		if (Events::isPressed(GLFW_KEY_W))
		{
			camera->position += camera->forward * speed * delta;
		}

		if (Events::isPressed(GLFW_KEY_S))
		{
			camera->position -= camera->forward * speed * delta;
		}

		if (Events::isPressed(GLFW_KEY_A))
		{
			camera->position -= camera->right * speed * delta;
		}

		if (Events::isPressed(GLFW_KEY_D))
		{
			camera->position += camera->right * speed * delta;
		}

		if (Events::isPressed(GLFW_KEY_SPACE))
		{
			camera->position += camera->up * speed * delta;
		}

		if (Events::isPressed(GLFW_KEY_LEFT_SHIFT))
		{
			camera->position -= camera->up * speed * delta;
		}

		//rotate camera
		if (Events::_cursor_locked) {
			camY += -Events::deltaY / Window::height * 2;
			camX += -Events::deltaX / Window::height * 2;

			if (camY < -radians(89.0f)) {
				camY = -radians(89.0f);
			}
			if (camY > radians(89.0f)) {
				camY = radians(89.0f);
			}

			camera->rotation = mat4(1.0f);
			camera->rotate(camY, camX, 0);
		}

		{
			vec3 end;
			vec3 norm;
			vec3 iend;
			voxel* vox = chunks->rayCast(camera->position, camera->forward, 10.0f, end, norm, iend);
			if (vox != nullptr) {
				if (Events::justClicked(GLFW_MOUSE_BUTTON_1)) {
					chunks->set((int)iend.x, (int)iend.y, (int)iend.z, 0);
				}
				if (Events::justClicked(GLFW_MOUSE_BUTTON_2)) {
					chunks->set((int)(iend.x) + (int)(norm.x), (int)(iend.y) + (int)(norm.y), (int)(iend.z) + (int)(norm.z), 2);
				}
			}
		}

		Chunk* closes[27];
		for (size_t i = 0; i < chunks->volume; i++) {
			Chunk* chunk = chunks->chunks[i];
			if (!chunk->modified)
				continue;
			chunk->modified = false;
			if (meshes[i] != nullptr)
				delete meshes[i];

			for (int i = 0; i < 27; i++)
				closes[i] = nullptr;
			for (size_t j = 0; j < chunks->volume; j++) {
				Chunk* other = chunks->chunks[j];

				int ox = other->x - chunk->x;
				int oy = other->y - chunk->y;
				int oz = other->z - chunk->z;

				if (abs(ox) > 1 || abs(oy) > 1 || abs(oz) > 1)
					continue;

				ox += 1;
				oy += 1;
				oz += 1;
				closes[(oy * 3 + oz) * 3 + ox] = other;
			}
			Mesh* mesh = renderer.render(chunk, (const Chunk**)closes);
			meshes[i] = mesh;
		}


		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Draw VAO
		shader->use();
		shader->uniformMatrix("projview", camera->getProjection() * camera->getView());
		texture->bind();
		mat4 model(1.0f);
		for (size_t i = 0; i < chunks->volume; i++) {
			Chunk* chunk = chunks->chunks[i];
			Mesh* mesh = meshes[i];
			model = glm::translate(mat4(1.0f), vec3(chunk->x * CHUNK_W + 0.5f, chunk->y * CHUNK_H + 0.5f, chunk->z * CHUNK_D + 0.5f));
			shader->uniformMatrix("model", model);
			mesh->draw(GL_TRIANGLES);
		}
		
		crosshairShader->use();
		crosshair->draw(GL_LINES);

		Window::swapBuffers();
		Events::pullEvents();
	}

	delete texture;
	delete shader;
	delete crosshair;
	delete crosshairShader;
	delete chunks;
	Window::terminate();
	return 0;
}