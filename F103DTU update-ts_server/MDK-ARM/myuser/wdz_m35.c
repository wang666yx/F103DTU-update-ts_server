#include "wdz_m35.h"
#include "string.h"
#include "cmsis_os.h"
#include "math.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim6;


extern uint8_t rsbuf1[];	  //USART1缓冲器
extern uint8_t rsbuf2[];   //USART2缓冲器
extern uint16_t rsbuf1pt_write;
extern uint16_t rsbuf1pt_read;
//extern uint16_t rsbuf1pt_TEMP_read;
extern uint16_t rsbuf2pt_write;
extern uint16_t rsbuf2pt_read;
//extern uint16_t rsbuf2pt_TEMP_read;

extern  uint32_t TimingDelay;
extern  uint8_t GPRS_Heartdata_error_count;

extern uint8_t AT_MESS_telphonenumber[7][16];
extern uint8_t AT_MESS_telphonenumber2[3][16];
extern uint16_t DTU_ADDRESS;
extern uint16_t Transmint_to_GPRS_hearttime;
extern uint16_t	Transmint_to_GPRS_Cycledata;


extern uint8_t MY_IP[4];
extern uint16_t MY_PORT;

uint8_t GPRS_Status=0;  //标识最终手机模块，GPRS网络状态，1为正常，可以连接服务器，0为有问题，需要处理
uint8_t MESS_Status=0;  //短信网络状态
uint8_t NET_Status=0;  //NET联网状态
uint8_t NET_Server_status=0; //远端服务器server状态



/*
功能;M35初始化程序，建立连接
*/
void my_init_m35()
{
    uint8_t *AT_COMM;
    uint8_t *AT_ANSWER;
    char IP0[4]= {0}; //用来存储10禁止IP地址，转换成的字符串，例如222，转出32H,32H,32H
    char IP1[4]= {0};
    char IP2[4]= {0};
    char IP3[4]= {0};

    char PORT[7]= {0};

    char ip_port[45]= {0};

    u8 ii=0;
    uint8_t my_gprs_status=0;




    /*
    	//第一步 软重启 软重启失败后进行硬重启 在软重启，失败开启15分钟定时后退出
        for(ii=0;ii<2;ii++)
    {
        AT_COMM=AT_REST;
    		AT_ANSWER=AN_Call_Ready;
    		my_gprs_status=my_at_com(USART_GPRS,AT_COMM,AT_ANSWER,1,3000);
        if(my_gprs_status==0)
    		{
    			LED2_ON;
    			gprs_status=0;
    			//开启硬件重启
    			 Delay_us(1);

    		}
    	}
    	*/

    //第1步，查询联网状态AT_CPIN,不用软重启命令了，直接断电重启吧。，断电重启大概时间15秒左右
    for(ii=0; ii<2; ii++)
    {
        AT_COMM=AT_CPIN;
        AT_ANSWER=AN_OK;
        my_gprs_status=my_at_com(USART_GPRS,AT_COMM,AT_ANSWER,3,1000);
        if(my_gprs_status==0)
        {
            //LED2_ON;
            GPRS_Status=0;
            //开启硬件重启
            //Delay_us(1);
        }
        else
        {
            break;
        }
    }

    //第二步 发AT测试命令
    if(my_gprs_status==1)
    {   AT_COMM=AT;
        AT_ANSWER=AN_OK;
        my_gprs_status=my_at_com(USART_GPRS,AT_COMM,AT_ANSWER,10,1000);
    }



    //第三步  //设置参数
    if(my_gprs_status==1)
    {
        //Delay_us(100);
        AT_COMM=AT_V1;
        AT_ANSWER=AN_OK;
        my_gprs_status=my_at_com(USART_GPRS,AT_COMM,AT_ANSWER,1,1000);//1秒钟1次指令
        //Delay_us(100);

        AT_COMM=AT_E1;
        my_gprs_status=my_at_com(USART_GPRS,AT_COMM,AT_ANSWER,1,1000);
        //Delay_us(100);
        AT_COMM=AT_CEMEE;
        my_gprs_status=my_at_com(USART_GPRS,AT_COMM,AT_ANSWER,1,1000);
        //Delay_us(100);
        AT_COMM=AT_IPR;
        my_gprs_status=my_at_com(USART_GPRS,AT_COMM,AT_ANSWER,1,1000);
        //Delay_us(100);
        AT_COMM=AT_W;
        my_gprs_status=my_at_com(USART_GPRS,AT_COMM,AT_ANSWER,1,1000);
    }

    //第4步，GPRS联网检查
    if(my_gprs_status==1)
    {
        AT_COMM=AT_CPIN;
        AT_ANSWER=AN_OK;
        my_gprs_status=my_at_com(USART_GPRS,AT_COMM,AT_ANSWER,3,1000);
    }
    //--------短信部分--------
    if(my_gprs_status==1)
    {
        AT_COMM=AT_CSMP;
        AT_ANSWER=AN_OK;
        //my_gprs_status=my_at_com(USART_GPRS,AT_COMM,AT_ANSWER,3,500);
        my_at_com(USART_GPRS,AT_COMM,AT_ANSWER,3,500);
    }
    if(my_gprs_status==1)
    {
        AT_COMM=AT_CPMS_SM;
        AT_ANSWER=AN_OK;
        my_gprs_status=my_at_com(USART_GPRS,AT_COMM,AT_ANSWER,3,1000);
    }
    if(my_gprs_status==1)
    {
        AT_COMM=AT_CMGF;
        AT_ANSWER=AN_OK;
        my_gprs_status=my_at_com(USART_GPRS,AT_COMM,AT_ANSWER,3,1000);
    }
    if(my_gprs_status==1)
    {
        AT_COMM=AT_CSCS;
        AT_ANSWER=AN_OK;
        my_gprs_status=my_at_com(USART_GPRS,AT_COMM,AT_ANSWER,3,1000);
    }
    if(my_gprs_status==1)
    {
        AT_COMM=AT_CSDH;
        AT_ANSWER=AN_OK;
        my_gprs_status=my_at_com(USART_GPRS,AT_COMM,AT_ANSWER,3,1000);

        if(my_gprs_status==1)    //短信网络状态标识

        {   MESS_Status=1;
            //my_AT_MESS_dellall(); //测试，删除所有短信
        }
        else
            MESS_Status=0;
    }

    //----------短信部分 结束--------

    //第6步，建立GPRS联网
    //第4步，查询联网状态//设置参数
    if(my_gprs_status==1)
    {
        AT_COMM=AT_CPIN;
        AT_ANSWER=AN_OK;
        my_gprs_status=my_at_com(USART_GPRS,AT_COMM,AT_ANSWER,3,1000);
    }
    if(my_gprs_status==1)
    {
        AT_COMM=AT_QIDEACT;
        AT_ANSWER=AN_OK;
        my_gprs_status=my_at_com(USART_GPRS,AT_COMM,AT_ANSWER,3,1000);
    }
    if(my_gprs_status==1)
    {
        AT_COMM=AT_CREG;
        AT_ANSWER=AN_OK;
        my_gprs_status=my_at_com(USART_GPRS,AT_COMM,AT_ANSWER,3,1000);
    }
    if(my_gprs_status==1)
    {
        AT_COMM=AT_QIMODE;
        AT_ANSWER=AN_OK;
        my_at_com(USART_GPRS,AT_COMM,AT_ANSWER,3,1000);
    }
    if(my_gprs_status==1)
    {
        AT_COMM=AT_QICSGP;
        AT_ANSWER=AN_OK;
        my_gprs_status=my_at_com(USART_GPRS,AT_COMM,AT_ANSWER,3,1000);
    }
    if(my_gprs_status==1)
    {
        AT_COMM=AT_QIREGAPP;
        AT_ANSWER=AN_OK;
        my_gprs_status=my_at_com(USART_GPRS,AT_COMM,AT_ANSWER,3,1000);
    }
    if(my_gprs_status==1)
    {
        AT_COMM=AT_QIACT;
        AT_ANSWER=AN_OK;
        my_gprs_status=my_at_com(USART_GPRS,AT_COMM,AT_ANSWER,3,1000);
        //my_gprs_status=1;  //调试使用，屏蔽掉此指令的停止动作
        if(my_gprs_status==1) NET_Status=1;  //进行net网络状态标识  AT+CSQ,测量信号强度 13,0
        else NET_Status=0;
    }
    if(my_gprs_status==1)
    {
        AT_COMM=AT_QILOCIP;
        AT_ANSWER=AN_OK;
        //my_at_com(USART_GPRS,AT_COMM,AT_ANSWER,1,100);  //查询IP地址，可以不用
    }
    //第7步，建立TCP连接
    if(my_gprs_status==1)
    {
        AT_COMM=AT_QICLOSE;

        AT_ANSWER=AN_CLOSE_OK;
        my_gprs_status=my_at_com(USART_GPRS,AT_COMM,AT_ANSWER,1,1000);

        itoa(MY_IP[0], IP0, 10);
        itoa(MY_IP[1], IP1, 10);
        itoa(MY_IP[2], IP2, 10);
        itoa(MY_IP[3], IP3, 10);
        itoa(MY_PORT, PORT, 10);

        strcat(ip_port,"AT+QIOPEN=\"TCP\",");
        strcat(ip_port,"\"");
        strcat(ip_port,IP0);
        strcat(ip_port,".");
        strcat(ip_port,IP1);
        strcat(ip_port,".");
        strcat(ip_port,IP2);
        strcat(ip_port,".");
        strcat(ip_port,IP3);
        strcat(ip_port,"\",\"");
        strcat(ip_port,PORT);
        strcat(ip_port,"\"\r\n");
        AT_COMM=(uint8_t *)ip_port;

        AT_ANSWER=AN_CNNECT_OK;
        my_gprs_status=my_at_com(USART_GPRS,AT_COMM,AT_ANSWER,3,1000);
    }

    //如果软启动失败，开启硬件启动，和15分钟定时器
    if(my_gprs_status==0)
    {
			 GPRS_Heartdata_error_count++; //记录断开
        LED1_ON; //上数，第一个红色灯,表示GPRS有问题
        GPRS_Status=0;
        NET_Server_status=0;
        // Delay_us(200);
        //开启15分钟定时，15分钟后再次进行初始化
			  
        my_speaker_long_voice(200);
    }
    else
    {
        GPRS_Status=1;
        NET_Server_status=0;
        GPRS_Heartdata_error_count=0;   //心跳包故障清零
        LED1_OFF; //上数第一个红灯，不亮，表示网络正常
        //测试
        printf("====M35 IS OK!\n");
        my_speaker_short_cycle_voice(3,25);

    }

    //2016-5-28  测量信号强度
    AT_GET_CSQ();

    //2016-05-28 结束

}


/*

AT命令的回答监测程序
输入参数：串口号，比较字符串
输出参数：如果回答正确返回1，回答错误返回0，15秒超时，返回0
*/
extern uint16_t my_tim6_count;
uint8_t my_AT_process_usart(uint8_t *Data,uint32_t delay_time)
{
    u32 address_first=USART_GPRS_BUFFERpt_READ;
    u32 i=0;
    char ch=0;
    uint8_t r_int=0;
    u16 j=0;
    uint16_t my_start_tim=0;

    my_start_tim=my_tim6_count;
    while((my_tim6_count-my_start_tim)<delay_time/100&&r_int==0)
    {   j=0;
        while(address_first!=USART_GPRS_BUFFERpt_WRITE)
        {
            ch=USART_GPRS_BUFFER[address_first];
            if(ch==Data[i])
            {   i++;
                address_first++;
            }
            else
            {   i=0;
                address_first++;
            }
            j++;
            if(address_first>=rsbuf_max)address_first=0;
            if(i>0&&Data[i]==0) {
                r_int=1;
                break;
            }
            else r_int=0;

        }
    }

    USART_GPRS_BUFFERpt_READ=address_first;  //缓冲数组读指针进行移动，移动到帧的末尾

    return(r_int);

}

/*
功能：循环发送AT指令n次，每间隔time时间，成功返回1，失败返回0
输入：串口名称，命令at,应答字符串，发送次数，发送时间间隔TIME*10ms
输出：成功1，失败0
*/
uint8_t my_at_com(UART_HandleTypeDef* USARTx,uint8_t *AT_COMM,uint8_t *AT_ANSWER,uint8_t n,uint32_t time)
{
    u16 i=0;
    u8 comm_answer=0;
    uint8_t my_gprs_status=0;
    for(i=0; i<n; i++)
    {

        USART_printf(USARTx,AT_COMM);
        comm_answer=my_AT_process_usart(AT_ANSWER,time);  //1秒钟1次指令
        if(comm_answer==1)
        {
            //USART_printf(&huart3,"%s=OK!!\r\n",AT_COMM);	  //调试使用，正式使用不用了
            my_gprs_status=1;
            break;
        }
        else
        {
            USART_printf(&huart3,"GPRS Send short data error: %s=%d\r\n",AT_COMM,i);
            my_gprs_status=0;

            USART_printf(USARTx,"\x1A");	//调试，强制结束本次对话过程
        }
    }
    return(my_gprs_status);
}





//***********短消息处理部分*************




/*
  功能;读取当前SIM卡电话号码，读取成功返回1，不成功返回0，把读取到的电话号码存入到数组中



AT+CPBS="ON"
[2014-12-23 09:31:59:323_S:] AT+CNUM
[2014-12-23 09:31:59:357_R:] AT+CNUM

[2014-12-23 09:31:59:404_R:] +CNUM: "","8618630674002",145,7,4

[2014-12-23 09:31:59:435_R:] OK

*/
uint8_t AT_Get_SIM_Telephonenumber(uint8_t *pt)
{
    uint8_t *AT_COMM;
    uint8_t *AT_ANSWER;
    uint8_t my_gprs_status=0;
    uint8_t firstaddress=0;
    uint8_t sencondaddress=0;
    uint8_t my_buf[256]= {0};
    uint16_t k=0;
    uint8_t my_status=0;

    AT_COMM=AT_CPBS;
    AT_ANSWER=AN_OK;
    my_gprs_status=my_at_com(USART_GPRS,AT_COMM,AT_ANSWER,3,100);

    AT_COMM=AT_CNUM;
    my_gprs_status=my_at_com(USART_GPRS,AT_COMM,AT_ANSWER,3,100);

    if(my_gprs_status==1)  //读取电话号
    {
        my_gprs_status=AT_Get_Frame(my_buf);

        if(my_gprs_status==1)
        {
            for(k=0; k<255; k++)
            {
                if(my_buf[k]==','&&my_status==0)
                {
                    firstaddress=k+2;
                    my_status=1;
                    k=k+2;

                }
                else if(my_buf[k]=='\"'&&my_status==1)
                {

                    sencondaddress=k;
                    my_status=2;
                    break;
                }

            }

            k=1;//第一个单元格存特殊字符0x01，表示字符串，短信发送时自动加0X1A
            while(firstaddress<sencondaddress)
            {
                pt[k]=my_buf[firstaddress];
                firstaddress++;
                k++;
            }
        }
    }
    //如果，取得的电话号为空号，就是没取到，就返回0，取到返回1
    /*  调试使用
    if(pt[0]!=pt[1])
    {
    	my_gprs_status=1;
    }
    else if(pt[0]==pt[1])
    {
    	my_gprs_status=0;
    }
    */
    return my_gprs_status;

}


/*
功能：对M35返回的结果，取一帧数据，放入到数组中，原理是利用 从后到前 0D 0A 之间的数据判断
参数:存储数据的数组首地址

[2014-12-23 09:31:59:323_S:] AT+CNUM
[2014-12-23 09:31:59:357_R:] AT+CNUM

[2014-12-23 09:31:59:404_R:] +CNUM: "","8618630674002",145,7,4

[2014-12-23 09:31:59:435_R:] OK
*/
uint8_t AT_Get_Frame(uint8_t *pt)
{
    uint16_t secondaddress=USART_GPRS_BUFFERpt_READ;
    uint16_t firstaddress=0;
    uint16_t k=0;
    uint8_t temp8=0;
    uint8_t my_status=0;

    if(secondaddress<8)secondaddress=USART_GPRS_BUFFERpt_READ+rsbuf_max-8;
    else secondaddress=USART_GPRS_BUFFERpt_READ-8;
    firstaddress=secondaddress;

    for(k=0; k<256; k++)
    {
        temp8=USART_GPRS_BUFFER[firstaddress];
        if(temp8==0X0A)
            my_status =1;
        else if(temp8==0X0D&&my_status==1)
            my_status=2;
        else my_status=0;

        if(my_status==2)
            break;

        if(firstaddress==0)
            firstaddress=rsbuf_max-1;
        else
            firstaddress=firstaddress-1;

    }

    //把数据放入到数组中
    if(my_status==2)
    {
        firstaddress=firstaddress+2;
        if(firstaddress>=rsbuf_max)
        {
            firstaddress=firstaddress-rsbuf_max;
        }
        k=0;

        while(firstaddress!=secondaddress)
        {
            pt[k]=USART_GPRS_BUFFER[firstaddress];
            k++;
            firstaddress++;
            if(firstaddress==rsbuf_max)
            {
                firstaddress=0;
            }
        }

        return 1;
    }
    else return 0;



}

/*
uint8_t GPRS_Status=0;  //标识最终手机模块，GPRS网络状态，1为正常，可以连接服务器，0为有问题，需要处理
uint8_t MESS_Status=0;  //短信网络状态
uint8_t NET_Status=0;  //NET联网状态
uint8_t NET_Server_status=0; //远端服务器server状态

功能：检查，重启M35模块程序，重启分为3个层次，
		如果NET_Server_status为0，其它为1，则现在M35已经注册GSM和GPRS网络，但是Server连接不上，重新初始化M35，连接网络
		如果NET_Status为0，则没有注册GPRS网络，重新连接，如果不行，软关机重启，连续3次，如果还不行
		如果还不行，把M35断电，开始计时，2小时后重新上电连接
*/

//uint16_t my_M35_Reset_count=0;
extern uint8_t GPRS_Heartdata_error_count;
void AT_M35_Reset(void)
{


    //if(GPRS_Status==0 && NET_Status==1 && NET_Server_status==0) //M35注册了GPRS网络，但是Server没有在线  MESS_Status==1 &&
    if(GPRS_Heartdata_error_count<(5) && GPRS_Heartdata_error_count>=3)
    {
        my_init_m35();
        USART_printf(&huart3,"M35_rest-%d\n",GPRS_Heartdata_error_count);

    }
    //else if(GPRS_Status==0 && NET_Status==0 && NET_Server_status==0)  //M35没有注册GPRS网络，Server没有在线
    else if(GPRS_Heartdata_error_count>=(5) && GPRS_Heartdata_error_count<(10) )
    {

        M35_PWR_ON;      //高电平持续3秒左右，者关机
        osDelay(3000);
        M35_PWR_OFF;    //给个低电平，正常工作或者关机状态下为低电平
        osDelay(100);
        //Delay_us(300);
        ////M35_PWR_ON;      //高电平持续3秒左右，开机
        //Delay_us(100);
        //M35_PWR_OFF;    //给个低电平，正常工作或者关机状态下为低电平


        my_init_m35();  //初始化操作
        //调试使用
        USART_printf(&huart3,"M35_PWR_CONTROL-%d\n",GPRS_Heartdata_error_count);
    }
    else if(GPRS_Heartdata_error_count>=(10) && GPRS_Heartdata_error_count<(15)) //GPRS_Heartdata_error_count%(12*5)<=2 &&
    {
//        M35_PWREN_CONTROL_OFF; //M35断电状态
//        osDelay(1000);
//        M35_PWREN_CONTROL_ON;  //M35上电，但没有开机
//        osDelay(1000);

//        M35_PWR_ON;      //高电平持续3秒左右，开机
//        osDelay(3000);
//        M35_PWR_OFF;    //给个低电平，正常工作或者关机状态下为低电平

        //=============
        M35_EMERG_OFF;
        M35_PWR_OFF;
        M35_PWREN_CONTROL_OFF; //M35断电状态
        osDelay(1000);
        M35_PWREN_CONTROL_ON;  //M35上电，但没有开机
        M35_PWR_ON;      //高电平持续3秒左右，开机或者关机
        osDelay(3000);
        M35_PWR_OFF;    //给个低电平，正常工作或者关机状态下为低电平


        //==============
        my_init_m35();  //初始化操作
        //调试使用
        USART_printf(&huart3,"M35_PWREN_CONTROL%d\n",GPRS_Heartdata_error_count);
    }

    else if(GPRS_Heartdata_error_count>=(15))
    {
        //TIM3断电，看门狗断电，然后1分钟14秒左右，断电重启主MCU

        //__HAL_RCC_TIM3_CLK_DISABLE();
        //调试使用
        USART_printf(&huart3,"M35_WATCHDOS_OFF-%d\n",GPRS_Heartdata_error_count);
        NVIC_SystemReset();
    }

}
/*
功能：获得101协议中发发送的数据长度, 最长1460
*/
uint16_t get_string_number(uint8_t *string)
{   uint8_t *pt=string;
    uint16_t  re_number=0;
    uint16_t  re_number2=0;

    //单字节长度
    if(*pt==0x10&&*(pt+5)==0x16)
    {
        re_number=6;
    }
    else if(*pt==0x68&&*(pt+3)==0x68)
    {
        re_number=*(pt+1)+6;
    }
    //双字节长度
    else if(*pt==0x10&&*(pt+7)==0x16)
    {
        re_number=8;
    }
    else if(*pt==0x68&&*(pt+5)==0x68)
    {
        re_number=*(pt+2);
        re_number=(re_number<<8)+*(pt+1);

        re_number2=*(pt+4);
        re_number2=(re_number2<<8)+*(pt+3);

        if(re_number==re_number2)
            re_number=re_number+8;
        else
            re_number=0;
    }

    return re_number;
}


/*

功能：短信发送，让DTU重新启动
*/

void my_reset_mcu2()  //重启MCU通过软命令
{
    __disable_fault_irq();
    NVIC_SystemReset();
}


//功能：--利用AT+??指令获得返回结果的字符串到数组中
//参数：第一个参数，为指令，,第二个参数，为回到指令，第三参数为存储数组
uint8_t AT_Get_AT_Answer_string(uint8_t *AT_COMM,uint8_t *AT_ANSWER,uint8_t *pt)
{
    //uint8_t *AT_COMM;
    //uint8_t *AT_ANSWER;
    uint8_t my_gprs_status=0;

    uint8_t my_buf[256]= {0};
    uint16_t k=0;


    my_gprs_status=my_at_com(USART_GPRS,AT_COMM,AT_ANSWER,3,100);

    if(my_gprs_status==1)
    {
        my_gprs_status=AT_Get_Frame(my_buf);

        if(my_gprs_status==1)
        {
            for(k=0; my_buf[k]!='\0' && k<256; k++)
            {
                pt[k]=my_buf[k];

            }
        }
    }

    return my_gprs_status;

}

/*
功能：发送AT+CSQ指令，测量信号强度
*/

uint8_t MY_AT_CSQ_Value=0;  //信号强度，0--31,99为没有信号

void AT_GET_CSQ()
{
    uint8_t *AT_COMM;
    uint8_t *AT_ANSWER;
    uint8_t my_buf[256]= {0};
//	uint8_t my_temp[256]={0};
    uint8_t result=0;
    uint8_t CSQ_Value=0;

    AT_COMM=AT_CSQ;
    AT_ANSWER=AN_OK;

    result=AT_Get_AT_Answer_string(AT_COMM,AT_ANSWER,my_buf);

    //HexToStr2(my_temp,my_buf,20);
    // USART_printf(&huart3,my_temp);
    // USART_printf(&huart3,"\r\n");

    if(result==1)
    {
        if(my_buf[7]==0X2C) //信号强度只有一位数
        {
            CSQ_Value=my_buf[6]-0X30;
        }
        else            //信号强度两位数
        {
            CSQ_Value=(my_buf[6]-0X30)*10+(my_buf[7]-0X30);
        }

        MY_AT_CSQ_Value=(uint8_t) abs((int)(CSQ_Value));


        //USART_printf(&huart3,"AT+CSQ=%s\r\n",my_buf);
        USART_printf(&huart3,"AT+CSQ=%d\r\n",MY_AT_CSQ_Value);

    }
    else
    {
        MY_AT_CSQ_Value=0XFF;
        USART_printf(&huart3,"AT+CSQ is ERROR!!\r\n");
    }

}

//=====2017-6-8===========

/*
功能：AT命令发送TCP数据
输入参数：发送的字符串，长度不要大于256字节==1460个字节
输出参数：发送成功返回1，失败返回0
*/
uint8_t my_at_senddata(uint8_t *string)
{
    uint8_t *AT_COMM;
    uint8_t *AT_ANSWER;
    uint8_t my_gprs_status=0;
    uint8_t transmit_buf[25]=AT_QISEND;
    uint8_t temp_number_buf[5]="0";
    int number=0;
//	char  ch=0x1A;
    if(GPRS_Status==1)
    {
        //获得要发送的数据的长度字节数，然后修改AT_QISEND+长度+\r\n，采用确定长度发送方式
        if(*string==0x10||*string==0x68)
        {
            number=get_string_number(string);
            temp_number_buf[0]=number/1000+0x30;
            temp_number_buf[1]=number%1000/100+0x30;
            temp_number_buf[2]=number%100/10+0x30;
            temp_number_buf[3]=number%10+0x30;

            strcat((char *)transmit_buf,(char *)temp_number_buf);
            strcat((char *)transmit_buf,"\r\n");
            AT_COMM=transmit_buf;  //固定长度发送
        }
        else
        {
            AT_COMM=AT_QISEND2;	  //非固定长度发送1A结束
        }



        AT_ANSWER=AN_QISEND_WAIT;
        my_gprs_status=my_at_com(USART_GPRS,AT_COMM,AT_ANSWER,3,300);
    }

    if(my_gprs_status==1)  //发送数据
    {

        AT_COMM=string;
        AT_ANSWER=AN_QISEND_OK;
        HAL_Delay(5);  //延时，M35转换过快，不能接受数据
        my_gprs_status=my_at_com(USART_GPRS,AT_COMM,AT_ANSWER,1,600);

    }
    return(my_gprs_status);


    //======
}

/*
功能：循环发送AT指令n次，每间隔time时间，成功返回1，失败返回0
输入：串口名称，命令at,应答字符串，发送次数，发送时间间隔TIME*10ms
输出：成功1，失败0
*/
uint8_t my_at_com_long_data(UART_HandleTypeDef* USARTx,uint8_t *AT_COMM,uint8_t *AT_ANSWER,uint8_t n,uint32_t time,uint16_t data_size)
{
    u16 i=0;
    u8 comm_answer=0;
    uint8_t my_gprs_status=0;
    for(i=0; i<n; i++)
    {

        //USART_printf(USARTx,AT_COMM);
        HAL_UART_Transmit(USARTx,AT_COMM,data_size,1000);
        comm_answer=my_AT_process_usart(AT_ANSWER,time);  //1秒钟1次指令
        if(comm_answer==1)
        {
            //USART_printf(&huart3,"%s=OK!!\r\n",AT_COMM);	  //调试使用，正式使用不用了
            my_gprs_status=1;
            //GPRS_Heartdata_error_count=0;
            break;
        }
        else
        {
            USART_printf(&huart3,"GPRS Send long data error: %s=%d\r\n",AT_COMM,i);
            my_gprs_status=0;

            USART_printf(USARTx,"\x1A");	//调试，强制结束本次对话过程

            //GPRS_Heartdata_error_count++;//发送失败计数，大于5次就重启M35@@@
        }
    }
    return(my_gprs_status);
}



//===========
#define my_GRPS_send_mx 1450
/*
功能：超长帧数据发送，1帧最长1450个字节。
*/
uint8_t my_at_senddata_long(uint8_t *string)
{
    uint8_t *AT_COMM;
    uint8_t *AT_ANSWER;
    uint8_t my_gprs_status=0;
    uint8_t transmit_buf[25]=AT_QISEND;
    uint8_t temp_number_buf[5]="0";
    uint8_t my_block=0;
    uint16_t my_remain=0;
    uint8_t ii=0;
    int number=0;
//	char  ch=0x1A;
    if(GPRS_Status==1)
    {
        //获得要发送的数据的长度字节数，然后修改AT_QISEND+长度+\r\n，采用确定长度发送方式
        if(*string==0x10||*string==0x68)
        {
            number=get_string_number(string);
            if(number!=0)
                my_gprs_status=1;
            else
            {
                my_gprs_status=0;
                return my_gprs_status;
            }
        }
    }
    //=====
    if(my_gprs_status==1)
    {
        my_block=number/my_GRPS_send_mx;
        my_remain=number%my_GRPS_send_mx;

    }

//==========
    for(ii=0; ii<my_block && my_gprs_status==1 ; ii++)
    {
        temp_number_buf[0]=my_GRPS_send_mx/1000+0x30;
        temp_number_buf[1]=my_GRPS_send_mx%1000/100+0x30;
        temp_number_buf[2]=my_GRPS_send_mx%100/10+0x30;
        temp_number_buf[3]=my_GRPS_send_mx%10+0x30;

        //发送send指令
        strcpy((char *)transmit_buf,AT_QISEND);
        strcat((char *)transmit_buf,(char *)temp_number_buf);
        strcat((char *)transmit_buf,"\r\n");
        AT_COMM=transmit_buf;  //固定长度发送
        AT_ANSWER=AN_QISEND_WAIT;
        my_gprs_status=my_at_com(USART_GPRS,AT_COMM,AT_ANSWER,3,300);
        //发送数据
        if(my_gprs_status==1)  //发送数据
        {

            AT_COMM=string+ii*my_GRPS_send_mx;
            AT_ANSWER=AN_QISEND_OK;
            HAL_Delay(5);  //延时，M35转换过快，不能接受数据
            my_gprs_status=my_at_com_long_data(USART_GPRS,AT_COMM,AT_ANSWER,1,600,my_GRPS_send_mx);

        }
    }
    if(my_remain!=0 && my_gprs_status==1)
    {

        temp_number_buf[0]=my_remain/1000+0x30;
        temp_number_buf[1]=my_remain%1000/100+0x30;
        temp_number_buf[2]=my_remain%100/10+0x30;
        temp_number_buf[3]=my_remain%10+0x30;

        //发送send指令
        strcpy((char *)transmit_buf,AT_QISEND);
        strcat((char *)transmit_buf,(char *)temp_number_buf);
        strcat((char *)transmit_buf,"\r\n");
        AT_COMM=transmit_buf;  //固定长度发送
        AT_ANSWER=AN_QISEND_WAIT;
        my_gprs_status=my_at_com(USART_GPRS,AT_COMM,AT_ANSWER,3,300);
        //发送数据
        if(my_gprs_status==1)  //发送数据
        {

            AT_COMM=string+my_block*my_GRPS_send_mx;
            AT_ANSWER=AN_QISEND_OK;
            HAL_Delay(5);  //延时，M35转换过快，不能接受数据
            my_gprs_status=my_at_com_long_data(USART_GPRS,AT_COMM,AT_ANSWER,1,600,my_remain);

        }


    }

    return(my_gprs_status);


    //======
}

