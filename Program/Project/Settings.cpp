#pragma endregion
#include "MyForm.h"
using namespace Project;
using namespace System;

//
//	Подгрузка настроек
//
System::Void Project::MyForm::LoadSettings() {
	if (Settings == false) {
		try {
			// Загружаем из INI
			bool visibleData = ReadIniValue("Columns", "Visible_Data", "1") == "1";
			bool visibleName = ReadIniValue("Columns", "Visible_Name", "1") == "1";
			bool visibleAbout = ReadIniValue("Columns", "Visible_About", "1") == "1";
			bool visibleText = ReadIniValue("Columns", "Visible_Text", "1") == "1";
			bool visibleScenes = ReadIniValue("Columns", "Visible_Scenes", "1") == "1";
			bool visibleMedia = ReadIniValue("Columns", "Visible_Media", "1") == "1";

			// Применяем к таблице
			Date_post->Visible = visibleData;
			name_post->Visible = visibleName;
			About_post->Visible = visibleAbout;
			Text_post->Visible = visibleText;
			Scencens_post->Visible = visibleScenes;
			ViewMedia_post->Visible = visibleMedia;

			// Применяем к чекбоксам
			CheckVisibleColums->SetItemChecked(0, visibleData);
			CheckVisibleColums->SetItemChecked(1, visibleName);
			CheckVisibleColums->SetItemChecked(2, visibleAbout);
			CheckVisibleColums->SetItemChecked(3, visibleText);
			CheckVisibleColums->SetItemChecked(4, visibleScenes);
			CheckVisibleColums->SetItemChecked(5, visibleMedia);

			Settings = true;
		}
		catch (Exception^ ex) {
			MessageBox::Show("Ошибка загрузки настроек:\n" + ex->Message, "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
	}
}

//
//	Сохранение настроек
//
System::Void Project::MyForm::BtnSaveSettings_Click(System::Object^ sender, System::EventArgs^ e) {
	try {
		// Сохраняем видимость столбцов
		WriteIniValue("Columns", "Visible_Data", CheckVisibleColums->GetItemChecked(0) ? "1" : "0");
		WriteIniValue("Columns", "Visible_Name", CheckVisibleColums->GetItemChecked(1) ? "1" : "0");
		WriteIniValue("Columns", "Visible_About", CheckVisibleColums->GetItemChecked(2) ? "1" : "0");
		WriteIniValue("Columns", "Visible_Text", CheckVisibleColums->GetItemChecked(3) ? "1" : "0");
		WriteIniValue("Columns", "Visible_Scences", CheckVisibleColums->GetItemChecked(4) ? "1" : "0");
		WriteIniValue("Columns", "Visible_Media", CheckVisibleColums->GetItemChecked(5) ? "1" : "0");

		// Применяем визуально
		Date_post->Visible = CheckVisibleColums->GetItemChecked(0);
		name_post->Visible = CheckVisibleColums->GetItemChecked(1);
		About_post->Visible = CheckVisibleColums->GetItemChecked(2);
		Text_post->Visible = CheckVisibleColums->GetItemChecked(3);
		Scencens_post->Visible = CheckVisibleColums->GetItemChecked(4);
		ViewMedia_post->Visible = CheckVisibleColums->GetItemChecked(5);

		SettingsGroup->Visible = false;
		Table_post->Enabled = true;
		button_New_post->Enabled = true;
		Settings = false;

		MessageBox::Show("Настройки успешно сохранены.", "Успех", MessageBoxButtons::OK, MessageBoxIcon::Information);
	}
	catch (Exception^ ex) {
		MessageBox::Show("Ошибка сохранения настроек:\n" + ex->Message, "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
}

System::Void Project::MyForm::CheckVisibleColums_MouseUp(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
	int index = CheckVisibleColums->IndexFromPoint(e->Location);
	if (index != -1) {
		bool current = CheckVisibleColums->GetItemChecked(index);
		CheckVisibleColums->SetItemChecked(index, !current);
	}
}