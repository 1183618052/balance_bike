#include <disp.h>
#include <uart.h>
#include <stdio.h>

void disp_clean(void)
{
    printf("\033[2J");
}
void disp_clean_line(void)      //�����ǰ�У�����λ��굽����
{
    printf("\33[2K\r");
}
void disp_cursor_reset(void)        //��λ���λ�ã��ص����Ͻ�
{
    printf("\033[H");
}
void disp_gotoxy(int x, int y)  //��ת��y��x��
{
    printf("\033[%d;%dH", y, x);
}
void disp_cursor_up(int y)      //����x��
{
    printf("\033[%dA", y);
}
void disp_cursor_down(int y)    //����y��
{
    printf("\033[%dB", y);
}
void disp_cursor_left(int x)    //����x��
{
    printf("\033[%dD", x);
}
void disp_cursor_right(int x)   //����x��
{
    printf("\033[%dC", x);
}
void disp_cursor_hide(void)     //���ع��
{
    printf("\033[?25l");
}
void disp_cursor_show(void)     //��ʾ���
{
    printf("\033[?25h");
}

