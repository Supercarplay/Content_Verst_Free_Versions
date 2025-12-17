#pragma endregion
#include "MyForm.h"
using namespace Project;
using namespace System;

//
//	Закрытие формы
//
System::Void Project::MyForm::MyForm_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e) {
	try {
		if (DBconnection != nullptr && DBconnection->State == ConnectionState::Open) {
			DBconnection->Close();
		}
	}
	catch (Exception^) {}
}

//
//	Настройки
//
System::Void Project::MyForm::Btnsettings_Click(System::Object^ sender, System::EventArgs^ e) {
	if (SettingsGroup->Visible == false) {
		SettingsGroup->Visible = true;
		Table_post->Enabled = false;
		button_New_post->Enabled = false;
		Settings = true;
	}
	else {
		SettingsGroup->Visible = false;
		Table_post->Enabled = true;
		button_New_post->Enabled = true;
		Settings = false;
	}
}

//
// Структура создания новых постов
//
System::Void Project::MyForm::button1_Click(System::Object^ sender, System::EventArgs^ e) {
	if (Panel_New_post->Visible == false) {
		Panel_New_post->Visible = true;
		button_New_post->Text = L"Закрыть окно";
		Table_post->Enabled = false;
		Btnsettings->Enabled = false;
		CheckApproved->SetItemChecked(0, false);
	}
	else {
		Panel_New_post->Visible = false;
		button_New_post->Text = L"Добавить пост";
		linkFile->Text = L"Ссылка на файл";
		selectedFileForNewPost = nullptr;
		BtnAddFiles->Text = L"Добавить файл";
		Table_post->Enabled = true;
		Btnsettings->Enabled = true;
		CheckApproved->SetItemChecked(0, false);
	}
}

//
//	Адаптация приложения при изменениях размеров
//
System::Void Project::MyForm::MyForm_Resize(System::Object^ sender, System::EventArgs^ e) {
	if (this->Width > 1400) {
		name_post->AutoSizeMode = DataGridViewAutoSizeColumnMode::None;
		name_post->Width = 250;
		About_post->AutoSizeMode = DataGridViewAutoSizeColumnMode::None;
		About_post->Width = 250;
	}
	else {
		name_post->AutoSizeMode = DataGridViewAutoSizeColumnMode::Fill;
		About_post->AutoSizeMode = DataGridViewAutoSizeColumnMode::Fill;
	}
}

//
//	Таймер
//
System::Void Project::MyForm::OnRefreshTimerTick(System::Object^ sender, System::EventArgs^ e) {
	MyForm_Load(sender, e);
	LoadSettings();
}

//
//	Окно покупки
//
System::Void Project::MyForm::BtnClose_buy_wind_Click(System::Object^ sender, System::EventArgs^ e) {
	Buy_Window->Visible = false;
	Btnsettings->Enabled = true;
	button_New_post->Enabled = true;
	Table_post->Enabled = true;
}
System::Void Project::MyForm::BTN_Buy_Click(System::Object^ sender, System::EventArgs^ e) {
	System::Diagnostics::Process::Start("https://vk.com/shcherbakovdmitrii");
}