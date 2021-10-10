#include "MPU_Sensor.h"

MPU_Sensor::MPU_Sensor():i_mpu6886_accel_mode{imu6886_AFS_2G} {
    imu6886.Init();
}

unsigned short MPU_Sensor::Get_Current_Accelerometer_Scale(void) {
    return i_mpu6886_accel_mode;
}

void MPU_Sensor::Increase_Accelerometer_Scale(void) {
    (i_mpu6886_accel_mode < imu6886_AFS_16G) ? i_mpu6886_accel_mode++ : i_mpu6886_accel_mode = imu6886_AFS_2G;
}

void MPU_Sensor::Use_Selected_Accelerometer_Scale(void) {
    imu6886.setAccelFsr(static_cast<MPU6886::Ascale>(i_mpu6886_accel_mode));
}

void MPU_Sensor::Get_Accel_Data(void) {
    imu6886.getAccelData(&Accel.X, &Accel.Y, &Accel.Z);
}

float MPU_Sensor::Accel_Data_X(void) {
    return Accel.X;
}

float MPU_Sensor::Accel_Data_Y(void) {
    return Accel.Y;
}

float MPU_Sensor::Accel_Data_Z(void) {
    return Accel.Z;
}

MPU_Sensor::~MPU_Sensor()
{
}
