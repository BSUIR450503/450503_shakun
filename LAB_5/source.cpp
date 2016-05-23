#include "libs.h"

#ifdef _WIN32
struct OperationInfo
{
	HANDLE hFile;            
	DWORD NumberOfBytes;	  
	CHAR  buf[100];			  
	DWORD  pos_out_file;	  
	OVERLAPPED Overlapped;
} info;

HINSTANCE library;
HANDLE events[3];


DWORD WINAPI ReaderThread(PVOID folderPATH)
{
	string folder(((const char*)folderPATH));               
	folder.append("\\");
	string fileMask = folder + "in?.txt";   
	char ReadFilePATH[MAX_PATH];

	BOOL(*Read)(OperationInfo*) = (BOOL(*)(OperationInfo*))GetProcAddress(library, "read"); 

	WIN32_FIND_DATA FindFileData; 
	HANDLE find_Handle,  
		hReadFile;    

	BOOL readResult = false;

	find_Handle = FindFirstFile(fileMask.c_str(), &FindFileData);

	if (find_Handle == INVALID_HANDLE_VALUE)
	{
		printf(" Error: %d\n", GetLastError());
		return 0;
	}

	while (1)
	{
		WaitForSingleObject(events[WRITE], INFINITE);

		strcpy(ReadFilePATH, folder.c_str());
		strcat(ReadFilePATH, FindFileData.cFileName);  
		hReadFile = CreateFile(ReadFilePATH, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);

		info.hFile = hReadFile;
		printf("Reading from %s...\n", ReadFilePATH);
		readResult = (Read)(&info);

		if (readResult)
			SetEvent(events[READ]);
		else
			printf("Error while read from %s!\n", ReadFilePATH);
		if (FindNextFile(find_Handle, &FindFileData))
		{
			CloseHandle(hReadFile);
			continue;
		}
		else break;
	}
	FindClose(find_Handle);
	CloseHandle(hReadFile);
	SetEvent(events[EXIT]);
	return 0;
}

DWORD WINAPI WriterThread(PVOID outFilePath)
{
	string file(((const char*)outFilePath));               
	file.append("\\out.txt");
	HANDLE hOutputFile = CreateFile(file.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_FLAG_OVERLAPPED, NULL);
	HANDLE forwait[2] = { events[READ], events[EXIT] };
	BOOL(*Write)(OperationInfo*) = (BOOL(*)(OperationInfo*))GetProcAddress(library, "write");
	while (1)
	{
		int event = WaitForMultipleObjects(2, forwait, FALSE, INFINITE) - WAIT_OBJECT_0;
		if (event == EXIT)
			break;
		info.hFile = hOutputFile;
		printf("Writing to %s...\n", file.c_str());
		(Write)(&info);
		SetEvent(events[WRITE]);
	}
	CloseHandle(hOutputFile);
	return 0;
}

 int main(int argc, char *argv[])
 {

	 HANDLE hEvent;        
	 HANDLE hThreads[2];   
	 char path[MAX_PATH];

	 hEvent = CreateEvent(NULL, FALSE, TRUE, TEXT("event"));  

	 events[WRITE] = CreateEvent(NULL, FALSE, TRUE, NULL);   
	 events[READ] = CreateEvent(NULL, FALSE, FALSE, NULL);  
	 events[EXIT] = CreateEvent(NULL, TRUE, FALSE, NULL); 

	 info.Overlapped.Offset = 0;
	 info.Overlapped.OffsetHigh = 0;
	 info.Overlapped.hEvent = hEvent;
	 info.pos_out_file = 0;
	 info.NumberOfBytes = sizeof(info.buf);

	 library = LoadLibrary("dll lib.dll");

	 printf("Enter path to dir: ");
	 gets(path);	 

	 hThreads[0] = CreateThread(NULL, 0, ReaderThread, (LPVOID)path, 0, NULL); 
	 hThreads[1] = CreateThread(NULL, 0, WriterThread, (LPVOID)path, 0, NULL); 

	 WaitForMultipleObjects(2, hThreads, TRUE, INFINITE);

	 CloseHandle(hThreads[0]);
	 CloseHandle(hThreads[1]);

	 CloseHandle(events[WRITE]);
	 CloseHandle(events[READ]);
	 CloseHandle(events[EXIT]);
	 CloseHandle(hEvent);
	 FreeLibrary(library);
	 printf("\n\n");
	 system("pause");
	 return 0;
 }
 #endif

 #ifdef __linux__
 struct FileInfo {
  char readFileName[500];
  char writeFileName[500];
  struct aiocb readCb;
  struct aiocb writeCb;
  char buffer[300];
  int bytesRead;
  int bytesWrite;
};

char fileNames[20][100];
struct FileInfo fileInfo;
pthread_t readThread;
pthread_t writeThread;
int numberOfFiles = 0;
pthread_mutex_t mutex;

void (*asyncronicWrite)(struct FileInfo *fileInfo);
void (*asyncronicRead)(struct FileInfo *fileInfo);

void findAllFiles() {
	struct dirent *dp;  
    DIR *dirp;     
    dirp = opendir("/home/thandor/Test"); 
    puts("Input files in directory: ");
    while ((dp = readdir(dirp)) != NULL) {  
        if(strstr(dp->d_name, "in") != NULL) {  
            strcpy(fileNames[numberOfFiles], dp->d_name);
            numberOfFiles++;
            puts(dp->d_name);
        }
    }  
}

void *readFunc(void * arg){
	void *ext_library;
	ext_library = dlopen("/home/thandor/lib.so",RTLD_LAZY);
	asyncronicRead = (void(*)(struct FileInfo *fileInfo)) dlsym(ext_library, "asyncronicRead");
	for(int i = 0; i < numberOfFiles; i++) {
		pthread_mutex_lock(&mutex);
		strcpy(fileInfo.readFileName, fileNames[i]);
		(*asyncronicRead)(&fileInfo);
		pthread_mutex_unlock(&mutex);
		puts("Wait for write...");
		usleep(100000);
	}
	dlclose(ext_library); 
	puts("\nDone!");
	return NULL;
}

void *writeFunc(void * arg) {
	usleep(10000);
	strcpy(fileInfo.writeFileName, "/home/thandor/Test/out.txt");
	void *ext_library;
	ext_library = dlopen("/home/thandor/lib.so",RTLD_LAZY);
	asyncronicWrite = (void(*)(struct FileInfo *fileInfo)) dlsym(ext_library, "asyncronicWrite");
	for(int i = 0; i < numberOfFiles; i++) {
		pthread_mutex_lock(&mutex);
		puts("Writing...");
		(*asyncronicWrite)(&fileInfo);
		pthread_mutex_unlock(&mutex);
		puts("Wait for read...");
		usleep(100000);
	}
	dlclose(ext_library);
	return NULL;
}

int main() {
  if(pthread_mutex_init(&mutex, NULL))
	{
	 	printf("Can't create mutex");
	 	return 0;
	}
  remove("/home/thandor/Test/out.txt");
  
  findAllFiles();

  pthread_create(&readThread, NULL, readFunc, NULL);
  pthread_create(&writeThread, NULL, writeFunc, NULL);

  pthread_join(readThread, NULL);
  pthread_join(writeThread, NULL);

  return 0;
}
  #endif