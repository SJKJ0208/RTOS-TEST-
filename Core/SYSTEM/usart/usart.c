#include "sys.h"
#include "usart.h"
#include "string.h"

////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//����1��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2019/9/17
//�汾��V1.5
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//********************************************************************************
//V1.3�޸�˵�� 
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.
//������printf��һ���ַ���ʧ��bug
//V1.4�޸�˵��
//1,�޸Ĵ��ڳ�ʼ��IO��bug
//2,�޸���USART_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
//3,������USART_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)
//4,�޸���EN_USART1_RX��ʹ�ܷ�ʽ
//V1.5�޸�˵��
//1,�����˶�UCOSII��֧��
////////////////////////////////////////////////////////////////////////////////// 	  


//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 0 ///��retarget��ʵ��
#pragma import(__use_no_semihosting)
//��׼����Ҫ��֧�ֺ���                 
struct __FILE
{
    int handle;

};

FILE __stdout;
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x)
{
    x = x;
}
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{
    while((USART1->SR&0X40)==0);//ѭ������,ֱ���������
    USART1->DR = (u8) ch;
    return ch;
}
#endif
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
u8 USART_RX_BUF2[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.

//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	  
u16 USART_RX_STA2=0;       //����״̬���

u8 bRxBuffer[RXBUFFERSIZE];///����hal�⴮�ڽ��ܻ���///�ڶ����������õ�
u8 aRxBuffer[RXBUFFERSIZE];//HAL��ʹ�õĴ��ڽ��ջ���

///���ھ��
UART_HandleTypeDef UART1_Handler; //UART���
UART_HandleTypeDef UART2_Handler;

///dmaת��
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;
//��ʼ��IO ����1 
//bound:������
void uart_init_1(u32 bound)
{	
	//UART ��ʼ������
	UART1_Handler.Instance=USART1;					    //USART1
	UART1_Handler.Init.BaudRate=bound;				    //������
	UART1_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //�ֳ�Ϊ8λ���ݸ�ʽ
	UART1_Handler.Init.StopBits=UART_STOPBITS_1;	    //һ��ֹͣλ
	UART1_Handler.Init.Parity=UART_PARITY_NONE;		    //����żУ��λ
	UART1_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //��Ӳ������
	UART1_Handler.Init.Mode=UART_MODE_TX_RX;		    //�շ�ģʽ

    if (HAL_UART_Init(&UART1_Handler) != HAL_OK) //HAL_UART_Init()��ʹ��UART1
    {
        Error_Handler();
    }
	HAL_UART_Receive_IT(&UART1_Handler, (u8 *)aRxBuffer, RXBUFFERSIZE);
    //�ú����Ὺ�������жϣ���־λUART_IT_RXNE���������ý��ջ����Լ����ջ���������������
}
void uart_init_2(u32 bound)
{

    /* USER CODE BEGIN USART2_Init 0 */

    /* USER CODE END USART2_Init 0 */

    /* USER CODE BEGIN USART2_Init 1 */

    /* USER CODE END USART2_Init 1 */
    UART2_Handler.Instance = USART2;
    UART2_Handler.Init.BaudRate = bound;
    UART2_Handler.Init.WordLength = UART_WORDLENGTH_8B;
    UART2_Handler.Init.StopBits = UART_STOPBITS_1;
    UART2_Handler.Init.Parity = UART_PARITY_NONE;
    UART2_Handler.Init.Mode = UART_MODE_TX_RX;
    UART2_Handler.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    UART2_Handler.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&UART2_Handler) != HAL_OK)      //HAL_UART_Init()��ʹ��UART1
    {
        Error_Handler();
    }
    /* USER CODE BEGIN USART2_Init 2 */
    HAL_UART_Receive_IT(&UART2_Handler, (uint8_t *)bRxBuffer, RXBUFFERSIZE);
    //�ú����Ὺ�������жϣ���־λUART_IT_RXNE���������ý��ջ����Լ����ջ������
    /* USER CODE END USART2_Init 2 */

}

//UART�ײ��ʼ����ʱ��ʹ�ܣ��������ã��ж�����
//�˺����ᱻHAL_UART_Init()����
//huart:���ھ��

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    //GPIO�˿�����
	GPIO_InitTypeDef GPIO_Initure;

	if(huart->Instance==USART1)//����Ǵ���1�����д���1 MSP��ʼ��
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();			//ʹ��GPIOAʱ��
		__HAL_RCC_USART1_CLK_ENABLE();			//ʹ��USART1ʱ��
		__HAL_RCC_AFIO_CLK_ENABLE();

		GPIO_Initure.Pin=GPIO_PIN_9;			//PA9
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//�����������
		GPIO_Initure.Pull=GPIO_PULLUP;			//����
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//����
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//��ʼ��PA9

		GPIO_Initure.Pin=GPIO_PIN_10;			//PA10
		GPIO_Initure.Mode=GPIO_MODE_AF_INPUT;	//ģʽҪ����Ϊ��������ģʽ��
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//��ʼ��PA10

///����dma_rx
        hdma_usart1_rx.Instance = DMA1_Channel5;
        hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
        hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
        hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
        hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
        hdma_usart1_rx.Init.Mode = DMA_NORMAL;
        hdma_usart1_rx.Init.Priority = DMA_PRIORITY_HIGH;
        if (HAL_DMA_Init(&hdma_usart1_rx) != HAL_OK)
        {
            Error_Handler();
        }
        __HAL_LINKDMA(huart,hdmarx,hdma_usart1_rx);

        hdma_usart1_tx.Instance = DMA1_Channel4;
        hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
        hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
        hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
        hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
        hdma_usart1_tx.Init.Mode = DMA_NORMAL;
        hdma_usart1_tx.Init.Priority = DMA_PRIORITY_HIGH;
        if (HAL_DMA_Init(&hdma_usart1_tx) != HAL_OK)
        {
            Error_Handler();
        }
        __HAL_LINKDMA(huart,hdmatx,hdma_usart1_tx);

        ///����ʹ���˽��ܣ�����Ҫ�����ж�
#if EN_USART1_RX
        HAL_NVIC_EnableIRQ(USART1_IRQn);				//ʹ��USART1�ж�ͨ��
        HAL_NVIC_SetPriority(USART1_IRQn,0,0);			//��ռ���ȼ�3�������ȼ�3
#endif
	}
    else if(huart->Instance==USART2)
    {
        GPIO_InitTypeDef GPIO_Initure2;
        /* USART2 clock enable */
        __HAL_RCC_USART2_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**USART2 GPIO Configuration
        PA2     ------> USART2_TX
        PA3     ------> USART2_RX
        */
        GPIO_Initure2.Pin = GPIO_PIN_2;
        GPIO_Initure2.Mode = GPIO_MODE_AF_PP;
        GPIO_Initure2.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOA, &GPIO_Initure2);

        GPIO_Initure2.Pin = GPIO_PIN_3;
        GPIO_Initure2.Mode = GPIO_MODE_INPUT;
        GPIO_Initure2.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_Initure2);

        /* USART2 interrupt Init */
        ///���ý����ж�
        HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(USART2_IRQn);
    }
}
void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

    if(uartHandle->Instance==USART1)
    {
        /* Peripheral clock disable */
        __HAL_RCC_USART1_CLK_DISABLE();

        /**USART1 GPIO Configuration
        PA9     ------> USART1_TX
        PA10     ------> USART1_RX
        */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

        /* USART1 interrupt Deinit */
        HAL_NVIC_DisableIRQ(USART1_IRQn);
    }
    else if(uartHandle->Instance==USART2)
    {
        /* Peripheral clock disable */
        __HAL_RCC_USART2_CLK_DISABLE();

        /**USART2 GPIO Configuration
        PA2     ------> USART2_TX
        PA3     ------> USART2_RX
        */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2|GPIO_PIN_3);

        /* USART2 interrupt Deinit */
        HAL_NVIC_DisableIRQ(USART2_IRQn);
    }

}

void USART_send_byte(uint8_t byte)
{
    while(__HAL_UART_GET_FLAG(&UART2_Handler,UART_FLAG_TC)!=SET);//�ȴ��������
    USART2->DR=byte;
}
//���Ͷ��ֽ�����
void USART_Send_bytes(uint8_t *Buffer, uint8_t Length)
{
    uint8_t i=0;
    while(i<Length)
    {
        USART_send_byte(Buffer[i++]);
    }
}
//���Ͷ��ֽ�����+У���
void USART_Send(uint8_t *Buffer, uint8_t Length)
{
    uint8_t i=0;
    while(i<Length)
    {
        if(i<(Length-1))
            Buffer[Length-1]+=Buffer[i];//�ۼ�Length-1ǰ������
        USART_send_byte(Buffer[i++]);
    }
}
//����һ֡����
void send_out(int16_t *data,uint8_t length,uint8_t send)
{
    uint8_t TX_DATA[30],i=0,k=0;
    memset(TX_DATA,0,(2*length+5));//���㻺��TX_DATA
    TX_DATA[i++]=0X5A;//֡ͷ
    TX_DATA[i++]=0X5A;//֡ͷ
    TX_DATA[i++]=send;//�����ֽ�
    TX_DATA[i++]=2*length;//���ݳ���
    for(k=0;k<length;k++)//�������ݵ�����TX_DATA
    {
        TX_DATA[i++]=(uint16_t)data[k]>>8;
        TX_DATA[i++]=(uint16_t)data[k];
    }
    USART_Send(TX_DATA,i);
}
void send_8bit_out(uint8_t *data,uint8_t length,uint8_t send)
{
    uint8_t TX_DATA[50],i=0,k=0;
    memset(TX_DATA,0,(2*length+5));//���㻺��TX_DATA
    TX_DATA[i++]=0X5A;//֡ͷ
    TX_DATA[i++]=0X5A;//֡ͷ
    TX_DATA[i++]=send;//�����ֽ�
    TX_DATA[i++]=length;//���ݳ���
    for(k=0;k<length;k++)//�������ݵ�����TX_DATA
    {
        TX_DATA[i++]=(uint16_t)data[k];
    }
    USART_Send(TX_DATA,i);
}

uint8_t RX_BUF_6050[50]={0},stata_6050=0;

//У��ͼ��
uint8_t CHeck(uint8_t *data)
{
    uint8_t sum=0,number=0,i=0;
    number=RX_BUF_6050[3]+5;
    if(number>42)//�����ϴ�����
        return 0;
    for(i=0;i<number-1;i++)
        sum+=RX_BUF_6050[i];
    if(sum==RX_BUF_6050[number-1])
    {
        memcpy(data,RX_BUF_6050,number);
        return 1;
    }
    else
        return 0;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    uint8_t RX_BUF[50]={0},stata=0;
    static uint8_t rebuf[13]={0},i=0;
	if(huart->Instance==USART1)//����Ǵ���1
	{
		if((USART_RX_STA&0x8000)==0)//����δ���
		{
			if(USART_RX_STA&0x4000)//���յ���0x0d
			{
				if(aRxBuffer[0]!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
				else USART_RX_STA|=0x8000;	//���������
			}
			else //��û�յ�0X0D
			{
				if(aRxBuffer[0]==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=aRxBuffer[0] ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����
				}
			}
		}
	}
    else if(huart->Instance==USART2)//����Ǵ���2
    {
        rebuf[i++]=bRxBuffer[0];
        if(rebuf[0]!=0x5a)//�ж�֡ͷ
            i=0;
        if((i==2)&&(rebuf[1]!=0x5a))//�ж�֡ͷ
            i=0;
        if(i>4)//��i����ֵ=5ʱ�������ֽڽ�����ϣ����ݳ����ֽڽ������
        {
            if(i==rebuf[3]+5)
            {
                memcpy(RX_BUF_6050,rebuf,i);
                stata_6050=1;
                i=0;
            }
        }
    }
}


//����1�жϷ������
void USART1_IRQHandler(void)
{
	u32 timeout=0;
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntEnter();
#endif
	HAL_UART_IRQHandler(&UART1_Handler);	//����HAL���жϴ����ú���
	timeout=0;
    while (HAL_UART_GetState(&UART1_Handler) != HAL_UART_STATE_READY)//�ȴ�����
	{
	 timeout++;////��ʱ����
     if(timeout>HAL_MAX_DELAY) break;

	}
	timeout=0;
	while(HAL_UART_Receive_IT(&UART1_Handler, (u8 *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)
        //һ�δ������֮�����¿����жϲ�����RxXferCountΪ1
	{
	 timeout++; //��ʱ����
	 if(timeout>HAL_MAX_DELAY) break;
	}
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntExit();
#endif
}


void USART2_IRQHandler(void)
{
    uint32_t  timeout=0;
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
    OSIntEnter();
#endif
    HAL_UART_IRQHandler(&UART2_Handler);
    timeout=0;
    while (HAL_UART_GetState(&UART2_Handler) != HAL_UART_STATE_READY)
        //�ȴ�����
    {
        timeout++;////��ʱ����
        if(timeout>HAL_MAX_DELAY) break;

    }
    timeout=0;
    while(HAL_UART_Receive_IT(&UART2_Handler, (uint8_t *)bRxBuffer, RXBUFFERSIZE) != HAL_OK)
        //һ�δ������֮�����¿����жϲ�����RxXferCountΪ1
    {
        timeout++; //��ʱ����
        if(timeout>HAL_MAX_DELAY) break;
    }
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
    OSIntExit();
#endif
}

/*�����������ֱ�Ӱ��жϿ����߼�д���жϷ������ڲ���*/

//����1�жϷ������
//void USART1_IRQHandler(void)
//{
//	u8 Res;
//	HAL_StatusTypeDef err;
//#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
//	OSIntEnter();
//#endif
//	if((__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_RXNE)!=RESET))  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
//	{
//		Res=USART1->DR;
//		if((USART_RX_STA&0x8000)==0)//����δ���
//		{
//			if(USART_RX_STA&0x4000)//���յ���0x0d
//			{
//				if(Res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
//				else USART_RX_STA|=0x8000;	//���������
//			}
//			else //��û�յ�0X0D
//			{
//				if(Res==0x0d)USART_RX_STA|=0x4000;
//				else
//				{
//					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
//					USART_RX_STA++;
//					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����
//				}
//			}
//		}
//	}
//	HAL_UART_IRQHandler(&UART1_Handler);
//#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
//	OSIntExit();
//#endif
//}
//#endif


