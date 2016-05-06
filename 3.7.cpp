#include <tchar.h>
#include <windows.h>
#include <stdio.h>

const int N = 6;
const int K = 3;

struct Argument {
    int inData;
    int outData;

    Argument() : inData(0), outData(1) { }
};

DWORD WINAPI Factorial(void *arg) {
    // Cast to int*
    Argument *info_arg = static_cast<Argument *>(arg);

    // Calculate the factorial of 'in' and write it in 'out'
    for (int i = 1; i <= info_arg->inData; ++i) {
        info_arg->outData *= i;
    }

    return 0;
}


int main() {

    // Threads creation
    Argument n;
    n.inData = N;
    DWORD ID1;
    HANDLE handle1 = CreateThread(NULL, 0, Factorial, &n, NULL, &ID1);
    if (handle1 == INVALID_HANDLE_VALUE) {
        _tprintf(_T("Unable to create thread. Error code: %d"), GetLastError());
        exit(1);
    }

    Argument k;
    k.inData = K;
    DWORD ID2;
    HANDLE handle2 = CreateThread(NULL, 0, Factorial, &k, NULL, &ID2);
    if (handle2 == INVALID_HANDLE_VALUE) {
        _tprintf(_T("Unable to create thread. Error code: %d"), GetLastError());
        exit(1);
    }

    Argument m;
    m.inData = n.inData - k.inData;
    DWORD ID3;
    HANDLE handle3 = CreateThread(NULL, 0, Factorial, &m, NULL, &ID3);
    if (handle3 == INVALID_HANDLE_VALUE) {
        _tprintf(_T("Unable to create thread. Error code: %d"), GetLastError());
        exit(1);
    }

	// Wait for all threads to end
    HANDLE ThreadsHandlesArray[3] = {handle1, handle2, handle3};
    WaitForMultipleObjects(3, ThreadsHandlesArray, TRUE, INFINITE);

    // Print the result
    int result = n.outData / (m.outData * k.outData);
    _tprintf(_T("%d\n"), result);

    // Close all unnecessary handles
    CloseHandle(handle1);
    CloseHandle(handle2);
    CloseHandle(handle3);

    return 0;
}