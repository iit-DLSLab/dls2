/*******************************************************************************
*                                                       ,----,                 *
*                                                     .'   .' \                *
*                                                   ,----,'    |               *
*               ________  ___       ________        |    :  .  ;               *
*              |\   ___ \|\  \     |\   ____\       ;    |.'  /                *
*              \ \  \_|\ \ \  \    \ \  \___|_      `----'/  ;                 *
*               \ \  \ \\ \ \  \    \ \_____  \       /  ;  /                  *
*                \ \  \_\\ \ \  \____\|____|\  \     ;  /  /-,                 *
*                 \ \_______\ \_______\____\_\  \   /  /  /.`|                 *
*                  \|_______|\|_______|\_________\./__;      :                 *
*                                     \|_________||   :    .'                  *
*                                                 ;   | .'                     *
*                                                 `---'                        *
*******************************************************************************/
#include "dls2/util/time/time.hpp"
#include "dls2/topics/topics.hpp"

using namespace dls;

Time::Time()
	: time_factor(1)
	, timeLink("Time::timeLink", dls::domains::signals)
{
	timeLink.addReader(
		"Time::sinc", 
		dls::topics::time_factor,
		std::function<void(void *)>
		{
			[&](void *tuple)
			{	
				DoubleMsg msg = *((DoubleMsg *)tuple);
					
				if(msg.value() > 1)
					Time::time_factor = msg.value();
			}
		}
	);

	timeLink.addWriter("time_writer", dls::topics::time_factor);
}

Time::~Time()
{ }

double Time::getRealTimeFactor()
{
	return this->time_factor;
}

void Time::setRealTimeFactor(double factor)
{
	if(factor < 1)
		return;

	this->time_factor = factor;

	DoubleMsg msg;
	msg.value(factor);

	this->timeLink.sendMessage("time_writer", (void*) &msg);
}