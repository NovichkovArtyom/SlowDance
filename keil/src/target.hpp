#ifndef TARGET_HPP
#define TARGET_HPP

#include "mcu.hpp"

enum LED_t
{
	LED_1,
	LED_2
};

class Target
{	
	public:
		static void Do();
		static void Init ();
//		static int GetTime();
//		static uint32_t GetFreq ();
//		static void SetFreq (channel_t, int);
		static void SetLedState (LED_t,bool);
		static bool GetLedState(LED_t);
		static bool CheckBut1 ();
		static bool CheckBut2 ();
		static int GetTemperature ();
	private:
	//static	bool isFreqChange;
};


#endif

