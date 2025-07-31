#include "addon_internal.hpp"

#include <reshade.hpp>

namespace api = addon::api;

namespace
{
	int LogMessage(reshade::log::level level, std::string_view message)
	{
		if (!api::OK())
			return api::NO_RESHADE;

		reshade::log::message(level, message.data());
		return 0;
	}
}

bool api::OK()
{
	return addon::IsAddonRegistered;
}

int api::LogError(std::string_view asMessage)
{
	return LogMessage(reshade::log::level::error, asMessage);
}

int api::LogWarning(std::string_view asMessage)
{
	return LogMessage(reshade::log::level::warning, asMessage);
}

int api::LogInfo(std::string_view asMessage)
{
	return LogMessage(reshade::log::level::info, asMessage);
}

int api::LogDebug(std::string_view asMessage)
{
	return LogMessage(reshade::log::level::debug, asMessage);
}
