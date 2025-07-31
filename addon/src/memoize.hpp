#pragma once

#include "addon/utils.hpp"

#include <unordered_map>
#include <utility>

template <typename F, typename = signature_t<F>>
struct memoize;

template <typename F, typename R, typename... Args>
struct memoize<F, R(Args...)>
{
private:
	F _func;

	// Makes `std::map::try_emplace` invoke the function when constructing values.
	struct cached
	{
		R value;

		cached(F &func, std::tuple<Args&&...> args) :
			value{ std::apply(func, std::move(args)) }
		{
		}
	};

	std::unordered_map<std::size_t, cached, std::identity> _cache;

public:
	memoize(F func) :
		_func{ std::move(func) }
	{
	}

	const R &operator()(Args... args)
	{
		auto args_pack = std::forward_as_tuple(std::forward<Args>(args)...);
		std::size_t hash = std::hash<decltype(args_pack)>{}(args_pack);
		auto [it, _] = _cache.try_emplace(hash, _func, std::move(args_pack));
		return it->second.value;
	}
};

template <typename F>
memoize(F) -> memoize<F>;

// Based on: https://stackoverflow.com/questions/7110301/generic-hash-for-tuples-in-unordered-map-unordered-set
template <typename... Types>
struct std::hash<std::tuple<Types...>>
{
	struct hasher
	{
		const std::size_t hash = 0;

		template <typename T>
		hasher operator^(const T &object)
		{
			std::size_t seed = hash;
			seed ^= std::hash<T>()(object) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			return { seed };
		}

		operator std::size_t() const { return hash; }
	};

	std::size_t operator()(const std::tuple<Types...> &tuple) const
	{
		return std::apply([](const Types&... items) { return (hasher{} ^ ... ^ items); }, tuple);
	}
};
