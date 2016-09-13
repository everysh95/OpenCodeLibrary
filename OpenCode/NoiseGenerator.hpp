#ifndef GECE27CD717FB4E56BBDA50CDA873DD69
#define GECE27CD717FB4E56BBDA50CDA873DD69

#include"CodeCore.hpp"
#include"Encode.hpp"
#include<random>
#include<algorithm>

namespace OpenCode
{

	template<typename T,typename LT>
	class NoiseGenerator
	{
		std::random_device rd;
		std::uniform_int_distribution<T> uni;
		size_t distans;
	public:
		NoiseGenerator(size_t code_size,size_t distans):rd(),uni(0,code_size * distans - 1),distans(distans){}
		NoiseGenerator(NoiseGenerator<T,LT>& ref_s):rd(),uni(ref_s.uni),distans(ref_s.distans){}
		T operator()()
		{
			size_t ni = uni(rd);
			T noise = 0;
			for (size_t i = 0; i < distans; i++)
			{
				T raw_noise = 1 << ni;
				if (count_bit(raw_noise) > 1)
				{
					raw_noise %= raw_noise << 1;
				}
				noise |= raw_noise;
			}
			return noise;
		}
		LT& operator()(LT& target)
		{
			LT::iterator e = std::end(target);
			for (LT::iterator p = std::begin(target); p != e; p++)
			{
				LT::value_type vp = *p;
				*p = vp ^ (*this)();
			}
			return target;
		}

	};

	template<typename LT>
	constexpr auto encode(LT& list,size_t code_size,size_t disttans)
	{
		return Encode<LT, NoiseGenerator<LT::value_type,LT> >(list, NoiseGenerator<LT::value_type,LT>(code_size, disttans));
	}

	template<typename LT>
	constexpr auto noise_generation(LT& list, size_t code_size, size_t distans)
	{
		return NoiseGenerator<LT::value_type, LT>(code_size, distans);
	}

	template<typename T,typename LT>
	class ListNoiseAdder
	{
		std::random_device rd;
		std::uniform_int<T> value_uniform;
	public:
		ListNoiseAdder(size_t code_size, size_t decoy_size) :value_uniform(code_size - decoy_size,code_size - 1) {}
		ListNoiseAdder(ListNoiseAdder<T,LT>& that) :value_uniform(that.value_uniform) {}
		LT& operator()(LT& list)
		{
			uniform_int<long long> index_uniform(0, static_cast<long long>(std::size(list)));
			for (long long i = index_uniform(rd); i >= 0; i--)
				list.insert(std::begin(list) + index_uniform(rd), value_uniform(rd));
			return list;
		}
	};

	template<typename LT>
	constexpr auto list_noise_add(LT& list, size_t bit_size, size_t decoy_size)
	{
		return ListNoiseAdder<LT::value_type, LT>(bit_size, decoy_size);
	}

	template<typename T,typename LT>
	class ListNoiseRemover
	{
		size_t code_size;
		size_t decoy_size;
	public:
		ListNoiseRemover(size_t code_size, size_t decoy_size) :code_size(code_size),decoy_size(decoy_size) {}
		LT& operator()(LT& list)
		{
			for (size_t i = code_size - decoy_size; i < code_size; i++)
			{
				auto np = std::remove(std::begin(list), std::end(list), i);
				list.erase(np, std::end(list));
			}
			return list;
		}
	};

	template<typename LT>
	constexpr auto list_noise_remove(LT& list, size_t bit_size, size_t decoy_size)
	{
		return ListNoiseRemover<LT::value_type, LT>(bit_size, decoy_size);
	}
}

#endif
