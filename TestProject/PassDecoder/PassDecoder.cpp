#include<vector>
#include<string>
#include<iostream>
#include<fstream>
#include<cstdio>
#include<regex>
#include"../../OpenCode/AutoCoding.hpp"
#include"../../OpenCode/CodeIO.hpp"

using CodeType = std::vector<uint64_t>;
using namespace std;
using namespace OpenCode;

int main(int argc, char** argv)
{
	CodeType key;
	uint64_t pass;
	cout << "pass number:" << flush;
	cin >> pass;
	key | key_genaration(key, 1 << 4, 4, pass);
	for (int i = 1; i < argc; i++)
	{
		string file_name = argv[i];
		auto pos = file_name.rfind(".cont");
		if (pos != string::npos && size(file_name) - pos == 5)
		{
			string buf;
			code_read(file_name, buf);
			code_write(file_name.substr(0, pos), buf | auto_decode(buf, key, 4, 2));
			remove(file_name.c_str());
		}
	}
	return 0;
}
