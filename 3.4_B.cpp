// There can be a problem concerning _ttoi and _tcscpy functions if you use Visual Studio 2015
// Solution is to either disable the deprecation or to use 'functionname'_s functions instead

#include <tchar.h>
#include <windows.h>
#include <stdio.h>

// How long B must wait (in milliseconds)
const int GIVEN_TIME = 8000;

int _tmain(int argc, TCHAR *argv[]) {

    // Get the parent process ID and open that process
    TCHAR StringId[10];
    _tcscpy(StringId, argv[1]);

    DWORD Id = _ttoi(StringId);

    HANDLE handle = OpenProcess(SYNCHRONIZE, FALSE, Id);
    if (handle == NULL) {
        _tprintf(_T("Unable to find A. Error code: %d\n"), GetLastError());
        exit(1);
    }

    // Wait a few seconds and check the status of A
    DWORD result = WaitForSingleObject(handle, GIVEN_TIME);
    if (result == WAIT_OBJECT_0)
        _tprintf(_T("A has already finished\n"));
    else if (result == WAIT_TIMEOUT)
        _tprintf(_T("A is still working\n"));
    else
        _tprintf(_T("Error while/after waiting. Error code: %d\n"), GetLastError());

    return 0;
}