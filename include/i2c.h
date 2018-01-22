#ifndef __I2C_IO_H__
#define __I2C_IO_H__
#include <stm32f10x_lib.h>

void i2c_init(void);    //���IO�ĳ�ʼ��
/*
  ��ʼ + �ӻ���ַд + �ڲ���ַ��cmd + length������ + ����
  device_addr��7λ��i2c�ӻ���ַ��
  lengthָ����data�����ݳ��ȣ���������뷢��cmd_addr��ͽ�������lengthָΪ0���ɡ�
*/
void i2c_master_write(u8 device_addr, u8 cmd_addr, u8 *dat, u8 length);

/*  ֻдһ�������д�����ݡ� */
void i2c_writeCmd(u8 device_addr, u8 cmd);

/*  ���ڲ�ָ���Ĵ���дһ���ֽ����ݡ� */
void i2c_writeReg8(u8 device_addr, u8 cmd_addr, u8 dat);

/*  ���ڲ�ָ���Ĵ���д2���ֽ����ݣ���8λ�ڵ͵�ַ����8λ�ڸߵ�ַ�� */
void i2c_writeReg16(u8 device_addr, u8 cmd_addr, u16 dat);

/*
  ��ʼ + �ӻ���ַд + �ڲ���ַ��cmd + ��ʼ + �ӻ���ַ�� + ��ȡlength������ + ����
  ��ȡ�ӻ���ַָ���Ĵ�����ָ�����ȵ����ݡ�
  device_addr��7λ��i2c�ӻ���ַ��
  cmd_addr���ڲ���ַ����һ���ֽ���ʱҲ����Ϊ��cmd��
  ��ȡlength�����ݷŵ�datָ��ĵط�
*/
void i2c_master_read(u8 device_addr, u8 cmd_addr, u8 *dat, u8 length);

/*  ��ȡ�ӻ���ַָ���Ĵ�����һ���ֽ����ݡ� */
u8 i2c_readReg8(u8 device_addr, u8 cmd_addr);

/*
  ��ȡָ���Ĵ���2���ֽڣ�����һ��16λ�����ص�ַ�ڵ�8λ���ߵ�ַ�ڸ�8λ�� */
u16 i2c_readReg16(u8 device_addr, u8 cmd_addr);
#endif
