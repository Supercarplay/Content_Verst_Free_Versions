#pragma endregion
#include "MyForm.h"
using namespace Project;
using namespace System;

//
//	Сохранение новых постов
//
System::Void Project::MyForm::Save_button_Click(System::Object^ sender, System::EventArgs^ e) {
	try {
		String^ insertQuery = "INSERT INTO TablePost "
			"(Users_ID, Date_post, name_post, About_post, Text_post, Scencens_post, ViewMedia_post, Files) "
			"VALUES (?, ?, ?, ?, ?, ?, ?, ?)";

		OleDbCommand^ cmd = gcnew OleDbCommand(insertQuery, DBconnection);
		if (currentUserId.HasValue) {
			cmd->Parameters->AddWithValue("@Users_ID", currentUserId.Value);
		}
		else {
			cmd->Parameters->AddWithValue("@Users_ID", DBNull::Value);
		}
		DateTime selectedDate = Swith_date_new_post->Value;
		DateTime selectedTime = TimePicker_new_post->Value;
		//	Файловая система
		try {
			String^ DopName = selectedDate.ToString("yyyyMMdd");
			String^ filePostDir = System::IO::Path::Combine(Application::StartupPath, "FilePost");
			String^ dest = System::IO::Path::Combine(filePostDir, DopName + "_" + fileName);
			String^ destServer = System::IO::Path::Combine(serverDir, DopName + "_" + fileName);
			System::IO::File::Copy(src, dest, true);
			System::IO::File::Copy(src, destServer, true);
			selectedFileForNewPost = DopName + "_" + fileName;
		}
		catch (Exception^ ex) {

		}

		DateTime combinedDateTime = selectedDate.Date + selectedTime.TimeOfDay;
		cmd->Parameters->AddWithValue("@Date_post", combinedDateTime);
		cmd->Parameters->AddWithValue("@name_post", Textbox_Name_new_post->Text);
		Object^ aboutValue = String::IsNullOrWhiteSpace(Textbox_About_new_post->Text)
			? static_cast<Object^>(DBNull::Value)
			: safe_cast<Object^>(Textbox_About_new_post->Text);
		cmd->Parameters->AddWithValue("@About_post", aboutValue);

		Object^ textValue = String::IsNullOrWhiteSpace(textBox_Text_New_post->Text)
			? static_cast<Object^>(DBNull::Value)
			: safe_cast<Object^>(textBox_Text_New_post->Text);
		cmd->Parameters->AddWithValue("@Text_post", textValue);

		Object^ scencensValue = String::IsNullOrWhiteSpace(textBox_Continuity_new_post->Text)
			? static_cast<Object^>(DBNull::Value)
			: safe_cast<Object^>(textBox_Continuity_new_post->Text);
		cmd->Parameters->AddWithValue("@Scencens_post", scencensValue);

		Object^ mediaValue = String::IsNullOrWhiteSpace(Swith_view_media->Text) || Swith_view_media->SelectedIndex == 0
			? static_cast<Object^>(DBNull::Value)
			: safe_cast<Object^>(Swith_view_media->Text);
		cmd->Parameters->AddWithValue("@ViewMedia_post", mediaValue);

		Object^ filesValue = selectedFileForNewPost
			? safe_cast<Object^>(selectedFileForNewPost)
			: static_cast<Object^>(DBNull::Value);
		cmd->Parameters->AddWithValue("@Files", filesValue);

		int rowsAffected = cmd->ExecuteNonQuery();

		MyForm_Load(sender, e);

		Panel_New_post->Visible = false;
		button_New_post->Text = L"Добавить пост";

		Textbox_Name_new_post->Clear();
		Textbox_About_new_post->Clear();
		textBox_Text_New_post->Clear();
		textBox_Continuity_new_post->Clear();
		Swith_view_media->SelectedIndex = 0;
		DeleteFile->Visible = false;
		DeleteFile->Enabled = false;
	}
	catch (Exception^ ex) {
		MessageBox::Show("Ошибка сохранения:\n" + ex->Message, "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	src = nullptr;
	fileName = nullptr;
	selectedFileForNewPost = nullptr;
	linkFile->Visible = false;
	BtnAddFiles->Text = L"Добавить файл";
	Table_post->Enabled = true;
	Btnsettings->Enabled = true;
}

//
//	Редактирование постов
//
System::Void Project::MyForm::Save_editbutton_Click(System::Object^ sender, System::EventArgs^ e) {
	try {
		String^ updateQuery = "UPDATE TablePost SET "
			"Date_post = ?, "
			"name_post = ?, "
			"About_post = ?, "
			"Text_post = ?, "
			"Scencens_post = ?, "
			"ViewMedia_post = ?, "
			"Files = ? "
			"WHERE ID = ?";


		OleDbCommand^ cmd = gcnew OleDbCommand(updateQuery, DBconnection);
		DateTime selectedDate = dateTimePicker_Editpost->Value;
		DateTime selectedTime = TimePicker_Editpost->Value;
		//	Файловая система
		try {
			String^ DopName = selectedDate.ToString("yyyyMMdd");
			String^ filePostDir = System::IO::Path::Combine(Application::StartupPath, "FilePost");
			String^ dest = System::IO::Path::Combine(filePostDir, DopName + "_" + fileName);
			String^ destServer = System::IO::Path::Combine(serverDir, DopName + "_" + fileName);
			System::IO::File::Copy(src, dest, true);
			System::IO::File::Copy(src, destServer, true);
			selectedFileForEditPost = DopName + "_" + fileName;
			//	Попытка удалить старый файл
			try {
				String^ DeleteFile = System::IO::Path::Combine(filePostDir, HistoryFileEditPost);
				String^ DeleteFileServer = System::IO::Path::Combine(serverDir, HistoryFileEditPost);
				if (System::IO::File::Exists(DeleteFile) && System::IO::File::Exists(DeleteFileServer)) {
					System::IO::File::Delete(DeleteFile);
					System::IO::File::Delete(DeleteFileServer);
				}
			}
			catch (Exception^ ex) {}
		}
		catch (Exception^ ex) {}

		DateTime combinedDateTime = selectedDate.Date + selectedTime.TimeOfDay;
		cmd->Parameters->AddWithValue("@Date_post", combinedDateTime);
		cmd->Parameters->AddWithValue("@name_post", Name_Edit_post->Text);
		Object^ aboutValue = String::IsNullOrWhiteSpace(About_Edit_post->Text)
			? static_cast<Object^>(DBNull::Value)
			: safe_cast<Object^>(About_Edit_post->Text);
		cmd->Parameters->AddWithValue("@About_post", aboutValue);
		Object^ textValue = String::IsNullOrWhiteSpace(text_Edit_post->Text)
			? static_cast<Object^>(DBNull::Value)
			: safe_cast<Object^>(text_Edit_post->Text);
		cmd->Parameters->AddWithValue("@Text_post", textValue);
		Object^ scencensValue = String::IsNullOrWhiteSpace(Continuity_Edit_post->Text)
			? static_cast<Object^>(DBNull::Value)
			: safe_cast<Object^>(Continuity_Edit_post->Text);
		cmd->Parameters->AddWithValue("@Scencens_post", scencensValue);
		Object^ mediaValue = String::IsNullOrWhiteSpace(View_media_Edit->Text) || View_media_Edit->SelectedIndex == 0
			? static_cast<Object^>(DBNull::Value)
			: safe_cast<Object^>(View_media_Edit->Text);
		cmd->Parameters->AddWithValue("@ViewMedia_post", mediaValue);
		Object^ filesValue = selectedFileForEditPost
			? safe_cast<Object^>(selectedFileForEditPost)
			: static_cast<Object^>(DBNull::Value);
		cmd->Parameters->AddWithValue("@Files", filesValue);
		cmd->Parameters->AddWithValue("@ID", currentEditPostID);

		int rowsAffected = cmd->ExecuteNonQuery();
		if (rowsAffected <= 0) {
			MessageBox::Show("Не найден пост для обновления.", "Предупреждение", MessageBoxButtons::OK, MessageBoxIcon::Warning);
		}

		isEditingFromArchive = false;
		ReturnArchive->Visible = false;
		MyForm_Load(this, gcnew System::EventArgs());
		Edit_post->Visible = false;
		currentEditPostID = 0;
		Table_post->Enabled = true;
		Btnsettings->Enabled = true;
		button_New_post->Enabled = true;
		DeleteEditFiles->Visible = false;
		DeleteEditFiles->Enabled = false;
	}
	catch (Exception^ ex) {
		MessageBox::Show("Ошибка обновления:\n" + ex->Message, "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	src = nullptr;
	fileName = nullptr;
	selectedFileForEditPost = nullptr;
	linkEditFile->Visible = false;
	BtnEditFile->Text = L"Добавить файл";
}

//
//	Добавление файлов
//
System::Void Project::MyForm::BtnAddFiles_Click(System::Object^ sender, System::EventArgs^ e) {

	if (openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
		try {
			src = openFileDialog->FileName;
			fileName = System::IO::Path::GetFileName(src);
			linkFile->Text = fileName;
			linkFile->Visible = true;
			BtnAddFiles->Text = L"Изменить файл";
			DeleteFile->Visible = true;
			DeleteFile->Enabled = true;
		}
		catch (Exception^ ex) {
			MessageBox::Show("Ошибка: " + ex->Message, "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
	}
}
System::Void Project::MyForm::BtnEditFile_Click(System::Object^ sender, System::EventArgs^ e) {
	if (openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
		try {
			src = openFileDialog->FileName;
			fileName = System::IO::Path::GetFileName(src);
			linkEditFile->Text = fileName;
			linkEditFile->Visible = true;
			BtnAddFiles->Text = L"Изменить файл";
			DeleteFile->Visible = true;
			DeleteFile->Enabled = true;
		}
		catch (Exception^ ex) {
			MessageBox::Show("Ошибка добавления файла: " + ex->Message, "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
	}
}

//
//	Удаление добавленного файла
//
System::Void Project::MyForm::DeleteFile_Click(System::Object^ sender, System::EventArgs^ e) {
	linkFile->Text = "Ссылка на файл";
	DeleteFile->Visible = false;
	DeleteFile->Enabled = false;
	src = nullptr;
	fileName = nullptr;
	selectedFileForNewPost = nullptr;
}
System::Void Project::MyForm::DeleteEditFiles_Click(System::Object^ sender, System::EventArgs^ e) {
	linkEditFile->Text = "Ссылка на файл";
	DeleteEditFiles->Visible = false;
	DeleteEditFiles->Enabled = false;
	src = nullptr;
	fileName = nullptr;
	selectedFileForEditPost = nullptr;
}