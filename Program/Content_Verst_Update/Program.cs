using System;
using System.IO;
using System.IO.Compression;

namespace Updater
{
    class Program
    {
        static void Main(string[] args)
        {
            if (args.Length < 2)
            {
                Console.WriteLine("Usage: Updater.exe <update.zip> <old.exe>");
                return;
            }

            string zipPath = args[0];
            string oldExePath = args[1];

            // Ждём, пока основное приложение завершится
            System.Threading.Thread.Sleep(2000);

            string destDir = Path.GetDirectoryName(oldExePath);

            try
            {
                if (Directory.Exists(destDir))
                {
                    // Удаляем все файлы в папке (осторожно!)
                    foreach (string file in Directory.GetFiles(destDir))
                    {
                        File.Delete(file);
                    }
                }
                ZipFile.ExtractToDirectory(zipPath, destDir);
                // Запускаем обновлённое приложение
                System.Diagnostics.Process.Start(oldExePath);
            }
            catch (Exception ex)
            {
                Console.WriteLine("Error: " + ex.Message);
                Console.ReadKey();
            }
        }
    }
}