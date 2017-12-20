#include "stm32f1xx_hal.h"
#include "bsp_usart1.h"
#include "bsp_SysTick.h"
#include <string.h>
#include "bsp_led.h"
#include "wdz_eeprom.h"
#include "my_def_value.h"



#define AT_REST "AT+CFUN=1,1\r\n"; //����������
#define AN_Call_Ready "Call Ready\r\n"

#define AT "AT\r\n";               //��������
#define AN_OK "OK\r\n"             //���ò���ָ��
#define AT_V1 "ATV1\r\n"
#define AT_E1 "ATE0\r\n"  //���� 1��Ϊ�л��ԣ�0Ϊû�л���ʾ
#define AT_CEMEE "AT+CMEE=0\r\n"
#define AT_IPR "AT+IPR=9600\r\n"
#define AT_W "AT&W\r\n"
#define AT_CPIN "AT+CPIN?\r\n"    //����SIM��ָ��
#define AT_CREG "AT+CREG?\r\n"   //����GPRS����
#define AT_QIDEACT "AT+QIDEACT\r\n"    //�Ͽ��������ӣ���ֹ�ظ��������ӱ���
#define AT_QIMODE "AT+QIMODE=0\r\n"
#define AT_QICSGP "AT+QICSGP=1,\"WDZNET\"\r\n"
#define AT_QIREGAPP "AT+QIREGAPP\r\n"
#define AT_QIACT "AT+QIACT\r\n"
#define AT_QILOCIP "AT+QILOCIP\r\n"
#define AT_QIHEAD "AT+QIHEAD=1\r\n"
#define AT_QIDNSIP0 "AT+QIDNSIP=0\r\n"  //����IP��������
#define AT_QIDNSIP1 "AT+QIDNSIP=1\r\n"   //����������������
//#define AT_QIOPEN(IP,PORT) "AT+QIOPEN=\"TCP\",\"(IP)\",\"(PORT)\"\r\n"   //����������������
#define AT_QIOPEN "AT+QIOPEN=\"TCP\",\r\n"   //����TCP����
#define AN_CNNECT_OK "CONNECT OK\r\n"
#define AT_QICLOSE "AT+QICLOSE\r\n"   //�Ͽ�TCP����
#define AN_CLOSE_OK "CLOSE OK\r\n"
#define AT_QISEND2 "AT+QISEND\r\n"   //����TCP����,�ǹ̶�����
#define AT_QISEND "AT+QISEND="   //����TCP���ݣ����͹̶�����,��󳤶�1460
#define AN_QISEND_WAIT ">"
#define AN_QISEND_OK "SEND OK\r\n"
//-----------���¶���ʹ��------------------------------
#define AT_CMGF "AT+CMGF=1\r\n"   //���Ÿ�ʽ���ı�ģʽ
#define AT_CSMP "AT+CSMP=17,167,0,0\r\n"  //��Ϣģʽ
#define AT_CSCS "AT+CSCS=\"GSM\"\r\n"   //�ַ�����ģʽ
#define AT_CSDH "AT+CSDH=0\r\n"   //����ͷģʽ��0ȥ������ͷ
#define AT_CPMS_SM "AT+CPMS=\"SM\",\"SM\",\"SM\"\r\n"  //��Ϣ�洢��ʽ

#define AT_CMGL "AT+CMGL=\"ALL\",1"

#define AT_CPMS "AT+CPMS?\r\n"   //����������
#define AN_CPMS_OK "+CPMS: \"SM\","

#define AT_CMGR "AT+CMGR="  //��ȡ��Ӧ��Ŷ���
#define AT_CMGD14 "AT+CMGD=1,4\r\n"   //ɾ�����ж���
#define AT_CMGS "AT+CMGS=\""    //���Ͷ��ŵ绰����

#define AT_CPBS "AT+CPBS=\"ON\"\r\n"  //���ñ����绰��ģʽ
#define AT_CNUM "AT+CNUM\r\n"   // ���ر����绰��
//---�����ź�ǿ��
#define AT_CSQ "AT+CSQ\r\n"  //  �����ź�ǿ��

#define my_GPRS_CRC_check 0


void my_init_m35(void);
uint8_t my_AT_process_usart(uint8_t *Data,uint32_t delay_time);
uint8_t my_at_com(UART_HandleTypeDef* USARTx,uint8_t *AT_COMM,uint8_t *AT_ANSWER,uint8_t n,uint32_t time);
uint8_t my_at_senddata(uint8_t *string);





uint8_t my_AT_MESS_send(uint8_t *smpt,uint8_t *telphonenumber); //�������Ͷ��ţ���Ϣ�ַ����͵绰��




void my_reset_mcu2(void);  //����MCUͨ��������




uint8_t AT_Get_Frame(uint8_t *pt);  //�������OK����ȡM35���ص���Ϣ
uint8_t AT_Get_SIM_Telephonenumber(uint8_t *pt); //��õ�ǰSIM���ĵ绰�ŷ��뵽��������pt
void AT_M35_Reset(void);   //��ʱ��M35���м�飬����������⣬���������
uint16_t get_string_number(uint8_t *string);

uint8_t AT_Get_AT_Answer_string(uint8_t *AT_COMM,uint8_t *AT_ANSWER,uint8_t *pt);
void AT_GET_CSQ(void);
//=========20170609
uint8_t my_at_senddata_long(uint8_t *string);  //GPRS���ͳ������ݣ�����1450���ֽ�