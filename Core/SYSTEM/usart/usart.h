#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h"
#ifdef __cplusplus
extern "C" {
#endif
#include "main.h"

#define USART_REC_LEN  	200  	//定义最大接收字节数 200
#define EN_USART1_RX    1
#define RXBUFFERSIZE    1
extern UART_HandleTypeDef UART1_Handler; //UART句柄
extern UART_HandleTypeDef UART2_Handler;

extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记

extern u8  USART_RX_BUF2[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符
extern u16 USART_RX_STA2;         		//接收状态标记
//如果想串口中断接收，请不要注释以下宏定义
void uart_init(u32 bound);
void USART_Send_bytes(uint8_t *Buffer, uint8_t Length);


///6050所需的配置
#define ACC 0X01//加计控制位
#define GYR 0X02//陀螺控制位
#define MAG 0X04//磁场控制位
#define RPY 0X08//欧拉角控制位
#define Q4  0X10//四元数控制位
void Usart_Int(uint32_t BaudRatePrescaler);
void Usart_Int2(uint32_t BaudRatePrescaler);
void USART_send_byte(uint8_t byte);
void USART_Send_bytes(uint8_t *Buffer, uint8_t Length);
void send_out(int16_t *data,uint8_t length,uint8_t send);
void send_8bit_out(uint8_t *data,uint8_t length,uint8_t send);
uint8_t CHeck(uint8_t *data_buf);
extern uint8_t stata;
extern uint8_t stata_6050;
extern uint8_t RX_BUF[50];
///6050所需的配置

///初始化串口的函数对外接口
void uart_init_1(u32 bound);
void uart_init_2(u32 bound);




#ifdef __cplusplus
}
#endif
#endif


