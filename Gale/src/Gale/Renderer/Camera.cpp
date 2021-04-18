#include "glpch.h"
#include "Camera.h"

namespace Gale {

	Camera::Camera(float left, float right, float bottom, float top) :
		m_Projection(glm::ortho(left, right, top, bottom, 0.0f, 1.0f)), m_Transformation(1.0f)
	{

	}

}