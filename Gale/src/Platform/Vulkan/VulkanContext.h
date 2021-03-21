#pragma once
#include "Gale/Base.h"
#include "Vulkan.h"
#include "VulkanInstance.h"
#include "VulkanDevice.h"

namespace Gale {

	class VulkanContext 
	{
	public:
		VulkanContext();
		~VulkanContext();

	private:
		Ref<VulkanInstance> m_Instance;
		Ref<VulkanPhysicalDevice> m_PhysicalDevice;
		Ref<VulkanDevice> m_Device;
	};

}