#include <addon.hpp>

#include <RE/Fallout.h>
#include <F4SE/F4SE.h>

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

F4SE_PLUGIN_LOAD(const F4SE::LoadInterface *a_f4se)
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
