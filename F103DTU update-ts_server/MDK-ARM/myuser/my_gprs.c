#include "my_gprs.h"
#include "my_cc1101.h"
#include "my_globle_extern.h"

/*
GPRS������������  uint8_t my_at_senddata(uint8_t *string);
GPRS����������������
uint8_t my_usart_101frame(uint8_t usart_port);
void my_process_resive_101usart1(void); //M35ʹ��
*/

uint8_t  WDZ_GPRS_101FCB = 0X00;


extern  uint32_t TimingDelay;
extern uint16_t DTU_ADDRESS;

extern	uint16_t USART1_address_first;
extern	uint16_t USART1_address_second;
extern	uint8_t USART1_length;
extern	uint8_t USART1_FRAME_status;
extern	uint8_t USART1_my_frame[256];
extern	uint8_t USART1_TRANSMINT_STATUS;
extern	uint8_t USART1_RESIVER_STATUS;


extern uint8_t rsbuf1[];	  //USART1������
extern uint8_t txbuf1[];
extern uint16_t rsbuf1pt_write;
extern uint16_t rsbuf1pt_read;
extern uint16_t rsbuf1pt_TEMP_read;
extern uint16_t rsbuf1pt_COMM_read;

//=====
extern	uint16_t USART3_address_first;
extern	uint16_t USART3_address_second;
extern	uint8_t USART3_length;
extern	uint8_t USART3_FRAME_status;
extern	uint8_t USART3_my_frame[256];
extern	uint8_t USART3_TRANSMINT_STATUS;
extern	uint8_t USART3_RESIVER_STATUS;


extern uint8_t rsbuf3[];	  //USART1������
extern uint8_t txbuf3[];
extern uint16_t rsbuf3pt_write;
extern uint16_t rsbuf3pt_read;
extern uint16_t rsbuf3pt_TEMP_read;
extern uint16_t rsbuf3pt_COMM_read;


//=====



extern struct rtc_time systmtime;  //RTCʵʱʱ��ʹ��
extern u8 rtcbuffer[];

//-------------------
extern uint8_t MY_GPRS_MCU_RsBuf[8];  //�洢�������Ե�ص�ѹ��̫���ܵ�ѹ���¶ȡ�ʪ�ȹ�4��8���ֽڵ�����

extern uint8_t MY_GPRS_Call_Single_data_buf[40];
extern uint8_t MY_GPRS_Call_Analog_data_buf[110];
extern uint8_t MY_GPRS_Call_Time_data_buf[7];
extern uint8_t MY_GPRS_Call_Single_data_number;
extern uint8_t MY_GPRS_Call_Analog_data_number;
extern uint8_t MY_GPRS_Call_Status;    //�洢�����������״̬��Ϊ1��ʾ���������ݣ�Ϊ0��ʾû����������

extern uint8_t MY_EEPROM_Buf[256];  //�����洢EEPROM�У�����������
extern uint8_t MY_GPRS_Cycle_Transmintdata_status;  //�����洢��ȡ���������ݷ���״̬��1Ϊ�ѷ��ͣ�2Ϊδ����


extern uint8_t GPRS_Status;  //��ʶ�����ֻ�ģ�飬GPRS����״̬��1Ϊ�������������ӷ�������0Ϊ�����⣬��Ҫ����
extern uint8_t MESS_Status;  //��������״̬
extern uint8_t NET_Status;  //NET����״̬
extern uint8_t NET_Server_status; //Զ�˷�����server״̬

//-----------------
extern	uint8_t MY_433_Alarmdata_NOtimeBuf[256]; //�洢 ��ʱ�� ��������
extern	uint8_t MY_433_Alarmdata_TimeBuf[256];  //�洢����ʱ�꣬��������
extern	uint8_t MY_433_ALarmdata_number;  // �洢��������Ϣ�����


extern	uint8_t MY_433_ALarmdata_NOtime_status; //Ϊ1����ʾ�յ���ʱ�걨������
extern	uint8_t MY_433_ALarmdata_Time_status;   //Ϊ1����ʾ�յ���ʱ�걨������

//extern uint8_t AT_MESS_telphonenumber[];

extern  uint8_t MY_MCU_RsBuf[];
extern uint8_t MY_433_Call_Single_data_buf[];
extern uint8_t MY_433_Call_Analog_data_buf[];
extern uint8_t MY_433_Call_Time_data_buf[];
extern uint8_t MY_433_Call_Single_data_number;
extern uint8_t MY_433_Call_Analog_data_number;

//------------
extern uint32_t MY_Table1_Alarmdata_StartAddress;
extern uint32_t MY_Table1_Alarmdata_EndAddress;
extern uint32_t MY_Table1_Alarmdata_WriteAddress;
extern uint32_t MY_Table1_Alarmdata_ReadAddress;


extern  uint8_t RE_ALarmData_Status;

extern uint8_t MY_MCU_getdata_status;
extern uint8_t MY_433_Call_Status;

extern	uint8_t my_433_anlag_buf[110];  //�洢433ģ��������������0���������洢��ֻҪ�б仯�����ñ�־λ����ת����
extern	uint8_t my_433_anlag_flag;  //��־λ��Ϊ0��ʾ�Ѿ������������ݣ�Ϊ1��ʾ�������ݵ��ǻ�û�з���



extern uint16_t MY_H_speed_cyc;  //����10����
extern uint16_t MY_H_speed_heart;  //����5����

extern uint16_t MY_M_speed_cyc;  //����15����
extern uint16_t MY_M_speed_heart;  //����9����

extern uint16_t MY_L_speed_cyc;  //����30����
extern uint16_t MY_L_speed_heart; //����7����

extern float MY_Speed_H_Gate;
extern float MY_Speed_L_Gate;

extern uint16_t my_tim6_count;

extern struct indicator_class my_indicator_data[];
extern uint8_t my_usart1_tx_buf1[];
/*
���ܣ���������ȴ����ض�����������
��������typeΪ���ͣ�1Ϊ�̶����ȣ�2Ϊ�ǹ̶����ȣ�txbufΪ����ָ������,address_typeΪ1��rxbufΪ����
��2,txbuf,2,rxbuf)���ͷǹ̶����ȣ�(1,txbuf,2,rxbuf)���͹̶�����
*/
uint8_t WDZ_GPRS_101transmint_commd_wait_commd(uint8_t type, uint8_t *txbuf, uint8_t address_type, uint8_t *rxbuf) //typeΪ���ͣ�1Ϊ�̶����ȣ�2Ϊ�ǹ̶����ȣ�txbufΪ����ָ������
{
    uint8_t FCB = 0x20;
    uint16_t my_dtu_address = DTU_ADDRESS;
    uint8_t my_status = 0;
    uint8_t n = 1; //���ʹ���
    uint8_t *my_txbuf = txbuf;
//	uint8_t temp=0;

    uint8_t linkcontrol = 0;
    uint8_t type_identification = 0;
    uint8_t  transmit_reason = 0;
    uint16_t my_start_time = 0;
//********************���ͷǹ̶�����֡��Ӧ��ΪOK֡
    if(type == 2)
    {
        //�������봦��
        if(WDZ_GPRS_101FCB == 0x00)
            my_txbuf[4] = my_txbuf[4] & (~FCB);
        else if(WDZ_GPRS_101FCB == 0x20)
            my_txbuf[4] = my_txbuf[4] | (FCB);
        //Ŀ�ĵ�ַ����
        my_txbuf[5] = my_dtu_address & 0xFF;
        my_txbuf[6] = my_dtu_address >> 8;
        if(address_type == 1)
        {
            my_txbuf[10] = my_txbuf[5];
            my_txbuf[11] = my_txbuf[6];
        }
        else if(address_type == 2)
        {
            my_txbuf[10] = my_txbuf[5];
            my_txbuf[11] = my_txbuf[6];
        }

        //У��λ����
        wdz_GPRS_101check_generate(my_txbuf);
        //����ָ��
        while((n) > 0 && my_status == 0)
        {

            my_at_senddata(my_txbuf); //����GPRS������������

            //Delay_us(300); //����ʹ��

            my_start_time = my_tim6_count;
            do {

                my_status = WDZ_GPRS_101receive_testactive(0X80, 00, 00, 1500); ///WDZ_GPRS_101recive_OKdata();  //�ȴ�0Kȷ�Ϸ������� 15��
                if(my_status == 1)break;
                else my_status = 0;
            } while(my_tim6_count - my_start_time <= 3);
            n--;
            if(n != 0) USART_printf(&huart1, "\x1A"); //��һ������������ֹӵ��

        }



    }


//**************���͹̶�����֡��Ӧ��Ϊ�̶�����֡
    else	if(type == 1 && rxbuf[0] == 0x10)
    {
        //�������յĿ������룬���ͣ�����ԭ��
        linkcontrol = rxbuf[1];
        type_identification = 0;
        transmit_reason = 0;

        //�������봦��
        if(WDZ_GPRS_101FCB == 0x00)
            my_txbuf[1] = my_txbuf[1] & (~FCB);
        else if(WDZ_GPRS_101FCB == 0x20)
            my_txbuf[1] = my_txbuf[1] | (FCB);
        //Ŀ�ĵ�ַ����
        my_txbuf[2] = my_dtu_address & 0xFF;
        my_txbuf[3] = my_dtu_address >> 8;

        //У��λ����
        wdz_GPRS_101check_generate(my_txbuf);
        //����ָ��
        while((n--) > 0 && my_status == 0)
        {
            //my_UART4_printf(UART4,my_txbuf);
            my_at_senddata(my_txbuf); //����GPRS������������

            //Delay_us(300); //����

            my_status = WDZ_GPRS_101receive_testactive(linkcontrol, type_identification, transmit_reason, 1500); //�ȴ���������
            if(my_status == 1)break;
            else my_status = 0;
        }
    }
//**********���͹̶�����֡��Ӧ��Ϊ�ǹ̶�����֡
    else if(type == 1 && rxbuf[0] == 0x68)
    {
        //�������յĿ������룬���ͣ�����ԭ��
        linkcontrol = rxbuf[4];
        type_identification = rxbuf[7];
        transmit_reason = rxbuf[9];

        //�������봦��
        if(WDZ_GPRS_101FCB == 0x00)
            my_txbuf[1] = my_txbuf[1] & (~FCB);
        else if(WDZ_GPRS_101FCB == 0x20)
            my_txbuf[1] = my_txbuf[1] | (FCB);
        //Ŀ�ĵ�ַ����
        my_txbuf[2] = my_dtu_address & 0xFF;
        my_txbuf[3] = my_dtu_address >> 8;

        //У��λ����
        wdz_GPRS_101check_generate(my_txbuf);
        //����ָ��
        while((n--) > 0 && my_status == 0)
        {
            //my_UART4_printf(UART4,my_txbuf);
            my_at_senddata(my_txbuf);

            //Delay_us(600); //����

            my_status = WDZ_GPRS_101receive_testactive(linkcontrol, type_identification, transmit_reason, 1500); //�ȴ���������
            if(my_status == 1)break;
            else my_status = 0;
        }
    }


    //��¼���ͱ仯֡
    WDZ_GPRS_101FCB = WDZ_GPRS_101FCB ^ 0x20;
    if(my_status == 1)
    {
        return 1;
    }
    else return 0;

}










/*
У���ּ��
*/
uint8_t wdz_GPRS_101char_check(uint8_t *buffer)
{
    uint16_t k = 0;
    uint8_t status68 = 0;
    uint8_t temp = 0;
    uint8_t check_char = 0x00;

    if(buffer[0] == 0x10)
    {
        status68 = 1;
    }
    else if(buffer[0] == 0x68)
    {
        status68 = 2;
    }

    if(status68 == 1) //�̶�����У��λ���
    {
        check_char = buffer[1] + buffer[2] + buffer[3];
        if(check_char == buffer[4])
            temp = 1;
        else temp = 0;


    }
    else if(status68 == 2) //�ǹ̶�����У��λ���
    {
        for(k = 0; k < buffer[1]; k++)
        {
            check_char = check_char + buffer[k + 4];
        }

        if(check_char == buffer[buffer[1] + 4])
            temp = 1;
        else temp = 0;
    }
    return temp;


}







/*
����У����
*/
void wdz_GPRS_101check_generate(uint8_t *buffer)
{
    uint16_t k = 0;
    uint8_t status68 = 0;

    uint8_t check_char = 0x00;

    if(buffer[0] == 0x10)
    {
        status68 = 1;
    }
    else if(buffer[0] == 0x68)
    {
        status68 = 2;
    }


    if(status68 == 1) //�̶�����У��λ���
    {
        check_char = buffer[1] + buffer[2] + buffer[3];
        buffer[4] = check_char;
    }
    else if(status68 == 2) //�ǹ̶�����У��λ���
    {
        for(k = 0; k < buffer[1]; k++)
        {
            check_char = check_char + buffer[k + 4];
        }
        buffer[buffer[1] + 4] = check_char;
    }


}






/*
���ܣ�����Ӧ��֡�������������͵���Ӧ������
���������Link_control��������룬type_identification-֡���ͣ�transmit_reason-����ԭ��time-���ʱ��
��������Ӧ����ȷΪ1��Ӧ������û��Ӧ��Ϊ0
*/
uint8_t WDZ_GPRS_101receive_testactive(uint8_t Link_control, uint8_t type_identification, uint8_t transmit_reason, uint16_t time)
{
    uint8_t temp = 0;
    uint8_t *rsbuf = USART1_my_frame;

    uint8_t status68 = 0; //2�����ǹ̶�����֡��1�����̶�����6��֡��0��������������
    uint16_t my_dtu_address = 0;
    uint16_t my_start_time = 0;



    my_start_time = my_tim6_count;
    while(my_tim6_count - my_start_time <= 3)
    {   my_usart_GPRS_101frame(1);
        if(USART1_FRAME_status > 0)
        {
            //֡ͷ���
            if(rsbuf[0] == 0x68)
            {
                status68 = 2;
            }

            else if(rsbuf[0] == 0x10)
            {
                status68 = 1;
            }

//************0x10�̶�����֡����
            //վ��ַ���
            if(status68 == 1)
            {
                my_dtu_address = rsbuf[3];
                my_dtu_address = (my_dtu_address << 8) + rsbuf[2];
                if(my_dtu_address == DTU_ADDRESS)
                    status68 = 1;
                else status68 = 0;
            }
            //У���ֽڼ��
            if(status68 == 1)
            {
                temp = wdz_GPRS_101char_check(rsbuf); //����ʱ���Կ����Ȳ�ʹ��
                if(temp == 1) status68 = 1;
                else status68 = 0;
            }



            //֡���ͼ�顢����ԭ����
            if(status68 == 1)
            {

                if((rsbuf[1] & 0x0f) == (Link_control & 0x0f))
                {
                    temp = 1;
                    //֡�Ĵ������̽���
                    USART1_FRAME_status = 0;
                    break;
                }
                else
                {   temp = 0;
                    //���ԣ��ѽ��յ���֡ת������3
                    USART_printf(&huart3, "GPRS44");
                    USART_printf(&huart3, USART1_my_frame);
                    USART_printf(&huart3, "\n");
                    my_display_ASCIIdata(USART1_my_frame);
                    //USART_printf(&huart3,"GPRS4");
                    //֡�Ĵ������̽���
                    USART1_FRAME_status = 0;
                }

                //֡�Ĵ������̽���
                USART1_FRAME_status = 0;
            }



//************0x68�ǹ̶�����֡����
            //��վ��ַ���
            if(status68 == 2)
            {
                my_dtu_address = rsbuf[6];
                my_dtu_address = (my_dtu_address << 8) + rsbuf[5];
                if(my_dtu_address == DTU_ADDRESS)
                    status68 = 2;
                else status68 = 0;
            }
            //У���ֽڼ��

            if(status68 == 2)
            {
                temp = wdz_GPRS_101char_check(rsbuf); //����ʱ���Կ����Ȳ�ʹ��
                if(temp == 1) status68 = 2;
                else status68 = 0;
            }



            //֡���ͼ�顢����ԭ���� 68 0D 0D 68 53 01 00 68 01 07 00 01 00 00 00 55 E3 FD 16
            if(status68 == 2)
            {
                if(((rsbuf[4] & 0x0f) == (Link_control & 0x0f)) && (rsbuf[7] == type_identification) && (rsbuf[9] == transmit_reason))
                {
                    temp = 1;
                    //֡�Ĵ������̽���
                    USART1_FRAME_status = 0;
                    break;
                }
                else
                {   temp = 0;
                    //���ԣ��ѽ��յ���֡ת������3
                    USART_printf(&huart3, "GPRS66-1");
                    USART_printf(&huart3, USART1_my_frame);
                    //USART_printf(&huart3,"GPRS6");
                    //֡�Ĵ������̽���
                    USART1_FRAME_status = 0;
                }
            }
            //********************�ǹ̶�����֡��������

            //֡�Ĵ������̽���
            USART1_FRAME_status = 0;

        }


    }

    return temp;

}

/*
���� :�������յ�101ָ�Ӧ���������ȷ��������ɶ�֡��������ʶΪ0��������ȷ���򲻴�����֡������״̬���ֲ���
*/
uint8_t WDZ_GPRS_101receive_testactive2(uint8_t Link_control, uint8_t type_identification, uint8_t transmit_reason, uint16_t time)
{
    uint8_t temp = 0;
    uint8_t *rsbuf = USART1_my_frame;

    uint8_t status68 = 0; //2�����ǹ̶�����֡��1�����̶�����6��֡��0��������������
    uint16_t my_dtu_address = 0;


    if(USART1_FRAME_status > 0)
    {
        //֡ͷ���
        if(rsbuf[0] == 0x68)
        {
            status68 = 2;
        }

        else if(rsbuf[0] == 0x10)
        {
            status68 = 1;
        }

//************0x10�̶�����֡����
        //վ��ַ���
        if(status68 == 1)
        {
            my_dtu_address = rsbuf[3];
            my_dtu_address = (my_dtu_address << 8) + rsbuf[2];
            if(my_dtu_address == DTU_ADDRESS)
                status68 = 1;
            else status68 = 0;
        }
        //У���ֽڼ��
        if(status68 == 1)
        {
            //temp=wdz_GPRS_101char_check(rsbuf);  //����ʱ���Կ����Ȳ�ʹ��
            temp = 1; //����ʹ�ã��ֽ�У��Ϊ��
            if(temp == 1) status68 = 1;
            else status68 = 0;
        }



        //֡���ͼ�顢����ԭ����
        if(status68 == 1)
        {

            if((rsbuf[1] & 0x0f) == (Link_control & 0x0f))
            {
                temp = 1;
                //֡�Ĵ������̽���
                USART1_FRAME_status = 0;
                //break;
            }
            else
            {   temp = 0;
                //���ԣ��ѽ��յ���֡ת������3
                USART_printf(&huart3, "GPRS44-2");
                USART_printf(&huart3, USART1_my_frame);
                USART_printf(&huart3, "\r\n");
                my_display_ASCIIdata(USART1_my_frame);
                //USART_printf(&huart3,"GPRS4");
                //֡�Ĵ������̽���
                //USART1_FRAME_status=0;
            }

            //֡�Ĵ������̽���
            //	USART1_FRAME_status=0;
        }



//************0x68�ǹ̶�����֡����
        //��վ��ַ���
        if(status68 == 2)
        {
            my_dtu_address = rsbuf[6];
            my_dtu_address = (my_dtu_address << 8) + rsbuf[5];
            if(my_dtu_address == DTU_ADDRESS)
                status68 = 2;
            else status68 = 0;
        }
        //У���ֽڼ��

        if(status68 == 2)
        {
            temp = wdz_GPRS_101char_check(rsbuf); //����ʱ���Կ����Ȳ�ʹ��
            if(temp == 1) status68 = 2;
            else status68 = 0;
        }



        //֡���ͼ�顢����ԭ���� 68 0D 0D 68 53 01 00 68 01 07 00 01 00 00 00 55 E3 FD 16
        if(status68 == 2)
        {
            if(((rsbuf[4] & 0x0f) == (Link_control & 0x0f)) && (rsbuf[7] == type_identification) && (rsbuf[9] == transmit_reason))
            {
                temp = 1;
                //֡�Ĵ������̽���
                USART1_FRAME_status = 0;
                //break;
            }
            else
            {   temp = 0;
                //���ԣ��ѽ��յ���֡ת������3
                USART_printf(&huart3, "GPRS66-2");
                USART_printf(&huart3, USART1_my_frame);
                //USART_printf(&huart3,"GPRS6");
                //֡�Ĵ������̽���
                //USART1_FRAME_status=0;
            }
        }
        //********************�ǹ̶�����֡��������

        //֡�Ĵ������̽���
        //USART1_FRAME_status=0;

    }




    return temp;

}







/*
���ܣ����ݽ��շ������˲���Ϊ����101Э�����ݣ���֡ͷ����֡β����֡������������������Ҫ��ȡ������һ֡���ݡ�
������������ں�
��������� ȡ֡���ݳɹ�����1��ȡ֡����ʧ�ܷ���0
*/
uint8_t my_usart_GPRS_101frame(uint8_t usart_port)
{
    uint8_t *rsbuf = 0;
    uint16_t *rsbufpt_read = 0;
    uint16_t *rsbufpt_COMM_read = 0;
    uint16_t *rsbufpt_write = 0;
    uint8_t *my_frame = 0;
    uint8_t *USART_FRAME = 0;


    uint16_t *address_first = 0;
    uint16_t *address_second = 0;
    uint8_t *length = 0;

    uint8_t ch1 = 0;
    uint16_t pt = 0;
    uint8_t tmp_status = 0;
    uint8_t status = 0;
    uint16_t ii = 0;

    uint8_t my_temp_status = 0;



//******���ڵ�ѡ��****************
    if(usart_port == 1)
    {
        rsbuf = rsbuf1;
        rsbufpt_read = &rsbuf1pt_read;
        rsbufpt_COMM_read = &rsbuf1pt_COMM_read;
        rsbufpt_write = &rsbuf1pt_write;
        address_first = &USART1_address_first;
        address_second = &USART1_address_second;
        length = &USART1_length;
        my_frame = USART1_my_frame;
        USART_FRAME = &USART1_FRAME_status;

        *rsbufpt_COMM_read = *rsbufpt_read;

    }

    if(usart_port == 3)
    {
        rsbuf = rsbuf3;
        rsbufpt_read = &rsbuf3pt_read;
        rsbufpt_COMM_read = &rsbuf3pt_COMM_read;
        rsbufpt_write = &rsbuf3pt_write;
        address_first = &USART3_address_first;
        address_second = &USART3_address_second;
        length = &USART3_length;
        my_frame = USART3_my_frame;
        USART_FRAME = &USART3_FRAME_status;

        *rsbufpt_COMM_read = *rsbufpt_read;

    }

////////****����ѡ�����***************



    while(*USART_FRAME == 0 && *rsbufpt_COMM_read != *rsbufpt_write) //ǰһ��ָ�����ɣ�����δ�������ַ�������д�����������������������һ��ָ����������ַ��������
    {

        //ȡһ��δ�������ַ�
        if(*rsbufpt_COMM_read == rsbuf_max - 1)
        {
            ch1 = rsbuf[*rsbufpt_COMM_read];
            *rsbufpt_COMM_read = 0;
            pt = *rsbufpt_COMM_read;
        }
        else
        {
            ch1 = rsbuf[*rsbufpt_COMM_read];
            *rsbufpt_COMM_read = *rsbufpt_COMM_read + 1;
            pt = *rsbufpt_COMM_read;
        }

        //����0X68֡ͷ��֡β���
        if(ch1 == 0x68)
        {
            tmp_status = 1;
        }
        else if(ch1 == 0x16)
        {
            tmp_status = 2;
        }
        else
        {
            *USART_FRAME = 0;
            my_temp_status = 0;
            tmp_status = 0;
        }


        //����0X68֡ͷ����
        if(tmp_status == 1)
        {

            //
            if((pt > 0 && pt < 4) && ch1 == rsbuf[rsbuf_max + pt - 4] && ch1 == 0x68)
            {

                *address_first = rsbuf_max + pt - 4;
                if(pt - 3 == 0)*length = rsbuf[pt - 3];
                else *length = rsbuf[rsbuf_max + pt - 3];

            }
            else if(pt >= 4 && ch1 == rsbuf[pt - 4] && ch1 == 0x68)
            {

                *address_first = pt - 4;
                *length = rsbuf[pt - 3];
            }
            else if(pt == 0 && ch1 == rsbuf[rsbuf_max - 4] && ch1 == 0x68)
            {

                *address_first = rsbuf_max - 4;
                *length = rsbuf[rsbuf_max - 3];
            }

            else
            {
                *USART_FRAME = 0;
                my_temp_status = 0;
                tmp_status = 0;
            }

        }
        //������֡β��֡ͷ�ķ���
        if(tmp_status == 2)
        {

            //�̶�����֡
            if(pt > 0 && pt < 6 && rsbuf[rsbuf_max + pt - 6] == 0x10)
            {

                *address_first = rsbuf_max + pt - 6;
                *length = 6;
                *address_second = pt - 1;
                *USART_FRAME = 1;
                my_temp_status = 1;

            }
            else if(pt >= 6 && rsbuf[pt - 6] == 0x10)
            {

                *address_first = pt - 6;
                *length = 6;
                *address_second = pt - 1;
                *USART_FRAME = 1;
                my_temp_status = 1;
            }
            else if(pt == 0 && rsbuf[rsbuf_max - 6] == 0x10)
            {

                *address_first = rsbuf_max - 6;
                *length = 6;
                *address_second = pt - 1;
                *USART_FRAME = 1;
                my_temp_status = 1;
            }
            //�ǹ̶�����֡
            if(pt - 6 - *address_first == (*length) && *address_first < pt)
            {
                *address_second = pt - 1;
                *USART_FRAME = 2;
                my_temp_status = 2;
            }
            else if(*address_first > pt && pt != 0)
            {
                if((pt + rsbuf_max - *address_first - 6) == (*length))
                {   *address_second = pt - 1;
                    *USART_FRAME = 2;
                    my_temp_status = 2;
                }
            }
            else if(pt == 0)
            {
                if((rsbuf_max - *address_first - 6) == (*length))
                {   *address_second = rsbuf_max - 1;
                    *USART_FRAME = 2;
                    my_temp_status = 2;
                }
            }
            //


        }
    }
//ȡһ֡���ݴ��뵽ָ��������



    if(my_temp_status > 0)	 //���������һ֡���ݣ��Ϳ�ʼ���������򷵻أ�������
    {
        //�������������
        for(ii = 0; ii < 256; ii++)
        {
            my_frame[ii] = 0;
        }

        //ȡ�̶�����ָ��
        if(rsbuf[*address_first] == 0x10)
        {
            for(ii = 0; ii < 6; ii++)
            {
                my_frame[ii] = rsbuf[*address_first];
                *address_first = *address_first + 1;
                if(*address_first >= rsbuf_max) *address_first = 0;
            }

            *rsbufpt_read = *address_second + 1;
            if(*rsbufpt_read >= rsbuf_max)*rsbufpt_read = 0;

            status = 1;
        }
        //ȡ�ǹ̶�����ָ��
        else if(rsbuf[*address_first] == 0x68)
        {
            for(ii = 0; ii < 6 + *length; ii++)
            {
                my_frame[ii] = rsbuf[*address_first];
                *address_first = *address_first + 1;
                if(*address_first >= rsbuf_max) *address_first = 0;
            }

            *rsbufpt_read = *address_second + 1;
            if(*rsbufpt_read >= rsbuf_max)*rsbufpt_read = 0;
            status = 1;
        }
        else
        {
            status = 0;
        }
    }

    //���з��ش���
    if(status == 1)
    {
        //*USART_FRAME=0;  //ȡ��һָ֡����б��
        //printf("\r\nOK=%s",my_frame);
        //USART_printf(USARTx,"\r\nOK=%s",my_frame);
        //USART_printf(USARTx,"%s",my_frame);
        return(1);
    }
    else
    {
        //USART_FRAME=0;  //ȡ��һָ֡����б��
        //printf("\r\nERROR");
        return(0);
    }

}








/*
����OK�϶�ȷ��ָ��

*/
void WDZ_GPRS_101Transmit_OKdata(void)
{
    uint16_t my_dtu_address = DTU_ADDRESS;
    uint8_t commd[] = TX_GPRS_101_OKdata;
    //my_UART4_printf(UART4,commd);

//Ŀ�ĵ�ַ����
    commd[2] = my_dtu_address & 0xFF;
    commd[3] = my_dtu_address >> 8;

    //У��λ����
    wdz_GPRS_101check_generate(commd);

    my_at_senddata(commd);

}






/*
�����ַ������Ƶ�������
*/
void wdz_GPRS_string_to_array(uint8_t *my_string, uint8_t *txbuf)
{
    uint32_t k = 0;
    uint32_t length = 0;
    if(my_string[0] == 0x10)
    {
        length = 6;
    }
    else if(my_string[0] == 0x68)
    {
        length = my_string[1] + 6;
    }
    //=================

    for(k = 0; k < length; k++)
    {
        txbuf[k] = my_string[k];
    }
    //===========
    if(my_string[0] == 0x10)
    {
        txbuf[2] = DTU_ADDRESS;
        txbuf[3] = (DTU_ADDRESS >> 8);
    }
    else if(my_string[0] == 0x68)
    {
        txbuf[5] = DTU_ADDRESS;
        txbuf[6] = (DTU_ADDRESS >> 8);

        txbuf[10] = DTU_ADDRESS; //
        txbuf[11] = (DTU_ADDRESS >> 8);


    }
    txbuf[length - 2] = my_fun_101check_generate(txbuf, 0);
    txbuf[k] = 0;
}








//*************�������������*******
/*
���ܣ��������������ȴ�Ӧ�����

������������10 D2 01 00 D3 16 ���� 10 F2 01 00 F3 16
����ȷ�ϰ�  10 80 01 00 81 16 ���� 10 82 01 00 83 16
*/
extern uint8_t link_status_GPRS;
extern uint8_t GPRS_Heartdata_error_count;  //�ж�������ʧ�ܵĴ����������5���ˣ��ͱ�ʶGPRS������ϣ�Ȼ�������������ֵ����M35�������ж�
//uint8_t WDZ_GPRS_101transmit_heartdata(void)
//{
//    uint8_t my_status = 0;
//    uint8_t my_rxbuf[7] = "\x10\x80\x01\x00\x81\x16";
//    uint8_t my_txbuf[7] = TX_GPRS_101_heartdata;


//    my_status = WDZ_GPRS_101transmint_commd_wait_commd(1, my_txbuf, 1, my_rxbuf);

//    if(my_status == 0)
//    {
//        USART_printf(&huart3, " Server heart-\r\n");
//        GPRS_Heartdata_error_count++;
//    }
//    else if(my_status == 1)
//    {
//        USART_printf(&huart3, " Server heart*\r\n");
//        GPRS_Heartdata_error_count = 0;
//    }
//    //��������״̬�жϣ����5��ʧ�ܣ��ͱ�ʶ���������⣬��������M35
//    if(GPRS_Heartdata_error_count >= 3)
//    {
//        GPRS_Status = 0;
//        NET_Server_status = 0;
//        //GPRS_Heartdata_error_count=0;

//        link_status_GPRS = 0; //GPRS��������Ͽ��ˣ�����Ϊ101��·����
//    }

//    return my_status;

//}








//*************���Ͳ���������������*************

/*
68 0C 0C 68 73 01 00 68 01 06 00 01 00 00 AA 55 E3 16��������(����)
        ���϶�ȷ�ϣ�10 80 01 00 81 16
 68 23 23 68 73 01 00 01 98 14 00 01 00 01 00 00 00 00 00 00 00 00 23 16(ң�����ݰ�)
 ��ң�����ݰ���
  ���������ݰ���
				���϶�ȷ�ϣ�10 80 01 00 81 16
*/

extern uint32_t MY_Table2_Cycledata_StartAddress;
extern uint32_t MY_Table2_Cycledata_EndAddress;
extern uint32_t MY_Table2_Cycledata_WriteAddress;
extern uint32_t MY_Table2_Cycledata_ReadAddress;
extern uint8_t MY_GPRS_Call_Single_data_number;
extern uint8_t MY_GPRS_Call_Analog_data_number;

//uint8_t WDZ_GPRS_101transmit_Cycle_data(void)  //������ʷ���ݣ��޸ķ���2016-05-27����ʷ����ֻ����һ��ġ�10*24=240������
//{
//    uint8_t tempbuf[12] = {0};
//    uint32_t startaddressPT = MY_Table2_Cycledata_StartAddress;
//    uint32_t endaddressPT = MY_Table2_Cycledata_EndAddress;
//    uint32_t writeaddressPT = MY_Table2_Cycledata_WriteAddress;
//    uint32_t readaddressPT = MY_Table2_Cycledata_ReadAddress;
//    uint32_t tableaddress = 0;

//    uint8_t my_status = 0;
//    uint8_t tempstaus = 0;
//    uint8_t my_rxbuf[7] = "\x10\x80\x01\x00\x81\x16";
//    uint8_t my_txbuf[256] = TX_GPRS_101_testdata;
//    uint16_t my_tx_cn = 0;


////***����������µı�ָ���ַ
//    SPI_EE_BufferRead2(tempbuf, 213, 12);

//    my_buf_writeto_val(tempbuf, &startaddressPT, &endaddressPT, &writeaddressPT, &readaddressPT);
//    MY_Table2_Cycledata_StartAddress = startaddressPT;
//    MY_Table2_Cycledata_EndAddress = endaddressPT;
//    MY_Table2_Cycledata_WriteAddress = writeaddressPT;
//    MY_Table2_Cycledata_ReadAddress = readaddressPT;

////--------------
////���ж�ָ��ĩβУ�飬�������ĩβ���ͷ�����ʼ��ַ��Ȼ��д�����
//    if(readaddressPT + 144 > endaddressPT)
//    {   readaddressPT = startaddressPT;
//        MY_Table2_Cycledata_ReadAddress = readaddressPT;
//        tempbuf[0] = readaddressPT & 0x0000ff;
//        tempbuf[1] = (readaddressPT >> 8) & 0x0000ff;
//        tempbuf[2] = (readaddressPT >> 16) & 0x0000ff;
//        SPI_EE_BufferWrite2(tempbuf, 222, 3);
//    }


////----------------------

//    if(readaddressPT > writeaddressPT)
//    {
//        while(readaddressPT > writeaddressPT)
//        {
//            if(GPRS_Status == 1 && link_status_GPRS == 1)
//                my_status = 1;
//            else
//                my_status = 0;

//            wdz_GPRS_string_to_array(TX_GPRS_101_testdata, my_txbuf); //����ָ��
//            //my_status=WDZ_GPRS_101transmint_commd(2,my_txbuf,2);
//            my_status = my_status = WDZ_GPRS_101transmint_commd_wait_commd(2, my_txbuf, 1, my_rxbuf);
//            if(my_status == 1)
//            {
//                //my_status=WDZ_101receive_testactive(0x03,0x68,0x07,300);
////***********��Ҫ****��������������****
////�ȴ�EEPROM�ж��������ŵ�3���ض������У�Ȼ����3��101��Ϣ����ң�ţ�ң�⡢��������ʱ�ꡣ
////�ȴ�Server�϶�ȷ�ϣ�ȷ�Ϻ��޸�EEPROM�����ݰ���GPRS����״̬��ͬʱ�޸Ķ�ָ���ַ

//                tempstaus = my_eeprom_data_to_cycle_array(); //�����ݴ��뵽GPRS����������*********
//                if(tempstaus == 0)
//                {
//                    if((MY_GPRS_Cycle_Transmintdata_status != 1 && MY_GPRS_Cycle_Transmintdata_status != 2) | (MY_GPRS_Call_Single_data_number != 18) || (MY_GPRS_Call_Analog_data_number != 36))
//                    {   //�ƶ���ָ��
//                        readaddressPT = writeaddressPT;	 //��Ϊ��ָ���Ѿ���ƫ��ֱ�Ӱ���У����дָ��2015-11.16
//                        tempbuf[0] = readaddressPT & 0x0000ff;
//                        tempbuf[1] = (readaddressPT >> 8) & 0x0000ff;
//                        tempbuf[2] = (readaddressPT >> 16) & 0x0000ff;
//                        tableaddress = 222;
//                        SPI_EE_BufferWrite2(tempbuf, tableaddress, 3);
//                        my_status = 0;
//                        return my_status;
//                    }
//                }


//                if(tempstaus == 1)
//                {
//                    //���ɷ������ݰ����������ݣ��ȷ�ң�š��ٷ�ң�⡢��󷢻���
//                    //ң�����ݰ� 68 23 23 68 73 01 00 01 98 14 00 01 00 01 00 00 00 00 00 00 00 00 23 16

//                    my_gprs_generate_101single_data(1, my_txbuf);
//                    //my_at_senddata(my_txbuf);

//                    //����
//                    //my_status=WDZ_GPRS_101receive_testactive(0x80,00,00,1500);

//                    my_status = my_status = WDZ_GPRS_101transmint_commd_wait_commd(2, my_txbuf, 1, my_rxbuf);
//                    if(my_status == 1)
//                    {

//                        //ң�����ݰ�
//                        //68 53 53 68 53 01 00 09 98 14 00 01 00 01 40 00 00 00 00 00 00 00 00 00 00 4B 16
//                        my_gprs_generate_101analog_data(1, my_txbuf);
//                        //my_at_senddata(my_txbuf);

//                        //����
//                        //my_status=WDZ_GPRS_101receive_testactive(0x80,00,00,1500);
//                        my_status = my_status = WDZ_GPRS_101transmint_commd_wait_commd(2, my_txbuf, 1, my_rxbuf);
//                    }
//                    else
//                    {
//                        my_status = 0;
//                        USART_printf(&huart3, "GPRS Sing data error"); //����ʹ��
//                    }


//                    if(my_status == 1)
//                    {
//                        //�������ݰ�
//                        //68 53 53 68 53 01 00 09 98 14 00 01 00 00 41 00 00 00 00 00 00 00 00 00 00 4B 16
//                        my_gprs_generate_101MCU_data(1, my_txbuf);
//                        //my_at_senddata(my_txbuf);
//                        //����
//                        //my_status=WDZ_GPRS_101receive_testactive(0x80,00,00,1500);
//                        my_status = my_status = WDZ_GPRS_101transmint_commd_wait_commd(2, my_txbuf, 1, my_rxbuf);

//                    }
//                    else
//                    {
//                        my_status = 0;
//                        USART_printf(&huart3, "GPRS angale data error"); //����ʹ��
//                    }

//                    if(my_status == 0)
//                    {
//                        USART_printf(&huart3, "GPRS MCU data error"); //����ʹ��
//                    }

//                }
//                else
//                {
//                    //���������ݰ����������ݰ�
//                    my_gprs_generate_101single_data(0, my_txbuf);
//                    my_at_senddata(my_txbuf);
//                    my_gprs_generate_101analog_data(0, my_txbuf);
//                    my_at_senddata(my_txbuf);
//                    my_gprs_generate_101MCU_data(0, my_txbuf);
//                    my_at_senddata(my_txbuf);

//                }

//                //����server�����Ŀ϶�ȷ�����ݰ�
//                //my_status=WDZ_GPRS_101receive_testactive(0x80,00,00,1500); //�ɵķ�������Ҫȷ�ϵķ���

//            }


//            else if(my_status == 0)
//            {   //Delay_us(100);
//                USART_printf(&huart3, " Server Cycle data error1\r\n");
//                break;
//                //LED3_TOGGLE;
//            }



//            if(my_status == 1) //���ͳɹ��ˣ�����
//            {
//                //���з���֡״̬�ֽ��޸ģ����ƶ�EEPROM�ı���ָ��
//                if(tempstaus == 1)
//                    my_GPRS_chang_tablereadpt(2, 1); //�ɹ����޸Ĵ�֡״̬Ϊ1����ʾ���ͳ�ȥ�ˣ��ƶ���ָ��
//                else if(tempstaus == 0)
//                {
//                    my_GPRS_chang_tablereadpt(2, 0);  //ʧ�ܣ�Ҳ�޸�Ϊ1����ָ�붯������//�޸�Ϊ2��δ���ͣ����ƶ�ָ��
//                    break;
//                }

//                //Delay_us(100);
//                USART_printf(&huart3, " Server Cycle data OK1*--%d\r\n", my_tx_cn);


//                //--2016-5-27 �����������ݿ��ƣ�ֻ����һ���������ʷ���ݣ���ֹ��ʱ��ռ�á�10*24=240
//                my_tx_cn++;  //����������������
//                if(my_tx_cn > TX_History_cyc_data_record)
//                {
//                    readaddressPT = writeaddressPT;	 //��Ϊ��ָ���Ѿ���ƫ��ֱ�Ӱ���У����дָ��2016-5-27
//                    tempbuf[0] = readaddressPT & 0x0000ff;
//                    tempbuf[1] = (readaddressPT >> 8) & 0x0000ff;
//                    tempbuf[2] = (readaddressPT >> 16) & 0x0000ff;
//                    tableaddress = 222;
//                    SPI_EE_BufferWrite2(tempbuf, tableaddress, 3);

//                }


//                // LED2_TOGGLE;
//            }
//            else if(my_status == 0)
//            {   //Delay_us(100);
//                USART_printf(&huart3, " Server Cycle data errort2 \r\n");
//                //LED3_TOGGLE;
//            }
//            //***����������µı�ָ���ַ
//            SPI_EE_BufferRead2(tempbuf, 213, 12);
//            my_buf_writeto_val(tempbuf, &startaddressPT, &endaddressPT, &writeaddressPT, &readaddressPT);
//            MY_Table2_Cycledata_StartAddress = startaddressPT;
//            MY_Table2_Cycledata_EndAddress = endaddressPT;
//            MY_Table2_Cycledata_WriteAddress = writeaddressPT;
//            MY_Table2_Cycledata_ReadAddress = readaddressPT;


//        }
//        //���ж�ָ���ƶ�
//        if(readaddressPT > endaddressPT || (readaddressPT + 144) > endaddressPT) //���ж�ָ��У��
//        {   readaddressPT = startaddressPT;

//            tempbuf[0] = readaddressPT & 0x0000ff;
//            tempbuf[1] = (readaddressPT >> 8) & 0x0000ff;
//            tempbuf[2] = (readaddressPT >> 16) & 0x0000ff;
//            tableaddress = 222;
//            SPI_EE_BufferWrite2(tempbuf, tableaddress, 3);

//        }


//    }
//    else if(readaddressPT < writeaddressPT)
//    {
//        while(readaddressPT < writeaddressPT)
//        {
//            if(GPRS_Status == 1 && link_status_GPRS == 1)
//                my_status = 1;
//            else
//                my_status = 0;

//            wdz_GPRS_string_to_array(TX_GPRS_101_testdata, my_txbuf); //����ָ��
//            //my_status=WDZ_GPRS_101transmint_commd(2,my_txbuf,2);
//            my_status = WDZ_GPRS_101transmint_commd_wait_commd(2, my_txbuf, 1, my_rxbuf);
//            if(my_status == 1)
//            {
//                //my_status=WDZ_101receive_testactive(0x03,0x68,0x07,300);
////***********��Ҫ****��������������****
////�ȴ�EEPROM�ж��������ŵ�3���ض������У�Ȼ����3��101��Ϣ����ң�ţ�ң�⡢��������ʱ�ꡣ
////�ȴ�Server�϶�ȷ�ϣ�ȷ�Ϻ��޸�EEPROM�����ݰ���GPRS����״̬��ͬʱ�޸Ķ�ָ���ַ

//                tempstaus = my_eeprom_data_to_cycle_array(); //�����ݴ��뵽GPRS����������*********
//                //ǿ��У��ָ�뻷�ڣ���EEPROM�ж��������Ǵ���,2015-11.16
//                if(tempstaus == 0)
//                {
//                    if((MY_GPRS_Cycle_Transmintdata_status != 1 && MY_GPRS_Cycle_Transmintdata_status != 2) | (MY_GPRS_Call_Single_data_number != 18) || (MY_GPRS_Call_Analog_data_number != 36))
//                    {   //�ƶ���ָ��
//                        readaddressPT = writeaddressPT;	 //��Ϊ��ָ���Ѿ���ƫ��ֱ�Ӱ���У����дָ��2015-11.16
//                        tempbuf[0] = readaddressPT & 0x0000ff;
//                        tempbuf[1] = (readaddressPT >> 8) & 0x0000ff;
//                        tempbuf[2] = (readaddressPT >> 16) & 0x0000ff;
//                        tableaddress = 222;
//                        SPI_EE_BufferWrite2(tempbuf, tableaddress, 3);
//                        my_status = 0;
//                        return my_status;
//                    }
//                }

//                if(tempstaus == 1)
//                {
//                    //���ɷ������ݰ����������ݣ��ȷ�ң�š��ٷ�ң�⡢��󷢻���
//                    //ң�����ݰ� 68 23 23 68 73 01 00 01 98 14 00 01 00 01 00 00 00 00 00 00 00 00 23 16

//                    my_gprs_generate_101single_data(1, my_txbuf);
//                    //my_at_senddata(my_txbuf);

//                    //����
//                    //my_status=WDZ_GPRS_101receive_testactive(0x80,00,00,1500);

//                    my_status = my_status = WDZ_GPRS_101transmint_commd_wait_commd(2, my_txbuf, 1, my_rxbuf);
//                    if(my_status == 1)
//                    {

//                        //ң�����ݰ�
//                        //68 53 53 68 53 01 00 09 98 14 00 01 00 01 40 00 00 00 00 00 00 00 00 00 00 4B 16
//                        my_gprs_generate_101analog_data(1, my_txbuf);
//                        //my_at_senddata(my_txbuf);

//                        //����
//                        //my_status=WDZ_GPRS_101receive_testactive(0x80,00,00,1500);
//                        my_status = my_status = WDZ_GPRS_101transmint_commd_wait_commd(2, my_txbuf, 1, my_rxbuf);
//                    }
//                    else
//                    {
//                        my_status = 0;
//                        USART_printf(&huart3, "GPRS Sing data error"); //����ʹ��
//                    }


//                    if(my_status == 1)
//                    {
//                        //�������ݰ�
//                        //68 53 53 68 53 01 00 09 98 14 00 01 00 00 41 00 00 00 00 00 00 00 00 00 00 4B 16
//                        my_gprs_generate_101MCU_data(1, my_txbuf);
//                        //my_at_senddata(my_txbuf);
//                        //����
//                        //my_status=WDZ_GPRS_101receive_testactive(0x80,00,00,1500);
//                        my_status = my_status = WDZ_GPRS_101transmint_commd_wait_commd(2, my_txbuf, 1, my_rxbuf);

//                    }
//                    else
//                    {
//                        my_status = 0;
//                        USART_printf(&huart3, "GPRS angale data error"); //����ʹ��
//                    }

//                    if(my_status == 0)
//                    {
//                        USART_printf(&huart3, "GPRS MCU data error"); //����ʹ��
//                    }

//                }
//                else
//                {
//                    //���������ݰ����������ݰ�
//                    my_gprs_generate_101single_data(0, my_txbuf);
//                    my_at_senddata(my_txbuf);
//                    my_gprs_generate_101analog_data(0, my_txbuf);
//                    my_at_senddata(my_txbuf);
//                    my_gprs_generate_101MCU_data(0, my_txbuf);
//                    my_at_senddata(my_txbuf);

//                }

//                //����server�����Ŀ϶�ȷ�����ݰ�
//                //my_status=WDZ_GPRS_101receive_testactive(0x80,00,00,1500); //�ɵķ�������Ҫȷ�ϵķ���

//            }


//            else if(my_status == 0)
//            {
//                USART_printf(&huart3, " Server Cycle data error3\r\n");
//                break;
//            }



//            if(my_status == 1) //���ͳɹ��ˣ�����
//            {
//                //���з���֡״̬�ֽ��޸ģ����ƶ�EEPROM�ı���ָ��
//                if(tempstaus == 1)
//                {
//                    my_GPRS_chang_tablereadpt(2, 1); //�ɹ����޸Ĵ�֡״̬Ϊ1����ʾ���ͳ�ȥ�ˣ��ƶ���ָ��
//                }
//                else if(tempstaus == 0)
//                {
//                    my_GPRS_chang_tablereadpt(2, 0);  //ʧ�ܣ��޸�Ϊ2��δ���ͣ����ƶ�ָ��
//                    USART_printf(&huart3, " Server Cycle data errort4-1 \r\n");
//                    break;
//                }

//                //Delay_us(100);
//                USART_printf(&huart3, " Server Cycle data OK2*--%d\r\n", my_tx_cn);

//                //--2016-5-27 �����������ݿ��ƣ�ֻ����һ���������ʷ���ݣ���ֹ��ʱ��ռ�á�10*24=240
//                my_tx_cn++;  //����������������
//                if(my_tx_cn > TX_History_cyc_data_record)
//                {
//                    readaddressPT = writeaddressPT;	 //��Ϊ��ָ���Ѿ���ƫ��ֱ�Ӱ���У����дָ��2016-5-27
//                    tempbuf[0] = readaddressPT & 0x0000ff;
//                    tempbuf[1] = (readaddressPT >> 8) & 0x0000ff;
//                    tempbuf[2] = (readaddressPT >> 16) & 0x0000ff;
//                    tableaddress = 222;
//                    SPI_EE_BufferWrite2(tempbuf, tableaddress, 3);

//                }
//                //---2016-05-27 �޸Ľ���
//                // LED2_TOGGLE;
//            }
//            else if(my_status == 0)
//            {   //Delay_us(100);
//                USART_printf(&huart3, " Server Cycle data errort4 \r\n");
//                //LED3_TOGGLE;
//            }
//            //***����������µı�ָ���ַ
//            SPI_EE_BufferRead2(tempbuf, 213, 12);
//            my_buf_writeto_val(tempbuf, &startaddressPT, &endaddressPT, &writeaddressPT, &readaddressPT);
//            MY_Table2_Cycledata_StartAddress = startaddressPT;
//            MY_Table2_Cycledata_EndAddress = endaddressPT;
//            MY_Table2_Cycledata_WriteAddress = writeaddressPT;
//            MY_Table2_Cycledata_ReadAddress = readaddressPT;


//        }
//        //���ж�ָ���ƶ�
//        if(readaddressPT > writeaddressPT) //���ж�ָ��У��
//        {   readaddressPT = writeaddressPT;

//            tempbuf[0] = readaddressPT & 0x0000ff;
//            tempbuf[1] = (readaddressPT >> 8) & 0x0000ff;
//            tempbuf[2] = (readaddressPT >> 16) & 0x0000ff;
//            tableaddress = 222;
//            SPI_EE_BufferWrite2(tempbuf, tableaddress, 3);

//        }


//    }










//    return my_status;

//}





/*
���ܣ�����ң�����ݰ�  68 23 23 68 73 01 00 02 ?? 14 01 00 01 00 ?? ??  ??  ** 16
*/
void my_gprs_generate_101single_data(uint8_t temp, uint8_t *my_rsbuf)
{
    uint8_t length = 0;


    if(temp == 1) //�������ݰ�
    {
        length = MY_GPRS_Call_Single_data_number;
        my_rsbuf[0] = 0x68;
        my_rsbuf[3] = 0x68;
        my_rsbuf[1] = length + 10 + 7; //18+10+7
        my_rsbuf[2] = length + 10 + 7;

        //�������봦��
        my_rsbuf[4] = 0x73; //��������Ϊ53/73
        my_GPRS_101_geneate_FCBword(my_rsbuf);

        //my_rsbuf[4]=0x73&WDZ_GPRS_101FCB;  //��������Ϊ53/73

        my_rsbuf[5] = DTU_ADDRESS;
        my_rsbuf[6] = (DTU_ADDRESS >> 8);

        my_rsbuf[7] = 0X02; //���ͱ�ʶ����ʱ��ĵ�����Ϣ��
        my_rsbuf[8] = length + 0x80; //��Ϣ�����
        my_rsbuf[9] = 0x14; //����ԭ��

        my_rsbuf[10] = DTU_ADDRESS; //�������ַ
        my_rsbuf[11] = (DTU_ADDRESS >> 8);

        my_rsbuf[12] = 0x01; //ң����Ϣ���׵�ַ
        my_rsbuf[13] = 0x00;

        my_buf1_to_buf2(MY_GPRS_Call_Single_data_buf, 0, my_rsbuf, 14, length); //�洢ң������
				
        //my_buf1_to_buf2(MY_GPRS_Call_Time_data_buf, 0, my_rsbuf, 14 + length, 7); //�洢ʱ��

        my_rsbuf[13 + length + 7 + 1] = 0XFF;
        my_rsbuf[13 + length + 7 + 1 + 1] = 0x16;

        wdz_GPRS_101check_generate(my_rsbuf); //����У���ֽ�



    }
    //����ʹ��
    else if(temp == 0) //����0���������ݰ�
    {
        length = 2;
        my_rsbuf[0] = 0x68;
        my_rsbuf[3] = 0x68;
        my_rsbuf[1] = length + 10;
        my_rsbuf[2] = length + 10;

        //�������봦��
        my_rsbuf[4] = 0x73; //��������Ϊ53/73
        my_GPRS_101_geneate_FCBword(my_rsbuf);

        my_rsbuf[5] = DTU_ADDRESS & 0X00FF;
        my_rsbuf[6] = (DTU_ADDRESS >> 8) & 0X00FF;

        my_rsbuf[7] = 0X02; //���ͱ�ʶ����ʱ��ĵ�����Ϣ��
        my_rsbuf[8] = 0x92; //��Ϣ�����
        my_rsbuf[9] = 0x14; //����ԭ��

        my_rsbuf[10] = DTU_ADDRESS & 0X00FF; //�������ַ
        my_rsbuf[11] = (DTU_ADDRESS >> 8) & 0X00FF;

        my_rsbuf[12] = 0x01; //ң����Ϣ���׵�ַ
        my_rsbuf[13] = 0x00;

        my_rsbuf[14] = 0x00;
        my_rsbuf[15] = 0x00;

        my_rsbuf[16] = 0XFF;
        my_rsbuf[17] = 0x16;

        wdz_GPRS_101check_generate(my_rsbuf); //����У���ֽ�
    }


}








/*
����;����ң�����ݰ�

//68 53 53 68 53 01 00 09 98 14 00 01 00 01 40 00 00 00 00 00 00 00 00 00 00 4B 16
*/
void my_gprs_generate_101analog_data(uint8_t temp, uint8_t *my_rsbuf)
{
    uint8_t length = 0;


    if(temp == 1) //�������ݰ�
    {
        length = MY_GPRS_Call_Analog_data_number;

        my_rsbuf[0] = 0x68;
        my_rsbuf[3] = 0x68;
        my_rsbuf[1] = length * 3 + 10; //0X24 ��36����Ϣ�壬ÿ����Ϣ��3���ֽ�
        my_rsbuf[2] = length * 3 + 10;

        my_rsbuf[4] = 0x73; //��������Ϊ53/73
        my_GPRS_101_geneate_FCBword(my_rsbuf);

        my_rsbuf[5] = DTU_ADDRESS;
        my_rsbuf[6] = (DTU_ADDRESS >> 8);

        my_rsbuf[7] = 0X09; //���ͱ�ʶ����ʱ���ң��������Ϣ��
        my_rsbuf[8] = length + 0x80; //��Ϣ�����
        my_rsbuf[9] = 0x14; //����ԭ��

        my_rsbuf[10] = DTU_ADDRESS; //�������ַ
        my_rsbuf[11] = (DTU_ADDRESS >> 8);;

        my_rsbuf[12] = 0x01; //ң����Ϣ���׵�ַ
        my_rsbuf[13] = 0x40;

        my_buf1_to_buf2(MY_GPRS_Call_Analog_data_buf, 0, my_rsbuf, 14, length * 3);


        my_rsbuf[13 + length * 3 + 1] = 0XFF;
        my_rsbuf[13 + length * 3 + 1 + 1] = 0x16;

        wdz_GPRS_101check_generate(my_rsbuf); //����У���ֽ�



    }
    else if(temp == 0) //����0���������ݰ�
    {
        length = 2;
        my_rsbuf[0] = 0x68;
        my_rsbuf[3] = 0x68;
        my_rsbuf[1] = length + 10;
        my_rsbuf[2] = length + 10;

        my_rsbuf[4] = 0x73; //��������Ϊ53/73
        my_GPRS_101_geneate_FCBword(my_rsbuf);

        my_rsbuf[5] = DTU_ADDRESS & 0X00FF;
        my_rsbuf[6] = (DTU_ADDRESS >> 8) & 0X00FF;

        my_rsbuf[7] = 0X09; //���ͱ�ʶ����ʱ��ĵ�����Ϣ��
        my_rsbuf[8] = 0xA4; //��Ϣ�����
        my_rsbuf[9] = 0x14; //����ԭ��

        my_rsbuf[10] = DTU_ADDRESS & 0X00FF; //�������ַ
        my_rsbuf[11] = (DTU_ADDRESS >> 8) & 0X00FF;;

        my_rsbuf[12] = 0x01; //ң����Ϣ���׵�ַ
        my_rsbuf[13] = 0x40;

        my_rsbuf[14] = 0x00;
        my_rsbuf[15] = 0x00;

        my_rsbuf[16] = 0XFF;
        my_rsbuf[17] = 0x16;

        wdz_GPRS_101check_generate(my_rsbuf); //����У���ֽ�
    }



}







/*
����;���ɻ������ݰ�,tempΪ1���������ݵ����ݰ���0����0���ݵ����ݰ�
*/
void my_gprs_generate_101MCU_data(uint8_t temp, uint8_t *my_rsbuf)
{
    uint8_t length = 0;

    if(temp == 1) //�������ݰ�
    {
        length = 4;

        my_rsbuf[0] = 0x68;
        my_rsbuf[3] = 0x68;
        my_rsbuf[1] = length * 2 + 10;
        my_rsbuf[2] = length * 2 + 10;

        my_rsbuf[4] = 0x73; //��������Ϊ53/73
        my_GPRS_101_geneate_FCBword(my_rsbuf);


        my_rsbuf[5] = DTU_ADDRESS;
        my_rsbuf[6] = (DTU_ADDRESS >> 8);

        my_rsbuf[7] = 0X09; //���ͱ�ʶ����ʱ��ĵ�����Ϣ��
        my_rsbuf[8] = length + 0x80; //��Ϣ�����
        my_rsbuf[9] = 0x14; //����ԭ��

        my_rsbuf[10] = DTU_ADDRESS; //�������ַ
        my_rsbuf[11] = (DTU_ADDRESS >> 8);;

        my_rsbuf[12] = 0x00; //ң����Ϣ���׵�ַ
        my_rsbuf[13] = 0x41;

        my_buf1_to_buf2(MY_GPRS_MCU_RsBuf, 0, my_rsbuf, 14, length * 2);

        my_rsbuf[13 + length * 2 + 1] = 0XFF;
        my_rsbuf[13 + length * 2 + 1 + 1] = 0x16;

        wdz_GPRS_101check_generate(my_rsbuf); //����У���ֽ�



    }
    else if(temp == 0) //����0���������ݰ�
    {
        length = 2;
        my_rsbuf[0] = 0x68;
        my_rsbuf[3] = 0x68;
        my_rsbuf[1] = length + 10;
        my_rsbuf[2] = length + 10;

        my_rsbuf[4] = 0x73; //��������Ϊ53/73
        my_GPRS_101_geneate_FCBword(my_rsbuf);

        my_rsbuf[5] = DTU_ADDRESS & 0X00FF;
        my_rsbuf[6] = (DTU_ADDRESS >> 8) & 0X00FF;

        my_rsbuf[7] = 0X09; //���ͱ�ʶ����ʱ��ĵ�����Ϣ��
        my_rsbuf[8] = 0x84; //��Ϣ�����
        my_rsbuf[9] = 0x14; //����ԭ��

        my_rsbuf[10] = DTU_ADDRESS & 0X00FF; //�������ַ
        my_rsbuf[11] = (DTU_ADDRESS >> 8) & 0X00FF;;

        my_rsbuf[12] = 0x00; //ң����Ϣ���׵�ַ
        my_rsbuf[13] = 0x41;

        my_rsbuf[14] = 0x00;
        my_rsbuf[15] = 0x00;

        my_rsbuf[16] = 0XFF;
        my_rsbuf[17] = 0x16;

        wdz_GPRS_101check_generate(my_rsbuf); //����У���ֽ�
    }


}







/*
������������
*/
void my_GPRS_101_geneate_FCBword(uint8_t *my_rsbuf)
{   uint8_t FCB = 0X20;

    if(my_rsbuf[0] == 0x68)
    {
        //�������봦��
        if(WDZ_GPRS_101FCB == 0x00)
            my_rsbuf[4] = my_rsbuf[4] & (~FCB);
        else if(WDZ_GPRS_101FCB == 0x20)
            my_rsbuf[4] = my_rsbuf[4] | (FCB);
    }
    else if(my_rsbuf[0] == 0x10)
    {
        //�������봦��
        if(WDZ_GPRS_101FCB == 0x00)
            my_rsbuf[1] = my_rsbuf[1] & (~FCB);
        else if(WDZ_GPRS_101FCB == 0x20)
            my_rsbuf[1] = my_rsbuf[1] | (FCB);
    }

    //��¼���ͱ仯֡
    WDZ_GPRS_101FCB = WDZ_GPRS_101FCB ^ 0x20;

}











/*
���ܣ�������·����
*/
uint8_t WDZ_GPRS_101Transmit_Link_data(void)
{
    uint8_t my_status = 0;
    uint8_t my_rxbuf[256] = "\x10\x8B\x01\x00\x8C\x16";
    uint8_t my_txbuf[256] = TX_GPRS_101_Linkquire_data;



    //��1����������������·���յ���·��Ӧ
    WDZ_GPRS_101FCB = 0; //��FCB���㣬����ת
    my_status = WDZ_GPRS_101transmint_commd_wait_commd(1, my_txbuf, 1, my_rxbuf);


    //��2�������͸�λԶ����·���յ��϶�ȷ��
    if(my_status == 1)
    {
        wdz_GPRS_string_to_array(TX_GPRS_101_Linkrest_data, my_txbuf);
        wdz_GPRS_string_to_array(RX_GPRS_101_OKdata, my_rxbuf);
        WDZ_GPRS_101FCB = 0; //��FCB���㣬����ת
        my_status = WDZ_GPRS_101transmint_commd_wait_commd(1, my_txbuf, 1, my_rxbuf);
        //10 C9 01 00 CA 16
        //my_rxbuf[1]=0XC9;
        //my_rxbuf[2]=DTU_ADDRESS&0X00FF;
        //my_rxbuf[3]=(DTU_ADDRESS>>8)&0X00FF;
        //wdz_GPRS_101char_check(my_rxbuf);

        //my_status=WDZ_GPRS_101transmint_commd_wait_commd(1,my_txbuf,1,my_rxbuf);
    }
    else if(my_status == 0)
    {
        USART_printf(&huart3, " Server LINK-1\r\n");

    }
    //��3�����յ�������·
    if(my_status == 1)
    {   //10 C9 01 00 CA 16
        my_status = WDZ_GPRS_101receive_testactive(0XC9, 0, 0, 600);

    }
    else if(my_status == 0)
    {
        USART_printf(&huart3, " Server LINK-2\r\n");
    }
    //��4����������Ӧ��·״̬,�յ���λԶ����·
    if(my_status == 1)
    {
        wdz_GPRS_string_to_array(TX_GPRS_101_Linkconfirm_data, my_txbuf);
        wdz_GPRS_string_to_array(RX_GPRS_101_Linkrest_data, my_rxbuf);
        WDZ_GPRS_101FCB = 0; //��FCB���㣬����ת
        my_status = WDZ_GPRS_101transmint_commd_wait_commd(1, my_txbuf, 1, my_rxbuf);

    }
    else if(my_status == 0)
    {
        USART_printf(&huart3, " Server LINK-3\r\n");
    }
    //��5�������Ϳ϶�ȷ��,�յ���ʼ������
    if(my_status == 1)
    {
        wdz_GPRS_string_to_array(TX_GPRS_101_OKdata, my_txbuf);
        wdz_GPRS_string_to_array(RX_GPRS_101_Linkover_data, my_rxbuf);
        WDZ_GPRS_101FCB = 0; //��FCB���㣬����ת
        my_status = WDZ_GPRS_101transmint_commd_wait_commd(1, my_txbuf, 1, my_rxbuf);

    }
    else if(my_status == 0)
    {
        USART_printf(&huart3, " Server LINK-4\r\n");
    }
    //��6�������Ϳ϶�ȷ��
    if(my_status == 1)
    {
        WDZ_GPRS_101Transmit_OKdata();
        USART_printf(&huart3, " Server LINK*\r\n");

    }
    else if(my_status == 0)
    {
        USART_printf(&huart3, " Server LINK-5\r\n");
    }


    return my_status;
}





//-------------�������ճ���Уʱ������----
/*
���ܣ�Уʱ����
*/
uint8_t WDZ_GPRS_101Resiver_AdjustTime_data(void)
{
    uint8_t my_status = 0;
    uint8_t my_rtcbuffer1[7] = {0};

    uint8_t k = 0;
    uint8_t my_rxbuf[256] = "\x10\x8B\x01\x00\x8C\x16";
    uint8_t my_txbuf[256] = TX_GPRS_101_Linkquire_data;

    //��1���������ʱ�������洢����ֵ�����Ϳ϶�ȷ�ϣ���У�����غ���ֵ,ȡ���º���ֵ��������ʱ�������(����ȷ��)
    //68 0C 0C 68 53 01 00 6A 01 06 01 00 00 00 3D C3 C6 16
    my_status = WDZ_GPRS_101receive_testactive2(0X53, 0X6A, 0X06, 300);
    if(my_status == 0)  return my_status; //   ���ָ����󣬷��أ������һ��ָ��

    if(my_status == 0)  return my_status; //   ���ָ����󣬷��أ������һ��ָ��
    if(my_status == 1)
    {
        //���Ϳ϶�ȷ��ָ��
        WDZ_GPRS_101Transmit_OKdata();

        //У������ֵ��ȡֵ��������������
        my_RTCtime_to_array(my_rtcbuffer1);//��ȡRTC
        my_rtcbuffer1[0] = USART1_my_frame[14];
        my_rtcbuffer1[1] = USART1_my_frame[15];
        my_array_to_RTCtime(my_rtcbuffer1); //Уʱ
        my_RTCtime_to_array(my_rtcbuffer1);//��ȡRTC


        //������ʱ�������,�յ��϶�ȷ��
        //68 0C 0C 68 73 01 00 6A 01 07 01 00 00 00 5B C3 05 16

        wdz_GPRS_string_to_array(TX_GPRS_101_delaytime_data, my_txbuf);
        my_txbuf[14] = my_rtcbuffer1[0];
        my_txbuf[15] = my_rtcbuffer1[1];
        wdz_GPRS_string_to_array(RX_GPRS_101_OKdata, my_rxbuf);
        my_status = WDZ_GPRS_101transmint_commd_wait_commd(2, my_txbuf, 1, my_rxbuf);

    }
    else if(my_status == 0)
    {
        USART_printf(&huart3, " Server Time-1\r\n");
    }
    //��2�����յ���ʱ�������
    if(my_status == 1)
    {   //68 0C 0C 68 73 01 00 6A 01 03 01 00 00 00 C4 01 A8 16
        my_status = WDZ_GPRS_101receive_testactive(0X73, 0x6A, 0X03, 300);

    }
    else if(my_status == 0)
    {
        USART_printf(&huart3, " Server Time-2\r\n");
    }
    //��3�������Ϳ϶�ȷ��,	�յ�ʱ��ͬ������
    if(my_status == 1)
    {
        WDZ_GPRS_101Transmit_OKdata();//���Ϳ϶�ȷ��

        //�յ�ʱ��ͬ������ 68 11 11 68 53 01 00 67 01 06 01 00 00 00 8B D4 0B 0F 05 07 0E 5616

        my_status = WDZ_GPRS_101receive_testactive(0X53, 0x67, 0X06, 300);


    }
    else if(my_status == 0)
    {
        USART_printf(&huart3, " Server Time-3\r\n");
    }

    //��4�������Ϳ϶�ȷ��,	ʱ��ͬ������(����ȷ��),���տ϶�ȷ��
    if(my_status == 1)
    {
        WDZ_GPRS_101Transmit_OKdata();//���Ϳ϶�ȷ��

        //�յ�ʱ��ͬ������ 68 11 11 68 53 01 00 67 01 06 01 00   00 00     8B D4 0B 0F 05 07 0E    56 16

        for(k = 0; k < 7; k++)
        {
            my_rtcbuffer1[k] = USART1_my_frame[k + 14];
        }
        my_array_to_RTCtime(my_rtcbuffer1); //Уʱ
        my_RTC_TO_EEPROM(my_rtcbuffer1, 0); //����EEPROM��

        wdz_GPRS_string_to_array(TX_GPRS_101_time_synchronization_data, my_txbuf); //���ɷ������ݰ�
        my_RTCtime_to_array(my_rtcbuffer1); //��ȡRTC
        for(k = 0; k < 7; k++)
        {
            my_txbuf[k + 14] = my_rtcbuffer1[k]; //USART1_my_frame
        }

        wdz_GPRS_string_to_array(RX_GPRS_101_OKdata, my_rxbuf);
        my_status = WDZ_GPRS_101transmint_commd_wait_commd(2, my_txbuf, 1, my_rxbuf);

    }
    else if(my_status == 0)
    {
        USART_printf(&huart3, " Server Time-4\r\n");
    }

    if(my_status == 1)
    {
        USART_printf(&huart3, " Server Time*\r\n");
    }

    if(my_status == 0)
    {
        USART_printf(&huart3, " Server Time-5\r\n");
    }

    return my_status;
}







/*
���ܣ���������ָ�������Ӧ����������
*/
uint8_t WDZ_GPRS_101Resiver_Call_Data(void)
{
    uint8_t my_status = 0;
    uint8_t my_rxbuf[256] = "\x10\x80\x01\x00\x81\x16";
    uint8_t my_txbuf[256] = {0};



    //��1�������յ�����ָ��	 ��վ���ͣ�68 0B 0B 68 73 01 00 64 01 06 01 00 00 00 14 F4 16
    my_status = WDZ_GPRS_101receive_testactive2(0X73, 0X64, 0X06, 300);
    if(my_status == 0)  return my_status; //   ���ָ����󣬷��أ������һ��ָ��

    if(my_status == 1)
    {
        //���Ϳ϶�ȷ��ָ��
        WDZ_GPRS_101Transmit_OKdata();

        WDZ_101transmit_Calldata(2);//433ģ������
        WDZ_MCUtransmit_Calldata();		//MCUģ������


        //���ɷ������ݰ����������ݣ��ȷ�ң�š��ٷ�ң�⡢��󷢻���
        //ң�����ݰ� 68 23 23 68 73 01 00 01 98 14 00 01 00 01 00 00 00 00 00 00 00 00 23 16

        my_gprs_generate_101single_data(1, my_txbuf);
        my_at_senddata(my_txbuf);

        //ң�����ݰ�
        //68 53 53 68 53 01 00 09 98 14 00 01 00 01 40 00 00 00 00 00 00 00 00 00 00 4B 16
        my_gprs_generate_101analog_data(1, my_txbuf);
        my_at_senddata(my_txbuf);
        //�������ݰ�
        //68 53 53 68 53 01 00 09 98 14 00 01 00 00 41 00 00 00 00 00 00 00 00 00 00 4B 16
        my_gprs_generate_101MCU_data(1, my_txbuf);
        my_at_senddata(my_txbuf);

        //������ֹ;��վ���ͣ�68 12 12 68 73 01 00 09 84 14 01 00 00 41 68 05 A1 00 00 6C 00 2D FE 16,�յ��϶�ȷ��
        wdz_GPRS_string_to_array(TX_GPRS_101_Calldata_over_data, my_txbuf);
        wdz_GPRS_string_to_array(RX_GPRS_101_OKdata, my_rxbuf);
        my_status = WDZ_GPRS_101transmint_commd_wait_commd(2, my_txbuf, 1, my_rxbuf);

        USART_printf(&huart3, " Server Call*\r\n");

    }
    else if(my_status == 0)
    {
        USART_printf(&huart3, " Server Call-1\r\n");
    }



    return my_status;
}



/*
���ܣ����ս��̸�λ������MCU
*/

uint8_t WDZ_GPRS_101Resiver_ResetMCU_Data(void)
{
    uint8_t my_status = 0;
//	uint8_t k=0;
    uint8_t my_rxbuf[256] = "\x10\x8B\x01\x00\x8C\x16";
    uint8_t my_txbuf[256] = TX_GPRS_101_Linkquire_data;

    //��1�������յ�����ָ��	 ��վ���ͣ�68 0B 0B 68 73 01 00 69 01 06 01 00 00 00 01 E6 16
    my_status = WDZ_GPRS_101receive_testactive2(0X73, 0X69, 0X06, 300);
    if(my_status == 0)  return my_status; //   ���ָ����󣬷��أ������һ��ָ��

    if(my_status == 1)
    {
        //���Ϳ϶�ȷ��ָ��
        WDZ_GPRS_101Transmit_OKdata();

        //��λ��������(����ȷ��)����վ�� 68 0B 0B 68 53 01 00 69 01 07 01 00 00 00 01 C7 16,�յ��϶�ȷ��
        wdz_GPRS_string_to_array(TX_GPRS_101_ResetActive_data, my_txbuf);
        wdz_GPRS_string_to_array(RX_GPRS_101_OKdata, my_rxbuf);
        my_status = WDZ_GPRS_101transmint_commd_wait_commd(2, my_txbuf, 1, my_rxbuf);

        USART_printf(&huart3, " Server reset*\r\n");

    }
    else if(my_status == 0)
    {
        USART_printf(&huart3, " Server reset-1\r\n");
    }
    //��2��������ϵͳ������ͨ���ر�Ӳ�����ſ�
    if(my_status == 1)
    {
        my_reset_mcu(); //������
    }
    else if(my_status == 0)
    {
        USART_printf(&huart3, " Server reset-2\r\n");
    }


    return my_status;
}

/*
����;������ʱ��ı�������֡��temp=1������Ч�����ݰ���0��������λ0�Ŀ����ݰ�

68 0B 0B 68 53 01 00 01 01 03 01 00 05 00 01 60 16
*/
void my_gprs_generate_101Alarm_Notime_data(uint8_t temp, uint8_t *my_rsbuf) //���ɣ���ʱ��ı������ݰ�
{
    uint8_t length = 0;
    //uint8_t k=0;
    length = MY_433_ALarmdata_number; //��Ϣ�����
    if(temp == 1)
    {
        my_rsbuf[0] = 0x68;
        my_rsbuf[3] = 0x68;
        my_rsbuf[1] = length * 3 + 8;
        my_rsbuf[2] = length * 3 + 8;

        my_rsbuf[4] = 0x73; //��������Ϊ53/73
        my_GPRS_101_geneate_FCBword(my_rsbuf);

        my_rsbuf[5] = DTU_ADDRESS & 0X00FF;
        my_rsbuf[6] = (DTU_ADDRESS >> 8) & 0X00FF;

        my_rsbuf[7] = 0X01; //���ͱ�ʶ������ʱ��ĵ�����Ϣ��
        my_rsbuf[8] = length; //��Ϣ�����
        my_rsbuf[9] = 0x03; //����ԭ��

        my_rsbuf[10] = DTU_ADDRESS & 0X00FF; //�������ַ
        my_rsbuf[11] = (DTU_ADDRESS >> 8) & 0X00FF;

        my_buf1_to_buf2(MY_433_Alarmdata_NOtimeBuf, 0, my_rsbuf, 12, length * 3); //�������ݱ�����MY_433_Alarmdata_NOtimeBuf������

        my_rsbuf[11 + length * 3 + 1] = 0XFF;
        my_rsbuf[11 + length * 3 + 1 + 1] = 0x16;

        wdz_GPRS_101check_generate(my_rsbuf); //����У���ֽ�


    }
    else if(temp == 0)
    {
        length = 2;
        my_rsbuf[0] = 0x68;
        my_rsbuf[3] = 0x68;
        my_rsbuf[1] = length + 10;
        my_rsbuf[2] = length + 10;

        my_rsbuf[4] = 0x73; //��������Ϊ53/73
        my_GPRS_101_geneate_FCBword(my_rsbuf);

        my_rsbuf[5] = DTU_ADDRESS & 0X00FF;
        my_rsbuf[6] = (DTU_ADDRESS >> 8) & 0X00FF;

        my_rsbuf[7] = 0X01; //���ͱ�ʶ����ʱ��ĵ�����Ϣ��
        my_rsbuf[8] = 0x00; //��Ϣ�����
        my_rsbuf[9] = 0x03; //����ԭ��

        my_rsbuf[10] = DTU_ADDRESS & 0X00FF; //�������ַ
        my_rsbuf[11] = (DTU_ADDRESS >> 8) & 0X00FF;;

        my_rsbuf[12] = 0x00; //ң����Ϣ���׵�ַ
        my_rsbuf[13] = 0x00;

        my_rsbuf[14] = 0x00;
        my_rsbuf[15] = 0x00;

        my_rsbuf[16] = 0XFF;
        my_rsbuf[17] = 0x16;

        wdz_GPRS_101check_generate(my_rsbuf); //����У���ֽ�

    }

}

/*
����;������ʱ��ı�������֡��temp=1������Ч�����ݰ���0��������λ0�Ŀ����ݰ�
68 1C 1C 68 73 01 00 1E 02 03 01 00 08 00 01 20 BA 16 0F 05 07 0D 05 00 01 49 BC 16 0F 05 07 0D 02 16
*/
void my_gprs_generate_101Alarm_Time_data(uint8_t temp, uint8_t *my_rsbuf) // ���ɣ���ʱ��ı������ݰ�
{
    uint8_t length = 0;
    //uint8_t k=0;
    length = MY_433_ALarmdata_number;
    if(temp == 1)
    {
        my_rsbuf[0] = 0x68;
        my_rsbuf[3] = 0x68;
        my_rsbuf[1] = length * 10 + 8;
        my_rsbuf[2] = length * 10 + 8;

        my_rsbuf[4] = 0x73; //��������Ϊ53/73
        my_GPRS_101_geneate_FCBword(my_rsbuf);

        my_rsbuf[5] = DTU_ADDRESS & 0X00FF;
        my_rsbuf[6] = (DTU_ADDRESS >> 8) & 0X00FF;

        my_rsbuf[7] = 0X1E; //���ͱ�ʶ������ʱ��ĵ�����Ϣ��
        my_rsbuf[8] = length; //��Ϣ�����
        my_rsbuf[9] = 0x03; //����ԭ��

        my_rsbuf[10] = DTU_ADDRESS & 0X00FF; //�������ַ
        my_rsbuf[11] = (DTU_ADDRESS >> 8) & 0X00FF;

        my_buf1_to_buf2(MY_433_Alarmdata_TimeBuf, 0, my_rsbuf, 12, length * 10);

        my_rsbuf[11 + length * 10 + 1] = 0XFF;
        my_rsbuf[11 + length * 10 + 1 + 1] = 0x16;

        wdz_GPRS_101check_generate(my_rsbuf); //����У���ֽ�


    }
    else if(temp == 0)
    {
        length = 2;
        my_rsbuf[0] = 0x68;
        my_rsbuf[3] = 0x68;
        my_rsbuf[1] = length + 10;
        my_rsbuf[2] = length + 10;

        my_rsbuf[4] = 0x73; //��������Ϊ53/73
        my_GPRS_101_geneate_FCBword(my_rsbuf);

        my_rsbuf[5] = DTU_ADDRESS & 0X00FF;
        my_rsbuf[6] = (DTU_ADDRESS >> 8) & 0X00FF;

        my_rsbuf[7] = 0X1E; //���ͱ�ʶ����ʱ��ĵ�����Ϣ��
        my_rsbuf[8] = 0x00; //��Ϣ�����
        my_rsbuf[9] = 0x03; //����ԭ��

        my_rsbuf[10] = DTU_ADDRESS & 0X00FF; //�������ַ
        my_rsbuf[11] = (DTU_ADDRESS >> 8) & 0X00FF;;

        my_rsbuf[12] = 0x00; //ң����Ϣ���׵�ַ
        my_rsbuf[13] = 0x00;

        my_rsbuf[14] = 0x00;
        my_rsbuf[15] = 0x00;

        my_rsbuf[16] = 0XFF;
        my_rsbuf[17] = 0x16;

        wdz_GPRS_101check_generate(my_rsbuf); //����У���ֽ�

    }



}



/*
���ܣ���վ�������ͱ������ݣ��ȷ�����ʱ�����ݣ����ʹ�ʱ������
*/
uint8_t WDZ_GPRS_101Transmit_Alarm_Data(void)
{
    uint8_t my_status = 0;
    uint8_t my_rxbuf[256] = "\x10\x80\x01\x00\x81\x16";
    uint8_t my_txbuf[256] = {0};

    my_gprs_generate_101Alarm_Notime_data(1, my_txbuf);

    my_status = WDZ_GPRS_101transmint_commd_wait_commd(2, my_txbuf, 1, my_rxbuf);
    if(my_status == 1)
    {
        my_gprs_generate_101Alarm_Time_data(1, my_txbuf);
        my_status = WDZ_GPRS_101transmint_commd_wait_commd(2, my_txbuf, 1, my_rxbuf);
        USART_printf(&huart3, " Server Alarm*\r\n");
    }
    else if(my_status == 0)
    {
        USART_printf(&huart3, " Server Alarm-1\r\n");
    }

    return my_status;
}





/*
���ܣ��������� ң�� ����
*/
uint8_t WDZ_GPRS_101Resiver_Adjustvalue_data(void)
{
    uint8_t my_status = 0;
    uint8_t my_rxbuf[256] = "\x68\x0C\x0C\x68\x00\x03\x00\x30\x01\x07\x03\x00\x02\x50\x0C\x00\x0E\x16";
    uint8_t my_databuf[256] = {0};
    uint8_t length = 0;
    uint8_t k = 0;
    //uint8_t my_txbuf[256]=TX_GPRS_101_Linkquire_data;
    my_status = WDZ_GPRS_101receive_testactive2(0X73, 0X30, 0X06, 300);
    if(my_status == 0)  return my_status; //   ���ָ����󣬷��أ������һ��ָ��

    if(my_status == 1)
    {
        length = USART1_my_frame[1];
        for(k = 0; k < length - 8; k++)
        {
            my_databuf[k] = USART1_my_frame[k + 12];
        }
        //���ú�����������Ӧ�Ĳ������ã�my_databuf������0,1�����ֽڴ����Ϣ���ַ������Ϊ���ݣ����õ�ַ���ж϶�Ӧ�Ĺ��ܣ�������Ӧ�Ĵ���
        WDZ_GPRS_Adjust_value(my_databuf);  //���ϵͳ����*******

        my_rxbuf[4] = 0x00;
        my_rxbuf[9] = 0x07;
        my_rxbuf[5] = DTU_ADDRESS & 0x00ff;
        my_rxbuf[6] = (DTU_ADDRESS >> 8) & 0x00ff;

        my_rxbuf[10] = DTU_ADDRESS & 0x00ff;
        my_rxbuf[11] = (DTU_ADDRESS >> 8) & 0x00ff;


        for(k = 0; k < length - 8; k++)
        {
            //my_databuf[k]=USART1_my_frame[k+12];
            my_rxbuf[12 + k] = my_databuf[k];
        }

        //my_rxbuf[12]=my_databuf[0];
        //my_rxbuf[13]=my_databuf[1];
        //my_rxbuf[14]=my_databuf[2];
        //my_rxbuf[15]=my_databuf[3];


        wdz_GPRS_101check_generate(my_rxbuf);
        my_at_senddata(my_rxbuf);


    }
    else if(my_status == 0)
    {
        USART_printf(&huart3, " Server Adjustdata-1\r\n");
    }



    return my_status;
}

/*
���ܣ����յ��Ĳ����������������Ӧ��������صĲ�������Ҫ����DTU��ַ5001��������ʱ��5002���������ݷ���ʱ��5003��Server��ַ6�ֽ�5004��
      �����ֻ���8�ֽ�5005��ɾ���ֻ�����5006
*/
extern uint16_t Transmint_to_GPRS_hearttime;
extern uint16_t Transmint_to_GPRS_Cycledata;

extern uint8_t MY_IP[4]; //222.222.118.3
extern uint16_t MY_PORT;  //8080  16λ����

void WDZ_GPRS_Adjust_value(uint8_t *rsbuf)
{
    uint16_t my_address = 0;
    // uint8_t k = 0;
    uint16_t my_temp_val16 = 0;
    //uint8_t my_buf[100] = {0};
    //uint8_t my_phonebuf[16] = {0};
    //uint8_t my_firstaddress = 0;
    uint8_t my_status = 0;
    //uint8_t my_temp = 0;
    uint8_t time_buf[7] = {0};
    uint16_t my_temp_dtu = 0;
    uint8_t my_temp_rxbuf[256] = RX_GPRS_101_OKdata;
    uint8_t my_temp_txbuf[256] = TX_GPRS_101_Chaxun_data;




    my_address = rsbuf[1];
    my_address = ((my_address << 8) & 0xff00) + rsbuf[0];
    //*******DTU��ַ
    if(my_address == 0x5001)
    {
        my_temp_dtu = DTU_ADDRESS;
        my_temp_val16 = rsbuf[3];
        my_temp_val16 = ((my_temp_val16 << 8) & 0xff00) + rsbuf[2];
        DTU_ADDRESS = my_temp_val16;
        AT25_WriteByte(rsbuf[2], EEPROM_DTU_Address); //���µ�DTU��ַд�뵽EEPROM��
        AT25_WriteByte(rsbuf[3], EEPROM_DTU_Address + 1);


        //����DTU��ַ
        my_temp_txbuf[5] = my_temp_dtu & 0xff;
        my_temp_txbuf[6] = (my_temp_dtu >> 8) & 0xff;
        my_temp_txbuf[10] = my_temp_dtu & 0xff;
        my_temp_txbuf[11] = (my_temp_dtu >> 8) & 0xff;

        my_temp_txbuf[12] = 0x01;
        my_temp_txbuf[13] = 0x50;
        my_temp_txbuf[14] = DTU_ADDRESS & 0xff;
        my_temp_txbuf[15] = (DTU_ADDRESS >> 8) & 0xff;
        wdz_GPRS_101check_generate(my_temp_txbuf);

        my_status = WDZ_GPRS_101transmint_commd_wait_commd2(2, my_temp_txbuf, 2, my_temp_rxbuf, my_temp_dtu);

        Delay_ms(3);
        my_reset_mcu();//��������MCU



    }
    //******������ʱ��
    else if(my_address == 0x5002)
    {
        my_temp_val16 = rsbuf[3];
        my_temp_val16 = ((my_temp_val16 << 8) & 0xff00) + rsbuf[2];
        Transmint_to_GPRS_hearttime = my_temp_val16;
        MY_M_speed_heart = my_temp_val16; //20160621

        AT25_WriteByte(rsbuf[2], EEPROM_Hearttime_Address); //���µ�������ʱ���ַд�뵽EEPROM��
        AT25_WriteByte(rsbuf[3], EEPROM_Hearttime_Address + 1);
    }
    //******�������ݷ���ʱ��
    else if(my_address == 0x5003)
    {
        my_temp_val16 = rsbuf[3];
        my_temp_val16 = ((my_temp_val16 << 8) & 0xff00) + rsbuf[2];
        Transmint_to_GPRS_Cycledata = my_temp_val16;
        MY_M_speed_cyc = my_temp_val16; //20160621

        AT25_WriteByte(rsbuf[2], EEPROM_Cycetime_Address); //���µ���������ʱ���ַд�뵽EEPROM��
        AT25_WriteByte(rsbuf[3], EEPROM_Cycetime_Address + 1);

    }

    //20160621****��ֵ���� ����
    else if(my_address == 0x5010)
    {
        my_temp_val16 = rsbuf[3];
        my_temp_val16 = ((my_temp_val16 << 8) & 0xff00) + rsbuf[2];
        MY_Speed_H_Gate = my_temp_val16 / 65535 * 15.0;
        AT25_WriteByte(rsbuf[2], EEPROM_SPEED_Gate_H_Address); //���µ���������ʱ���ַд�뵽EEPROM��
        AT25_WriteByte(rsbuf[3], EEPROM_SPEED_Gate_H_Address + 1);




    }
    else if(my_address == 0x5011)
    {
        my_temp_val16 = rsbuf[3];
        my_temp_val16 = ((my_temp_val16 << 8) & 0xff00) + rsbuf[2];
        MY_H_speed_cyc = my_temp_val16;
        AT25_WriteByte(rsbuf[2], EEPROM_SPEED_H_Cyc_Address); //���µ���������ʱ���ַд�뵽EEPROM��
        AT25_WriteByte(rsbuf[3], EEPROM_SPEED_H_Cyc_Address + 1);

        my_temp_val16 = rsbuf[5];
        my_temp_val16 = ((my_temp_val16 << 8) & 0xff00) + rsbuf[4];
        MY_H_speed_heart = my_temp_val16;
        //AT25_WriteByte(rsbuf[4], EEPROM_SPEED_H_Heart_Address); //���µ���������ʱ���ַд�뵽EEPROM��
        //AT25_WriteByte(rsbuf[5], EEPROM_SPEED_H_Heart_Address + 1);
    }

    else if(my_address == 0x5012)
    {
        my_temp_val16 = rsbuf[3];
        my_temp_val16 = ((my_temp_val16 << 8) & 0xff00) + rsbuf[2];
        MY_L_speed_cyc = my_temp_val16;
        AT25_WriteByte(rsbuf[2], EEPROM_SPEED_H_Cyc_Address); //���µ���������ʱ���ַд�뵽EEPROM��
        AT25_WriteByte(rsbuf[3], EEPROM_SPEED_H_Cyc_Address + 1);

        my_temp_val16 = rsbuf[5];
        my_temp_val16 = ((my_temp_val16 << 8) & 0xff00) + rsbuf[4];
        MY_L_speed_heart = my_temp_val16;
        AT25_WriteByte(rsbuf[4], EEPROM_SPEED_L_Cyc_Address); //���µ���������ʱ���ַд�뵽EEPROM��
        AT25_WriteByte(rsbuf[5], EEPROM_SPEED_L_Cyc_Address + 1);
    }




    //********Server��ַ
    else if(my_address == 0x5004)
    {
        MY_IP[0] = rsbuf[2];
        MY_IP[1] = rsbuf[3];
        MY_IP[2] = rsbuf[4];
        MY_IP[3] = rsbuf[5];

        my_temp_val16 = rsbuf[7];
        my_temp_val16 = ((my_temp_val16 << 8) & 0xff00) + rsbuf[6];
        MY_PORT = my_temp_val16;

        SPI_EE_BufferWrite2(MY_IP, EEPROM_IP_Address, 4); //���µ� IP�� �˿ں� д�뵽EEPROM��
        AT25_WriteByte(rsbuf[6], EEPROM_IPPort_Address);
        AT25_WriteByte(rsbuf[7], EEPROM_IPPort_Address + 1);

        //����ȷ��ָ��
        my_temp_txbuf[1] = 0X0C + 4;
        my_temp_txbuf[2] = 0X0C + 4;

        my_temp_txbuf[5] = DTU_ADDRESS & 0xff;
        my_temp_txbuf[6] = (DTU_ADDRESS >> 8) & 0xff;
        my_temp_txbuf[10] = DTU_ADDRESS & 0xff;
        my_temp_txbuf[11] = (DTU_ADDRESS >> 8) & 0xff;

        my_temp_txbuf[12] = 0x04;
        my_temp_txbuf[13] = 0x50;

        my_temp_txbuf[14] = MY_IP[0];
        my_temp_txbuf[15] = MY_IP[1];
        my_temp_txbuf[16] = MY_IP[2];
        my_temp_txbuf[17] = MY_IP[3];

        my_temp_txbuf[18] = MY_PORT & 0x00ff;
        my_temp_txbuf[19] = (MY_PORT >> 8) & 0x00ff;

        my_temp_txbuf[20] = 0X00;
        my_temp_txbuf[21] = 0X16;

        wdz_GPRS_101check_generate(my_temp_txbuf);
        my_status = WDZ_GPRS_101transmint_commd_wait_commd(2, my_temp_txbuf, 2, my_temp_rxbuf);

        Delay_ms(3);
        my_reset_mcu();//��������MCU

    }
    //*****��ѯָ�����DTU��ַ��������������ʱ�䡢IP��PORT��time

    else if(my_address == 0x5020)
    {   my_status = 0;
        WDZ_GPRS_101Transmit_OKdata();
        //����DTU��ַ
        my_temp_txbuf[5] = DTU_ADDRESS & 0xff;
        my_temp_txbuf[6] = (DTU_ADDRESS >> 8) & 0xff;
        my_temp_txbuf[10] = DTU_ADDRESS & 0xff;
        my_temp_txbuf[11] = (DTU_ADDRESS >> 8) & 0xff;

        my_temp_txbuf[12] = 0x01;
        my_temp_txbuf[13] = 0x50;
        my_temp_txbuf[14] = DTU_ADDRESS & 0xff;
        my_temp_txbuf[15] = (DTU_ADDRESS >> 8) & 0xff;
        wdz_GPRS_101check_generate(my_temp_txbuf);

        my_status = WDZ_GPRS_101transmint_commd_wait_commd(2, my_temp_txbuf, 2, my_temp_rxbuf);
        //��������ʱ��
        if(my_status == 1)
        {
            my_temp_txbuf[12] = 0x02;
            my_temp_txbuf[13] = 0x50;
            my_temp_txbuf[14] = Transmint_to_GPRS_hearttime & 0xff;
            my_temp_txbuf[15] = (Transmint_to_GPRS_hearttime >> 8) & 0xff;
            wdz_GPRS_101check_generate(my_temp_txbuf);
            my_status = WDZ_GPRS_101transmint_commd_wait_commd(2, my_temp_txbuf, 2, my_temp_rxbuf);
        }
        else if(my_status == 0)
        {
            USART_printf(&huart3, " chaxun error-1!\r\n");
        }
        //��������ʱ��
        if(my_status == 1)
        {
            my_temp_txbuf[12] = 0x03;
            my_temp_txbuf[13] = 0x50;
            my_temp_txbuf[14] = Transmint_to_GPRS_Cycledata & 0xff;
            my_temp_txbuf[15] = (Transmint_to_GPRS_Cycledata >> 8) & 0xff;
            wdz_GPRS_101check_generate(my_temp_txbuf);
            my_status = WDZ_GPRS_101transmint_commd_wait_commd(2, my_temp_txbuf, 2, my_temp_rxbuf);
        }
        else if(my_status == 0)
        {
            USART_printf(&huart3, " chaxun error-2!\r\n");
        }
        //IP��ַ��PORT
        if(my_status == 1)
        {
            my_temp_txbuf[1] = 0X0C + 4;
            my_temp_txbuf[2] = 0X0C + 4;
            my_temp_txbuf[12] = 0x04;
            my_temp_txbuf[13] = 0x50;

            my_temp_txbuf[14] = MY_IP[0];
            my_temp_txbuf[15] = MY_IP[1];
            my_temp_txbuf[16] = MY_IP[2];
            my_temp_txbuf[17] = MY_IP[3];

            my_temp_txbuf[18] = MY_PORT & 0x00ff;
            my_temp_txbuf[19] = (MY_PORT >> 8) & 0x00ff;

            my_temp_txbuf[20] = 0X00;
            my_temp_txbuf[21] = 0X16;

            wdz_GPRS_101check_generate(my_temp_txbuf);
            my_status = WDZ_GPRS_101transmint_commd_wait_commd(2, my_temp_txbuf, 2, my_temp_rxbuf);
        }
        else if(my_status == 0)
        {
            USART_printf(&huart3, " chaxun error-3!\r\n");
        }
        //DTU��ʱ��
        if(my_status == 1)
        {
            my_temp_txbuf[1] = 0X0C + 5;
            my_temp_txbuf[2] = 0X0C + 5;
            my_temp_txbuf[12] = 0x19;
            my_temp_txbuf[13] = 0x50;

            my_RTCtime_to_array(time_buf);  //��ȡRTCʱ��
            my_temp_txbuf[14] = time_buf[0];
            my_temp_txbuf[15] = time_buf[1];
            my_temp_txbuf[16] = time_buf[2];
            my_temp_txbuf[17] = time_buf[3];
            my_temp_txbuf[18] = time_buf[4];
            my_temp_txbuf[19] = time_buf[5];
            my_temp_txbuf[20] = time_buf[6];

            my_temp_txbuf[21] = 0X00;
            my_temp_txbuf[22] = 0X16;

            wdz_GPRS_101check_generate(my_temp_txbuf);
            my_status = WDZ_GPRS_101transmint_commd_wait_commd(2, my_temp_txbuf, 2, my_temp_rxbuf);
        }
        else if(my_status == 0)
        {
            USART_printf(&huart3, " chaxun error-4!\r\n");
        }

    }




}

/*
���ܣ��绰�����飬�绰���벾���Ƿ�������绰����
*/
uint8_t WDZ_GPRS_Phonenumber_check(uint8_t *phone_source_buf1, uint8_t *phone_getnew_buf2)
{
    uint8_t my_status = 0;
    uint8_t k = 0;
    uint8_t mm = 0;
    uint8_t my_firstaddress = 0;


    for(k = 0; k < 100; k++)
    {
        if(phone_source_buf1[k] == phone_getnew_buf2[mm])
        {
            if(phone_getnew_buf2[mm] == 0x00)
            {
                my_firstaddress = k;
                break;
            }
            mm++;


        }
        else mm = 0;

    }

    if(my_firstaddress > 0) my_status = (my_firstaddress / 16) * 16;
    else my_status = 0;



    return my_status;

}

/*
���ܣ��������յ���101ָ����з�����������Ӧ�Ķ���
*/
void WDZ_GPRS_101Resiver_Analyse(void)
{
    uint8_t my_status = 0;
    my_usart_GPRS_101frame(1);  //ȡһ��������101֡
    if(USART1_FRAME_status > 0)
    {
        my_status = WDZ_GPRS_101Resiver_AdjustTimeSimple_data(); //1  ����,��Уʱ

        if(my_status == 0) my_status = WDZ_GPRS_101Resiver_ResetMCU_Data(); //2  ���ԣ����̸�λ��������MCU

        if(my_status == 0) my_status = WDZ_GPRS_101Resiver_Adjustvalue_data(); //3 ����ͨ��,����DTU���������������ڡ�IP��port

        if(my_status == 0) my_status == WDZ_GPRS_101Resiver_AdjustTime_data(); //4

        if(my_status == 0) my_status = WDZ_GPRS_101Resiver_CallHistory_data(); //5

        if(my_status == 0) my_status = WDZ_GPRS_101Resiver_Call_Data(); //6  ͨ�� ��������

        if(my_status == 0) my_status = WDZ_GPRS_101Resiver_ControlMonitor_Data(); //7ͨ��������

        if(my_status == 0) my_status = WDZ_GPRS_101Resiver_Cycle_analog_time_Data(); //8 ͨ��������ָʾ��ʱ��

        //9�������޷�ֵ������
        //10�������޷�ֵ������
        //11���ø�������ֵ
        //12���ø�������ֵ
        //13���õ�������ֵ
        //14���õ�������ֵ




    }
    USART1_FRAME_status = 0;
}







/*
���ܣ�������ʷ���ݣ�����ǰû�з��ͳ�ȥ�����ݲ�����ȥ
ԭ������ѯEEPROM��ң�ź�ң������Ա�������֡�ķ���״̬֡���з�����û�з��͵����ݾͷ��ͳ�ȥ��Ȼ���޸�״̬֡���ȷ�����ң�š�ң�⡢�������ٷ��������ݡ�
			�������ݶ���������ڷ������ٽ���֡��
			**�Ƚ��鷳��һ��֡,ʹ��Ч���д�����*****
*/
uint8_t WDZ_GPRS_101Resiver_CallHistory_data() //
{
    uint8_t my_status = 0;
    uint8_t my_rxbuf[256] = "\x10\x80\x01\x00\x81\x16";
    uint8_t my_txbuf[256] = {0};
    uint8_t my_buf[256] = {0};
//	uint8_t my_getdata_status=1;
    uint32_t n = 0;
    uint32_t first_address = EEPROM_Cycle_Table2_StartAddress;
    uint32_t second_address = EEPROM_Cycle_Table2_EndAddress + 1;
    uint8_t single_data_number = 0;
    uint8_t cycle_data_number = 0;
    uint8_t alarm_data_number = 0;
    uint8_t rd_status = 0;
    uint8_t length = 0;
    uint32_t my_count = 0;

    //��1����������ʷ���ݲ�ѯ���68 0B 0B 68 53 01 00 64 01 2C 01 00 00 00 14 FF 16
    my_status = WDZ_GPRS_101receive_testactive2(0X73, 0X64, 0X2C, 300);
    if(my_status == 0)  return my_status; //   ���ָ����󣬷��أ������һ��ָ��

    if(my_status == 1)
    {
        //���Ϳ϶�ȷ��ָ��
        WDZ_GPRS_101Transmit_OKdata();
        //��� �����������ݣ�ÿ����һ���������ݣ��ȴ�һ��ȷ��OK֡
        for(n = first_address; n < second_address;)
        {
            SPI_EE_BufferRead2(my_buf, n, 3); //��ȡǰһ֡���ݵ�ǰ3���ַ�
            rd_status = my_buf[0];
            single_data_number = my_buf[1];
            cycle_data_number = my_buf[2];
            length = 3 + 8 + single_data_number + cycle_data_number * 3 + 7;
            if(rd_status == 2) //��֡����û�з��ͳ�ȥ,���з��ʹ���
            {
                SPI_EE_BufferRead2(my_buf, n, length);
                my_gprs_generate_101single_history_data(1, my_buf, my_txbuf); //����һ֡ң������
                my_at_senddata(my_txbuf);

                my_gprs_generate_101analog_history_data(1, my_buf, my_txbuf); //����һ֡ң������
                my_at_senddata(my_txbuf);

                my_gprs_generate_101MCU_history_data(1, my_buf, my_txbuf); //����һ֡��������
                my_at_senddata(my_txbuf);

                my_status = WDZ_GPRS_101receive_testactive(0x80, 00, 00, 300); //ȷ���յ�OK֡

                if(my_status == 1)
                {
                    AT25_WriteByte(0x01, n);
                    my_count++;  //���ͳɹ�һ��,����+1
                    n = n + length; //��֡���ݴ������ˣ�ָ���ƶ�����һ֡����
                }
                else if(my_status == 0)
                {
                    break;
                }
            }
            else if(rd_status == 1) //��֡�����Ѿ���������ˣ�������һ֡���ݽ��м��
            {
                n = n + length; //ָ���ƶ�����һ֡����
            }
            else if(rd_status == 0) //������������ݼ������ˣ������˳���
            {
                break;
            }



        }



    }
    if(my_status == 1) //��ѯ�������ݱ�������δ���ͳɹ��ı�������֡
    {
        first_address = EEPROM_Alarm_Table1_StartAddress;
        second_address = EEPROM_Alarm_Table1_EndAddress;

        //��ѯTable1 ����ñ�������
        //��� �����������ݣ�ÿ����һ���������ݣ��ȴ�һ��ȷ��OK֡
        for(n = first_address; n < second_address;)
        {
            SPI_EE_BufferRead2(my_buf, n, 3); //��ȡǰһ֡���ݵ�ǰ3���ַ�
            rd_status = my_buf[0];
            alarm_data_number = my_buf[1];  //��֡�����к��е���Ϣ�����
            length = 2 + alarm_data_number * 10; //�����֡���ݵ��ܳ���
            if(rd_status == 2) //��֡����û�з��ͳ�ȥ,���з��ʹ���
            {
                SPI_EE_BufferRead2(my_buf, n, length);
                my_gprs_generate_101Alarm_Time_history_data(1, my_buf, my_txbuf); //����һ֡ң������
                my_at_senddata(my_txbuf);

                my_status = WDZ_GPRS_101receive_testactive(0x80, 00, 00, 300); //ȷ���յ�OK֡

                if(my_status == 1)
                {
                    AT25_WriteByte(0x01, n);
                    my_count++;  //���ͳɹ�һ��,����+1
                    n = n + length; //��֡���ݴ������ˣ�ָ���ƶ�����һ֡����
                }
                else if(my_status == 0)
                {
                    break;
                }
            }
            else if(rd_status == 1) //��֡�����Ѿ���������ˣ�������һ֡���ݽ��м��
            {
                n = n + length; //ָ���ƶ�����һ֡����
            }
            else if(rd_status == 0) //������������ݼ������ˣ������˳���
            {
                break;
            }



        }

    }
    //������ֹ��������֡
    if(my_status == 1)
    {
        //������ֹ;��վ���ͣ�68 0B 0B 68 53 01 00 64 01 0A 01 00 00 00 14 FF 16,�յ��϶�ȷ��
        //									68 0B 0B 68 53 01 00 64 01 0A 01 00 00 00 14 FF 16
        wdz_GPRS_string_to_array(TX_GPRS_101_Callhistory_over_data, my_txbuf);
        wdz_GPRS_string_to_array(RX_GPRS_101_OKdata, my_rxbuf);
        my_status = WDZ_GPRS_101transmint_commd_wait_commd(2, my_txbuf, 1, my_rxbuf);

        USART_printf(&huart3, " Server Callhistorydata*\r\n");

    }
    else if(my_status == 0)
    {
        USART_printf(&huart3, " Server Callhistorydata--\r\n");
    }

    return my_status;

}


/*
����;�򻯰� ʱ��ͬ�� ָ��

��19��ʱ��ͬ�������վ����68 11 11 68 53 01 00 67 01 06 01 00 00 00 8B D4 0B 0F 05 07 0E 5616
��20���϶�ȷ�ϣ���վ����10 80 01 00 81 16
��21��ʱ��ͬ������(����ȷ��)����վ����68 11 11 68 73 01 00 67 01 07 00 01 00 00 B2 9D 0E 0F 05 07 0E 6A 16
��22���϶�ȷ�ϣ���վ����10 80 01 00 81 16
*/
uint8_t WDZ_GPRS_101Resiver_AdjustTimeSimple_data(void)
{
    uint8_t my_status = 0;
    uint8_t my_rtcbuffer1[7] = {0};

    uint8_t k = 0;
    uint8_t my_rxbuf[256] = "\x10\x8B\x01\x00\x8C\x16";
    uint8_t my_txbuf[256] = TX_GPRS_101_Linkquire_data;

    //��1���������ʱ�������洢����ֵ�����Ϳ϶�ȷ�ϣ���У�����غ���ֵ,ȡ���º���ֵ��������ʱ�������(����ȷ��)
    //68 0C 0C 68 53 01 00 6A 01 06 01 00 00 00 3D C3 C6 16
    my_status = WDZ_GPRS_101receive_testactive2(0X53, 0X67, 0X06, 300);
    if(my_status == 0)  return my_status; //   ���ָ����󣬷��أ������һ��ָ��

    //��4�������Ϳ϶�ȷ��,	ʱ��ͬ������(����ȷ��),���տ϶�ȷ��
    if(my_status == 1)
    {
        WDZ_GPRS_101Transmit_OKdata();//���Ϳ϶�ȷ��

        //�յ�ʱ��ͬ������ 68 11 11 68 53 01 00 67 01 06 01 00   00 00     8B D4 0B 0F 05 07 0E    56 16

        for(k = 0; k < 7; k++)
        {
            my_rtcbuffer1[k] = USART1_my_frame[k + 14];
        }
        my_array_to_RTCtime(my_rtcbuffer1); //Уʱ
        my_RTC_TO_EEPROM(my_rtcbuffer1, 0); //����EEPROM��

        wdz_GPRS_string_to_array(TX_GPRS_101_time_synchronization_data, my_txbuf); //���ɷ������ݰ�
        my_RTCtime_to_array(my_rtcbuffer1); //��ȡRTC
        for(k = 0; k < 7; k++)
        {
            my_txbuf[k + 14] = my_rtcbuffer1[k];
        }

        wdz_GPRS_string_to_array(RX_GPRS_101_OKdata, my_rxbuf);
        my_status = WDZ_GPRS_101transmint_commd_wait_commd(2, my_txbuf, 1, my_rxbuf);

    }
    else if(my_status == 0)
    {
        USART_printf(&huart3, " Server Simple Time-4\r\n");
    }

    if(my_status == 1)
    {
        USART_printf(&huart3, " Server Simple Time*\r\n");
    }

    if(my_status == 0)
    {
        USART_printf(&huart3, " Server  Simple Time-5\r\n");
    }

    return my_status;

}








/*
���ܣ�����ң�����ݰ�  68 23 23 68 73 01 00 02 ?? 14 01 00 01 00 ?? ??  ??  ** 16
*/
void my_gprs_generate_101single_history_data(uint8_t temp, uint8_t *my_buf, uint8_t *my_rsbuf)
{
    uint8_t length = 0;
    uint8_t alarm_data_number = 0;
    uint8_t cycle_data_number = 0;


    if(temp == 1) //�������ݰ�
    {
        length = my_buf[1];
        alarm_data_number = my_buf[1];
        cycle_data_number = my_buf[2];


        my_rsbuf[0] = 0x68;
        my_rsbuf[3] = 0x68;
        my_rsbuf[1] = length + 10 + 7;
        my_rsbuf[2] = length + 10 + 7;

        //�������봦��
        my_rsbuf[4] = 0x73; //��������Ϊ53/73
        my_GPRS_101_geneate_FCBword(my_rsbuf);

        //my_rsbuf[4]=0x73&WDZ_GPRS_101FCB;  //��������Ϊ53/73

        my_rsbuf[5] = DTU_ADDRESS & 0X00FF;
        my_rsbuf[6] = (DTU_ADDRESS >> 8) & 0X00FF;

        my_rsbuf[7] = 0X02; //���ͱ�ʶ����ʱ��ĵ�����Ϣ��
        my_rsbuf[8] = length + 0x80; //��Ϣ�����
        my_rsbuf[9] = 0x14; //����ԭ��

        my_rsbuf[10] = DTU_ADDRESS & 0X00FF; //�������ַ
        my_rsbuf[11] = (DTU_ADDRESS >> 8) & 0X00FF;

        my_rsbuf[12] = 0x01; //ң����Ϣ���׵�ַ
        my_rsbuf[13] = 0x00;

        my_buf1_to_buf2(my_buf, 3 + 8, my_rsbuf, 14, length); //�洢ң������

        my_buf1_to_buf2(my_buf, 3 + 8 + alarm_data_number + cycle_data_number * 3, my_rsbuf, 14 + length, 7); //�洢ʱ��

        my_rsbuf[13 + length + 7 + 1] = 0XFF;
        my_rsbuf[13 + length + 7 + 1 + 1] = 0x16;

        wdz_GPRS_101check_generate(my_rsbuf); //����У���ֽ�



    }
    else if(temp == 0) //����0���������ݰ�
    {
        length = 2;
        my_rsbuf[0] = 0x68;
        my_rsbuf[3] = 0x68;
        my_rsbuf[1] = length + 10;
        my_rsbuf[2] = length + 10;

        //�������봦��
        my_rsbuf[4] = 0x73; //��������Ϊ53/73
        my_GPRS_101_geneate_FCBword(my_rsbuf);

        my_rsbuf[5] = DTU_ADDRESS & 0X00FF;
        my_rsbuf[6] = (DTU_ADDRESS >> 8) & 0X00FF;

        my_rsbuf[7] = 0X02; //���ͱ�ʶ����ʱ��ĵ�����Ϣ��
        my_rsbuf[8] = 0x00; //��Ϣ�����
        my_rsbuf[9] = 0x14; //����ԭ��

        my_rsbuf[10] = DTU_ADDRESS & 0X00FF; //�������ַ
        my_rsbuf[11] = (DTU_ADDRESS >> 8) & 0X00FF;

        my_rsbuf[12] = 0x01; //ң����Ϣ���׵�ַ
        my_rsbuf[13] = 0x00;

        my_rsbuf[14] = 0x00;
        my_rsbuf[15] = 0x00;

        my_rsbuf[16] = 0XFF;
        my_rsbuf[17] = 0x16;

        wdz_GPRS_101check_generate(my_rsbuf); //����У���ֽ�
    }


}

/*
����;����ң�����ݰ�

//68 53 53 68 53 01 00 09 98 14 00 01 00 01 40 00 00 00 00 00 00 00 00 00 00 4B 16
*/
void my_gprs_generate_101analog_history_data(uint8_t temp, uint8_t *my_buf, uint8_t *my_rsbuf)
{
    uint8_t length = 0;
    uint8_t single_data_number = 0;
//	uint8_t cycle_data_number=0;


    if(temp == 1) //�������ݰ�
    {
        length = my_buf[2];
        single_data_number = my_buf[1];
//		cycle_data_number=my_buf[2];

        my_rsbuf[0] = 0x68;
        my_rsbuf[3] = 0x68;
        my_rsbuf[1] = length * 3 + 10;
        my_rsbuf[2] = length * 3 + 10;

        my_rsbuf[4] = 0x73; //��������Ϊ53/73
        my_GPRS_101_geneate_FCBword(my_rsbuf);

        my_rsbuf[5] = DTU_ADDRESS & 0X00FF;
        my_rsbuf[6] = (DTU_ADDRESS >> 8) & 0X00FF;

        my_rsbuf[7] = 0X09; //���ͱ�ʶ����ʱ��ĵ�����Ϣ��
        my_rsbuf[8] = length + 0x80; //��Ϣ�����
        my_rsbuf[9] = 0x14; //����ԭ��

        my_rsbuf[10] = DTU_ADDRESS & 0X00FF; //�������ַ
        my_rsbuf[11] = (DTU_ADDRESS >> 8) & 0X00FF;;

        my_rsbuf[12] = 0x01; //ң����Ϣ���׵�ַ
        my_rsbuf[13] = 0x40;

        my_buf1_to_buf2(my_buf, 3 + 8 + single_data_number, my_rsbuf, 14, length * 3);


        my_rsbuf[13 + length * 3 + 1] = 0XFF;
        my_rsbuf[13 + length * 3 + 1 + 1] = 0x16;

        wdz_GPRS_101check_generate(my_rsbuf); //����У���ֽ�



    }
    else if(temp == 0) //����0���������ݰ�
    {
        length = 2;
        my_rsbuf[0] = 0x68;
        my_rsbuf[3] = 0x68;
        my_rsbuf[1] = length + 10;
        my_rsbuf[2] = length + 10;

        my_rsbuf[4] = 0x73; //��������Ϊ53/73
        my_GPRS_101_geneate_FCBword(my_rsbuf);

        my_rsbuf[5] = DTU_ADDRESS & 0X00FF;
        my_rsbuf[6] = (DTU_ADDRESS >> 8) & 0X00FF;

        my_rsbuf[7] = 0X09; //���ͱ�ʶ����ʱ��ĵ�����Ϣ��
        my_rsbuf[8] = 0x00; //��Ϣ�����
        my_rsbuf[9] = 0x14; //����ԭ��

        my_rsbuf[10] = DTU_ADDRESS & 0X00FF; //�������ַ
        my_rsbuf[11] = (DTU_ADDRESS >> 8) & 0X00FF;;

        my_rsbuf[12] = 0x01; //ң����Ϣ���׵�ַ
        my_rsbuf[13] = 0x40;

        my_rsbuf[14] = 0x00;
        my_rsbuf[15] = 0x00;

        my_rsbuf[16] = 0XFF;
        my_rsbuf[17] = 0x16;

        wdz_GPRS_101check_generate(my_rsbuf); //����У���ֽ�
    }



}







/*
����;���ɻ������ݰ�,tempΪ1���������ݵ����ݰ���0����0���ݵ����ݰ�
*/
void my_gprs_generate_101MCU_history_data(uint8_t temp, uint8_t *my_buf, uint8_t *my_rsbuf)
{
    uint8_t length = 0;

    if(temp == 1) //�������ݰ�
    {
        length = 4;

        my_rsbuf[0] = 0x68;
        my_rsbuf[3] = 0x68;
        my_rsbuf[1] = length * 2 + 10;
        my_rsbuf[2] = length * 2 + 10;

        my_rsbuf[4] = 0x73; //��������Ϊ53/73
        my_GPRS_101_geneate_FCBword(my_rsbuf);


        my_rsbuf[5] = DTU_ADDRESS & 0X00FF;
        my_rsbuf[6] = (DTU_ADDRESS >> 8) & 0X00FF;

        my_rsbuf[7] = 0X09; //���ͱ�ʶ����ʱ��ĵ�����Ϣ��
        my_rsbuf[8] = length + 0x80; //��Ϣ�����
        my_rsbuf[9] = 0x14; //����ԭ��

        my_rsbuf[10] = DTU_ADDRESS & 0X00FF; //�������ַ
        my_rsbuf[11] = (DTU_ADDRESS >> 8) & 0X00FF;;

        my_rsbuf[12] = 0x00; //ң����Ϣ���׵�ַ
        my_rsbuf[13] = 0x41;

        my_buf1_to_buf2(my_buf, 3, my_rsbuf, 14, length * 2);



        my_rsbuf[13 + length * 2 + 1] = 0XFF;
        my_rsbuf[13 + length * 2 + 1 + 1] = 0x16;

        wdz_GPRS_101check_generate(my_rsbuf); //����У���ֽ�



    }
    else if(temp == 0) //����0���������ݰ�
    {
        length = 2;
        my_rsbuf[0] = 0x68;
        my_rsbuf[3] = 0x68;
        my_rsbuf[1] = length + 10;
        my_rsbuf[2] = length + 10;

        my_rsbuf[4] = 0x73; //��������Ϊ53/73
        my_GPRS_101_geneate_FCBword(my_rsbuf);

        my_rsbuf[5] = DTU_ADDRESS & 0X00FF;
        my_rsbuf[6] = (DTU_ADDRESS >> 8) & 0X00FF;

        my_rsbuf[7] = 0X09; //���ͱ�ʶ����ʱ��ĵ�����Ϣ��
        my_rsbuf[8] = 0x00; //��Ϣ�����
        my_rsbuf[9] = 0x14; //����ԭ��

        my_rsbuf[10] = DTU_ADDRESS & 0X00FF; //�������ַ
        my_rsbuf[11] = (DTU_ADDRESS >> 8) & 0X00FF;;

        my_rsbuf[12] = 0x00; //ң����Ϣ���׵�ַ
        my_rsbuf[13] = 0x41;

        my_rsbuf[14] = 0x00;
        my_rsbuf[15] = 0x00;

        my_rsbuf[16] = 0XFF;
        my_rsbuf[17] = 0x16;

        wdz_GPRS_101check_generate(my_rsbuf); //����У���ֽ�
    }


}

/*
����;������ʱ��ı�������֡��temp=1������Ч�����ݰ���0��������λ0�Ŀ����ݰ�
68 1C 1C 68 73 01 00 1E 02 03 01 00 08 00 01 20 BA 16 0F 05 07 0D 05 00 01 49 BC 16 0F 05 07 0D 02 16
*/
void my_gprs_generate_101Alarm_Time_history_data(uint8_t temp, uint8_t *my_buf, uint8_t *my_rsbuf) // ���ɣ���ʱ��ı������ݰ�
{
    uint8_t length = 0;
    //uint8_t k=0;
    length = my_buf[1];
    if(temp == 1)
    {
        my_rsbuf[0] = 0x68;
        my_rsbuf[3] = 0x68;
        my_rsbuf[1] = length * 10 + 8;
        my_rsbuf[2] = length * 10 + 8;

        my_rsbuf[4] = 0x73; //��������Ϊ53/73
        my_GPRS_101_geneate_FCBword(my_rsbuf);

        my_rsbuf[5] = DTU_ADDRESS & 0X00FF;
        my_rsbuf[6] = (DTU_ADDRESS >> 8) & 0X00FF;

        my_rsbuf[7] = 0X1E; //���ͱ�ʶ������ʱ��ĵ�����Ϣ��
        my_rsbuf[8] = length; //��Ϣ�����
        my_rsbuf[9] = 0x03; //����ԭ��

        my_rsbuf[10] = DTU_ADDRESS & 0X00FF; //�������ַ
        my_rsbuf[11] = (DTU_ADDRESS >> 8) & 0X00FF;

        my_buf1_to_buf2(my_buf, 2, my_rsbuf, 12, length * 10);

        my_rsbuf[11 + length * 10 + 1] = 0XFF;
        my_rsbuf[11 + length * 10 + 1 + 1] = 0x16;

        wdz_GPRS_101check_generate(my_rsbuf); //����У���ֽ�


    }
    else if(temp == 0)
    {
        length = 2;
        my_rsbuf[0] = 0x68;
        my_rsbuf[3] = 0x68;
        my_rsbuf[1] = length + 10;
        my_rsbuf[2] = length + 10;

        my_rsbuf[4] = 0x73; //��������Ϊ53/73
        my_GPRS_101_geneate_FCBword(my_rsbuf);

        my_rsbuf[5] = DTU_ADDRESS & 0X00FF;
        my_rsbuf[6] = (DTU_ADDRESS >> 8) & 0X00FF;

        my_rsbuf[7] = 0X1E; //���ͱ�ʶ����ʱ��ĵ�����Ϣ��
        my_rsbuf[8] = 0x00; //��Ϣ�����
        my_rsbuf[9] = 0x03; //����ԭ��

        my_rsbuf[10] = DTU_ADDRESS & 0X00FF; //�������ַ
        my_rsbuf[11] = (DTU_ADDRESS >> 8) & 0X00FF;;

        my_rsbuf[12] = 0x00; //ң����Ϣ���׵�ַ
        my_rsbuf[13] = 0x00;

        my_rsbuf[14] = 0x00;
        my_rsbuf[15] = 0x00;

        my_rsbuf[16] = 0XFF;
        my_rsbuf[17] = 0x16;

        wdz_GPRS_101check_generate(my_rsbuf); //����У���ֽ�

    }



}
//--------�����ó���--------------------------
/*
���ܣ����ԣ�����������д�뵽GPRS���������У�������ң��֡��ң��֡���������� �й�
*/
void my_test_calldata_to_GPRS_array(void)
{
    uint8_t my_status = 0;
    uint8_t my_txbuf[256] = {0};

    my_status = my_eeprom_data_to_cycle_array();

    if(my_status == 1)
    {
        //���ɷ������ݰ����������ݣ��ȷ�ң�š��ٷ�ң�⡢��󷢻���
        //ң�����ݰ� 68 23 23 68 73 01 00 01 98 14 00 01 00 01 00 00 00 00 00 00 00 00 23 16

        my_gprs_generate_101single_data(1, my_txbuf);
        USART_printf(&huart3, "\r\n");
        USART_printf(&huart3, my_txbuf);
        USART_printf(&huart3, "\r\n");

        //ң�����ݰ�
        //68 53 53 68 53 01 00 09 98 14 00 01 00 01 40 00 00 00 00 00 00 00 00 00 00 4B 16
        my_gprs_generate_101analog_data(1, my_txbuf);
        USART_printf(&huart3, "\r\n");
        USART_printf(&huart3, my_txbuf);
        USART_printf(&huart3, "\r\n");
        //�������ݰ�
        //68 53 53 68 53 01 00 09 98 14 00 01 00 00 41 00 00 00 00 00 00 00 00 00 00 4B 16
        my_gprs_generate_101MCU_data(1, my_txbuf);
        USART_printf(&huart3, "\r\n");
        USART_printf(&huart3, my_txbuf);
        USART_printf(&huart3, "\r\n");
    }

}

/*
���ܣ����в����������ݼ�⼰����

��Ҫ�Ĵ�������MY_433_Alarmdata_NOtimeBuf��MY_433_Alarmdata_timeBuf���Ʊ������ݣ�MY_433_ALarmdata_number������Ϣ�����
Ȼ���������к������з���
      my_status=WDZ_GPRS_101Transmit_Alarm_Data();//����GPRS���緢�ͱ������ݣ�
			if(my_status==1)
			my_GPRS_chang_tablereadpt(1,1); //ͬʱ�޸�eeprom�У���д״̬��дָ�롣
			else my_GPRS_chang_tablereadpt(1,0);  //ֻ�޸Ķ�ָ���ַ�������޸�֡����״̬�ֽڡ�
*/
void my_re_transmit_alarmdata()
{
    int transmint_alarmdata_status = 0;
    int my_status = 0;

    uint8_t my_buf[256] = {0};

    uint32_t first_address = 0;
    uint32_t second_address = 0;
    uint8_t alarm_data_number = 0;
    uint8_t rd_status = 0;
    uint8_t length = 0;
    int k = 0;
    uint8_t tempbuf[12] = {0};
    uint32_t startaddressPT = MY_Table1_Alarmdata_StartAddress;
    uint32_t endaddressPT = MY_Table1_Alarmdata_EndAddress;
    uint32_t writeaddressPT = MY_Table1_Alarmdata_WriteAddress;
    uint32_t readaddressPT = MY_Table1_Alarmdata_ReadAddress;
    uint32_t tableaddress = 210; //TABLE1��ָ���׵�ַ
    int temp_address1 = 0;
    int temp_address2 = 0;

    transmint_alarmdata_status = AT25_ReadByte(200); //��ȡ����״̬λ

    //transmint_alarmdata_status=1;//����ʹ�ã�ǿ��Ϊ1����������ʱ���ε�@@@@@

    if(transmint_alarmdata_status == 1) //Ϊ1��ʾ����������Ҫ����
    {

        //***����������µı�ָ���ַ
        SPI_EE_BufferRead2(tempbuf, 201, 12);
        if(tempbuf[0] == tempbuf[1] && tempbuf[1] == tempbuf[2] && tempbuf[2] == tempbuf[3] && tempbuf[3] == tempbuf[4] && tempbuf[4] == tempbuf[5])
        {

            my_val_writeto_eeprom(tempbuf, startaddressPT, endaddressPT, writeaddressPT, readaddressPT, 201);

        }
        else
        {

            my_buf_writeto_val(tempbuf, &startaddressPT, &endaddressPT, &writeaddressPT, &readaddressPT);

            MY_Table1_Alarmdata_StartAddress = startaddressPT;
            MY_Table1_Alarmdata_EndAddress = endaddressPT;
            MY_Table1_Alarmdata_WriteAddress = writeaddressPT;
            MY_Table1_Alarmdata_ReadAddress = readaddressPT;

        }
        //-----��Ҫ���ж�ָ��У�飬�����ĩβ�˴��¸�ֵ����Ϊ��ʼ��ַ��Ȼ��ʼ��
        if(readaddressPT + 12 > endaddressPT)
        {   readaddressPT = startaddressPT;
            MY_Table1_Alarmdata_ReadAddress = readaddressPT;
            tempbuf[0] = readaddressPT & 0x0000ff;
            tempbuf[1] = (readaddressPT >> 8) & 0x0000ff;
            tempbuf[2] = (readaddressPT >> 16) & 0x0000ff;
            SPI_EE_BufferWrite2(tempbuf, 210, 3);
        }
        //-------------------------



        first_address = readaddressPT;
        //first_address=startaddressPT;  //�����ã���������ʱɾ�� @@@@@
        second_address = writeaddressPT;

        //��ѯTable1 ����ñ������ݣ�������ʱ�걨������֡����ʱ�걨������֡

        if(first_address < second_address) //1-дָ���ڸߵ�ַ����ָ���ڵ͵�ַ
        {
            while(first_address < second_address)
            {   my_status = 0;
                SPI_EE_BufferRead2(my_buf, first_address, 252); //��ȡǰһ֡���ݵ�ǰ3���ַ�
                rd_status = my_buf[0]; //GPRS����״̬��2Ϊδ���ͣ�1Ϊ�ѷ���
                alarm_data_number = my_buf[1];  //��֡�����к��е���Ϣ�����
                //����ָ�����ǿ��У�� 2015-11-16
                if((rd_status != 1 && rd_status != 2) || alarm_data_number != 1)
                {
                    first_address = second_address; // ����ָ��У������һ������
                    MY_Table1_Alarmdata_ReadAddress = first_address;
                    my_save_PTTO_EEROM(first_address, tableaddress);
                    return ;
                }


                length = 2 + alarm_data_number * 10; //�����֡���ݵ��ܳ���
                length = 12; //�̶����ȣ�����û�в�����

                if(rd_status == 2 && alarm_data_number == 1 ) //��֡����û�з��ͳ�ȥ,���з��ʹ���
                {
                    SPI_EE_BufferRead2(my_buf, first_address, length); //1�ֽ�ΪGPRS����״̬��2�ֽ�Ϊ������3�ֽ�Ϊ��ַ��λ��4�ֽ�Ϊ��ַ��λ��5�ֽ�Ϊֵ��6-12Ϊʱ��
                    for(k = 0; k < alarm_data_number; k++) //ÿ����Ϣ��ռ10���ֽڣ���ַ2����ֵһ����ʱ��7��
                    {   temp_address1 = k * 10;
                        temp_address2 = k * 12;
                        MY_433_Alarmdata_TimeBuf[temp_address1 + 0] = my_buf[2 + temp_address2];
                        MY_433_Alarmdata_TimeBuf[temp_address1 + 1] = my_buf[3 + temp_address2];
                        MY_433_Alarmdata_TimeBuf[temp_address1 + 2] = my_buf[4 + temp_address2];
                        MY_433_Alarmdata_TimeBuf[temp_address1 + 3] = my_buf[5 + temp_address2];
                        MY_433_Alarmdata_TimeBuf[temp_address1 + 4] = my_buf[6 + temp_address2];
                        MY_433_Alarmdata_TimeBuf[temp_address1 + 5] = my_buf[7 + temp_address2];
                        MY_433_Alarmdata_TimeBuf[temp_address1 + 6] = my_buf[8 + temp_address2];
                        MY_433_Alarmdata_TimeBuf[temp_address1 + 7] = my_buf[9 + temp_address2];
                        MY_433_Alarmdata_TimeBuf[temp_address1 + 8] = my_buf[10 + temp_address2];
                        MY_433_Alarmdata_TimeBuf[temp_address1 + 9] = my_buf[11 + temp_address2];
                    }
                    for(k = 0; k < alarm_data_number; k++) //ÿ����Ϣ��ռ3���ֽڣ���ַ2����ֵһ��
                    {   temp_address1 = k * 3;
                        temp_address2 = k * 12;
                        MY_433_Alarmdata_NOtimeBuf[temp_address1 + 0] = my_buf[2 + temp_address2];
                        MY_433_Alarmdata_NOtimeBuf[temp_address1 + 1] = my_buf[3 + temp_address2];
                        MY_433_Alarmdata_NOtimeBuf[temp_address1 + 2] = my_buf[4 + temp_address2];
                    }
                    MY_433_ALarmdata_number = alarm_data_number; //��Ϣ�����

                    my_status = WDZ_GPRS_101Transmit_Alarm_Data(); //����GPRS���緢�ͱ������ݣ�
                    if(my_status == 1)
                    {
                        my_GPRS_chang_tablereadpt(1, 1); //ͬʱ�޸�eeprom�У���д״̬�Ͷ�ָ���ַ��
                        first_address = first_address + length; //ָ���ƶ�����һ֡����
                        //��ָ���¼��rom��
                        // tempbuf[0]=first_address&0x0000ff;
                        // tempbuf[1]=(first_address>>8)&0x0000ff;
                        //  tempbuf[2]=(first_address>>16)&0x0000ff;
                        //  SPI_EE_BufferWrite2(tempbuf,tableaddress,3);
                    }
                    else
                    {
                        my_GPRS_chang_tablereadpt(1, 0); //��ָ�벻�ƶ��������֡����״̬Ϊ2���޸Ĳ���״̬Ϊ1
                        //Ϊ�˷�ֹ��ָ�벻�ƶ������������⣬��������������֡���ȴ�����һ��timeɨ�����ڷ�����һ��֡
                        //	first_address=first_address+length;   //ָ���ƶ�����һ֡����??

                        //��ָ���¼��rom��
                        // tempbuf[0]=first_address&0x0000ff;
                        // tempbuf[1]=(first_address>>8)&0x0000ff;
                        //tempbuf[2]=(first_address>>16)&0x0000ff;
                        //SPI_EE_BufferWrite2(tempbuf,tableaddress,3);
                        //	my_save_PTTO_EEROM(first_address,tableaddress);
                        break;  //����ʧ���ˣ��Ͳ�Ҫ�ڼ�������
                    }
                }
                else if(rd_status == 1 && alarm_data_number == 1) //��֡�����Ѿ���������ˣ�������һ֡���ݽ��м��
                {

                    first_address = first_address + length; //ָ���ƶ�����һ֡����

                    //��ָ���¼��rom��
                    //tempbuf[0]=first_address&0x0000ff;
                    //tempbuf[1]=(first_address>>8)&0x0000ff;
                    // tempbuf[2]=(first_address>>16)&0x0000ff;
                    // SPI_EE_BufferWrite2(tempbuf,tableaddress,3);
                    MY_Table1_Alarmdata_ReadAddress = first_address;
                    my_save_PTTO_EEROM(first_address, tableaddress);
                }
                else if(rd_status == 0 || rd_status > 2 || alarm_data_number != 1) //���ָ��������⣬����Ư����,��ָ���ƶ���дָ�봦
                {
                    first_address = first_address + 12;
                }
                //�ж϶�ָ���Ƿ��������
                if(first_address + 12 >= endaddressPT) //
                {
                    first_address = startaddressPT;
                }

            }

            if(my_status == 1) //Ϊ1��ʾ�ɹ��ˣ����в������ݶ���ȥ��
            {
                AT25_WriteByte(0x00, 200);
                RE_ALarmData_Status = 0x00;
                first_address = second_address; // ����ָ��У������һ������

                //��ָ���¼��rom��
                // tempbuf[0]=first_address&0x0000ff;
                //tempbuf[1]=(first_address>>8)&0x0000ff;
                //tempbuf[2]=(first_address>>16)&0x0000ff;
                //SPI_EE_BufferWrite2(tempbuf,tableaddress,3);
                MY_Table1_Alarmdata_ReadAddress = first_address;
                my_save_PTTO_EEROM(first_address, tableaddress);
            }
            else if(my_status == 0 && first_address >= second_address)
            {
                AT25_WriteByte(0x00, 200); //�������֡��������,�����˲�����������״̬
                RE_ALarmData_Status = 00;
                //ָ��У��
                first_address = second_address; // ����ָ��У������һ������

                //��ָ���¼��rom��
                //tempbuf[0]=first_address&0x0000ff;
                //tempbuf[1]=(first_address>>8)&0x0000ff;
                //tempbuf[2]=(first_address>>16)&0x0000ff;
                //SPI_EE_BufferWrite2(tempbuf,tableaddress,3);
                MY_Table1_Alarmdata_ReadAddress = first_address;
                my_save_PTTO_EEROM(first_address, tableaddress);

            }


        }
        else if(first_address > second_address) //2-дָ���ڵ͵�ַ����ָ���ڸߵ�ַ
        {
            while((first_address + 12) < endaddressPT)
            {   my_status = 0;
                SPI_EE_BufferRead2(my_buf, first_address, 3); //��ȡǰһ֡���ݵ�ǰ3���ַ�
                rd_status = my_buf[0]; //GPRS����״̬��2Ϊδ���ͣ�1Ϊ�ѷ���
                alarm_data_number = my_buf[1];  //��֡�����к��е���Ϣ�����
                //����ָ�����ǿ��У�� 2015-11-16
                if((rd_status != 1 && rd_status != 2) || alarm_data_number != 1)
                {
                    first_address = second_address; // ����ָ��У������һ������
                    MY_Table1_Alarmdata_ReadAddress = first_address;
                    my_save_PTTO_EEROM(first_address, tableaddress);
                    return ;
                }



                length = 2 + alarm_data_number * 10; //�����֡���ݵ��ܳ���
                length = 12; //�̶����ȣ�����û�в�����

                if(rd_status == 2 && alarm_data_number == 1) //��֡����û�з��ͳ�ȥ,���з��ʹ���
                {
                    SPI_EE_BufferRead2(my_buf, first_address, length); //1�ֽ�ΪGPRS����״̬��2�ֽ�Ϊ������3�ֽ�Ϊ��ַ��λ��4�ֽ�Ϊ��ַ��λ��5�ֽ�Ϊֵ��6-12Ϊʱ��
                    for(k = 0; k < alarm_data_number; k++) //ÿ����Ϣ��ռ10���ֽڣ���ַ2����ֵһ����ʱ��7��
                    {
                        temp_address1 = k * 10;
                        temp_address2 = k * 12;
                        MY_433_Alarmdata_TimeBuf[temp_address1 + 0] = my_buf[2 + temp_address2];
                        MY_433_Alarmdata_TimeBuf[temp_address1 + 1] = my_buf[3 + temp_address2];
                        MY_433_Alarmdata_TimeBuf[temp_address1 + 2] = my_buf[4 + temp_address2];
                        MY_433_Alarmdata_TimeBuf[temp_address1 + 3] = my_buf[5 + temp_address2];
                        MY_433_Alarmdata_TimeBuf[temp_address1 + 4] = my_buf[6 + temp_address2];
                        MY_433_Alarmdata_TimeBuf[temp_address1 + 5] = my_buf[7 + temp_address2];
                        MY_433_Alarmdata_TimeBuf[temp_address1 + 6] = my_buf[8 + temp_address2];
                        MY_433_Alarmdata_TimeBuf[temp_address1 + 7] = my_buf[9 + temp_address2];
                        MY_433_Alarmdata_TimeBuf[temp_address1 + 8] = my_buf[10 + temp_address2];
                        MY_433_Alarmdata_TimeBuf[temp_address1 + 9] = my_buf[11 + temp_address2];
                    }
                    for(k = 0; k < alarm_data_number; k++) //ÿ����Ϣ��ռ3���ֽڣ���ַ2����ֵһ��
                    {
                        temp_address1 = k * 3;
                        temp_address2 = k * 12;
                        MY_433_Alarmdata_NOtimeBuf[temp_address1 + 0] = my_buf[2 + temp_address2];
                        MY_433_Alarmdata_NOtimeBuf[temp_address1 + 1] = my_buf[3 + temp_address2];
                        MY_433_Alarmdata_NOtimeBuf[temp_address1 + 2] = my_buf[4 + temp_address2];
                    }
                    MY_433_ALarmdata_number = alarm_data_number; //��Ϣ�����

                    my_status = WDZ_GPRS_101Transmit_Alarm_Data(); //����GPRS���緢�ͱ������ݣ�
                    if(my_status == 1 )
                    {
                        my_GPRS_chang_tablereadpt(1, 1); //ͬʱ�޸�eeprom�У���д״̬�Ͷ�ָ���ַ��
                        first_address = first_address + length; //ָ���ƶ�����һ֡����
                        //��ָ���¼��rom��
                        //tempbuf[0]=first_address&0x0000ff;
                        //tempbuf[1]=(first_address>>8)&0x0000ff;
                        //tempbuf[2]=(first_address>>16)&0x0000ff;
                        //SPI_EE_BufferWrite2(tempbuf,tableaddress,3);
                    }
                    else
                    {
                        my_GPRS_chang_tablereadpt(1, 0); //��ָ�벻�ƶ��������֡����״̬Ϊ2���޸Ĳ���״̬Ϊ1
                        //Ϊ�˷�ֹ��ָ�벻�ƶ������������⣬��������������֡���ȴ�����һ��timeɨ�����ڷ�����һ��֡
                        //first_address=first_address+length;   //ָ���ƶ�����һ֡����

                        //��ָ���¼��rom��
                        //tempbuf[0]=first_address&0x0000ff;
                        //tempbuf[1]=(first_address>>8)&0x0000ff;
                        //tempbuf[2]=(first_address>>16)&0x0000ff;
                        //SPI_EE_BufferWrite2(tempbuf,tableaddress,3);
                        //  my_save_PTTO_EEROM(first_address,tableaddress);

                        break;  //����ʧ���ˣ��Ͳ�Ҫ�ڼ�������
                    }
                }
                else if(rd_status == 1 && alarm_data_number == 1) //��֡�����Ѿ���������ˣ�������һ֡���ݽ��м��
                {
                    first_address = first_address + length; //ָ���ƶ�����һ֡����

                    //��ָ���¼��rom��
                    //tempbuf[0]=first_address&0x0000ff;
                    //tempbuf[1]=(first_address>>8)&0x0000ff;
                    //tempbuf[2]=(first_address>>16)&0x0000ff;
                    //SPI_EE_BufferWrite2(tempbuf,tableaddress,3);
                    MY_Table1_Alarmdata_ReadAddress = first_address;
                    my_save_PTTO_EEROM(first_address, tableaddress);
                }
                else if(rd_status == 0 || alarm_data_number == 0 || alarm_data_number > 1) //���ָ��������⣬����Ư����,��ָ�븴λΪ1
                {
                    first_address = endaddressPT;
                }

            }

            if(my_status == 1) //Ϊ1��ʾ�ɹ��ˣ����в������ݶ���ȥ��
            {
                //AT25_WriteByte(0x00,200);
                first_address = startaddressPT; // ����ָ��У������һ������

                //��ָ���¼��rom��
                //tempbuf[0]=first_address&0x0000ff;
                //tempbuf[1]=(first_address>>8)&0x0000ff;
                //tempbuf[2]=(first_address>>16)&0x0000ff;
                //SPI_EE_BufferWrite2(tempbuf,tableaddress,3);
                MY_Table1_Alarmdata_ReadAddress = first_address;
                my_save_PTTO_EEROM(first_address, tableaddress);
            }
            else if(my_status == 0 && (first_address + 12) > endaddressPT)
            {
                //ָ��У��
                first_address = startaddressPT; // ����ָ��У������һ������

                //��ָ���¼��rom��
                //tempbuf[0]=first_address&0x0000ff;
                //tempbuf[1]=(first_address>>8)&0x0000ff;
                //tempbuf[2]=(first_address>>16)&0x0000ff;
                // SPI_EE_BufferWrite2(tempbuf,tableaddress,3);
                MY_Table1_Alarmdata_ReadAddress = first_address;
                my_save_PTTO_EEROM(first_address, tableaddress);

            }



        }




    }

}

/*
	���ܣ��洢mypt��ֵַ�ĵ�3���ֽڵ�EEPROM��tabladdress��ʼ�ĵ�ַ
	*/
void my_save_PTTO_EEROM(uint32_t mypt, uint32_t tableaddress)
{   uint8_t tempbuf[3] = {0};
    uint32_t first_address = mypt;
    tempbuf[0] = first_address & 0x0000ff;
    tempbuf[1] = (first_address >> 8) & 0x0000ff;
    tempbuf[2] = (first_address >> 16) & 0x0000ff;
    SPI_EE_BufferWrite2(tempbuf, tableaddress, 3);
}


/*
���ܣ�����  ����  ָ�������Ӧ����������
*/
uint8_t WDZ_GPRS_101Resiver_ControlMonitor_Data(void)
{
    uint8_t my_status = 0;
//	uint8_t my_rxbuf[256]="\x10\x80\x01\x00\x81\x16";
    uint8_t my_txbuf[256] = {0X68, 0X0E, 0X0E, 0X68, 0X00, 0X01, 0X00, 0X2D, 0X02, 0X07, 0X01, 0X00, 0X01, 0X60, 0X01, 0X02, 0X60, 0X00, 0XFF, 0X16};
    uint8_t *myrsbuf1 = USART1_my_frame;
    uint8_t status_value = 0;
    uint8_t my_address = 0;


    //��1�������յ�����ָ��	 ��վ���ͣ�68 0B 0B 68 73 01 00 64 01 06 01 00 00 00 14 F4 16
    my_status = WDZ_GPRS_101receive_testactive2(0X73, 0X2D, 0X06, 300);
    if(my_status == 0)  return my_status; //   ���ָ����󣬷��أ������һ��ָ��

    if(my_status == 1)
    {
        //���Ϳ϶�ȷ��ָ��
        //	WDZ_GPRS_101Transmit_OKdata();
        my_txbuf[5] = DTU_ADDRESS & 0X00FF;
        my_txbuf[6] = DTU_ADDRESS >> 8;
        my_txbuf[8] = 2;
        my_txbuf[10] = DTU_ADDRESS & 0X00FF;
        my_txbuf[11] = DTU_ADDRESS >> 8;
        my_txbuf[12] = myrsbuf1[12]; //01
        my_txbuf[13] = myrsbuf1[13]; //60
        my_txbuf[14] = myrsbuf1[14]; //ָʾ����ַ��01,02,03��04��05,06,07,08,09
        my_txbuf[15] = myrsbuf1[15]; //02,���ƣ�03��λ
        my_txbuf[16] = myrsbuf1[16]; //60
        my_txbuf[17] = myrsbuf1[17]; //00
        wdz_GPRS_101check_generate(my_txbuf); //����У���ֽ�

        //my_at_senddata(my_txbuf);  //����GPRS����ȷ��ָ��
        //����433���������з��Ʋ���
        if(my_txbuf[15] == 0X02) //����
            status_value = 0x81;
        else if(my_txbuf[15] == 0X03)
            status_value = 0x80;
        if(my_txbuf[14] >= 4 && my_txbuf[14] <= 6)
            my_address = my_txbuf[14] + 1;
        else if(my_txbuf[14] >= 7 && my_txbuf[14] <= 9)
            my_address = my_txbuf[14] + 2;
        else if(my_txbuf[14] >= 1 && my_txbuf[14] <= 3)
            my_address = my_txbuf[14] + 0;

        my_status = WDZ_101Transmit_Control_monitor(my_address, status_value);		//��433ģ�鷢��ָ�����ָʾ��
    }
    else if(my_status == 0)
    {
        USART_printf(&huart3, " Server ControlMonitor fail-1\r\n");
    }

    if(my_status == 1)
    {
        USART_printf(&huart3, " Server ControlMonitor* %d\r\n", my_address);
        my_at_senddata(my_txbuf);  //����GPRS����ȷ��ָ��
    }
    else if(my_status == 0)
    {
        USART_printf(&huart3, " Server ControlMonitor fail-2\r\n");
    }



    return my_status;
}

void my_reset_mcu()  //����MCUͨ��������
{
    __disable_fault_irq();
    NVIC_SystemReset();
}

/*
���ܣ���������ȴ����ض�����������,����DTU��ַʹ�ã
��������typeΪ���ͣ�1Ϊ�̶����ȣ�2Ϊ�ǹ̶����ȣ�txbufΪ����ָ������,address_typeΪ1��rxbufΪ���գ�DTU��ַ
��2,txbuf,2,rxbuf)���ͷǹ̶����ȣ�(1,txbuf,2,rxbuf)���͹̶�����
*/
uint8_t WDZ_GPRS_101transmint_commd_wait_commd2(uint8_t type, uint8_t *txbuf, uint8_t address_type, uint8_t *rxbuf, uint16_t my_temp_dtu_address) //typeΪ���ͣ�1Ϊ�̶����ȣ�2Ϊ�ǹ̶����ȣ�txbufΪ����ָ������
{
    uint8_t FCB = 0x20;
    uint16_t my_dtu_address = my_temp_dtu_address;
    uint8_t my_status = 0;
    uint8_t n = 1; //���ʹ���
    uint8_t *my_txbuf = txbuf;
//	uint8_t temp=0;

    uint8_t linkcontrol = 0;
    uint8_t type_identification = 0;
    uint8_t  transmit_reason = 0;
    uint16_t my_start_time = 0;
//********************���ͷǹ̶�����֡��Ӧ��ΪOK֡
    if(type == 2)
    {
        //�������봦��
        if(WDZ_GPRS_101FCB == 0x00)
            my_txbuf[4] = my_txbuf[4] & (~FCB);
        else if(WDZ_GPRS_101FCB == 0x20)
            my_txbuf[4] = my_txbuf[4] | (FCB);
        //Ŀ�ĵ�ַ����
        my_txbuf[5] = my_temp_dtu_address & 0xFF;
        my_txbuf[6] = my_temp_dtu_address >> 8;
        if(address_type == 1)
        {
            my_txbuf[10] = my_txbuf[5];
            my_txbuf[11] = my_txbuf[6];
        }
        else if(address_type == 2)
        {
            my_txbuf[10] = my_txbuf[5];
            my_txbuf[11] = my_txbuf[6];
        }

        //У��λ����
        wdz_GPRS_101check_generate(my_txbuf);
        //����ָ��
        while((n) > 0 && my_status == 0)
        {

            my_at_senddata(my_txbuf); //����GPRS������������

            //Delay_us(300); //����ʹ��


            my_start_time = my_tim6_count;
            do {

                my_status = WDZ_GPRS_101receive_testactive(0X80, 00, 00, 1500); ///WDZ_GPRS_101recive_OKdata();  //�ȴ�0Kȷ�Ϸ������� 15��
                if(my_status == 1)break;
                else my_status = 0;
            } while(my_tim6_count - my_start_time <= 3);
            n--;
            if(n != 0) USART_printf(&huart1, "\x1A"); //��һ������������ֹӵ��

        }



    }


//**************���͹̶�����֡��Ӧ��Ϊ�̶�����֡
    else	if(type == 1 && rxbuf[0] == 0x10)
    {
        //�������յĿ������룬���ͣ�����ԭ��
        linkcontrol = rxbuf[1];
        type_identification = 0;
        transmit_reason = 0;

        //�������봦��
        if(WDZ_GPRS_101FCB == 0x00)
            my_txbuf[1] = my_txbuf[1] & (~FCB);
        else if(WDZ_GPRS_101FCB == 0x20)
            my_txbuf[1] = my_txbuf[1] | (FCB);
        //Ŀ�ĵ�ַ����
        my_txbuf[2] = my_dtu_address & 0xFF;
        my_txbuf[3] = my_dtu_address >> 8;

        //У��λ����
        wdz_GPRS_101check_generate(my_txbuf);
        //����ָ��
        while((n--) > 0 && my_status == 0)
        {
            //my_UART4_printf(UART4,my_txbuf);
            my_at_senddata(my_txbuf); //����GPRS������������

            //Delay_us(300); //����

            my_status = WDZ_GPRS_101receive_testactive(linkcontrol, type_identification, transmit_reason, 1500); //�ȴ���������
            if(my_status == 1)break;
            else my_status = 0;
        }
    }
//**********���͹̶�����֡��Ӧ��Ϊ�ǹ̶�����֡
    else if(type == 1 && rxbuf[0] == 0x68)
    {
        //�������յĿ������룬���ͣ�����ԭ��
        linkcontrol = rxbuf[4];
        type_identification = rxbuf[7];
        transmit_reason = rxbuf[9];

        //�������봦��
        if(WDZ_GPRS_101FCB == 0x00)
            my_txbuf[1] = my_txbuf[1] & (~FCB);
        else if(WDZ_GPRS_101FCB == 0x20)
            my_txbuf[1] = my_txbuf[1] | (FCB);
        //Ŀ�ĵ�ַ����
        my_txbuf[2] = my_temp_dtu_address & 0xFF;
        my_txbuf[3] = my_temp_dtu_address >> 8;

        //У��λ����
        wdz_GPRS_101check_generate(my_txbuf);
        //����ָ��
        while((n--) > 0 && my_status == 0)
        {
            //my_UART4_printf(UART4,my_txbuf);
            my_at_senddata(my_txbuf);

            //Delay_us(600); //����

            my_status = WDZ_GPRS_101receive_testactive(linkcontrol, type_identification, transmit_reason, 1500); //�ȴ���������
            if(my_status == 1)break;
            else my_status = 0;
        }
    }


    //��¼���ͱ仯֡
    WDZ_GPRS_101FCB = WDZ_GPRS_101FCB ^ 0x20;
    if(my_status == 1)
    {
        return 1;
    }
    else return 0;

}

/*
���ܣ�����ָʾ��ң����������ʱ�䣬һ��ֻ�ܵ���һ��ָʾ����
*/
uint8_t WDZ_GPRS_101Resiver_Cycle_analog_time_Data(void)
{
    uint8_t my_status = 0;
//	uint8_t my_rxbuf[256]="\x10\x80\x01\x00\x81\x16";
//68 0E 0E 68 73 03 00 3D 02 07 03 00 01 70 01 02 70 01 72 16

    uint8_t my_txbuf[256] = {0X68, 0X0E, 0X0E, 0X68, 0X73, 0X03, 0X00, 0X3D, 0X02, 0X07, 0X03, 0X00, 0X01, 0X70, 0X01, 0X02, 0X70, 0X01, 0XFF, 0X16};
    uint8_t *myrsbuf1 = USART1_my_frame;
    uint8_t status_value = 0;
    uint8_t my_address = 0;


    //��1�������յ�����ָ��	 ��վ���ͣ�68 0E 0E 68 73 03 00 3D 02 06 03 00 01 70 01 02 70 01 72 16
    my_status = WDZ_GPRS_101receive_testactive2(0X73, 0X3D, 0X06, 300);
    if(my_status == 0)  return my_status; //   ���ָ����󣬷��أ������һ��ָ��

    if(my_status == 1)
    {
        //���Ϳ϶�ȷ��ָ��
        //	WDZ_GPRS_101Transmit_OKdata();
        my_txbuf[5] = DTU_ADDRESS & 0X00FF;
        my_txbuf[6] = DTU_ADDRESS >> 8;
        my_txbuf[8] = 2;
        my_txbuf[10] = DTU_ADDRESS & 0X00FF;
        my_txbuf[11] = DTU_ADDRESS >> 8;
        my_txbuf[12] = myrsbuf1[12]; //01
        my_txbuf[13] = myrsbuf1[13]; //70
        my_txbuf[14] = myrsbuf1[14]; //ָʾ����ַ��01,02,03��04��05,06,07,08,09
        my_txbuf[15] = myrsbuf1[15]; //02,
        my_txbuf[16] = myrsbuf1[16]; //70
        my_txbuf[17] = myrsbuf1[17]; //01����ʱ��
        wdz_GPRS_101check_generate(my_txbuf); //����У���ֽ�

        //my_at_senddata(my_txbuf);  //����GPRS����ȷ��ָ��
        //����433����������ָʾ��ʱ�����

        status_value = my_txbuf[17];

        if(my_txbuf[14] >= 4 && my_txbuf[14] <= 6)
            my_address = my_txbuf[14] + 1;
        else if(my_txbuf[14] >= 7 && my_txbuf[14] <= 9)
            my_address = my_txbuf[14] + 2;
        else if(my_txbuf[14] >= 1 && my_txbuf[14] <= 3)
            my_address = my_txbuf[14] + 0;

        my_status = WDZ_101Transmit_Cycle_analog_time(my_address, status_value);		//��433ģ�鷢��ָ�����ָʾ��
    }
    else if(my_status == 0)
    {
        USART_printf(&huart3, " Server CycleAnalogTime fail-1\r\n");
    }

    if(my_status == 1)
    {
        USART_printf(&huart3, " Server CycleAnalogTime* %d\r\n", my_address);
        my_at_senddata(my_txbuf);  //����GPRS����ȷ��ָ��
    }
    else if(my_status == 0)
    {
        USART_printf(&huart3, " Server CycleAnalogTime fail-2\r\n");
    }



    return my_status;
}

/*

���ܣ�ʵʱ�����������ݣ�ֱ�ӷ��͹������ݣ�����EEPROM�н��ж�ȡ�����������ݺ��޸�EEPROM�м�¼��״̬������������ʷ���ݷ���
*/
uint8_t WDZ_GPRS_101transmit_Cycle_realtime_data(void)
{
    uint8_t tempbuf[12] = {0};
    uint32_t startaddressPT = MY_Table2_Cycledata_StartAddress;
    uint32_t endaddressPT = MY_Table2_Cycledata_EndAddress;
    uint32_t writeaddressPT = MY_Table2_Cycledata_WriteAddress;
    uint32_t readaddressPT = MY_Table2_Cycledata_ReadAddress;
    uint32_t tableaddress = 0;
    uint8_t rs_temp_buf22[512] = {0}; //����ʹ��
    uint8_t my_status = 0;
    uint8_t tempstaus = 0;
    uint8_t my_rxbuf[256] = "\x10\x80\x01\x00\x81\x16";
    uint8_t my_txbuf[256] = TX_GPRS_101_testdata;



//***����������µı�ָ���ַ
    SPI_EE_BufferRead2(tempbuf, 213, 12);

    my_buf_writeto_val(tempbuf, &startaddressPT, &endaddressPT, &writeaddressPT, &readaddressPT);
    MY_Table2_Cycledata_StartAddress = startaddressPT;
    MY_Table2_Cycledata_EndAddress = endaddressPT;
    MY_Table2_Cycledata_WriteAddress = writeaddressPT;
    MY_Table2_Cycledata_ReadAddress = readaddressPT;

//--------------
//���ж�ָ��ĩβУ�飬�������ĩβ���ͷ�����ʼ��ַ��Ȼ��д�����
    if(readaddressPT + 144 > endaddressPT)
    {   readaddressPT = startaddressPT;
        MY_Table2_Cycledata_ReadAddress = readaddressPT;
        tempbuf[0] = readaddressPT & 0x0000ff;
        tempbuf[1] = (readaddressPT >> 8) & 0x0000ff;
        tempbuf[2] = (readaddressPT >> 16) & 0x0000ff;
        SPI_EE_BufferWrite2(tempbuf, 222, 3);
    }


//----------------------


//----------------
    if(readaddressPT + 144 == writeaddressPT) //��Ϊ��������������ʷ���ݣ���ָ���Ѿ��Ƶ����һ����¼���׵�ַ
    {

        if(GPRS_Status == 1 && link_status_GPRS == 1)
            my_status = 1;
        else
            my_status = 0;

        wdz_GPRS_string_to_array(TX_GPRS_101_testdata, my_txbuf); //����ָ��
        //my_status=WDZ_GPRS_101transmint_commd(2,my_txbuf,2);
        my_status = WDZ_GPRS_101transmint_commd_wait_commd(2, my_txbuf, 1, my_rxbuf);
        if(my_status == 1)
        {
            //my_status=WDZ_101receive_testactive(0x03,0x68,0x07,300);
//***********��Ҫ****��������������****
//ֱ�Ӵ�433�Ĺ������ݴ洢��Ԫ�ж�ȡ���ŵ�3���ض������У�Ȼ����3��101��Ϣ����ң�ţ�ң�⡢��������ʱ�ꡣ
//�ȴ�Server�϶�ȷ�ϣ�ȷ�Ϻ��޸�EEPROM�����ݰ���GPRS����״̬��ͬʱ�޸Ķ�ָ���ַ

            //tempstaus=my_eeprom_data_to_cycle_array(); //�����ݴ��뵽GPRS����������*********
            //MY_433_Call_Single_data_number
            //MY_433_Call_Analog_data_number
            //MY_MCU_RsBuf  //8���ֽ�
            //MY_433_Call_Single_data_buf   2*9=18
            //MY_433_Call_Analog_data_buf  3*4*9=108    8+18+108=134  134+7=141��ʱ�꣬ 141+3=144��״̬��ң�Ÿ�����ң�����
            //MY_433_Call_Time_data_buf  7���ֽ�

            if(MY_MCU_getdata_status == 1)
                my_buf1_to_buf2(MY_MCU_RsBuf, 0, MY_GPRS_MCU_RsBuf, 0, 8); //��������

            if(MY_433_Call_Status == 1) //����ң�š�ң�⡢ʱ������
            {
                MY_GPRS_Call_Single_data_number = MY_433_Call_Single_data_number;
                MY_GPRS_Call_Analog_data_number = MY_433_Call_Analog_data_number;
                my_buf1_to_buf2(MY_433_Call_Single_data_buf, 0, MY_GPRS_Call_Single_data_buf, 0, MY_GPRS_Call_Single_data_number); //ң������
                my_buf1_to_buf2(MY_433_Call_Analog_data_buf, 0, MY_GPRS_Call_Analog_data_buf, 0, MY_GPRS_Call_Analog_data_number * 3); //ң������
                my_buf1_to_buf2(MY_433_Call_Time_data_buf, 0, MY_GPRS_Call_Time_data_buf, 0, 7); //ʱ������
            }

            tempstaus = 1;
            if(tempstaus == 1)
            {
                if(MY_433_Call_Status == 1)
                {
                    //���ɷ������ݰ����������ݣ��ȷ�ң�š��ٷ�ң�⡢��󷢻���
                    //ң�����ݰ� 68 23 23 68 73 01 00 01 98 14 00 01 00 01 00 00 00 00 00 00 00 00 23 16
                    my_gprs_generate_101single_data(1, my_txbuf);
                    //my_at_senddata(my_txbuf);

                    //����
                    //my_status=WDZ_GPRS_101receive_testactive(0x80,00,00,1500);
                    //����ң������
                    my_status = my_status = WDZ_GPRS_101transmint_commd_wait_commd(2, my_txbuf, 1, my_rxbuf);
                    if(my_status == 1)
                    {

                        //ң�����ݰ�
                        //68 53 53 68 53 01 00 09 98 14 00 01 00 01 40 00 00 00 00 00 00 00 00 00 00 4B 16
                        my_gprs_generate_101analog_data(1, my_txbuf);
                        //my_at_senddata(my_txbuf);

                        //����
                        //my_status=WDZ_GPRS_101receive_testactive(0x80,00,00,1500);
                        //����ң������
                        my_status = my_status = WDZ_GPRS_101transmint_commd_wait_commd(2, my_txbuf, 1, my_rxbuf);


                        //����ʹ�ã���������@@@@@@@

                        USART_printf(&huart3, "real cycle data to show-433M data\r\n");
                        HexToStr2(rs_temp_buf22, MY_GPRS_Call_Single_data_buf, 18);
                        USART_printf(&huart3, rs_temp_buf22);
                        USART_printf(&huart3, "\r\n");
                        HexToStr2(rs_temp_buf22, MY_GPRS_Call_Analog_data_buf, 108);
                        USART_printf(&huart3, rs_temp_buf22);
                        USART_printf(&huart3, "\r\n");
                        HexToStr2(rs_temp_buf22, MY_GPRS_Call_Time_data_buf, 7);
                        USART_printf(&huart3, rs_temp_buf22);
                        USART_printf(&huart3, "\r\n");

                    }
                    else
                    {
                        my_status = 0;
                        USART_printf(&huart3, "GPRS real Sing data error\r\n"); //����ʹ��
                    }
                }
                //���ͻ�������
                if(my_status == 1 && MY_MCU_getdata_status == 1)
                {
                    //�������ݰ�
                    //68 53 53 68 53 01 00 09 98 14 00 01 00 00 41 00 00 00 00 00 00 00 00 00 00 4B 16
                    my_gprs_generate_101MCU_data(1, my_txbuf);
                    //my_at_senddata(my_txbuf);
                    //����
                    //my_status=WDZ_GPRS_101receive_testactive(0x80,00,00,1500);
                    my_status = my_status = WDZ_GPRS_101transmint_commd_wait_commd(2, my_txbuf, 1, my_rxbuf);

                    //����ʹ�ã���������@@@@@@@
                    USART_printf(&huart3, "real cycle data to show-MCU data\r\n");
                    HexToStr2(rs_temp_buf22, MY_GPRS_MCU_RsBuf, 8);
                    USART_printf(&huart3, rs_temp_buf22);
                    USART_printf(&huart3, "\r\n");

                }
                else
                {
                    my_status = 0;
                    USART_printf(&huart3, "GPRS real angale data error\r\n"); //����ʹ��
                }

                if(my_status == 0)
                {
                    USART_printf(&huart3, "GPRS MCU data error\r\n"); //����ʹ��
                }

            }
            else
            {
                //���������ݰ����������ݰ�
                my_gprs_generate_101single_data(0, my_txbuf);
                my_at_senddata(my_txbuf);
                my_gprs_generate_101analog_data(0, my_txbuf);
                my_at_senddata(my_txbuf);
                my_gprs_generate_101MCU_data(0, my_txbuf);
                my_at_senddata(my_txbuf);

            }

            //����server�����Ŀ϶�ȷ�����ݰ�
            //my_status=WDZ_GPRS_101receive_testactive(0x80,00,00,1500); //�ɵķ�������Ҫȷ�ϵķ���

        }


        else if(my_status == 0)
        {
            USART_printf(&huart3, " Server real Cycle data error3\r\n");

        }



        if(my_status == 1) //���ͳɹ��ˣ�����
        {
            //���з���֡״̬�ֽ��޸ģ����ƶ�EEPROM�ı���ָ��
            if(tempstaus == 1)
                my_GPRS_chang_tablereadpt(2, 1); //�ɹ����޸Ĵ�֡״̬Ϊ1����ʾ���ͳ�ȥ�ˣ��ƶ���ָ��
            else if(tempstaus == 0)
            {
                my_GPRS_chang_tablereadpt(2, 0);  //ʧ�ܣ��޸�Ϊ2��δ���ͣ����ƶ�ָ��
                USART_printf(&huart3, " Server real Cycle data errort4-1 \r\n");

            }

            //Delay_us(100);
            USART_printf(&huart3, " Server real Cycle data OK2*\r\n");

            // LED2_TOGGLE;
        }
        else if(my_status == 0)
        {   //Delay_us(100);
            USART_printf(&huart3, " Server real Cycle data errort4 \r\n");
            //LED3_TOGGLE;
        }
        //***����������µı�ָ���ַ
        SPI_EE_BufferRead2(tempbuf, 213, 12);
        my_buf_writeto_val(tempbuf, &startaddressPT, &endaddressPT, &writeaddressPT, &readaddressPT);
        MY_Table2_Cycledata_StartAddress = startaddressPT;
        MY_Table2_Cycledata_EndAddress = endaddressPT;
        MY_Table2_Cycledata_WriteAddress = writeaddressPT;
        MY_Table2_Cycledata_ReadAddress = readaddressPT;

        //���ж�ָ���ƶ�
        if(readaddressPT > writeaddressPT) //���ж�ָ��У��
        {   readaddressPT = writeaddressPT;

            tempbuf[0] = readaddressPT & 0x0000ff;
            tempbuf[1] = (readaddressPT >> 8) & 0x0000ff;
            tempbuf[2] = (readaddressPT >> 16) & 0x0000ff;
            tableaddress = 222;
            SPI_EE_BufferWrite2(tempbuf, tableaddress, 3);

        }


    }

//���������з���״̬����
    MY_MCU_getdata_status = 0; //PW�����������ݴ������
    MY_433_Call_Status = 0;  //433ģ���������ݴ������

    return my_status;

}


/*

���ܣ�ֻ�����������ݣ�������433ģ������
*/
uint8_t WDZ_GPRS_101transmit_Cycle_TH_data(void)
{
    uint8_t rs_temp_buf22[512] = {0}; //����ʹ��
    uint8_t my_status = 0;
    uint8_t my_rxbuf[256] = "\x10\x80\x01\x00\x81\x16";
    uint8_t my_txbuf[256] = TX_GPRS_101_testdata;
    int ii = 0;

//----------------

    if(GPRS_Status == 1 && MY_MCU_getdata_status == 1)
        my_status = 1;
    else
        my_status = 0;

    if(my_status == 1)
    {
        wdz_GPRS_string_to_array(TX_GPRS_101_testdata, my_txbuf); //����ָ��
        my_status = WDZ_GPRS_101transmint_commd_wait_commd(2, my_txbuf, 1, my_rxbuf);
    }
    if(my_status == 1)
    {



        my_buf1_to_buf2(MY_MCU_RsBuf, 0, MY_GPRS_MCU_RsBuf, 0, 8); //��������

        MY_GPRS_Call_Single_data_number = 0X12;
        MY_GPRS_Call_Analog_data_number = 0X24;
        for(ii = 0; ii < 40; ii++)
            MY_GPRS_Call_Single_data_buf[ii] = 0XEF; //ģ�����ݣ���0XEF����ʾ
        for(ii = 0; ii < 110; ii++)
            MY_GPRS_Call_Analog_data_buf[ii] = 0XEF;
        my_RTCtime_to_array(MY_GPRS_Call_Time_data_buf);

        //���ɷ������ݰ����������ݣ��ȷ�ң�š��ٷ�ң�⡢��󷢻���
        //ң�����ݰ� 68 23 23 68 73 01 00 01 98 14 00 01 00 01 00 00 00 00 00 00 00 00 23 16
        my_gprs_generate_101single_data(1, my_txbuf);
        //����ң������
        my_status = my_status = WDZ_GPRS_101transmint_commd_wait_commd(2, my_txbuf, 1, my_rxbuf);

    }
    if(my_status == 1)
    {

        //ң�����ݰ�
        //68 53 53 68 53 01 00 09 98 14 00 01 00 01 40 00 00 00 00 00 00 00 00 00 00 4B 16
        my_gprs_generate_101analog_data(1, my_txbuf);
        //����ң������
        my_status = my_status = WDZ_GPRS_101transmint_commd_wait_commd(2, my_txbuf, 1, my_rxbuf);


        //����ʹ�ã���������@@@@@@@
        HexToStr2(rs_temp_buf22, MY_GPRS_Call_Single_data_buf, 18);
        USART_printf(&huart3, rs_temp_buf22);
        USART_printf(&huart3, "\r\n");
        HexToStr2(rs_temp_buf22, MY_GPRS_Call_Analog_data_buf, 108);
        USART_printf(&huart3, rs_temp_buf22);
        USART_printf(&huart3, "\r\n");
        HexToStr2(rs_temp_buf22, MY_GPRS_Call_Time_data_buf, 7);
        USART_printf(&huart3, rs_temp_buf22);
        USART_printf(&huart3, "\r\n");

    }
    else
    {
        my_status = 0;
        USART_printf(&huart3, "GPRS TH Sing data error\r\n"); //����ʹ��
    }

    //���ͻ�������
    if(my_status == 1)
    {
        //�������ݰ�
        //68 53 53 68 53 01 00 09 98 14 00 01 00 00 41 00 00 00 00 00 00 00 00 00 00 4B 16
        my_gprs_generate_101MCU_data(1, my_txbuf);
        //my_at_senddata(my_txbuf);
        //����
        //my_status=WDZ_GPRS_101receive_testactive(0x80,00,00,1500);
        my_status = my_status = WDZ_GPRS_101transmint_commd_wait_commd(2, my_txbuf, 1, my_rxbuf);

        //����ʹ�ã���������@@@@@@@
        USART_printf(&huart3, "real TH cycle data to show-MCU data\r\n");
        HexToStr2(rs_temp_buf22, MY_GPRS_MCU_RsBuf, 8);
        USART_printf(&huart3, rs_temp_buf22);
        USART_printf(&huart3, "\r\n");

    }
    else
    {
        my_status = 0;
        USART_printf(&huart3, "GPRS real TH angale data error\r\n"); //����ʹ��
    }

    if(my_status == 0)
    {
        USART_printf(&huart3, "GPRS MCU TH data error\r\n"); //����ʹ��
    }

    if(my_status == 1) //���ͳɹ��ˣ�����
    {

        USART_printf(&huart3, " Server real TH Cycle data OK2*\r\n");

        // LED2_TOGGLE;
    }

//���������з���״̬����
    MY_MCU_getdata_status = 0; //PW�����������ݴ������
    MY_433_Call_Status = 0;  //433ģ���������ݴ������

    return my_status;

}

/*
���յ����ַ�ת����ASCII�������ʾ
*/

void my_display_ASCIIdata(uint8_t *rsbuf)
{
//	int tt=0;
    int length = 0;
//	uint8_t *mypt=rsbuf;
//	uint8_t my_temp1,my_temp2;
    uint8_t desbuf[512] = {0};

    if(*rsbuf == 0x10) length = 6;
    else if(*rsbuf == 0x68) length = *(rsbuf + 1) + 6;


    HexToStr2(desbuf, rsbuf, length);
    USART_printf(&huart3, desbuf);

    /*
    for(tt=0;tt<length;tt++)
    {
    	my_temp1=*mypt/16;
    	my_temp2=*mypt%16;
    	USART_printf(&huart3,"%d%d-",my_temp1,my_temp2);
    mypt++;
    }
    */
    USART_printf(&huart3, "\r\n");

}

/*

���ܣ�����433ģ�鱻���յ���ң�����ݣ�����GPRS���緢�ͣ�����͸�����䣬���ݲ����д洢
*/
uint8_t WDZ_GPRS_101transmit_analog_data(void)
{
    uint8_t rs_temp_buf22[512] = {0}; //����ʹ��
    uint8_t my_status = 0;
    uint8_t my_rxbuf[256] = "\x10\x80\x01\x00\x81\x16";
    uint8_t my_txbuf[256] = TX_GPRS_101_testdata;
    int ii = 0;

//----------------

    if(GPRS_Status == 1 && MY_MCU_getdata_status == 1)
        my_status = 1;
    else
        my_status = 0;

    if(my_status == 1)
    {
        wdz_GPRS_string_to_array(TX_GPRS_101_testdata, my_txbuf); //����ָ��
        my_status = WDZ_GPRS_101transmint_commd_wait_commd(2, my_txbuf, 1, my_rxbuf);
    }
    if(my_status == 1)
    {



        my_buf1_to_buf2(MY_MCU_RsBuf, 0, MY_GPRS_MCU_RsBuf, 0, 8); //��������

        MY_GPRS_Call_Single_data_number = 0X12;
        MY_GPRS_Call_Analog_data_number = 0X24;
        for(ii = 0; ii < 40; ii++)
            MY_GPRS_Call_Single_data_buf[ii] = 0XEF; //ģ�����ݣ���0XEF����ʾ
        for(ii = 0; ii < 110; ii++)
            MY_GPRS_Call_Analog_data_buf[ii] = my_433_anlag_buf[ii];	 //��ȡ433�������յ�ģ����
        my_RTCtime_to_array(MY_GPRS_Call_Time_data_buf);

        //���ɷ������ݰ����������ݣ��ȷ�ң�š��ٷ�ң�⡢��󷢻���
        //ң�����ݰ� 68 23 23 68 73 01 00 01 98 14 00 01 00 01 00 00 00 00 00 00 00 00 23 16
        my_gprs_generate_101single_data(1, my_txbuf);
        //����ң������
        my_status = my_status = WDZ_GPRS_101transmint_commd_wait_commd(2, my_txbuf, 1, my_rxbuf);

    }
    if(my_status == 1)
    {

        //ң�����ݰ�
        //68 53 53 68 53 01 00 09 98 14 00 01 00 01 40 00 00 00 00 00 00 00 00 00 00 4B 16
        my_gprs_generate_101analog_data(1, my_txbuf);
        //����ң������
        my_status = my_status = WDZ_GPRS_101transmint_commd_wait_commd(2, my_txbuf, 1, my_rxbuf);


        //����ʹ�ã���������@@@@@@@
        HexToStr2(rs_temp_buf22, MY_GPRS_Call_Single_data_buf, 18);
        USART_printf(&huart3, rs_temp_buf22);
        USART_printf(&huart3, "\r\n");
        HexToStr2(rs_temp_buf22, MY_GPRS_Call_Analog_data_buf, 108);
        USART_printf(&huart3, rs_temp_buf22);
        USART_printf(&huart3, "\r\n");
        HexToStr2(rs_temp_buf22, MY_GPRS_Call_Time_data_buf, 7);
        USART_printf(&huart3, rs_temp_buf22);
        USART_printf(&huart3, "\r\n");

    }
    else
    {
        my_status = 0;
        USART_printf(&huart3, "GPRS ANALOG Sing data error\r\n"); //����ʹ��
    }

    //���ͻ�������
    if(my_status == 1)
    {
        //�������ݰ�
        //68 53 53 68 53 01 00 09 98 14 00 01 00 00 41 00 00 00 00 00 00 00 00 00 00 4B 16
        my_gprs_generate_101MCU_data(1, my_txbuf);
        //my_at_senddata(my_txbuf);
        //����
        //my_status=WDZ_GPRS_101receive_testactive(0x80,00,00,1500);
        my_status = my_status = WDZ_GPRS_101transmint_commd_wait_commd(2, my_txbuf, 1, my_rxbuf);

        //����ʹ�ã���������@@@@@@@
        USART_printf(&huart3, "real ANALOG cycle data to show-MCU data\r\n");
        HexToStr2(rs_temp_buf22, MY_GPRS_MCU_RsBuf, 8);
        USART_printf(&huart3, rs_temp_buf22);
        USART_printf(&huart3, "\r\n");

    }
    else
    {
        my_status = 0;
        USART_printf(&huart3, "GPRS real ANALOG angale data error\r\n"); //����ʹ��
    }

    if(my_status == 0)
    {
        USART_printf(&huart3, "GPRS MCU ANALOG data error\r\n"); //����ʹ��
    }

    if(my_status == 1) //���ͳɹ��ˣ�����
    {

        USART_printf(&huart3, " Server real ANALOG Cycle data OK2*\r\n");

        // LED2_TOGGLE;
    }

//���������з���״̬����
    MY_MCU_getdata_status = 0; //PW�����������ݴ������
    MY_433_Call_Status = 0;  //433ģ���������ݴ������

    return my_status;

}

/*
���ܣ�����AT+CSQ�������ź�����

*/

extern uint8_t MY_AT_CSQ_Value;
int my_GPRS_AT_CSQ()
{

    uint8_t rs_temp_buf22[512] = {0}; //����ʹ��
    uint8_t my_status = 0;
    uint8_t my_rxbuf[256] = "\x10\x80\x01\x00\x81\x16";
    uint8_t my_txbuf[256] = TX_GPRS_101_testdata;


//----------------

    if(GPRS_Status == 1)
        my_status = 1;
    else
        my_status = 0;

    if(my_status == 1)
    {
        wdz_GPRS_string_to_array(TX_GPRS_101_testdata, my_txbuf); //����ָ��
        my_status = WDZ_GPRS_101transmint_commd_wait_commd(2, my_txbuf, 1, my_rxbuf);
    }



    //���ͻ�������
    if(my_status == 1)
    {
        //�������ݰ�
        //68 53 53 68 53 01 00 09 98 14 00 01 00 00 41 00 00 00 00 00 00 00 00 00 00 4B 16
        my_gprs_generate_101CSQ_data(1, my_txbuf);
        //my_at_senddata(my_txbuf);
        //����
        //my_status=WDZ_GPRS_101receive_testactive(0x80,00,00,1500);
        my_status = my_status = WDZ_GPRS_101transmint_commd_wait_commd(2, my_txbuf, 1, my_rxbuf);

        //����ʹ�ã���������@@@@@@@
        USART_printf(&huart3, "GRPS CSQ:");
        HexToStr2(rs_temp_buf22, my_txbuf, 18);
        USART_printf(&huart3, rs_temp_buf22);
        USART_printf(&huart3, "\r\n");

    }
    else
    {
        my_status = 0;
        USART_printf(&huart3, "GPRS GRPS CSQ error1\r\n"); //����ʹ��
    }

    if(my_status == 0)
    {
        USART_printf(&huart3, "GPRS GRPS CSQ error2\r\n"); //����ʹ��
    }

    if(my_status == 1) //���ͳɹ��ˣ�����
    {

        USART_printf(&huart3, " GRPS CSQ OK1*\r\n");

        // LED2_TOGGLE;
    }

//���������з���״̬����

    return my_status;

}

/*
���ܣ�����GPRS�ź�����
*/
void my_gprs_generate_101CSQ_data(uint8_t temp, uint8_t *my_rsbuf)
{
    uint8_t length = 0;

    if(temp == 1) //�������ݰ�
    {
        length = 1;

        my_rsbuf[0] = 0x68;
        my_rsbuf[3] = 0x68;
        my_rsbuf[1] = length * 2 + 10;
        my_rsbuf[2] = length * 2 + 10;

        my_rsbuf[4] = 0x73; //��������Ϊ53/73
        my_GPRS_101_geneate_FCBword(my_rsbuf);


        my_rsbuf[5] = DTU_ADDRESS & 0X00FF;
        my_rsbuf[6] = (DTU_ADDRESS >> 8) & 0X00FF;

        my_rsbuf[7] = 0X09; //���ͱ�ʶ����ʱ��ĵ�����Ϣ��
        my_rsbuf[8] = length + 0x80; //��Ϣ�����
        my_rsbuf[9] = 0x14; //����ԭ��

        my_rsbuf[10] = DTU_ADDRESS & 0X00FF; //�������ַ
        my_rsbuf[11] = (DTU_ADDRESS >> 8) & 0X00FF;;

        my_rsbuf[12] = 0x00; //ң����Ϣ���׵�ַ
        my_rsbuf[13] = 0x42;

        //my_buf1_to_buf2(MY_GPRS_MCU_RsBuf,0,my_rsbuf,14,length*2);

        my_rsbuf[14] = MY_AT_CSQ_Value;
        my_rsbuf[15] = 0X00;

        //
        my_rsbuf[13 + length * 2 + 1] = 0XFF;
        my_rsbuf[13 + length * 2 + 1 + 1] = 0x16;

        wdz_GPRS_101check_generate(my_rsbuf); //����У���ֽ�



    }
    else if(temp == 0) //����0���������ݰ�
    {
        length = 2;
        my_rsbuf[0] = 0x68;
        my_rsbuf[3] = 0x68;
        my_rsbuf[1] = length + 10;
        my_rsbuf[2] = length + 10;

        my_rsbuf[4] = 0x73; //��������Ϊ53/73
        my_GPRS_101_geneate_FCBword(my_rsbuf);

        my_rsbuf[5] = DTU_ADDRESS & 0X00FF;
        my_rsbuf[6] = (DTU_ADDRESS >> 8) & 0X00FF;

        my_rsbuf[7] = 0X09; //���ͱ�ʶ����ʱ��ĵ�����Ϣ��
        my_rsbuf[8] = 0x84; //��Ϣ�����
        my_rsbuf[9] = 0x14; //����ԭ��

        my_rsbuf[10] = DTU_ADDRESS & 0X00FF; //�������ַ
        my_rsbuf[11] = (DTU_ADDRESS >> 8) & 0X00FF;;

        my_rsbuf[12] = 0x00; //ң����Ϣ���׵�ַ
        my_rsbuf[13] = 0x42;

        my_rsbuf[14] = 0x00;
        my_rsbuf[15] = 0x00;

        my_rsbuf[16] = 0XFF;
        my_rsbuf[17] = 0x16;

        wdz_GPRS_101check_generate(my_rsbuf); //����У���ֽ�
    }


}

/*
���ܣ�ң�ⲹ��
*/
void my_gprs_generate_101yaoce2_data(uint8_t *my_rsbuf)
{
    uint8_t length = 12; //��Ϣ�����
    uint8_t jj = 0;
    //֡ͷ
    my_rsbuf[0] = 0x68;
    my_rsbuf[3] = 0x68;
    my_rsbuf[1] = length * 2 + 10;
    my_rsbuf[2] = length * 2 + 10;
    //�����򲿷�
    my_rsbuf[4] = 0x73; //��������Ϊ53/73
    //my_GPRS_101_geneate_FCBword(my_rsbuf);
    my_rsbuf[5] = DTU_ADDRESS;
    my_rsbuf[6] = (DTU_ADDRESS >> 8);
    my_rsbuf[7] = 0X09; //���ͱ�ʶ����ʱ��ĵ�����Ϣ��
    my_rsbuf[8] = length + 0x80; //��Ϣ�����
    my_rsbuf[9] = 0x67; //����ԭ��
    my_rsbuf[10] = DTU_ADDRESS; //�������ַ
    my_rsbuf[11] = (DTU_ADDRESS >> 8);;
    my_rsbuf[12] = 0x01; //ң����Ϣ���׵�ַ
    my_rsbuf[13] = 0x42;
    //֡β
    my_rsbuf[13 + length * 2 + 1] = 0XFF;
    my_rsbuf[13 + length * 2 + 1 + 1] = 0x16;
    //���ݲ���
    for(jj = 0; jj < 3; jj++)
    {   //1�¶ȣ�2��Դ��3�ο���ѹ��4�ɵ�أ�5���ϵ�ѹ��6̫���ܣ�7﮵��
        my_rsbuf[14 + 8 * jj] = my_indicator_data[jj].DC_data_buf[5 * 2]; //6̫����
        my_rsbuf[15 + 8 * jj] = my_indicator_data[jj].DC_data_buf[5 * 2 + 1];

        my_rsbuf[16 + 8 * jj] = my_indicator_data[jj].DC_data_buf[4 * 2]; //5���ϵ�ѹ
        my_rsbuf[17 + 8 * jj] = my_indicator_data[jj].DC_data_buf[4 * 2 + 1];


        my_rsbuf[18 + 8 * jj] = my_indicator_data[jj].DC_data_buf[3 * 2]; //4�ɵ��
        my_rsbuf[19 + 8 * jj] = my_indicator_data[jj].DC_data_buf[3 * 2 + 1];


        my_rsbuf[20 + 8 * jj] = my_indicator_data[jj].AC_data_buf[4]; //�벨����ֵ
        my_rsbuf[21 + 8 * jj] = my_indicator_data[jj].AC_data_buf[5];

    }

    wdz_GPRS_101check_generate(my_rsbuf); //����У���ֽ�

}

/*
���ܣ�ң��12T
*/
void my_gprs_generate_101yaoce12T_data(uint8_t *my_rsbuf)
{
    uint8_t length = 3; //��Ϣ�����
    uint8_t jj = 0, ii = 0;
    //֡ͷ
    my_rsbuf[0] = 0x68;
    my_rsbuf[3] = 0x68;
    my_rsbuf[1] = length * 48 + 10;
    my_rsbuf[2] = length * 48 + 10;
    //�����򲿷�
    my_rsbuf[4] = 0x73; //��������Ϊ53/73
    //my_GPRS_101_geneate_FCBword(my_rsbuf);
    my_rsbuf[5] = DTU_ADDRESS;
    my_rsbuf[6] = (DTU_ADDRESS >> 8);
    my_rsbuf[7] = 0X09; //���ͱ�ʶ����ʱ��ĵ�����Ϣ��
    my_rsbuf[8] = length + 0x80; //��Ϣ�����
    my_rsbuf[9] = 0x68; //����ԭ��
    my_rsbuf[10] = DTU_ADDRESS; //�������ַ
    my_rsbuf[11] = (DTU_ADDRESS >> 8);;
    my_rsbuf[12] = 0x01; //ң����Ϣ���׵�ַ
    my_rsbuf[13] = 0x43;
    //֡β
    my_rsbuf[13 + length * 2 + 1] = 0XFF;
    my_rsbuf[13 + length * 2 + 1 + 1] = 0x16;
    //���ݲ���
    for(jj = 0; jj < 3; jj++)
    {   //12T ����+�糡
        for(ii = 0; ii < 24; ii++)
        {
            my_rsbuf[14 + 48 * jj + ii] = my_indicator_data[jj].AC12T_ALL_Current_data_buf[ii ]; //����
            
        }
				for(ii = 0; ii < 24; ii++)
        {
            my_rsbuf[14 + 48 * jj + ii +24] = my_indicator_data[jj].AC12T_ALL_dianchang_data_buf[ii]; //�糡

        }

    }

    wdz_GPRS_101check_generate(my_rsbuf); //����У���ֽ�

}
/*
���ܣ�����ͬ��ֵ
*/
extern uint16_t my_gprs_count_time; //GPRSͨ�ţ��������ݣ����ݸ�SERVER��DTU�յ���zsq�ļ���ֵ
extern uint8_t  my_gprs_RTC_buf[];

void my_gprs_generate_101yaoce1_COUNTSYN_data(uint8_t *my_rsbuf)
{
    uint8_t length = 1; //��Ϣ�����

    //֡ͷ
    my_rsbuf[0] = 0x68;
    my_rsbuf[3] = 0x68;
    my_rsbuf[1] = length * 9 + 10;
    my_rsbuf[2] = length * 9 + 10;
    //�����򲿷�
    my_rsbuf[4] = 0x73; //��������Ϊ53/73
    //my_GPRS_101_geneate_FCBword(my_rsbuf);
    my_rsbuf[5] = DTU_ADDRESS;
    my_rsbuf[6] = (DTU_ADDRESS >> 8);
    my_rsbuf[7] = 0XDC; //���ͱ�ʶ����ʱ��ĵ�����Ϣ��
    my_rsbuf[8] = length + 0x80; //��Ϣ�����
    my_rsbuf[9] = 0x69; //����ԭ��
    my_rsbuf[10] = DTU_ADDRESS; //�������ַ
    my_rsbuf[11] = (DTU_ADDRESS >> 8);;
    my_rsbuf[12] = 0x01; //ң����Ϣ���׵�ַ
    my_rsbuf[13] = 0x4F;
    //֡β
    my_rsbuf[13 + length * 9 + 1] = 0XFF;
    my_rsbuf[13 + length * 9 + 1 + 1] = 0x16;
    //���ݲ���


    my_rsbuf[14] = my_gprs_count_time;
    my_rsbuf[15] = (my_gprs_count_time>>8);

    my_rsbuf[16] = my_gprs_RTC_buf[0];
    my_rsbuf[17] = my_gprs_RTC_buf[1];
    my_rsbuf[18] = my_gprs_RTC_buf[2];
    my_rsbuf[19] = my_gprs_RTC_buf[3];
    my_rsbuf[20] = my_gprs_RTC_buf[4];
    my_rsbuf[21] = my_gprs_RTC_buf[5];
    my_rsbuf[22] = my_gprs_RTC_buf[6];




    wdz_GPRS_101check_generate(my_rsbuf); //����У���ֽ�

}


/*
���ܣ�������������ʹ�õ�12���ַ�
*/
extern uint8_t my_indicator_tx_index;

extern struct indicator_alarm_class my_indicator_alarm_data[];
void my_fun_gprs_generate_12T_data(uint8_t *txbuf)
{
	uint8_t ii=0,jj=0;
	
	wdz_GPRS_string_to_array(TX_GPRS_101_ALarm_single_12T_data, my_usart1_tx_buf1);
	for(ii=0;ii<3;ii++) //�������12T����
	{
		if(ii==my_indicator_tx_index)
		{
			for(jj=0;jj<24;jj++)
			{
				my_usart1_tx_buf1[14+ii*48+jj]=my_indicator_alarm_data[ii].AC12T_ALL_Current_data_buf[jj];		//����	
			}
				for(jj=0;jj<24;jj++)
			{
				my_usart1_tx_buf1[14+ii*48+jj+24]=my_indicator_alarm_data[ii].AC12T_ALL_dianchang_data_buf[jj];	//�糡
			}
		}	
		else //���������������������
		{
			for(jj=0;jj<24;jj++)
			{
				my_usart1_tx_buf1[14+ii*48+jj]=my_indicator_data[ii].AC12T_ALL_Current_data_buf[jj];		//����	
			}
				for(jj=0;jj<24;jj++)
			{
				my_usart1_tx_buf1[14+ii*48+jj+24]=my_indicator_data[ii].AC12T_ALL_dianchang_data_buf[jj];			//�糡
			}
			
			
			
		}

		
	}
	
	
	
	
	
	
	
}