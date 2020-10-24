#ifndef __KEY_H__
#define __KEY_H__

#include "common.h"

/*****************************************�û�������������************************************************/
//�û��������

#define KEY1_PRESSED_LEVEL        RESET          //KEY1���µ�ʱ���Ǹߵ�ƽ
#define KEY2_PRESSED_LEVEL        RESET          //KEY2���µ�ʱ���Ǹߵ�ƽ
#define KEY3_PRESSED_LEVEL        RESET          //KEY3���µ�ʱ���Ǹߵ�ƽ
#define KEY4_PRESSED_LEVEL        RESET          //KEY4���µ�ʱ���Ǹߵ�ƽ
#define KEY5_PRESSED_LEVEL        RESET          //KEY5���µ�ʱ���Ǹߵ�ƽ
#define KEY6_PRESSED_LEVEL        RESET          //KEY5���µ�ʱ���Ǹߵ�ƽ

//������������������
#define KEY1_GPIO_PIN             1                 
#define KEY1_GPIO_PORT            P2                      
#define KEY1_GPIO_CONFIG          (GPI|PULL_UP)

#define KEY2_GPIO_PIN             1                 
#define KEY2_GPIO_PORT            P1                      
#define KEY2_GPIO_CONFIG          (GPI|PULL_UP)

#define KEY3_GPIO_PIN             1                 
#define KEY3_GPIO_PORT            P8                      
#define KEY3_GPIO_CONFIG          (GPI|PULL_UP)

#define KEY4_GPIO_PIN             2                 
#define KEY4_GPIO_PORT            P6                     
#define KEY4_GPIO_CONFIG          (GPI|PULL_UP)

#define KEY5_GPIO_PIN             3                 
#define KEY5_GPIO_PORT            P6                      
#define KEY5_GPIO_CONFIG          (GPI|PULL_UP)

#define KEY6_GPIO_PIN             4                 
#define KEY6_GPIO_PORT            P6                      
#define KEY6_GPIO_CONFIG          (GPI|PULL_UP)

#define KEY_PRESS_WOBBLE_TIME	  10      //�������¶���ʱ�䡣Ҳ��������ʱ��,��λms
#define KEY_REALSE_WOBBLE_TIME    10      //�����ͷŶ���ʱ�䡣Ҳ��������ʱ��,��λms
#define KEY_LONG_CLICK_PERIOD     500     //��������ʱ����Сֵ����λms
#define KEY_DOUBLE_CLICK_PERIOD   200     //����˫��ʱ�����ֵ,��λms 

#define KEY_TIMING_PROCESS_TIME   20      //������������,��λms 

/************�������ݾ�����Ҫ���ģ�ֱ��ʹ�ü���*******************************/

typedef enum
{ 
  KEY1		,
  KEY2		,
  KEY3	        ,
  KEY4	        ,
  KEY5	        ,
  KEY6	        ,
  KEY_NUM       ,
}KEYn;  

typedef enum
{
  KEY_NONE_CLICK    ,
  KEY_CLICK	        ,
  KEY_DOUBLE_CLICK  ,
  KEY_LONG_CLICK	,		
}KEY_EVENT;

void KEY_Init(KEYn,void (*ClickedCallBack)(KEY_EVENT)); 	                //������ʼ������
BOOL KEY_isPressed(KEYn);						        //�жϰ����Ƿ���														//����״̬��ȡ
void KEY_20msTimingProcess(KEYn);					        //�������ڵ��ú���											//���������Դ���������

#endif
