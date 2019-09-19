

#ifndef ADC_HPP
#define ADC_HPP



// ������������ ������ ADC

enum ADC_Channel_t
{
	ADC_CHANNEL_MAGNET = 5, // ����� �������������, ����������� �� ������� ������� (Potentiometr 1)
	ADC_CHANNEL_LED		 = 6,	// ����� �������������, ����������� �� ������������� ����� (Potentiometr 2 )
	ADC_CHANNEL_TEMPERATURE = 16 // �����������
};

class HardADC
{
	public:
		
			static unsigned GetAdcConversionResult(ADC_Channel_t channel);
};
		
#endif

