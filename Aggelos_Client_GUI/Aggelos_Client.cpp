#include "Aggelos_Client.h"

Aggelos_Client::Aggelos_Client(std::string ipAddress, int port)
	:p_ipAddress(ipAddress), p_port(port) {
};

Aggelos_Client::~Aggelos_Client() {
	CleanUp();
}

void Aggelos_Client::MarshalString(System::String^ s, std::string& os) {
	using namespace System::Runtime::InteropServices;
	const char* chars =	(const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
	os = chars;
	Marshal::FreeHGlobal(System::IntPtr((void*)chars));
}

//Inicjacja winsocka
bool Aggelos_Client::Init() {
	WSAData data;
	WORD ver = MAKEWORD(2, 2);

	int wsInit = WSAStartup(ver, &data);
	//Zwrot informacji, ze jest blad lub nie
	return wsInit == 0;
}

SOCKET Aggelos_Client::CreateSocket() {
	SOCKET sockt = socket(AF_INET, SOCK_STREAM, 0);
	if (sockt != INVALID_SOCKET) {
		//Powiazanie socketu z IP i portem
		sockaddr_in hint;
		hint.sin_family = AF_INET;
		hint.sin_port = htons(p_port);
		inet_pton(AF_INET, p_ipAddress.c_str(), &hint.sin_addr);
		//Podlaczenie do serwera
		int connectionResult = connect(sockt, (sockaddr*)& hint, sizeof(hint));
		if (connectionResult != SOCKET_ERROR) {
			return sockt;
		}
		else {
			return -1;
		}
	}
}

void Aggelos_Client::CleanUp() {
	WSACleanup();
}

void Aggelos_Client::Send(int serverSocket, std::string msg) {
	send(serverSocket, msg.c_str(), msg.size() + 1, 0);
}

System::String^ Aggelos_Client::Receive(int serverSocket) {
	char buf[MAX_BUFFER_SIZE];
	ZeroMemory(buf, MAX_BUFFER_SIZE);
	int bytesReceived = 0;
	bytesReceived = recv(serverSocket, buf, MAX_BUFFER_SIZE, 0);
	std::string recMess = std::string(buf, 0, bytesReceived) + "\n";
	System::String^ receivedMessage = gcnew System::String(recMess.c_str());
	return receivedMessage;
}