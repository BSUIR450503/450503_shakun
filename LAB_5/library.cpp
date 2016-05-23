#include "libs.h"

#ifdef _WIN32
#ifdef __cplusplus 
extern "C" {         
#endif

	struct OperationInfo
	{
		HANDLE hFile;             
		DWORD NumberOfBytes;	  
		CHAR  buf[100];			  
		DWORD  pos_out_file;	  
		OVERLAPPED Overlapped;
	} info;

__declspec(dllexport) BOOL read(OperationInfo *info)
{
	BOOL readResult;
	DWORD Num;
	info->Overlapped.Offset = 0;

	ReadFile(info->hFile, info->buf, info->NumberOfBytes, NULL, &info->Overlapped);
	WaitForSingleObject(info->Overlapped.hEvent, 1000);
	readResult = GetOverlappedResult(info->hFile, &info->Overlapped, &Num, FALSE);

	return readResult;
}
__declspec(dllexport) BOOL write(OperationInfo *info)
{
	BOOL writeResult;
	DWORD NumberOfBytesTrasferred;
	info->Overlapped.Offset = info->pos_out_file;

	WriteFile(info->hFile, info->buf, info->Overlapped.InternalHigh, NULL, &info->Overlapped);
	WaitForSingleObject(info->Overlapped.hEvent, 1000);
	writeResult = GetOverlappedResult(info->hFile, &info->Overlapped, &NumberOfBytesTrasferred, FALSE);

	if (writeResult) info->pos_out_file = info->pos_out_file + NumberOfBytesTrasferred;
	return writeResult;
}

#ifdef __cplusplus
}
#endif
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


void asyncronicRead(struct FileInfo *fileInfo) {
  printf("\n\nReading from %s...\n",fileInfo->readFileName);

  int file = open(fileInfo->readFileName, O_RDONLY, 0);
  if (file == -1) {
    printf("Can't to open file for read!");
    return;
  }

  fileInfo->readCb.aio_nbytes = sizeof(fileInfo->buffer);
  fileInfo->readCb.aio_fildes = file;
  fileInfo->readCb.aio_offset = 0;
  fileInfo->readCb.aio_buf = fileInfo->buffer;
  
  aio_read(&fileInfo->readCb);
  while(aio_error(&fileInfo->readCb) == EINPROGRESS);
  fileInfo->bytesRead = aio_return(&fileInfo->readCb);
  close(file);
}

void asyncronicWrite(struct FileInfo *fileInfo) {

 int file = open(fileInfo->writeFileName, O_CREAT | O_RDWR | O_APPEND, 0666);
  if (file == -1) {
    printf("Can't to open file for write!");
    return;
  }

  fileInfo->writeCb.aio_nbytes = fileInfo->bytesRead;
  fileInfo->writeCb.aio_fildes = file;
  fileInfo->writeCb.aio_buf = fileInfo->buffer;
  
  aio_write(&fileInfo->writeCb);
  while(aio_error(&fileInfo->writeCb) == EINPROGRESS);
  fileInfo->bytesWrite = aio_return(&fileInfo->writeCb);
  close(file);
}
#endif