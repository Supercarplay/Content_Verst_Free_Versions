#pragma once
namespace Project {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Data::OleDb;

	/// <summary>
	/// Сводка для MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{

	public:
		static String^ GetConnectionString() {
			// Получаем папку, откуда запущено приложение
			String^ appDir = System::Windows::Forms::Application::StartupPath;
			// Формируем полный путь к файлу БД
			String^ dbPath = System::IO::Path::Combine(appDir, "Database.accdb");
			// Возвращаем строку подключения
			return "Provider=Microsoft.ACE.OLEDB.12.0;"
				"Data Source=" + dbPath + ";"
				"Persist Security Info=False;";
		}

	private:
		String^ GetSettingsFilePath() {
			// Получаем папку, откуда запущено приложение
			String^ appDir = System::Windows::Forms::Application::StartupPath;
			String^ filePath = System::IO::Path::Combine(appDir, "settings.ini");

			// Если файл не существует — создаём его с минимальным содержимым
			if (!System::IO::File::Exists(filePath)) {
				String^ defaultContent =
					"[Columns]\r\n"
					"Visible_Data=1\r\n"
					"Visible_Name=1\r\n"
					"Visible_About=1\r\n"
					"Visible_Text=1\r\n"
					"Visible_Scenes=1\r\n"
					"Visible_Media=1\r\n";

				System::IO::File::WriteAllText(filePath, defaultContent, System::Text::Encoding::UTF8);
			}

			return filePath;
		}

		void WriteIniValue(String^ section, String^ key, String^ value) {
			String^ path = GetSettingsFilePath();
			array<String^>^ lines = System::IO::File::ReadAllLines(path);
			bool foundSection = false;
			bool updated = false;
			System::Text::StringBuilder^ sb = gcnew System::Text::StringBuilder();

			for (int i = 0; i < lines->Length; i++) {
				String^ line = lines[i]->Trim();
				if (line->StartsWith("[") && line->EndsWith("]")) {
					if (foundSection && !updated) {
						sb->AppendFormat("{0}={1}\r\n", key, value);
						updated = true;
					}
					foundSection = (line->Equals("[" + section + "]"));
					sb->AppendLine(line);
				}
				else if (foundSection && !line->StartsWith(";") && line->Contains("=")) {
					array<String^>^ parts = line->Split('=');
					if (parts->Length >= 2 && parts[0]->Trim()->Equals(key, StringComparison::OrdinalIgnoreCase)) {
						sb->AppendFormat("{0}={1}\r\n", key, value);
						updated = true;
						continue;
					}
					sb->AppendLine(line);
				}
				else {
					sb->AppendLine(line);
				}
			}

			if (!updated) {
				if (!foundSection) {
					sb->AppendFormat("\r\n[{0}]\r\n", section);
				}
				sb->AppendFormat("{0}={1}\r\n", key, value);
			}

			System::IO::File::WriteAllText(path, sb->ToString());
		}

		String^ ReadIniValue(String^ section, String^ key, String^ defaultValue) {
			try {
				String^ path = GetSettingsFilePath();
				array<String^>^ lines = System::IO::File::ReadAllLines(path);
				bool inSection = false;

				for each(String ^ line in lines) {
					String^ trimmed = line->Trim();
					if (trimmed->StartsWith("[") && trimmed->EndsWith("]")) {
						inSection = (trimmed->Equals("[" + section + "]"));
					}
					else if (inSection && trimmed->Contains("=") && !trimmed->StartsWith(";")) {
						array<String^>^ parts = trimmed->Split('=');
						if (parts->Length >= 2 && parts[0]->Trim()->Equals(key, StringComparison::OrdinalIgnoreCase)) {
							return parts[1]->Trim();
						}
					}
				}
			}
			catch (...) {}
			return defaultValue;
		}
	private:
		OleDbConnection^ DBconnection;
		int currentEditPostID;
		Nullable<int> currentUserId;
		Nullable<int> currentTelegramID;
		String^ selectedFileForNewPost;
		String^ selectedFileForEditPost;
		String^ HistoryFileNewPost;
		String^ HistoryFileEditPost;
		bool isEditingFromArchive = false;
		String^ fileName;
		String^ src;
		bool Settings = false;
	private: System::Windows::Forms::Label^ Name_new_post;
	private: System::Windows::Forms::TextBox^ Textbox_Name_new_post;
	private: System::Windows::Forms::Label^ About_new_post;
	private: System::Windows::Forms::TextBox^ Textbox_About_new_post;
	private: System::Windows::Forms::Label^ Date_new_post;
	private: System::Windows::Forms::DateTimePicker^ Swith_date_new_post;
	private: System::Windows::Forms::DateTimePicker^ TimePicker_new_post;
	private: System::Windows::Forms::DateTimePicker^ TimePicker_Editpost;
	private: System::Windows::Forms::Label^ view_media;
	private: System::Windows::Forms::ComboBox^ Swith_view_media;
	private: System::Windows::Forms::Label^ Text_New_post;
	private: System::Windows::Forms::Label^ Continuity_new_post;
	private: System::Windows::Forms::TextBox^ textBox_Text_New_post;
	private: System::Windows::Forms::TextBox^ textBox_Continuity_new_post;
	private: System::Windows::Forms::Button^ generation_text_post;
	private: System::Windows::Forms::Button^ Generation_Continuity_new_post;
	private: System::Windows::Forms::Button^ Save_button;
	private: System::Windows::Forms::GroupBox^ Panel_New_post;
	private: System::Windows::Forms::DataGridView^ Table_post;
	private: System::Windows::Forms::GroupBox^ Edit_post;
	private: System::Windows::Forms::Button^ Save_editbutton;
	private: System::Windows::Forms::TextBox^ Continuity_Edit_post;
	private: System::Windows::Forms::TextBox^ text_Edit_post;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::ComboBox^ View_media_Edit;
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::DateTimePicker^ dateTimePicker_Editpost;
	private: System::Windows::Forms::Label^ label4;
	private: System::Windows::Forms::TextBox^ About_Edit_post;
	private: System::Windows::Forms::Label^ label5;
	private: System::Windows::Forms::TextBox^ Name_Edit_post;
	private: System::Windows::Forms::Label^ label6;
	private: System::Windows::Forms::Button^ Btnsettings;
	private: System::Windows::Forms::PictureBox^ pictureBox1;
	private: System::Windows::Forms::OpenFileDialog^ openFileDialog;
	private: System::Windows::Forms::Button^ BtnAddFiles;
	private: System::Windows::Forms::LinkLabel^ linkFile;
	private: System::Windows::Forms::LinkLabel^ linkEditFile;
	private: System::Windows::Forms::Button^ BtnEditFile;
	private: System::Windows::Forms::GroupBox^ SettingsGroup;
	private: System::Windows::Forms::GroupBox^ Buy_Window;
	private: System::Windows::Forms::Label^ label11;
	private: System::Windows::Forms::Label^ Hello_User;
	private: System::Windows::Forms::Button^ BtnClose_buy_wind;
	private: System::Windows::Forms::Button^ BTN_Buy;
	private: System::Windows::Forms::Button^ DeleteFile;
	private: System::Windows::Forms::Label^ ReturnArchive;
	private: System::Windows::Forms::CheckedListBox^ CheckVisibleColums;
	private: System::Windows::Forms::Label^ label12;
	private: System::Windows::Forms::Label^ label9;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ ID;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ Date_post;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ name_post;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ About_post;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ Text_post;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ Scencens_post;
	private: System::Windows::Forms::DataGridViewLinkColumn^ ViewMedia_post;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ Files_post;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ IsAprroved;
	private: System::Windows::Forms::DataGridViewButtonColumn^ EditButton;
	private: System::Windows::Forms::DataGridViewButtonColumn^ DeleteButton;
	private: System::Windows::Forms::Button^ BtnSaveSettings;
	private: System::Windows::Forms::Button^ DeleteEditFiles;
	private:
		System::Void MyForm_Load(System::Object^ sender, System::EventArgs^ e);
		System::Void LoadSettings();
		System::Void button1_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void Save_button_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void Table_post_CellContentClick(System::Object^ sender, System::Windows::Forms::DataGridViewCellEventArgs^ e);
		System::Void Save_editbutton_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void BtnAddFiles_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void BtnEditFile_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void MyForm_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e);
		System::Void MyForm_Resize(System::Object^ sender, System::EventArgs^ e);
		System::Void BtnClose_buy_wind_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void BTN_Buy_Click(System::Object^ sender, System::EventArgs^ e);
		//Удаление файлов
		System::Void DeleteFile_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void DeleteEditFiles_Click(System::Object^ sender, System::EventArgs^ e);
		//Настройки
		System::Void Btnsettings_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void BtnSaveSettings_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void CheckVisibleColums_MouseUp(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
	public:
		MyForm(void)
		{
			InitializeComponent();
			String^ connectString = MyForm::GetConnectionString();
			DBconnection = gcnew OleDbConnection(connectString);

			this->AutoScroll = true;
			Table_post->RowHeadersVisible = false;
			Table_post->GridColor = System::Drawing::Color::White;
			Table_post->CellBorderStyle = DataGridViewCellBorderStyle::Single;

			try {
				DBconnection->Open();
			}
			catch (Exception^ ex) {
				MessageBox::Show("Ошибка подключения к базе данных:\n" + ex->Message,
					"Критическая ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
				Application::Exit();
				return;
			}
			LoadSettings();
		}
	protected:
		/// <summary>
		/// Освободить все используемые ресурсы.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^ button_New_post;
	protected:

	protected:

	protected:

	protected:

	private:
		/// <summary>
		/// Обязательная переменная конструктора.
		/// </summary>
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Требуемый метод для поддержки конструктора — не изменяйте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(MyForm::typeid));
			System::Windows::Forms::DataGridViewCellStyle^ dataGridViewCellStyle4 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
			System::Windows::Forms::DataGridViewCellStyle^ dataGridViewCellStyle5 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
			System::Windows::Forms::DataGridViewCellStyle^ dataGridViewCellStyle6 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
			this->button_New_post = (gcnew System::Windows::Forms::Button());
			this->Name_new_post = (gcnew System::Windows::Forms::Label());
			this->Textbox_Name_new_post = (gcnew System::Windows::Forms::TextBox());
			this->About_new_post = (gcnew System::Windows::Forms::Label());
			this->Textbox_About_new_post = (gcnew System::Windows::Forms::TextBox());
			this->Date_new_post = (gcnew System::Windows::Forms::Label());
			this->Swith_date_new_post = (gcnew System::Windows::Forms::DateTimePicker());
			this->view_media = (gcnew System::Windows::Forms::Label());
			this->TimePicker_new_post = (gcnew System::Windows::Forms::DateTimePicker());
			this->TimePicker_Editpost = (gcnew System::Windows::Forms::DateTimePicker());
			this->Swith_view_media = (gcnew System::Windows::Forms::ComboBox());
			this->Text_New_post = (gcnew System::Windows::Forms::Label());
			this->Continuity_new_post = (gcnew System::Windows::Forms::Label());
			this->textBox_Text_New_post = (gcnew System::Windows::Forms::TextBox());
			this->textBox_Continuity_new_post = (gcnew System::Windows::Forms::TextBox());
			this->generation_text_post = (gcnew System::Windows::Forms::Button());
			this->Generation_Continuity_new_post = (gcnew System::Windows::Forms::Button());
			this->Save_button = (gcnew System::Windows::Forms::Button());
			this->Panel_New_post = (gcnew System::Windows::Forms::GroupBox());
			this->DeleteFile = (gcnew System::Windows::Forms::Button());
			this->linkFile = (gcnew System::Windows::Forms::LinkLabel());
			this->BtnAddFiles = (gcnew System::Windows::Forms::Button());
			this->Edit_post = (gcnew System::Windows::Forms::GroupBox());
			this->ReturnArchive = (gcnew System::Windows::Forms::Label());
			this->DeleteEditFiles = (gcnew System::Windows::Forms::Button());
			this->linkEditFile = (gcnew System::Windows::Forms::LinkLabel());
			this->BtnEditFile = (gcnew System::Windows::Forms::Button());
			this->Save_editbutton = (gcnew System::Windows::Forms::Button());
			this->Continuity_Edit_post = (gcnew System::Windows::Forms::TextBox());
			this->text_Edit_post = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->View_media_Edit = (gcnew System::Windows::Forms::ComboBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->dateTimePicker_Editpost = (gcnew System::Windows::Forms::DateTimePicker());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->About_Edit_post = (gcnew System::Windows::Forms::TextBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->Name_Edit_post = (gcnew System::Windows::Forms::TextBox());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->Table_post = (gcnew System::Windows::Forms::DataGridView());
			this->ID = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Date_post = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->name_post = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->About_post = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Text_post = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Scencens_post = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->ViewMedia_post = (gcnew System::Windows::Forms::DataGridViewLinkColumn());
			this->Files_post = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->IsAprroved = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->EditButton = (gcnew System::Windows::Forms::DataGridViewButtonColumn());
			this->DeleteButton = (gcnew System::Windows::Forms::DataGridViewButtonColumn());
			this->Btnsettings = (gcnew System::Windows::Forms::Button());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->openFileDialog = (gcnew System::Windows::Forms::OpenFileDialog());
			this->SettingsGroup = (gcnew System::Windows::Forms::GroupBox());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->CheckVisibleColums = (gcnew System::Windows::Forms::CheckedListBox());
			this->BtnSaveSettings = (gcnew System::Windows::Forms::Button());
			this->Buy_Window = (gcnew System::Windows::Forms::GroupBox());
			this->BtnClose_buy_wind = (gcnew System::Windows::Forms::Button());
			this->BTN_Buy = (gcnew System::Windows::Forms::Button());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->Hello_User = (gcnew System::Windows::Forms::Label());
			this->Panel_New_post->SuspendLayout();
			this->Edit_post->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Table_post))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			this->SettingsGroup->SuspendLayout();
			this->Buy_Window->SuspendLayout();
			this->SuspendLayout();
			// 
			// button_New_post
			// 
			resources->ApplyResources(this->button_New_post, L"button_New_post");
			this->button_New_post->Name = L"button_New_post";
			this->button_New_post->UseVisualStyleBackColor = true;
			this->button_New_post->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
			// 
			// Name_new_post
			// 
			resources->ApplyResources(this->Name_new_post, L"Name_new_post");
			this->Name_new_post->ForeColor = System::Drawing::Color::White;
			this->Name_new_post->Name = L"Name_new_post";
			// 
			// Textbox_Name_new_post
			// 
			this->Textbox_Name_new_post->AcceptsReturn = true;
			this->Textbox_Name_new_post->AcceptsTab = true;
			resources->ApplyResources(this->Textbox_Name_new_post, L"Textbox_Name_new_post");
			this->Textbox_Name_new_post->BackColor = System::Drawing::Color::White;
			this->Textbox_Name_new_post->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->Textbox_Name_new_post->ForeColor = System::Drawing::Color::Black;
			this->Textbox_Name_new_post->Name = L"Textbox_Name_new_post";
			this->Textbox_Name_new_post->Tag = L"Name_Project";
			// 
			// About_new_post
			// 
			resources->ApplyResources(this->About_new_post, L"About_new_post");
			this->About_new_post->Name = L"About_new_post";
			// 
			// Textbox_About_new_post
			// 
			this->Textbox_About_new_post->AcceptsReturn = true;
			this->Textbox_About_new_post->AcceptsTab = true;
			resources->ApplyResources(this->Textbox_About_new_post, L"Textbox_About_new_post");
			this->Textbox_About_new_post->BackColor = System::Drawing::Color::White;
			this->Textbox_About_new_post->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->Textbox_About_new_post->ForeColor = System::Drawing::Color::Black;
			this->Textbox_About_new_post->Name = L"Textbox_About_new_post";
			this->Textbox_About_new_post->Tag = L"Name_Project";
			// 
			// Date_new_post
			// 
			resources->ApplyResources(this->Date_new_post, L"Date_new_post");
			this->Date_new_post->Name = L"Date_new_post";
			// 
			// Swith_date_new_post
			// 
			resources->ApplyResources(this->Swith_date_new_post, L"Swith_date_new_post");
			this->Swith_date_new_post->MinDate = System::DateTime(2000, 1, 1, 0, 0, 0, 0);
			this->Swith_date_new_post->Name = L"Swith_date_new_post";
			// 
			// view_media
			// 
			resources->ApplyResources(this->view_media, L"view_media");
			this->view_media->Name = L"view_media";
			// 
			// TimePicker_new_post
			// 
			this->TimePicker_new_post->Format = System::Windows::Forms::DateTimePickerFormat::Time;
			resources->ApplyResources(this->TimePicker_new_post, L"TimePicker_new_post");
			this->TimePicker_new_post->Name = L"TimePicker_new_post";
			this->TimePicker_new_post->ShowUpDown = true;
			this->TimePicker_new_post->Value = System::DateTime(2025, 11, 16, 9, 0, 0, 0);
			// 
			// TimePicker_Editpost
			// 
			this->TimePicker_Editpost->Format = System::Windows::Forms::DateTimePickerFormat::Time;
			resources->ApplyResources(this->TimePicker_Editpost, L"TimePicker_Editpost");
			this->TimePicker_Editpost->Name = L"TimePicker_Editpost";
			this->TimePicker_Editpost->ShowUpDown = true;
			this->TimePicker_Editpost->Value = System::DateTime(2025, 11, 25, 0, 0, 0, 0);
			// 
			// Swith_view_media
			// 
			resources->ApplyResources(this->Swith_view_media, L"Swith_view_media");
			this->Swith_view_media->FormattingEnabled = true;
			this->Swith_view_media->Items->AddRange(gcnew cli::array< System::Object^  >(3) {
				resources->GetString(L"Swith_view_media.Items"),
					resources->GetString(L"Swith_view_media.Items1"), resources->GetString(L"Swith_view_media.Items2")
			});
			this->Swith_view_media->Name = L"Swith_view_media";
			// 
			// Text_New_post
			// 
			resources->ApplyResources(this->Text_New_post, L"Text_New_post");
			this->Text_New_post->Name = L"Text_New_post";
			// 
			// Continuity_new_post
			// 
			resources->ApplyResources(this->Continuity_new_post, L"Continuity_new_post");
			this->Continuity_new_post->Name = L"Continuity_new_post";
			// 
			// textBox_Text_New_post
			// 
			this->textBox_Text_New_post->AcceptsReturn = true;
			this->textBox_Text_New_post->AcceptsTab = true;
			resources->ApplyResources(this->textBox_Text_New_post, L"textBox_Text_New_post");
			this->textBox_Text_New_post->BackColor = System::Drawing::Color::White;
			this->textBox_Text_New_post->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_Text_New_post->ForeColor = System::Drawing::Color::Black;
			this->textBox_Text_New_post->Name = L"textBox_Text_New_post";
			this->textBox_Text_New_post->Tag = L"Name_Project";
			// 
			// textBox_Continuity_new_post
			// 
			this->textBox_Continuity_new_post->AcceptsReturn = true;
			this->textBox_Continuity_new_post->AcceptsTab = true;
			resources->ApplyResources(this->textBox_Continuity_new_post, L"textBox_Continuity_new_post");
			this->textBox_Continuity_new_post->BackColor = System::Drawing::Color::White;
			this->textBox_Continuity_new_post->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox_Continuity_new_post->ForeColor = System::Drawing::Color::Black;
			this->textBox_Continuity_new_post->Name = L"textBox_Continuity_new_post";
			this->textBox_Continuity_new_post->Tag = L"Name_Project";
			// 
			// generation_text_post
			// 
			resources->ApplyResources(this->generation_text_post, L"generation_text_post");
			this->generation_text_post->ForeColor = System::Drawing::Color::Black;
			this->generation_text_post->Name = L"generation_text_post";
			this->generation_text_post->UseVisualStyleBackColor = true;
			// 
			// Generation_Continuity_new_post
			// 
			resources->ApplyResources(this->Generation_Continuity_new_post, L"Generation_Continuity_new_post");
			this->Generation_Continuity_new_post->ForeColor = System::Drawing::Color::Black;
			this->Generation_Continuity_new_post->Name = L"Generation_Continuity_new_post";
			this->Generation_Continuity_new_post->UseVisualStyleBackColor = true;
			// 
			// Save_button
			// 
			resources->ApplyResources(this->Save_button, L"Save_button");
			this->Save_button->ForeColor = System::Drawing::Color::Black;
			this->Save_button->Name = L"Save_button";
			this->Save_button->UseVisualStyleBackColor = true;
			this->Save_button->Click += gcnew System::EventHandler(this, &MyForm::Save_button_Click);
			// 
			// Panel_New_post
			// 
			resources->ApplyResources(this->Panel_New_post, L"Panel_New_post");
			this->Panel_New_post->Controls->Add(this->DeleteFile);
			this->Panel_New_post->Controls->Add(this->Save_button);
			this->Panel_New_post->Controls->Add(this->Generation_Continuity_new_post);
			this->Panel_New_post->Controls->Add(this->generation_text_post);
			this->Panel_New_post->Controls->Add(this->textBox_Continuity_new_post);
			this->Panel_New_post->Controls->Add(this->textBox_Text_New_post);
			this->Panel_New_post->Controls->Add(this->Continuity_new_post);
			this->Panel_New_post->Controls->Add(this->Text_New_post);
			this->Panel_New_post->Controls->Add(this->Swith_view_media);
			this->Panel_New_post->Controls->Add(this->Swith_date_new_post);
			this->Panel_New_post->Controls->Add(this->Date_new_post);
			this->Panel_New_post->Controls->Add(this->TimePicker_new_post);
			this->Panel_New_post->Controls->Add(this->view_media);
			this->Panel_New_post->Controls->Add(this->Textbox_About_new_post);
			this->Panel_New_post->Controls->Add(this->About_new_post);
			this->Panel_New_post->Controls->Add(this->Textbox_Name_new_post);
			this->Panel_New_post->Controls->Add(this->Name_new_post);
			this->Panel_New_post->Controls->Add(this->linkFile);
			this->Panel_New_post->Controls->Add(this->BtnAddFiles);
			this->Panel_New_post->ForeColor = System::Drawing::Color::White;
			this->Panel_New_post->Name = L"Panel_New_post";
			this->Panel_New_post->TabStop = false;
			this->Panel_New_post->Tag = L"New_post";
			// 
			// DeleteFile
			// 
			resources->ApplyResources(this->DeleteFile, L"DeleteFile");
			this->DeleteFile->ForeColor = System::Drawing::Color::Red;
			this->DeleteFile->Name = L"DeleteFile";
			this->DeleteFile->UseVisualStyleBackColor = true;
			this->DeleteFile->Click += gcnew System::EventHandler(this, &MyForm::DeleteFile_Click);
			// 
			// linkFile
			// 
			resources->ApplyResources(this->linkFile, L"linkFile");
			this->linkFile->LinkColor = System::Drawing::Color::White;
			this->linkFile->Name = L"linkFile";
			this->linkFile->TabStop = true;
			// 
			// BtnAddFiles
			// 
			resources->ApplyResources(this->BtnAddFiles, L"BtnAddFiles");
			this->BtnAddFiles->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(0)),
				static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(0)));
			this->BtnAddFiles->Name = L"BtnAddFiles";
			this->BtnAddFiles->UseVisualStyleBackColor = true;
			this->BtnAddFiles->Click += gcnew System::EventHandler(this, &MyForm::BtnAddFiles_Click);
			// 
			// Edit_post
			// 
			resources->ApplyResources(this->Edit_post, L"Edit_post");
			this->Edit_post->Controls->Add(this->ReturnArchive);
			this->Edit_post->Controls->Add(this->DeleteEditFiles);
			this->Edit_post->Controls->Add(this->linkEditFile);
			this->Edit_post->Controls->Add(this->BtnEditFile);
			this->Edit_post->Controls->Add(this->Save_editbutton);
			this->Edit_post->Controls->Add(this->Continuity_Edit_post);
			this->Edit_post->Controls->Add(this->text_Edit_post);
			this->Edit_post->Controls->Add(this->label1);
			this->Edit_post->Controls->Add(this->label2);
			this->Edit_post->Controls->Add(this->View_media_Edit);
			this->Edit_post->Controls->Add(this->label3);
			this->Edit_post->Controls->Add(this->dateTimePicker_Editpost);
			this->Edit_post->Controls->Add(this->label4);
			this->Edit_post->Controls->Add(this->TimePicker_Editpost);
			this->Edit_post->Controls->Add(this->About_Edit_post);
			this->Edit_post->Controls->Add(this->label5);
			this->Edit_post->Controls->Add(this->Name_Edit_post);
			this->Edit_post->Controls->Add(this->label6);
			this->Edit_post->ForeColor = System::Drawing::Color::White;
			this->Edit_post->Name = L"Edit_post";
			this->Edit_post->TabStop = false;
			this->Edit_post->Tag = L"Edit_post";
			// 
			// ReturnArchive
			// 
			resources->ApplyResources(this->ReturnArchive, L"ReturnArchive");
			this->ReturnArchive->Name = L"ReturnArchive";
			// 
			// DeleteEditFiles
			// 
			this->DeleteEditFiles->ForeColor = System::Drawing::Color::Red;
			resources->ApplyResources(this->DeleteEditFiles, L"DeleteEditFiles");
			this->DeleteEditFiles->Name = L"DeleteEditFiles";
			this->DeleteEditFiles->UseVisualStyleBackColor = true;
			this->DeleteEditFiles->Click += gcnew System::EventHandler(this, &MyForm::DeleteEditFiles_Click);
			// 
			// linkEditFile
			// 
			resources->ApplyResources(this->linkEditFile, L"linkEditFile");
			this->linkEditFile->LinkColor = System::Drawing::Color::White;
			this->linkEditFile->Name = L"linkEditFile";
			this->linkEditFile->TabStop = true;
			// 
			// BtnEditFile
			// 
			resources->ApplyResources(this->BtnEditFile, L"BtnEditFile");
			this->BtnEditFile->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(0)),
				static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(0)));
			this->BtnEditFile->Name = L"BtnEditFile";
			this->BtnEditFile->UseVisualStyleBackColor = true;
			this->BtnEditFile->Click += gcnew System::EventHandler(this, &MyForm::BtnEditFile_Click);
			// 
			// Save_editbutton
			// 
			resources->ApplyResources(this->Save_editbutton, L"Save_editbutton");
			this->Save_editbutton->ForeColor = System::Drawing::Color::Black;
			this->Save_editbutton->Name = L"Save_editbutton";
			this->Save_editbutton->UseVisualStyleBackColor = true;
			this->Save_editbutton->Click += gcnew System::EventHandler(this, &MyForm::Save_editbutton_Click);
			// 
			// Continuity_Edit_post
			// 
			this->Continuity_Edit_post->AcceptsReturn = true;
			this->Continuity_Edit_post->AcceptsTab = true;
			resources->ApplyResources(this->Continuity_Edit_post, L"Continuity_Edit_post");
			this->Continuity_Edit_post->BackColor = System::Drawing::Color::White;
			this->Continuity_Edit_post->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->Continuity_Edit_post->ForeColor = System::Drawing::Color::Black;
			this->Continuity_Edit_post->Name = L"Continuity_Edit_post";
			this->Continuity_Edit_post->Tag = L"Name_Project";
			// 
			// text_Edit_post
			// 
			this->text_Edit_post->AcceptsReturn = true;
			this->text_Edit_post->AcceptsTab = true;
			resources->ApplyResources(this->text_Edit_post, L"text_Edit_post");
			this->text_Edit_post->BackColor = System::Drawing::Color::White;
			this->text_Edit_post->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->text_Edit_post->ForeColor = System::Drawing::Color::Black;
			this->text_Edit_post->Name = L"text_Edit_post";
			this->text_Edit_post->Tag = L"Name_Project";
			// 
			// label1
			// 
			resources->ApplyResources(this->label1, L"label1");
			this->label1->Name = L"label1";
			// 
			// label2
			// 
			resources->ApplyResources(this->label2, L"label2");
			this->label2->Name = L"label2";
			// 
			// View_media_Edit
			// 
			resources->ApplyResources(this->View_media_Edit, L"View_media_Edit");
			this->View_media_Edit->FormattingEnabled = true;
			this->View_media_Edit->Items->AddRange(gcnew cli::array< System::Object^  >(3) {
				resources->GetString(L"View_media_Edit.Items"),
					resources->GetString(L"View_media_Edit.Items1"), resources->GetString(L"View_media_Edit.Items2")
			});
			this->View_media_Edit->Name = L"View_media_Edit";
			// 
			// label3
			// 
			resources->ApplyResources(this->label3, L"label3");
			this->label3->Name = L"label3";
			// 
			// dateTimePicker_Editpost
			// 
			resources->ApplyResources(this->dateTimePicker_Editpost, L"dateTimePicker_Editpost");
			this->dateTimePicker_Editpost->MinDate = System::DateTime(2000, 1, 1, 0, 0, 0, 0);
			this->dateTimePicker_Editpost->Name = L"dateTimePicker_Editpost";
			// 
			// label4
			// 
			resources->ApplyResources(this->label4, L"label4");
			this->label4->Name = L"label4";
			// 
			// About_Edit_post
			// 
			this->About_Edit_post->AcceptsReturn = true;
			this->About_Edit_post->AcceptsTab = true;
			resources->ApplyResources(this->About_Edit_post, L"About_Edit_post");
			this->About_Edit_post->BackColor = System::Drawing::Color::White;
			this->About_Edit_post->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->About_Edit_post->ForeColor = System::Drawing::Color::Black;
			this->About_Edit_post->Name = L"About_Edit_post";
			this->About_Edit_post->Tag = L"Name_Project";
			// 
			// label5
			// 
			resources->ApplyResources(this->label5, L"label5");
			this->label5->Name = L"label5";
			// 
			// Name_Edit_post
			// 
			this->Name_Edit_post->AcceptsReturn = true;
			this->Name_Edit_post->AcceptsTab = true;
			resources->ApplyResources(this->Name_Edit_post, L"Name_Edit_post");
			this->Name_Edit_post->BackColor = System::Drawing::Color::White;
			this->Name_Edit_post->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->Name_Edit_post->ForeColor = System::Drawing::Color::Black;
			this->Name_Edit_post->Name = L"Name_Edit_post";
			this->Name_Edit_post->Tag = L"Name_Project";
			// 
			// label6
			// 
			resources->ApplyResources(this->label6, L"label6");
			this->label6->ForeColor = System::Drawing::Color::White;
			this->label6->Name = L"label6";
			// 
			// Table_post
			// 
			this->Table_post->AllowUserToAddRows = false;
			this->Table_post->AllowUserToDeleteRows = false;
			resources->ApplyResources(this->Table_post, L"Table_post");
			this->Table_post->AutoSizeRowsMode = System::Windows::Forms::DataGridViewAutoSizeRowsMode::AllCells;
			this->Table_post->BackgroundColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(67)),
				static_cast<System::Int32>(static_cast<System::Byte>(93)));
			this->Table_post->BorderStyle = System::Windows::Forms::BorderStyle::None;
			dataGridViewCellStyle4->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
			dataGridViewCellStyle4->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(33)),
				static_cast<System::Int32>(static_cast<System::Byte>(71)));
			dataGridViewCellStyle4->Font = (gcnew System::Drawing::Font(L"Times New Roman", 9.75F));
			dataGridViewCellStyle4->ForeColor = System::Drawing::Color::White;
			dataGridViewCellStyle4->Padding = System::Windows::Forms::Padding(5);
			dataGridViewCellStyle4->SelectionBackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)),
				static_cast<System::Int32>(static_cast<System::Byte>(33)), static_cast<System::Int32>(static_cast<System::Byte>(71)));
			dataGridViewCellStyle4->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
			dataGridViewCellStyle4->WrapMode = System::Windows::Forms::DataGridViewTriState::True;
			this->Table_post->ColumnHeadersDefaultCellStyle = dataGridViewCellStyle4;
			this->Table_post->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(11) {
				this->ID, this->Date_post,
					this->name_post, this->About_post, this->Text_post, this->Scencens_post, this->ViewMedia_post, this->Files_post, this->IsAprroved,
					this->EditButton, this->DeleteButton
			});
			dataGridViewCellStyle5->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleLeft;
			dataGridViewCellStyle5->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(33)),
				static_cast<System::Int32>(static_cast<System::Byte>(71)));
			dataGridViewCellStyle5->Font = (gcnew System::Drawing::Font(L"Times New Roman", 9.75F));
			dataGridViewCellStyle5->ForeColor = System::Drawing::Color::White;
			dataGridViewCellStyle5->SelectionBackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)),
				static_cast<System::Int32>(static_cast<System::Byte>(33)), static_cast<System::Int32>(static_cast<System::Byte>(71)));
			dataGridViewCellStyle5->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
			dataGridViewCellStyle5->WrapMode = System::Windows::Forms::DataGridViewTriState::False;
			this->Table_post->DefaultCellStyle = dataGridViewCellStyle5;
			this->Table_post->GridColor = System::Drawing::Color::Black;
			this->Table_post->Name = L"Table_post";
			this->Table_post->ReadOnly = true;
			dataGridViewCellStyle6->Font = (gcnew System::Drawing::Font(L"Times New Roman", 10));
			this->Table_post->RowsDefaultCellStyle = dataGridViewCellStyle6;
			this->Table_post->CellContentClick += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &MyForm::Table_post_CellContentClick);
			// 
			// ID
			// 
			resources->ApplyResources(this->ID, L"ID");
			this->ID->Name = L"ID";
			this->ID->ReadOnly = true;
			// 
			// Date_post
			// 
			resources->ApplyResources(this->Date_post, L"Date_post");
			this->Date_post->Name = L"Date_post";
			this->Date_post->ReadOnly = true;
			this->Date_post->Resizable = System::Windows::Forms::DataGridViewTriState::False;
			// 
			// name_post
			// 
			resources->ApplyResources(this->name_post, L"name_post");
			this->name_post->Name = L"name_post";
			this->name_post->ReadOnly = true;
			this->name_post->Resizable = System::Windows::Forms::DataGridViewTriState::False;
			// 
			// About_post
			// 
			resources->ApplyResources(this->About_post, L"About_post");
			this->About_post->Name = L"About_post";
			this->About_post->ReadOnly = true;
			this->About_post->Resizable = System::Windows::Forms::DataGridViewTriState::False;
			// 
			// Text_post
			// 
			resources->ApplyResources(this->Text_post, L"Text_post");
			this->Text_post->Name = L"Text_post";
			this->Text_post->ReadOnly = true;
			this->Text_post->Resizable = System::Windows::Forms::DataGridViewTriState::False;
			// 
			// Scencens_post
			// 
			resources->ApplyResources(this->Scencens_post, L"Scencens_post");
			this->Scencens_post->Name = L"Scencens_post";
			this->Scencens_post->ReadOnly = true;
			this->Scencens_post->Resizable = System::Windows::Forms::DataGridViewTriState::False;
			// 
			// ViewMedia_post
			// 
			this->ViewMedia_post->ActiveLinkColor = System::Drawing::Color::RosyBrown;
			resources->ApplyResources(this->ViewMedia_post, L"ViewMedia_post");
			this->ViewMedia_post->LinkColor = System::Drawing::Color::White;
			this->ViewMedia_post->Name = L"ViewMedia_post";
			this->ViewMedia_post->ReadOnly = true;
			this->ViewMedia_post->Resizable = System::Windows::Forms::DataGridViewTriState::False;
			this->ViewMedia_post->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::Automatic;
			this->ViewMedia_post->VisitedLinkColor = System::Drawing::Color::White;
			// 
			// Files_post
			// 
			resources->ApplyResources(this->Files_post, L"Files_post");
			this->Files_post->Name = L"Files_post";
			this->Files_post->ReadOnly = true;
			// 
			// IsAprroved
			// 
			resources->ApplyResources(this->IsAprroved, L"IsAprroved");
			this->IsAprroved->Name = L"IsAprroved";
			this->IsAprroved->ReadOnly = true;
			// 
			// EditButton
			// 
			resources->ApplyResources(this->EditButton, L"EditButton");
			this->EditButton->Name = L"EditButton";
			this->EditButton->ReadOnly = true;
			this->EditButton->Resizable = System::Windows::Forms::DataGridViewTriState::False;
			// 
			// DeleteButton
			// 
			resources->ApplyResources(this->DeleteButton, L"DeleteButton");
			this->DeleteButton->Name = L"DeleteButton";
			this->DeleteButton->ReadOnly = true;
			this->DeleteButton->Resizable = System::Windows::Forms::DataGridViewTriState::False;
			// 
			// Btnsettings
			// 
			resources->ApplyResources(this->Btnsettings, L"Btnsettings");
			this->Btnsettings->Cursor = System::Windows::Forms::Cursors::Hand;
			this->Btnsettings->FlatAppearance->BorderSize = 5;
			this->Btnsettings->Name = L"Btnsettings";
			this->Btnsettings->UseVisualStyleBackColor = true;
			this->Btnsettings->Click += gcnew System::EventHandler(this, &MyForm::Btnsettings_Click);
			// 
			// pictureBox1
			// 
			resources->ApplyResources(this->pictureBox1, L"pictureBox1");
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->TabStop = false;
			// 
			// openFileDialog
			// 
			this->openFileDialog->FileName = L"openFileDialog";
			// 
			// SettingsGroup
			// 
			resources->ApplyResources(this->SettingsGroup, L"SettingsGroup");
			this->SettingsGroup->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(60)),
				static_cast<System::Int32>(static_cast<System::Byte>(130)));
			this->SettingsGroup->Controls->Add(this->label12);
			this->SettingsGroup->Controls->Add(this->label9);
			this->SettingsGroup->Controls->Add(this->CheckVisibleColums);
			this->SettingsGroup->Controls->Add(this->BtnSaveSettings);
			this->SettingsGroup->ForeColor = System::Drawing::Color::White;
			this->SettingsGroup->Name = L"SettingsGroup";
			this->SettingsGroup->TabStop = false;
			// 
			// label12
			// 
			resources->ApplyResources(this->label12, L"label12");
			this->label12->Name = L"label12";
			// 
			// label9
			// 
			resources->ApplyResources(this->label9, L"label9");
			this->label9->Name = L"label9";
			// 
			// CheckVisibleColums
			// 
			this->CheckVisibleColums->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)),
				static_cast<System::Int32>(static_cast<System::Byte>(60)), static_cast<System::Int32>(static_cast<System::Byte>(130)));
			this->CheckVisibleColums->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->CheckVisibleColums->ForeColor = System::Drawing::Color::White;
			this->CheckVisibleColums->Items->AddRange(gcnew cli::array< System::Object^  >(6) {
				resources->GetString(L"CheckVisibleColums.Items"),
					resources->GetString(L"CheckVisibleColums.Items1"), resources->GetString(L"CheckVisibleColums.Items2"), resources->GetString(L"CheckVisibleColums.Items3"),
					resources->GetString(L"CheckVisibleColums.Items4"), resources->GetString(L"CheckVisibleColums.Items5")
			});
			resources->ApplyResources(this->CheckVisibleColums, L"CheckVisibleColums");
			this->CheckVisibleColums->Name = L"CheckVisibleColums";
			this->CheckVisibleColums->SelectionMode = System::Windows::Forms::SelectionMode::None;
			this->CheckVisibleColums->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::CheckVisibleColums_MouseUp);
			// 
			// BtnSaveSettings
			// 
			this->BtnSaveSettings->ForeColor = System::Drawing::Color::Black;
			resources->ApplyResources(this->BtnSaveSettings, L"BtnSaveSettings");
			this->BtnSaveSettings->Name = L"BtnSaveSettings";
			this->BtnSaveSettings->UseVisualStyleBackColor = true;
			this->BtnSaveSettings->Click += gcnew System::EventHandler(this, &MyForm::BtnSaveSettings_Click);
			// 
			// Buy_Window
			// 
			resources->ApplyResources(this->Buy_Window, L"Buy_Window");
			this->Buy_Window->Controls->Add(this->BtnClose_buy_wind);
			this->Buy_Window->Controls->Add(this->BTN_Buy);
			this->Buy_Window->Controls->Add(this->label11);
			this->Buy_Window->Controls->Add(this->Hello_User);
			this->Buy_Window->ForeColor = System::Drawing::Color::White;
			this->Buy_Window->Name = L"Buy_Window";
			this->Buy_Window->TabStop = false;
			// 
			// BtnClose_buy_wind
			// 
			resources->ApplyResources(this->BtnClose_buy_wind, L"BtnClose_buy_wind");
			this->BtnClose_buy_wind->ForeColor = System::Drawing::Color::Red;
			this->BtnClose_buy_wind->Name = L"BtnClose_buy_wind";
			this->BtnClose_buy_wind->UseVisualStyleBackColor = true;
			this->BtnClose_buy_wind->Click += gcnew System::EventHandler(this, &MyForm::BtnClose_buy_wind_Click);
			// 
			// BTN_Buy
			// 
			this->BTN_Buy->ForeColor = System::Drawing::Color::Black;
			resources->ApplyResources(this->BTN_Buy, L"BTN_Buy");
			this->BTN_Buy->Name = L"BTN_Buy";
			this->BTN_Buy->UseVisualStyleBackColor = true;
			this->BTN_Buy->Click += gcnew System::EventHandler(this, &MyForm::BTN_Buy_Click);
			// 
			// label11
			// 
			resources->ApplyResources(this->label11, L"label11");
			this->label11->Name = L"label11";
			// 
			// Hello_User
			// 
			resources->ApplyResources(this->Hello_User, L"Hello_User");
			this->Hello_User->Name = L"Hello_User";
			// 
			// MyForm
			// 
			resources->ApplyResources(this, L"$this");
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(33)),
				static_cast<System::Int32>(static_cast<System::Byte>(71)));
			this->Controls->Add(this->Buy_Window);
			this->Controls->Add(this->SettingsGroup);
			this->Controls->Add(this->Btnsettings);
			this->Controls->Add(this->Edit_post);
			this->Controls->Add(this->Panel_New_post);
			this->Controls->Add(this->button_New_post);
			this->Controls->Add(this->Table_post);
			this->Controls->Add(this->pictureBox1);
			this->Name = L"MyForm";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &MyForm::MyForm_FormClosing);
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->Resize += gcnew System::EventHandler(this, &MyForm::MyForm_Resize);
			this->Panel_New_post->ResumeLayout(false);
			this->Panel_New_post->PerformLayout();
			this->Edit_post->ResumeLayout(false);
			this->Edit_post->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Table_post))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			this->SettingsGroup->ResumeLayout(false);
			this->SettingsGroup->PerformLayout();
			this->Buy_Window->ResumeLayout(false);
			this->Buy_Window->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
	};
}