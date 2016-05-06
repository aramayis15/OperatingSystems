#include <tchar.h>
#include <windows.h>
#include <stdio.h>

int _tmain() {

    // Startup info with 'ShowWindow' field set to "SW_SHOWMINIMIZED"
    STARTUPINFO startupinfo;
    ZeroMemory(&startupinfo, sizeof(startupinfo));
    startupinfo.cb = sizeof((startupinfo));
    startupinfo.dwFlags = STARTF_USESHOWWINDOW;
    startupinfo.wShowWindow = SW_SHOWMINIMIZED;

    // Process Information by default
    PROCESS_INFORMATION process_information;
    ZeroMemory(&process_information,sizeof(process_information));

    TCHAR name [] = _T("notepad.exe");

    // Open notepad in minimized state (because of Startup Info)
     if(!CreateProcess(NULL, name, NULL, NULL, FALSE, NULL, NULL, NULL, &startupinfo, &process_information))
        _tprintf(_T("Unable to open notepad. Error code: %d\n"), GetLastError());


    // Close all unnecessary handles
    CloseHandle(process_information.hProcess);
    CloseHandle(process_information.hThread);

    // Wait 2 seconds and finish
    Sleep(2000);

    return 0;
}