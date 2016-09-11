#include<vector>
#include<string>
#include<iostream>
#include<fstream>
#include<cstdio>
#include<cstdlib>
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
	if (string(argv[1]) == "-pass")
	{
		uint64_t pass;
		cout << "pass number:" << flush;
		cin >> pass;
		key | key_genaration(key, 1 << 4, 4);
	}
	for (int i = 1; i < argc; i++)
	{
		string file_name = argv[i];
		if (key_file_name != file_name)
		{
			string buf;
			code_read(file_name, buf);
			code_write(file_name + ".cont", buf | auto_encode(buf, key, 4, 4));
			remove(file_name.c_str());
		}
	}
	code_write(key_file_name, key | list_convert(key, string()));
	return 0;
}