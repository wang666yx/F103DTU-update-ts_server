#ifndef __LED_H
#define	__LED_H

//#include "stm32f1xx_hal.h"
//#include "bsp_SysTick.h"
#include "stm32f1xx_hal.h"
/** the macro definition to trigger the led on or off
  * 1 - off
  *0 - on
  */
#define ON  0
#define OFF 1




/* ֱ�Ӳ����Ĵ����ķ�������IO */
#define	digitalHi(p,i)			{p->BSRR=i;}			//����Ϊ�ߵ�ƽ		
#define digitalLo(p,i)			{p->BRR=i;}				//����͵�ƽ
#define digitalToggle(p,i)		{p->ODR ^=i;}			//�����ת״̬


/* �������IO�ĺ� */
#define EEPROM_NSS1_TOGGLE		digitalToggle(GPIOA,GPIO_PIN_4)
#define EEPROM_NSS1_OFF				digitalHi(GPIOA,GPIO_PIN_4)
#define EEPROM_NSS1_ON				digitalLo(GPIOA,GPIO_PIN_4)      //�͵�ƽѡͨ������

#define EEPROM_NSS2_TOGGLE		digitalToggle(GPIOC,GPIO_PIN_4)
#define EEPROM_NSS2_OFF				digitalHi(GPIOC,GPIO_PIN_4)
#define EEPROM_NSS2_ON				digitalLo(GPIOC,GPIO_PIN_4)     //�͵�ƽѡͨ������

#define M35_PWR_TOGGLE				digitalToggle(GPIOA,GPIO_PIN_11)
#define M35_PWR_ON						digitalHi(GPIOA,GPIO_PIN_11)  //�ߵ�ƽ��M35�ܽ������ܵ�ͨ������͵�ƽ���ػ��򿪻�
#define M35_PWR_OFF						digitalLo(GPIOA,GPIO_PIN_11)  //�͵�ƽ����������������

#define M35_EMERG_TOGGLE			digitalToggle(GPIOA,GPIO_PIN_12)
#define M35_EMERG_ON					digitalHi(GPIOA,GPIO_PIN_12)  //�ߵ�ƽ��M35�ܽ������ܵ�ͨ������͵�ƽ�������ػ�
#define M35_EMERG_OFF					digitalLo(GPIOA,GPIO_PIN_12)  //�͵�ƽ����������������

#define LED1_TOGGLE						digitalToggle(GPIOB,GPIO_PIN_12)
#define LED1_OFF							digitalHi(GPIOB,GPIO_PIN_12)
#define LED1_ON								digitalLo(GPIOB,GPIO_PIN_12)   //�͵�ƽѡͨ������������LED

#define LED2_TOGGLE						digitalToggle(GPIOB,GPIO_PIN_13)
#define LED2_OFF							digitalHi(GPIOB,GPIO_PIN_13)
#define LED2_ON								digitalLo(GPIOB,GPIO_PIN_13)

#define LED3_TOGGLE						digitalToggle(GPIOB,GPIO_PIN_14)
#define LED3_OFF							digitalHi(GPIOB,GPIO_PIN_14)
#define LED3_ON								digitalLo(GPIOB,GPIO_PIN_14)

#define LED4_TOGGLE						digitalToggle(GPIOB,GPIO_PIN_15)
#define LED4_OFF							digitalHi(GPIOB,GPIO_PIN_15)
#define LED4_ON								digitalLo(GPIOB,GPIO_PIN_15)

#define LED5_TOGGLE						digitalToggle(GPIOC,GPIO_PIN_0)
#define LED5_OFF							digitalHi(GPIOC,GPIO_PIN_0)
#define LED5_ON								digitalLo(GPIOC,GPIO_PIN_0)

#define LED6_TOGGLE						digitalToggle(GPIOC,GPIO_PIN_1)
#define LED6_OFF							digitalHi(GPIOC,GPIO_PIN_1)
#define LED6_ON								digitalLo(GPIOC,GPIO_PIN_1)

#define SPEAKER_TOGGLE				digitalToggle(GPIOC,GPIO_PIN_2) //�ߵ�ƽ��Ч�����ȷ������͵�ƽ������
#define SPEAKER_ON						//digitalHi(GPIOC,GPIO_PIN_2) //������
#define SPEAKER_OFF						digitalLo(GPIOC,GPIO_PIN_2)   //������

#define WDI1_TOGGLE						digitalToggle(GPIOC,GPIO_PIN_5)  //ι��3-5s���ߵͷ�ת
#define WDI1_OFF							digitalHi(GPIOC,GPIO_PIN_5)
#define WDI1_ON								digitalLo(GPIOC,GPIO_PIN_5)

#define M35_PWREN_CONTROL_TOGGLE			digitalToggle(GPIOC,GPIO_PIN_7)  //�ߵ�ƽ�����������͵�ƽ�ϵ�
#define M35_PWREN_CONTROL_ON					digitalHi(GPIOC,GPIO_PIN_7)  //�ϵ繤��
#define M35_PWREN_CONTROL_OFF					digitalLo(GPIOC,GPIO_PIN_7)  //�ϵ粻����


void LED_GPIO_Config(void);

void my_speaker_long_voice(uint16_t mytime);
void my_speaker_short_cycle_voice(uint8_t number,uint16_t shorttime);

#endif /* __LED_H */