// process.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <windows.h>
#include <fstream>
#include <iostream>
#include <string>
#include <conio.h>
#include <cstdlib>
#include <atlconv.h>


using namespace std;

#define N 5 
HANDLE hMutex[N];
HANDLE mainMutex;

int main()
{
	USES_CONVERSION;
	hMutex[0] = CreateMutex(NULL, FALSE, A2W("Mutex0"));
	hMutex[1] = CreateMutex(NULL, FALSE, A2W("Mutex1"));
	hMutex[2] = CreateMutex(NULL, FALSE, A2W("Mutex2"));
	hMutex[3] = CreateMutex(NULL, FALSE, A2W("Mutex3"));
	hMutex[4] = CreateMutex(NULL, FALSE, A2W("Mutex4"));
	mainMutex = CreateMutex(NULL, FALSE, A2W("mainMutex"));
	char lpszCommandLine[70];
	
	STARTUPINFO si;
	SECURITY_ATTRIBUTES saProcess;
	PROCESS_INFORMATION pi[5];

	saProcess.nLength = sizeof(saProcess);
	saProcess.lpSecurityDescriptor = NULL;
	saProcess.bInheritHandle = TRUE;

	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	for (int i = 1; i < 5; i++) {   
		sprintf_s(lpszCommandLine, "Mutex.exe %d Mutex%d Mutex%d mainMutex", i, i - 1, i);
		if (!CreateProcess(NULL, A2W(lpszCommandLine), &saProcess, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi[i - 1])) {
			printf("CreateProcess finished with error code %d\n", GetLastError());
			return 0;
		}
	}
		sprintf_s(lpszCommandLine, "Mutex.exe %d Mutex%d Mutex%d mainMutex", 5, 4, 0);  
		if (!CreateProcess(NULL, A2W(lpszCommandLine), &saProcess, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi[4])) {
			printf("CreateProcess finished with error code %d\n", GetLastError());
			return 0;
		}
	
		system("pause");
	for (int i = 0; i < 5; i++) {
		CloseHandle(pi[i].hThread);
		CloseHandle(pi[i].hProcess);
		CloseHandle(hMutex[i]);
	}
	CloseHandle(mainMutex);
}
