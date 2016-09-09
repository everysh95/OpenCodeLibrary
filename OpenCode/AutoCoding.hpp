#ifndef G8550A35D9510460691B290311CFBBE06
#define G8550A35D9510460691B290311CFBBE06

#include"CodeCore.hpp"
#include"Decode.hpp"
#include"Encode.hpp"
#include"ListConvert.hpp"
#include"NoiseGenerator.hpp"
#include"KeyGenerator.hpp"

namespace OpenCode
{

	template<typename CLT,typename VLT>
	class AutoDecode
	{
		CLT& str;
		VLT& key;
		size_t bit_size;
	public:
		AutoDecode(CLT& str, VLT& key, size_t bit_size) :str(str), key(key), bit_size(bit_size)
		{}
		CLT operator()(VLT& target)
		{
			return str = target | decode(key) | list_convert_from(target, str, bit_size);
		}
		CLT operator()(CLT& target)
		{
			return str = target = target | list_convert(target, key) | (*this);
		}
	};

	template<typename CLT,typename VLT>
	constexpr AutoDecode<CLT, VLT> auto_decode(CLT& list, VLT& key,size_t bit_size)
	{
		return AutoDecode<CLT, VLT>(list,key, bit_size);
	}

	template<typename CLT,typename VLT,typename PT = unsigned long long,typename NG = std::mt19937>
	class AutoEncode
	{
		CLT& str;
		VLT& key;
		size_t bit_size;
		size_t distance;
	public:
		AutoEncode(CLT& str, VLT& key, size_t bit_size, size_t distance) :str(str), key(key), bit_size(bit_size), distance(distance)
		{
			if (key.empty())key = key | key_genaration(key, 1 << bit_size, distance);
		}
		AutoEncode(CLT& str, VLT& key, size_t bit_size, size_t distance, PT pass) :str(str), key(key), bit_size(bit_size), distance(distance)
		{
			if (key.empty())key = key | key_genaration<VLT, PT, NG>(key, 1 << bit_size, distance, pass);
		}
		CLT operator()(VLT& target)
		{
			target = str | list_convert_to(str, target, bit_size);
			VLT buffer;
			do
			{
				buffer = target;
				buffer | encode(key, 1 << bit_size, distance >> 1);
			} while ((VLT(buffer) | auto_decode(CLT(), key, bit_size)) != str);
			;
			str = (target = buffer) | list_convert(buffer, str);
			return str;
		}
		CLT operator()(CLT& target)
		{
			return str = target = str | list_convert_to(str, key, bit_size) | (*this);
		}
	};

	template<typename CLT,typename VLT>
	constexpr AutoEncode<CLT, VLT> auto_encode(CLT& list, VLT& key,size_t bit_size,size_t distance)
	{
		return AutoEncode<CLT, VLT>(list, key, bit_size, distance);
	}
	template<typename CLT,typename VLT,typename PT = unsigned,typename NG = std::mt19937>
	constexpr AutoEncode<CLT, VLT,PT,NG> auto_encode(CLT& list, VLT& key,size_t bit_size,size_t distance,PT pass)
	{
		return AutoEncode<CLT, VLT, PT, NG>(list, key, bit_size, distance, pass);
	}

}

#endif
