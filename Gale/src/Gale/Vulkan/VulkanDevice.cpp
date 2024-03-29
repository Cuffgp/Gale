#include "glpch.h"

#include "Gale/Vulkan/VulkanDevice.h"

namespace Gale {

	VulkanDevice* VulkanDevice::s_Device = nullptr;

	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData)
	{
		if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
		{
			GL_ERROR("[Vulkan] {}", pCallbackData->pMessage);
		}

		return VK_FALSE;
	}

	VulkanDevice::VulkanDevice()
	{
		CreateInstance();
		SetupDebugMessenger();
		CreatePhysicalDevice();
		CreateDevice();
	}

	VulkanDevice::~VulkanDevice()
	{
		vkDestroyDescriptorPool(m_Device, m_DescriptorPool, nullptr);
		vkDestroyCommandPool(m_Device, m_CommandPool, nullptr);
		vkDestroyDevice(m_Device, nullptr);

		if (m_Validation)
		{
			auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_Instance, "vkDestroyDebugUtilsMessengerEXT");
			if (func)
			{
				func(m_Instance, m_DebugMessenger, nullptr);
			}
			else
			{
				GL_ERROR("Debug extension not found.");
			}
		}

		vkDestroyInstance(m_Instance,nullptr);
	}

	void VulkanDevice::Init()
	{
		s_Device = new VulkanDevice();
	}

	void VulkanDevice::Destroy()
	{
		delete s_Device;
	}

	void VulkanDevice::CreateInstance()
	{
		// Enumerate and save the avalible instance extensions
		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		m_InstanceExtensions.resize(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, m_InstanceExtensions.data());

		GL_TRACE("Avalible instance extensions:");
		for (auto extension : m_InstanceExtensions)
		{
			GL_TRACE("\t {}", extension.extensionName);
		}

		// List required layers and instance extensions
		const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };

		std::vector<const char*> instanceExtensions = { VK_KHR_SURFACE_EXTENSION_NAME };
		instanceExtensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
		if (m_Validation)
		{
			instanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		// Create the vulkan instance
		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Gale Application";
		appInfo.pEngineName = "Gale Engine";
		appInfo.apiVersion = VK_API_VERSION_1_3;

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		createInfo.enabledExtensionCount = static_cast<uint32_t>(instanceExtensions.size());
		createInfo.ppEnabledExtensionNames = instanceExtensions.data();

		if (m_Validation)
		{
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();
		}
		else
		{
			createInfo.enabledLayerCount = 0;
		}

		if (vkCreateInstance(&createInfo, nullptr, &m_Instance) != VK_SUCCESS)
		{
			GL_ERROR("Failed to create instance!");
		}
	}

	void VulkanDevice::SetupDebugMessenger()
	{
		if (m_Validation)
		{
			VkDebugUtilsMessengerCreateInfoEXT createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
			createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
			createInfo.pfnUserCallback = DebugCallback;
			createInfo.pUserData = nullptr;

			auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_Instance, "vkCreateDebugUtilsMessengerEXT");
			if (func)
			{
				if (func(m_Instance, &createInfo, nullptr, &m_DebugMessenger) != VK_SUCCESS)
				{
					GL_ERROR("Failed to setup debug messenger.");
				}
			}
			else
			{
				GL_ERROR("Debug extension not found.");
			}
		}
	}

	void VulkanDevice::CreatePhysicalDevice()
	{
		// Take the first physical device avalible
		GL_INFO("Selecting Physical Device");
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(m_Instance, &deviceCount, nullptr);
		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(m_Instance, &deviceCount, devices.data());
		GL_INFO("Physical devices: {}", deviceCount);

		for (const auto& physicalDevice : devices)
		{
			VkPhysicalDeviceProperties deviceProperties;
			vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);

			GL_INFO("{}, {}", deviceProperties.deviceName,
				string_VkPhysicalDeviceType(deviceProperties.deviceType));
		}
		
		m_PhysicalDevice = devices[0];
		vkGetPhysicalDeviceProperties(m_PhysicalDevice, &m_PhysicalDeviceProperties);

		// Find the queue familys for graphics, compute, and transfer
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queueFamilyCount, nullptr);
		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		for (const auto& queueFamily : queueFamilies)
		{
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				if (m_QueueFamilyIndices.Graphics == UINT32_MAX)
				{
					m_QueueFamilyIndices.Graphics = i;
				}
			}

			if (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT)
			{
				if (m_QueueFamilyIndices.Compute == UINT32_MAX)
				{
					m_QueueFamilyIndices.Compute = i;
				}
			}

			if (queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT)
			{
				if (m_QueueFamilyIndices.Transfer == UINT32_MAX)
				{
					m_QueueFamilyIndices.Transfer = i;
				}
			}
			i++;
		}

		// Enumerate and save the avalible device extensions
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(m_PhysicalDevice, nullptr, &extensionCount, nullptr);
		m_DeviceExtensions.resize(extensionCount);
		vkEnumerateDeviceExtensionProperties(m_PhysicalDevice, nullptr, &extensionCount, m_DeviceExtensions.data());

		GL_TRACE("Avalible device extensions:");
		for (auto extension : m_DeviceExtensions)
		{
			GL_TRACE("\t {}", extension.extensionName);
		}
	}

	void VulkanDevice::CreateDevice()
	{
		// Required features
		VkPhysicalDeviceFeatures deviceFeatures{};
		deviceFeatures.samplerAnisotropy = VK_TRUE;
		deviceFeatures.fillModeNonSolid = VK_TRUE;
		deviceFeatures.wideLines = VK_TRUE;
		deviceFeatures.geometryShader = VK_TRUE;

		// Required extensions
		const std::vector<const char*> deviceExtensions =
		{
			VK_KHR_SWAPCHAIN_EXTENSION_NAME,
			VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME,
		};

		VkPhysicalDeviceDynamicRenderingFeaturesKHR dynamicRenderingFeaturesKHR{};
		dynamicRenderingFeaturesKHR.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES_KHR;
		dynamicRenderingFeaturesKHR.dynamicRendering = VK_TRUE;
		dynamicRenderingFeaturesKHR.pNext = nullptr;

		// Create the device and queues
		float queuePriority = 1.0f;
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = m_QueueFamilyIndices.Graphics;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;

		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.pQueueCreateInfos = &queueCreateInfo;
		createInfo.queueCreateInfoCount = 1;
		createInfo.pEnabledFeatures = &deviceFeatures;
		createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
		createInfo.ppEnabledExtensionNames = deviceExtensions.data();
		createInfo.pNext = &dynamicRenderingFeaturesKHR;

		if (vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, &m_Device) != VK_SUCCESS)
		{
			GL_ERROR("Failed to create logical device!");
		}

		//GL_INFO("Queue indices: {}, {}, {}", m_QueueFamilyIndices.Graphics, m_QueueFamilyIndices.Compute, m_QueueFamilyIndices.Transfer);
		vkGetDeviceQueue(m_Device, m_QueueFamilyIndices.Graphics, 0, &m_GraphicsQueue);
		vkGetDeviceQueue(m_Device, m_QueueFamilyIndices.Compute, 0, &m_ComputeQueue);
		vkGetDeviceQueue(m_Device, m_QueueFamilyIndices.Transfer, 0, &m_TransferQueue);

		// Create the command pool
		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		poolInfo.queueFamilyIndex = m_QueueFamilyIndices.Graphics;

		if (vkCreateCommandPool(m_Device, &poolInfo, nullptr, &m_CommandPool) != VK_SUCCESS)
		{
			GL_ERROR("Failed to create command pool!");
		}

		// Create the descriptor pool
		VkDescriptorPoolSize pool_sizes[] =
		{
			{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
		};
		VkDescriptorPoolCreateInfo pool_info = {};
		pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		pool_info.maxSets = 1000 * 11;
		pool_info.poolSizeCount = (uint32_t)11;
		pool_info.pPoolSizes = pool_sizes;

		if (vkCreateDescriptorPool(m_Device, &pool_info, nullptr, &m_DescriptorPool) != VK_SUCCESS)
		{
			GL_ERROR("Failed to create descriptor pool!");
		}
	}

	uint32_t VulkanDevice::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
	{
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(m_PhysicalDevice, &memProperties);
		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
		{
			if ((typeFilter & (1 << i)) &&
				(memProperties.memoryTypes[i].propertyFlags & properties) == properties)
			{
				return i;
			}
		}
		GL_ERROR("Failed to find suitable memory type!");
	}

	VkFormat VulkanDevice::FindSupportedFormat(
		const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
	{
		for (VkFormat format : candidates)
		{
			VkFormatProperties props;
			vkGetPhysicalDeviceFormatProperties(m_PhysicalDevice, format, &props);

			if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
			{
				return format;
			}
			else if (
				tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
			{
				return format;
			}
		}
		GL_ERROR("Failed to find supported format!");
	}

	VkCommandBuffer VulkanDevice::BeginSingleTimeCommands()
	{
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = m_CommandPool;
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(m_Device, &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);
		return commandBuffer;
	}

	void VulkanDevice::EndSingleTimeCommands(VkCommandBuffer commandBuffer)
	{
		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(m_GraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(m_GraphicsQueue);

		vkFreeCommandBuffers(m_Device, m_CommandPool, 1, &commandBuffer);
	}

	void VulkanDevice::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
	{
		VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

		VkBufferCopy copyRegion{};
		copyRegion.srcOffset = 0;  // Optional
		copyRegion.dstOffset = 0;  // Optional
		copyRegion.size = size;
		vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

		EndSingleTimeCommands(commandBuffer);
	}

	void VulkanDevice::CreateImageWithInfo(
		const VkImageCreateInfo& imageInfo,
		VkMemoryPropertyFlags properties,
		VkImage& image,
		VkDeviceMemory& imageMemory)
	{
		if (vkCreateImage(m_Device, &imageInfo, nullptr, &image) != VK_SUCCESS)
		{
			GL_ERROR("Failed to create image!");
		}

		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(m_Device, image, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);

		if (vkAllocateMemory(m_Device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS)
		{
			GL_ERROR("Failed to allocate image memory!");
		}

		if (vkBindImageMemory(m_Device, image, imageMemory, 0) != VK_SUCCESS)
		{
			GL_ERROR("Failed to bind image memory!");
		}
	}

}