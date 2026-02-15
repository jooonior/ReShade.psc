#pragma once

#include "addon/api.hpp"
#include "addon/utils.hpp"
#include "addon/config.hpp"

#include <array>
#include <string_view>

// Should be set when this header is included in a plugin project.
#ifdef PLUGIN_FLAVOR
extern "C" __declspec(dllexport) const char *NAME = "Papyrus";
extern "C" __declspec(dllexport) const char *DESCRIPTION = "Papyrus bindings for ReShade's addon API. " PLUGIN_FLAVOR " version.";
#endif

namespace addon
{
	namespace detail
	{
		consteval std::string_view strip_namespace(std::string_view name)
		{
			std::size_t start = name.find_last_of(":") + 1;
			name.remove_prefix(start);
			return name;
		}
	}

#define INVOKE_CALLBACK(Callback, Function) \
	Callback.operator()<Function>(detail::strip_namespace(#Function))

	template<typename T>
	void ForEachAPIFunction(T callback)
	{
		INVOKE_CALLBACK(callback, api::OK);
		INVOKE_CALLBACK(callback, api::LogError);
		INVOKE_CALLBACK(callback, api::LogWarning);
		INVOKE_CALLBACK(callback, api::LogInfo);
		INVOKE_CALLBACK(callback, api::LogDebug);
		INVOKE_CALLBACK(callback, api::SetTechniqueState);
		INVOKE_CALLBACK(callback, api::GetTechniqueState);
		INVOKE_CALLBACK(callback, api::SetPreprocessorDefinition);
	}

#undef INVOKE_CALLBACK
}
