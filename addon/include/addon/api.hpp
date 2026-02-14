#pragma once

#include <string_view>

#define ScriptName
#define ReShade
#define native
#define hidden

#define Property
#define AutoReadOnly
#define Int constexpr int

#define Function
#define global
#define native

#define string std::string_view

namespace addon::api
{
#include "Scripts/Source/User/ReShade.psc"
}

#undef ScriptName
#undef ReShade
#undef native
#undef hidden

#undef Property
#undef AutoReadOnly
#undef Int

#undef Function
#undef global
#undef native

#undef string
