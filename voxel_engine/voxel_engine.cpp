#include "voxel_engine.h"

int WIDTH = 480;
int HEIGHT = 320;

float vertices[] =
{ // x     y     z     u     v
   -1.0f,-1.0f, 0.0f, 0.0f, 0.0f,
	1.0f,-1.0f, 0.0f, 1.0f, 0.0f,
   -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

	1.0f,-1.0f, 0.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
   -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
};

int main()
{
	Window::initialize(WIDTH, HEIGHT, "Cool window");
	Events::initialize();

	Shader* shader = load_shader("main.glslv", "main.glslf");
	if (shader == nullptr)
	{
		std::cerr << "FAILED TO LOAD SHADERS" << std::endl;
		Window::terminate();
		return -1;
	}

	Texture* texture = load_texture("Textures/test_texture.png");
	if (texture == nullptr)
	{
		std::cerr << "failed to load texture" << std::endl;
		delete shader;
		Window::terminate();
		return -1;
	}

	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	glClearColor(0.1, 0.1, 0.1, 1);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Camera* camera = new Camera(glm::vec3(0, 0, 1), glm::radians(90.0f));

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

		if (Events::justPressed(GLFW_MOUSE_BUTTON_1))
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

			if (camY < -glm::radians(89.0f)) {
				camY = -glm::radians(89.0f);
			}
			if (camY > glm::radians(89.0f)) {
				camY = glm::radians(89.0f);
			}

			camera->rotation = glm::mat4(1.0f);
			camera->rotate(camY, camX, 0);
		}


		glClear(GL_COLOR_BUFFER_BIT);

		//Draw VAO
		shader->use();
		shader->uniformMatrix("model", model);
		shader->uniformMatrix("proj_view", camera->getProjection()*camera->getView());
		texture->bind();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		Window::swapBuffers();
		Events::pullEvents();
	}

	delete texture;
	delete shader;
	glDeleteTextures(1, &VAO);
	glDeleteBuffers(1, &VBO);
	Window::terminate();
	return 0;
}