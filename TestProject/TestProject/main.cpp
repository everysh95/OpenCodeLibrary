#include<vector>
#include<string>
#include<iostream>
#include"../../OpenCode/EasyCoding.hpp"

using CodeType = std::vector<unsigned long long>;
using namespace std;
using namespace OpenCode;

int main()
{
	CodeType k, t;
	string str,rstr;
	random_device rd;
	unsigned long long pass = rd();

	cout << "test string:" << flush;
	cin >> str;
	cout << "----start defult test----" << endl;
	cout << "strint:" << str << endl;
	cout << "encode:" << (str | auto_encode(str, k, 4, 4)) << endl;
	cout << "decode:" << (str | auto_decode(str, k, 4)) << endl;

	k = CodeType();
	cout << "----start pass test-----" << endl;
	cout << "string:" << str << endl;
	cout << "password:" << pass << endl;
	cout << "encode:" << (str | auto_encode(str, k, 4, 4, pass)) << endl;
	rstr = str;
	cout << "decode:" << (str | auto_decode(str, k, 4)) << endl;

	cout << "----reflush key codes----" << endl;
	k | key_genaration(k, 1 << 4, 4, pass);
	cout << "password:" << pass << endl;
	cout << "decode:" << (rstr | auto_decode(rstr, k, 4)) << endl;
	cout << "-------------------------" << endl;
	return 0;
}

