#ifndef _M5STACK_SCREEN_
#define _M5STACK_SCREEN_

#include <M5Stack.h>

#define MAX_LCD_CHAR 27

class M5Stack_Screen
{
    private:
        char screen_msg[MAX_LCD_CHAR];

    public:
        // Métodos relacionados con menús
        void Show_Accelerometer_Menu(void);
        void Show_Connectivity_Menu(void);
        void Show_SD_Initialization_Menu(void);

        // Métodos relacionados con la configuración de la pantalla
        void Clear_Screen(int);
        void Change_Text_Size(unsigned short);
        void Set_Text_Color(int, int);
        void Set_Cursor(unsigned short, unsigned short);

        // Métodos relacionados con la visualización de mensajes de la pantalla
        void Print_Message_On_Screen(const char*, ...);
        void Print_Message_On_Screen(int, int, const char *, ...);

        // Constructores, destructores, etc
        M5Stack_Screen();
        M5Stack_Screen(const M5Stack_Screen &&) = delete;
        M5Stack_Screen(const M5Stack_Screen &) = delete;
        ~M5Stack_Screen();
};

#endif
