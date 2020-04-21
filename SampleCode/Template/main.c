/******************************************************************************
 * @file     main.c
 * @version  V1.00
 * @brief    A project template for M031 MCU.
 *
 * Copyright (C) 2017 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/
#include <stdio.h>
#include <string.h>

#include "NuMicro.h"

typedef enum{
	flag_1ms = 0 ,
	flag_5ms ,
	flag_10ms ,
	flag_50ms ,	
	flag_100ms ,
	
	flag_DEFAULT	
}Flag_Index;

typedef enum{
	TIMER_1MS = 1 ,
	TIMER_5MS = 5 ,
	TIMER_10MS = 10 ,	
	TIMER_50MS = 50 ,
	TIMER_100MS = 100 ,
	TIMER_1S = 1000 ,
	
	TIMER_DEFAULT	
}TIMER_Define;

uint16_t conter_1ms = 0;

uint8_t BitFlag = 0;
#define BitFlag_ON(flag)							(BitFlag|=flag)
#define BitFlag_OFF(flag)							(BitFlag&=~flag)
#define BitFlag_READ(flag)							((BitFlag&flag)?1:0)
#define ReadBit(bit)								(uint8_t)(1<<bit)

#define is_flag_set(idx)							(BitFlag_READ(ReadBit(idx)))
#define set_flag(idx,en)							( (en == 1) ? (BitFlag_ON(ReadBit(idx))) : (BitFlag_OFF(ReadBit(idx))))

void loop_100ms(void)
{
	static uint16_t CNT = 1;
	static uint16_t LOG = 0;

	if (is_flag_set(flag_100ms))
	{		
		set_flag(flag_100ms,DISABLE);	

		if (CNT++ == (TIMER_1S/TIMER_100MS))
		{
			CNT = 1;
			printf("%s : %4d\r\n",__FUNCTION__,LOG++);
		}			
	}
}

void loop_50ms(void)
{
	static uint16_t CNT = 1;
	static uint16_t LOG = 0;
	
	if (is_flag_set(flag_50ms))
	{		
		set_flag(flag_50ms,DISABLE);	

		if (CNT++ == (TIMER_1S/TIMER_50MS))
		{
			CNT = 1;
			printf("%s : %4d\r\n",__FUNCTION__,LOG++);
		}			
	}
}



void loop_10ms(void)
{
	static uint16_t CNT = 1;
	static uint16_t LOG = 0;

	if (is_flag_set(flag_10ms))
	{		
		set_flag(flag_10ms,DISABLE);	

		if (CNT++ == (TIMER_1S/TIMER_10MS))
		{
			CNT = 1;
			printf("%s : %4d\r\n",__FUNCTION__,LOG++);
		}		
	}
}

void loop_5ms(void)
{
	static uint16_t CNT = 1;
	static uint16_t LOG = 0;

	if (is_flag_set(flag_5ms))
	{		
		set_flag(flag_5ms,DISABLE);	

		if (CNT++ == (TIMER_1S/TIMER_5MS))
		{
			CNT = 1;
			printf("%s : %4d\r\n",__FUNCTION__,LOG++);
		}		
	}
}

void loop_1ms(void)
{
	static uint16_t CNT = 1;
	static uint16_t LOG = 0;

	if (is_flag_set(flag_1ms))
	{		
		set_flag(flag_1ms,DISABLE);
		
		if (CNT++ == (TIMER_1S/TIMER_1MS))
		{
			CNT = 1;
			printf("%s : %4d\r\n",__FUNCTION__,LOG++);

//			PB14 ^= 1;
		}		
	}
}

void timer_counter(void)
{
	conter_1ms ++;
	
	set_flag(flag_1ms,ENABLE);
	
	if(!(conter_1ms %TIMER_5MS)){
		set_flag(flag_5ms,ENABLE);}
	
	if(!(conter_1ms %TIMER_10MS)){
		set_flag(flag_10ms,ENABLE);}
	
	if(!(conter_1ms %TIMER_50MS)){
		set_flag(flag_50ms,ENABLE);}
	
	if(!(conter_1ms %TIMER_100MS)){
		set_flag(flag_100ms,ENABLE);}

	if(conter_1ms >= 65500){
		conter_1ms = 0;}
}

void UARTx_Process(void)
{
	uint8_t res = 0;
	
	res = UART_READ(UART0);

	if (res == 'x' || res == 'X')
	{
		NVIC_SystemReset();
	}

	if (res > 0x7F)
	{
		printf("invalid command\r\n");
	}
	else
	{
		switch(res)
		{
			case '1':

				break;				
		}
	}
}

void UART02_IRQHandler(void)
{
    if(UART_GET_INT_FLAG(UART0, UART_INTSTS_RDAINT_Msk | UART_INTSTS_RXTOINT_Msk))     /* UART receive data available flag */
    {
        while(UART_GET_RX_EMPTY(UART0) == 0)
        {
            UARTx_Process();
        }
    }

    if(UART0->FIFOSTS & (UART_FIFOSTS_BIF_Msk | UART_FIFOSTS_FEF_Msk | UART_FIFOSTS_PEF_Msk | UART_FIFOSTS_RXOVIF_Msk))
    {
        UART_ClearIntFlag(UART0, (UART_INTSTS_RLSINT_Msk| UART_INTSTS_BUFERRINT_Msk));
    }
}

void UART0_Init(void)
{
    SYS_ResetModule(UART0_RST);

    /* Configure UART0 and set UART0 baud rate */
    UART_Open(UART0, 115200);

	/* Set UART receive time-out */
	UART_SetTimeoutCnt(UART0, 20);

	UART0->FIFO &= ~UART_FIFO_RFITL_4BYTES;
	UART0->FIFO |= UART_FIFO_RFITL_8BYTES;

	/* Enable UART Interrupt - */
	UART_ENABLE_INT(UART0, UART_INTEN_RDAIEN_Msk | UART_INTEN_TOCNTEN_Msk | UART_INTEN_RXTOIEN_Msk);
	
	NVIC_EnableIRQ(UART02_IRQn);

	printf("\r\nCLK_GetCPUFreq : %8d\r\n",CLK_GetCPUFreq());
	printf("CLK_GetHXTFreq : %8d\r\n",CLK_GetHXTFreq());
	printf("CLK_GetLXTFreq : %8d\r\n",CLK_GetLXTFreq());	
	printf("CLK_GetPCLK0Freq : %8d\r\n",CLK_GetPCLK0Freq());
	printf("CLK_GetPCLK1Freq : %8d\r\n",CLK_GetPCLK1Freq());	
}

void TMR1_IRQHandler(void)
{	
    if(TIMER_GetIntFlag(TIMER1) == 1)
    {
        TIMER_ClearIntFlag(TIMER1);

		timer_counter();	
    }
}

void TIMER1_Init(void)
{
    TIMER_Open(TIMER1, TIMER_PERIODIC_MODE, 1000);
    TIMER_EnableInt(TIMER1);
    NVIC_EnableIRQ(TMR1_IRQn);	
    TIMER_Start(TIMER1);
}

void SYS_Init(void)
{
    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Enable HIRC clock (Internal RC 48MHz) */
    CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk);
//    CLK_EnableXtalRC(CLK_PWRCTL_HXTEN_Msk);
	
    /* Wait for HIRC clock ready */
    CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk);
//    CLK_WaitClockReady(CLK_STATUS_HXTSTB_Msk);
	
    /* Select HCLK clock source as HIRC and HCLK source divider as 1 */
    CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HIRC, CLK_CLKDIV0_HCLK(1));

    /* Enable UART0 clock */
    CLK_EnableModuleClock(UART0_MODULE);
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART0SEL_PCLK0, CLK_CLKDIV0_UART0(1));
	
    CLK_EnableModuleClock(TMR1_MODULE);
    CLK_SetModuleClock(TMR1_MODULE, CLK_CLKSEL1_TMR1SEL_PCLK0, 0);
//    CLK_SetModuleClock(TMR1_MODULE, CLK_CLKSEL1_TMR1SEL_HIRC, 0);
	
    /* Update System Core Clock */
    SystemCoreClockUpdate();

    /* Set PB multi-function pins for UART0 RXD=PB.12 and TXD=PB.13 */
    SYS->GPB_MFPH = (SYS->GPB_MFPH & ~(SYS_GPB_MFPH_PB12MFP_Msk | SYS_GPB_MFPH_PB13MFP_Msk))    |       \
                    (SYS_GPB_MFPH_PB12MFP_UART0_RXD | SYS_GPB_MFPH_PB13MFP_UART0_TXD);

    /* Lock protected registers */
    SYS_LockReg();
}

/*
 * This is a template project for M031 series MCU. Users could based on this project to create their
 * own application without worry about the IAR/Keil project settings.
 *
 * This template application uses external crystal as HCLK source and configures UART0 to print out
 * "Hello World", users may need to do extra system configuration based on their system design.
 */

int main()
{
    SYS_Init();

    UART0_Init();
	
	TIMER1_Init();
	
    GPIO_SetMode(PB, BIT14, GPIO_MODE_OUTPUT);
	
    /* Got no where to go, just loop forever */
    while(1)
    {
		loop_1ms();	
		loop_5ms();
		loop_10ms();
		loop_50ms();
		loop_100ms();
	
    }
}

/*** (C) COPYRIGHT 2017 Nuvoton Technology Corp. ***/
