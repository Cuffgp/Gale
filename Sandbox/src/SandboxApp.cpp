#include "Gale.h"
#include "Gale/Core/EntryPoint.h"

class SandboxApp : public Gale::Application
{
public:
	SandboxApp()
	{

	}

	~SandboxApp()
	{

	}
private:

};

Gale::Application* Gale::CreateApplication()
{
	Gale::RendererAPI::Set(RendererAPI::API::DirectX);

	switch (RendererAPI::Current())
	{
	case RendererAPI::API::Vulkan: { Gale::VulkanDevice::Init(); break; }
	case RendererAPI::API::DirectX: { Gale::DirectXDevice::Init(); break; }
	}

	return new SandboxApp();
}