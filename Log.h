#pragma once
#include <Windows.h>
#include <iostream>

using namespace std;

//#define CRITICAL_SECTION_MODE

#define MAX_LOGS 1000
#define MAX_TEXT 128

struct LogRecord {
	DWORD threadId;
	int priority;
	DWORD tick;
	char message[MAX_TEXT];
};

struct LogBuffer {
	LogRecord records[MAX_LOGS];
	LONG index;
};

#ifdef CRITICAL_SECTION_MODE

CRITICAL_SECTION cs;

#endif // CRITICAL_SECTION_MODE

const char* GetThreadPriority(DWORD priority) {
	switch (priority) {
	case THREAD_PRIORITY_NORMAL:
		return "_NORMAL      ";
	case THREAD_PRIORITY_BELOW_NORMAL:
		return "_BELOW_NORMAL";
	case THREAD_PRIORITY_HIGHEST:
		return "_HIGHEST     ";
	case THREAD_PRIORITY_LOWEST:
		return "_LOWEST      ";
	default:
		return "_NOT_FOUND   ";
	}
}

DWORD WINAPI Log(LPVOID lpParam) {

	LogBuffer* buffer = (LogBuffer*)lpParam;

	while (buffer->index < MAX_LOGS) {

		if (buffer->index >= MAX_LOGS) {
			break;
		}

#ifdef CRITICAL_SECTION_MODE

		EnterCriticalSection(&cs);

#endif // CRITICAL_SECTION_MODE

		LogRecord newLog{};

		HANDLE currThread = GetCurrentThread();

		newLog.priority = GetThreadPriority(currThread);
		newLog.tick = GetTickCount();
		newLog.threadId = GetCurrentThreadId();
		sprintf_s(newLog.message, MAX_TEXT, "Log message from %u", newLog.threadId); 

		buffer->records[buffer->index] = newLog;
		buffer->index++;

#ifdef CRITICAL_SECTION_MODE

		LeaveCriticalSection(&cs);

#endif // CRITICAL_SECTION_MODE
		srand(newLog.threadId);
		Sleep(rand() % 90 + 11);
	}

	return 0;
}

DWORD WINAPI Monitor(LPVOID lpParam) {

	LogBuffer* buffer = (LogBuffer*)lpParam;

	while (buffer->index <= MAX_LOGS - 1) {

#ifdef CRITICAL_SECTION_MODE

		EnterCriticalSection(&cs);

#endif // CRITICAL_SECTION_MODE

		int start = buffer->index - 5;
		if (start < 0) start = 0;

	cout << " --- // Последние 5 логов" << endl << endl;
		for (int i = start; i < buffer->index; i++) {
			cout << "  #" << i + 1 << "\tThread: " << buffer->records[i].threadId;
			cout << " | Prio: " << GetThreadPriority(buffer->records[i].priority) << " | Tick: " << buffer->records[i].tick;
			cout << " | " << buffer->records[i].message << endl;
		}
	cout << endl;

#ifdef CRITICAL_SECTION_MODE

		LeaveCriticalSection(&cs);

#endif // CRITICAL_SECTION_MODE


		Sleep(100);
	}

	return 0;
}
