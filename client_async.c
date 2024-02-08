#undef UNICODE
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>

#define BUFSIZE 512

VOID CALLBACK WriteCompletion(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped);
VOID CALLBACK ReadCompletion(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped);

int main(int argc, TCHAR* argv[])
{
	HANDLE hPipe;
	TCHAR chBuf[BUFSIZE];
	BOOL fSuccess;
	DWORD cbRead, cbWritten, dwMode;
	LPTSTR lpszPipename = TEXT("\\\\.\\pipe\\mynamedpipe");
	OVERLAPPED ovWrite = { 0 };
	OVERLAPPED ovRead = { 0 };

	printf("Enter a number: ");
	int n;
	scanf("%d", &n);

	TCHAR lpvMessage[32]; // Assuming a maximum of 32 characters for the number
	sprintf(lpvMessage, "%d", n);

	while (1)
	{
		hPipe = CreateFile(
			lpszPipename,
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			OPEN_EXISTING,
			FILE_FLAG_OVERLAPPED,
			NULL);

		if (hPipe != INVALID_HANDLE_VALUE)
			break;
		if (GetLastError() != ERROR_PIPE_BUSY)
		{
			printf("Nu pot deschide pipe!");
			return 0;
		}

		if (!WaitNamedPipe(lpszPipename, 20000))
		{
			printf("timeout nu pot deschide pipe");
			return 0;
		}
	}

	dwMode = PIPE_READMODE_MESSAGE;
	fSuccess = SetNamedPipeHandleState(
		hPipe,
		&dwMode,
		NULL,
		NULL);

	if (!fSuccess)
	{
		printf("Eroare la setarea pipe-ului");
		return 0;
	}

	// Send message asynchronously
	WriteFileEx(
		hPipe,
		lpvMessage,
		(lstrlen(lpvMessage) + 1) * sizeof(TCHAR),
		&ovWrite,
		WriteCompletion);

	// Read message asynchronously
	ReadFileEx(
		hPipe,
		chBuf,
		BUFSIZE * sizeof(TCHAR),
		&ovRead,
		ReadCompletion);

	// Wait for input to terminate
	_getch();

	CloseHandle(hPipe);
	return 0;
}

VOID CALLBACK WriteCompletion(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped)
{
	if (dwErrorCode == 0)
		printf("Message sent successfully.\n");
	else
		printf("Error sending message: %d\n", dwErrorCode);
}

VOID CALLBACK ReadCompletion(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped)
{
	if (dwErrorCode == 0)
		printf("Received message: %s\n", (TCHAR*)lpOverlapped->hEvent);
	else
		printf("Error receiving message: %d\n", dwErrorCode);
}