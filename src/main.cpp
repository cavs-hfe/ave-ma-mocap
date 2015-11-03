// Disable linker warning about truncation to 255 characters in debug info with std::string
#pragma warning (disable: 4786)

//   Standard include headers
#define _WIN32_WINNT 0x0400 // this is required to use the TryEnterCriticalSection() function

#include <WinSock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <iomanip>

//  EVaRT SDK headers
#include "EVaRT.h"

// Our project headers
#include "wrappers.h"
#include "fifo.h"
#include "recorders.h"
#include "utils.h"

// Prototypes for local functions
static int EVaRT_Data_Handler(int DataType, void *Data); // EVaRT SDK thread calls this function
static int Handle_Error(const char * msg, int code);

//  Constants
#define DEFAULT_HOST			"localhost"				// EVaRT host machine
#define DEFAULT_ITERATIONS		"10000"					// number of iterations to perform

//  Globals
static CRITICAL_SECTION		gCriticalSection;			// Windows critical section object
static TrcRecorder*			gTrcRecorder = NULL;
static bool gGotMarkerList = false;

//socket used for communicating with PedSim server
SOCKET ConnectSocket = INVALID_SOCKET;

// Entry point
int main(int argc, char* argv[])
{
	char	lHost[80];
	int		lDataTypes;
	int		lNumTypes = 0;

	//if we are passed a host as an argument, use it. otherwise prompt
	if (argc == 2) {
		strcpy(lHost, argv[1]);
	}
	else {
		printf("\n\nPress <Enter> to accept default values\n\n");
		promptInput("Enter host machine", DEFAULT_HOST, lHost, 80);
	}

	//connect socket
	WSADATA wsaData;

	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ConnectSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	struct sockaddr_in clientService;
	clientService.sin_family = AF_INET;
	clientService.sin_addr.s_addr = inet_addr("192.168.1.2");
	clientService.sin_port = htons(8888);

	// Connect to server.
	iResult = connect(ConnectSocket, (SOCKADDR*)&clientService, sizeof(clientService));
	if (iResult == SOCKET_ERROR) {
		closesocket(ConnectSocket);
		printf("Unable to connect to server: %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	// Send an initial buffer
	char *sendbuf = "mocaps";
	iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	// Determine which data types will be streamed
	lDataTypes = 0;
	lDataTypes |= TRC_DATA;
	lNumTypes++;

	// Initialize the Windows critical section object
	InitializeCriticalSection(&gCriticalSection);

	// Initialize EVaRT SDK, only call this function once
	EVaRT_Initialize();

	// Tell EVaRT what function to call when it has something to send us
	// The callback function is called by the EVaRT SDK thread, not our main thread
	EVaRT_SetDataHandlerFunc(EVaRT_Data_Handler);

	// Make a connection to EVaRT running on the specified host
	// Connect/Disconnect can be called multiple times
	if (Handle_Error("EVaRT_Connect", EVaRT_Connect(lHost)) == OK)
	{
		// Make sure we're connected
		if (EVaRT_IsConnected())
		{
			TimeoutTimer t(2.0);  // two second time out

			// Get the marker list if streaming TRC data
			if (lDataTypes & TRC_DATA)
			{
				// The marker list is provided through our callback function set above
				EVaRT_RequestMarkerList();

				t.Begin();
				while (!t.IsExpired() && !gGotMarkerList)
				{
					Sleep(10);
				}

				if (!gGotMarkerList)	printf("Did not get a marker list\n");
			}

			printf("\n\n");

			// Tell EVaRT which data types to send us
			if (Handle_Error("EVaRT_SetDataTypesWanted", EVaRT_SetDataTypesWanted(lDataTypes)) == OK)
			{

				// Tell EVaRT to start sending us frames of data
				// Our callback function will get called once for each type of data we are streaming for each frame
				Handle_Error("EVaRT_StartStreaming", EVaRT_StartStreaming());

				while (true)
				{
					Sleep(10); // Not required, but otherwise CPU will be at 100%
				}

				// Ignore any more data from EVaRT
				EnterCriticalSection(&gCriticalSection);

				// Tell EVaRT to stop sending frames.
				// Because of network latency, our callback function may still get called however.
				// That is why we use the critical section object.
				Handle_Error("EVaRT_StopStreaming", EVaRT_StopStreaming());

				LeaveCriticalSection(&gCriticalSection);

				// shutdown the connection since no more data will be sent
				iResult = shutdown(ConnectSocket, SD_SEND);
				if (iResult == SOCKET_ERROR) {
					printf("shutdown failed with error: %d\n", WSAGetLastError());
					closesocket(ConnectSocket);
					WSACleanup();
					return 1;
				}

				char recvbuf[512];
				int recvbuflen = 512;
				// Receive until the peer closes the connection
				do {

					iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
					if (iResult > 0)
						printf("Bytes received: %d\n", iResult);
					else if (iResult == 0)
						printf("Connection closed\n");
					else
						printf("recv failed with error: %d\n", WSAGetLastError());

				} while (iResult > 0);

				// cleanup
				closesocket(ConnectSocket);
				WSACleanup();
			}
			else
			{
				printf("Error setting data types wanted...Exiting\n");
			}

			// Disconnect from EVaRT
			EVaRT_Disconnect();
		}
		else
		{
			printf("EVaRT_Connect returned OK, but not connected...Exiting\n");
		}
	}
	else
	{
		printf("Could not connect to EVaRT host %s...Exiting\n", lHost);
	}

	// Shutdown EVaRT SDK, only call this once
	EVaRT_Exit();
	DeleteCriticalSection(&gCriticalSection);

	printf("\n\n");
	system("pause");
	return 0;
}



// Callback function which is called by the EVaRT SDK thread
// The void * pointer is a pointer to data in the EVaRT SDK library (dll)
// DON'T WRITE TO THIS MEMORY!
static int EVaRT_Data_Handler(int DataType, void *Data)
{
	static int numMarkers = 0;
	static int iResult = 0;
	std::ostringstream stringStream;
	std::string copyOfStr;

	// Example of how you could protect global data in your main thread
	if (TryEnterCriticalSection(&gCriticalSection) == 0)
	{
		return 0;
	}

	switch (DataType)
	{
		case MARKER_LIST:
		{
			sMarkerList *p = (sMarkerList *)Data;

			if (p->nMarkers > 0)
			{
				gGotMarkerList = true;
				numMarkers = p->nMarkers;
			}
		}
		break;
		case TRC_DATA:
		{
			TrcFrameWrapper f((sTrcFrame *)Data, numMarkers);
			Point3 pt;

			f.GetMarkerLocation(0, pt);
			fprintf(stderr, "%f, %f, %f\n", pt[0], pt[1], pt[2]);

			stringStream.clear();
			stringStream.str(std::string());
			stringStream << "head," << pt[0] << "," << pt[1] << "," << pt[2] << "\n";
			copyOfStr = stringStream.str();
			iResult = send(ConnectSocket, copyOfStr.c_str(), copyOfStr.length(), 0);
			if (iResult == SOCKET_ERROR) {
				printf("send failed with error: %d\n", WSAGetLastError());
			}
		}
		break;
	}
	LeaveCriticalSection(&gCriticalSection);
	return 0;
}

// Print error messages from calling EVaRT SDK functions
static int Handle_Error(const char * msg, int code)
{
	if (code != OK)
	{
		switch (code)
		{
		case ERRFLAG:
			printf("%s: General Error\n", msg);
			break;
		case API_ERROR:
			printf("%s: API Error\n", msg);
			break;
		case NETWORK_ERROR:
			printf("%s: Network Error\n", msg);
			break;
		case FILE_ERROR:
			printf("%s: File Error\n", msg);
			break;
		default:
			printf("%s: Unknown Error\n", msg);
			break;
		}
	}
	return code;
}