#ifndef _MPU6886_SENSORS_
#define _MPU6886_SENSORS_

#include "MPU6886.h"

enum Scales {
    imu6886_AFS_2G = 1, 
    imu6886_AFS_4G, 
    imu6886_AFS_8G, 
    imu6886_AFS_16G
};

struct MPU6886_Accel{
    float X{0.0F};
    float Y{0.0F};
    float Z{0.0F};
};

class MPU_Sensor
{
    private:
        MPU6886 imu6886;
        MPU6886_Accel Accel;
        unsigned short i_mpu6886_accel_mode;

    public:
        // Accelerometer

        // Métodos relacionados con la selección de escala
        void Use_Selected_Accelerometer_Scale(void);
        void Increase_Accelerometer_Scale(void);
        unsigned short Get_Current_Accelerometer_Scale(void);

        // Métodos relacionados con operaciones con el acelerómetro
        void Get_Accel_Data(void);
        float Accel_Data_X(void);
        float Accel_Data_Y(void);
        float Accel_Data_Z(void);

        // Gyroscope

        // Constructores, destructores, etc
        MPU_Sensor();
        MPU_Sensor(const MPU_Sensor &&) = delete;
        MPU_Sensor(const MPU_Sensor &) = delete;
        ~MPU_Sensor();
};

#endif
