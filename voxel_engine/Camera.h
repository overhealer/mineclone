#include <glm/glm.hpp>
#include <glm/ext.hpp>

class Camera
{
	void updateVectors();
public:
	glm::vec3 forward;
	glm::vec3 up;
	glm::vec3 right;

	glm::mat4 rotation;

	float fov;
	glm::vec3 position;
	Camera(glm::vec3 position, float fov);
	~Camera();

	void rotate(float x, float y, float z);

	glm::mat4 getProjection();
	glm::mat4 getView();
};

