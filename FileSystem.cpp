#include "FileSystem.h"

unsigned short SDManager::SD_Start(void)
{
    // Inicializamos la SD
  (SD.begin()) ? cardType = SD.cardType() : cardType = CARD_NONE;
  return cardType;
}

SDManager::SDManager(){}



bool FileManager::Write_File(fs::FS &fs, const char * path, char *sync_h, char *message, char *t_ini)
{
  File file = fs.open(path, FILE_WRITE);

  if (file && file.print(sync_h) && file.println(message) && file.println(t_ini)) {
    file.close();
    return true;
  }

  return false;
}

bool FileManager::Append_File(fs::FS &fs, const char * path, const char * message)
{
  File file = fs.open(path, FILE_APPEND);

  if (file && file.print(message)) {
    file.close();
    return true;
  }

  return false;
}

bool FileManager::Read_File(fs::FS &fs, const char * path)
{
  float testX = 0.0F;
  File file = fs.open(path);

  if (!file)
    return false;

  while (file.available())
  {
    // read zeit - skip over
    file.parseFloat();
    // read accX - sum up
    testX = testX + file.parseFloat();
    // read accY - skip over
    file.parseFloat();
    // read accZ - skip over
    file.parseFloat();
  }

  file.close();

  return true;
}
