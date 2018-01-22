/* ��ǰ����0�ȣ������Ǹ����������� */
#include <angle.h>

int mk_mid = 1500+140;        //�����м�ֵ��Ҳ����0�ȵ�����ֵ����λ��us

void angle_init()
{
    /* ������ʱ����ͨ���������£�
       ��ʱ��  TIMx_CH1    TIMx_CH2    TIMx_CH3    TIMx_CH4
       TIM1    PA8         PA9         PA10        PA11
       TIM2    PA0         PA1         PA2         PA3
       TIM3    PA6         PA7         PB0         PB1
       TIM4    PB6         PB7         PB8         PB9
       TIM5    PA0         PA1         PA2         PA3
       TIM6    -           -           -           -
       TIM7    -           -           -           -
       TIM8    PC6         PC7         PC8         PC9     */
    /* ʹ�ܶ�ʱ��ʱ��ģ�顣
       �߼���ʱ��TIM1��TIM8���ڸ���ʱ����APB2�ϣ�ͨ�ö�ʱ��TIM2~TIM7���ڵ���ʱ����APB1�ϡ�
       ��Ȼ��ͬ�Ķ�ʱ�����ܹ��ڲ�ͬ��APB�����ϣ�����ͨ����APB�����У�������ʱ����ʱ�Ӷ���72MHz������ϸԭ����ʼǣ� */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    /* ���ö�ʱ��ʱ��Դ */
    // ��ʱ��Ĭ��ʱ����ͨ��APB�����ģ��������ﲻ��Ҫ����
    /* ʹ�ܽ�Ҫ��Ϊpwm�����GPIO��ʱ�ӣ�����������鵽 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    /* ���ý�Ҫ��Ϊpwm��������ŵ����ģʽΪ"���츴�����" */
    {
        GPIO_InitTypeDef temp;
        temp.GPIO_Speed = GPIO_Speed_50MHz;
        temp.GPIO_Mode = GPIO_Mode_AF_PP;
        temp.GPIO_Pin = GPIO_Pin_6;     //�������������ö�Ӧ������
        GPIO_Init(GPIOB, &temp);
    }
    /* ���ö�ʱ������������Ԫ */
    {
        TIM_TimeBaseInitTypeDef temp;
        temp.TIM_Prescaler = 72-1;  /* Ԥ��Ƶ�Ĵ�����[0, 65535]���������Ǹ�tim��APB������Ƶ�ʶ���72MHz������72-1���õ���1MHz��
                                       72Mhz/(TIM_Prescaler+1) = CK_CNT����������ʱ��Ƶ�ʣ���TIM_Prescaler = 72MHz/CK_CNT - 1 */
        temp.TIM_Period = 20000-1;     /* �Զ�װ�ؼĴ�����[0, 65535]������100-1���õ���100��������������Ϊ������ڡ�
                                          ����Ԥ��Ƶ���úü��������ں�(1/CK_CNT)��������ڵ�(TIM_Period+1)�����Ǽ�������������ڡ�*/
        //���������������һ��������ڹ�ʽ��������� = (TIM_Prescaler+1)*(TIM_Period+1)/72000000����λ��s��
        temp.TIM_CounterMode = TIM_CounterMode_Up; /* ����ģʽ��
                                                      TIM_CounterMode_Up                TIM ���ϼ���ģʽ
                                                      TIM_CounterMode_Down              TIM ���¼���ģʽ
                                                      TIM_CounterMode_CenterAligned1    TIM �������ģʽ 1 ����ģʽ
                                                      TIM_CounterMode_CenterAligned2    TIM �������ģʽ 2 ����ģʽ
                                                      TIM_CounterMode_CenterAligned3    TIM �������ģʽ 3 ����ģʽ  */
        temp.TIM_ClockDivision = TIM_CKD_DIV1; /* ������Ƶ�����벶��ʱʹ�á� */
        TIM_TimeBaseInit(TIM4, &temp);
    }
    /* ��ʼ��ָ����ʱ��ָ��ͨ��������Ƚϲ��� */
    {
        TIM_OCInitTypeDef temp;
        temp.TIM_OCMode = TIM_OCMode_PWM1;      //��ģʽ����ռ�ձ�Ϊ100%ʱ�޷������ȫ�ĸߵ�ƽ�����������һ�����ڵĵ͵�ƽ
        temp.TIM_OutputState = TIM_OutputState_Enable;  //ʹ��
        temp.TIM_Pulse = mk_mid;    //�ȽϼĴ�����ֵ
        temp.TIM_OCPolarity = TIM_OCPolarity_High;      //��Ч��ƽΪ�ߵ�ƽ
        TIM_OC1Init(TIM4, &temp);
    }
    /* ʹ��ָ����ʱ��ָ��ͨ����Ԥװ�ؼĴ��� */
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
    /* ʹ��ָ����ʱ�����Զ���װ�ص�Ԥװ�ؼĴ�������λ */
    TIM_ARRPreloadConfig(TIM4, DISABLE); //ENABLE-�ȽϼĴ���ֵ�޸ĺ�������Ч��DISABLE-�ȽϼĴ���ֵ�޸ĺ��¸�������Ч
    /* ʹ�ܶ�ʱ�� */
    TIM_Cmd(TIM4, ENABLE);
}

static void tim4_setmk(int mk)  //����������λ��us����Χ��0~20000
{
    TIM_SetCompare1(TIM4, mk);
}

void angle_setangle(int deg)    //���ýǶȣ���λ�Ƕȡ���ǰ����0�������Ǹ�����������
{
    #define ANGLE_LIMIT 40
    double mkPdeg = 2000/180;   //ÿһ�ȽǶȶ�Ӧ������仯��
    if(deg>ANGLE_LIMIT) deg = ANGLE_LIMIT;
    else if(deg < -ANGLE_LIMIT) deg = -ANGLE_LIMIT;
    tim4_setmk(mk_mid - mkPdeg*deg);
}

