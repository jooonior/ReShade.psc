#include "addon_internal.hpp"

#include <reshade.hpp>

#include <vector>

using reshade::api::effect_runtime;

namespace
{
	std::vector<effect_runtime *> runtimes;

	void on_init_effect_runtime(effect_runtime *runtime)
	{
		runtimes.push_back(runtime);
	}

	void on_destroy_effect_runtime(effect_runtime *runtime)
	{
		std::erase(runtimes, runtime);
	}
}

effect_runtime *addon::GetRuntime(int index)
{
	return index < runtimes.size() ? runtimes[index] : nullptr;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		if (!reshade::register_addon(hinstDLL))
			return FALSE;
		reshade::register_event<reshade::addon_event::init_effect_runtime>(on_init_effect_runtime);
		reshade::register_event<reshade::addon_event::destroy_effect_runtime>(on_destroy_effect_runtime);
		addon::IsAddonRegistered = true;
		break;
	case DLL_PROCESS_DETACH:
		reshade::unregister_addon(hinstDLL);
		addon::IsAddonRegistered = false;
		break;
	}
	return TRUE;
}
