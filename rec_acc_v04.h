#ifndef REC_ACC_V04
#define REC_ACC_V04

#include <ezTime.h>
#include <WiFi.h>
#include <M5Stack.h>
#include "EspMQTTClient.h"
#include "FileSystem.h"

#include "SPI.h"
#include "time.h"
#include "math.h"
#include "MPU_Sensor.h"
#include "M5Stack_Screen.h"

void Accelerometer_Scale_Selection(void);
void Connectivity_Mode_Menu(void);
void SD_Initialization(void);

#endif
