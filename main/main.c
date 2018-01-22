#include <stm32f10x_lib.h>
#include <stdio.h>
#include <i2c.h>
#include <gy521.h>
#include <uart.h>
#include <disp.h>
#include <motor.h>
#include <angle.h>
#include "balance.c"
#include "control.c"

int flag_disp=0;        //��ʱ��ͨ���˱���֪ͨ��ѭ��ˢ����ʾ��1-ˢ��
int disp_type = 0;      //0-��������ʾ���棬1-�ֱ���ʾ����

int fputc(int ch, FILE *f)
{
    USART_SendData(USART2, (u8)ch);
    while (SET!=USART_GetFlagStatus(USART2, USART_FLAG_TC));
    return (ch);
}

void delay_init() //��ϵͳ��ʱ����ѯ��ʽ��ʱ
{
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); //���ó�AHB��8��Ƶ��Ҳ����9MHz
    SysTick_ITConfig(DISABLE);
}
void udelay(u32 n) //��ʱus�����뷶Χ 1~1864135
{
    //����SysTickʱ������Ϊ9MHz��������ʱ1us��Ҫ���õļĴ����� 9
    u32 reg;
    reg = 9*n;
    SysTick_CounterCmd(SysTick_Counter_Clear); //��ռ�����
    SysTick_SetReload(reg); //���üĴ���
    SysTick_CounterCmd(SysTick_Counter_Enable); //����������
    while(SysTick_GetFlagStatus(SysTick_FLAG_COUNT) == RESET); //�ȴ���ʱ����
    SysTick_CounterCmd(SysTick_Counter_Disable); //ֹͣ��ʱ��
}
void mdelay(u16 n) //��ʱms�����뷶Χ 1~1864
{
    //����SysTickʱ������Ϊ9MHz��������ʱ1ms��Ҫ���õļĴ����� 9000
    u32 reg;
    reg = 9000*n;
    SysTick_CounterCmd(SysTick_Counter_Clear); //��ռ�����
    SysTick_SetReload(reg); //���üĴ���
    SysTick_CounterCmd(SysTick_Counter_Enable); //����������
    while(SysTick_GetFlagStatus(SysTick_FLAG_COUNT) == RESET); //�ȴ���ʱ����
    SysTick_CounterCmd(SysTick_Counter_Disable); //ֹͣ��ʱ��
}
int test_pin = 0;
void test_pin_init()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    {
        GPIO_InitTypeDef temp;
        temp.GPIO_Speed = GPIO_Speed_50MHz;
        temp.GPIO_Mode = GPIO_Mode_Out_PP;
        temp.GPIO_Pin = GPIO_Pin_12;
        GPIO_Init(GPIOB, &temp);
    }
}
void set_test_pin(int a)
{
    if(a)
    {
        GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_SET);
        test_pin = 1;
    }
    else
    {
        GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_RESET);
        test_pin = 0;
    }
}
void turn_test_pin()
{
    if(!test_pin)
    {
        GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_SET);
        test_pin = 1;
    }
    else
    {
        GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_RESET);
        test_pin = 0;
    }
}

void RCC_init()
{
    ErrorStatus HSEStartUpStatus;    //��Ϊ�洢HSE��ʼ��״̬����ʱ����
    RCC_DeInit();    //��λ
    RCC_HSEConfig(RCC_HSE_ON);    //�����ⲿ���پ���HSE
    HSEStartUpStatus = RCC_WaitForHSEStartUp();    //�ȴ�HSE����ok��ʧ��
    if(HSEStartUpStatus == SUCCESS)    //����ok
    {
        //PLL
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);    //����PLLʱ������ΪHSE/1��9��Ƶ������HSE��8MHz������PLL���Ϊ72MHz
        RCC_PLLCmd(ENABLE);        //ʹ��PLL
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);    //�ȴ�PLL����ȶ�
     
        //Flash��ע�⣡����SYSCLK��ʼ��֮ǰһ��Ҫ����flash��ʱ���ں�Ԥȡָ���
        FLASH_SetLatency(FLASH_Latency_2);    //������ʱ����
        /*FLASHʱ���ӳټ������ڣ��ȴ�����ͬ��������
          �Ƽ����յ�Ƭ��ϵͳ����Ƶ�ʣ���MHzʱ��ȡLatency=0��
          24��MHzʱ��ȡLatency=1��
          48~72MHzʱ��ȡLatency=2*/
        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);    //Ԥȡָ����ʹ��
     
        //SYSCLK
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);    //ѡ��PLLʱ����ΪSYSCLKʱ��Դ
        while(RCC_GetSYSCLKSource() != 0x08);        //�ȴ�PLLʱ�ӳ�ΪSYSCLKʱ��Դ
        //AHB
        RCC_HCLKConfig(RCC_SYSCLK_Div1);    //����AHB��ʱ��ԴΪSYSCLK 1��Ƶ��Ҳ����72MHz
        RCC_PCLK1Config(RCC_HCLK_Div2);        //����APB1ΪHCLK 2��Ƶ��Ҳ����36MHz
        RCC_PCLK2Config(RCC_HCLK_Div1);        //����APB2ΪHCLK 1��Ƶ��Ҳ����72MHz
    }
}

void timer2_init()
{
    /* ʹ�ܶ�ʱ��ʱ��ģ�顣
       �߼���ʱ��TIM1��TIM8���ڸ���ʱ����APB2�ϣ�ͨ�ö�ʱ��TIM2~TIM7���ڵ���ʱ����APB1�ϡ�
       ��Ȼ��ͬ�Ķ�ʱ�����ܹ��ڲ�ͬ��APB�����ϣ�����ͨ����APB�����У�������ʱ����ʱ�Ӷ���72MHz������ϸԭ����ʼǣ� */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    /* ���ö�ʱ��ʱ��Դ */
    // ��ʱ��Ĭ��ʱ����ͨ��APB�����ģ��������ﲻ��Ҫ����
    /* ���ö�ʱ������������Ԫ */
    {
        TIM_TimeBaseInitTypeDef temp;
        temp.TIM_Prescaler = 72-1;  /* Ԥ��Ƶ�Ĵ�����[0, 65535]���������Ǹ�tim��APB������Ƶ�ʶ���72MHz��
                                       72Mhz/(TIM_Prescaler+1) = CK_CNT����������ʱ��Ƶ�ʣ���TIM_Prescaler = 72MHz/CK_CNT - 1 */
        temp.TIM_Period = 20000-1;     /* �Զ�װ�ؼĴ�����[0, 65535]��
                                          ����Ԥ��Ƶ���úü��������ں�(1/CK_CNT)��������ڵ�(TIM_Period+1)�����Ǽ�������������ڡ�*/
        //���������������һ��������ڹ�ʽ��������� = (TIM_Prescaler+1)*(TIM_Period+1)/72000000����λ��s��
        temp.TIM_CounterMode = TIM_CounterMode_Up; /* ����ģʽ��
                                                      TIM_CounterMode_Up                TIM ���ϼ���ģʽ
                                                      TIM_CounterMode_Down              TIM ���¼���ģʽ
                                                      TIM_CounterMode_CenterAligned1    TIM �������ģʽ 1 ����ģʽ
                                                      TIM_CounterMode_CenterAligned2    TIM �������ģʽ 2 ����ģʽ
                                                      TIM_CounterMode_CenterAligned3    TIM �������ģʽ 3 ����ģʽ  */
        temp.TIM_ClockDivision = TIM_CKD_DIV1; /* ������Ƶ�����벶��ʱʹ�á� */
        TIM_TimeBaseInit(TIM2, &temp);
    }
    /* ���ö�ʱ���жϣ�������ж� */
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    /* ����nvic��ǰ�����Ѿ��ڳ����ʼִ���� NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2) �����˷��� */
    {
        NVIC_InitTypeDef temp;
        temp.NVIC_IRQChannel = TIM2_IRQChannel;
        temp.NVIC_IRQChannelPreemptionPriority = 1;
        temp.NVIC_IRQChannelSubPriority = 1;
        temp.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&temp);
    }
    /* ʹ�ܶ�ʱ�� */
    TIM_Cmd(TIM2, ENABLE);
}
void TIM2_IRQHandler(void)      //20msһ���ж�
{
    set_test_pin(1);
    if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)     //������������¼�
    {
        static int count = 0;
        count ++;
        if(count == 5)  //100ms
        {
            count = 0;
            flag_disp = 1;
        }
        balance_tick();
        
        TIM_ClearITPendingBit(TIM2, TIM_FLAG_Update);  // ������һ��Ҫ�����־λ
        //�����ж���ʲôҲ������ֻҪtim���жϴ��ˣ����жϴ�������һ��Ҫ������һ�������ѱ�־λ�����������ܻ�Ӱ�쵽����ģ�鶼��������������
    }
    set_test_pin(0);
}

int main(int argc, char *argv[])
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    RCC_init();
    delay_init();
    test_pin_init();
    uart2_init();
    i2c_init();
    mdelay(500);mpu6050_init();
    motor_init();
    angle_init();
    timer2_init();
    balance_init();

    motor_setzkb(100);
    disp_clean();
    while(1)
    {
        if(flag_disp)
        {
            flag_disp = 0;
            if(disp_type == 0)  //��������ʾ
            {
                disp_gotoxy(1,1);
                disp_clean_line();
                printf("sensordata: ax=%.3f \tay=%.3f \taz=%.3f \tgx=%.3f \tgy=%.3f \tgz=%.3f\n\r", sensor_raw_data.ax, sensor_raw_data.ay, sensor_raw_data.az, sensor_raw_data.gx, sensor_raw_data.gy, sensor_raw_data.gz);
                disp_gotoxy(1, 2);
                disp_clean_line();
                printf("attitude: angle_a=%.3f \tangle_g=%.3f   \tangle=%.3f \tangle_car=%.3f\n\r", attitude_data.angle_a, attitude_data.angle_g, attitude_data.angle, attitude_data.angle_car);
#if BALANCE_TYPE == 1
                disp_gotoxy(1,3);
                disp_clean_line();
                printf("balance: balance_angle=%.3f \tkp=%.3f \tkd=%.3f \td_angle=%.3f \tax=%.3f \tangle=%.3f \n\r", balance_data.balance_angle, balance_data.kp, balance_data.kd, balance_data.d_angle, balance_data.ax, balance_data.angle);
#elif BALANCE_TYPE == 2
                disp_gotoxy(1,3);
                disp_clean_line();
                printf("balance: balance_angle=%.3f \tkp=%.3f \tkd=%.3f \td_angle=%.3f \tax=%.3f \tangle=%.3f \n\r", balance_data.balance_angle, balance_data.kp, balance_data.kd, balance_data.d_angle, balance_data.ax, balance_data.angle);
#endif
            }
            else if(disp_type == 1)     //�ֱ�����ʾ
            {
#if BALANCE_TYPE == 1
                disp_gotoxy(8, 1);
                disp_clean_line();
                switch(focus)
                {
                case 0: printf("kp"); break;
                case 1: printf("kd"); break;
                case 2: printf("L"); break;
                case 3: printf("Lm"); break;
                case 4: printf("V"); break;
                }
                disp_gotoxy(1,2);
                printf("kp=%.3f  ", balance_data.kp);
                disp_gotoxy(8,2);
                printf("kd=%.3f  ", balance_data.kd);
                disp_gotoxy(1,3);
                printf("L=%.3f  ", balance_data.L);
                disp_gotoxy(8,3);
                printf("Lm=%.3f  ", balance_data.Lm);
                disp_gotoxy(1,4);
                printf("V=%.3f  ", balance_data.V);
                disp_gotoxy(8,4);
                printf("j=%.3f", balance_data.balance_angle);
#elif BALANCE_TYPE == 2
                disp_gotoxy(8, 1);
                disp_clean_line();
                switch(focus)
                {
                case 0: printf("kp"); break;
                case 1: printf("kd"); break;
                }
                disp_gotoxy(1,2);
                printf("kp=%.3f  ", balance_data.kp);
                disp_gotoxy(1,3);
                printf("kd=%.3f  ", balance_data.kd);
#endif
            }
        }
    }
}
