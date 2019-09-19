

#include "pwm.hpp"

// #define SYSCLKSOURCE 48000000U;



void HardPWM::Init ()
{
	// magnet PWM
	TIM1->PSC = 600U-1U; // Prescale
	TIM1->ARR = 1000U - 1U; // Counter/ 80HZ
	TIM1->DIER |= 1U; // Update event EN
	TIM1->CCR1 = 100U; // Pulse widht
	TIM1->CCER |= 1U; // Enable channel 1 as output
	TIM1->BDTR |= 1 << 15; //Main out En
	TIM1->CCER &=~ (1U << 1); // Active hight level
	TIM1->CCMR1 = 6U << 4; // PWM mode 1
	TIM1->CR1 &= ~ (1U << 4); // Upcount
	TIM1->CCR1 = 100U - 1U; // 10% PWM

	// TIM1->CR1 |= 1U; // EN counter
	
	// LED PWM
	TIM2->PSC = 600U-1U; 
	TIM2->ARR = 980U - 1U; // 81,6Hz
	TIM2->CCMR1 |= 6U << 12; // channel 2 PWM mode 1
	TIM2->CCER |= 1 << 4; // EN CH2
	TIM2->CR1 &= ~(1U << 4); 
	TIM2->CCR2 = 98U - 1U;  // 10% PWM
	TIM2->BDTR |= TIM_BDTR_MOE;
	//TIM2->CR1 |= 1U; //EN
	
}

void HardPWM::SetFreq (PWMchannel_t channel, int freq)
{
	switch (channel)
	{
		case CHANNEL_MAGNET:
		{
			uint32_t result = (((uint32_t)48000000U / freq) - 1) ;
			TIM1->ARR = result;
			break;
		}
		case CHANNEL_LED:
		{
			uint32_t result = ((uint32_t)(48000000U / freq) - 1 );
			TIM2->ARR = result;
			break;
		}
		default:
			break;
}
}

unsigned GetFreq(PWMchannel_t channel)
{
	switch (channel)
	{
		case CHANNEL_LED:
		{
			return (unsigned) TIM2->ARR;
		//	break;
		}
		case CHANNEL_MAGNET:
		{
			return (unsigned) TIM1->ARR;
			// break;
		}
		default:
		{
			return 0;
			//	break;
		}
	}
}
