#include <WS2tcpip.h>
#include <string>

#pragma once
#pragma comment(lib, "ws2_32.lib")

#define MAX_BUFFER_SIZE (32768)

class CTcpListener;

//wiadomosc zwrotna
typedef void (*MessageReceivedHandler)(CTcpListener* listener, int socketId, std::string msg);
 
class CTcpListener {

public:
	//konstruktor
	CTcpListener(std::string ipAddress, int port, MessageReceivedHandler handler);

	//dekonstruktor
	~CTcpListener();

	//Wysylanie wiadomosci do konkretnego klienta
	void Send(int clientSocket, std::string msg);

	//Inicjacja WinSocka
	bool Init();

	//Glowna czesc przetwarzania
	void Run();

	//Czyszczenie WinSocka
	void Cleanup();

private:
	std::string m_IpAddress;
	int m_Port;
	MessageReceivedHandler MessageReceived;
	//Utworzenie socketu
	SOCKET CreateSocket();
	//Oczekiwanie na polaczenie
	SOCKET WaitForConn(SOCKET listenSock);
};