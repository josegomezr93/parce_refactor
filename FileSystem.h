#pragma once

#ifndef File_System_H
#define File_System_H

#include "FS.h"
#include "SD.h"

class FileManager
{
    private:
        static File measurement_txt;
        static File log_txt;
        string filename{"/acceleration.txt"};

    public:
        // Funciones asociadas al manejo de ficheros
        bool Write_File(fs::FS &fs, const char *path, char *sync_h, char *message, char *t_ini);
        bool Read_File(fs::FS &fs, const char *path);
        bool Append_File(fs::FS &fs, const char *path, const char *message);

        // Constructores, destructores, etc
        FileManager():FileManager(filename);
        FileManager(string filename);   
        FileManager(const FileManager &&) = delete;
        FileManager(const FileManager &) = delete;
        ~FileManager();
};

class SDManager
{
    private:
        uint8_t cardType{CARD_NONE};

    public:
        FileManager File_Manager;

        // Métodos para obtener información de la SD
        bool SD_Initialized(void);
        bool Card_Type(void);

        // Métodos asociados a las acciones de la SD
        unsigned short SD_Start(void);

        // Constructores, destructores, etc
        SDManager();
        SDManager(const SDManager &&) = delete;
        SDManager(const SDManager &) = delete;
        ~SDManager();
};

#endif
