#pragma once

#include "addon/utils.hpp"

#include <list>
#include <unordered_map>
#include <utility>

template <typename F, typename = signature_t<F>>
struct memoize;

template <typename F, typename R, typename... Args>
struct memoize<F, R(Args...)>
{
private:
	using key_type = std::tuple<std::decay_t<Args>...>;

	F _func;
	std::size_t _capacity;
	// Pointers into `std::unordered_map` are not invalidated on resize.
	std::list<const key_type *> _lru;

	// Makes `std::map::try_emplace` invoke the function when constructing values.
	struct cached
	{
		R value;
		decltype(_lru)::iterator it;

		cached(F &func, Args... args) :
			value{ std::invoke(func, std::forward<Args>(args)...) }
		{
		}
	};

	std::unordered_map<key_type, cached> _cache;

public:
	memoize(std::size_t capacity, F func) :
		_func{ std::move(func) }, _capacity{ capacity }
	{
	}

	const R &operator()(Args... args)
	{
		auto [it, created] = _cache.try_emplace(std::tuple(args...), _func, std::forward<Args>(args)...);
		if (created)
		{
			if (_cache.size() > _capacity) {
				_cache.erase(*_lru.back());
				_lru.pop_back();
			}

			_lru.emplace_front(&it->first);
			it->second.it = _lru.begin();
		}
		else
		{
			// Move accessed key to front.
			_lru.splice(_lru.begin(), _lru, it->second.it);
		}

		return it->second.value;
	}
};

template <typename F>
memoize(std::size_t, F) -> memoize<F>;

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
