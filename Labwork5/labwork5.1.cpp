#include <stdio.h>
#include "windows.h"


int main()
{
 
  TCHAR sMutexName[]{ L"OneInstance" };

  HANDLE hOneInstance = OpenMutex(MUTEX_ALL_ACCESS, FALSE, sMutexName);

  if (!hOneInstance)
  {
    hOneInstance = CreateMutex(NULL, TRUE, sMutexName);
    printf("Is Running");
    while (1) {}
  }
  else 
  {
    printf("One intance is available \n");
    Sleep(2000);
  }
}