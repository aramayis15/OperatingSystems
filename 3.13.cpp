#include <tchar.h>
#include <stdio.h>
#include <windows.h>

// Events
HANDLE firstThreadCanWork;
HANDLE secondThreadCanWork;

int globalArray[300];

// Shared index for threads
int currentIndex = 0;


DWORD WINAPI SetValue_1(void *) {

    while (currentIndex < 300) {

        // Set values of items with indexes 'currentIndex' and 'currentIndex+1'
        globalArray[currentIndex++] = 1;

        // Wait for second thread to work
        SetEvent(secondThreadCanWork);
        WaitForSingleObject(firstThreadCanWork, INFINITE);
        ResetEvent(firstThreadCanWork);
    }

    SetEvent(secondThreadCanWork);

    return 0;
}

DWORD WINAPI SetValue_2(void *) {

    // Array starts with '1', so wait for 1st thread to set 2 first items
    WaitForSingleObject(secondThreadCanWork, INFINITE);

    while (currentIndex < 300) {

        // Set values of items with indexes 'currentIndex' and 'currentIndex+1'
        globalArray[currentIndex++] = 2;


        // Wait for first thread to work
        SetEvent(firstThreadCanWork);
        WaitForSingleObject(secondThreadCanWork, INFINITE);
        ResetEvent(secondThreadCanWork);
    }

    SetEvent(firstThreadCanWork);

    return 0;
}

int main() {

    // Event Creation
    firstThreadCanWork = CreateEvent(NULL, FALSE, FALSE, NULL);
    secondThreadCanWork = CreateEvent(NULL, FALSE, FALSE, NULL);

    // Threads Creation
    DWORD firstThreadID;
    HANDLE firstThreadHandle = CreateThread(NULL, 0, SetValue_1, NULL, 0, &firstThreadID);
    if (firstThreadHandle == INVALID_HANDLE_VALUE) {
        _tprintf(_T("Unable to create thread. Error code: %d \n"), (int) GetLastError());
        exit(1);
    }

    DWORD secondThreadID;
    HANDLE secondThreadHandle = CreateThread(NULL, 0, SetValue_2, NULL, 0, &secondThreadID);
    if (secondThreadHandle == INVALID_HANDLE_VALUE) {
        _tprintf(_T("Unable to create thread. Error code: %d \n"), (int) GetLastError());
        exit(1);
    }

    // Wait for all threads to end
    HANDLE ThreadsHandlesArray[2] = {firstThreadHandle, secondThreadHandle};
    WaitForMultipleObjects(2, ThreadsHandlesArray, TRUE, INFINITE);

    // Print the array
    for (int i = 0; i < 300; ++i) {
        _tprintf(_T("%d\n"), globalArray[i]);
    }

    // Close all unnecessary handles
    CloseHandle(firstThreadCanWork);
    CloseHandle(secondThreadCanWork);
    CloseHandle(firstThreadHandle);
    CloseHandle(secondThreadHandle);


    return 0;
}