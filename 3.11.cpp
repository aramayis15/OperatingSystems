#include <tchar.h>
#include <stdio.h>
#include <windows.h>

// Names are for first thread
HANDLE canFill;
HANDLE canPrint;

int globalArray[30];


DWORD WINAPI FillTheFirstInterval(void *) {

    for (int i = 0; i < 15; ++i) {
        globalArray[i] = 1;
    }

    // Let second thread work
    SetEvent(canFill);

    //Wait for 2nd thread to fill the Second half of array
    WaitForSingleObject(canPrint, INFINITE);

    for (int i = 0; i < 30; ++i) {
        _tprintf(_T("%d\n"), globalArray[i]);
    }

    return 0;
}

DWORD WINAPI FillTheSecondInterval(void *) {

    //Wait for 1st thread to fill the globalArray with Fibonacci numbers
    WaitForSingleObject(canFill, INFINITE);

    for (int i = 15; i < 30; ++i) {
        globalArray[i] = 2;
    }

    // Let first thread work
    SetEvent(canPrint);

    return 0;
}

int main() {

    // Event Creation
    canFill = CreateEvent(NULL, TRUE, FALSE, NULL);
    canPrint = CreateEvent(NULL, TRUE, FALSE, NULL);

    // Threads Creation
    DWORD firstThreadID;
    HANDLE firstProcessHandle = CreateThread(NULL, 0, FillTheFirstInterval, NULL, 0, &firstThreadID);
    if (firstProcessHandle == INVALID_HANDLE_VALUE) {
        _tprintf(_T("Unable to create thread. Error code: %d \n"), (int) GetLastError());
        exit(1);
    }

    DWORD secondThreadID;
    HANDLE secondThreadHandle = CreateThread(NULL, 0, FillTheSecondInterval, NULL, 0, &secondThreadID);
    if (secondThreadHandle == INVALID_HANDLE_VALUE) {
        _tprintf(_T("Unable to create thread. Error code: %d \n"), (int) GetLastError());
        exit(1);
    }

    // Wait for all threads to end
    HANDLE ThreadsHandlesArray[2] = {firstProcessHandle, secondThreadHandle};
    WaitForMultipleObjects(2, ThreadsHandlesArray, TRUE, INFINITE);

    // Close all unnecessary handles
    CloseHandle(canFill);
    CloseHandle(canPrint);
    CloseHandle(firstProcessHandle);
    CloseHandle(secondThreadHandle);

    return 0;
}