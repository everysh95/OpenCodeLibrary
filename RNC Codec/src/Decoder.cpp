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
	string key_file_name = "codekeyfile.key";
	CodeType key;
	{
		string buf;
		code_read(key_file_name, buf);
		key = buf | list_convert(buf, key);
	}
	if (key.empty())
	{
		uint64_t pass;
		cout << "pass number:" << flush;
		cin >> pass;
		key | key_genaration(key, 1 << 4, 4);
	}
	for (int i = 1; i < argc; i++)
	{
		string file_name = argv[i];
		auto pos = file_name.rfind(".cont");
		if (pos != string::npos && (file_name.size()) - pos == 5)
		{
			string buf;
			code_read(file_name, buf);
			code_write(file_name.substr(0, pos), buf | auto_decode(buf, key, 4, 2));
			remove(file_name.c_str());
		}
	}
	code_write(key_file_name, key | list_convert(key, string()));
	return 0;
}
