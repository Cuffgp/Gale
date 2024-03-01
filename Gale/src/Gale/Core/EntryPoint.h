#pragma once

#include "Gale/Renderer/RendererAPI.h"
#include "Gale/Vulkan/VulkanDevice.h"
#include "Gale/DirectX/DirectXDevice.h"
#include "Gale/Core/Log.h"

extern Gale::Application* Gale::CreateApplication();

int main(int argc, char** argv)
{
	Gale::Log::Init();

	auto app = Gale::CreateApplication();
	app->Run();
	delete app;

	if (Gale::RendererAPI::Current() == Gale::RendererAPI::API::Vulkan)
	{
		Gale::VulkanDevice::Destroy();
	}
	else
	{
		Gale::DirectXDevice::Destroy();
	}
}