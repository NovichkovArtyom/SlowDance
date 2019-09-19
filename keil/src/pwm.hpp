


#include "mcu.hpp"



enum PWMchannel_t
{
	CHANNEL_MAGNET,
	CHANNEL_LED
};

class HardPWM
{
	public:
	static	void Init (); 
	static	void SetFreq (PWMchannel_t, int); // Set freqvency in HZ
	static	unsigned GetFreq (PWMchannel_t); // Get freqvency
	static	void TurnOn ();
};

