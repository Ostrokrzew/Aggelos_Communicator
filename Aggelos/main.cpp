#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <sstream>
#include <ctime>

#pragma comment (lib, "ws2_32.lib")
#pragma warning(disable:4996)

using namespace std;

int main() {

	//Inicjacja WinSocka
	WSADATA winsockData;
	WORD ver = MAKEWORD(2, 2);

	int winsockOK = WSAStartup(ver, &winsockData);
	if (winsockOK != 0) {
		cerr << "Error with initialazing WinSock. Bye!" << endl;
		return 1;
	}
	
	//Otwarcie socketu
	SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket == INVALID_SOCKET) {
		cerr << "Error with creating listening socket. Bye!" << endl;
		WSACleanup();
		return 1;
	}

	//Powiazanie socketu z IP i portem
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(listenSocket, (sockaddr*)& hint, sizeof(hint));

	//Ustawienie socketu jako nasluchowego
	listen(listenSocket, SOMAXCONN);

	fd_set master;

	FD_ZERO(&master);
	FD_SET(listenSocket, &master);

	while (true) {
		fd_set copy = master;
		//Pobieranie czasu
		time_t lcltm;
		time(&lcltm);
		char tmbuf[64];
		ZeroMemory(tmbuf, 64);
		struct tm* timeinfo;
		timeinfo = localtime(&lcltm);
		strftime(tmbuf, 64, "%r %A, %d %b", timeinfo);

		int socketCounter = select(0, &copy, nullptr, nullptr, nullptr);

		for (int i = 0; i < socketCounter; i++) {
			SOCKET sock = copy.fd_array[i];
			if (sock == listenSocket) {
				//Akceptacja polaczenia
				sockaddr_in client;
				int clientSize = sizeof(client);
				SOCKET clientSocket = accept(listenSocket, (sockaddr*)& client, &clientSize);
				std::cout << "SOCKET #" << clientSocket << " connected" << std::endl;

				//Wiadomosc do swiezo podlaczonego klienta
				string welcomeMsg = "Welcome to our chat.\r\n";
				send(clientSocket, welcomeMsg.c_str(), welcomeMsg.size() + 1, 0);

				//Dodanie polaczenia do listy polaczonych klientow
				FD_SET(clientSocket, &master);

				//Powiadomienie o nowym uczestniku rozmowy
				for (int i = 0; i < master.fd_count; i++) {
					SOCKET sendSocket = master.fd_array[i];
					if (sendSocket != listenSocket && sendSocket != sock) {
						ostringstream ss;
						ss << "SOCKET #" << clientSocket << " joined conversation at " << tmbuf << std::endl;
						string sendString = ss.str();
						send(sendSocket, sendString.c_str(), sendString.size() + 1, 0);
					}
				}
			}
			else {
				char buffer[8192];
				ZeroMemory(buffer, 8192);

				//Odbieranie wiadomosci
				int bytesReceived = recv(sock, buffer, 8192, 0);
				if (bytesReceived <= 0) {
					closesocket(sock);
					std::cout << "SOCKET #" << sock << " disconnected" << std::endl;
					FD_CLR(sock, &master);
					for (int i = 0; i < master.fd_count; i++) {
						SOCKET sendSocket = master.fd_array[i];
						if (sendSocket != listenSocket && sendSocket != sock) {
							ostringstream ss;
							ss << "SOCKET #" << sock << " left conversation at " << tmbuf << std::endl;
							string sendString = ss.str();
							send(sendSocket, sendString.c_str(), sendString.size() + 1, 0);
						}
					}
				}
				//Rozeslanie wiadomosci do pozostalych klientow
				else {
					for (int i = 0; i < master.fd_count; i++) {
						SOCKET sendSocket = master.fd_array[i];
						if (sendSocket != listenSocket && sendSocket != sock) {
							ostringstream ss;
							ss << "SOCKET #" << sock << ": " << buffer << "\r";
							string sendString = ss.str();
							send(sendSocket, sendString.c_str(), sendString.size() + 1, 0);
						}
					}
				}
			}
		}
	}

	//Czyszczenie WinSocka
	WSACleanup();

	return 0;
}

////Oczekiwanie na polaczenie
//sockaddr_in client;
//int clientSize = sizeof(client);

//SOCKET clientSocket = accept(listenSocket, (sockaddr*)& client, &clientSize);
//if (clientSocket == INVALID_SOCKET) {
//	cerr << "Error with creating client socket. Bye!" << endl;
//	WSACleanup();
//	return 1;
//}

//char host[NI_MAXHOST];		//nazwa klienta
//char service[NI_MAXHOST];		//Port, do ktorego jest podlaczony klient

//ZeroMemory(host, NI_MAXHOST);
//ZeroMemory(service, NI_MAXHOST);

//if (getnameinfo((sockaddr*)& client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
//	cout << host << " connected on port " << service << endl;
//}
//else {
//	inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
//	cout << host << " connected on port " << ntohs(client.sin_port) << endl;
//}

////Zamkniecie socketu nasluchowego
//closesocket(listenSocket);

////Akceptacja klienta i broadcasting
//char buf[8192];

//while (true)
//{
//	ZeroMemory(buf, 8192);
//	//Oczekiwanie na dane od klienta
//	int bytesReceived = recv(clientSocket, buf, 8192, 0);
//	if (bytesReceived == SOCKET_ERROR) {
//		cerr << "Error in receiving data. Bye!" << endl;
//		WSACleanup();
//		break;
//	}

//	if (bytesReceived == 0) {
//		cout << "Client disconnected." << endl;
//		break;
//	}

//	cout << string(buf, 0, bytesReceived + 1) << endl;

//	//Broadcast
//	send(clientSocket, buf, bytesReceived + 1, 0); //+1, bo ZeroMemory dodaje jedno zero, ktore trzeba zawrzec w odpowiedzi, zeby prawidlowo sie odeslala
//}

////Zamkniecie socketu
//closesocket(clientSocket);