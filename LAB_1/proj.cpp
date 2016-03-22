#ifdef _WIN32
#include <iostream>
#include <fstream>
#include <windows.h>
#include <stdio.h>
#include <conio.h>
using namespace std;
#endif

int main(int argc, TCHAR* argv[])
{
	int a;
	fstream fst;
	fst.open("file.txt", ios::in | ios::out);
	if (!fst.is_open())
		cout << "file is not open!";
	fst >> a;
	fst.close();
	cout << "Daughter process"<<endl;
	return a*a;
}