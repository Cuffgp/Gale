#include "glpch.h"
#include "VulkanContext.h"

namespace Gale {

	VulkanContext::VulkanContext()
	{
		m_Instance = CreateRef<VulkanInstance>();
	}

	VulkanContext::~VulkanContext()
	{
	}


}