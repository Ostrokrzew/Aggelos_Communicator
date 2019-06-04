#include "MainClientWindow.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThread]

void main() {
	Aggelos_Client client("127.0.0.1", 54000);
	client.Init();
	AggelosClientGUI::MainClientWindow::servSocket = client.CreateSocket();

	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	AggelosClientGUI::MainClientWindow form;
	Application::Run(% form);
}