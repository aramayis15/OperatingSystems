#include <tchar.h>
#include <stdio.h>
#include <windows.h>

int const ARRAY_SIZE = 30;

HANDLE eventHandle;

int globalArray[ARRAY_SIZE];


DWORD WINAPI FillWithFibonacciNumbers(void *) {

    //First 2 Fibonacci numbers
    globalArray[0] = 0;
    globalArray[1] = 1;

    for (int i = 2; i < ARRAY_SIZE; ++i) {
        globalArray[i] = globalArray[i - 2] + globalArray[i - 1];
    }

    // Let second and third threads work
    SetEvent(eventHandle);

    return 0;
}

DWORD WINAPI PrintGlobalArray(void *arg) {

    int *name = static_cast<int *>(arg);

    //Wait for 1st thread to fill the globalArray with Fibonacci numbers
    WaitForSingleObject(eventHandle, INFINITE);

    // Print globalArray (In this case: Fibonacci Numbers)
    _tprintf(_T("First %d Fibonacci Numbers (by Thread_%d):\n"), ARRAY_SIZE, (*name));

    for (int i = 0; i < ARRAY_SIZE; ++i) {
        _tprintf(_T("%d)by Thread_%d\t%d\n"), i + 1, (*name), globalArray[i]);
    }

    return 0;
}

int main() {

    // Event Creation
    eventHandle = CreateEvent(NULL, TRUE, FALSE, NULL);

    // Parameters for 2nd and 3rd threads (for printing)
    int secondThreadArg = 2;
    int thirdThreadArg = 3;


    // Threads Creation
    DWORD firstThreadID;
    HANDLE firstThreadHandle = CreateThread(NULL, 0, FillWithFibonacciNumbers, NULL, 0, &firstThreadID);
    if (firstThreadHandle == INVALID_HANDLE_VALUE) {
        _tprintf(_T("Unable to create thread. Error code: %d \n"), (int) GetLastError());
        exit(1);
    }

    DWORD secondThreadID;
    HANDLE secondThreadHandle = CreateThread(NULL, 0, PrintGlobalArray, &secondThreadArg, 0, &secondThreadID);
    if (secondThreadHandle == INVALID_HANDLE_VALUE) {
        _tprintf(_T("Unable to create thread. Error code: %d \n"), (int) GetLastError());
        exit(1);
    }

    DWORD thirdThreadID;
    HANDLE thirdThreadHandle = CreateThread(NULL, 0, PrintGlobalArray, &thirdThreadArg, 0, &thirdThreadID);
    if (thirdThreadHandle == INVALID_HANDLE_VALUE) {
        _tprintf(_T("Unable to create thread. Error code: %d \n"), (int) GetLastError());
        exit(1);
    }

    // Wait for all threads to end
    HANDLE ThreadsHandlesArray[3] = {firstThreadHandle, secondThreadHandle, thirdThreadHandle};
    WaitForMultipleObjects(3, ThreadsHandlesArray, TRUE, INFINITE);

    // Close all unnecessary handles
    CloseHandle(eventHandle);
    CloseHandle(firstThreadHandle);
    CloseHandle(secondThreadHandle);
    CloseHandle(thirdThreadHandle);

    return 0;
}