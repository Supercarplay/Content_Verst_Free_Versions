#include "MyForm.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Globalization;
using namespace System::Threading;
using namespace System::Net;
using namespace System::IO;
using namespace System::Diagnostics;

// Вспомогательная функция: извлечь текст между двумя подстроками
String^ ExtractBetween(String^ source, String^ start, String^ end) {
    int startIndex = source->IndexOf(start);
    if (startIndex == -1) return "";
    startIndex += start->Length;
    int endIndex = source->IndexOf(end, startIndex);
    if (endIndex == -1) return "";
    return source->Substring(startIndex, endIndex - startIndex);
}

// Вспомогательная функция: сравнить версии
bool IsNewerVersion(String^ remoteVer, String^ currentVer) {
    try {
        remoteVer = remoteVer->Trim()->ToLower()->Replace("v", "");
        currentVer = currentVer->Trim()->ToLower()->Replace("v", "");
        Version^ vRemote = gcnew Version(remoteVer);
        Version^ vCurrent = gcnew Version(currentVer);
        return vRemote > vCurrent;
    }
    catch (...) {
        return false;
    }
}

[STAThreadAttribute]
int main(array<String^>^ args) {
    //  ПРОВЕРКА АВТООБНОВЛЕНИЯ
    try {
        String^ GITHUB_VERSION_URL = "https://github.com/Supercarplay/Content_Verst_Free_Versions/releases/latest/download/version.json";
        WebClient^ client = gcnew WebClient();
        client->Headers->Add("User-Agent", "ContentVerst-Updater");

        String^ json = client->DownloadString(GITHUB_VERSION_URL);
        String^ remoteVersion = ExtractBetween(json, "\"version\": \"", "\"");
        String^ downloadUrl = ExtractBetween(json, "\"download_url\": \"", "\"");

        if (!String::IsNullOrEmpty(remoteVersion) && !String::IsNullOrEmpty(downloadUrl)) {
            if (IsNewerVersion(remoteVersion, Application::ProductVersion)) {
                String^ tempFile = Path::Combine(Path::GetTempPath(), "ContentVerst_update.zip");
                client->DownloadFile(downloadUrl, tempFile);

                // Запуск Updater.exe и завершение текущего процесса
                ProcessStartInfo^ psi = gcnew ProcessStartInfo();
                psi->FileName = "Updater.exe";
                psi->Arguments = "\"" + tempFile + "\" \"" + Application::ExecutablePath + "\"";
                psi->UseShellExecute = true;
                Process::Start(psi);
                return 0;
            }
        }
    }
    catch (Exception^ ex) {
    }

    //  Запуск приложения
    Thread::CurrentThread->CurrentUICulture = gcnew CultureInfo("ru-RU");
    Thread::CurrentThread->CurrentCulture = gcnew CultureInfo("ru-RU");
    Application::SetCompatibleTextRenderingDefault(false);
    Application::EnableVisualStyles();
    Project::MyForm form;
    Application::Run(% form);
    return 0;
}