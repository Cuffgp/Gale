#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Gale {

	class Camera 
	{
	public:
		Camera(float left, float right, float bottom, float top);

	private:
		glm::mat4 m_Projection;
		glm::mat4 m_Transformation;

		glm::vec3 m_Position;
		glm::vec3 m_Facing;
	};

}