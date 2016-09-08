#ifndef G1DAF699655004E8283E3496E0D3ACABD
#define G1DAF699655004E8283E3496E0D3ACABD

#include"CodeCore.hpp"

namespace OpenCode
{
	//復号化関数
	//code:暗号化されたデータ
	//code_list:鍵候補の数
	//code_list:鍵候補のリスト
	template<typename CT,typename CLT>
	CT& decode(CT& code,CT list_size,CLT code_list)
	{
		CT mi = 0, mv = count_bit(code_list[0] ^ code);
		for (CT i = 1; i < list_size; i++)
		{
			CT v = count_bit(code_list[i] ^ code);
			if (mv > v)
			{
				mi = i;
				mv = v;
			}
		}
		code = mi;
		return code;
	}

	template<typename LT>
	class Decode
	{
		LT code_list;
		size_t list_size;
	public:
		Decode(LT code_list):code_list(code_list),list_size(std::size(code_list)){}
		LT& operator()(LT& target)
		{
			LT::iterator e = std::end(target);
			for (LT::iterator p = std::begin(target); p != e; p++)
			{
				LT::value_type  vp = *p;
				decode(vp, (decltype(vp))list_size,code_list);
				*p = vp;
			}
			return target;
		}
	};


	template<typename LT>
	constexpr Decode<LT> decode(LT list)
	{
		return Decode<LT>(list);
	}

}

#endif