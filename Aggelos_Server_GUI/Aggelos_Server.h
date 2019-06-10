#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <sstream>
#include <fstream>
#include <ctime>

#pragma comment (lib, "ws2_32.lib")
#pragma once

#define MAX_BUFFER_SIZE (16384)

class Aggelos_Server {
public:
	Aggelos_Server(std::string, int);

	~Aggelos_Server();

	//Inicjacja winsocka
	bool Init();

	SOCKET CreateListenSocket();

	fd_set CreateFd_set(SOCKET);

	static std::string CreateClientSocket(SOCKET, fd_set, char*, char*, char*);

	//static int Receive(SOCKET, char*);

	static void TimeShow(char*);

	static void TimeShowShort(char*);

	//static void Info(int, int, char, std::string);

	//static void Message(int, int, char, char);

	//static void Broadcast(SOCKET, char*, char*, fd_set, char*, SOCKET);

	//static void AcceptAndHello(SOCKET, SOCKET, fd_set, char*);

	void Send(std::string, fd_set);

	static void MarshalString(System::String^, std::string&);

	static System::String^ RecMessage(std::string);
		
	std::string s_ipAddress;		//IP serwera
	int s_port;						//port nasluchowy serwera
	char buffer[MAX_BUFFER_SIZE];	//bufor na odbierane wiadomosci
	char tmbuf[24];					//czas serwera
	char tmbufsh[8];				//czas serwera krotki
};