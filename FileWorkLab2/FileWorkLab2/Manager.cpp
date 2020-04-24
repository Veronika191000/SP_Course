

#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_WCSTOK
#define _CRT_NON_CONFORMING_SWPRINTFS

#include <windows.h>
#include <string.h>
#include <tchar.h>
#include <iomanip>
#include <iostream>
#include <fcntl.h>
#include <io.h>


#define MAX_BUFFER 1000
#define N_MEASURES 4
#define MAX_ARGC 3
#define FILE_NAME 40
#define DISP_STEP 15


using namespace std;


//Creating file 
void CreateDir(LPTSTR dirPath) {
         if (!*dirPath) throw (INT)2;
         if (!CreateDirectory(dirPath, NULL)) {
           wcout << "Sorry, error occured while creating directory." << endl;
           throw GetLastError();
   }
         wcout << "File was created" << endl;
}

//Change directory 
void ChangeDir(LPTSTR dirPath){
  if (!*dirPath) throw (INT)2;
  SetCurrentDirectory(dirPath);
  GetCurrentDirectory(MAX_BUFFER, dirPath);
  wcout << "Directory has been changed to " << dirPath << endl;
}

// Copy file
void Copyfile(LPTSTR dirPath, LPTSTR src) {

    if (!*dirPath || !*src) throw (INT)1;
    if (!CopyFile(src, dirPath, FALSE)) {
      wcout << "Sorry, can't copy the file." << endl;
      throw GetLastError();
    }
    wcout << "Successful operation" << endl;
}
//Delete file or Directory
void DeleteFileOrDirectory(LPTSTR dirPath) {

  if (!*dirPath) throw (INT)2;

  DWORD attr = GetFileAttributes(dirPath);

  if (attr & FILE_ATTRIBUTE_DIRECTORY) {
    if (!RemoveDirectory(dirPath)) {
      throw GetLastError();
    }
  }
  else {
    if (!DeleteFile(dirPath)) {
      throw GetLastError();
    }
  }
  wcout << "File or directory was deleted!" << endl;
}
// Directory inf
void DirectoryInformaion(LPTSTR dirPath) {
    dirPath = new TCHAR[MAX_BUFFER];
    GetCurrentDirectory(MAX_BUFFER, dirPath);
    wcout << dirPath << endl; 
}
//Check path
BOOL IsNormal(LPCTSTR path) {
  INT i = 0;
  while (path[i] != TEXT('\0')) {
    if (path[i] == TEXT(':')) {
      return TRUE;
    }
    i++;
  }
  return FALSE;
}

//Decoding information
BOOL SplitInputAndDecode(LPTSTR input, LPTSTR com, LPTSTR dirPath, LPTSTR src, LPTSTR pathfile) {

  //split input string
  LPWSTR sep = LPWSTR(L" \n");
  LPTSTR token;
  INT argc = 0;
  LPTSTR argv[MAX_ARGC * sizeof(LPWSTR)] = {};
  token = wcstok(input, sep);
  if (!token) return FALSE;
  while (token) {
    argv[argc] = token;
    argc++;
    token = wcstok(NULL, sep);
  }

  wcscpy(com, argv[0]);

  if (argc > 1) {
    if (!IsNormal(argv[1])) {
      GetFullPathName(argv[1], MAX_BUFFER, dirPath, &pathfile);
    }
    else {
      wcscpy(dirPath, argv[1]);
    }
  }
  if (argc > 2) {
    if (!IsNormal(argv[2])) {
      GetFullPathName(argv[2], MAX_BUFFER, src, &pathfile);
    }
    else {
      wcscpy(src, argv[2]);
    }
  }
  return TRUE;
}
//File inf
void FileInf(LPTSTR dirPath) {
        if (!*dirPath) throw (INT)2;
        WIN32_FIND_DATA info;
        if (FindFirstFile(dirPath, &info) == INVALID_HANDLE_VALUE) throw (DWORD)2;
        wcout << "File information:\n" << endl;

        //if it's a directory
        if (info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
          wcout << "<directory>" << endl;
        }

        wcout << "NameFile: " << info.cFileName << endl;
        wcout << "Attribs mask: " << info.dwFileAttributes << endl;
        wcout << "Alter: " << info.cAlternateFileName << endl;

        unsigned long long size = (unsigned long long)info.nFileSizeHigh << 32 | info.nFileSizeLow;
        LPCSTR measures[] = { "B", "KB" ,"MB" ,"GB" };
        INT i = 0;
        while (size >> 10 * ++i);
        i--;

        wcout << "SizeFile: " << (size >> 10 * i) << " " << measures[i] << endl;

        SYSTEMTIME stime;
        FileTimeToSystemTime(&info.ftCreationTime, &stime);

        wcout << "Created: " << stime.wHour << ":" << stime.wMinute << " "
          << stime.wDay << "." << stime.wMonth << "." << stime.wYear << endl;
      
}

void PrDirectory(LPCTSTR cdir) {

  LPTSTR dir = new TCHAR[MAX_BUFFER];
  memcpy(dir, cdir, MAX_BUFFER * sizeof(TCHAR));

  wcscat(dir, TEXT("/*.*"));
  WIN32_FIND_DATA fd;
  HANDLE hFind = ::FindFirstFile(dir, &fd);
  if (hFind != INVALID_HANDLE_VALUE) {

    do {
      // read all (real) files in current folder
      // , delete '!' read other 2 default folder . and ..
      //do printing
      wcout << left << setw(FILE_NAME) << fd.cFileName << " ";

      unsigned long long sizeOfFile = (unsigned long long)fd.nFileSizeHigh << 32 | fd.nFileSizeLow;
      LPCTSTR measures[] = { TEXT("B"), TEXT("KB") ,TEXT("MB") ,TEXT("GB") };
      INT i = 0;
      while (sizeOfFile >> 10 * ++i);
      i--;

      wcout << left << setw(DISP_STEP);
      if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
   
        TCHAR buf[50];
        memset(buf, 0, 100);
        swprintf(buf, TEXT("%d "), (sizeOfFile >> 10 * i));
        wcscat(buf, measures[i]);
        wcout << buf;
      }
      else {
        wcout << "<dir>";
      }

      SYSTEMTIME stime;
      FileTimeToSystemTime(&fd.ftCreationTime, &stime);

      wcout<< stime.wDay << "." << stime.wMonth << "." << stime.wYear;
      wcout << endl;

    } while (::FindNextFile(hFind, &fd));
    FindClose(hFind);
  }
  else {
    wcout << "Error code  :" << GetLastError() << endl;
  }
  delete dir;
}
//Print directory 
void PrintDir(LPTSTR dirPath) {
  if (!GetCurrentDirectory(MAX_BUFFER, dirPath)) {
    throw;
  }
  PrDirectory(dirPath);
}
int _tmain(INT argc, TCHAR** argv)
{
  // enabling all Unicode chars in console
  _setmode(_fileno(stdout), _O_U16TEXT);

  LPTSTR dirPath = new TCHAR[MAX_BUFFER], src = new TCHAR[MAX_BUFFER], pfile = new TCHAR[MAX_BUFFER];
  LPTSTR com = new TCHAR[MAX_BUFFER];
  LPTSTR input = new TCHAR[MAX_BUFFER];
  int operation;
  memset(src, 0, MAX_BUFFER * sizeof(TCHAR));
  memset(dirPath, 0, MAX_BUFFER * sizeof(TCHAR));
  memset(input, 0, MAX_BUFFER * sizeof(TCHAR));
  memset(com, 0, MAX_BUFFER * sizeof(TCHAR));
  memset(pfile, 0, MAX_BUFFER * sizeof(TCHAR));
  wcout << "\t <File Manager>" << endl;
  wcout << " Create dirirectory : mkdir name  " << endl; // crdir
  wcout << " Change current directory : cd dirPath - >" << endl;//chdir
  wcout << " Information about directory : infdir" << endl;
  wcout << " Print current directory : ls - " << endl;//prdir
  wcout << " Copy file : cp  dirPath src " << endl;
  wcout << " Delete file or empty directory : rm name - " << endl;
  wcout << "Full info about file : inffile name - " << endl;
  while (TRUE) {
    try {
      int k =0 ;
      if (!fgetws(input, MAX_BUFFER, stdin)) {
        continue;
      }
      if (!SplitInputAndDecode(input, com, dirPath, src, pfile)) {
        continue;
      }

       int i = 0;
       string strCommand;
      while (com[i]!= '\0')
      {
        strCommand += com[i];
        i++;
      }
      if (strCommand == "mkdir") {
        CreateDir(dirPath);
        continue;
      }
      else if (strCommand == "cd") {
        ChangeDir(dirPath);
      }
      else if (strCommand == "ls") {

        DirectoryInformaion(dirPath);
      }
      else if (strCommand == "cp") {
        Copyfile(dirPath, src);
      }
      else if (strCommand == "rm") {
        DeleteFileOrDirectory(dirPath);
      }
      else if (strCommand == "infdir") {
        PrintDir(dirPath);
      }
      else if (strCommand == "inffile") {
        FileInf(dirPath);
      }
      else {
        wcout << L"Command doesn't exist." << endl;
      }
    }
    catch (INT e) {
      switch (e) {
      case 1:
        wcout << "Source folder not  found." << endl;
        break;
      case 2:
        wcout << "Destination folder not found." << endl;
        break;
      }
    }

    catch (DWORD e) {
      LPTSTR message = new TCHAR[MAX_BUFFER];
      FormatMessage(
        // use system message tables to retrieve error text
        FORMAT_MESSAGE_FROM_SYSTEM
        // allocate buffer on local heap for error text
        | FORMAT_MESSAGE_ALLOCATE_BUFFER
        | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, e,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPWSTR)&message,
        0, NULL);
      wcout << L"Error: " << message << endl;
    }
  }


  getchar();
  return 0;
}