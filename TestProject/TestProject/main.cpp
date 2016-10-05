#include<vector>
#include<string>
#include<iostream>
#include<fstream>
#include<cstdio>
#include"../../OpenCode/AutoCoding.hpp"
#include"../../OpenCode/CodeIO.hpp"

using CodeType = std::vector<uint64_t>;
using namespace std;
using namespace OpenCode;

int main()
{
	CodeType key;
	string com, file_name;
	string key_file_name = "codekeyfile.key";
	random_device rd;
	unsigned long long pass = rd();
	bool pass_mode = false;

	cout << "flie name:" << flush;
	cin >> file_name;
	{
		string buf;
		code_read(key_file_name, buf);
		key = buf | list_convert(buf, key);
	}
	do
	{
		cout << "command>" << flush;
		cin >> com;
		if (com == "sb")
		{
			string buf;
			code_read(file_name, buf);
			cout << "code bytes" << endl;
			show_bytes(buf);
		}
		if (com == "sk")
		{
			string buf;
			cout << "orignal key bits" << endl;
			show_bits(key);
			cout << "flie key bits" << endl;
			code_read(key_file_name, buf);
			show_bits(buf | list_convert(buf, key));
		}
		if (com == "sc")
		{
			string buf;
			code_read(file_name, buf);
			cout << "file_contents:" << (buf | auto_decode(buf, key, 4, 2)) << endl;
		}
		if (com == "rc")
		{
			cout << "new_file_contents:" << flush;
			string buf;
			cin >> buf;
			code_write(file_name, buf | auto_encode(buf, key, 4, 2, 4));
			if(!pass_mode) code_write(key_file_name, key | list_convert(key, string()));
		}
		if (com == "rk")
			remove(key_file_name.c_str());
		if (com == "nk")
		{
			string buf;
			code_read(file_name, buf);
			buf | auto_decode(buf, key, 4, 2);
			key | key_genaration(key, 1 << 4, 4);
			code_write(file_name, buf | auto_encode(buf, key, 4, 2, 4));
			code_write(key_file_name, key | list_convert(key, string()));
			pass_mode = false;
		}
		if (com == "pk")
		{
			string buf;
			unsigned long long pass;
			cout << "pass number:" << flush;
			cin >> pass;
			if (!key.empty())
			{
				code_read(file_name, buf);
				buf | auto_decode(buf, key, 4);
			}
			key | key_genaration(key, 1 << 4, 4, pass);
			remove(key_file_name.c_str());
			if (!key.empty())code_write(file_name, buf | auto_encode(buf, key, 4, 4));
			pass_mode = true;
		}
		if (com == "op")
			cin >> file_name;
		if (com == "rm")
			remove(file_name.c_str());

	} while (com != "q");

	return 0;
}

