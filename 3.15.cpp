#include <tchar.h>
#include <stdio.h>
#include <windows.h>

const int N = 8;

// Event
HANDLE eventHandle;

// Current number to be printed by thread
int numberToPrint = 1;

DWORD WINAPI SetValue_1(void *) {

    while (true) {
        WaitForSingleObject(eventHandle, INFINITE);
        _tprintf(_T("%d\n"), numberToPrint++);
        ResetEvent(eventHandle);
    }
    return 0;
}

int main() {

    // Event Creation
    eventHandle = CreateEvent(NULL, FALSE, FALSE, NULL);

    // Thread Creation
    DWORD ThreadID;

    HANDLE ThreadHandle = CreateThread(NULL, 0, SetValue_1, NULL, 0, &ThreadID);
    if (ThreadHandle == INVALID_HANDLE_VALUE) {
        _tprintf(_T("Unable to create thread. Error code: %d \n"), (int) GetLastError());
        exit(1);
    }

    // Set Event every second
    for (int i = 0; i < N; ++i) {
        Sleep(1000);
        SetEvent(eventHandle);
    }

    // Close all unnecessary handles
    CloseHandle(eventHandle);
    CloseHandle(ThreadHandle);

    return 0;
}