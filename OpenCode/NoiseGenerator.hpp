#ifndef GECE27CD717FB4E56BBDA50CDA873DD69
#define GECE27CD717FB4E56BBDA50CDA873DD69

#include"CodeCore.hpp"
#include"Encode.hpp"
#include<random>

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

}

#endif
