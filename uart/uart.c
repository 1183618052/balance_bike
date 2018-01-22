#include <uart.h>
#include <stm32f10x_lib.h>

void uart2_init(void)
{
    /*����uart����GPIO��ʱ�ӣ�����uart���õ���������±�
      ���ں�    RXD     TXD
      1         PA10    PA9
      2         PA3     PA2
      3         PB11    PB10
      4         PC11    PC10
      5         PD2     PC12     */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    /*��������ʱ�ӡ�
      ����1��APB2�����ϣ�����2~5��APB1�����ϡ�  */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    /* �ֱ������õ�������GPIO���ŵ�ģʽ */
    {
        GPIO_InitTypeDef inittype_GPIOA;
        //TXD����λ���츴��ģʽ
        inittype_GPIOA.GPIO_Speed=GPIO_Speed_50MHz;
        inittype_GPIOA.GPIO_Mode=GPIO_Mode_AF_PP;
        inittype_GPIOA.GPIO_Pin = GPIO_Pin_2; //TXD����
        GPIO_Init(GPIOA, &inittype_GPIOA);
        //RXD����Ϊ�����������������
        inittype_GPIOA.GPIO_Speed=GPIO_Speed_50MHz;
//        inittype_GPIOA.GPIO_Mode=GPIO_Mode_IN_FLOATING; //����Ϊ��������Ļ����� GPIO_Mode_IPU
        inittype_GPIOA.GPIO_Mode=GPIO_Mode_IPU;
        inittype_GPIOA.GPIO_Pin = GPIO_Pin_3; //RXD����
        GPIO_Init(GPIOA, &inittype_GPIOA);
    }
    /* ��ʼ������ */
    {
        USART_InitTypeDef temp;
        temp.USART_BaudRate = 115200;
        temp.USART_WordLength = USART_WordLength_8b;
        temp.USART_StopBits = USART_StopBits_1;
        temp.USART_Parity = USART_Parity_No;
        temp.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //���ͺͽ��ն�ʹ��
        temp.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
        USART_Init(USART2, &temp);
    }
    /* ��������жϵĻ���������һ�
       ����һ������ǰ��Ӧ������nvic��ͷ�ļ���c�ļ����룬���ڳ����ʼ���ú��˷��顣*/
    {
        NVIC_InitTypeDef temp;
        temp.NVIC_IRQChannel = USART2_IRQChannel; //����USART1���жϣ�2��3��4��5ֻҪ�ĸ���ż���
        temp.NVIC_IRQChannelPreemptionPriority = 3;
        temp.NVIC_IRQChannelSubPriority = 3;    //���ȼ����
        temp.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&temp);
    }
    /* ��������жϵĻ���������һ�
       ���ô��ڸ����жϿ��أ��ڶ����������ж����ͣ��������жϣ�
       USART_IT_PE      ��ż�����ж�
       USART_IT_TXE     �����ж�
       USART_IT_TC      ��������ж�
       USART_IT_RXNE    �����ж�
       USART_IT_IDLE    ���������ж�
       USART_IT_LBD     LIN �жϼ���ж�
       USART_IT_CTS     CTS �ж�
       USART_IT_ERR     �����ж�
       ���ϸ��������жϿɷֱ����ÿ��أ����е��ж϶���һ���жϴ�������USART1_IRQHandler��
    */
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); //�����ж�
    /* ʹ�ܴ��� */
    USART_Cmd(USART2, ENABLE);
}
void uart2_sendB(u8 dat)
{
    USART_SendData(USART2, dat);
    while(SET!=USART_GetFlagStatus(USART2, USART_FLAG_TC));     //�ȴ��������
}
void uart2_sendStr(u8 *p)
{
    for(;*p!='\0';p++)
        uart2_sendB(*p);
}

void USART2_IRQHandler(void)
{
    if(USART_GetITStatus(USART2, USART_IT_RXNE)) //�����ж��ǽ��������ж�
    {
        u8 get;
        void get_control_dat(u8 dat);
        get = USART_ReceiveData(USART2);
        //�յ����������ﴦ��
        get_control_dat(get);
    }
}

