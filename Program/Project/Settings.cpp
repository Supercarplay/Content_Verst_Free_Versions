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
			String^ Settingsquery = "SELECT [Visible_Data], [Visible_Name], [Visible_About], [Visible_Text], [Visible_Scences],"
				"[Visible_Media], [has_Approved], [End_Word] "
				"FROM UserSettings WHERE [ID_Users] = @ID";
			OleDbCommand^ Settingscmd = gcnew OleDbCommand(Settingsquery, DBconnection);
			Settingscmd->Parameters->AddWithValue("@ID", currentUserId.Value);

			OleDbDataReader^ readerSettings = Settingscmd->ExecuteReader();
			if (readerSettings->Read()) {
				//Основная таблица
				Date_post->Visible = Convert::ToBoolean(readerSettings["Visible_Data"]);
				name_post->Visible = Convert::ToBoolean(readerSettings["Visible_Name"]);
				About_post->Visible = Convert::ToBoolean(readerSettings["Visible_About"]);
				Text_post->Visible = Convert::ToBoolean(readerSettings["Visible_Text"]);
				Scencens_post->Visible = Convert::ToBoolean(readerSettings["Visible_Scences"]);
				ViewMedia_post->Visible = Convert::ToBoolean(readerSettings["Visible_Media"]);

				//Отображение в настройках
				CheckVisibleColums->SetItemChecked(0, Convert::ToBoolean(readerSettings["Visible_Data"]));
				CheckVisibleColums->SetItemChecked(1, Convert::ToBoolean(readerSettings["Visible_Name"]));
				CheckVisibleColums->SetItemChecked(2, Convert::ToBoolean(readerSettings["Visible_About"]));
				CheckVisibleColums->SetItemChecked(3, Convert::ToBoolean(readerSettings["Visible_Text"]));
				CheckVisibleColums->SetItemChecked(4, Convert::ToBoolean(readerSettings["Visible_Scences"]));
				CheckVisibleColums->SetItemChecked(5, Convert::ToBoolean(readerSettings["Visible_Media"]));
			}
			else {
				Date_post->Visible = true;
				name_post->Visible = true;
				About_post->Visible = true;
				Text_post->Visible = true;
				Scencens_post->Visible = true;
				ViewMedia_post->Visible = true;
			}
			readerSettings->Close();
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
	if (!currentUserId.HasValue) {
		MessageBox::Show("Вы не авторизованы.", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
		return;
	}

	try {
		String^ updateQuery = "UPDATE Login SET [ID_Group] = @ID_Group WHERE [ID] = @ID";
		OleDbCommand^ cmd = gcnew OleDbCommand(updateQuery, DBconnection);
		cmd->Parameters->AddWithValue("@ID", currentUserId.Value);
		cmd->ExecuteNonQuery();

		String^ checkQuery = "SELECT COUNT(*) FROM UserSettings WHERE [ID_Users] = @UserID";
		OleDbCommand^ checkCmd = gcnew OleDbCommand(checkQuery, DBconnection);
		checkCmd->Parameters->AddWithValue("@UserID", currentUserId.Value);
		int count = safe_cast<int>(checkCmd->ExecuteScalar());

		String^ SettingsQuery;
		if (count > 0) {
			//	Обновляем существующую запись
			SettingsQuery = "UPDATE UserSettings SET "
							"[Visible_Data] = @Visible_Data, "
							"[Visible_Name] = @Visible_Name, "
							"[Visible_About] = @Visible_About, "
							"[Visible_Text] = @Visible_Text, "
							"[Visible_Scences] = @Visible_Scences, "
							"[Visible_Media] = @Visible_Media, "
							"[End_Word] = @End_Word "
							"WHERE [ID_Users] = @UserID";
		}
		else {
			//	Создаём новую запись
			SettingsQuery = "INSERT INTO UserSettings ("
							"[ID_Users], [Visible_Data], [Visible_Name], [Visible_About], "
							"[Visible_Text], [Visible_Scences], [Visible_Media], [End_Word]) "
							"VALUES (@UserID, @Visible_Data, @Visible_Name, @Visible_About, "
							"@Visible_Text, @Visible_Scences, @Visible_Media, @End_Word)";
		}

		OleDbCommand^ SettingsCmd = gcnew OleDbCommand(SettingsQuery, DBconnection);
		SettingsCmd->Parameters->AddWithValue("@UserID", currentUserId.Value);
		SettingsCmd->Parameters->AddWithValue("@Visible_Data", CheckVisibleColums->GetItemChecked(0));
		SettingsCmd->Parameters->AddWithValue("@Visible_Name", CheckVisibleColums->GetItemChecked(1));
		SettingsCmd->Parameters->AddWithValue("@Visible_About", CheckVisibleColums->GetItemChecked(2));
		SettingsCmd->Parameters->AddWithValue("@Visible_Text", CheckVisibleColums->GetItemChecked(3));
		SettingsCmd->Parameters->AddWithValue("@Visible_Scences", CheckVisibleColums->GetItemChecked(4));
		SettingsCmd->Parameters->AddWithValue("@Visible_Media", CheckVisibleColums->GetItemChecked(5));
		SettingsCmd->ExecuteNonQuery();

		//	Применение настроек видимости столбцов
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