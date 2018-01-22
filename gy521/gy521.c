#include <gy521.h>
#include <i2c.h>
#include <stdio.h>

/******************������mpu6050�������Ĵ�����������****************/
static void mpu6050_writeReg(u8 regAddress,u8 dat)//��mpu6050��ָ���Ĵ���д������
{
    /************ �������i2c�ӿں�������I2C�豸���ӻ���ַmpu6050_address)��regAddressд��һ���ֽ�����dat *************/
    //wiringPiI2CWriteReg8(gy521, regAddress, dat);
    i2c_writeReg8(mpu6050_address, regAddress, dat);
}
static u8 mpu6050_readReg(u8 regAddress)//��mpu6050��ȡָ���ļĴ���
{
    /************ �������i2c�ӿں�������I2C�豸���ӻ���ַmpu6050_address)��regAddress��ȡһ���ֽ����ݲ����� *************/
    //return (u8)wiringPiI2CReadReg8(gy521, regAddress);
    return i2c_readReg8(mpu6050_address, regAddress);
}

/*********************************************�����Ƕ��⹫���ĺ���*************************************/
void mpu6050_init(void)
{
    mpu6050_writeReg(PWR_MGMT_1,0x00);//�������״̬
    mpu6050_writeReg(SMPLRT_DIV,0x07);//���ò����ʡ�����Ƶ��=���������Ƶ��/��1+SMPLRT_DIV��
    mpu6050_writeReg(CONFIG,0x06);//���õ�ͨ�˲��������幦�ܲ����

//�������ü��ٶȴ�����������
    switch(mpu6050_AFS_SEL)
    {
    case 0://+-2g
        mpu6050_writeReg(ACCEL_CONFIG,0x00);
        break;
    case 1://+-4g
        mpu6050_writeReg(ACCEL_CONFIG,0x08);
        break;
    case 2://+-8g
        mpu6050_writeReg(ACCEL_CONFIG,0x10);
        break;
    case 3://+-16g
        mpu6050_writeReg(ACCEL_CONFIG,0x18);
        break;
    }

//�������������ǵ�����
    switch(mpu6050_FS_SEL)
    {
    case 0://+-250 deg/s
        mpu6050_writeReg(GYRO_CONFIG,0x00);
        break;
    case 1://+-500 deg/s
        mpu6050_writeReg(GYRO_CONFIG,0x08);
        break;
    case 2://+-1000 deg/s
        mpu6050_writeReg(GYRO_CONFIG,0x10);
        break;
    case 3://+-2000 deg/s
        mpu6050_writeReg(GYRO_CONFIG,0x18);
        break;
    }
}

int mpu6050_getAccelXData(void)//����x����ٶȵ�ԭʼ����
{
    u8 h,l;
    u32 temp;
    h=mpu6050_readReg(ACCEL_XOUT_H);
    l=mpu6050_readReg(ACCEL_XOUT_L);
    temp = (h<<8)+l;
    if(temp&0x8000)     //�Ǹ���
    {
        temp=~temp;
        temp&=0xffff;
        temp+=1;
        return -((int)temp);
    }
    else
        return (int)temp;
}
int mpu6050_getAccelYData(void)//����y����ٶȵ�ԭʼ����
{
    u8 h,l;
    u32 temp;
    h=mpu6050_readReg(ACCEL_YOUT_H);
    l=mpu6050_readReg(ACCEL_YOUT_L);
    temp = (h<<8)+l;
    if(temp&0x8000)     //�Ǹ���
    {
        temp=~temp;
        temp&=0xffff;
        temp+=1;
        return -((int)temp);
    }
    else
        return (int)temp;
}
int mpu6050_getAccelZData(void)//����z����ٶȵ�ԭʼ����
{
    u8 h,l;
    u32 temp;
    h=mpu6050_readReg(ACCEL_ZOUT_H);
    l=mpu6050_readReg(ACCEL_ZOUT_L);
    temp = (h<<8)+l;
    if(temp&0x8000)     //�Ǹ���
    {
        temp=~temp;
        temp&=0xffff;
        temp+=1;
        return -((int)temp);
    }
    else
        return (int)temp;
}
int mpu6050_getGyroXData(void)//����x����ٶȵ�ԭʼ����
{
    u8 h,l;
    u32 temp;
    h=mpu6050_readReg(GYRO_XOUT_H);
    l=mpu6050_readReg(GYRO_XOUT_L);
    temp = (h<<8)+l;
    if(temp&0x8000)     //�Ǹ���
    {
        temp=~temp;
        temp&=0xffff;
        temp+=1;
        return -((int)temp);
    }
    else
        return (int)temp;
}
int mpu6050_getGyroYData(void)//����y����ٶȵ�ԭʼ����
{
    u8 h,l;
    u32 temp;
    h=mpu6050_readReg(GYRO_YOUT_H);
    l=mpu6050_readReg(GYRO_YOUT_L);
    temp = (h<<8)+l;
    if(temp&0x8000)     //�Ǹ���
    {
        temp=~temp;
        temp&=0xffff;
        temp+=1;
        return -((int)temp);
    }
    else
        return (int)temp;
}
int mpu6050_getGyroZData(void)//����z����ٶȵ�ԭʼ����
{
    u8 h,l;
    u32 temp;
    h=mpu6050_readReg(GYRO_ZOUT_H);
    l=mpu6050_readReg(GYRO_ZOUT_L);
    temp = (h<<8)+l;
    if(temp&0x8000)     //�Ǹ���
    {
        temp=~temp;
        temp&=0xffff;
        temp+=1;
        return -((int)temp);
    }
    else
        return (int)temp;
}
int mpu6050_getTemperatureData(void)//�����¶ȵ�ԭʼ����
{
    u8 h,l;
    u32 temp;
    h=mpu6050_readReg(TEMP_OUT_H);
    l=mpu6050_readReg(TEMP_OUT_L);
    temp = (h<<8)+l;
    if(temp&0x8000)     //�Ǹ���
    {
        temp=~temp;
        temp&=0xffff;
        temp+=1;
        return -((int)temp);
    }
    else
        return (int)temp;
}

/*******************************************************************
ע�⣺�����ԭʼ���ݵĶ�ȡ������ȷ�ģ����������ԭʼ���ݻ����ʵ������ֵ��
�������⣬Ӧ���Ǿ��Ȳ�����ɵģ�c�����ڽ��и���������ʱ���ǻ����
*******************************************************************/
double mpu6050_getAccelX(void)//����x����ٶȵ�����ֵ����λ��m/s^2
{
    double temp;
    switch(mpu6050_AFS_SEL)
    {
    case 0://+-2g
        temp=(double)mpu6050_getAccelXData()/16384;//��ʱ�õ��ĵ�λ��g
        temp*=9.8;//�õ���λ��m/s^2
        break;
    case 1://+-4g
        temp=(double)mpu6050_getAccelXData()/8192;//��ʱ�õ��ĵ�λ��g
        temp*=9.8;//�õ���λ��m/s^2
        break;
    case 2://+-8g
        temp=(double)mpu6050_getAccelXData()/4096;//��ʱ�õ��ĵ�λ��g
        temp*=9.8;//�õ���λ��m/s^2
        break;
    case 3://+-16g
        temp=(double)mpu6050_getAccelXData()/2048;//��ʱ�õ��ĵ�λ��g
        temp*=9.8;//�õ���λ��m/s^2
        break;
    }
    return temp;
}
double mpu6050_getAccelY(void)//����y����ٶȵ�����ֵ����λ��m/s^2
{
    double temp;
    switch(mpu6050_AFS_SEL)
    {
    case 0://+-2g
        temp=(double)mpu6050_getAccelYData()/16384;//��ʱ�õ��ĵ�λ��g
        temp*=9.8;//�õ���λ��m/s^2
        break;
    case 1://+-4g
        temp=(double)mpu6050_getAccelYData()/8192;//��ʱ�õ��ĵ�λ��g
        temp*=9.8;//�õ���λ��m/s^2
        break;
    case 2://+-8g
        temp=(double)mpu6050_getAccelYData()/4096;//��ʱ�õ��ĵ�λ��g
        temp*=9.8;//�õ���λ��m/s^2
        break;
    case 3://+-16g
        temp=(double)mpu6050_getAccelYData()/2048;//��ʱ�õ��ĵ�λ��g
        temp*=9.8;//�õ���λ��m/s^2
        break;
    }
    return temp;
}
double mpu6050_getAccelZ(void)//����z����ٶȵ�����ֵ����λ��m/s^2
{
    double temp;
    switch(mpu6050_AFS_SEL)
    {
    case 0://+-2g
        temp=(double)mpu6050_getAccelZData()/16384;//��ʱ�õ��ĵ�λ��g
        temp*=9.8;//�õ���λ��m/s^2
        break;
    case 1://+-4g
        temp=(double)mpu6050_getAccelZData()/8192;//��ʱ�õ��ĵ�λ��g
        temp*=9.8;//�õ���λ��m/s^2
        break;
    case 2://+-8g
        temp=(double)mpu6050_getAccelZData()/4096;//��ʱ�õ��ĵ�λ��g
        temp*=9.8;//�õ���λ��m/s^2
        break;
    case 3://+-16g
        temp=(double)mpu6050_getAccelZData()/2048;//��ʱ�õ��ĵ�λ��g
        temp*=9.8;//�õ���λ��m/s^2
        break;
    }
    return temp;
}
double mpu6050_getGyroX(void)//����x��Ľ��ٶȣ���λ�� deg/s
{
    double temp;
    switch(mpu6050_FS_SEL)
    {
    case 0://+-250 deg/s
        temp=(double)mpu6050_getGyroXData()*0.007629394;//�õ��ĵ�λ�� deg/s
        break;
    case 1://+-500 deg/s
        temp=(double)mpu6050_getGyroXData()*0.015258789;//�õ��ĵ�λ�� deg/s
        break;
    case 2://+-1000 deg/s
        temp=(double)mpu6050_getGyroXData()*0.030517578;//�õ��ĵ�λ�� deg/s
        break;
    case 3://+-2000 deg/s
        temp=(double)mpu6050_getGyroXData()*0.061035156;//�õ��ĵ�λ�� deg/s
        break;
    }
    return temp;
}
double mpu6050_getGyroY(void)//����y��Ľ��ٶȣ���λ�� deg/s
{
    double temp;
    switch(mpu6050_FS_SEL)
    {
    case 0://+-250 deg/s
        temp=(double)mpu6050_getGyroYData()*0.007629394;//�õ��ĵ�λ�� deg/s
        break;
    case 1://+-500 deg/s
        temp=(double)mpu6050_getGyroYData()*0.015258789;//�õ��ĵ�λ�� deg/s
        break;
    case 2://+-1000 deg/s
        temp=(double)mpu6050_getGyroYData()*0.030517578;//�õ��ĵ�λ�� deg/s
        break;
    case 3://+-2000 deg/s
        temp=(double)mpu6050_getGyroYData()*0.061035156;//�õ��ĵ�λ�� deg/s
        break;
    }
    return temp;
}
double mpu6050_getGyroZ(void)//����z��Ľ��ٶȣ���λ�� deg/s
{
    double temp;
    switch(mpu6050_FS_SEL)
    {
    case 0://+-250 deg/s
        temp=(double)mpu6050_getGyroZData()*0.007629394;//�õ��ĵ�λ�� deg/s
        break;
    case 1://+-500 deg/s
        temp=(double)mpu6050_getGyroZData()*0.015258789;//�õ��ĵ�λ�� deg/s
        break;
    case 2://+-1000 deg/s
        temp=(double)mpu6050_getGyroZData()*0.030517578;//�õ��ĵ�λ�� deg/s
        break;
    case 3://+-2000 deg/s
        temp=(double)mpu6050_getGyroZData()*0.061035156;//�õ��ĵ�λ�� deg/s
        break;
    }
    return temp;
}
float mpu6050_getTemperature(void)//�����¶ȣ���λ�����϶�
{
    return (float)mpu6050_getTemperatureData()/340 + 36.53;
}

