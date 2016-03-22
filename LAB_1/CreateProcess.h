#ifdef linux //jghjmnkj
#include <stdio.h>
#include<stdlib.h>
#include<errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#endif

#ifdef _WIN32
#include <iostream>
#include <fstream>
#include "windows.h"
#endif

using namespace std;

class CreateProc
{
public:
	void Create(int number)
	{
#ifdef _WIN32

	STARTUPINFO startupInfo;
	PROCESS_INFORMATION procInfo;
	TCHAR CommandLine[] = TEXT("project");

	ZeroMemory(&startupInfo, sizeof(startupInfo));
	startupInfo.cb = sizeof(startupInfo);
	ZeroMemory(&procInfo, sizeof(procInfo));
		
			fstream fst;
			fst.open("file.txt", ios::in | ios::out);
			if (!fst.is_open())
				cout << "file is not open!";
			fst << number;
			fst.close();	

	if (CreateProcess(NULL,      // �� ������������ ��� ������ 
		CommandLine,             // ��������� ������
		NULL,                    // ���������� �������� �� �����������. 
		NULL,                    // ���������� ������ �� �����������. 
		FALSE,                   // ��������� ���������� ������������
		0,                       // ��� ������ �������� ��������
		NULL,                    // ���� ���������� ��������� ������������� ��������
		NULL,                    // ������������ ������� ������� ������������� ��������
		&startupInfo,            // ��������� �� ���������  STARTUPINFO.
		&procInfo)               // ��������� �� ��������� ���������� � ��������.
		)
		{
			cout << "Parent process"<<endl;
			WaitForSingleObject(procInfo.hProcess, INFINITE);
			DWORD exit_code;
			GetExitCodeProcess(procInfo.hProcess, &exit_code);
			cout << "number^2=" << exit_code <<endl;
		}

		CloseHandle(procInfo.hProcess);
		CloseHandle(procInfo.hThread);
#endif

#ifdef linux
		
	pid_t pid;
	int tmp;
	pid=fork()
		if(pid<0)
		{
			printf("ERROR!\n");
			exit(-1);
		}
		else if(pid == 0)
		{
			printf("DAUGHTER\n");
			number*=number;
			exit(number);
		}
		else
		{
			printf("PARENT\n");
			wait(&tmp);
			printf("RESULT:%d\n", WEXITSTATUS(tmp));
			printf("PARENT: Exit!\n");
		}
  
#endif
	}

};
