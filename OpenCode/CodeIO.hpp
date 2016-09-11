#ifndef GFB75D1A724964535A939223A13CD680F
#define  GFB75D1A724964535A939223A13CD680F

#include"CodeCore.hpp"
#include"ListConvert.hpp"
#include<string>
#include<fstream>
#include<iostream>
#include<vector>
#include<cstdint>
#include<iomanip>

namespace OpenCode
{
	template<typename S,typename CLT>
	void code_write(S s, CLT& cio)
	{
		std::ofstream ofs(s, std::ios_base::binary | std::ios_base::out);
		ofs.write(std::data(cio), std::size(cio));
		ofs.close();
	}

	template<typename S,typename CLT>
	CLT code_read(S file_name, CLT& cio)
	{
		std::ifstream s(file_name, std::ios_base::binary | std::ios_base::out);
		cio = CLT();
		if (!s)return cio;
		while (true)
		{
			CLT::value_type c = 0;
			s.read(&c, sizeof(c));
			if (s.eof())break;
			cio.insert(std::end(cio), c);
		}
		s.close();
		return cio;
	}

	template<typename CLT>
	void show_bytes(CLT list)
	{
		std::vector<uint8_t> byte_list = list | list_convert(list, byte_list);
		std::cout << std::hex;
		for (auto&& b : byte_list)
		{
			std::cout << std::setw(2) << std::setfill('0') << (static_cast<unsigned int>(b) & 0xff) << " " << std::flush;
		}
		std::cout << std::dec << std::endl;
	}

	template<typename CLT>
	void show_bits(CLT list)
	{
		//std::vector<uint8_t> byte_list = list | list_convert(list, byte_list);
		for (auto&& b : list)
		{
			for (int i = sizeof(b) * 8 - 1; i >= 0; i--)
				std::cout << ((b >> i) % 2);
			std::cout << std::endl;
		}
		std::cout << std::dec << std::endl;
	}
}

#endif
