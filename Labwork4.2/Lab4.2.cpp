

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include <locale.h>
#include <tchar.h>


#define MAXVAL 100
#define MINVAL 10

#define MINCOUNT 5
#define MAXCOUNT 10


int addrInd, sizeInd;

int SumElements() {

  int* arr = (int*)TlsGetValue(sizeInd);
  int count = (int)TlsGetValue(addrInd);
  int r = 0;
  for (size_t i = 0; i < count; i++)
  {
    r += arr[i];
  }
  return r;
}
void Divider(int* arr, int count, int* array) {

  for (size_t i = 0; i < count; i++)
  {
    int curr = arr[i];
    array[i] = curr;
  }
}
void Thread() {

  //making generator random
  DWORD currId = GetCurrentThreadId();
  srand((unsigned int)currId);

  //get count
  int count = rand() % (MAXCOUNT - MINCOUNT) + MINCOUNT;

  //making values random
  int* values = new int[count];
  for (size_t j = 0; j < count; j++)
  {
    values[j] = rand() % (MAXVAL - MINVAL) + MINVAL;
  }

  int* Array = new int[count];
  Divider(values, count, Array);

  //save to TLS
  if (!TlsSetValue(sizeInd, (LPVOID)Array)) {
    printf("First value has't been set\n");
  }

  if (!TlsSetValue(addrInd, (LPVOID)count)) {
    printf("Second value  has't been set\n");
  }

  printf("Thread ID: %d Sum: %d\n", currId, SumElements());

  delete[] values;
  delete[] Array;
}

int main(int argc, TCHAR** argv)
{
  if (argc != 2) {
    printf("You entered an invalid number of arguments");
    exit(0);
  }
  int countThreads = _ttoi(argv[1]);

  HANDLE* threads = new HANDLE[countThreads];
  //allocate tls for all the threads
  addrInd = TlsAlloc();
  sizeInd = TlsAlloc();
  for (size_t i = 0; i < countThreads; i++)
  {
    DWORD IDThread;
    threads[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Thread, NULL, 0, &IDThread);
  }

  for (size_t i = 0; i < countThreads; i++)
    WaitForSingleObject(threads[i], INFINITE);

  TlsFree(addrInd);//free TLS
  delete[] threads;

  return 0;
}