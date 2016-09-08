#ifndef G663BF241788548C98D5E972777FF4BD9
#define G663BF241788548C98D5E972777FF4BD9

#include"CodeCore.hpp"

namespace OpenCode
{

	//暗号化関数
	//value:暗号化したいデータ
	//noise:乱数鍵
	//code_list:鍵候補のリスト
	template<typename CT,typename CLT>
		CT& encode(CT& value,CT noise,CLT code_list)
		{
			value = code_list[value] ^ noise;
			return value;
		}

	template<typename LT,typename FT>
	class Encode
	{
		LT code_list;
		FT noise_generator;
	public:
		Encode(LT code_list,FT noise_generator):code_list(code_list),noise_generator(noise_generator){}
		Encode(Encode<LT,FT>& ref_s):code_list(ref_s.code_list),noise_generator(ref_s.noise_generator){}
		LT& operator()(LT& target)
		{
			LT::iterator e = std::end(target);
			for (LT::iterator p = std::begin(target); p != e; p++)
			{
				LT::value_type vp = *p;
				encode(vp, noise_generator() , code_list);
				*p = vp;
			}
			return target;
		}
	};


	template<typename LT,typename FT>
	constexpr Encode<LT,FT> encode(LT list,FT generator)
		{
			return Encode<LT,FT>(list,generator);
		}

}

#endif