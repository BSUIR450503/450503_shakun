#include "CreateProcess.h"


int main()
{
	int number;
	cout << "Input number:" << endl;
	cin >> number;
	CreateProc proc;
    proc.Create(number);
	system("pause");
    return 0;
}