#include "MainServerWindow.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThread]

void main() {
	Aggelos_Server server = Aggelos_Server("127.0.0.1", 54000);
	server.Init();
	AggelosServerGUI::MainServerWindow::listenSocket = server.CreateListenSocket();
	AggelosServerGUI::MainServerWindow::buffer = server.buffer;
	AggelosServerGUI::MainServerWindow::tmbuf = server.tmbuf;
	AggelosServerGUI::MainServerWindow::tmbufsh = server.tmbufsh;
	AggelosServerGUI::MainServerWindow::master = &server.CreateFd_set(AggelosServerGUI::MainServerWindow::listenSocket);
	
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(true);
	AggelosServerGUI::MainServerWindow form;
	Application::Run(% form);
}


