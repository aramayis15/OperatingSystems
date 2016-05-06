// There can be a problem concerning _itot and _tcscat functions if you use Visual Studio 2015
// Solution is to either disable the deprecation or to use 'functionname'_s functions instead

#include <tchar.h>
#include <windows.h>
#include <stdio.h>

int _tmain() {

    // Startup info and process information are by default
    STARTUPINFO startupinfo;
    ZeroMemory(&startupinfo, sizeof(startupinfo));
    startupinfo.cb = sizeof((startupinfo));

    PROCESS_INFORMATION process_information;
    ZeroMemory(&process_information, sizeof(process_information));


    TCHAR name[] = _T("There must be the location of the 'B'");

    // Pass the current process ID to child process (in this case it's B) by command line
    DWORD Id = GetCurrentProcessId();
    TCHAR StringId[10];
    _itot(Id, StringId, 10);

    _tcscat(name, _T(" "));
    _tcscat(name, StringId);

    // Open B
    if (!CreateProcess(NULL, name, NULL, NULL, FALSE, NULL, NULL, NULL, &startupinfo, &process_information))
        _tprintf(_T("Unable to open B. Error code: %d\n"), GetLastError());

    // Wait for 5 seconds
    Sleep(5000);

    // Close all unnecessary handles
    CloseHandle(process_information.hProcess);
    CloseHandle(process_information.hThread);

    return 0;
}