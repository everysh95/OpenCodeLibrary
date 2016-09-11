#ifndef G4CD08FCDE7774C8CAC691C00E43D1665
#define G4CD08FCDE7774C8CAC691C00E43D1665

#include"CodeCore.hpp"

namespace OpenCode
{
	template<typename CLT,typename VLT>
	class ListConvert
	{
		size_t ct_size;
		size_t vt_size;
	public:
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
						b += static_cast<VLT::value_type>(*p) & ((1ULL << ct_size) - 1);
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
						ans.push_back((b >> (vt_size * i)) & ((1ULL << vt_size) - 1));
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
	constexpr ListConvert<CLT,VLT> list_convert(CLT& before,VLT& after)
	{
		return ListConvert<CLT,VLT>(sizeof(CLT::value_type) * 8,sizeof(VLT::value_type) * 8);
	}
	template<typename CLT,typename VLT>
	constexpr ListConvert<CLT,VLT> list_convert_from(CLT& before,VLT& after,size_t before_size)
	{
		return ListConvert<CLT, VLT>(before_size, sizeof(VLT::value_type) * 8);
	}
	template<typename CLT,typename VLT>
	constexpr ListConvert<CLT,VLT> list_convert_to(CLT& before,VLT& after,size_t after_size)
	{
		return ListConvert<CLT, VLT>(sizeof(CLT::value_type) * 8, after_size);
	}

}

#endif