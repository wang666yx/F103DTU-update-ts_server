
#include "stm32f1xx_hal.h"
//#include "bsp_SysTick.h"
#include <string.h>
#include "bsp_led.h"
#include "wdz_rtc_time.h"
#include "wdz_m35.h"
#include "wdz_eeprom.h"
//#include "wdz_MCU.h"

//101ָ��
#define COMM_101_START_LINK "\x10\x41\x32\x42\x34\x16"
#define COMM_101_OK "\x10\x00\x01\x00\x01\x16"       //10 00 01 00 01 16
#define COMM_101_HEART1 "\x10\xF2\x01\x00\xF3\x16"   // 10 F2 01 00 F3 16
#define COMM_101_HEART2 "\x10\xD2\x01\x00\xD3\x16"   // 10 D2 01 00 D3 16
#define COMM_101_TIME_ACTIVE "\x68\x11\x11\x68\xD3\x01\x00\x67\x01\x06\x01\x00\x00\x00\x6C\xAE\x16\x0F\xA5\x07\x0D\x3B\x16"   //68 11 11 68 D3 01 00 67 01 06 01 00 00 00 6C AE 16 0F A5 07 0D 3B 16
#define COMM_101_TIME_CONFIRM "\x68\x11\x11\x68\x53\x01\x00\x67\x01\x07\x01\x00\x00\x00\xAD\xD3\x0B\x0F\x05\x07\x0D\x77\x16"   //68 11 11 68 53 01 00 67 01 07 01 00 00 00 AD D3 0B 0F 05 07 0D 77 16


//------------433ģ��ͨ��ָ��
#define TX101_Linkquire_data "\x10\xC9\x01\x00\xCA\x16"
#define TX101_Linkrest_data "\x10\xC0\x01\x00\xC1\x16"
#define TX101_Linkconfirm_data "\x10\x8B\x01\x00\x8C\x16"
#define TX101_OKdata "\x10\x80\x01\x00\x81\x16"

#define TX101_calldata "\x68\x0B\x0B\x68\xF3\x01\x00\x64\x01\x06\x01\x00\x00\x00\x14\x74\x16";

#define TX101_delaytime_data "\x68\x0C\x0C\x68\xD3\x01\x00\x6A\x01\x06\x01\x00\x00\x00\x3D\xC3\x46\x16";
#define TX101_delaytime_burst_data "\x68\x0C\x0C\x68\xF3\x01\x00\x6A\x01\x03\x01\x00\x00\x00\xC4\x01\x28\x16"
#define TX101_time_synchronization_data "\x68\x11\x11\x68\xD3\x01\x00\x67\x01\x06\x01\x00\x00\x00\x8B\xD4\x0B\x0F\xA5\x07\x0D\x75\x16"

#define TX101_restdata "\x68\x0B\x0B\x68\xF3\x01\x00\x69\x01\x06\x01\x00\x00\x00\x01\x66\x16";

#define TX101_heartdata "\x10\xF2\x01\x00\xF3\x16";  //������
#define TX101_testdata "\x68\x0C\x0C\x68\x73\x01\x00\x68\x01\x06\x00\x01\x00\x00\xAA\x55\xE3\x16";
#define TX101_controlMonitor "\x68\x0B\x0B\x68\xD3\x01\x00\x2D\x01\x06\x01\x00\x01\x60\x80\xEA\x16"
//68 0B 0B 68 D3 01 00 2D 01 06 01 00 01 60 80 EA 16
#define TX101_Cycleanalogtime "\x68\x0B\x0B\x68\xF3\x01\x00\x3D\x01\x06\x01\x00\x01\x70\x01\xEA\x16"
//68 0B 0B 68 F3 01 00 3D 01 06 01 00 01 70 01 AB 16

#define TXMCU_restdata "\x10\xD3\x01\x00\xD4\x16";  //ϵͳ������
#define TXMCU_433restdata "\x10\xD4\x01\x00\xD5\x16";  //433������

//#define TX101_Linkrest_data "\x10\x40\x01\x00\x41\x16"
//#define TX101_Linkconfirm_data "\x10\x0B\x01\x00\x0C\x16"



uint8_t my_usart_101frame(uint8_t usart_port);
//void my_process_resive_101usart1(void); //M35ʹ��
void my_process_resive_101usart2(void); //PWERģ��ʹ��
void my_process_resive_101usart3(void);// ����ʹ��
void my_process_resive_101usart4(void);//433ģ��ʹ��


uint8_t my_process_transmit_101usart1(uint8_t *commpt,uint8_t N,uint16_t Time);//�����������n�Σ����timeʱ�䣬�ȴ��ظ�
void my_test_usart1(uint8_t *COMM_TRANSMIT);  //����101���������������
uint8_t my_process_transmit_101usart2(uint8_t *commpt,uint8_t N,uint16_t Time);




uint8_t wdz_101char_check(uint8_t *buffer);//���У����
void wdz_101check_generate(uint8_t *buffer);//����У����
uint8_t WDZ_101recive_OKdata(void);
uint8_t WDZ_101receive_testactive(uint8_t Link_control,uint8_t type_identification,uint8_t transmit_reason,uint16_t time);

uint8_t WDZ_101transmint_commd(uint8_t type,uint8_t *txbuf,uint8_t address_type);//ͨ�÷���ָ��ȴ����ؿ϶�ȷ������ɹ�Ϊ1��ʧ��Ϊ0
uint8_t WDZ_101transmint_commd_wait_commd(uint8_t type,uint8_t *txbuf,uint8_t address_type,uint8_t *rxbuf) ;


void wdz_string_to_array(uint8_t *my_string,uint8_t *txbuf);


void WDZ_101Transmit_OKdata(void);  //��������OK֡
uint8_t WDZ_101transmit_heartdata(void); //����������������
uint8_t WDZ_101transmit_testdata(void);  //�������� ��������
uint8_t WDZ_101transmit_Resetdata(void);  //�������� ���̸�λ ����
uint8_t WDZ_101transmit_Calldata(uint8_t linktype);   //�������� �����������1Ϊ���洢���ݣ�2Ϊ�洢����
uint8_t WDZ_101Transmit_LinkInit(void);   //��������  ������· ����
uint8_t WDZ_101Transmit_Delaytimeadjust(void); //��������  Уʱ����
uint8_t WDZ_101Transmit_Control_monitor(uint8_t address,uint8_t status_value);//��������
uint8_t WDZ_101Transmit_Cycle_analog_time(uint8_t address,uint8_t status_value);//����ָʾ��ң����������ʱ��

//--���Գ���
void HexToStr(uint8_t *pbDest, uint8_t *pbSrc, int nLen);
//��������N��OK֡����ֹ433ģ��ӵ��
void wdz_sendOKfrma(int n);
uint8_t WDZ_MCUtransmit_restdata(void);
uint8_t WDZ_MCUtransmit_433restdata(void);

extern uint8_t WDZ_MCUtransmint_commd_wait_commd(uint8_t type,uint8_t *txbuf,uint8_t address_type,uint8_t *rxbuf);

void test_controlmonitor(int status);  //����ʹ�ã�Ϊ1ʹ�ã�Ϊ0��ʹ��

void my_reset2_mcu(void);
void my_test_alarm_data(void);

uint8_t WDZ_101transmint_commd2(uint8_t type,uint8_t *txbuf,uint8_t address_type,uint8_t n_count);
