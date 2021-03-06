#include "stm32f10x_conf.h"
#include "stm32f10x.h"
#include <periph/usart1.h>
#include <periph/usart3.h>
#include <stdio.h>
#include "controller.h"
#include "soft_timers/SoftwareTimer.h"
#include "soft_timers/SoftwareTimer2.h"

void CLOCK_init(void);
static void gpio_init(void);


int main(void)
{
	CLOCK_init();
	__enable_irq();
	gpio_init();

	USART1_init();
	USART_Cmd(USART1, ENABLE);
	NVIC_EnableIRQ(USART1_IRQn);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	USART3_init();
    USART_Cmd(USART3, ENABLE);
	NVIC_EnableIRQ(USART3_IRQn);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

	printf("Init\r\n");
	CONTROLLER_init();
	while(1)
	{
		ResultCode code = CONTROLLER_perform_action();
		ProgramState state = CONTROLLER_get_state();
		switch(code)
		{
			case NONE:
				break;

			case INITIAL_ERROR:
				printf("Initial error in state %d\r\n", state);
				CONTROLLER_clear_error();
				break;

			case TIMEOUT:
				printf("Timeout error in state %d\r\n", state);
				break;
		}
	}

    return 0;
}


/*
 * *************************************
 * AHB  Freq = HCLK
 * APB2 Freq = PCLK2
 * APB1 Freq = PCLK1
 * *************************************
 */
void CLOCK_init(void)
{
    /* HCLK = SYSCLK / 1 = 72MHz */
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;

    /* PCLK2 = HCLK / 2 = 36MHz */
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;

    /* PCLK1 = HCLK / 2 = 36MHz */
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;
}


static void gpio_init(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

	/* PA3(reset) 2MHz push-pull, high(no reset) */
	//GPIOA->CRL &= ~GPIO_CRL_CNF3;
	//GPIOA->CRL |= GPIO_CRL_MODE3_1;
	//GPIOA->BSRR |= GPIO_BSRR_BS3;

	/* PA8(reset) 2MHz push-pull, high(no reset)*/
	GPIOA->CRH &= ~GPIO_CRH_CNF8;
	GPIOA->CRH |= GPIO_CRH_MODE8_1;
	GPIOA->BSRR |= GPIO_BSRR_BS8;

	/* SCK alternate push-pull 50MHz */
	GPIOA->CRL &= ~GPIO_CRL_CNF5;
	GPIOA->CRL |= GPIO_CRL_CNF5_1;
	GPIOA->CRL |= GPIO_CRL_MODE5;

	/* MOSI alternate push-pull 50MHz */
	GPIOA->CRL &= ~GPIO_CRL_CNF7;
	GPIOA->CRL |= GPIO_CRL_CNF7_1;
	GPIOA->CRL |= GPIO_CRL_MODE7;

	/* NSS alternate push-pull 50MHz */
	GPIOA->CRL &= ~GPIO_CRL_CNF4;
	GPIOA->CRL |= GPIO_CRL_CNF4_1;
	GPIOA->CRL |= GPIO_CRL_MODE4;

	/* MISO input floating */
	GPIOA->CRL &= ~GPIO_CRL_CNF6;
	GPIOA->CRL &= ~GPIO_CRL_MODE6;
	GPIOA->CRL |= GPIO_CRL_CNF6_0;
}






