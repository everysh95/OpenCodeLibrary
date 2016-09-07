#ifndef G8550A35D9510460691B290311CFBBE06
#define G8550A35D9510460691B290311CFBBE06

#include"Decode.h"
#include"Encode.h"
#include<random>
#include<functional>
#include<vector>
#include<algorithm>
#include<cstdint>

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
			for (size_t i = 0; i < code_size * distans; i++)
				buffer.push_back(i);
			LT k;
			std::random_device rd;
			for (size_t i = 0; i < code_size; i++)
			{
				LT::value_type l = 0;
				for (int j = 0; j < distans; j++)
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

	template<typename CLT,typename VLT>
	class ListConvert
	{
		size_t ct_size;
		size_t vt_size;
	public:
		ListConvert():ct_size(sizeof(CLT::value_type) * 8),vt_size(sizeof(VLT::value_type) * 8){}
		ListConvert(size_t code_type_size, size_t value_type_size) :ct_size(code_type_size), vt_size(value_type_size) {}
		VLT operator()(CLT& target)
		{
			VLT ans;
			if (ct_size < vt_size)
			{
				size_t conv = vt_size / ct_size;
				auto e = std::end(target);
				for (auto p = std::begin(target); p != e;)
				{
					VLT::value_type b = 0;
					for (size_t i = 0; i < conv && p != e; i++, p++)
					{
						b <<= ct_size;
						b += static_cast<VLT::value_type>(*p) % (1 << ct_size);
					}
					ans.push_back(b);
				}
			}
			else if (ct_size > vt_size)
			{
				size_t conv = ct_size / vt_size;
				auto e = std::end(target);
				for (auto p = std::begin(target); p != e; p++)
				{
					CLT::value_type b = *p;
					for (int i = conv - 1; i >= 0; i--)
					{
						ans.push_back((b >> (vt_size * i)) % (1 << vt_size));
					}
				}
			}
			else
			{
				auto e = std::end(target);
				for (auto p = std::begin(target); p != e; p++)
					ans.push_back(static_cast<VLT::value_type>(*p));
			}
			return ans;
		}
	};

	template<typename CLT,typename VLT>
	constexpr ListConvert<CLT,VLT> list_convert(CLT before,VLT after)
	{
		return ListConvert<CLT,VLT>();
	}
	template<typename CLT,typename VLT>
	constexpr ListConvert<CLT,VLT> list_convert_from(CLT before,VLT after,size_t before_size)
	{
		return ListConvert<CLT, VLT>(before_size, sizeof(VLT::value_type) * 8);
	}

	template<typename CLT,typename VLT>
	constexpr ListConvert<CLT,VLT> list_convert_to(CLT before,VLT after,size_t after_size)
	{
		return ListConvert<CLT, VLT>(sizeof(CLT::value_type) * 8, after_size);
	}
}

#endif
