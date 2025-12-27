#pragma endregion
#include "MyForm.h"
using namespace Project;
using namespace System;

//
//Функционал приложения при загрузке и закрытие
//
System::Void Project::MyForm::MyForm_Load(System::Object^ sender, System::EventArgs^ e) {
	Table_post->AutoGenerateColumns = false;
	Date_post->AutoSizeMode = DataGridViewAutoSizeColumnMode::None;
	Date_post->Width = 75;
	ViewMedia_post->AutoSizeMode = DataGridViewAutoSizeColumnMode::None;
	ViewMedia_post->Width = 60;

	EditButton->AutoSizeMode = DataGridViewAutoSizeColumnMode::None;
	EditButton->Width = 50;
	EditButton->UseColumnTextForButtonValue = true;
	EditButton->Text = L"✎";

	DeleteButton->AutoSizeMode = DataGridViewAutoSizeColumnMode::None;
	DeleteButton->Width = 50;
	DeleteButton->UseColumnTextForButtonValue = true;
	DeleteButton->Text = L"✕";

	//Сохранения места прокрутки при обновление таблицы
	int savedScrollIndex = -1;
	if (Table_post->Rows->Count > 0) {
		savedScrollIndex = Table_post->FirstDisplayedScrollingRowIndex;
	}

	OleDbCommand^ command;
	String^ query = "SELECT [ID], [Date_post], [name_post], [About_post], [Text_post], [Scencens_post], [ViewMedia_post], [Files] FROM TablePost ORDER BY [Date_post]";
	command = gcnew OleDbCommand(query, DBconnection);
	OleDbDataAdapter^ adapter = gcnew OleDbDataAdapter(command);
	DataTable^ dataTable = gcnew DataTable();

	adapter->Fill(dataTable);

	Table_post->DataSource = dataTable;
	ID->DataPropertyName = "ID";
	Date_post->DataPropertyName = "Date_post";
	name_post->DataPropertyName = "name_post";
	About_post->DataPropertyName = "About_post";
	Text_post->DataPropertyName = "Text_post";
	Scencens_post->DataPropertyName = "Scencens_post";
	ViewMedia_post->DataPropertyName = "ViewMedia_post";
	Files_post->DataPropertyName = "Files";

	Table_post->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;
	Table_post->DefaultCellStyle->WrapMode = DataGridViewTriState::True;
	Table_post->AutoSizeRowsMode = DataGridViewAutoSizeRowsMode::AllCells;
	for each(DataGridViewColumn ^ col in Table_post->Columns) {
		col->SortMode = DataGridViewColumnSortMode::NotSortable;
	}
	//Возвращение на место прокрутки после обновление таблицы
	if (savedScrollIndex >= 0 && savedScrollIndex < Table_post->Rows->Count) {
		Table_post->FirstDisplayedScrollingRowIndex = savedScrollIndex;
	}
}


//
// Структура взаимодействия с кнопками удаление и редактирования и просмотр файла
//
System::Void Project::MyForm::Table_post_CellContentClick(System::Object^ sender, System::Windows::Forms::DataGridViewCellEventArgs^ e) {
	if (e->RowIndex < 0) return;

	int editColIndex = Table_post->Columns["EditButton"]->Index;
	int deleteColIndex = Table_post->Columns["DeleteButton"]->Index;

	if (e->ColumnIndex == Table_post->Columns["ViewMedia_post"]->Index) {
		DataGridViewRow^ row = Table_post->Rows[e->RowIndex];
		Object^ fileObj = row->Cells["Files_post"]->Value;
		Object^ dateObj = row->Cells["Date_post"]->Value;
		String^ DopName;
		if (dateObj != nullptr && dateObj != DBNull::Value) {
			DateTime postDate = safe_cast<DateTime>(dateObj);
			DopName = postDate.ToString("yyyyMMdd");
		}
		if (fileObj != nullptr && fileObj != DBNull::Value) {
			String^ relativePath = safe_cast<String^>(fileObj);
			String^ filePostDir = System::IO::Path::Combine(Application::StartupPath, "FilePost");
			String^ fullPath = System::IO::Path::Combine(filePostDir, relativePath);

			if (System::IO::File::Exists(fullPath)) {
				try {
					System::Diagnostics::Process::Start(fullPath);
				}
				catch (Exception^ ex) {
					MessageBox::Show("Файл не обнаружен", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
			}
		} else {
			MessageBox::Show("К этому посту не прикреплён файл.", "Информация", MessageBoxButtons::OK, MessageBoxIcon::Information);
		}
		return;
	}
	if (e->ColumnIndex == editColIndex) {

		ReturnArchive->Visible = false;
		DataGridViewRow^ row = Table_post->Rows[e->RowIndex];
		Object^ idObj = row->Cells["ID"]->Value;
		if (idObj == nullptr || idObj == DBNull::Value) {
			MessageBox::Show("Не удалось загрузить ID записи.", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return;
		}
		currentEditPostID = Convert::ToInt32(idObj);

		Object^ dateObj = row->Cells["Date_post"]->Value;
		Object^ nameObj = row->Cells["name_post"]->Value;
		Object^ aboutObj = row->Cells["About_post"]->Value;
		Object^ textObj = row->Cells["Text_post"]->Value;
		Object^ scencensObj = row->Cells["Scencens_post"]->Value;
		Object^ mediaObj = row->Cells["ViewMedia_post"]->Value;
		Object^ FileObj = row->Cells["Files_post"]->Value;

		if (dateObj != nullptr && dateObj != DBNull::Value) {
			DateTime postDateTime = safe_cast<DateTime>(dateObj);
			dateTimePicker_Editpost->Value = postDateTime.Date;
			TimePicker_Editpost->Value = DateTime::Today + postDateTime.TimeOfDay;
		}

		Name_Edit_post->Text = (nameObj != nullptr && nameObj != DBNull::Value) ? safe_cast<String^>(nameObj) : "";
		About_Edit_post->Text = (aboutObj != nullptr && aboutObj != DBNull::Value) ? safe_cast<String^>(aboutObj) : "";
		text_Edit_post->Text = (textObj != nullptr && textObj != DBNull::Value) ? safe_cast<String^>(textObj) : "";
		Continuity_Edit_post->Text = (scencensObj != nullptr && scencensObj != DBNull::Value) ? safe_cast<String^>(scencensObj) : "";
		linkEditFile->Text = (FileObj != nullptr && FileObj != DBNull::Value) ? safe_cast<String^>(FileObj) : "";

		View_media_Edit->SelectedIndex = 0;
		if (mediaObj != nullptr && mediaObj != DBNull::Value) {
			String^ mediaStr = safe_cast<String^>(mediaObj);
			for (int i = 0; i < View_media_Edit->Items->Count; i++) {
				if (View_media_Edit->Items[i]->ToString() == mediaStr) {
					View_media_Edit->SelectedIndex = i;
					break;
				}
			}
		}
		if (FileObj != nullptr && FileObj != DBNull::Value) {
			String^ fileName = safe_cast<String^>(FileObj);
			linkEditFile->Text = fileName;
			linkEditFile->Visible = true;
			selectedFileForEditPost = fileName;
			DeleteEditFiles->Visible = true;
			DeleteEditFiles->Enabled = true;
		}
		else {
			linkEditFile->Text = "";
			linkEditFile->Visible = false;
			selectedFileForEditPost = nullptr;
			DeleteEditFiles->Visible = false;
			DeleteEditFiles->Enabled = false;
		}

		if (Edit_post->Visible == false) {
			Edit_post->Visible = true;
			Table_post->Enabled = false;
			Btnsettings->Enabled = false;
			button_New_post->Enabled = false;
		}
	}
	else if (e->ColumnIndex == deleteColIndex) {
		String^ postName = safe_cast<String^>(Table_post->Rows[e->RowIndex]->Cells["name_post"]->Value);
		System::Windows::Forms::DialogResult res = MessageBox::Show(
			"Удалить пост \"" + postName + "\"?",
			"Подтверждение",
			MessageBoxButtons::YesNo,
			MessageBoxIcon::Question
		);
		if (res == System::Windows::Forms::DialogResult::Yes) {
			try {
				String^ deleteQuery = "DELETE FROM TablePost WHERE [ID] = ?";
				OleDbCommand^ cmd = gcnew OleDbCommand(deleteQuery, DBconnection);
				int idToDelete = Convert::ToInt32(Table_post->Rows[e->RowIndex]->Cells["ID"]->Value);
				cmd->Parameters->AddWithValue("ID", idToDelete);

				cmd->ExecuteNonQuery();
				MyForm_Load(this, gcnew System::EventArgs());
			}
			catch (Exception^ ex) {
				MessageBox::Show("Ошибка удаления:\n" + ex->Message, "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
			}
		}
	}
}