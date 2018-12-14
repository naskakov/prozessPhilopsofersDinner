// Mutex.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include<windows.h>
#include <stdio.h>
#include <stdlib.h>
#include<iostream>
#include<time.h>
#include <atlconv.h>
#include <fstream>

using namespace std;

char timebuf[80];
SYSTEMTIME st;

#define N 2 

struct Philosopher      
{
	int number;
};


HANDLE forkMutex[N];    
HANDLE mainMutex;

void addlogsph(const char* data, int i) {
	WaitForSingleObject(mainMutex, INFINITE);
	GetLocalTime(&st);
	sprintf_s(timebuf, "%d.%d.%d %d:%d:%d:%d", st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	ofstream logfile("logfile.txt", ofstream::app);
	if (data == "Error") {
		logfile << timebuf << " " << data << endl;
	}
	else { logfile << timebuf << " " << "Pholosopher " << i << " " << data << endl; }
	logfile.close();
	ReleaseMutex(mainMutex);
}

void think(int i)      
{
	string buf;
	buf = "thinking";
	addlogsph(buf.c_str(), i);
}

void take_forks(int i)  
{
	DWORD dw = WaitForSingleObject(forkMutex[0], 100);
	string buf;
	switch (dw) {
	case WAIT_OBJECT_0: {
		buf = "take left fork";
		addlogsph(buf.c_str(), i);
		buf = "wait right fork";
		addlogsph(buf.c_str(), i);
		DWORD dw1 = WaitForSingleObject(forkMutex[1], 100);
		switch (dw1) {
		case WAIT_OBJECT_0:
			buf = "is eating";
			addlogsph(buf.c_str(), i);
			Sleep(1000);
			ReleaseMutex(forkMutex[1]);
			ReleaseMutex(forkMutex[0]);
			break;
		case WAIT_TIMEOUT:
			ReleaseMutex(forkMutex[0]);
			break;
		}
		break; }
	case WAIT_TIMEOUT:
		break;
	case WAIT_FAILED:
		addlogsph("Error", i);
		break;
	}
}


int main(int argc, char *argv[])
{
	USES_CONVERSION;
	Philosopher phil;
	phil.number = atoi(argv[1]);

	forkMutex[0] = OpenMutex(MUTEX_ALL_ACCESS, FALSE, A2W(argv[2]));
	forkMutex[1] = OpenMutex(MUTEX_ALL_ACCESS, FALSE, A2W(argv[3]));
	mainMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, A2W(argv[4]));


		while (1)    
		{  
			think(phil.number); 
			take_forks(phil.number);   
		}
		
}
