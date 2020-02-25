#include <stdio.h>
#include <Windows.h>
#include <locale.h>

void ErrorInfo() {
  setlocale(LC_ALL, "Russian");
  MEMORYSTATUS mem_status;//�������� ���������� � �������
  GlobalMemoryStatus(&mem_status);//��������� ���. ��� ������� � ��������� ��� � ������� ������ ��
  // ��������� ������, ������ ��������� �� ���� � ��� ������� ������������ NULL � �������� GetLastError
  HLOCAL memory_object = LocalAlloc(LPTR, mem_status.dwTotalVirtual * 100);
  if (memory_object == NULL) {
    LPVOID descript_mess;//�������� ������
    DWORD error_code = GetLastError();//������������ ��� ��������� ��������� ������
    //��������� �� ���� ������ �������� �� ��������
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL,
      error_code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&descript_mess, 0, NULL);
    char* err_mess = (char*)descript_mess;
    wprintf(L"Fail generation. Code : 0x%x\n", error_code);
    wprintf(L"Error description message: %s\n", err_mess);
  }
}

void SystemInfo() {
  SYSTEM_INFO system_info;
  GetSystemInfo(&system_info);// ���������� ���� � ����������� � �������
  printf("  Hardware information: \n");
  printf("  OEM ID: %u\n", system_info.dwOemId);
  printf("  Number of processors: %u\n", system_info.dwNumberOfProcessors);
  printf("  Page size: %u\n", system_info.dwPageSize);
  printf("  Processor type: %u\n", system_info.dwProcessorType);
  printf("  Minimum application address: %lx\n", system_info.lpMinimumApplicationAddress);
  printf("  Maximum application address: %lx\n", system_info.lpMaximumApplicationAddress);
  printf("  Active processor mask: %u\n", system_info.dwActiveProcessorMask);
}


void main(int argc, char* argv[]) {
  if ((argc != 2 || ((strcmp(argv[1], "-e") != 0) && (strcmp(argv[1], "-s"))) != 0)) {
    printf("You need to write only 2 arguments:\n");
    printf("Maybe you need to write -e or -s\n");
  }
  else if (strcmp(argv[1], "-e") == 0) {
    ErrorInfo();
  }
  else SystemInfo();

}