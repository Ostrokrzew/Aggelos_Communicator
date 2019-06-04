#include "TcpLstener.h"

using namespace std;

void Listener_MessageReceived(CTcpListener* listener, int client, string msg);

int main() {
	CTcpListener server("127.0.0.1", 54000, Listener_MessageReceived);

	if (server.Init()) {
		server.Run();
	}
}

void Listener_MessageReceived(CTcpListener* listener, int client, string msg) {
	listener->Send(client, msg);
}