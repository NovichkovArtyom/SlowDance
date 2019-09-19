


#include "adc.hpp"
#include "mcu.hpp"


unsigned HardADC::GetAdcConversionResult(ADC_Channel_t channel)
{
	/// �������� ������������ ���
	RCC -> APB2RSTR |= 1U << 9U; // Reset ADC interface
	RCC -> APB2ENR |=  1U << 9U;// EN ADC clock
	ADC1-> CFGR2 |=  1U << 30U; // ADC clock = PCLK / 2
	
	/// ��������� ���������� ���
	while ((ADC1->CR & ADC_CR_ADEN) != 0)
{
/* For robust implementation, add here time-out management */
}
	
	/// �������� ������������� ������ ��� �������������
	if(channel == ADC_CHANNEL_TEMPERATURE)
	{
		ADC1_COMMON -> CCR |= ADC_CCR_TSEN; 
	}

	/// ��������� ������ ���
	ADC1->SMPR = 4;  // sampling time 
	ADC1->CR = ADC_CR_ADEN; 
	while(!(ADC1->ISR & ADC_ISR_ADRDY))
	{
	}
	
	/// ��������� �������������� �� �������� ������
	ADC1->CHSELR = 1 << channel;
	ADC1->CR = ADC_CR_ADSTART;
	while(ADC1->CR & ADC_CR_ADSTART)
	{
	}

	/// �������� ��������� ��������������
	uint32_t result = ADC1->DR;  // ��� ������ � �����������? ������ uint32, ����� ������ 16 ��� 

	/// �������� ���
	ADC1->CR = ADC_CR_ADDIS;
	while(ADC1->CR & ADC_CR_ADDIS)
	{
	}

	/// ��������� ������������ ���
	RCC->APB2RSTR |= RCC_APB2RSTR_ADCRST;
	RCC->APB2ENR &= ~RCC_APB2ENR_ADCEN;	

	/// ������� ��������� ��������������
	return result;

}

