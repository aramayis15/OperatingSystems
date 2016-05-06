#include <tchar.h>
#include <stdio.h>
#include <windows.h>

CRITICAL_SECTION critical_section;

const int ARRAY_SIZE = 30;
int globalArray[ARRAY_SIZE];


DWORD WINAPI FillWithFibonacciNumbers(void *) {

    for (int i = 0; i < ARRAY_SIZE; ++i) {

        // Critical Section
        EnterCriticalSection(&critical_section);
        if (i == 0 || i == 1) {
            globalArray[i] = i;
            LeaveCriticalSection(&critical_section);
            continue;
        }

        globalArray[i] = globalArray[i - 2] + globalArray[i - 1];
        LeaveCriticalSection(&critical_section);
    }

    return 0;
}

DWORD WINAPI PrintGlobalArray(void *) {

    // Print globalArray (In this case: Fibonacci Numbers)
    _tprintf(_T("First %d Fibonacci Numbers:\n"), ARRAY_SIZE);

    for (int i = 0; i < ARRAY_SIZE; ++i) {

        //Critical Section
        EnterCriticalSection(&critical_section);
        _tprintf(_T("%d)\t%d\n"), i + 1, globalArray[i]);
        LeaveCriticalSection(&critical_section);
    }

    return 0;
}

int main() {

    // Critical Section Initialization
    InitializeCriticalSection(&critical_section);

    // Threads Creation
    DWORD firstThreadID;  
    HANDLE firstProcessHANDLE = CreateThread(NULL, 0, FillWithFibonacciNumbers, NULL, 0, &firstThreadID);
    if (firstProcessHANDLE == INVALID_HANDLE_VALUE) {
        _tprintf(_T("Unable to create thread. Error code: %d \n"), (int) GetLastError());
        exit(1);
    }

	DWORD secondThreadID;
    HANDLE secondProcessHANDLE = CreateThread(NULL, 0, PrintGlobalArray, NULL, 0, &secondThreadID);
    if (secondProcessHANDLE == INVALID_HANDLE_VALUE) {
        _tprintf(_T("Unable to create thread. Error code: %d \n"), (int) GetLastError());
        exit(1);
    }

    // Wait for all threads to end
    HANDLE ThreadsHandlesArray[2] = {firstProcessHANDLE, secondProcessHANDLE};
    WaitForMultipleObjects(2, ThreadsHandlesArray, TRUE, INFINITE);

    // Critical section deletion
    DeleteCriticalSection(&critical_section);

    // Close all unnecessary handles
    CloseHandle(firstProcessHANDLE);
    CloseHandle(secondProcessHANDLE);

    return 0;
}