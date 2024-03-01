#pragma once

#include "Gale/Core/Base.h"

#include "Gale/Renderer/ShaderData.h"

namespace Gale {

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		static Ref<VertexBuffer> Create(void* data, uint32_t size, VertexInput input);

		virtual const VertexInput& GetInput() const = 0;
		virtual void SetData(void* data) = 0;
	private:

	};

}