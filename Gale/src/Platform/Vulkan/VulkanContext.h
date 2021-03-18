#pragma once
#include "Vulkan.h"
#include "GLFW/glfw3.h"

namespace Gale {

	class VulkanContext 
	{
	public:
		VulkanContext();
		~VulkanContext();

	private:
		static VkInstance m_Instance;
	};

}