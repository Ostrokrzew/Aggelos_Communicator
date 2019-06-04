#include "Aggelos_Client.h"

#pragma once

namespace AggelosClientGUI {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Podsumowanie informacji o MainClientWindow
	/// </summary>
	public ref class MainClientWindow : public System::Windows::Forms::Form
	{
	public:
		static SOCKET servSocket;

		MainClientWindow(void)
		{
			InitializeComponent();
		}

	protected:
		/// <summary>
		/// Wyczyœæ wszystkie u¿ywane zasoby.
		/// </summary>
		~MainClientWindow()
		{
			if (components)
			{
				delete components;
			}
		}


	private: System::Windows::Forms::ListBox^ files;
	private: System::Windows::Forms::TextBox^ msg;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::OpenFileDialog^ openFile;
	private: System::Windows::Forms::Button^ sendMsg;
	private: System::Windows::Forms::Button^ send;
	private: System::Windows::Forms::Button^ save;
	private: System::Windows::Forms::SaveFileDialog^ saveFile;
	private: System::Windows::Forms::TextBox^ conversation;
	private: System::Windows::Forms::Label^ label2;
	private: System::ComponentModel::BackgroundWorker^ recipient;


	private: System::ComponentModel::IContainer^ components;



	private:
		/// <summary>
		/// Wymagana zmienna projektanta.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Metoda wymagana do obs³ugi projektanta — nie nale¿y modyfikowaæ
		/// jej zawartoœci w edytorze kodu.
		/// </summary>
		void InitializeComponent(void)
		{
			this->files = (gcnew System::Windows::Forms::ListBox());
			this->msg = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->openFile = (gcnew System::Windows::Forms::OpenFileDialog());
			this->sendMsg = (gcnew System::Windows::Forms::Button());
			this->send = (gcnew System::Windows::Forms::Button());
			this->save = (gcnew System::Windows::Forms::Button());
			this->saveFile = (gcnew System::Windows::Forms::SaveFileDialog());
			this->conversation = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->recipient = (gcnew System::ComponentModel::BackgroundWorker());
			this->SuspendLayout();
			// 
			// files
			// 
			this->files->AllowDrop = true;
			this->files->BackColor = System::Drawing::Color::LightSlateGray;
			this->files->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->files->Font = (gcnew System::Drawing::Font(L"Microsoft JhengHei UI", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->files->ItemHeight = 22;
			this->files->Location = System::Drawing::Point(826, 37);
			this->files->Name = L"files";
			this->files->Size = System::Drawing::Size(168, 640);
			this->files->TabIndex = 0;
			// 
			// msg
			// 
			this->msg->AcceptsReturn = true;
			this->msg->AcceptsTab = true;
			this->msg->BackColor = System::Drawing::Color::LightSteelBlue;
			this->msg->Font = (gcnew System::Drawing::Font(L"Microsoft JhengHei UI", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->msg->Location = System::Drawing::Point(15, 546);
			this->msg->MaxLength = 65535;
			this->msg->Multiline = true;
			this->msg->Name = L"msg";
			this->msg->Size = System::Drawing::Size(796, 131);
			this->msg->TabIndex = 1;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft JhengHei UI", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label1->Location = System::Drawing::Point(12, 9);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(135, 25);
			this->label1->TabIndex = 2;
			this->label1->Text = L"Conversation";
			this->label1->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// sendMsg
			// 
			this->sendMsg->Location = System::Drawing::Point(674, 704);
			this->sendMsg->Name = L"sendMsg";
			this->sendMsg->Size = System::Drawing::Size(137, 37);
			this->sendMsg->TabIndex = 3;
			this->sendMsg->Text = L"Send message";
			this->sendMsg->UseVisualStyleBackColor = true;
			this->sendMsg->Click += gcnew System::EventHandler(this, &MainClientWindow::SendMsg_Click);
			// 
			// send
			// 
			this->send->Location = System::Drawing::Point(826, 704);
			this->send->Name = L"send";
			this->send->Size = System::Drawing::Size(77, 37);
			this->send->TabIndex = 4;
			this->send->Text = L"Send file";
			this->send->UseVisualStyleBackColor = true;
			this->send->Click += gcnew System::EventHandler(this, &MainClientWindow::Send_Click);
			// 
			// save
			// 
			this->save->Location = System::Drawing::Point(917, 704);
			this->save->Name = L"save";
			this->save->Size = System::Drawing::Size(77, 37);
			this->save->TabIndex = 5;
			this->save->Text = L"Save file";
			this->save->UseVisualStyleBackColor = true;
			this->save->Click += gcnew System::EventHandler(this, &MainClientWindow::Save_Click);
			// 
			// saveFile
			// 
			this->saveFile->SupportMultiDottedExtensions = true;
			// 
			// conversation
			// 
			this->conversation->AccessibleRole = System::Windows::Forms::AccessibleRole::Text;
			this->conversation->BackColor = System::Drawing::Color::LightGray;
			this->conversation->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->conversation->Cursor = System::Windows::Forms::Cursors::IBeam;
			this->conversation->Enabled = false;
			this->conversation->Font = (gcnew System::Drawing::Font(L"Microsoft JhengHei UI", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->conversation->Location = System::Drawing::Point(15, 37);
			this->conversation->MaxLength = 2097151;
			this->conversation->Multiline = true;
			this->conversation->Name = L"conversation";
			this->conversation->ReadOnly = true;
			this->conversation->Size = System::Drawing::Size(796, 492);
			this->conversation->TabIndex = 6;
			this->conversation->TabStop = false;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft JhengHei UI", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label2->Location = System::Drawing::Point(821, 9);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(143, 25);
			this->label2->TabIndex = 7;
			this->label2->Text = L"Files on server";
			this->label2->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// recipient
			// 
			this->recipient->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &MainClientWindow::Recipient_DoWork);
			// 
			// MainClientWindow
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 17);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::RoyalBlue;
			this->ClientSize = System::Drawing::Size(1006, 753);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->conversation);
			this->Controls->Add(this->save);
			this->Controls->Add(this->send);
			this->Controls->Add(this->sendMsg);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->msg);
			this->Controls->Add(this->files);
			this->Font = (gcnew System::Drawing::Font(L"Microsoft JhengHei UI", 7.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->ForeColor = System::Drawing::Color::Black;
			this->Name = L"MainClientWindow";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Aggelos Chat Application";
			this->Load += gcnew System::EventHandler(this, &MainClientWindow::MainClientWindow_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
		private: System::Void MainClientWindow_Load(System::Object^ sender, System::EventArgs^ e) {
			recipient->RunWorkerAsync();
		}
		private: System::Void Send_Click(System::Object^ sender, System::EventArgs^ e) {

		}
		private: System::Void Save_Click(System::Object^ sender, System::EventArgs^ e) {

		}
		private: System::Void SendMsg_Click(System::Object^ sender, System::EventArgs^ e) {

			String^ text = msg->Text;
			std::string message;
			Aggelos_Client::MarshalString(text, message);
			message += "\r\n";
			Aggelos_Client::Send(servSocket, message);
			message = "ME: " + message;
			System::String^ selfMsg = gcnew System::String(message.c_str());
			conversation->Text += selfMsg;
			msg->Clear();
		}
		private: System::Void Refresher_Tick(System::Object^ sender, System::EventArgs^ e) {
			String^ recvText = Aggelos_Client::Receive(servSocket);
			conversation->Text += recvText;
		}
		private: System::Void Recipient_DoWork(System::Object^ sender, System::ComponentModel::DoWorkEventArgs^ e) {
			String^ recvText;
			while (true) {
				recvText = Aggelos_Client::Receive(servSocket)->Trim() + "\r\n";
				conversation->Text += recvText;
			}
		}
	};
}
