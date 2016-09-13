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
			typename LT::iterator e = std::end(target);
			for (typename LT::iterator p = std::begin(target); p != e; p++)
			{
				typename LT::value_type vp = *p;
				*p = vp ^ (*this)();
			}
			return target;
		}

	};

	template<typename LT>
	auto noise_generation(LT& list, size_t code_size, size_t distans)
	{
		NoiseGenerator<typename LT::value_type, LT> ans(code_size, distans);
		return ans;
	}

	template<typename LT>
	auto encode(LT& list,size_t code_size,size_t distans)
	{
		NoiseGenerator<typename LT::value_type, LT> ng(code_size, distans);
		Encode<LT, decltype(ng) > ans(list, ng);
		return ans;
	}

	template<typename T,typename LT>
	class ListNoiseAdder
	{
		std::random_device rd;
		std::uniform_int_distribution<T> value_uniform;
	public:
		ListNoiseAdder(size_t code_size, size_t text_size) :value_uniform(text_size, code_size - 1) {}
		ListNoiseAdder(ListNoiseAdder<T,LT>& that) :value_uniform(that.value_uniform) {}
		LT& operator()(LT& list)
		{
			std::uniform_int_distribution<long long> index_uniform(0, static_cast<long long>(std::end(list) - std::begin(list)));
			for (long long i = index_uniform(rd); i >= 0; i--)
				list.insert(std::begin(list) + index_uniform(rd), value_uniform(rd));
			return list;
		}
	};

	template<typename LT>
	ListNoiseAdder<typename LT::value_type, LT> list_noise_add(LT& list, size_t bit_size, size_t decoy_size)
	{
		ListNoiseAdder<typename LT::value_type, LT> lna(bit_size, decoy_size);
		return lna;
	}

	template<typename T,typename LT>
	class ListNoiseRemover
	{
		size_t code_size;
		size_t text_size;
	public:
		ListNoiseRemover(size_t code_size, size_t text_size) :code_size(code_size),text_size(text_size) {}
		LT& operator()(LT& list)
		{
			for (size_t i = text_size; i < code_size; i++)
			{
				auto np = std::remove(std::begin(list), std::end(list), i);
				list.erase(np, std::end(list));
			}
			return list;
		}
	};

	template<typename LT>
	auto list_noise_remove(LT& list, size_t bit_size, size_t text_size)
	{
		return ListNoiseRemover<typename LT::value_type, LT>(bit_size, text_size);
	}
}

#endif
