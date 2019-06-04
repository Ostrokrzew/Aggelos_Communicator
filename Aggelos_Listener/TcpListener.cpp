#include "TcpLstener.h"
#include <iostream>

//konstruktor
CTcpListener::CTcpListener(std::string ipAddress, int port, MessageReceivedHandler handler)
	: m_IpAddress(ipAddress), m_Port(port), MessageReceived(handler) {
}

//dekonstruktor
CTcpListener::~CTcpListener() {
	Cleanup();
}

//Wysylanie wiadomosci do konkretnego klienta
void CTcpListener::Send(int clientSocket, std::string msg) {
	send(clientSocket, msg.c_str(), msg.size() + 1, 0);
}

//Inicjacja WinSocka
bool CTcpListener::Init() {
	WSAData data;
	WORD ver = MAKEWORD(2, 2);

	int wsInit = WSAStartup(ver, &data);
	//Zwrot informacji, ze jest blad lub nie
	return wsInit == 0;
}

//Glowna czesc przetwarzania
void CTcpListener::Run() {
	char buf[MAX_BUFFER_SIZE];

	while (true) {
		//Otwarcie socketu nasluchowego
		SOCKET listening = CreateSocket();
		if (listening == INVALID_SOCKET) {
			break;
		}
		//socket klienta
		SOCKET client = WaitForConn(listening);
		if (client != INVALID_SOCKET) {
			std::cout << m_IpAddress << " connected on port " << m_Port << std::endl;
			//Zamkniecie socketu nasluchowego, bo niepotrzebny
			closesocket(listening);
			//Odbieranie i wysylanie wiadomosci
			int bytesReceived = 0;
			do {
				ZeroMemory(buf, MAX_BUFFER_SIZE);
				bytesReceived = recv(client, buf, MAX_BUFFER_SIZE, 0);
				if (bytesReceived > 0) {
					if (MessageReceived != NULL) {
						MessageReceived(this, client, std::string(buf, 0, bytesReceived));
						std::cout << m_IpAddress << "> " << std::string(buf, 0, bytesReceived + 1) << "\r";
					}
				}
			} while (bytesReceived > 0);
			//Zamkniecie socketu klienta
			closesocket(client);
			std::cout << m_IpAddress << " disconnected" << std::endl;
		}
	}
}

//Czyszczenie WinSocka
void CTcpListener::Cleanup() {
	WSACleanup();
}

//Utworzenie socketu nasluchowego, zwiazanie go z portem i adresem
SOCKET CTcpListener::CreateSocket() {
	SOCKET listenSock = socket(AF_INET, SOCK_STREAM, 0);
	if (listenSock != INVALID_SOCKET) {
		sockaddr_in hint;
		hint.sin_family = AF_INET;
		hint.sin_port = htons(m_Port);
		inet_pton(AF_INET, m_IpAddress.c_str(), &hint.sin_addr);

		int bindOK = bind(listenSock, (sockaddr*)& hint, sizeof(hint));
		if (bindOK != SOCKET_ERROR) {
			int listenOK = listen(listenSock, SOMAXCONN);
			if (listenOK == SOCKET_ERROR) {
				return -1;
			}
		}
		else {
			return -1;
		}
	}
	return listenSock;
}
//Oczekiwanie na polaczenie
SOCKET CTcpListener::WaitForConn(SOCKET listenSock) {
	SOCKET client = accept(listenSock, NULL, NULL);
	return client;
}