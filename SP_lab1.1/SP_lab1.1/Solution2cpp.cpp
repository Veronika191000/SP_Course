#include <stdio.h>
#include <Windows.h>
#include <locale.h>

#define BytesToRead  2048 

void AsciiToUnicode(HANDLE sourseFile, HANDLE resultFile) {
  BYTE lpBuffer[BytesToRead];
  DWORD dwBytes;
  while (ReadFile(sourseFile, lpBuffer,
    BytesToRead,//������� ����� ������� ������
    (LPDWORD)&dwBytes,//���������� ������� ������� ������o
    NULL))
  {
    if (dwBytes == 0) {//EOF
      break;
    }
    else {
      //������� �� ��������� MultiByteToWideChar ���������� ������ ��������� ������ � �������x
      int str_length = MultiByteToWideChar(CP_UTF8, 0, (LPCCH)lpBuffer, dwBytes, NULL, 0);
      //����� �������� ����� ��������� ����� � ���� ��������
      wchar_t* str = new wchar_t[str_length];
      //�������������� ��������� ASCII to Unicode, 
      MultiByteToWideChar(CP_UTF8, 0,
        (LPCCH)lpBuffer,//��������� �� �������� ������
        dwBytes, //������ ������
        str, //��������� �� �����
        str_length);//������ ������
      if (!WriteFile(resultFile, str, str_length * sizeof(wchar_t), (LPDWORD)&dwBytes, NULL)) {
        printf("\nFaile\n");
        break;
      }
    }
  }
  CloseHandle(sourseFile);
  CloseHandle(resultFile);
  exit(0);
}

void UnicodeToAscii(HANDLE sourseFile, HANDLE new_file) {
  setlocale(LC_ALL, "Russian");
  BYTE lpBuffer[BytesToRead];
  DWORD dwBytes;
  while (ReadFile(sourseFile, lpBuffer, BytesToRead, (LPDWORD)&dwBytes, NULL))
  {
    if (dwBytes == 0) {
      break;
    }
    else {
      //������� �� ��������� MultiByteToWideChar ���������� ������ ��������� ������ � �������� ������
      int u_str_length = MultiByteToWideChar(CP_UTF8, 0, (LPCCH)lpBuffer, dwBytes, NULL, 0);
      //����� �������� ����� ��������� ����� � ���� ��������
      wchar_t* u_str = new wchar_t[u_str_length];
      MultiByteToWideChar(CP_UTF8, 0, (LPCCH)lpBuffer, dwBytes, u_str, u_str_length);
      //������� �� ��������� WideCharToMultiByte, ����� �������� ������ �������� ������ (ANSI)
      int a_str_length = WideCharToMultiByte(CP_ACP, 0, u_str, u_str_length, NULL, 0, NULL, NULL);
      //��������� �� �����, ������� �������� ��������������� ������.
      char* a_str = new char[a_str_length];
      WideCharToMultiByte(CP_ACP, 0, u_str, u_str_length, a_str, a_str_length, NULL, NULL);
      if (!WriteFile(new_file, a_str, a_str_length * sizeof(char), (LPDWORD)&dwBytes, NULL)) {
        printf("\nFail\n");
        break;
      }
    }
  }
  CloseHandle(sourseFile);
  CloseHandle(new_file);
  exit(0);
}

int main(int argc, char* argv[]) {
  if (argc != 4 || ((strcmp(argv[1], "-a") != 0) && (strcmp(argv[1], "-u")) != 0)) {
    printf("You need to write only 4 arguments\n");
    printf("Write -a : ASCII to UNICODE\n");
    printf("Write -u : UNICODE to ASCII\n");
    printf(" File pathes  file 1:path to source file 2 : path to result  \n");
    return 1;
  }
  //������ ��� ������ � ������ HANDLE
  HANDLE resourse_file = CreateFileA(argv[2], GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  HANDLE result_file = CreateFileA(argv[3], GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
  if (resourse_file != INVALID_HANDLE_VALUE && result_file != INVALID_HANDLE_VALUE) {
    if (strcmp(argv[1], "-a") == 0) {
      AsciiToUnicode(resourse_file, result_file);
    }
    else UnicodeToAscii(resourse_file, result_file);
  }
  else {
    printf("\nSomething went wrong(\n");
  }
}
