#pragma once

#include "Vulkan.h"


namespace Gale {

	struct QueueFamilyIndices
	{
		int32_t graphicsFamily = -1;
		int32_t presentFamily = -1;

		bool isComplete() { return graphicsFamily >= 0; }
	};

	class VulkanPhysicalDevice 
	{
	public:
		VulkanPhysicalDevice();
		~VulkanPhysicalDevice();

		bool isDeviceSuitable(VkPhysicalDevice device);
		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

		VkPhysicalDevice GetPhysicalDevice() { return m_PhysicalDevice; }
		QueueFamilyIndices GetIndices() { return m_Indices; }
	private:
		VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
		QueueFamilyIndices m_Indices;
	};

	class VulkanDevice 
	{
	public:
		VulkanDevice(Ref<VulkanPhysicalDevice> physicalDevice);
		~VulkanDevice();

	private:
		VkDevice m_Device;
		VkQueue m_GraphicsQueue;
	};

}