#ifndef __GY521_H__
#define __GY521_H__
/*
  mpu6050ģ���������
  ����IIC����ͨ�ţ������IIC����������Ҳ���������
  ÿ�����ϵļ��ٶȻ��߽��ٶȶ�ȡ��������16λ�з������ݣ��ָ�8λ�͵�8λ��ȡ����
  �����ṩ���¼���������
  void mpu6050_init();���ݵ�ǰ������ɳ�ʼ��
  int mpu6050_getAccelXData();����x����ٶȵ�ԭʼ����
  int mpu6050_getAccelYData();����y����ٶȵ�ԭʼ����
  int mpu6050_getAccelZData();����z����ٶȵ�ԭʼ����
  int mpu6050_getGyroXData();����x����ٶȵ�ԭʼ����
  int mpu6050_getGyroYData();����y����ٶȵ�ԭʼ����
  int mpu6050_getGyroZData();����z����ٶȵ�ԭʼ����
  int mpu6050_getTemperatureData();�����¶ȵ�ԭʼ����

  float mpu6050_getAccelX();����x����ٶȵ�����ֵ����λ��m/s^2
  float mpu6050_getAccelY();����y����ٶȵ�����ֵ����λ��m/s^2
  float mpu6050_getAccelZ();����z����ٶȵ�����ֵ����λ��m/s^2
  float mpu6050_getGyroX();����x��Ľ��ٶȣ���λ�� deg/s���ǶȲ��ǻ���
  float mpu6050_getGyroY();����y��Ľ��ٶȣ���λ�� deg/s
  float mpu6050_getGyroZ();����z��Ľ��ٶȣ���λ�� deg/s
  float mpu6050_getTemperature();�����¶ȣ���λ�����϶�

  void mpu6050_setAddress(unsigned char);���ã���������ģ���ַ�ĸ�6λ��
*/
/************������������***************/
#define mpu6050_address 0x68/*mpu6050��Ĭ�ϵ�ַ�� WHO_AM_I + AD0��WHO_AM_I�Ĵ�����Ĭ��ֵ��
                              0x68��AD0��������ʱĬ��Ϊ0��*/
#define mpu6050_AFS_SEL 0/*���ü��ٶȴ����������̣�
                           0+-2g
                           1+-4g
                           2+-8g
                           3+-16g
                           ���������ú�֮�󣬺���ĳ�ʼ���Ⱥ����Ͳ����ٿ����ˣ�����������������õ�����
                         */
#define mpu6050_FS_SEL 0/*���������ǵ����̣�
                          0+-250 deg/s
                          1+-500 deg/s
                          2+-1000 deg/s
                          3+-2000 deg/s
                          ���������ú�֮�󣬺���ĳ�ʼ���Ⱥ����Ͳ����ٿ����ˣ�����������������õ�����
                        */

/******************************������mpu6050�ڲ���ַ����**************************/
#define SMPLRT_DIV 0x19//�����ǲ����ʣ�����ֵ��0x07(125Hz)
/*����Ƶ��=���������Ƶ��/��1+SMPLRT_DIV��
  �������������Ƶ�ʣ������ֲ���˵�˵���û�����ס����ձ�������������÷�����
  ���������Ƶ��=1000Hz��*/

#define CONFIG 0x1A//��ͨ�˲�Ƶ�ʣ�����ֵ��0x06(5Hz)
/* ���幦�ܲ�̫���*/

#define GYRO_CONFIG 0x1B//�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
/*D7D6D5 D4 D3 D2D1D0
  XG_STYG_STZG_ST|FS_SEL| ---
  ��3λ�ֱ��������������Լ죬1-�Լ죬0-���Լ죨Ĭ�ϣ�
  FS_SEL�����������������̣�
  FS_SEL���������ÿ����λ1����
  0+-250 deg/s  250/32768 =  125/16384 ~= 0.007629394 deg/s
  1+-500 deg/s  500/32768 =  125/8192 ~=  0.015258789 deg/s
  2+-1000 deg/s  1000/32768 = 125/4096 ~=  0.030517578 deg/s
  3+-2000 deg/s  2000/32768 = 125/2048 ~=  0.061035156 deg/s*/

#define ACCEL_CONFIG 0x1C//���ټ��Լ졢������Χ����ͨ�˲�Ƶ�ʣ�����ֵ��0x00(���Լ죬2G��5Hz)
/*D7D6D5 D4  D3D2D1D0
  XA_STYA_STZA_ST|AFS_SEL|---
  ��3λ�ֱ������ü��ٶȴ������Լ죬1-�Լ죬0-���Լ죨Ĭ�ϣ�
  AFS_SEL�����������������̣�
  AFS_SEL���������ÿ����λ1����
  0+-2g  1/16384 g
  1+-4g  1/8192 g
  2+-8g  1/4096 g
  3+-16g  1/2048 g*/

#define ACCEL_XOUT_H 0x3B//x����ٶȵĸ�2λע�⣺��λ�͵�8λ��λ�����һ����һ���з���int������
#define ACCEL_XOUT_L 0x3C//x����ٶȵĵ�8λ
#define ACCEL_YOUT_H 0x3D//y����ٶȵĸ�2λע�⣺��λ�͵�8λ��λ�����һ����һ���з���int������
#define ACCEL_YOUT_L 0x3E//y����ٶȵĵ�8λ
#define ACCEL_ZOUT_H 0x3F//z����ٶȵĸ�2λע�⣺��λ�͵�8λ��λ�����һ����һ���з���int������
#define ACCEL_ZOUT_L 0x40//z����ٶȵĵ�8λ

#define TEMP_OUT_H 0x41
#define TEMP_OUT_L 0x42
/*�����������¶ȣ�����һ��Ҳ��һ���з���16λ���ݡ�
  Ҫ��������϶��������ʽ��
  Temperature in degrees C = (TEMP_OUT Register Value as a signed quantity)/340 + 36.53
*/

#define GYRO_XOUT_H 0x43//x����ٶȵĸ�2λע�⣺��λ�͵�8λ��λ�����һ����һ���з���int������
#define GYRO_XOUT_L 0x44//x����ٶȵĵ�8λ
#define GYRO_YOUT_H 0x45//y����ٶȵĸ�2λע�⣺��λ�͵�8λ��λ�����һ����һ���з���int������
#define GYRO_YOUT_L 0x46//y����ٶȵĵ�8λ
#define GYRO_ZOUT_H 0x47//z����ٶȵĸ�2λע�⣺��λ�͵�8λ��λ�����һ����һ���з���int������
#define GYRO_ZOUT_L 0x48//z����ٶȵĵ�8λ
#define PWR_MGMT_1 0x6B//��Դ��������ֵ��0x00(��������)
#define WHO_AM_I 0x75//IIC��ַ�Ĵ���(Ĭ����ֵ0x68��ֻ��)

/**** �����ǽӿں��� ****/
void mpu6050_init(void);
int mpu6050_getAccelXData(void);//����x����ٶȵ�ԭʼ����
int mpu6050_getAccelYData(void);//����y����ٶȵ�ԭʼ����
int mpu6050_getAccelZData(void);//����z����ٶȵ�ԭʼ����
int mpu6050_getGyroXData(void);//����x����ٶȵ�ԭʼ����
int mpu6050_getGyroYData(void);//����y����ٶȵ�ԭʼ��
int mpu6050_getGyroZData(void);//����z����ٶȵ�ԭʼ����
int mpu6050_getTemperatureData(void);//�����¶ȵ�ԭʼ����
/*******************************************************************
ע�⣺�����ԭʼ���ݵĶ�ȡ������ȷ�ģ����������ԭʼ���ݻ����ʵ������ֵ��
�������⣬Ӧ���Ǿ��Ȳ�����ɵģ�c�����ڽ��и���������ʱ���ǻ����
******************************************************************/
double mpu6050_getAccelX(void);//����x����ٶȵ�����ֵ����λ��m/s^2
double mpu6050_getAccelY(void);//����y����ٶȵ�����ֵ����λ��m/s^2
double mpu6050_getAccelZ(void);//����z����ٶȵ�����ֵ����λ��m/s^2
double mpu6050_getGyroX(void);//����x��Ľ��ٶȣ���λ�� deg/s
double mpu6050_getGyroY(void);//����y��Ľ��ٶȣ���λ�� deg/s
double mpu6050_getGyroZ(void);//����z��Ľ��ٶȣ���λ�� deg/s
float mpu6050_getTemperature(void);//�����¶ȣ���λ�����϶�

#endif
