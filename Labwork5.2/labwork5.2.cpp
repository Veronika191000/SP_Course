#include <stdio.h>
#include <iostream>
#include "windows.h" 
#include <ctime>
#include "tchar.h"

#define MIN_THREAD_SLEEPING 1
#define MAX_THREAD_SLEEPING 3
#define FILE_HANDLES_COUNT 3
#define BUFF_FOR_WRITING_TO_LOG_FILE_SIZE 32



// struct for contain params for new threads
typedef struct {
  DWORD id;
  DWORD timeSleep;
} NewThreaParams, *LPNewThreadParams;

// global variables
HANDLE hFilesAccessSemaphore;
CONST TCHAR ctsFileForSaving[] = L"Log.txt";

using namespace std;

// func for creating new thread
DWORD WINAPI NewThread(LPVOID param)
{
  // save params in stack
  DWORD dwThreadUserId = ((LPNewThreadParams)param)->id;
  DWORD dwThreadUserSleepingTime = ((LPNewThreadParams)param)->timeSleep;

  HANDLE hFileForSaving; // handle to file for saving info
  DWORD dwWaitingRes = ERROR_SUCCESS;
  clock_t  startTime = clock(), endTime;

  // wait for resourse (handle to file)
  WaitForSingleObject(hFilesAccessSemaphore, INFINITE);

  printf_s("Thread %d get resourse\n", dwThreadUserId);

  printf_s("Thread %d will sleep for %d second\n", dwThreadUserId, dwThreadUserSleepingTime);
  Sleep(dwThreadUserSleepingTime * 1000);

  // create handle to file
  if ((hFileForSaving = CreateFile(ctsFileForSaving,
    GENERIC_WRITE,
    FILE_SHARE_WRITE,
    NULL,
    OPEN_ALWAYS,
    FILE_ATTRIBUTE_NORMAL,
    NULL)) == INVALID_HANDLE_VALUE)
  {
    cout << "Error from thread " << dwThreadUserId << ", cant open file\n";
    ReleaseSemaphore(hFilesAccessSemaphore, 1, NULL);
    ExitThread(1);
  }

  // set file pointer to end
  if ((dwWaitingRes = SetFilePointer(hFileForSaving, 0, NULL, FILE_END)) == INVALID_SET_FILE_POINTER)
  {
    cout << "Error from thread " << dwThreadUserId << ", cant set a file pointer in the file\n";
    ReleaseSemaphore(hFilesAccessSemaphore, 1, NULL);
    ExitThread(2);
  }
  endTime = clock();

  // buff for writing to file
  TCHAR buff[BUFF_FOR_WRITING_TO_LOG_FILE_SIZE]{ '\0' };
  DWORD dwWritenBytes;

  // make string for saving in file
  _sntprintf_s(buff, BUFF_FOR_WRITING_TO_LOG_FILE_SIZE, L"Thread %d lasted %d seconds \n", dwThreadUserId, ((endTime - startTime) / 1000));

  // write to file
  if (!WriteFile(hFileForSaving, buff, _tcslen(buff) * sizeof(TCHAR), &dwWritenBytes, NULL))
  {
    cout << "Error from thread " << dwThreadUserId << ", cant write into the file\n";
    ReleaseSemaphore(hFilesAccessSemaphore, 1, NULL);
    ExitThread(3);
  }
  // clos handle to file
  CloseHandle(hFileForSaving);
  printf_s("Thread %d complete, bytes writen to file:%d\n", dwThreadUserId, dwWritenBytes);

  // allow other thread use resourse (handle to file)
  ReleaseSemaphore(hFilesAccessSemaphore, 1, NULL);
  return 0;
}
void StartThreads(DWORD dwCountThreads, HANDLE* Threads, DWORD dwRandRange) {
  for (int i = 0; i < dwCountThreads; i++)
  {
    srand(clock());
    NewThreaParams param = { 0 }; // struct for containing params for new thread
    param.id = i; // set user id
    param.timeSleep = (rand() % dwRandRange) + MIN_THREAD_SLEEPING; // rand time to sleeping
    Threads[i] = CreateThread(NULL, NULL, NewThread, (LPVOID)&param, NULL, NULL);
    if ((!Threads[i]) || (Threads[i] == INVALID_HANDLE_VALUE))
    {
      cout << "Error, cant start thread number: " << i << endl;
      exit(0);
    }
    Sleep(100); // for srand, for more differance in time between each rand()
  }
}
int main()
{
  HANDLE* Threads;

  // get count threads
  DWORD dwCountThreads;
  cout << "File handles count =>" << FILE_HANDLES_COUNT << endl;
  cout << "Input count of threads:";
  scanf_s("%d", &dwCountThreads);

  // alloc memory for new threads handles
  Threads = (HANDLE*)malloc(sizeof(HANDLE) * dwCountThreads);

  // create counter semaphore for control count handles to file
  hFilesAccessSemaphore = CreateSemaphore(NULL, FILE_HANDLES_COUNT, FILE_HANDLES_COUNT, L"FilesSemaphore");
  if (!hFilesAccessSemaphore || hFilesAccessSemaphore == INVALID_HANDLE_VALUE)
  {
    cout << " Cant create semaphore\n";
    return 1;
  }

  DWORD dwRandRange = MAX_THREAD_SLEEPING - MIN_THREAD_SLEEPING + 1;
  StartThreads(dwCountThreads, Threads, dwRandRange);

  WaitForMultipleObjects(dwCountThreads, Threads, TRUE, INFINITE); // wait until all thread completed

  // close all handles
  CloseHandle(hFilesAccessSemaphore);
  for (int i = 0; i < dwCountThreads; i++)
    CloseHandle(Threads[i]);
  free(Threads);

  CloseHandle(hFilesAccessSemaphore);
  cout << "All threads complete\n";
  getchar();
  return 0;
}

