#ifndef __DISP_H__
#define __DISP_H__
/* ���������Ѿ���ɴ��ڳ�ʼ���������ض���printf����֮��
   �����ṩһЩ�к������ڲ����ն˹�����ʾ�������ն˹����ﹹ�����档 */
#include <stm32f10x_lib.h>

void disp_clean(void);
void disp_clean_line(void);      //�����ǰ�У�����λ��굽����
void disp_cursor_reset(void);        //��λ���λ�ã��ص����Ͻǣ�1��1�У�
void disp_gotoxy(int x, int y);  //��ת��y��x�У�x��y���Ǵ�1��ʼ����
void disp_cursor_up(int y);      //����x��
void disp_cursor_down(int y);    //����y��
void disp_cursor_left(int x);    //����x��
void disp_cursor_right(int x);   //����x��
void disp_cursor_hide(void);     //���ع�꣬��secureCRT�в�����Ч��
void disp_cursor_show(void);     //��ʾ��꣬��secureCRT�в�����Ч��

#endif
