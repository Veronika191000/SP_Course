#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include <locale.h>

#define BUFFER_SIZE 1024
#define MAXFILENUMBERS 8



bool unzippingFiles()
{
  bool result = true;
  LPSTR inputFileName = new CHAR[MAX_PATH];
  LPSTR outputFileName = new CHAR[MAX_PATH];
  printf("Full path to archive: ");
  scanf("%s", inputFileName);
  printf("Full output path: ");
  scanf("%s", outputFileName);
  LPSTR cmdLine = new CHAR[MAX_PATH];
  ZeroMemory(cmdLine, MAX_PATH);
  strncpy(cmdLine, "7z.exe zip ", MAX_PATH - strlen(cmdLine));
  strncat(cmdLine, inputFileName, MAX_PATH - strlen(cmdLine));
  strncat(cmdLine, " -o", MAX_PATH - strlen(cmdLine));
  strncat(cmdLine, outputFileName, MAX_PATH - strlen(cmdLine));
  strncat(cmdLine, " -y", MAX_PATH - strlen(cmdLine));
  STARTUPINFOA startInfo;
  ZeroMemory(&startInfo, sizeof(startInfo));
  startInfo.cb = sizeof(startInfo);
  startInfo.dwFlags |= STARTF_USESTDHANDLES;
  PROCESS_INFORMATION procInfo;
  ZeroMemory(&procInfo, sizeof(procInfo));
  if (!CreateProcessA(NULL, cmdLine, NULL, NULL, TRUE, NORMAL_PRIORITY_CLASS, NULL, NULL, &startInfo, &procInfo)) {
    result = false;
  }
  return result;
}
bool archivingFiles()
{
  bool result = true;
  LPSTR* inputFiles = new CHAR *[MAXFILENUMBERS];
  int fileCount;
  printf("Number of files: ");
  scanf("%d", &fileCount);
  printf("Full path to %d files\n", fileCount);
  LPSTR fileName = new CHAR[MAX_PATH];
  for (int i = 0; i < fileCount; i++) {
    printf("Full path to file:");
    scanf("%s", fileName);
    inputFiles[i] = new CHAR[MAX_PATH];
    strcpy(inputFiles[i], fileName);
  }
  LPSTR archiveName = new CHAR[MAX_PATH];
  printf("Full path to archive: ");
  scanf("%s", archiveName);
  // Create anonim channel
  HANDLE hInReadPipe;
  HANDLE hInWritePipe;
  HANDLE hOutReadPipe;
  HANDLE hOutWritePipe;
  CreatePipe(&hInReadPipe, &hInWritePipe, NULL, 0);
  CreatePipe(&hOutReadPipe, &hOutWritePipe, NULL, 0);
  if (hInReadPipe == INVALID_HANDLE_VALUE || hInWritePipe == INVALID_HANDLE_VALUE ||
    hOutReadPipe == INVALID_HANDLE_VALUE || hOutWritePipe == INVALID_HANDLE_VALUE) {
    /*  print_problems();*/
    result = false;
  }
  STARTUPINFOA startInfo;
  ZeroMemory(&startInfo, sizeof(startInfo));
  startInfo.cb = sizeof(startInfo);	// Initialize size of structure in Byte
  startInfo.hStdInput = hInReadPipe;	//Defines an input descriptor for a process
  startInfo.hStdOutput = hOutWritePipe;	//Defines an input descriptor for a process
  startInfo.hStdError = hOutWritePipe;	//Defines an error descriptor for a process
  startInfo.dwFlags |= STARTF_USESTDHANDLES;
  // Set standard input, standard output, and standard error handling
  PROCESS_INFORMATION procInfo;
  ZeroMemory(&procInfo, sizeof(procInfo));
  LPSTR cmdLine = new CHAR[MAX_PATH];
  ZeroMemory(cmdLine, MAX_PATH);
  // a (Add) command
  strncpy(cmdLine, "7z.exe zip ", MAX_PATH - strlen(cmdLine));
  strncat(cmdLine, archiveName, MAX_PATH - strlen(cmdLine));
  for (int i = 0; i < fileCount; i++)
  {
    strncat(cmdLine, " ", MAX_PATH - strlen(cmdLine));
    strncat(cmdLine, inputFiles[i], MAX_PATH - strlen(cmdLine));
  }
  strncat(cmdLine, " ", MAX_PATH - strlen(cmdLine));
  //Create new process.
  if (!CreateProcessA(NULL, cmdLine, NULL, NULL, TRUE, NORMAL_PRIORITY_CLASS, NULL, NULL, &startInfo, &procInfo)) {
    //print_problems();
    result = false;
  }
  if (result) {
    WaitForSingleObject(procInfo.hProcess, INFINITE);
    DWORD readBytes = 0;
    LPSTR outBuffer = new CHAR[BUFFER_SIZE];
    ZeroMemory(outBuffer, BUFFER_SIZE);
    OVERLAPPED overlapped;
    ReadFile(hOutReadPipe, outBuffer, BUFFER_SIZE, &readBytes, &overlapped);
    CloseHandle(procInfo.hProcess);
    CloseHandle(procInfo.hThread);
  }
  CloseHandle(hOutReadPipe);
  CloseHandle(hOutWritePipe);
  CloseHandle(hInReadPipe);
  CloseHandle(hInWritePipe);
  return result;
}
int main(int argc, char* argv[])
{
  if (((strcmp(argv[1], "-zip") != 0) && (strcmp(argv[1], "-unzip")) != 0)) {
    printf("\n Something went wrong :(\n");
    exit(0);
  }
  if (strcmp(argv[1], "-zip") == 0) {
    if (archivingFiles())
      printf("Your files have been archived :)");
    else printf("Your files haven't been archived :(");
  }
  else {
    if (unzippingFiles())
      printf("The files were unziped.");
    else printf("The files were not unziped.");
  }
  return 0;
}
