#pragma once
#include "Gale/Base.h"
#include "Vulkan.h"
#include "VulkanInstance.h"

namespace Gale {

	class VulkanContext 
	{
	public:
		VulkanContext();
		~VulkanContext();

	private:
		Ref<VulkanInstance> m_Instance;
	};

}