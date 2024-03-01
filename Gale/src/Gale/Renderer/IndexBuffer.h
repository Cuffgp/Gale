#pragma once

#include "Gale/Core/Base.h"

namespace Gale {

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		static Ref<IndexBuffer> Create(void* data, uint32_t size);

		virtual uint32_t GetIndexCount() = 0;

	private:

	};

}