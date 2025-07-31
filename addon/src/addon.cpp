#include "addon_internal.hpp"

#include <reshade.hpp>

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		if (!reshade::register_addon(hinstDLL))
			return FALSE;
		addon::IsAddonRegistered = true;
		break;
	case DLL_PROCESS_DETACH:
		reshade::unregister_addon(hinstDLL);
		addon::IsAddonRegistered = false;
		break;
	}
	return TRUE;
}
