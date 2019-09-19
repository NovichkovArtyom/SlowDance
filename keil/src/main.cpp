#include "target.hpp"
#include "pwm.hpp"
int main (void)
{


	Target::Init();
	HardPWM::Init();
	Target::SetLedState(LED_1,true);
	
while(true)
{
	Target::Do();
}


}


