#include <tchar.h>
#include <windows.h>
#include <stdio.h>

DWORD WINAPI PrintEvenNumbers(void *) {
    for (int i = 0; i < 50; i += 2) {
        _tprintf(_T("%d\n"), i);
    }

    return 0;
}

DWORD WINAPI PrintOddNumbers(void *) {
    for (int i = 1; i < 50; i += 2) {
        _tprintf(_T("%d\n"), i);
    }

    return 0;
}

int main() {

    // Threads creation
    DWORD firstThreadID;
    HANDLE firsThreadHandle = CreateThread(NULL, 0, PrintEvenNumbers, NULL, NULL, &firstThreadID);
    if (firsThreadHandle == INVALID_HANDLE_VALUE) {
        _tprintf(_T("Unable to create thread. Error code: %d\n"), GetLastError());
        exit(1);
    }

	DWORD secondThreadID;
    HANDLE secondThreadHandle = CreateThread(NULL, 0, PrintOddNumbers, NULL, NULL, &secondThreadID);
    if (secondThreadHandle == INVALID_HANDLE_VALUE) {
        _tprintf(_T("Unable to create thread. Error code: %d\n"), GetLastError());
        exit(1);
    }

    // Wait for all threads to end
    HANDLE ThreadsHandlesArray[2] = {firsThreadHandle, secondThreadHandle};
    WaitForMultipleObjects(2, ThreadsHandlesArray, TRUE, INFINITE);

    // Close all unnecessary handles
    CloseHandle(firsThreadHandle);
    CloseHandle(secondThreadHandle);

    return 0;
}