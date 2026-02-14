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
	F _func;
	std::size_t _capacity;
	std::list<std::size_t> _lru;

	// Makes `std::map::try_emplace` invoke the function when constructing values.
	struct cached
	{
		R value;
		decltype(_lru)::iterator it;

		cached(F &func, std::tuple<Args&&...> args) :
			value{ std::apply(func, std::move(args)) }
		{
		}
	};

	std::unordered_map<std::size_t, cached, std::identity> _cache;

public:
	memoize(std::size_t capacity, F func) :
		_func{ std::move(func) }, _capacity{ capacity }
	{
	}

	const R &operator()(Args... args)
	{
		auto args_pack = std::forward_as_tuple(std::forward<Args>(args)...);
		std::size_t hash = std::hash<decltype(args_pack)>{}(args_pack);

		auto [it, created] = _cache.try_emplace(hash, _func, std::move(args_pack));
		if (created)
		{
			if (_cache.size() > _capacity) {
				_cache.erase(_lru.back());
				_lru.pop_back();
			}

			_lru.push_front(hash);
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
