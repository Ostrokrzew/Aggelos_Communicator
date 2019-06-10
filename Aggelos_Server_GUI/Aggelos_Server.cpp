#include "Aggelos_Server.h"

Aggelos_Server::Aggelos_Server(std::string ipAddress, int port)
	:s_ipAddress(ipAddress), s_port(port) {
}

Aggelos_Server::~Aggelos_Server() {
	WSACleanup();
}

bool Aggelos_Server::Init() {
	WSADATA winsockData;
	WORD ver = MAKEWORD(2, 2);
	int winsockOK = WSAStartup(ver, &winsockData);
	return winsockOK != 0;
}

SOCKET Aggelos_Server::CreateListenSocket() {
	//Tworze socket nasluchowy
	SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, 0);

	//Powiazanie socketu z IP i portem
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(listenSocket, (sockaddr*)& hint, sizeof(hint));

	//Ustawienie socketu jako nasluchowego
	listen(listenSocket, SOMAXCONN);

	return listenSocket;
}

fd_set Aggelos_Server::CreateFd_set(SOCKET listenSocket) {
	fd_set master;
	FD_ZERO(&master);
	FD_SET(listenSocket, &master);
	return master;
}

//void Aggelos_Server::Info(int socketIn, int socketOut, char tmbuf, std::string left_or_join) {
//	std::ostringstream ss;
//	ss << "SOCKET #" << socketIn << " " << left_or_join << " conversation at " << tmbuf << std::endl;
//	std::string sendString = ss.str();
//	send(socketOut, sendString.c_str(), sendString.size() + 1, 0);
//}

//void Aggelos_Server::Message(int socketIn, int socketOut, char tmbufsh, char buffer) {
//	std::ostringstream ss;
//	ss << tmbufsh << "\r\n" << "SOCKET #" << socketIn << ": " << buffer << "\r";
//	std::string sendString = ss.str();
//	send(socketOut, sendString.c_str(), sendString.size() + 1, 0);
//}

void Aggelos_Server::TimeShow(char *tmbuf) {
	//Pobieranie czasu
	time_t lcltm;
	time(&lcltm);
	ZeroMemory(tmbuf, 24);
	struct tm* timeinfo;
	timeinfo = localtime(&lcltm);
	strftime(tmbuf, 24, "%r %A, %d %b", timeinfo);
}

void Aggelos_Server::TimeShowShort(char *tmbufsh) {
	time_t lcltm;
	time(&lcltm);
	ZeroMemory(tmbufsh, 8);
	struct tm* timeinfo;
	timeinfo = localtime(&lcltm);
	strftime(tmbufsh, 8, "%r", timeinfo);
}

std::string Aggelos_Server::CreateClientSocket(SOCKET listenSocket, fd_set master, char* buffer, char* tmbuf, char* tmbufsh) {
	fd_set copy = master;
	std::string sendString;
	int socketCounter = select(0, &copy, nullptr, nullptr, nullptr);

	for (int i = 0; i < socketCounter; i++) {
		SOCKET sock = copy.fd_array[i];
		if (sock == listenSocket) {
			sockaddr_in client;
			int clientSize = sizeof(client);
			SOCKET clientSocket = accept(listenSocket, (sockaddr*)& client, &clientSize);
			std::cout << "SOCKET #" << clientSocket << " connected" << std::endl;

			//Wiadomosc do swiezo podlaczonego klienta
			std::string welcomeMsg = "Welcome to our chat.\r\n";
			send(clientSocket, welcomeMsg.c_str(), welcomeMsg.size() + 1, 0);

			//Dodanie polaczenia do listy polaczonych klientow
			FD_SET(clientSocket, &master);

			//Powiadomienie o nowym uczestniku rozmowy
			for (int i = 0; i < master.fd_count; i++) {
				SOCKET sendSocket = master.fd_array[i];
				if (sendSocket != listenSocket && sendSocket != sock) {
					std::ostringstream ss;
					ss << "SOCKET #" << sock << " joined conversation at " << tmbuf << std::endl;
					std::string sendString = ss.str();
					send(sendSocket, sendString.c_str(), sendString.size() + 1, 0);
				}
			}
		}
		else {
			ZeroMemory(buffer, MAX_BUFFER_SIZE);
			//Odbieranie wiadomosci
			int bytesReceived = 0;
			bytesReceived = recv(sock, buffer, MAX_BUFFER_SIZE, 0);
			if (bytesReceived <= 0) {
				closesocket(sock);
				std::cout << "SOCKET #" << sock << " disconnected" << std::endl;
				FD_CLR(sock, &master);
				for (int i = 0; i < master.fd_count; i++) {
					SOCKET sendSocket = master.fd_array[i];
					if (sendSocket != listenSocket && sendSocket != sock) {
						std::ostringstream ss;
						ss << "SOCKET #" << sock << " left conversation at " << tmbuf << std::endl;
						std::string sendString = ss.str();
						send(sendSocket, sendString.c_str(), sendString.size() + 1, 0);
					}
				}
			}
			//Rozeslanie wiadomosci do pozostalych klientow
			else {
				for (int i = 0; i < master.fd_count; i++) {
					SOCKET sendSocket = master.fd_array[i];
					if (sendSocket != listenSocket && sendSocket != sock) {
						std::ostringstream ss;
						ss << tmbufsh << "\r\n" << "SOCKET #" << sock << ": " << buffer << "\r";
						std::string sendString = ss.str();
						send(sendSocket, sendString.c_str(), sendString.size() + 1, 0);
					}
				}
			}
		}
	}
	return sendString;
}

//int Aggelos_Server::Receive(SOCKET sock, char* buffer) {
//	int bytesReceived = 0;
//	bytesReceived = recv(sock, buffer, MAX_BUFFER_SIZE, 0);
//	return bytesReceived;
//}

//void Aggelos_Server::Broadcast(SOCKET sock, char* buffer, char* tmbuf, fd_set master, char* tmbufsh, SOCKET listenSocket) {
//	ZeroMemory(buffer, MAX_BUFFER_SIZE);
//	//Odbieranie wiadomosci
//	int bytes = Receive(sock, buffer);
//	if (bytes <= 0) {
//		closesocket(sock);
//		std::cout << "SOCKET #" << sock << " disconnected" << std::endl;
//		FD_CLR(sock, &master);
//		for (int i = 0; i < master.fd_count; i++) {
//			SOCKET sendSocket = master.fd_array[i];
//			if (sendSocket != listenSocket && sendSocket != sock) {
//				Info(sock, sendSocket, *tmbuf, "left");
//			}
//		}
//	}
//	//Rozeslanie wiadomosci do pozostalych klientow
//	else {
//		for (int i = 0; i < master.fd_count; i++) {
//			SOCKET sendSocket = master.fd_array[i];
//			if (sendSocket != listenSocket && sendSocket != sock) {
//				Message(sock, sendSocket, *tmbufsh, *buffer);
//			}
//		}
//	}
//}

//void Aggelos_Server::AcceptAndHello(SOCKET sock, SOCKET listenSocket, fd_set master, char* tmbuf) {
//	//Akceptacja polaczenia
//	sockaddr_in client;
//	int clientSize = sizeof(client);
//	SOCKET clientSocket = accept(listenSocket, (sockaddr*)& client, &clientSize);
//	std::cout << "SOCKET #" << clientSocket << " connected" << std::endl;
//
//	//Wiadomosc do swiezo podlaczonego klienta
//	std::string welcomeMsg = "Welcome to our chat.\r\n";
//	send(clientSocket, welcomeMsg.c_str(), welcomeMsg.size() + 1, 0);
//
//	//Dodanie polaczenia do listy polaczonych klientow
//	FD_SET(clientSocket, &master);
//
//	//Powiadomienie o nowym uczestniku rozmowy
//	for (int i = 0; i < master.fd_count; i++) {
//		SOCKET sendSocket = master.fd_array[i];
//		if (sendSocket != listenSocket && sendSocket != sock) {
//			Info(sock, sendSocket, *tmbuf, "joined");
//		}
//	}
//}

void Aggelos_Server::Send(std::string msg, fd_set master) {
	for (int i = 0; i < master.fd_count; i++) {
		SOCKET sendSocket = master.fd_array[i];
		send(sendSocket, msg.c_str(), msg.size() + 1, 0);
	}
}

void Aggelos_Server::MarshalString(System::String^ s, std::string& os) {
	using namespace System::Runtime::InteropServices;
	const char* chars = (const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
	os = chars;
	Marshal::FreeHGlobal(System::IntPtr((void*)chars));
}

System::String^ Aggelos_Server::RecMessage(std::string recMess) {
	//std::string recMess = std::string((char*) &buffer, 0, bytesReceived) + "\n";
	System::String^ receivedMessage = gcnew System::String(recMess.c_str());
	return receivedMessage;
}