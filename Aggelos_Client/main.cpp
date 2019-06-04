#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <fstream>

#pragma comment (lib, "ws2_32.lib")

using namespace std;

int main() {
	string ipAddress = "127.0.0.1";		//IP serwera
	int port = 54000;					//port nasluchowy serwera

	//Inicjacja winsocka
	WSADATA winsockData;
	WORD ver = MAKEWORD(2, 2);

	int winsockOK = WSAStartup(ver, &winsockData);
	if (winsockOK != 0) {
		cerr << "Error with initialazing WinSock. Bye!" << endl;
		return 1;
	}

	//Otwarcie socketu
	SOCKET sockt = socket(AF_INET, SOCK_STREAM, 0);
	if (sockt == INVALID_SOCKET) {
		cerr << "Error with creating socket. Bye!" << endl;
		WSACleanup();
		return 1;
	}

	//Powiazanie socketu z IP i portem
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	//Podlaczenie do serwera
	int connectionResult = connect(sockt, (sockaddr*)&hint, sizeof(hint));
	if (connectionResult == SOCKET_ERROR) {
		cerr << "Error with connecting server. Bye!" << endl;
		closesocket(sockt);
		WSACleanup();
		return 1;
	}

	//Wysylanie i odbieranie danych
	char buf[8192];
	string userInput;
	string userName;

	do {
		cout << "> ";
		getline(cin, userInput);
		
		if (userInput.size() > 0) {
			int sendResult = send(sockt, userInput.c_str(), userInput.size() + 1, 0); //+1, bo stringi w C++ sa struktura, ktora posiada podazajace zero, ktore nalezy zawrzec, aby wiadomosc byla poprawna
			if (sendResult != SOCKET_ERROR) {
				ZeroMemory(buf, 8192);
				int bytesReceived = recv(sockt, buf, 8192, 0);
				if (bytesReceived > 0) {
					cout << "SERVER> " << string(buf, 0, bytesReceived) << "\r";
				}
			}
		}
	} while (userInput.size() > 0);

	//Zamkniecie polaczenia
	closesocket(sockt);
	WSACleanup();

	return 0;
}