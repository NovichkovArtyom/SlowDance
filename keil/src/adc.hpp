

#ifndef ADC_HPP
#define ADC_HPP



// Используемые каналы ADC

enum ADC_Channel_t
{
	ADC_CHANNEL_MAGNET = 5, // Канал потенциометра, отвечающего за частоту магнита (Potentiometr 1)
	ADC_CHANNEL_LED		 = 6,	// Канал потенциометра, отвечающего за светдодиодную ленту (Potentiometr 2 )
	ADC_CHANNEL_TEMPERATURE = 16 // Температура
};

class HardADC
{
	public:
		
			static unsigned GetAdcConversionResult(ADC_Channel_t channel);
};
		
#endif

