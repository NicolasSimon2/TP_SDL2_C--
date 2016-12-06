#include "prototypes.h"




int initSockClient(sock_client *mSocket) {

	int iResult;
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &mSocket->wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	ZeroMemory(&mSocket->hints, sizeof(mSocket->hints));
	mSocket->hints.ai_family = AF_UNSPEC;
	mSocket->hints.ai_socktype = SOCK_STREAM;
	mSocket->hints.ai_protocol = IPPROTO_TCP;

	return 1;

}

int resolveSocket(sock_client *mSocket, char *ip[]) {

	int iResult;

	iResult = getaddrinfo(ip[1], DEFAULT_PORT, &mSocket->hints, &mSocket->result);//récupère l ip via argument passé dans le programme
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	return 1;
}

int createAndConnectSocket(sock_client *mSocket) {

	int iResult;
	// Attempt to connect to an address until one succeeds
	for (mSocket->ptr = mSocket->result; mSocket->ptr != NULL;mSocket->ptr = mSocket->ptr->ai_next) {

		// Create a SOCKET for connecting to server
		mSocket->ConnectSocket = socket(mSocket->ptr->ai_family, mSocket->ptr->ai_socktype,
			mSocket->ptr->ai_protocol);
		if (mSocket->ConnectSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}

		// Connect to server.
		iResult = connect(mSocket->ConnectSocket, mSocket->ptr->ai_addr, (int)mSocket->ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(mSocket->ConnectSocket);
			mSocket->ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(mSocket->result);

	if (mSocket->ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}

		return 1;

}


void recvXYBySocket(sock_client *mSocket) {

	char recvbuf[DEFAULT_BUFLEN];
	int iResult;
	int recvbuflen = DEFAULT_BUFLEN;
	char bufxy[DEFAULT_BUFLEN];
	char valx[DEFAULT_BUFLEN];
	char valy[DEFAULT_BUFLEN];
	int i = 0;
	int j = 1;
	int c = 0;
	int valconvX;
	int valconvY;


	
	//char *bufx=malloc (sizeof (*bufx)*DEFAULT_BUFLEN); //chaine ayant un max de 511 caractère
	//char *bufy= malloc(sizeof(*bufy)*8);//chaine ayant un max de 7 caractère

	do {

		iResult = recv(mSocket->ConnectSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0) {
			//printf("Bytes received: %d\n", iResult);
			while (i < iResult) {
				
				
				if (recvbuf[i] == 'x') {

					while (recvbuf[i] != 'y' && recvbuf[i + 1] != 'y' && i<DEFAULT_BUFLEN) {

						bufxy[i] = recvbuf[j];
				
						if (isdigit(*bufxy)) {

							valx[c] = bufxy[i];
							c++;

						}
						i++;
						j++;
					

					}		
					valconvX = atoi(valx);
					printf("%d\n", valconvX);
					i++;
					j++;
					c = 0;
					
					
				}
				if (recvbuf[i] == 'y') {

					while (recvbuf[i] != 'x' && recvbuf[i + 1] != 'x'&& i<DEFAULT_BUFLEN) {

						bufxy[i] = recvbuf[j];
				
							if(isdigit(*bufxy)) {

								valy[c] = bufxy[i];
								c++;
							}
						
					
							i++;
							j++;
						
						
						}
						valconvY = atoi(valy);
						printf("%d\n", valconvY);
						i++;
						j++;
						c = 0;
					
					}
	

				}
			}

		} while (iResult > 0);


		

		closesocket(mSocket->ConnectSocket);
		WSACleanup();


	}





