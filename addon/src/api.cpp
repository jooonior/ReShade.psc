#include "addon_internal.hpp"
#include "memoize.hpp"

#include <reshade.hpp>
#include <re2/re2.h>

namespace api = addon::api;

using namespace reshade::api;

// Wrap an invocable expression in a perfect-forwarding generic lambda.
#define TO_LAMBDA(Func) ([&](auto&&... args) { return Func(std::forward<decltype(args)>(args)...); })

#define RETURN_IF_FAILED(Result) do { if (Result < 0) return Result; } while(0);
#define RETURN_IF_BAD_RUNTIME(Runtime) do { if (Runtime == nullptr) return api::BAD_RUNTIME; } while(0);

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

	auto RegExp = memoize(256, [](std::string_view s) {
		return RE2(absl::string_view{ s.data(), s.size() });
	});

	template<typename F>
	int EnumerateTechniques(effect_runtime *runtime, std::string_view pattern, F f)
	{
		const RE2 &regex = RegExp(pattern);
		if (!regex.ok())
		{
			addon::LogError("Error in RegExp {:?}: {}", pattern, regex.error());
			return api::BAD_REGEX;
		}

		int count = 0;

		runtime->enumerate_techniques(nullptr, [&](auto, effect_technique technique) {
			std::string name = GetStringFromReShade(TO_LAMBDA(runtime->get_technique_name), technique);
			if (!RE2::FullMatch(name, regex))
				return;

			std::invoke(f, technique);
			count += 1;
		});

		return count;
	}
}

int api::SetTechniqueState(std::string_view asTechniquePattern, bool abEnabled, int aiRuntime)
{
	effect_runtime *runtime = addon::GetRuntime(aiRuntime);
	RETURN_IF_BAD_RUNTIME(runtime);

	int count = EnumerateTechniques(runtime, asTechniquePattern, [&](effect_technique technique) {
		runtime->set_technique_state(technique, abEnabled);
	});

	return count;
}

int api::GetTechniqueState(std::string_view asTechniquePattern, bool abEnabled, int aiRuntime)
{
	effect_runtime *runtime = addon::GetRuntime(aiRuntime);
	RETURN_IF_BAD_RUNTIME(runtime);

	bool all_match = true;

	int count = EnumerateTechniques(runtime, asTechniquePattern, [&](effect_technique technique) {
		all_match &= runtime->get_technique_state(technique) == abEnabled;
	});

	RETURN_IF_FAILED(count);

	return all_match ? count : 0;
}

int api::SetPreprocessorDefinition(std::string_view asName, std::string_view asValue, int aiRuntime)
{
	effect_runtime *runtime = addon::GetRuntime(aiRuntime);
	RETURN_IF_BAD_RUNTIME(runtime);

	runtime->set_preprocessor_definition(asName.data(), asValue.data());
	return 0;
}
