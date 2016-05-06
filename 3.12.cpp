#include <tchar.h>
#include <stdio.h>
#include <windows.h>

// Event
HANDLE eventHandle;

int globalArray[300];

const int N = 200;
const int M = 3;

DWORD WINAPI FillTheArray(void *) {

    // Fill the array
    for (int i = 0; i < N; ++i) {
        globalArray[i] = i;
    }

    // Let second thread print the array
    SetEvent(eventHandle);

    return 0;
}

DWORD WINAPI TryToPrintArray(void *) {

    //Wait M milliseconds for 1st thread to fill the globalArray and print the array if it's done
    DWORD waitResult = WaitForSingleObject(eventHandle, M);

    if (waitResult == WAIT_OBJECT_0)
        for (int i = 0; i < 300; ++i) {
            _tprintf(_T("%d\n"),globalArray[i]);
        }
    else if(waitResult == WAIT_TIMEOUT)
    {
        _tprintf(_T("Can't print the array, because it isn't filled yet.\n"));
    }
    else
    {
        _tprintf(_T("%d\n"),(int) GetLastError());
    }

    return 0;
}

int main() {

    // Event Creation
    eventHandle = CreateEvent(NULL, TRUE, FALSE, NULL);

    // Threads Creation
    DWORD firstThreadID;
    HANDLE firstThreadHandle = CreateThread(NULL, 0, FillTheArray, NULL, 0, &firstThreadID);
    if (firstThreadHandle == INVALID_HANDLE_VALUE) {
        _tprintf(_T("Unable to create thread. Error code: %d \n"), (int) GetLastError());
        exit(1);
    }

    DWORD secondThreadID;
    HANDLE secondThreadHandle = CreateThread(NULL, 0, TryToPrintArray, NULL, 0, &secondThreadID);
    if (secondThreadHandle == INVALID_HANDLE_VALUE) {
        _tprintf(_T("Unable to create thread. Error code: %d \n"), (int) GetLastError());
        exit(1);
    }

    // Wait for all threads to end
    HANDLE ThreadsHandlesArray[2] = {firstThreadHandle, secondThreadHandle};
    WaitForMultipleObjects(2, ThreadsHandlesArray, TRUE, INFINITE);

    // Close all unnecessary handles
    CloseHandle(eventHandle);
    CloseHandle(firstThreadHandle);
    CloseHandle(secondThreadHandle);

    return 0;
}