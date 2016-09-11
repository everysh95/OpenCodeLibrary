#ifndef GF0B3039598B84CDDA20A579ECB483DA1
#define GF0B3039598B84CDDA20A579ECB483DA1

#include<iterator>

namespace OpenCode
{
	
	template<typename LT,typename F>
	auto operator|(LT& target,F& effecter) -> decltype(effecter(target))
	{
		return effecter(target);
	}

	template<typename CT>
	constexpr CT count_bit(CT target)
	{
		return (target > 0) ? target % 2 + count_bit(target >> 1) : 0;
	}

}

#endif