/*#undef UNICODE
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

	// Create event objects for overlapped I/O
	ovWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (ovWrite.hEvent == NULL)
	{
		printf("Failed to create event object for write operation.\n");
		return 0;
	}

	ovRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (ovRead.hEvent == NULL)
	{
		printf("Failed to create event object for read operation.\n");
		CloseHandle(ovWrite.hEvent); // Cleanup if creation fails
		return 0;
	}

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
			CloseHandle(ovWrite.hEvent); // Cleanup
			CloseHandle(ovRead.hEvent);  // Cleanup
			return 0;
		}

		if (!WaitNamedPipe(lpszPipename, 20000))
		{
			printf("timeout nu pot deschide pipe");
			CloseHandle(ovWrite.hEvent); // Cleanup
			CloseHandle(ovRead.hEvent);  // Cleanup
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
		CloseHandle(ovWrite.hEvent); // Cleanup
		CloseHandle(ovRead.hEvent);  // Cleanup
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
	WaitForSingleObject(ovWrite.hEvent, INFINITE);// prev there was _getch();

	//Cleanup
	CloseHandle(hPipe);
	CloseHandle(ovWrite.hEvent);
	CloseHandle(ovRead.hEvent);

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
	{
		if (dwErrorCode == 0)
		{
			OVERLAPPED* pOverlapped = lpOverlapped;
			printf("Received message from server: %s\n", (TCHAR*)pOverlapped->hEvent);

			// No cleanup necessary
		}
		else
			printf("Error receiving message: %d\n", dwErrorCode);
	}
}
*/
#undef UNICODE
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>

#define BUFSIZE 4096

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

    // Create event objects for overlapped I/O
    ovWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (ovWrite.hEvent == NULL)
    {
        printf("Failed to create event object for write operation.\n");
        return 0;
    }

    ovRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (ovRead.hEvent == NULL)
    {
        printf("Failed to create event object for read operation.\n");
        CloseHandle(ovWrite.hEvent); // Cleanup if creation fails
        return 0;
    }

    printf("Enter a number: ");
    int n;
    scanf("%d", &n);


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
            CloseHandle(ovWrite.hEvent); // Cleanup
            CloseHandle(ovRead.hEvent);  // Cleanup
            return 0;
        }

        if (!WaitNamedPipe(lpszPipename, 20000))
        {
            printf("timeout nu pot deschide pipe");
            CloseHandle(ovWrite.hEvent); // Cleanup
            CloseHandle(ovRead.hEvent);  // Cleanup
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
        CloseHandle(ovWrite.hEvent); // Cleanup
        CloseHandle(ovRead.hEvent);  // Cleanup
        return 0;
    }

 
    //Trimite numere catre server
    for (int i = 0; i <= n; i++)
    {
        if (i < n)
        {
            TCHAR lpvMessage[32]; // Assuming a maximum of 32 characters for the number
            sprintf(lpvMessage, "%d", i);
            fSuccess = WriteFile(
                hPipe,
                lpvMessage,
                (lstrlen(lpvMessage) + 1) * sizeof(TCHAR),
                &cbWritten,
                &ovWrite);



            if (!fSuccess)
            {
                printf("Error sending message: %d\n", GetLastError());
                CloseHandle(ovWrite.hEvent); // Cleanup
                CloseHandle(ovRead.hEvent);  // Cleanup
                return 0;
            }
            else
            {
                printf(TEXT("Am trimis catre server : %s\n"), lpvMessage);
            }
        }

        // Citire mesaj
        fSuccess = ReadFile(
            hPipe,
            chBuf,
            BUFSIZE * sizeof(TCHAR),
            &cbRead,
            &ovRead);


            printf(TEXT("Am primit de la server raspunsul : %s\n"), chBuf);


        if (!fSuccess && GetLastError() != ERROR_IO_PENDING)
        {
            printf("Error receiving message: %d\n", GetLastError());
            CloseHandle(ovWrite.hEvent); // Cleanup
            CloseHandle(ovRead.hEvent);  // Cleanup
            return 0;
        }



        // Wait for the read completion event
        WaitForSingleObject(ovRead.hEvent, INFINITE);

    }

    // Cleanup
    CloseHandle(hPipe);
    CloseHandle(ovWrite.hEvent);
    CloseHandle(ovRead.hEvent);

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
    {
        printf("Received message from server: %s\n", (TCHAR*)lpOverlapped->hEvent);
    }
    else
        printf("Error receiving message: %d\n", dwErrorCode);
}