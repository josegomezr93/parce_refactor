#define M5STACK_MPU6886

#include "rec_acc_v04.h"

// Ubicamos en Stack para tener acceso a variables de interés mas rapido
static MPU_Sensor Sensor;

// Ubicamos en Heap para preservar la memoria del stack a interfaces mas importantes
static M5Stack_Screen* Screen = (M5Stack_Screen*)malloc(sizeof(M5Stack_Screen));
static SDManager* SD_Manager = (SDManager*)malloc(sizeof(SDManager));

#define INTERVAL 5000
const char *ssid[]{"cjs102"};
const char *password[]{"qazwsxedc"};
//const char *ssid     = "Voodoo_lounge";
//const char *password = "M640659543";

// Variables y funciones asociadas a la interrupción causada por el timer
int totalInterruptCounter;

volatile int interruptCounter;
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  interruptCounter++;
  portEXIT_CRITICAL_ISR(&timerMux);
}

EspMQTTClient client(
  "cjs102",          // Nombre de la red Wifi
  "qazwsxedc",       // Contraseña de la red Wifi
  "192.168.185.107", // MQTT Broker server ip, es la dirección ip de la máquina en donde corre thingsboard
  "M5stack",         // Nombre de usuario que pusiste en "Credenciales del dispositivo"
  "parcero",         // Contraseña que pusiste en "Credenciales del dispositivo"
  "M5stacktest",     // ID Cliente que pusiste en "Credenciales del dispositivo"
  1883               // The MQTT port, default to 1883. this line can be omitted
);

bool refresh_screen = true;
bool button_a = false;
bool measurement = false;
float zeit = 0.0F;
float testX = 0.0F;
char msg[40];
char msg1[40];
char msg_cloud[40]; // dentro del task
char test[18];
String gmt;

//boolean start = true;
unsigned long zeit_start = 0.0;
unsigned long zeit_end = 0.0;
unsigned long lastMicros = 0.0;

//--------------------------- Setup -------------------------------

void setup() {
  // Enable the web updater.
  // User and password default to values of MQTTUsername and MQTTPassword.
  // These can be overrited with enableHTTPWebUpdater("user", "password").
  //client.enableHTTPWebUpdater();
  //client.enableDebuggingMessages(); // Enable debugging messages sent to serial output

  Serial.begin(115200);

  //Inicializamos el dispositivo y el acelerómetro
  M5.begin();
  M5.Power.begin();

  SD_Initialization();

  Accelerometer_Scale_Selection();

  Connectivity_Mode_Menu();

    //Inicializamos la interrupción del timer 
  /*timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1, true);
  timerAlarmEnable(timer);*/
}

void loop()
{
  Serial.print("Here!");
  
    /*if (interruptCounter > 0) {
 
    portENTER_CRITICAL(&timerMux);
    interruptCounter--;
    portEXIT_CRITICAL(&timerMux);
 
    totalInterruptCounter++;

    Serial.print("An interrupt as occurred. Total number: ");
    Serial.println(totalInterruptCounter);
  }*/

  delay(2000);
}

void onConnectionEstablished()
{
  int i = 0;
  M5.update();

  if (refresh_screen == true) {
    refresh_screen = false;
    M5.Lcd.clear(BLACK);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
    M5.Lcd.println("Menu principal");
    M5.Lcd.println("----------------");
    M5.Lcd.println("Pulse A para iniciar");
    M5.Lcd.println("las mediciones");
  }

  if (M5.BtnA.wasReleased() || M5.BtnA.pressedFor(1000, 200))
  {
    button_a = (!(button_a));
    measurement = (!(measurement));
  }

  //start measurement
  if (button_a)
  {
    M5.Lcd.clear(BLACK);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextColor(TFT_GREEN, TFT_BLACK);
    M5.Lcd.println("Tecla A pulsada");
    M5.Lcd.println("tomando registros");
    M5.Lcd.setTextColor(TFT_RED, TFT_BLACK);
    M5.Lcd.println("");
    M5.Lcd.println("Pulse B para detener");
    M5.Lcd.println("toma de registros");
    File file = SD.open( "/acceleration.txt", FILE_APPEND);

    while (measurement)
    {
      // ToDo! Hay que refactorizar la manera en la que se controlan los períodos de recolecta de datos
      // Para ello hay que emplear las interrupciones de timer
      if (micros() - lastMicros > INTERVAL) {
        lastMicros = micros();
        zeit = millis() - zeit_start;
        Sensor.Get_Accel_Data();
        snprintf(msg, sizeof(msg), "%5.0f %5.4f %5.4f %5.4f\n", zeit, Sensor.Accel_Data_X(), Sensor.Accel_Data_Y(), Sensor.Accel_Data_Z());
        file.print(msg);
        i++;

        if (i == 100)
        {
          i = 0;

          // You can activate the retain flag by setting the third parameter to true
          client.publish("v1/devices/me/telemetry", test);
        }
      }

      //stop measurement
      M5.BtnB.read();
      if (M5.BtnB.wasReleased() || M5.BtnB.pressedFor(1000, 200))
      {
        button_a = (!(button_a));
        measurement = (!(measurement));
        M5.Lcd.clear(BLACK);
        M5.Lcd.setCursor(0, 0);
        M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
        M5.Lcd.println("Tecla B pulsada");
        M5.Lcd.println("deteniendo mediciones");
        M5.Lcd.println("cerrando fichero");
        delay(1000);
        refresh_screen = true;
      }
    }
    file.close();
  }
}

inline void Accelerometer_Scale_Selection(void)
{
  unsigned long ul_start_time{millis()};

  Screen->Show_Accelerometer_Menu();

  while(true) {
      M5.update();
      
      // Si presionamos el botón 1, incrementaremos la escala del acelerómetro
      if (M5.BtnA.wasReleased() || M5.BtnA.pressedFor(1000, 200)) {
        Sensor.Increase_Accelerometer_Scale();
        Screen->Print_Message_On_Screen(0, 144, "Escala actual -> AFS_%i G ", (int)pow(2, Sensor.Get_Current_Accelerometer_Scale()));
        ul_start_time = millis();
      }

      // Si presionamos el botón 2, procederemos a asignaremos al M5Stack el modo del acelerómetro
      if (M5.BtnB.wasReleased() || M5.BtnB.pressedFor(1000, 200) || ((millis() - ul_start_time) > 10000))
        break;
  };

  Screen->Print_Message_On_Screen("Escala AFS_%iG elegida", (int)pow(2, Sensor.Get_Current_Accelerometer_Scale()));

  //ToDo: Guardar la escala elegida en la SD
  delay(2000);
}

inline void Connectivity_Mode_Menu(void) {

  unsigned long ul_start_time{millis()};

  Screen->Show_Connectivity_Menu();

  while (true)
  {
    M5.update();

    if (M5.BtnA.wasReleased() || M5.BtnA.pressedFor(1000, 200)) {

      Timezone myTZ;

      Screen->Print_Message_On_Screen("Modo con conexion elegido");

/*    WiFi.begin(ssid, password);

      waitForSync();

      myTZ.setLocation("Europe/Madrid");
      zeit_start = millis();

      gmt = myTZ.dateTime("d-M-y H:i:s.v T");
      M5.Lcd.println(gmt);
      
      */
      break;
    } 
    else if (M5.BtnB.wasReleased() || M5.BtnB.pressedFor(1000, 200)) {
      Screen->Print_Message_On_Screen("Modo sin conexion elegido");
      gmt = "Sin conexion";
      break;
    }
  }
  delay(2000);

}

inline void SD_Initialization(void) {

  char copy[30];
  
  // Inicializamos la SD
  SD_Manager->SD_Start();

  Screen->Show_SD_Initialization_Menu();

  delay(2000);

  //create and print file headers
  snprintf(msg1, sizeof(msg1), "%5.0f\n", zeit_start);
  gmt.toCharArray(copy, 30);

  while (true)
  {
    M5.update();

    if (M5.BtnA.wasReleased() || M5.BtnA.pressedFor(1000, 200))
    {
      File file = SD.open( "/acceleration.txt", FILE_APPEND);

      button_a = !button_a;

      if (!file) {
        Screen->Print_Message_On_Screen("No existe el fichero!");
        Screen->Print_Message_On_Screen("Creando uno nuevo");
        SD_Manager->File_Manager.Write_File(SD, "/acceleration.txt", copy, "\r\n   Time    aX       aY        aZ  \r\n", msg1);
      } else {
        Screen->Print_Message_On_Screen("Se usa el fichero actual");
      }
      break;
    }

    if (M5.BtnB.wasReleased() || M5.BtnB.pressedFor(1000, 200))
    {
      Screen->Print_Message_On_Screen("Creando fichero nuevo");
      SD_Manager->File_Manager.Write_File(SD, "/acceleration.txt", copy, "\r\n   Time    aX       aY        aZ  \r\n", msg1);
      break;
    }
  }
  delay(2000);
}
