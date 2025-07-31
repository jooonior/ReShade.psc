#pragma once

#include "addon.hpp"

#include <reshade.hpp>

namespace addon
{
	inline bool IsAddonRegistered = false;

	reshade::api::effect_runtime *GetRuntime(int index);

	namespace detail
	{
		template<typename... Args>
		void LogMessage(reshade::log::level level, std::format_string<Args...> fmt, Args&&... args)
		{
			std::string message = std::format(fmt, std::forward<Args>(args)...);
			reshade::log::message(level, message.c_str());
		}
	}

	template<typename... Args>
	void LogError(std::format_string<Args...> fmt, Args&&... args)
	{
		detail::LogMessage(reshade::log::level::error, fmt, std::forward<Args>(args)...);
	}

	template<typename... Args>
	void LogWarning(std::format_string<Args...> fmt, Args&&... args)
	{
		detail::LogMessage(reshade::log::level::warning, fmt, std::forward<Args>(args)...);
	}

	template<typename... Args>
	void LogInfo(std::format_string<Args...> fmt, Args&&... args)
	{
		detail::LogMessage(reshade::log::level::info, fmt, std::forward<Args>(args)...);
	}

	template<typename... Args>
	void LogDebug(std::format_string<Args...> fmt, Args&&... args)
	{
		detail::LogMessage(reshade::log::level::debug, fmt, std::forward<Args>(args)...);
	}
}
