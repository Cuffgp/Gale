#include "glpch.h"
#include "VulkanContext.h"

namespace Gale {

	VulkanContext::VulkanContext()
	{
		m_Instance = CreateRef<VulkanInstance>();
		m_PhysicalDevice = CreateRef<VulkanPhysicalDevice>();
		m_Device = CreateRef<VulkanDevice>(m_PhysicalDevice);
	}

	VulkanContext::~VulkanContext()
	{
	}


}