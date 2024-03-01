#pragma once

#include "Gale/Core/Base.h"


namespace Gale {

	class Pipeline
	{
	public:
		virtual ~Pipeline() = default;

		static Ref<Pipeline> Create(std::string filePath);
		static Ref<Pipeline> Create(std::string vertPath, std::string fragPath);
	private:

	};

}