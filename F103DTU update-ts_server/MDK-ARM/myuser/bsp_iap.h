#ifndef __BSP_IAP_H__
#define	__BSP_IAP_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "my_def_value.h"

/* 类型定义 ------------------------------------------------------------------*/
/************************** IAP 数据类型定义********************************/
typedef  void ( * pIapFun_TypeDef ) ( void ); //定义一个函数类型的参数.

/* 宏定义 --------------------------------------------------------------------*/
/************************** IAP 宏参数定义********************************/
//是否更新 APP 到 FLASH，否则更新到 RAM
#define User_Flash

#ifdef User_Flash
#define APP_START_ADDR2       0x8020000  	//应用程序起始地址(存放在FLASH)，bank备份区
#define APP_START_ADDR         0x8010000  	//应用程序起始地址(存放在FLASH)，执行区
#else
#define APP_START_ADDR       0x20001000  	//应用程序起始地址(存放在RAM)
#endif

/************************** IAP 外部变量********************************/
#define APP_FLASH_LEN  			 (rsbuf3_max-1024) // 51k-1k
#define APP_update_status1_add  0x807F000
#define APP_update_status2_add  0x807F002

#define APP_update_length1_add 0x807F004
#define APP_update_length2_add 0x807F008
#define APP_update_password 0xF1F1



/* 函数声明 ------------------------------------------------------------------*/
void IAP_Write_App_Bin( uint32_t appxaddr, uint8_t * appbuf, uint32_t applen);	//在指定地址开始,写入bin
void IAP_ExecuteApp( uint32_t appxaddr );			                              //执行flash里面的app程序

#endif /* __BSP_IAP_H__ */

/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
void my_fun_update_get_data_to_flash(void);
void my_fun_update_send_data(uint8_t my_port);
//__asm void MSR_MSP ( uint32_t ulAddr );

uint8_t my_fun_write_update_data_to_FLASH(void);
