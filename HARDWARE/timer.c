#include "timer.h"
#include "delay.h"

/***********************************************************************************************
* TIM4
***********************************************************************************************/
extern vu16 USART2_RX_STA;
extern vu16 USART3_RX_STA;
u8 count_tim4=0;
u8 count_tim4h=0;
//����TIM4Ԥװ������ֵ
void TIM4_SetARR(u16 period)
{
     TIM_SetCounter(TIM4,0);//���������
     TIM4->ARR&=0x00;       //����Ԥװ������ֵΪ0
     TIM4->ARR|= period;    //����Ԥװ������ֵ
}
//ͨ�ö�ʱ��4�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��4!
void TIM4_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM4 , ENABLE); //ʱ��ʹ��

    //��ʱ��TIM4��ʼ��
    TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
    TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

    TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM4�ж�,���������ж�

    //�ж����ȼ�NVIC����
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //�����ȼ�2��
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
    NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���

    TIM_Cmd(TIM4, ENABLE);  //ʹ��TIMx
}

//��ʱ��4�жϷ������
void TIM4_IRQHandler(void)   //TIM4�ж�
{
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //���TIM4�����жϷ������
    {
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //���TIMx�����жϱ�־
        USART2_RX_STA|=1<<15;    //��ǽ������
        TIM_Cmd(TIM4, DISABLE); //�ر�TIM4 
    }
}

/***********************************************************************************************
* TIM7
***********************************************************************************************/
//��ʱ��7�жϷ������            
void TIM7_IRQHandler(void)
{   
    if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)//�Ǹ����ж�
    {                  
        USART3_RX_STA|=1<<15;   //��ǽ������
        TIM_ClearITPendingBit(TIM7, TIM_IT_Update  );  //���TIM7�����жϱ�־    
        TIM_Cmd(TIM7, DISABLE);  //�ر�TIM7 
    }       
}
 
//ͨ�ö�ʱ��7�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ42M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz 
//ͨ�ö�ʱ���жϳ�ʼ��
//����ʼ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��         
void TIM7_Int_Init(u16 arr,u16 psc)
{   
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);//TIM7ʱ��ʹ��    
    
    //��ʱ��TIM7��ʼ��
    TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ   
    TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
    TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM7�ж�,���������ж�
    
    TIM_Cmd(TIM7,ENABLE);//������ʱ��7
    
    NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;      //�����ȼ�2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure); //����ָ���Ĳ�����ʼ��VIC�Ĵ���
}