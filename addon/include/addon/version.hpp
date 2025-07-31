#pragma once

#include <array>
#include <format>

namespace addon
{
	struct version_t
	{
		std::uint8_t major = 0;
		std::uint8_t minor = 0;
		std::uint8_t patch = 0;
		std::uint8_t tweak = 0;

		constexpr operator std::uint32_t() const
		{
			std::uint32_t version = 0;
			for (auto n : std::array{ major, minor, patch, tweak })
			{
				version <<= 8;
				version += n;
			}
			return version;
		}
	};

	static_assert(version_t{ 1, 2, 3, 4 } == 0x01020304);
}

template <>
struct std::formatter<addon::version_t>
{
	constexpr auto parse(std::format_parse_context &ctx)
	{
		return ctx.begin();
	}

	auto format(const addon::version_t &v, std::format_context &ctx) const
	{
		return std::format_to(ctx.out(), "{}.{}.{}.{}", v.major, v.minor, v.patch, v.tweak);
	}
};
