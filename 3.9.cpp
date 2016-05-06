#include <tchar.h>
#include <windows.h>
#include <stdio.h>

// Sizes for matrix and vector
const int ROWS = 4;
const int COLUMNS = 3;
const int VECTOR_SIZE = COLUMNS;

// Matrix and Vector to multiply
int myMatrix[ROWS][COLUMNS] = {
        {1, 2, 3},
        {0, 2, 1},
        {1, 3, 2},
        {3, 0, 1}
};
int myVector[VECTOR_SIZE] = {3, 2, 4};

// Threads
DWORD ThreadsIDsArray[ROWS];
HANDLE ThreadsHandlesArray[ROWS];

// Argument 'struct' for threads
struct VectorsAndProduct {
    int *firstVector;
    int *secondVector;
    int product;
};


void MultiplyMatrixByVector(int matrix[ROWS][COLUMNS], int vector[VECTOR_SIZE], int resultVector[]);

DWORD WINAPI MultiplyVectorByVector(void *);

int main() {

    // "Result" vector creation
    const int RESULT_VECTOR_SIZE = ROWS;
    int myResultVector[RESULT_VECTOR_SIZE];

    // Do multiplication
    MultiplyMatrixByVector(myMatrix, myVector, myResultVector);

    // Print the "Result" vector
    for (int i = 0; i < RESULT_VECTOR_SIZE; ++i) {
        _tprintf(_T("%d\n"), myResultVector[i]);
    }

    return 0;
}

void MultiplyMatrixByVector(int matrix[ROWS][COLUMNS], int vector[VECTOR_SIZE], int resultVector[]) {

    VectorsAndProduct vectors[ROWS];

    for (int i = 0; i < ROWS; ++i) {

        // Set object fields to pass as an argument
        vectors[i].firstVector = matrix[i];
        vectors[i].secondVector = vector;
        vectors[i].product = 0;

        // Threads creation
        ThreadsHandlesArray[i] = CreateThread(NULL, 0, MultiplyVectorByVector, &vectors[i], NULL, &ThreadsIDsArray[0]);
        if (ThreadsHandlesArray[i] == INVALID_HANDLE_VALUE) {
            _tprintf(_T("Unable to create thread. Error code: %d\n"), GetLastError());

            //There is a need to close all previous handles if one of the handles is INVALID
            for (int j = 0; j < i; ++j) {
                CloseHandle(ThreadsHandlesArray[j]);
            }

            return;
        }
    }

    // Wait for all threads to end
    WaitForMultipleObjects(ROWS, ThreadsHandlesArray, TRUE, INFINITE);
    for (int i = 0; i < ROWS; ++i) {
        CloseHandle(ThreadsHandlesArray[i]);
    }

    // Fill the "Result" vector
    for (int i = 0; i < ROWS; ++i) {
        resultVector[i] = vectors[i].product;
    }
}

DWORD WINAPI MultiplyVectorByVector(void *arg) {

    // Cast to VectorsAndProduct*
    VectorsAndProduct *instance = static_cast<VectorsAndProduct *>(arg);

    // Multiply a vector by a vector
    for (int i = 0; i < COLUMNS; ++i) {
        instance->product += (instance->firstVector[i] * instance->secondVector[i]);
    }

    return 0;
}