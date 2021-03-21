#include "glpch.h"

#include "VulkanInstance.h"
#include "VulkanDevice.h"

namespace Gale {

	//   --------------------------------
	//	 ---- Vulkan Physical Device ----
	//   --------------------------------

	VulkanPhysicalDevice::VulkanPhysicalDevice()
	{
		auto instance = VulkanInstance::Get();

		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

		if (deviceCount == 0)
			GL_ERROR("Failed to find GPUs with Vulkan support!");

		GL_INFO("Found {} physical devices.", deviceCount);

		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

		for (const auto& device : devices) 
		{
			if (isDeviceSuitable(device))
			{
				m_PhysicalDevice = device;
				break;
			}
		}

		if (m_PhysicalDevice == VK_NULL_HANDLE)
			GL_ERROR("failed to find a suitable GPU!");

	}

	VulkanPhysicalDevice::~VulkanPhysicalDevice()
	{

	}

	bool VulkanPhysicalDevice::isDeviceSuitable(VkPhysicalDevice device)
	{
		VkPhysicalDeviceProperties deviceProperties;
		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceProperties(device, &deviceProperties);
		vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

		m_Indices = findQueueFamilies(device);

		GL_ERROR("Graphics indicies {}", m_Indices.graphicsFamily);

		return m_Indices.isComplete();
	}

	QueueFamilyIndices VulkanPhysicalDevice::findQueueFamilies(VkPhysicalDevice device)
	{
		QueueFamilyIndices indices;

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		for (const auto& queueFamily : queueFamilies)
		{
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
				indices.graphicsFamily = i;

			//if (indices.isComplete())
			//	break;
			
			i++;
		}

		return indices;
	}

	//   --------------------------------
	//	 ---- Vulkan Logical Device -----
	//   --------------------------------

	VulkanDevice::VulkanDevice(Ref<VulkanPhysicalDevice> PhysicalDevice)
	{
		auto physicalDevice = PhysicalDevice->GetPhysicalDevice();
		auto indices = PhysicalDevice->GetIndices();

		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = indices.graphicsFamily;
		queueCreateInfo.queueCount = 1;
		float queuePriority = 1.0f;
		queueCreateInfo.pQueuePriorities = &queuePriority;

		VkPhysicalDeviceFeatures deviceFeatures{};

		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

		createInfo.pQueueCreateInfos = &queueCreateInfo;
		createInfo.queueCreateInfoCount = 1;

		createInfo.pEnabledFeatures = &deviceFeatures;
		createInfo.enabledExtensionCount = 0;

		if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &m_Device) != VK_SUCCESS)
			GL_ERROR("Failed to create logical device!");

		vkGetDeviceQueue(m_Device, indices.graphicsFamily, 0, &m_GraphicsQueue);


	}

	VulkanDevice::~VulkanDevice()
	{
		vkDestroyDevice(m_Device, nullptr);
	}
}