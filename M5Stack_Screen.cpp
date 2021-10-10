#include "M5Stack_Screen.h"

M5Stack_Screen::M5Stack_Screen() {
    memset(screen_msg, ' ', MAX_LCD_CHAR);
    Set_Text_Color(0xFFFF, 0x0000);
    Change_Text_Size(2);
    Clear_Screen(0x0000);
}

void M5Stack_Screen::Show_Accelerometer_Menu(void) {

    Clear_Screen(0x0000);

    M5.Lcd.println("- Menu del acelerometro -");
    M5.Lcd.println("Elija la escala del acel.");
    M5.Lcd.println("1) Pulse el boton 1");
    M5.Lcd.println("   para navegar");
    M5.Lcd.println("2) Pulse el boton 2 para ");
    M5.Lcd.println("   seleccionarla");
    M5.Lcd.println("--------------------------");
    M5.Lcd.println("Escalas disponibles:");
    M5.Lcd.println("2G, 4G, 8G, 16G");   
}

void M5Stack_Screen::Show_Connectivity_Menu(void) {

    Clear_Screen(0x0000);

    M5.Lcd.println("-  Menu de conectividad  -");
    M5.Lcd.println("--------------------------");
    M5.Lcd.println("Presiona 1 para conectar");
    M5.Lcd.println("  a la red Wifi");
    M5.Lcd.println("                         ");
    M5.Lcd.println("Presiona 2 para continuar");
    M5.Lcd.println("  SIN conexion");
    M5.Lcd.println("--------------------------");
}

void M5Stack_Screen::Show_SD_Initialization_Menu(void) {

    Clear_Screen(0x0000);

    M5.Lcd.println("FICHERO");
    M5.Lcd.println("Use la tecla 1 para");
    M5.Lcd.println("conservar fichero actual");
    M5.Lcd.println("Use la tecla 2 para");
    M5.Lcd.println("usar un nuevo fichero");
}

void M5Stack_Screen::Print_Message_On_Screen(const char *string, ...) {

    va_list arg_ptr;

    memset(screen_msg, ' ', MAX_LCD_CHAR);

    va_start(arg_ptr, string);

    vsprintf(screen_msg, string, arg_ptr);

    M5.Lcd.println(screen_msg);

    va_end(arg_ptr);
}

void M5Stack_Screen::Print_Message_On_Screen(int x, int y, const char *string, ...) {

    va_list arg_ptr;

    memset(screen_msg, ' ', MAX_LCD_CHAR);

    Set_Cursor(x, y);

    va_start(arg_ptr, string);

    vsprintf(screen_msg, string, arg_ptr);

    M5.Lcd.println(screen_msg);

    va_end(arg_ptr);
}

void M5Stack_Screen::Change_Text_Size(unsigned short size) {
    M5.Lcd.setTextSize(size);
}

void M5Stack_Screen::Set_Text_Color(int Text, int Background) {
    M5.Lcd.setTextColor(Text, Background);
}

void M5Stack_Screen::Clear_Screen(int var_Color) {
    M5.Lcd.clear(var_Color);
    M5.Lcd.setCursor(0, 0);
}

void M5Stack_Screen::Set_Cursor(unsigned short x, unsigned short y) {
    if (x <= 320 && y <= 240)
        M5.Lcd.setCursor(x, y);
}

M5Stack_Screen::~M5Stack_Screen() {
}
