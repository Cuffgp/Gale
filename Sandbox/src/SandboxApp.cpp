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

	if (Gale::RendererAPI::Current() == Gale::RendererAPI::API::Vulkan)
		Gale::VulkanDevice::Init();
	else
		Gale::DirectXDevice::Init();

	return new SandboxApp();
}