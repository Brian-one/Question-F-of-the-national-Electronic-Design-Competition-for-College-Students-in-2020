/*******************************************************************************
*            �������� IAR7.2 for msp430f5529
*            ����ƽ̨ msp430f5529
*-------------------------------------------------------------------------------
* LMT70               ֱ��ʹ�õ�ADC ����  P6.1  
*-------------------------------------------------------------------------------
* MLX90614            I2C0:  SDA      SCL
*                            P3.0     P3.1
*                            P4.1     P4.2
*-------------------------------------------------------------------------------
* OLED(������ʹ��LCD)   D0    D1    RES    DC
*                      P4.3  P4.0  P3.7   P8.2
*-------------------------------------------------------------------------------
* UCA1                 TX    RX
*                      P44   P45
*-------------------------------------------------------------------------------
* LCD                  SCL   SDI   CS    RST   DC
*                      P3.2  P2.2  P2.7  P2.4  P2.0
*
* ���ſɸ�����������
*-------------------------------------------------------------------------------
* LMT70���ݵĲɼ���Ҫ�� LMT70.c �� LMT70.h �� 
* MLX90614���ݵĲɼ���Ҫ�� msp430f5529_i2c.c ĩβ��
* �˲��㷨��Ҫ�� Filter.c �� Filter.h ��
********************************************************************************/

#include "include.h"
#include "stdio.h"

//�¶���ֵ
#define MIN_Temp 30.0
#define MAX_Temp 46.0

//��ʼ�¶���ֵ
float Threshold_temp = 32.0;

//�����¶���ʾ��ֵ
float Lcd_stop_temp = 0.0;

//���²������ڴ��ڽ�������
char databuf[256];
static int num = 0;

//��� ����������ѧ����
uint16_t data=1000;
float fdata=1.0;

//��� ����������ѧ����
__interrupt void TIMER1_A0_IRQ_Handler()
{
   GPIO_TurnBit  (P4, 7);
   data++;  
   fdata+=0.1;
}

//����
__interrupt void UART_RX_IRQ_Handler()
{ 
  if(UART_GetITStatus(UART1,UART_RX_IRQn) == TRUE)   //�������ĳһ���жϱ�־
  {
    //�������ʸ�ʱ�������ж��ﲻҪ�������¼�������ᶪʧ���ݣ�������ʱ��ҲҪ���һ��
    databuf[num]=UART_GetChar     (UART1);                 //��ȡһ���ֽ�1���ֽ�
    num++;
    UART_ClearITPendingBit(UART1,UART_RX_IRQn);    //�������ĳһ���жϱ�־
  }
}

//���ܰ�����ʼ��
void function_key()
{
  KEY_Init(KEY1,NULL); 	                //������ʼ������ P2.1  Threshold_temp ++
  KEY_Init(KEY2,NULL); 	                //������ʼ������ P1.1  Threshold_temp --
  KEY_Init(KEY3,NULL); 	                //������ʼ������ P8.1  ����ͨ�� �ַ���1 ʶ������
  KEY_Init(KEY4,NULL); 	                //������ʼ������ P6.2  KEY4 ����ͨ�� �ַ���2 ѧϰ����
  KEY_Init(KEY5,NULL); 	                //������ʼ������ P6.3
  //KEY_Init(KEY6,NULL); 	        //������ʼ������ P6.4
}

//�¶ȱ���
void Temp_warn()
{
  if(Mlx90614_Filter() > Threshold_temp)
  {
    GPIO_ResetBits  (PORT6, GPIO_Pin_0);     //����ߵ�ƽ ��������
  }
  if(Mlx90614_Filter() <= Threshold_temp)
  {
    GPIO_SetBits(PORT6, GPIO_Pin_0);         //����͵�ƽ ����������
  }
}

//����׼��
void JGZX()
{
  GPIO_ResetBits  (PORT7, GPIO_Pin_0);
  DELAY_MS(400);
  GPIO_SetBits(PORT7, GPIO_Pin_0);
}

//��������
void Key_set()
{
  //KEY1������ֵ Threshold_temp
     if(KEY_isPressed(KEY1) && (Threshold_temp < MAX_Temp) && (Threshold_temp >= MIN_Temp))
    {
      Threshold_temp ++;
    }
    //KEY2��С��ֵ Threshold_temp
    if(KEY_isPressed(KEY2) && (Threshold_temp <= MAX_Temp) && (Threshold_temp > MIN_Temp))
    {
      Threshold_temp --;
    }
    //KEY3 ����ͨ�� �ַ���1 ʶ������
    if(KEY_isPressed(KEY3))
    {
      UART_Printf   (UART1,"1");
      LCD_Printf_8x16(0,55,"Judge Face&Mask!");
      //OLED_PrintfAt(FONT_ASCII_8X16,6,0,"Judge Face&Mask!");
      
    }
    //KEY4 ����ͨ�� �ַ���2 ѧϰ����
    if(KEY_isPressed(KEY4))
    {
     UART_Printf   (UART1,"2");
     LCD_Printf_8x16(0,55,"Learning Faces!  ");
     //OLED_PrintfAt(FONT_ASCII_8X16,6,0,"Learning Faces!  ");
    }
    /********************************************
    //KEY5 ����ͨ�� �ַ���3 �ж��Ƿ������
    if(KEY_isPressed(KEY5))
    {
     UART_Printf   (UART1,"3");
     OLED_PrintfAt(FONT_ASCII_8X16,6,0,"Judge Mask!      ");
    }
    ********************************************/
    //KEY5 ���º�������ʾ
    if(KEY_isPressed(KEY5))
    {
      Lcd_stop_temp = Mlx90614_Filter();
      LCD_Printf_8x16(0,87,"Temp: %.1f C",Lcd_stop_temp);       //��ʾMLX90614�¶�����
      JGZX();
      //OLED_DispChineseAt(FONT_CHINESE_16X16,2,0,"�¶�");          //39
      //OLED_PrintfAt(FONT_ASCII_8X16,2,32,":%.1f",Oled_stop_temp);//��ʾMLX90614�¶�����
      //OLED_DispChineseAt(FONT_CHINESE_16X16,2,72,"��");
    }
}

//��������һ��
void FMQS()
{
  //������
  GPIO_ResetBits  (PORT6, GPIO_Pin_0);     //����ߵ�ƽ ��������
  DELAY_MS(200);
  GPIO_SetBits(PORT6, GPIO_Pin_0);         //����͵�ƽ ����������
}

//���ڽ���open mv����
void open_mv_data()
{
  //������    databuf[0] = '6'
  //��������  databuf[0] = '5'
  //UART_Printf   (UART1," Hello-CTGU!\n UART-TEST!\n");  //���ڷ�������
  if(num > 0)
  {
    //OLED_PrintfAt(FONT_ASCII_8X16,0,0,"%s",databuf);    //�����ڽ��յ�������ʾ���� ע���������ʾ�����ݾ����ַ��������ַ���
    if(databuf[0] == '5')
    {
      LCD_Printf_8x16(0,103,"NO MASK!!!      ");
    }
    if(databuf[0] == '6')
    {
      LCD_Printf_8x16(0,103,"MASK YES        ");
    }
    if(databuf[0] == '-')
    {
      LCD_Printf_8x16(0,103,"%s",databuf);      ////�����ڽ��յ�������ʾ���� ע���������ʾ������
      GPIO_ResetBits  (PORT6, GPIO_Pin_0);     //����ߵ�ƽ ��������
      DELAY_MS(6000);
      GPIO_SetBits(PORT6, GPIO_Pin_0);         //����͵�ƽ ����������
    }
    if(databuf[0] == 'P' || databuf[0] == 'S' || databuf[0] == 'N')
    {
      FMQS();
      LCD_Printf_8x16(0,103,"%s",databuf);      ////�����ڽ��յ�������ʾ���� ע���������ʾ������
      DELAY_MS(5000);
    }
    for(int i=0;i < num;i++)
    {
      databuf[i] = 0;
    }
    num = 0;
  }
}

//����С��
void Little_bear()
{
  LCD_Draw_Circle(44,47,5,WHITE);           //����
  LCD_Draw_Circle(84,47,5,WHITE);           //����
  LCD_Draw_Circle(64,26,20,WHITE);          //��
  //LCD_Draw_Circle(64,28,18,WHITE);          //��Բ
  LCD_Draw_Dot(54,32,WHITE);
  LCD_Draw_Dot(55,32,WHITE);
  LCD_Draw_Dot(56,32,WHITE);
  LCD_Draw_Dot(53,32,WHITE);
  LCD_Draw_Dot(52,32,WHITE);
  
  LCD_Draw_Dot(54,31,WHITE);
  LCD_Draw_Dot(55,31,WHITE);
  LCD_Draw_Dot(56,31,WHITE);
  LCD_Draw_Dot(53,31,WHITE);
  LCD_Draw_Dot(52,31,WHITE);
  
  LCD_Draw_Dot(74,32,WHITE);
  LCD_Draw_Dot(73,32,WHITE);
  LCD_Draw_Dot(72,32,WHITE);
  LCD_Draw_Dot(71,32,WHITE);
  LCD_Draw_Dot(70,32,WHITE);
  
  LCD_Draw_Dot(74,31,WHITE);
  LCD_Draw_Dot(73,31,WHITE);
  LCD_Draw_Dot(72,31,WHITE);
  LCD_Draw_Dot(71,31,WHITE);
  LCD_Draw_Dot(70,31,WHITE);
  
  LCD_Draw_Line(93,28,93,40,WHITE);         //��̾��
  LCD_Draw_Line(94,28,94,40,WHITE);
  LCD_Draw_Dot(93,20,WHITE);
  LCD_Draw_Dot(93,21,WHITE);
  LCD_Draw_Dot(94,20,WHITE);
  LCD_Draw_Dot(94,21,WHITE);
  
  LCD_Draw_Line(102,28,102,40,WHITE);
  LCD_Draw_Line(103,28,103,40,WHITE);
  LCD_Draw_Dot(102,20,WHITE);
  LCD_Draw_Dot(102,21,WHITE);
  LCD_Draw_Dot(103,20,WHITE);
  LCD_Draw_Dot(103,21,WHITE);

  LCD_Draw_Line(111,28,111,40,WHITE);
  LCD_Draw_Line(112,28,112,40,WHITE);
  LCD_Draw_Dot(111,20,WHITE);
  LCD_Draw_Dot(111,21,WHITE);
  LCD_Draw_Dot(112,20,WHITE);
  LCD_Draw_Dot(112,21,WHITE);
  
  LCD_Draw_Line(60,16,68,16,WHITE);         //���
}

//������
void main()
{
  DisableInterrupts();            //��ֹ���ж�
  SMCLK_OUT();
  LED_Init(LED_ALL);              //LED�Ƴ�ʼ��
  LCD_INIT();                     //LCD��ʼ��   128 160
  //OLED_Init();                  //OLED��ʼ��
  
  Set_Vector_Handler(VECTOR_TIMER_A1_CCR0, TIMER1_A0_IRQ_Handler);    //���ö�ʱ��TA1���жϷ������Ϊ��TIMER1_A0_IRQ_Handler
  TIMER_Interval_Ms(TIMER_A1, 500);                                   //����TA1Ϊ��ʱ�жϣ��ж�ʱ��Ϊ500Ms
  TIMER_ITConfig (TIMER_A1, TIMER_CCR0_IRQn, ENABLE);                 //ʹ��TIMER��ĳһ���ж�
  
  //����Ϊ�û����󲿷ֳ�ʼ��
  
  //ADC_Init(ADC_CH1,ADC_VREF_2_5V,ADC_12bit);       //��ʼ��ADCͨ��,P6.1
  I2C_MasterInit   (I2C0, 0x00,50000);            //��ʼ��I2C0ģ��Ϊ����
  UART_Init        (UART1,115200);                              //��ʼ��UART1ģ��,������115200�������ʸ�ʱ��ð���ʱ�� ���Щ
  Set_Vector_Handler(VECTOR_UART1,UART_RX_IRQ_Handler);         //���ý����ж�����
  UART_ITConfig  (UART1,UART_RX_IRQn,TRUE);                     //�����ڽ����ж�
  function_key();                               //���ܰ�����ʼ��
  GPIO_MultiBits_Init(P6,BIT0,GPO);             //������ P6.0
  GPIO_MultiBits_Init(P7,BIT0,GPO);             //����׼��
  GPIO_SetBits(PORT7, GPIO_Pin_0);
  //GPIO_ResetBits  (PORT6, GPIO_Pin_0);        //����ߵ�ƽ ��������
  GPIO_SetBits(PORT6, GPIO_Pin_0);              //����͵�ƽ ����������
  
  Little_bear();         //����С��
  LCD_Show_Picture(34,119,93,159,logxx);    //��ʾ����log 127 159
  
  
  EnableInterrupts();
  while(1)
  {
     Key_set();           //��������
     Temp_warn();         //�¶ȱ���
     open_mv_data();      //���ڽ���open mv����
     //OLED_PrintfAt(FONT_ASCII_6X8,0,0,"LMT70: %.3f C",Lmt70_Filter());       //��ʾLMT70�¶�����
     //OLED_PrintfAt(FONT_ASCII_8X16,2,0,"Temp:%.2f C",Mlx90614_Filter());     //��ʾMLX90614�¶�����
     
     //LCD_Printf_8x16(0,103,"%s",databuf);//��ʾһ���ַ��� һ�����16���ַ�
     //LCD_Printf_8x16(0,103,"People 3 Mask OK");
     
     LCD_Printf_8x16(0,87,"Temp: ");            //�¶�
     LCD_Printf_8x16(0,71,"Threshold: %.0f C",Threshold_temp);    //��ʾ�¶���ֵ
     //LCD_Printf_8x16(0,55,"Judge Face&Mask!");
     
     //OLED_DispChineseAt(FONT_CHINESE_16X16,4,0,"��ֵ");
     //OLED_PrintfAt(FONT_ASCII_8X16,4,32,":%.0f",Threshold_temp);     //��ʾ�¶���ֵ
     
     DELAY_MS(200);
  }
}
