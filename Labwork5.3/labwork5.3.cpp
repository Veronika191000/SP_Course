#include <stdio.h>
#include "iostream"
#include "windows.h"
#include <ctime>

#define COUNT_EL_ARRAY 10000000
#define ARRAY_NUM_MIN 0 
#define ARRAY_NUM_MAX 100000 

#define SUM_OF_ELEMENTS 0
#define MIN_MAX_VALUE 1
#define AVARAGE_VALUE 2


DWORD WINAPI WithCriticalSection(LPVOID param);
DWORD WINAPI WithoutCriticalSection(LPVOID param);
void GenereteArrayNums(int* arr); // generate numbers for array

CRITICAL_SECTION CriticalSection; // critical section
HANDLE* threadHandles; // handles of creat threads
CONST DWORD COUNT_THREADS = 3; // count threads
int arrayForCalculating[COUNT_EL_ARRAY]{ 0 }; 

using namespace std;


LONG64 AvarageValue(int* mass)
{
  LONG64 result = 0;
  for (int i = 0; i < COUNT_EL_ARRAY; i++)
  {
    result += mass[i];
  }
  result /= COUNT_EL_ARRAY;
  cout << "Avarage:" << result << endl;
  return result;
}

void MaxMinValue(int* mass)
{
  int max = mass[0];
  int min = mass[0];
  for (int r = 1; r < COUNT_EL_ARRAY; r++)
  {
    if (max < mass[r]) max = mass[r];
    if (min > mass[r]) min = mass[r];
  }
  cout << "\nMin: " << min << endl;
  cout << "Max: " << max << endl;
}

int SumOfElemets(int* mass)
{
  int sum = mass[0];
  for (int i = 1; i < COUNT_EL_ARRAY; i++)
  {
    sum += mass[i];
  }
  cout << "Sum of elements:" << sum << endl;;
  return sum;
}

int main(int argc, char* argv[])
{
  clock_t start, end;

  InitializeCriticalSection(&CriticalSection);

  threadHandles = (HANDLE*)malloc(sizeof(HANDLE) * COUNT_THREADS);

  GenereteArrayNums(arrayForCalculating); // generate array nums

  // start with using critical section
  cout << "Start function with critical section\n";
  start = clock();
  for (int i = 0; i < COUNT_THREADS; i++) // create new threads
  {
    threadHandles[i] = CreateThread(NULL,NULL, WithCriticalSection,(LPVOID)i,NULL,NULL);
  }
  WaitForMultipleObjects(COUNT_THREADS, threadHandles, TRUE, INFINITE);
  end = clock();
  cout << "Function time in critical section : " << end - start << " ms.\n";
  //cout << "\n***********************************************************************\n"<<end;
  // start without using critical section
  cout << "\nStart function without critical section\n";
  start = clock();
  for (int i = 0; i < COUNT_THREADS; i++) // create new threads
  {
    threadHandles[i] = CreateThread(NULL,NULL,WithoutCriticalSection,LPVOID(i), NULL,NULL);
  }
  // wait until all threads complete
  WaitForMultipleObjects(COUNT_THREADS, threadHandles, TRUE, INFINITE);
  end = clock();
  cout << "Function time without critical section : " << end - start << " ms.\n";

  for (int i = 0; i < COUNT_THREADS; i++)
    CloseHandle(threadHandles[i]);
  DeleteCriticalSection(&CriticalSection);
  return 0;
}
void GenereteArrayNums(int* arr)
{
  DWORD randRange = ARRAY_NUM_MAX - ARRAY_NUM_MIN;
  // array generation
  for (DWORD i = 0; i < COUNT_EL_ARRAY; i++)
  {
    arr[i] = (rand() % randRange) + ARRAY_NUM_MIN;
  }
}

//  with using critical section 
DWORD WINAPI WithCriticalSection(LPVOID param)
{
  switch ((int)param)
  {
  case SUM_OF_ELEMENTS:
  {
    EnterCriticalSection(&CriticalSection);
    SumOfElemets(arrayForCalculating);
    LeaveCriticalSection(&CriticalSection);
  } break;
  case MIN_MAX_VALUE:
  {
    EnterCriticalSection(&CriticalSection);
    MaxMinValue(arrayForCalculating);
    LeaveCriticalSection(&CriticalSection);
  } break;
  case AVARAGE_VALUE:
  {
    EnterCriticalSection(&CriticalSection);
    AvarageValue(arrayForCalculating);
    LeaveCriticalSection(&CriticalSection);
  } break;
  default:
    break;
  }
  return 0;
}

//  without using critical section 
DWORD WINAPI WithoutCriticalSection(LPVOID param)
{
  switch ((int)param)
  {
  case SUM_OF_ELEMENTS:
  {
    SumOfElemets(arrayForCalculating);
  } break;
  case MIN_MAX_VALUE:
  {
    MaxMinValue(arrayForCalculating);
  } break;
  case AVARAGE_VALUE:
  {
    AvarageValue(arrayForCalculating);
  } break;
  default:
    break;
  }
  return 0;
}



