#include <WS2tcpip.h>
#include <string>

#pragma once
#pragma comment(lib, "ws2_32.lib")

#define MAX_BUFFER_SIZE (16384)

class Aggelos_Client {
public:
	Aggelos_Client(std::string, int );

	~Aggelos_Client();

	static void MarshalString(System::String^, std::string&);

	//Inicjacja winsocka
	bool Init();
	
	void CleanUp();

	static void Send(int, std::string);

	static System::String^ Receive(int serverSocket);

	SOCKET CreateSocket();

private:
	std::string p_ipAddress;	//IP serwera
	int p_port;					//port nasluchowy serwera
};