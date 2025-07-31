#include <addon.hpp>

#pragma warning(push)
#include "F4SE/F4SE.h"
#include "RE/Fallout.h"
#include <spdlog/sinks/basic_file_sink.h>
#pragma warning(pop)

#define DLLEXPORT __declspec(dllexport)

namespace logger = F4SE::log;

bool InitLogger()
{
	auto path = logger::log_directory();
	if (!path)
	{
		return false;
	}

	*path /= std::format("{}.log", PLUGIN_NAME);
	auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);

	auto log = std::make_shared<spdlog::logger>("global log", std::move(sink));

	log->set_level(spdlog::level::info);
	log->flush_on(spdlog::level::warn);

	spdlog::set_default_logger(std::move(log));
	spdlog::set_pattern("%g(%#): [%^%l%$] %v");

	return true;
}

extern "C" DLLEXPORT bool F4SEAPI F4SEPlugin_Query(const F4SE::QueryInterface *a_f4se, F4SE::PluginInfo *a_info)
{
	a_info->infoVersion = F4SE::PluginInfo::kVersion;
	a_info->name = PLUGIN_NAME;
	a_info->version = addon::version;

	if (!InitLogger())
		return false;

	logger::info("{} v{}", a_info->name, std::format("{}", addon::version));

	if (a_f4se->IsEditor())
	{
		logger::critical("Loaded in editor");
		return false;
	}

	const auto runtime_version = a_f4se->RuntimeVersion();
	if (runtime_version < F4SE::RUNTIME_1_10_162)
	{
		logger::critical("Unsupported runtime: v{}", runtime_version.string());
		return false;
	}

	if (!addon::api::OK())
	{
		logger::warn("ReShade not found");
	}

	return true;
}

namespace
{
	template <auto Func, typename = signature_t<decltype(Func)>>
	struct NativeMethodAdapter;

	template <auto Func, typename R, typename... Args>
	struct NativeMethodAdapter<Func, R(Args...)>
	{
		static R invoke(std::monostate, Args... args)
		{
			return std::invoke(Func, std::forward<Args>(args)...);
		}
	};
}

extern "C" DLLEXPORT bool F4SEAPI F4SEPlugin_Load(const F4SE::LoadInterface *a_f4se)
{
	F4SE::Init(a_f4se);

	const F4SE::PapyrusInterface *papyrus = F4SE::GetPapyrusInterface();
	papyrus->Register([](RE::BSScript::IVirtualMachine *a_vm) {
		addon::ForEachAPIFunction([&]<auto Function>(std::string_view name)
		{
			a_vm->BindNativeMethod("ReShade", name, NativeMethodAdapter<Function>::invoke);
		});
		return true;
	});

	addon::api::LogInfo("F4SE Plugin loaded. Papyrus bindings registered.");

	return true;
}
