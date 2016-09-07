#include<vector>
#include<string>
#include<iostream>
#include<cmath>
#include<cstdlib>
#include<ctime>
#include"../../OpenCode/EasyCoding.h"

using CodeType = unsigned long long;

template<typename T>
void print_vec(std::vector<T> t)
{
	std::cout << std::hex;
	for(auto&& v : t)
		std::cout << static_cast<unsigned long long int>(v) << " ";
	std::cout << std::dec << std::endl;
}

int main()
{
	std::string str;
	std::cin >> str;

	std::vector<uint8_t> v8 = str | OpenCode::list_convert(str, v8);

	std::vector<CodeType> t, t2;

	t = v8 | OpenCode::list_convert_to(v8, t, 4);

	std::vector<CodeType> k;
	k | OpenCode::key_genaration(k, 1 << 4, 4);

	std::vector<CodeType> k2 = k;
	k2 | OpenCode::noise_generation(k2, 1 << 4, 1);

	t | OpenCode::encode(k2, 1 << 4, 1);

	v8 = t | OpenCode::list_convert_from(t, v8, 4);
	str = v8 | OpenCode::list_convert(v8, str);
	std::cout << "encode:" << str << std::endl;

	t | OpenCode::decode(k);

	v8 = t | OpenCode::list_convert_from(t, v8, 4);
	str = v8 | OpenCode::list_convert(v8, str);
	std::cout << "decode:" << str << std::endl;

	return 0;
}

