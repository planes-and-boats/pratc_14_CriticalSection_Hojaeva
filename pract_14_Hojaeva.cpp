#include "Log.h"
#include <string>
#include <conio.h>

int main()
{
	setlocale(NULL, "Russian");
	srand(time(NULL));

#ifdef CRITICAL_SECTION_MODE

	InitializeCriticalSection(&cs);

#endif

	LogBuffer buffer{ 0 };
	HANDLE hThread[4];
	DWORD IDThread[4];

	cout << endl << endl << "--- ^-^ // Добро пожаловать в программу учета событий // ^-^ ---" << endl << endl << endl;

	cout << " --- // Нажмите на любую клавишу для простмотра журнала событий...";
	_getch();
	cout << endl;

	system("cls");

	cout << endl << " --- ^-^ // ЖУРНАЛ СОБЫТИЙ // ^-^ --- " << endl << endl;

	try {

		if (!SetProcessAffinityMask(GetCurrentProcess(), 1)) {
			throw runtime_error("не удалось задать маску сходства для процесса, код - " + to_string(GetLastError() + '.'));
		}

		hThread[0] = CreateThread(NULL, 0, Log, &buffer, 0, &IDThread[0]);
		if (hThread[0] == NULL)
			throw runtime_error("не удалось создать поток, код - " + to_string(GetLastError() + '.'));
		SetThreadPriority(hThread[0], THREAD_PRIORITY_HIGHEST);

		hThread[1] = CreateThread(NULL, 0, Log, &buffer, 0, &IDThread[1]);
		if (hThread[1] == NULL)
			throw runtime_error("не удалось создать поток, код - " + to_string(GetLastError() + '.'));
		SetThreadPriority(hThread[1], THREAD_PRIORITY_NORMAL);

		hThread[2] = CreateThread(NULL, 0, Log, &buffer, 0, &IDThread[2]);
		if (hThread[2] == NULL)
			throw runtime_error("не удалось создать поток, код - " + to_string(GetLastError() + '.'));
		SetThreadPriority(hThread[2], THREAD_PRIORITY_BELOW_NORMAL);

		hThread[3] = CreateThread(NULL, 0, Monitor, &buffer, 0, &IDThread[3]); 
		if (hThread[3] == NULL)
			throw runtime_error("не удалось создать поток, код - " + to_string(GetLastError() + '.'));
		SetThreadPriority(hThread[3], THREAD_PRIORITY_LOWEST);



		WaitForMultipleObjects(4, hThread, TRUE, INFINITE);
		
		for (int i = 0; i < 4; i++) {
			CloseHandle(hThread[i]);
		}
	}
	catch (exception& e) {
		cout << endl << "Ошибка: " << e.what() << endl << endl;
	}

#ifdef CRITICAL_SECTION_MODE

	DeleteCriticalSection(&cs);

#endif // CRITICAL_SECTION_MODE

	cout << endl << " --- ^-^ // КОНЕЦ // ^-^ --- " << endl << endl;

	cout << endl << "--- // Количество логов: " << buffer.index << endl;

	cout << endl << endl << "--- ^-^ // КОНЕЦ ПРОГРАММЫ // ^-^ ---" << endl << endl;


	cout << " --- // Нажмите на любую клавишу для завершения...";
	_getch();
	cout << endl;

	return 0;
}
