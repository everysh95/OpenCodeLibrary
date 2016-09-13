#ifndef GF6C0D21313924D53BEEA840E454099AA
#define GF6C0D21313924D53BEEA840E454099AA

#include"CodeCore.hpp"
#include"ListConvert.hpp"
#include<random>
#include<vector>
#include<algorithm>


namespace OpenCode
{
	template<typename LT>
	class KeyGenerator
	{
		size_t code_size;
		size_t distans;
	public:
		KeyGenerator(size_t code_size,size_t distans):code_size(code_size),distans(distans){}
		LT& operator()(LT& target)
		{
			target = LT();
			std::vector<size_t> buffer;
			for (size_t i = 0; i < code_size * distans; i++)
				buffer.push_back(i);
			std::random_device rd;
			for (size_t i = 0; i < code_size; i++)
			{
				typename LT::value_type l = 0;
				for (int j = 0; j < distans; j++)
				{
					std::uniform_int_distribution<size_t> uni(0, std::end(buffer) - std::begin(buffer) - 1);
					size_t bi = uni(rd);
					size_t ri = buffer[bi];
					buffer.erase(std::begin(buffer) + bi);
					typename LT::value_type raw = 1ULL << ri;
					if (count_bit(raw) > 1)
						raw %= 1ULL << (ri + 1);
					l |= raw;
				}
				target.push_back(l);
			}
			return target;
		}
	};

	template<typename LT,typename PT,typename NG = std::mt19937>
	class KeyGeneratorOnPass
	{
		size_t code_size;
		size_t distans;
		PT pass;
	public:
		KeyGeneratorOnPass(size_t code_size,size_t distans,PT pass):code_size(code_size),distans(distans),pass(pass){}
		LT& operator()(LT& target)
		{
			target = LT();
			std::vector<size_t> buffer;
			for (size_t i = 0; i < code_size * distans; i++)
				buffer.push_back(i);
			LT k;
			NG rd(pass);
			for (size_t i = 0; i < code_size; i++)
			{
				typename LT::value_type l = 0;
				for (int j = 0; j < distans; j++)
				{
					std::uniform_int_distribution<size_t> uni(0, std::end(buffer) - std::begin(buffer) - 1);
					size_t bi = uni(rd);
					size_t ri = buffer[bi];
					buffer.erase(std::begin(buffer) + bi);
					typename LT::value_type raw = 1ULL << ri;
					if (count_bit(raw) > 1)
						raw %= 1ULL << (ri + 1);
					l |= raw;
				}
				target.push_back(l);
			}
			return target;
		}
	};

	template<typename LT>
	constexpr KeyGenerator<LT> key_genaration(LT& list, size_t code_size, size_t distans)
	{
		return KeyGenerator<LT>(code_size, distans);
	}

	template<typename LT,typename PT,typename NG = std::mt19937>
	constexpr KeyGeneratorOnPass<LT,PT,NG> key_genaration(LT& list, size_t code_size, size_t distans,PT pass)
	{
		return KeyGeneratorOnPass<LT, PT, NG>(code_size, distans, pass);
	}

}

#endif