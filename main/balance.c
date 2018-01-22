/*
  ����Ǳ��ṩ�Ľӿ��� angle_setangle()������Ƕȣ���ǰ����0�����������������Ǹ���
  ����ϵ����Ϊˮƽ���ҷ�����x����������ֱ���Ϸ�����y��������
  ���г��Ƕȣ���ֱ������0�ȣ����������������Ǹ���
  �����ǲ���Ľ��ٶ�Ҳ�ǣ�������תΪ����������תΪ����
 */

#include <math.h>
/**************************** ���������� *************************/
typedef struct {
    double ax_cali;
    double ay_cali;
    double az_cali;
    double gx_cali;
    double gy_cali;
    double gz_cali;
} sensor_raw_data_cali_t;
sensor_raw_data_cali_t sensor_raw_data_cali;
typedef struct {        //����ԭʼ���ݣ�û�о���ӳ�䣬��������У׼
    double ax;
    double ay;
    double az;
    double gx;
    double gy;
    double gz;
} sensor_raw_data_t;
sensor_raw_data_t sensor_raw_data;
void get_sensor_data()  //��ȡ���ݲ�У׼�����ݱ�����sensor_raw_data��
{
    sensor_raw_data.ax = -mpu6050_getAccelX() + sensor_raw_data_cali.ax_cali;
//    sensor_raw_data.ay = -mpu6050_getAccelY() + sensor_raw_data_cali.ay_cali;
    sensor_raw_data.az = -mpu6050_getAccelZ() + sensor_raw_data_cali.az_cali;
//    sensor_raw_data.gx = mpu6050_getGyroX() + sensor_raw_data_cali.gx_cali;
    sensor_raw_data.gy = mpu6050_getGyroY() + sensor_raw_data_cali.gy_cali;
    sensor_raw_data.gz = mpu6050_getGyroZ() + sensor_raw_data_cali.gz_cali;
}
/*************************** ��̬���� **********************/
#define PI 3.141592654
typedef struct {
    double ax;  //ӳ����ax��ˮƽ������������
    double ay;  //ӳ����ay����ֱ������������
    double palstance;   //ӳ���Ľ��ٶȣ����������������Ǹ�

    //�Ƕȣ���ֱ������0�����������������Ǹ�
    double angle_a;     //���ٶȼ�����ĽǶ�
    double angle_g;     //���ٶȵ��Ӽ�����ĽǶ�
    double angle;       //����ƽ���˲���õ��ĽǶ�

    /* С�����
       ���������������̬�����������������ֱ����ĽǶȣ���С����������ƫ�ƽ��λ�ò�һ������ֱ���Ϸ���
       ��������������ڽǶȣ��ó�С��ƫ�����ĵĽǶȡ�
     */
    double angle_core;  //�������ڽǶȣ�����һ��С�������ǹ̶�ֵ
    double angle_car;   //ƫ�����ĵĽǶȣ���������ǽ���PID�㷨ʱ��ƫ���
} attitude_data_t;
attitude_data_t attitude_data;
void attitude_1()
{
    static double angle_last;
    attitude_data.ax = -sensor_raw_data.ax;
    attitude_data.ay = sensor_raw_data.az/0.97; //������̬��������װƫ���0.97���ϵ���ͽ�������ƫ��
    attitude_data.palstance = -sensor_raw_data.gy/0.97 - sensor_raw_data.gz*0.25;  //��λ�Ƕ�ÿ��
    //���ٶȼ���Ƕ�
    attitude_data.angle_a = -atan(attitude_data.ax/attitude_data.ay)*180/PI;    //�õ��ĵ�λ�ǽǶ�ÿ��
    //�����Ǽ���Ƕ�
    attitude_data.angle_g = angle_last + attitude_data.palstance*0.02;
    //����ƽ���˲�
#define A_GYRO 0.99 //��������ռ�ı���
    attitude_data.angle = A_GYRO*attitude_data.angle_g + (1-A_GYRO)*attitude_data.angle_a;
    angle_last = attitude_data.angle;

    //����ƫ��С�����ĵĽǶ�
    attitude_data.angle_car = attitude_data.angle - attitude_data.angle_core;
}
/*************************** ƽ�ⲿ�� ***************************/
#define BALANCE_TYPE 1  //1-����ģ�ͽ�Ϊ��ϸ��2-û����ϸ������ģ�ͣ��Ƕ�ֱ�Ӷ�Ӧ���ٶȡ�

#if BALANCE_TYPE == 1
typedef struct {
    double balance_angle;       //Ŀ��ƽ��Ƕ�
    double kp, kd;
    double L, Lm, V;    //L��ǰ���ִ��׵���룬Lm�����ľ�����ֵľ��룬V��ǰ���ٶȡ����ǹ��ʵ�λ
    double d_angle;     //�Ƕ�ƫ�Ŀ��Ƕ�-��ǰ�Ƕȣ�������бƫ��Ϊ����������бƫ��Ϊ��
    double ax;          //�����ں�����ٶȣ����������������Ǹ�
    double angle;       //�����̵ĽǶȣ���ǰ����0�����������������Ǹ�
} balance_data_t;
balance_data_t balance_data;
#define limit_angle     40      //�޷���С����λ�ǽǶ�
void balance_1()
{
    double temp;
    balance_data.d_angle = balance_data.balance_angle - attitude_data.angle_car;

    balance_data.ax = balance_data.kp*balance_data.d_angle - balance_data.kd*attitude_data.palstance;
    temp = -balance_data.ax/balance_data.V/balance_data.V*balance_data.Lm;
    if(temp < -1) temp=-1;
    else if(temp > 1) temp=1;
    balance_data.angle = atan((balance_data.L/balance_data.Lm)*tan(asin(temp)))*180/PI;
    if(balance_data.angle > limit_angle) balance_data.angle = limit_angle;
    else if(balance_data.angle < -limit_angle) balance_data.angle = -limit_angle;
    angle_setangle((int)balance_data.angle);
}
#elif BALANCE_TYPE == 2
typedef struct {
    double balance_angle;       //Ŀ��ƽ��Ƕ�
    double kp, kd;
    double d_angle;     //�Ƕ�ƫ�Ŀ��Ƕ�-��ǰ�Ƕȣ�������бƫ��Ϊ����������бƫ��Ϊ��
    double ax;          //�����ں�����ٶȣ����������������Ǹ�
    double angle;       //�����̵ĽǶȣ���ǰ����0�����������������Ǹ�
} balance_data_t;
balance_data_t balance_data;
#define limit_angle     40      //�޷���С����λ�ǽǶ�
void balance_2()
{
    balance_data.d_angle = balance_data.balance_angle - attitude_data.angle_car;
    balance_data.angle = -balance_data.kp*balance_data.d_angle + balance_data.kd*attitude_data.palstance;
    if(balance_data.angle > limit_angle) balance_data.angle = limit_angle;
    else if(balance_data.angle < -limit_angle) balance_data.angle = -limit_angle;
    angle_setangle((int)balance_data.angle);
}
#endif

void balance_tick()  //20msһ��
{
    get_sensor_data();
    attitude_1();
#if BALANCE_TYPE == 1
    balance_1();
#elif BALANCE_TYPE == 2
    balance_2();
#endif
}

void balance_init()
{
    sensor_raw_data_cali.ax_cali=0.61;
    sensor_raw_data_cali.ay_cali=0;
    sensor_raw_data_cali.az_cali=0.3;
    sensor_raw_data_cali.gx_cali=5.85;
    sensor_raw_data_cali.gy_cali=-1.91;
    sensor_raw_data_cali.gz_cali=-19.9;

    attitude_data.ax = 0;
    attitude_data.ay = 0;
    attitude_data.palstance = 0;
    attitude_data.angle_a = 0;
    attitude_data.angle_g = 0;
    attitude_data.angle = 0;
    attitude_data.angle_core = -1.22;       //���ĽǶ�
    attitude_data.angle_car = 0;

#if BALANCE_TYPE == 1
    balance_data.balance_angle = 0;
    balance_data.kp = 0.1;
    balance_data.kd = 0.01;
    balance_data.L=0.145;
    balance_data.Lm=0.035;
    balance_data.V=0.3625;
    balance_data.d_angle = 0;
    balance_data.ax = 0;
    balance_data.angle = 0;
#elif BALANCE_TYPE == 2
    balance_data.balance_angle = 0;
    balance_data.kp = 6;
    balance_data.kd = 0.5;
    balance_data.d_angle = 0;
    balance_data.ax = 0;
    balance_data.angle = 0;
#endif
}


