#include <iostream>
#include "sha256.h"
using namespace std;

int main()
{
	string input = "grape";
	string output1 = sha256(input);

	cout << "sha256('" << input << "'):" << output1 << endl;
	return 0;
}
