

#include "glpch.h"

/*
#include "VulkanImgui.h"
#include "Gale/Core/Application.h"

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_vulkan.h"

namespace Gale {

	VulkanImgui::VulkanImgui(Ref<VulkanDevice> device, Ref<VulkanSwapChain> swapchain):
		m_Device(device), m_Swapchain(swapchain)
	{
		Init();
	}

	VulkanImgui::~VulkanImgui()
	{
	}

	void VulkanImgui::Init()
	{
		// Create descriptor pool

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
		pool_info.maxSets = 1000;
		pool_info.poolSizeCount = std::size(pool_sizes);
		pool_info.pPoolSizes = pool_sizes;

		VkDescriptorPool imguiPool;
		vkCreateDescriptorPool(m_Device->device(), &pool_info, nullptr, &imguiPool);

		// Create render pass

		VkAttachmentDescription attachment = {};
		attachment.format = m_Swapchain->getSwapChainImageFormat();
		attachment.samples = VK_SAMPLE_COUNT_1_BIT;
		attachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
		attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachment.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference color_attachment = {};
		color_attachment.attachment = 0;
		color_attachment.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass = {};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &color_attachment;

		VkSubpassDependency dependency = {};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;  // or VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		VkRenderPassCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		info.attachmentCount = 1;
		info.pAttachments = &attachment;
		info.subpassCount = 1;
		info.pSubpasses = &subpass;
		info.dependencyCount = 1;
		info.pDependencies = &dependency;
		if (vkCreateRenderPass(m_Device->device(), &info, nullptr, &imGuiRenderPass) != VK_SUCCESS) {
			throw std::runtime_error("Could not create Dear ImGui's render pass");
		}

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForVulkan(Application::Get().GetWindow().GetWindowPointer(), true);
		ImGui_ImplVulkan_InitInfo init_info = {};
		init_info.Instance = m_Device->getInstance();
		init_info.PhysicalDevice = m_Device->getPhysicalDevice();
		init_info.Device = m_Device->device();
		init_info.QueueFamily = 0;
		init_info.Queue = m_Device->graphicsQueue();
		init_info.PipelineCache = nullptr;
		init_info.DescriptorPool = imguiPool;
		init_info.Allocator = nullptr;
		init_info.MinImageCount = m_Swapchain->imageCount();
		init_info.ImageCount = m_Swapchain->imageCount();
		init_info.CheckVkResultFn = nullptr;
		ImGui_ImplVulkan_Init(&init_info, imGuiRenderPass);
	}

	void VulkanImgui::Begin()
	{
	}

	void VulkanImgui::End()
	{
	}

}

*/