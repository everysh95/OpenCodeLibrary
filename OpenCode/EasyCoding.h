#ifndef G8550A35D9510460691B290311CFBBE06
#define G8550A35D9510460691B290311CFBBE06

#include"Decode.h"
#include"Encode.h"
#include<random>
#include<functional>
#include<vector>
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
			unsigned int ni = uni(rd);
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
	constexpr auto encode(LT list,size_t code_size,size_t disttans)
	{
		return Encode<LT, NoiseGenerator<LT::value_type,LT> >(list, NoiseGenerator<LT::value_type,LT>(code_size, disttans));
	}

	template<typename LT>
	constexpr auto noise_generation(LT list, size_t code_size, size_t distans)
	{
		return NoiseGenerator<LT::value_type, LT>(code_size, distans);
	}

	template<typename LT>
	class KeyGenerator
	{
		size_t code_size;
		size_t distans;
	public:
		KeyGenerator(size_t code_size,size_t distans):code_size(code_size),distans(distans){}
		LT& operator()(LT& target)
		{
			std::vector<size_t> buffer;
			for (int i = 0; i < code_size * distans; i++)
				buffer.push_back(i);
			LT k;
			std::random_device rd;
			for (int i = 0; i < code_size; i++)
			{
				LT::value_type l = 0;
				for (int j = 0; j < 3; j++)
				{
					std::uniform_int_distribution<unsigned int> uni(0, std::size(buffer) - 1);
					size_t bi = uni(rd);
					int ri = buffer[bi];
					buffer.erase(std::begin(buffer) + bi);
					LT::value_type raw = 1 << ri;
					if (count_bit(raw) > 1)
						raw %= raw << 1;
					l |= raw;
				}
				target.push_back(l);
			}
			return target;
		}
	};

	template<typename LT>
	constexpr KeyGenerator<LT> key_genaration(LT list, size_t code_size, size_t distans)
	{
		return KeyGenerator<LT>(code_size, distans);
	}
}

#endif
