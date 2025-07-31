#include "addon_internal.hpp"
#include "memoize.hpp"

#include <reshade.hpp>
#include <re2/re2.h>

namespace api = addon::api;

using namespace reshade::api;

// Wrap an invocable expression in a perfect-forwarding generic lambda.
#define TO_LAMBDA(Func) ([&](auto&&... args) { return Func(std::forward<decltype(args)>(args)...); })

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

namespace
{
	template <typename Func, typename... Args>
		requires std::invocable<Func, Args..., char *, std::size_t *>
	std::string GetStringFromReShade(Func getter, Args... args)
	{
		// Get size.
		std::size_t size;
		std::invoke(getter, args..., nullptr, &size);

		// Get string.
		std::string buffer(size, 0);
		std::invoke(getter, args..., buffer.data(), &size);
		buffer.resize(size);

		return buffer;
	}

	auto RegExp = memoize([](std::string_view s) {
		return RE2(absl::string_view{ s.data(), s.size() });
	});
}

int api::SetTechniqueState(std::string_view asTechniquePattern, bool abEnabled, int aiRuntime)
{
	reshade::api::effect_runtime *runtime = addon::GetRuntime(aiRuntime);
	if (!runtime)
		return api::BAD_RUNTIME;

	const RE2 &regex = RegExp(asTechniquePattern);
	if (!regex.ok())
	{
		addon::LogError("Error in RegExp {:?}: {}", asTechniquePattern, regex.error());
		return api::BAD_REGEX;
	}

	int count = 0;

	runtime->enumerate_techniques(nullptr, [&](auto, effect_technique technique) {
		std::string name = GetStringFromReShade(TO_LAMBDA(runtime->get_technique_name), technique);
		if (!RE2::FullMatch(name, regex))
			return;

		runtime->set_technique_state(technique, abEnabled);
		count += 1;
	});

	return count;
}
