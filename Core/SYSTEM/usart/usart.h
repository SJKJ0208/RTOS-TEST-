#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h"
#ifdef __cplusplus
extern "C" {
#endif
#include "main.h"

#define USART_REC_LEN  	200  	//�����������ֽ��� 200
#define EN_USART1_RX    1
#define RXBUFFERSIZE    1
extern UART_HandleTypeDef UART1_Handler; //UART���
extern UART_HandleTypeDef UART2_Handler;

extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���

extern u8  USART_RX_BUF2[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з�
extern u16 USART_RX_STA2;         		//����״̬���
//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart_init(u32 bound);
void USART_Send_bytes(uint8_t *Buffer, uint8_t Length);


///6050���������
#define ACC 0X01//�Ӽƿ���λ
#define GYR 0X02//���ݿ���λ
#define MAG 0X04//�ų�����λ
#define RPY 0X08//ŷ���ǿ���λ
#define Q4  0X10//��Ԫ������λ
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
///6050���������

///��ʼ�����ڵĺ�������ӿ�
void uart_init_1(u32 bound);
void uart_init_2(u32 bound);




#ifdef __cplusplus
}
#endif
#endif


