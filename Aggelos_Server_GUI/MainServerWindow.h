#include "Aggelos_Server.h"

#pragma once

namespace AggelosServerGUI {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Podsumowanie informacji o MainServerWindow
	/// </summary>
	public ref class MainServerWindow : public System::Windows::Forms::Form
	{
	public:
		static SOCKET listenSocket;
		static fd_set* master;
		static char* tmbuf;
		static char* tmbufsh;
		static char* buffer;

		MainServerWindow(void)
		{

			InitializeComponent();
		}

	protected:
		/// <summary>
		/// Wyczyœæ wszystkie u¿ywane zasoby.
		/// </summary>
		~MainServerWindow()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::ComponentModel::BackgroundWorker^ listener;
	private: System::Windows::Forms::TextBox^ conversationBox;


	private: System::Windows::Forms::TextBox^ messageBox;

	private: System::Windows::Forms::Button^ sendToAll;
	private: System::Windows::Forms::ComboBox^ listOfClients;

	private: System::Windows::Forms::Button^ sendToChosen;
	private: System::Windows::Forms::ListBox^ fileList;
	private: System::Windows::Forms::Button^ upload;
	private: System::Windows::Forms::Button^ download;

	private:
		/// <summary>
		/// Wymagana zmienna projektanta.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Metoda wymagana do obs³ugi projektanta — nie nale¿y modyfikowaæ
		/// jej zawartoœci w edytorze kodu.
		/// </summary>
		void InitializeComponent(void)
		{
			this->listener = (gcnew System::ComponentModel::BackgroundWorker());
			this->conversationBox = (gcnew System::Windows::Forms::TextBox());
			this->messageBox = (gcnew System::Windows::Forms::TextBox());
			this->sendToAll = (gcnew System::Windows::Forms::Button());
			this->listOfClients = (gcnew System::Windows::Forms::ComboBox());
			this->sendToChosen = (gcnew System::Windows::Forms::Button());
			this->fileList = (gcnew System::Windows::Forms::ListBox());
			this->upload = (gcnew System::Windows::Forms::Button());
			this->download = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// listener
			// 
			this->listener->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &MainServerWindow::Listener_DoWork);
			// 
			// conversationBox
			// 
			this->conversationBox->Enabled = false;
			this->conversationBox->ForeColor = System::Drawing::Color::Black;
			this->conversationBox->Location = System::Drawing::Point(12, 12);
			this->conversationBox->Multiline = true;
			this->conversationBox->Name = L"conversationBox";
			this->conversationBox->ReadOnly = true;
			this->conversationBox->Size = System::Drawing::Size(701, 388);
			this->conversationBox->TabIndex = 0;
			this->conversationBox->TextChanged += gcnew System::EventHandler(this, &MainServerWindow::ConversationBox_TextChanged);
			// 
			// messageBox
			// 
			this->messageBox->Location = System::Drawing::Point(12, 406);
			this->messageBox->Multiline = true;
			this->messageBox->Name = L"messageBox";
			this->messageBox->Size = System::Drawing::Size(701, 119);
			this->messageBox->TabIndex = 1;
			// 
			// sendToAll
			// 
			this->sendToAll->Location = System::Drawing::Point(560, 532);
			this->sendToAll->Name = L"sendToAll";
			this->sendToAll->Size = System::Drawing::Size(152, 39);
			this->sendToAll->TabIndex = 2;
			this->sendToAll->Text = L"Send to all";
			this->sendToAll->UseVisualStyleBackColor = true;
			// 
			// listOfClients
			// 
			this->listOfClients->FormattingEnabled = true;
			this->listOfClients->Location = System::Drawing::Point(722, 501);
			this->listOfClients->Name = L"listOfClients";
			this->listOfClients->Size = System::Drawing::Size(186, 24);
			this->listOfClients->TabIndex = 4;
			// 
			// sendToChosen
			// 
			this->sendToChosen->Location = System::Drawing::Point(722, 532);
			this->sendToChosen->Name = L"sendToChosen";
			this->sendToChosen->Size = System::Drawing::Size(186, 39);
			this->sendToChosen->TabIndex = 5;
			this->sendToChosen->Text = L"Send to selected";
			this->sendToChosen->UseVisualStyleBackColor = true;
			this->sendToChosen->Click += gcnew System::EventHandler(this, &MainServerWindow::SendToChosen_Click);
			// 
			// fileList
			// 
			this->fileList->AllowDrop = true;
			this->fileList->FormattingEnabled = true;
			this->fileList->ItemHeight = 16;
			this->fileList->Location = System::Drawing::Point(722, 13);
			this->fileList->Name = L"fileList";
			this->fileList->Size = System::Drawing::Size(186, 388);
			this->fileList->TabIndex = 6;
			// 
			// upload
			// 
			this->upload->Location = System::Drawing::Point(722, 407);
			this->upload->Name = L"upload";
			this->upload->Size = System::Drawing::Size(83, 39);
			this->upload->TabIndex = 7;
			this->upload->Text = L"Send file";
			this->upload->UseVisualStyleBackColor = true;
			// 
			// download
			// 
			this->download->Location = System::Drawing::Point(821, 407);
			this->download->Name = L"download";
			this->download->Size = System::Drawing::Size(87, 39);
			this->download->TabIndex = 8;
			this->download->Text = L"Load file";
			this->download->UseVisualStyleBackColor = true;
			// 
			// MainServerWindow
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(920, 583);
			this->Controls->Add(this->download);
			this->Controls->Add(this->upload);
			this->Controls->Add(this->fileList);
			this->Controls->Add(this->sendToChosen);
			this->Controls->Add(this->listOfClients);
			this->Controls->Add(this->sendToAll);
			this->Controls->Add(this->messageBox);
			this->Controls->Add(this->conversationBox);
			this->Name = L"MainServerWindow";
			this->Text = L"Aggelos Server";
			this->Load += gcnew System::EventHandler(this, &MainServerWindow::MainServerWindow_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
		
		private: System::Void MainServerWindow_Load(System::Object^ sender, System::EventArgs^ e) {
			listener->RunWorkerAsync();
		}
		private: System::Void SendToChosen_Click(System::Object^ sender, System::EventArgs^ e) {

		}
		private: System::Void Listener_DoWork(System::Object^ sender, System::ComponentModel::DoWorkEventArgs^ e) {
			System::String^ rcvMsg;
			while (true) {
				Aggelos_Server::TimeShow(tmbuf);
				Aggelos_Server::TimeShowShort(tmbufsh);
				rcvMsg = Aggelos_Server::RecMessage(Aggelos_Server::CreateClientSocket(this->listenSocket, *this->master, this->buffer, this->tmbuf, this->tmbufsh));
				rcvMsg = rcvMsg->Trim() + "\r\n";
				conversationBox->Text += rcvMsg;
			}
		}
	};
}
