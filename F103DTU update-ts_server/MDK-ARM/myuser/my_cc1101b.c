#include "my_cc1101.h"
#define RF_reg_count 47
#define my_CC1101_default_channer 0x02    //оƬĬ��CC1101�ŵ�
uint8_t my_CC1101_chip_address=0x01;  //оƬ��ַ��Ĭ��Ϊ0x05
uint8_t my_cc1101_dest_address=0xFE;
uint16_t my_CC1101_sync_word=0x0102;  //CC1101ͬ���֣�Ĭ��Ϊ0x8799
uint8_t my_CC1101_change_channer=my_CC1101_default_channer;  //��̬�ŵ���



//10, 7, 5, 0, -5, -10, -15, -20, dbm output power, 0x12 == -30dbm
INT8U PaTabel[] = { 0xc0, 0xC8, 0x84, 0x60, 0x68, 0x34, 0x1D, 0x0E};


extern uint8_t MY_CC1101_STATUS;
extern uint8_t Cmd_CC1101_RX[];


static const uint8_t CC1101InitData[RF_reg_count][2]=
{   /*
    {CC1101_IOCFG2,      0x0E},  //
    {CC1101_IOCFG1,      0x2E},
    {CC1101_IOCFG0,      0x07},  //
    {CC1101_FIFOTHR,     0x47},  //Ĭ��47���������
    {CC1101_PKTCTRL1,    0x06},  //06
    {CC1101_PKTCTRL0,    0x05},  //
    {CC1101_CHANNR,      0x01},  //
    {CC1101_FSCTRL1,     0x06},  //
    {CC1101_FREQ2,       0x0F},
    {CC1101_FREQ1,       0x62},
    {CC1101_FREQ0,       0x76},
    {CC1101_MDMCFG4,     0xF6},
    {CC1101_MDMCFG3,     0x43},
    {CC1101_MDMCFG2,     0x13},
    {CC1101_DEVIATN,     0x15},
    {CC1101_MCSM0,       0x18},  //
    {CC1101_FOCCFG,      0x16},
    {CC1101_WORCTRL,     0xFB},  //
    {CC1101_FSCAL3,      0xE9},
    {CC1101_FSCAL2,      0x2A},
    {CC1101_FSCAL1,      0x00},
    {CC1101_FSCAL0,      0x1F},
    {CC1101_TEST2,       0x81},
    {CC1101_TEST1,       0x35},
    {CC1101_MCSM1,       0x3B},//============
    */
    {CC1101_IOCFG2,      0x0E},
    {CC1101_IOCFG1,      0x2E},
    {CC1101_IOCFG0,      0x06},  //0X06��������ʱΪ0��������λ�ߵ�ƽ��0X46��������Ϊ�ߵ�ƽ��������λ�͵�ƽ
    {CC1101_FIFOTHR,     0x4E},
    {CC1101_SYNC1,       0x87},
    {CC1101_SYNC0,       0x99},
    {CC1101_PKTLEN,      0xFF},
    {CC1101_PKTCTRL1,    0x06},  //0x06������ַУ�飬04����������ַУ��
    {CC1101_PKTCTRL0,    0x05},  //05 0000 0101
    {CC1101_ADDR,        0x05},
    {CC1101_CHANNR,      my_CC1101_default_channer},//�ŵ�
    {CC1101_FSCTRL1,     0x06},
    {CC1101_FSCTRL0,     0x00},
    {CC1101_FREQ2,       0x0F},
    {CC1101_FREQ1,       0x62},
    {CC1101_FREQ0,       0x76},
    {CC1101_MDMCFG4,     0xF6},
    {CC1101_MDMCFG3,     0x83},
    {CC1101_MDMCFG2,     0x13},  //Ĭ��Ϊ0X13
    {CC1101_MDMCFG1,     0x22},
    {CC1101_MDMCFG0,     0xF8},
    {CC1101_DEVIATN,     0x15},
    {CC1101_MCSM2,       0x07},
    {CC1101_MCSM1,       0x30},
    {CC1101_MCSM0,       0x18},
    {CC1101_FOCCFG,      0x16},
    {CC1101_BSCFG,       0x6C},
    {CC1101_AGCCTRL2,    0x03},
    {CC1101_AGCCTRL1,    0x40},
    {CC1101_AGCCTRL0,    0x91},
    {CC1101_WOREVT1,     0x87},
    {CC1101_WOREVT0,     0x6B},
    {CC1101_WORCTRL,     0xFB},
    {CC1101_FREND1,      0x56},
    {CC1101_FREND0,      0x10},
    {CC1101_FSCAL3,      0xE9},
    {CC1101_FSCAL2,      0x2A},
    {CC1101_FSCAL1,      0x00},
    {CC1101_FSCAL0,      0x1F},
    {CC1101_RCCTRL1,     0x41},
    {CC1101_RCCTRL0,     0x00},
    {CC1101_FSTEST,      0x59},
    {CC1101_PTEST,       0x7F},
    {CC1101_AGCTEST,     0x3F},
    {CC1101_TEST2,       0x81},
    {CC1101_TEST1,       0x35},
    {CC1101_TEST0,       0x0B},
};


// ������ر���
INT8U   COM_TxNeed = 0;
INT8U   COM_TimeOut = 0;
INT8U   COM_RxCounter = 0;
INT8U   COM_TxCounter = 0;
INT8U   COM_RxBuffer[65] = {0};
INT8U   COM_TxBuffer[65] = {0};

/*===========================================================================
* ���� ��RF_RecvHandler() => �������ݽ��մ���                               *
============================================================================*/
void RF_RecvHandler(void)
{
    INT8U length=0, recv_buffer[64]= {0};
    uint8_t i=0;

    //__HAL_GPIO_EXTI_CLEAR_IT(PIN_CC_IRQ); //�����������ⲿ�жϣ�ÿ���жϵ������������һ������Ҫ�ֱ����
    // HAL_NVIC_EnableIRQ(EXIT_CC_IRQ_GD0);
    CC1101SetTRMode(RX_MODE);           // �������ģʽ



    if (1 == CC_IRQ_READ())             // �������ģ���Ƿ���������ж�
    {
        while (CC_IRQ_READ() == 1);

        // ��ȡ���յ������ݳ��Ⱥ���������
        length = CC1101RecPacket(recv_buffer);

        // �жϽ��������Ƿ���ȷ
        if (length <= SEND_MAX && length>0)
        {
            LED3_ON;               // LED��˸������ָʾ�յ�����
            for(i=0; i<length; i++)
            {
                printf("[%X]--",recv_buffer[i]);
            }
            printf("\n");
            //printf("%s\n",recv_buffer);
            LED3_OFF;

        }
    }
}

/*===========================================================================
* ���� : BSP_RF_SendPacket() => ���߷������ݺ��������ڽ������ݣ�ת����CC1101                        *
============================================================================*/
void RF_SendPacket(void)
{
    INT8U i=0,j=0, length=0, buffer[65]= {0};

    if ((0==COM_TimeOut) && (COM_RxCounter>0) )
    {
        if(MY_CC1101_STATUS==0x01 || MY_CC1101_STATUS== 0x02) return;
        else MY_CC1101_STATUS=0X01;

        //HAL_NVIC_DisableIRQ(EXIT_CC_IRQ_GD0); //�ر��жϣ�����Ӧ�����ж�
        CC1101SetIdle();

        //LED3_ON;

        length = COM_RxCounter;

        COM_RxCounter = 0;

        for (i=0; i<length; i++)   {
            buffer[i] = COM_RxBuffer[i];
        }

        //CC1101SendPacket(buffer, length, ADDRESS_CHECK);    // �������ݣ�Ĭ�ϵ���ַ���ͷ�ʽ
        for(j=0; j<=0x6; j++)			 //ע�⣬Ƶ�ʵļ��㷽������Ƶ+ͨ����*�������������Ҫ���ǣ��п��ܳ���CC1101��Ƶ�ʷ�Χ������CC1101����
        {
            my_CC1101_change_channer=j;
            CC1101WriteReg(CC1101_CHANNR,my_CC1101_change_channer); //�任�ŵ�
            buffer[0]=j+0X30;  //����ʹ�ã���Ϊ�ŵ��ŵı��


            for(i=0xFA; i<0XFF; i++) //���ַ���ͣ�ע��0��ַ�ǹ㲥��ַ,�任��ַ����
            {
                buffer[length]='-';
                buffer[length+1]=i-0XFA+0X30;
                buffer[length+2]='\n';
                CC1101SendPacket_add(buffer, length+3, ADDRESS_CHECK,i);
            }
        }

        //	HAL_NVIC_ClearPendingIRQ(EXIT_CC_IRQ_GD0); //�����ʷ�ж�������Ϣ EXIT_CC_IRQ_GD0������������ã���֪��Ϊʲô


        // __HAL_GPIO_EXTI_CLEAR_IT(PIN_CC_IRQ); //�����������ⲿ�жϣ�ÿ���жϵ������������һ������Ҫ�ֱ����
        //	HAL_NVIC_EnableIRQ(EXIT_CC_IRQ_GD0);


        // CC1101SetTRMode(RX_MODE);                           // �������ģʽ

        printf("USART TO CC1101\n");

        MY_CC1101_STATUS=0x00;
    }
}

void RF_SendPacket2(void)
{
    INT8U i=0, length=0, buffer[65]= {0};

    // if ((0==COM_TimeOut) && (COM_RxCounter>0))
    {


        LED3_ON;

        length = COM_RxCounter;

        COM_RxCounter = 0;
        length=5;
        COM_RxBuffer[0]=0x31;
        COM_RxBuffer[1]=0x32;
        COM_RxBuffer[2]=0x33;
        COM_RxBuffer[3]=0x34;
        COM_RxBuffer[4]=0x35;
        COM_RxBuffer[5]=0x36;

        for (i=0; i<length; i++)   {
            buffer[i] = COM_RxBuffer[i];
        }

        CC1101SendPacket(buffer, length, ADDRESS_CHECK);    // ��������
        CC1101SetTRMode(RX_MODE);                           // �������ģʽ

        HAL_Delay(2500);

        LED3_OFF;
    }
}




/*===========================================================================
* ���� ��RF_Initial() => ��ʼ��RFоƬ                                       *
* ˵�� ��CC1101�Ĳ������Ѿ�������C�⣬��CC1101.c�ļ��� �ṩSPI��CSN������	*
         ���ɵ������ڲ����к����û������ٹ���CC1101�ļĴ����������⡣       *
============================================================================*/
void RF_Initial(void)
{
    CC1101Init();               // ��ʼ��CC1101�Ĵ���
    CC1101SetTRMode(RX_MODE);   // ����ģʽ
}

/*
================================================================================
Function : CC1101RecPacket( )
    Receive a packet
INPUT    : rxBuffer, A buffer store the received data
OUTPUT   : 1:received count, 0:no data
================================================================================
*/
INT8U RS_buf_status[2]= {0}; //�����洢���յ������ݵ�RSSI��crc���

INT8U CC1101RecPacket( INT8U *rxBuffer )
{

    INT8U pktLen;
//    INT16U x;
    //j = 0;

    if ( CC1101GetRXCnt( ) != 0 )
    {
        pktLen = CC1101ReadReg(CC1101_RXFIFO);           // Read length byte
        //printf("rx_count=%d  length=%d \n",	CC1101GetRXCnt(),pktLen); //����ʹ��



        if( ( CC1101ReadReg( CC1101_PKTCTRL1 ) & ~0x03 ) != 0 )
        {
            //  x = CC1101ReadReg(CC1101_RXFIFO);  //����ֽ��ǵ�ַ���������ĵ���ַ��1���ֽ�

        }
        if( pktLen == 0 )           {
            return 0;
        }
        else                      {
            ;
        } // { pktLen --; }
        CC1101ReadMultiReg(CC1101_RXFIFO, rxBuffer, pktLen); // Pull data
        CC1101ReadMultiReg(CC1101_RXFIFO, RS_buf_status, 2);   // Read  status bytes

        CC1101ClrRXBuff( );

        if( RS_buf_status[1] & CRC_OK ) {
            return pktLen;
        }
        else                     {
            return 0;
        }
    }
    else
    {
        return 0;    // Error
    }
}


/*
================================================================================
Function : CC1101SendPacket( )
    Send a packet
INPUT    : txbuffer, The buffer stores data to be sent
           size, How many bytes should be sent
           mode, Broadcast or address check packet
OUTPUT   : None
================================================================================
*/
void CC1101SendPacket( INT8U *txbuffer, INT8U size, TX_DATA_MODE mode )
{
    INT8U address;

    HAL_NVIC_DisableIRQ(EXIT_CC_IRQ_GD0); //�ر��жϣ������������ж�

    if( mode == BROADCAST )             {
        address = 0;
    }
    else if( mode == ADDRESS_CHECK )    {
        address = CC1101ReadReg( CC1101_ADDR );
    }

    CC1101ClrTXBuff( );

    address=my_cc1101_dest_address; //����ʹ�ã�д����շ���ַ
    if( ( CC1101ReadReg( CC1101_PKTCTRL1 ) & ~0x03 ) != 0 )
    {
        CC1101WriteReg( CC1101_TXFIFO, size + 1 );
        CC1101WriteReg( CC1101_TXFIFO, address );
    }
    else
    {
        CC1101WriteReg( CC1101_TXFIFO, size );
    }

    CC1101WriteMultiReg( CC1101_TXFIFO, txbuffer, size );
    CC1101SetTRMode( TX_MODE );

    while( HAL_GPIO_ReadPin(PORT_CC_IRQ,PIN_CC_IRQ) != 1 );
    while( HAL_GPIO_ReadPin(PORT_CC_IRQ,PIN_CC_IRQ) == 1 );
    //HAL_Delay(1000);



    CC1101ClrTXBuff( );  //�������FIFO
}


/*
================================================================================
Function : CC1101ClrTXBuff( )
    Flush the TX buffer of CC1101
INPUT    : None
OUTPUT   : None
================================================================================
*/
void CC1101ClrTXBuff( void )
{
    CC1101SetIdle();//MUST BE IDLE MODE
    CC1101WriteCmd( CC1101_SFTX );
}

/*
================================================================================
Function : CC1101SetIdle( )
    Set the CC1101 into IDLE mode
INPUT    : None
OUTPUT   : None
================================================================================
*/
void CC1101SetIdle( void )
{
    CC1101WriteCmd(CC1101_SIDLE);
}

/*
================================================================================
Function : CC1101ClrRXBuff( )
    Flush the RX buffer of CC1101
INPUT    : None
OUTPUT   : None
================================================================================
*/
void CC1101ClrRXBuff( void )
{
    CC1101SetIdle();//MUST BE IDLE MODE
    CC1101WriteCmd( CC1101_SFRX );
}

/*
================================================================================
Function : CC1101ReadMultiReg( )
    Read some bytes from the rigisters continously
INPUT    : addr, The address of the register
           buff, The buffer stores the data
           size, How many bytes should be read
OUTPUT   : None
================================================================================
*/
void CC1101ReadMultiReg( INT8U addr, INT8U *buff, INT8U size )
{
    INT8U i, j;
    CC_CSN_LOW( );
    SPI_ExchangeByte( addr | READ_BURST);
    for( i = 0; i < size; i ++ )
    {
        for( j = 0; j < 20; j ++ );
        *( buff + i ) = SPI_ExchangeByte( 0xFF );
    }
    CC_CSN_HIGH( );
}



/*
================================================================================
Function : CC1101GetRXCnt( )
    Get received count of CC1101
INPUT    : None
OUTPUT   : How many bytes hae been received
================================================================================
*/
INT8U CC1101GetRXCnt( void )
{
    return ( CC1101ReadStatus( CC1101_RXBYTES )  & BYTES_IN_RXFIFO );
}

/*
================================================================================
Function : CC1101SetTRMode( )
    Set the device as TX mode or RX mode
INPUT    : mode selection
OUTPUT   : None
================================================================================
*/
void CC1101SetTRMode( TRMODE mode )
{
    if( mode == TX_MODE )
    {
        CC1101WriteReg(CC1101_IOCFG0,0x06);
        CC1101WriteCmd( CC1101_STX );
    }
    else if( mode == RX_MODE )
    {
        CC1101WriteReg(CC1101_IOCFG0,0x06);
        CC1101WriteCmd( CC1101_SRX );
    }
}



/*
================================================================================
Function : CC1101Init( )
    Initialize the CC1101, User can modify it
INPUT    : None
OUTPUT   : None
================================================================================
*/
void CC1101Init( void )
{
    volatile uint8_t i, j;

    CC1101Reset( );

    for( i = 0; i < RF_reg_count; i++ )  //д��23���Ĵ��� RF_reg_count
    {
        CC1101WriteReg( CC1101InitData[i][0], CC1101InitData[i][1] );
    }


    CC1101SetAddress( my_CC1101_chip_address, BROAD_0AND255 );
    CC1101SetSYNC( my_CC1101_sync_word );//0X8799
    CC1101WriteReg(CC1101_MDMCFG1,   0x72); //Modem Configuration

    CC1101WriteMultiReg(CC1101_PATABLE, PaTabel, 8 );

    i = CC1101ReadStatus( CC1101_PARTNUM );//for test, must be 0x80
    printf("PARTNUM=[%X]  ",i);
    i = CC1101ReadStatus( CC1101_VERSION );//for test, refer to the datasheet
    printf("VERSION=[%X]  \n",i);
}

/*
================================================================================
Function : CC1101ReadStatus( )
    Read a status register
INPUT    : addr, The address of the register
OUTPUT   : the value read from the status register
================================================================================
*/
INT8U CC1101ReadStatus( INT8U addr )
{
    INT8U i;
    CC_CSN_LOW( );
    SPI_ExchangeByte( addr | READ_BURST);
    i = SPI_ExchangeByte( 0xFF );
    CC_CSN_HIGH( );
    return i;
}



/*
================================================================================
Function : CC1101WriteMultiReg( )
    Write some bytes to the specified register
INPUT    : addr, The address of the register
           buff, a buffer stores the values
           size, How many byte should be written
OUTPUT   : None
================================================================================
*/
void CC1101WriteMultiReg( INT8U addr, INT8U *buff, INT8U size )
{
    INT8U i;
    CC_CSN_LOW( );
    SPI_ExchangeByte( addr | WRITE_BURST );
    for( i = 0; i < size; i ++ )
    {
        SPI_ExchangeByte( *( buff + i ) );
    }
    CC_CSN_HIGH( );
}




/*
================================================================================
Function : CC1101SetSYNC( )
    Set the SYNC bytes of the CC1101
INPUT    : sync, 16bit sync
OUTPUT   : None
================================================================================
*/
void CC1101SetSYNC( INT16U sync )
{
    CC1101WriteReg(CC1101_SYNC1, 0xFF & ( sync>>8 ) );
    CC1101WriteReg(CC1101_SYNC0, 0xFF & sync );
}


/*
================================================================================
Function : CC1101SetAddress( )
    Set the address and address mode of the CC1101
INPUT    : address, The address byte
           AddressMode, the address check mode
OUTPUT   : None
================================================================================
*/
void CC1101SetAddress( uint8_t address, ADDR_MODE AddressMode)
{
    uint8_t btmp = CC1101ReadReg( CC1101_PKTCTRL1 ) & ~0x03;
    CC1101WriteReg(CC1101_ADDR, address);
    if     ( AddressMode == BROAD_ALL )     {}
    else if( AddressMode == BROAD_NO  )     {
        btmp |= 0x01;
    }
    else if( AddressMode == BROAD_0   )     {
        btmp |= 0x02;
    }
    else if( AddressMode == BROAD_0AND255 ) {
        btmp |= 0x03;
    }
}


/*
================================================================================
Function : CC1101ReadReg( )
    read a byte from the specified register
INPUT    : addr, The address of the register
OUTPUT   : the byte read from the rigister
================================================================================
*/
uint8_t CC1101ReadReg( uint8_t addr )
{
    uint8_t i;
    CC_CSN_LOW( );
    SPI_ExchangeByte( addr | READ_SINGLE);
    i = SPI_ExchangeByte( 0xFF );
    CC_CSN_HIGH( );
    return i;
}


/*
================================================================================
Function : CC1101Reset( )
    Reset the CC1101 device
INPUT    : None
OUTPUT   : None
================================================================================
*/
void CC1101Reset( void )
{
    uint8_t x;

    CC_CSN_HIGH( );
    HAL_Delay(50);
    CC_CSN_LOW( );
    HAL_Delay(50);
    CC_CSN_HIGH( );
    for( x = 0; x < 100; x ++ );
    CC1101WriteCmd( CC1101_SRES );
}

void CC_CSN_LOW(void)
{

    HAL_GPIO_WritePin(PORT_CC_CSN,PIN_CC_CSN,GPIO_PIN_RESET);
    while( HAL_GPIO_ReadPin(PORT_CC_CSN,PIN_CC_CSN)!=GPIO_PIN_RESET);

}
void CC_CSN_HIGH( void)
{
    HAL_GPIO_WritePin(PORT_CC_CSN,PIN_CC_CSN,GPIO_PIN_SET);
}

/*
================================================================================
Function : CC1101WriteCmd( )
    Write a command byte to the device
INPUT    : command, the byte you want to write
OUTPUT   : None
================================================================================
*/
void CC1101WriteCmd( uint8_t command )
{   // uint8_t x1=0;
    CC_CSN_LOW( );
    // x1=SPI_ExchangeByte( command );
    SPI_ExchangeByte( command );
    CC_CSN_HIGH( );
    //printf("writeCmd -[%X]-read:[%X]\n",command,x1);
}

/*
================================================================================
Function : CC1101WriteReg( )
    Write a byte to the specified register
INPUT    : addr, The address of the register
           value, the byte you want to write
OUTPUT   : None
================================================================================
*/
void CC1101WriteReg( uint8_t addr, uint8_t value )
{
//		uint8_t x1=0,x2=0;
    CC_CSN_LOW( );
    SPI_ExchangeByte( addr );
    SPI_ExchangeByte( value );
    CC_CSN_HIGH( );
    //printf("write reg read data:[%X]-[%X]\n",x1,x2);

}

/*
================================================================================
Function : CC1101WORInit( )
    Initialize the WOR function of CC1101
INPUT    : None
OUTPUT   : None
================================================================================
*/
void  CC1101WORInit( void )
{
    CC1101WriteReg(CC1101_MCSM2,0x00);  //0x16
    CC1101WriteReg(CC1101_MCSM0,0x18);  //0X18
    CC1101WriteReg(CC1101_WOREVT1,0x8C);  //0x1E
    CC1101WriteReg(CC1101_WOREVT0,0xA0);  //0x1f
    CC1101WriteReg(CC1101_WORCTRL,0x78); //0X20   Wake On Radio Control

    CC1101WriteCmd( CC1101_SWORRST );
}

//------------�Զ���---------
void RF_RecvHandler_intrrupt(void)
{
    INT8U length=0, recv_buffer[64]= {0};
    uint8_t i=0;
    int RSSI_dBm=0;

    if(MY_CC1101_STATUS==0x01 || MY_CC1101_STATUS== 0x02) return;
    else MY_CC1101_STATUS=0X02;


    //__HAL_GPIO_EXTI_CLEAR_IT(PIN_CC_IRQ); //�����������ⲿ�жϣ�ÿ���жϵ������������һ������Ҫ�ֱ����
    //HAL_NVIC_EnableIRQ(EXIT_CC_IRQ_GD0);
    // if (1 == CC_IRQ_READ())             // �������ģ���Ƿ���������ж�
    {
        while (CC_IRQ_READ() == 1);

        // ��ȡ���յ������ݳ��Ⱥ���������
        length = CC1101RecPacket(recv_buffer);

        // �жϽ��������Ƿ���ȷ
        if (length <= SEND_MAX && length>0)
        {
            LED3_ON;               // LED��˸������ָʾ�յ�����

            printf("RX-data:");
            for(i=0; i<length; i++)
            {
                Cmd_CC1101_RX[i]=recv_buffer[i];
                printf("[%X]--",recv_buffer[i]);
            }
            //printf("\n");
            // printf("%s\n",recv_buffer);

            RSSI_dBm=RS_buf_status[0];
            if(RSSI_dBm>=128) RSSI_dBm=(RSSI_dBm-256)/2-75;
            else RSSI_dBm=(RSSI_dBm)/2-75;

            printf("::RX_BUF_RSSI:[%X]-[%d]\n",RS_buf_status[0],RSSI_dBm);

            LED3_OFF;

        }
    }

    CC1101SetTRMode(RX_MODE);           // �������ģʽ
    MY_CC1101_STATUS=0X00;
    //CC1101WriteCmd(CC1101_SWOR); //����WORģʽ
}

void my_read_all_reg(void)
{
    int i=0;
    uint8_t read_buf[RF_reg_count]= {0};
    for(i=0; i<RF_reg_count; i++)
    {
        read_buf[i]=CC1101ReadReg( CC1101InitData[i][0]);

        // CC1101WriteReg( CC1101InitData[i][0], CC1101InitData[i][1] );

    }

    for(i=0; i<RF_reg_count; i++)
    {
        printf("address-[%2X] write-[%2X] read-[%2X]\n",CC1101InitData[i][0], CC1101InitData[i][1],read_buf[i]);

    }


}


/*
================================================================================
Function : CC1101SendPacket_add( )  ��Ŀ���ַ�ķ��ͺ���
    Send a packet
INPUT    : txbuffer, The buffer stores data to be sent
           size, How many bytes should be sent
           mode, Broadcast or address check packet
OUTPUT   : None

����address���Ŀ���ַ��һ���ֽ�
================================================================================
*/
void CC1101SendPacket_add( INT8U *txbuffer, INT8U size, TX_DATA_MODE mode,INT8U address)
{
    HAL_NVIC_DisableIRQ(EXIT_CC_IRQ_GD0); //�ر��жϣ������������ж�

    if( mode == BROADCAST )             {
        address = 0;
    }
    //else if( mode == ADDRESS_CHECK )    { address = CC1101ReadReg( CC1101_ADDR ); }

    CC1101ClrTXBuff( );

    //address=my_cc1101_dest_address; //����ʹ�ã�д����շ���ַ
    if( ( CC1101ReadReg( CC1101_PKTCTRL1 ) & ~0x03 ) != 0 )
    {
        CC1101WriteReg( CC1101_TXFIFO, size + 1 ); //1���ֽڳ���+��1���ֽڵĵ�ַ+��Ч����,���ⳤ�ȷ���
        CC1101WriteReg( CC1101_TXFIFO, address );  //��ַ+��Ч���ݣ��̶����ȷ���
    }
    else
    {
        CC1101WriteReg( CC1101_TXFIFO, size ); //���ӵ�ַ
    }

    CC1101WriteMultiReg( CC1101_TXFIFO, txbuffer, size ); //д����Ч����
    CC1101SetTRMode( TX_MODE );  //����ģʽ

    while( HAL_GPIO_ReadPin(PORT_CC_IRQ,PIN_CC_IRQ) != 1 );  //�������ݲ����ߵ�ƽ�ж�
    while( HAL_GPIO_ReadPin(PORT_CC_IRQ,PIN_CC_IRQ) == 1 );  //�ߵ�ƽ��Ϊ�͵�ƽ

    CC1101ClrTXBuff( );  //�������FIFO
    CC1101SetTRMode(RX_MODE);  // �������ģʽ

    // __HAL_GPIO_EXTI_CLEAR_IT(PIN_CC_IRQ); //�����������ⲿ�жϣ�ÿ���жϵ������������һ������Ҫ�ֱ����
    //HAL_NVIC_EnableIRQ(EXIT_CC_IRQ_GD0); //---------���������ж�

    //CC1101SetTRMode(RX_MODE);  // �������ģʽ


}
void RF_SendPacket3(void)  //����ʹ�ã�����FIFO�����ʹ��
{
    INT8U i=0,length=0, buffer[65]= {0};

    if ((0==COM_TimeOut) && (COM_RxCounter>0) )
    {
        if(MY_CC1101_STATUS==0x01 || MY_CC1101_STATUS== 0x02) return;
        else MY_CC1101_STATUS=0X01;

        //HAL_NVIC_DisableIRQ(EXIT_CC_IRQ_GD0); //�ر��жϣ�����Ӧ�����ж�
        CC1101SetIdle();

        //LED3_ON;

        length = COM_RxCounter;

        COM_RxCounter = 0;

        for (i=0; i<length; i++)   {
            buffer[i] = COM_RxBuffer[i];
        }

        CC1101SendPacket(buffer, length, ADDRESS_CHECK);    // �������ݣ�Ĭ�ϵ���ַ���ͷ�ʽ




        // __HAL_GPIO_EXTI_CLEAR_IT(PIN_CC_IRQ); //�����������ⲿ�жϣ�ÿ���жϵ������������һ������Ҫ�ֱ����
        //	HAL_NVIC_EnableIRQ(EXIT_CC_IRQ_GD0);


        // CC1101SetTRMode(RX_MODE);                           // �������ģʽ

        printf("USART SINGLE TO CC1101\n");

        MY_CC1101_STATUS=0x00;
    }
}