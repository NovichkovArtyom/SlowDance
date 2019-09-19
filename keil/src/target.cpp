#include "target.hpp"
#include "adc.hpp"
#include "pwm.hpp" 


unsigned diffabs (unsigned a, unsigned b);
/// Режимы работы вывода микроконтроллера
enum McuPinMode_t
{
	PIN_MODE_INPUT = 0U,			///< Дискретный вход
	PIN_MODE_OUTPUT = 1U,			///< Дискретный выход
	PIN_MODE_ALTERNATE = 2U,		///< Альтернативная функция порта
	PIN_MODE_ANALOG = 3U,			///< Аналоговый режим
};


/// Скорость работы вывода микроконтроллера
enum McuPinSpeed_t
{
	PIN_SPEED_LOW = 0U,				///< Медленная
	PIN_SPEED_MEDIUM = 1U,			///< Средняя
	PIN_SPEED_HIGH = 2U,			///< Быстрая
	PIN_SPEED_VERY_HIGH = 3U,		///< очень быстрая
};


/// Режимы работы встроенных подтягивающих резисторов
enum McuPullMode_t
{
	PIN_PULL_DISABLED = 0U,			///< Подтяжки отключены
	PIN_PULL_UP = 1U,				///< Подтяжка к питанию
	PIN_PULL_DOWN = 2U,				///< Подтяжка к земле
	PIN_PULL_RESERVED = 3U,			///< Зарезервированное значение
};


/*____________________________________________________*/


void Target::Init(){
 //isFreqChange = 0;
	
	// Clock source configuration
/*	RCC->CR &= 
		~ (1 << 31) & // Reset PPL DIV for MCO
		~ (7 << 28) &	// Reset MCO DIV
		~ (15 << 24)&	// Reset MCO (MCO out disabled) 
		~ (15 << 18)& // Reset PLLMUL
		~ (1 << 17) & // Reset PLLXTPRE bit (devider for pll entery, same than PREDDIV[0] from CFGR2 register)
		~ (3 << 15) & // Reset PLL input clk source
		~ (1 << 14) & // Reset ADC presc
		~ (7 << 8)  & // Reset PCLK presc
*/
	/*____________________________________________________*/
	
	//Clock source config
	RCC -> CR	= 0x00000083;  // Reset state, clock source from HSI
	
	RCC -> CFGR = 0x0; // Reset state  
	
	RCC -> CFGR |= 
	(1 << 16); // HSE ON
	
	
	RCC -> CFGR |=
	(2 << 18) | // Configure PLLMUL as *4
	(2 << 15) ; // Configure HSE/Preddiv as PLL input
	
	RCC -> CFGR2 = 0x0;// Reset PLL PREDDIV (not devided)
	RCC -> CFGR3 = 0x0; //	Reset USART clock source (PLL as clock source USART,I2C, USB at 48HSI) 
	RCC -> CFGR |= 1 << 24 ; // PLL ON
	
	
	unsigned long temp = 0;
	while (RCC->CR & ~(1 << 17)) // HSE On? 
	{
		temp++;
		if (temp > 48000000)
		{
				NVIC_SystemReset(); // Reset
		}
	}
	
	temp = 0;
	
	while (RCC->CR & ~(1 << 25)) // PLL On?
	{
		temp++;
		if (temp > 48000000)
		{
				NVIC_SystemReset(); // Reset
		}
	}
	// Set PLL as clock source
	
	RCC -> CFGR = 2; 
	
/*____________________________________________________*/

	// AHB, APB configuration 
	RCC -> AHBENR = 	15 << 17; // IOPA - IOPF ON
	RCC -> APB2ENR = 	1 << 22 | 7 << 16 | 1 << 14 | 1 << 11 | 1 << 9 | 7 << 5 | 1 << 0; // All preipheral clock On (without SPI)
	RCC -> APB1ENR = 	1 << 28 |  15 << 17 | 1 << 8 | 2 << 4 | 2 << 0; // All usart, timers PWR ON
	
	
/*____________________________________________________*/
	
	// RTC configuration
//	RCC -> BDCR = 0x00000018; // Reset 	
//	RCC -> BDCR |= 3 << 8;
	RCC -> BDCR |= 1 << 15; // RTC clk EN

	
/*____________________________________________________*/

	// Ports configuration
	
	// Port A
	GPIOA->MODER =	
					(PIN_MODE_INPUT << 0) |				// PA0 ------> NOT_USED(INPUT)
					(PIN_MODE_ALTERNATE << 2) |			// PA1 ------> LED_MK(ALTERNATE)
					(PIN_MODE_INPUT << 4) |				// PA2 ------> NOT_USED(INPUT)
					(PIN_MODE_INPUT << 6) |				// PA3 ------> NOT_USED(INPUT)
					(PIN_MODE_INPUT << 8) |				// PA4 ------> NOT_USED(INPUT)
					(PIN_MODE_ANALOG << 10) |			// PA5 ------> Potentiometr 1(ANALOG)
					(PIN_MODE_ANALOG << 12) |			// PA6 ------> Potentiometr 2(ANALOG)
					(PIN_MODE_INPUT << 14) |			// PA7 ------> NOT_USED(INPUT)
					(PIN_MODE_INPUT << 16) |			// PA8 ------> Button 3(INPUT)
					(PIN_MODE_ALTERNATE << 18) |	// PA9 ------> USART_TX(ALTERNATE)
					(PIN_MODE_ALTERNATE << 20) |	// PA10 -----> USART_RX(ALTERNATE)
					(PIN_MODE_INPUT << 22) |			// PA11 -----> NOT_USED(INPUT)
					(PIN_MODE_INPUT << 24) |			// PA12 -----> NOT_USED(INPUT)
					(PIN_MODE_ALTERNATE << 26) |	// PA13 -----> SWDIO(ALTERNATE)
					(PIN_MODE_ALTERNATE << 28) |	// PA14 -----> SWCLK(ALTERNATE) 
					(PIN_MODE_INPUT << 30);				// PA15 -----> NOT_USED(INPUT)

    GPIOA->PUPDR = 	(PIN_PULL_DISABLED << 0) |// PA0 ------> NOT_USED(PULL_DISABLED)
					(PIN_PULL_DOWN << 2) |     				// PA1 ------> LED_MK(PULL_DISABLED)
					(PIN_PULL_DISABLED << 4) |      	// PA2 ------> NOT_USED(PULL_DISABLED)
					(PIN_PULL_DISABLED << 6) | 		    // PA3 ------> NOT_USED(PULL_DISABLED)
					(PIN_PULL_DISABLED << 8) | 		   	// PA4 ------> NOT_USED(PULL_DISABLED)
					(PIN_PULL_DISABLED << 10) |    	  // PA5 ------> Potentiometr 1(PULL_DISABLED)
					(PIN_PULL_DISABLED << 12) |    		// PA6 ------> Potentiometr 2(PULL_DISABLED)
					(PIN_PULL_DISABLED << 14) |    		// PA7 ------> NOT_USED(PULL_DISABLED)
					(PIN_PULL_UP << 16) |    					// PA8 ------> Button 3(PULLUP)
					(PIN_PULL_DISABLED << 18) |   	  // PA9 ------> USART_TX(PULL_DISABLED)
					(PIN_PULL_DISABLED << 20) |  		  // PA10 -----> USART_RX(PULL_DISABLED)
					(PIN_PULL_DISABLED << 22) |    		// PA11 -----> NOT_USED(PULL_DISABLED)
					(PIN_PULL_DISABLED << 24) |  			// PA12 -----> NOT_USED(PULL_DISABLED)
					(PIN_PULL_DISABLED << 26) | 	   	// PA13 -----> SWDIO(PULL_DISABLED)
					(PIN_PULL_DISABLED << 28) |	 		 // PA14 -----> SWCLK(PULL_DISABLED)
					(PIN_PULL_DISABLED << 30);				// PA15 -----> NOT_USED(PULLUP)

	GPIOA->OTYPER = 0; // CONFIG - PuPd
	GPIOA->OSPEEDR = (PIN_SPEED_VERY_HIGH << 28) | (PIN_SPEED_VERY_HIGH << 26) | (PIN_SPEED_VERY_HIGH << 10) | (PIN_SPEED_VERY_HIGH << 12);
	GPIOA->BSRR = 0;
	
	// Port B
	GPIOB->MODER =	
					(PIN_MODE_INPUT << 0) |			// PB0 ------> NOT_USED(INPUT)
					(PIN_MODE_INPUT << 2) |			// PB1 ------> NOT_USED(INPUT)
					(PIN_MODE_INPUT << 4) |			// PB2 ------> NOT_USED(INPUT)
					(PIN_MODE_ALTERNATE << 6) |	// PB3 ------> SWO(ALTERNATE)
					(PIN_MODE_OUTPUT << 8) |		// PB4 ------> LED 1(OUTPUT)
					(PIN_MODE_OUTPUT << 10) |		// PB5 ------> LED 2(OUTPUT)
					(PIN_MODE_INPUT << 12) |		// PB6 ------> NOT_USED(INPUT)
					(PIN_MODE_INPUT << 14) |		// PB7 ------> NOT_USED(INPUT)
					(PIN_MODE_INPUT << 16) |		// PB8 ------> NOT_USED(INPUT)
					(PIN_MODE_INPUT << 18) |		// PB9 ------> NOT_USED(INPUT)
					(PIN_MODE_INPUT << 20) |		// PB10 ------> NOT_USED(INPUT)
					(PIN_MODE_INPUT << 22) |		// PB11 ------> NOT_USED(INPUT)
					(PIN_MODE_INPUT << 24) |		// PB12 ------> NOT_USED(INPUT)
					(PIN_MODE_ALTERNATE << 26) |		// PB13 ------> Magnet (ALTERNATE)
					(PIN_MODE_INPUT << 28) |		// PB14 ------> Button 1(INPUT)
					(PIN_MODE_INPUT << 30),			// PB15 ------> Button 2(INPUT)

    GPIOB->PUPDR =
					(PIN_PULL_DISABLED << 0) |				// PB0 ------>  NOT_USED(PULL_DISABLED)
					(PIN_PULL_DISABLED << 2) |  			// PB1 ------>  NOT_USED(PULL_DISABLED)
					(PIN_PULL_DISABLED << 4) |     		// PB2 ------>  NOT_USED(PULL_DISABLED)
					(PIN_PULL_DISABLED << 6) | 				// PB3 ------>  NOT_USED(PULL_DISABLED)
					(PIN_PULL_DISABLED << 8) | 				// PB4 ------>	NOT_USED(PULL_DISABLED)
					(PIN_PULL_DISABLED << 10) | 			// PB5 ------>	NOT_USED(PULL_DISABLED)
					(PIN_PULL_DISABLED << 12) | 			// PB6 ------>	NOT_USED(PULL_DISABLED)
					(PIN_PULL_DISABLED << 14) | 			// PB7 ------> 	NOT_USED(PULL_DISABLED)
					(PIN_PULL_DISABLED << 16) |    		// PB8 ------> 	NOT_USED(PULL_DISABLED)
					(PIN_PULL_DISABLED << 18) | 			// PB9 ------> 	NOT_USED(PULL_DISABLED)
					(PIN_PULL_DISABLED << 20) | 			// PB10 ------> NOT_USED(PULL_DISABLED)
					(PIN_PULL_DISABLED << 22) |    		// PB11 ------> NOT_USED(PULL_DISABLED)
					(PIN_PULL_DISABLED << 24) |  			// PB12 ------> NOT_USED(PULL_DISABLED)
					(PIN_PULL_DISABLED << 26) | 			// PB13 ------> NOT_USED(PULL_DISABLED)
					(PIN_PULL_DISABLED << 28) | 			// PB14 ------> NOT_USED(PULL_DISABLED)
					(PIN_PULL_DISABLED << 30);				// PB15 ------> NOT_USED(PULL_DISABLED)

  GPIOB->OTYPER = 0;		// CONFIG - PuPd
	GPIOB->OSPEEDR = (PIN_SPEED_VERY_HIGH << 6);  // SWO 

	// Port C
	GPIOC->MODER =	(PIN_MODE_INPUT << 0) |			// PC0 ------> NOT_USED(INPUT)
					(PIN_MODE_INPUT << 2) |			// PC1 ------> NOT_USED(INPUT)
					(PIN_MODE_INPUT << 4) |			// PC2 ------> NOT_USED(INPUT)
					(PIN_MODE_INPUT << 6) |			// PC3 ------> NOT_USED(INPUT)
					(PIN_MODE_INPUT << 8) |			// PC4 ------> NOT_USED(INPUT)
					(PIN_MODE_INPUT << 10) |		// PC5 ------> NOT_USED(INPUT)
					(PIN_MODE_INPUT << 12) |		// PC6 ------> NOT_USED(INPUT)
					(PIN_MODE_INPUT << 14) |		// PC7 ------> NOT_USED(INPUT)
					(PIN_MODE_INPUT << 16) |		// PC8 ------> NOT_USED(INPUT)
					(PIN_MODE_INPUT << 18) |		// PC9 ------> NOT_USED(INPUT)
					(PIN_MODE_INPUT << 20) |		// PC10 -----> NOT_USED(INPUT)
					(PIN_MODE_INPUT << 22) |		// PC11 -----> NOT_USED(INPUT)
					(PIN_MODE_INPUT << 24) |		// PC12 -----> NOT_USED(INPUT)
					(PIN_MODE_INPUT << 26) |		// PC13 -----> NOT_USED(INPUT)
					(PIN_MODE_INPUT << 28) |		// PC14 -----> OSC32(INPUT)
					(PIN_MODE_OUTPUT << 30);		// PC15 -----> OSC32(OUTPUT)

    GPIOC->PUPDR = 	
					(PIN_PULL_DISABLED << 0) |				// PC0 ------> NOT_USED(PULL_DISABLED)
					(PIN_PULL_DISABLED << 2) |  			// PC1 ------> NOT_USED(PULL_DISABLED)
					(PIN_PULL_DISABLED << 4) |     		// PC2 ------> NOT_USED(PULL_DISABLED)
					(PIN_PULL_DISABLED << 6) | 				// PC3 ------> NOT_USED(PULL_DISABLED)
					(PIN_PULL_DISABLED << 8) | 				// PC4 ------> NOT_USED(PULL_DISABLED)
					(PIN_PULL_DISABLED << 10) | 			// PC5 ------> NOT_USED(PULL_DISABLED)
					(PIN_PULL_DISABLED << 12) | 			// PC6 ------> NOT_USED(PULL_DISABLED)
					(PIN_PULL_DISABLED << 14) | 			// PC7 ------> NOT_USED(PULL_DISABLED)
					(PIN_PULL_DISABLED << 16) |    		// PC8 ------> NOT_USED(PULL_DISABLED)
					(PIN_PULL_DISABLED << 18) | 			// PC9 ------> NOT_USED(PULL_DISABLED)
					(PIN_PULL_DISABLED << 20) | 			// PC10 -----> NOT_USED(PULL_DISABLED)
					(PIN_PULL_DISABLED << 22) |    		// PC11 -----> NOT_USED(PULL_DISABLED)
					(PIN_PULL_DISABLED << 24) |  			// PC12 -----> NOT_USED(PULL_DISABLED)
					(PIN_PULL_DISABLED << 26) | 			// PC13 -----> NOT_USED(PULL_DISABLED)
					(PIN_PULL_DISABLED << 28) | 			// PC14 -----> NOT_USED(PULL_DISABLED)
					(PIN_PULL_DISABLED << 30);				// PC15 -----> OSC32(PULL_DISABLED)

  GPIOC->OTYPER = 0; 
	GPIOC->OSPEEDR = 0; 
	
	


/*	
	RCC->CR |= 
	1 << 19 | // CSS ON
	0 << 18 | // No bypass 
	1 << 16 ; // HSE On
*/	
	

/*	//Configuration PLL, AHB, APB1&APB2 bus
	RCC->CFGR |= 
	4 << 18 | // PLLMUL = HSE*6 (72MHz)
	0 << 17 | // HSE not devided, 
	1 << 16 | // HSE as PLL entery
	0 << 14 |	// ADC not prescale
	0 << 15 | // ADC not prescale
	0 << 11 | // APB2 not devided
	4 <<  8 | // APB1 devided by 2
	0 <<  4;  // AHB not devided
	
//	RCC->CIR &= ~(1 << 18); // Clear interrupt PLL Ready flag
	RCC->CR |= 1 << 24; // PLL ON
	
	temp = 0;
	while (RCC->CR & (1 << 25)) // PLL Ready
	{
		temp++;
		if (temp > 72000000) // 1s
		{
				NVIC_SystemReset(); // Reset
		}
	}
 
		RCC->CFGR |= 2 << 0; // Set PLL as SYSCLK
		
*/		
	
}

void Target::SetLedState (LED_t LED, bool state){
	
	switch (LED)
	{
		case LED_1:
	{ 
		if (state)
		GPIOB ->BSRR |= 1U << 4U;
		else 
		GPIOB ->BSRR |= 1U << 20U;
		break;
	
	}
		case LED_2:
	{
			if (state)
			GPIOB ->BSRR |= 1U << 5U;
			else 
			GPIOB ->BSRR |= 1U << 21U;
	}
		default:
			break;
	}
}


bool Target::GetLedState (LED_t LED)
{
	switch (LED)
	{
		case LED_1:
		{
			return (bool)(GPIOB->IDR & (1 << 4));
		}
		case LED_2:
			return (bool)(GPIOB->IDR & (1 << 5));
	}
}


int Target::GetTemperature()
{
	/// Параметры калибровки температурного датчика (см reference manual)
	enum 
	{ 
		CALIBRATION_VOLTAGE = 3000,
		CALIBRATION_TEMPERATURE_LOW = 30,
		CALIBRATION_TEMPERATURE_HIGH = 110,
	};
	
	/// Получить результат преобразоания
	int result = (int)HardADC::GetAdcConversionResult(ADC_CHANNEL_TEMPERATURE);

	/// Инициализировать калибровочные значения из памяти микроконтроллера (см. reference manual)
	const uint16_t calibrationValue110 = *(const uint16_t* )0x1FFFF7C2;
	const uint16_t calibrationValue30 = *(const uint16_t* )0x1FFFF7B8;
	
	/// Сконвертировать результат преобразования для получения температуры (см. reference manual)
	result = (((result * 3300) / CALIBRATION_VOLTAGE) - (int32_t)calibrationValue30);
	result = result * (int32_t)(CALIBRATION_TEMPERATURE_HIGH - CALIBRATION_TEMPERATURE_LOW);
	result = result / (int32_t)(calibrationValue110 - calibrationValue30);
	result = result + CALIBRATION_TEMPERATURE_LOW;
	
	/// Вернуть результат
	return result;
}	
	

void Target::Do()
{
	
	if (GetLedState(LED_2))
	{
		SetLedState(LED_2, false);
	}
	// Get current potencional and convert to Hz*10
	unsigned magnetFreqtemp = (unsigned)(HardADC::GetAdcConversionResult(ADC_CHANNEL_MAGNET) / 3276);
	unsigned LEDFreqtemp = (unsigned) (HardADC::GetAdcConversionResult(ADC_CHANNEL_LED) / 3276);
		
	// Get current frequency 	and convert to Hz*10
	unsigned magnetFreq = (unsigned) (HardPWM::GetFreq (CHANNEL_MAGNET) / 3276);
	unsigned LEDFreq = (unsigned) 	 (HardPWM::GetFreq (CHANNEL_LED) / 3276);
	
	// Compare and set new frequency and blink a led
	if (magnetFreqtemp != magnetFreq)
	{
		HardPWM::SetFreq(CHANNEL_MAGNET, magnetFreqtemp);
		SetLedState(LED_2, true);
	}
	if (LEDFreqtemp != LEDFreq)
	{
		HardPWM::SetFreq(CHANNEL_LED, LEDFreqtemp);
		SetLedState(LED_2, true);
	}
	

}

