#include<vector>
#include<iostream>
#include<cmath>
#include<cstdlib>
#include<ctime>
#include"OpenCode/EasyCoding.h"

using CodeType = unsigned long long;

void print_vec(std::vector<CodeType>);

int main()
{
	std::vector<CodeType> t;
	for (int i = 0; i < 0x100; i++)
	//for (int i = 0; i < 0x10; i++)
		t.push_back(i % 0x10);

	std::vector<CodeType> k;
	k | OpenCode::key_genaration(k, 16, 3);

	std::vector<CodeType> k2 = k;
	k2 | OpenCode::noise_generation(k2, 16, 1);

	print_vec(t);
	t | OpenCode::encode(k2, 16, 1);
	print_vec(t);
	t | OpenCode::decode(k);
	print_vec(t);

	return 0;
}

void print_vec(std::vector<CodeType> t)
{
	std::cout << std::hex;
	for(auto&& v : t)
		std::cout << v << " ";
	std::cout << std::dec << std::endl;
}