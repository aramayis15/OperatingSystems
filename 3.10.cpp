#include <tchar.h>
#include <windows.h>
#include <stdio.h>

bool isPrime(int number) {
    if(number == 0 || number==1)
        return false;
    for (int i = 2; i * i <= number; ++i) {
        if (number % i == 0)
            return false;
    }

    return true;
}


const int N = 15;
const int ARRAY_SIZE = 100;

// Threads
DWORD ThreadsIDsArray[N];
HANDLE ThreadsHandlesArray[N];

struct Array_Interval {
    int *arr;
    int start_index;
    int finish_index;
    int quantity;
} array_interval[N];


DWORD WINAPI PrimeNumbersCount(void *arg) {
    Array_Interval *instance = static_cast<Array_Interval *>(arg);
    instance->quantity = 0;

    for (int i = instance->start_index; i <= instance->finish_index; ++i) {
        if (isPrime(instance->arr[i]) == true)
            ++(instance->quantity);
    }

    return 0;
}


int main() {

    // Array creation
    int array[ARRAY_SIZE];
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        array[i] = i;
    }


    for (int i = 0; i < N; ++i) {

        // Set object fields to pass as an argument
        if (i == N - 1) {
            array_interval[i].arr = array;
            array_interval[i].start_index = i * (ARRAY_SIZE / N);
            array_interval[i].finish_index = ARRAY_SIZE - 1;
        }
        else {
            array_interval[i].arr = array;
            array_interval[i].start_index = i * (ARRAY_SIZE / N);
            array_interval[i].finish_index = (i + 1) * (ARRAY_SIZE / N) - 1;
        }

        // Threads creation
        ThreadsHandlesArray[i] = CreateThread(NULL, 0, PrimeNumbersCount, &array_interval[i], NULL, &ThreadsIDsArray[i]);
        if (ThreadsHandlesArray[i] == INVALID_HANDLE_VALUE) {
            _tprintf(_T("Unable to create thread. Error code: %d\n"), GetLastError());

            //There is a need to close all previous handles if one of the handles is INVALID
            for (int j = 0; j < i; ++j) {
                CloseHandle(ThreadsHandlesArray[i]);
            }

            exit(1);
        }
    }

    // Wait for all threads to end
    WaitForMultipleObjects(N, ThreadsHandlesArray, TRUE, INFINITE);
    for (int i = 0; i < N; ++i) {
        CloseHandle(ThreadsHandlesArray[i]);
    }

    // How many prime numbers are in the array
    int result = 0;
    for (int i = 0; i < N; ++i)
        result += array_interval[i].quantity;

    // Print result
    _tprintf(_T("%d\n"), result);

    return 0;
}