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

	switch (Gale::RendererAPI::Current())
	{
	case Gale::RendererAPI::API::Vulkan: { Gale::VulkanDevice::Destroy(); break; }
	case Gale::RendererAPI::API::DirectX: { Gale::DirectXDevice::Destroy(); break; }
	}
}