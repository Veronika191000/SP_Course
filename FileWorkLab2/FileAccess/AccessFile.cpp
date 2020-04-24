// FileData.cpp : Defines the entry point for the console application.
//


#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<windows.h>
#include<string>
#include<stdio.h>
#include<string.h>
#include<tchar.h>

#define STEP 100
#define INFO_SIZE 80
#define FREE_ROW_ID -1
#define LINE_T_SIZE sizeof(LINE_T)

using namespace std;


typedef struct {
  DWORD id;
  FILETIME time;
  CHAR info[INFO_SIZE];
  DWORD changed;
}LINE_T;

typedef struct {
  DWORD lastId;
  DWORD fileSize;
  DWORD exists;
}FIRST_LINE_T;



void ModifyLine(INT);
DWORD FindLine(INT);
void DeleteLineFromFile(INT);
void ShowInformationFile();
void InitData();
void WriteLineToFile();

HFILE file;


int _tmain(INT argc, TCHAR** argv) {

  HANDLE std = GetStdHandle(STD_INPUT_HANDLE);
  DWORD mode;
  GetConsoleMode(std, &mode);
  SetConsoleMode(std, mode | ENABLE_PROCESSED_INPUT);

  LPTSTR act = NULL;
  LPCTSTR fileName = L"../fileData.txt";

  try {

    if (argv[1] == NULL) throw 1;
    //Create file
    file = (HFILE)CreateFile(
      (LPCTSTR)fileName, GENERIC_WRITE | GENERIC_READ, 0,
      NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    int sizeOfFile = GetFileSize((HANDLE)file, NULL);
    if (sizeOfFile == 0) {
      InitData();
    }
    BYTE commmand = (BYTE)act[1];
    INT num;
    if (commmand == 's')  ShowInformationFile();
    else if (commmand == 'i') while (TRUE) WriteLineToFile();
    else if (commmand == 'd') {
      argv[2] == NULL ? throw 2 : num = _ttoi(argv[2]); DeleteLineFromFile(num);
    }
    else if (commmand == 'm') {
      argv[2] == NULL ? throw 2 : num = _ttoi(argv[2]);
      cout << "Input new info: ";
      ModifyLine(num);
    }
  }
  catch (int e) {
    cout << "Error: argument is wrong " << endl;
  }

  CloseHandle((HANDLE)file);
}

DWORD FindLine(INT num) {
  //Find free row
  DWORD count;
  BOOL res = TRUE;
  LINE_T line = {};
  SetFilePointer((HANDLE)file, LINE_T_SIZE, NULL, FILE_BEGIN);

  do res = ReadFile((HANDLE)file, (LPVOID)&line, LINE_T_SIZE, &count, NULL);
  while ((res & count != 0) && (line.id != num));

  if (line.id == num) {
    int offset = (LINE_T_SIZE);
    return SetFilePointer((HANDLE)file, -offset, NULL, FILE_CURRENT);
  }
  else {
    SetFilePointer((HANDLE)file, 0, NULL, FILE_END);
  }
  return FALSE;
}

void ModifyLine(INT num) {
  DWORD id = -1, count, newChang;
  if (FindLine(num)) {
    LINE_T line = {};
    ReadFile((HANDLE)file, (LPVOID)&line, LINE_T_SIZE, &count, NULL);

    //new inf
    if (!fgets(line.info, INFO_SIZE, stdin)) {
      return;
    }

    //set count of changes
    line.changed++;

    int offset = (LINE_T_SIZE);
    SetFilePointer((HANDLE)file, -offset, NULL, FILE_CURRENT);
    WriteFile((HANDLE)file, (LPVOID)&line, LINE_T_SIZE, &count, NULL);
  }
  else {
    cout << "Record doesn't exist." << endl;
  }
}

void DeleteLineFromFile(INT num) {
  DWORD id = FREE_ROW_ID, count;
  if (FindLine(num)) {
    WriteFile((HANDLE)file, (LPVOID)&id, sizeof(DWORD), &count, NULL);
  }
  else {
    cout << "Record doesn't exist." << endl;
    return;
  }
  //dec index
  FIRST_LINE_T fline = {};
  DWORD red;
  SetFilePointer((HANDLE)file, 0, NULL, FILE_BEGIN);
  ReadFile((HANDLE)file, (LPVOID)&fline, sizeof(FIRST_LINE_T), &red, NULL);
  fline.exists--;
  SetFilePointer((HANDLE)file, 0, NULL, FILE_BEGIN);
  WriteFile((HANDLE)file, (LPVOID)&fline, sizeof(FIRST_LINE_T), &red, NULL);
}

void WriteLineToFile() {

  LINE_T fileline = {};
  FIRST_LINE_T fline = {};

  //init tmp memory
  DWORD red;
  SetFilePointer((HANDLE)file, 0, NULL, FILE_BEGIN);
  ReadFile((HANDLE)file, (LPVOID)&fline, sizeof(FIRST_LINE_T), &red, NULL);
  //correct dif between first and others lines
  SetFilePointer((HANDLE)file, LINE_T_SIZE, NULL, FILE_BEGIN);

  //get info from file
  if (!fgets(fileline.info, INFO_SIZE, stdin)) {
    return;
  }

  //update info
  fline.exists++;
  fline.lastId++;
  fline.fileSize = GetFileSize((HANDLE)file, NULL);

  //apply first line
  SetFilePointer((HANDLE)file, 0, NULL, FILE_BEGIN);
  WriteFile((HANDLE)file, (LPVOID)&fline, LINE_T_SIZE, &red, NULL);

  //struct init
  fileline.id = fline.lastId;

  //get time
  SYSTEMTIME st;
  GetSystemTime(&st); // gets current time
  SystemTimeToFileTime(&st, &fileline.time); // converts to file time format

  //changed are 0 already

  //find free row
  FindLine(FREE_ROW_ID);
  WriteFile((HANDLE)file, (LPVOID)&fileline, LINE_T_SIZE, &red, NULL);
}

void ShowInformationFile() {
 
  DWORD count = 0;
  FIRST_LINE_T fline = {};
  //set file pointer
  SetFilePointer((HANDLE)file, 0, NULL, FILE_BEGIN);
  //read file
  ReadFile((HANDLE)file, (LPVOID)&fline, sizeof(FIRST_LINE_T), &count, NULL);
  //correct dif between first and others lines
  SetFilePointer((HANDLE)file, LINE_T_SIZE, NULL, FILE_BEGIN);
  BOOL result = FALSE;
  LINE_T line = {};
  while(TRUE){
    result = ReadFile((HANDLE)file, (LPVOID)&line, LINE_T_SIZE, &count, NULL);

    if (result & count == 0) {
      break;
    }

    if (line.id == FREE_ROW_ID) {
      continue;
    }

    SYSTEMTIME st = {};
    FileTimeToSystemTime(&line.time, (LPSYSTEMTIME)&st);

    cout << "#" << line.id << endl;
    cout << "CreatedTime: " << st.wHour << ":" << st.wMinute
      << " " << st.wDay << "." << st.wMonth << "." << st.wYear << endl;
    cout << "Message: " << line.info << endl;
    cout << "Number of change : " << line.changed << " times" << endl;
    cout << "\n\t***\n";
  } 

  cout << "\n-----------------" << endl;
  cout << "Total: " << fline.exists << " records, " << fline.fileSize << " bytes." << endl;
}
//data initialize
void InitData() {
  LPSTR bufer = new CHAR[LINE_T_SIZE];
  memset(bufer, 0, LINE_T_SIZE);
  DWORD written;
  BOOL res = WriteFile((HANDLE)file, bufer, LINE_T_SIZE, &written, NULL);
  delete[](bufer);
}

