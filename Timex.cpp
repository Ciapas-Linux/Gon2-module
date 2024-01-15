
#include "Arduino.h"
#include "Timex.h"

Timex::Timex(void)
{
	curr_time.hour = 0;
	curr_time.minute = 0;
	curr_time.second = 0;
	
	/* for(int i = 0;i < MAX_TIME_BASE;i++)
	{
		time_base[i].time_id = 0;
		time_base[i].hour = 0;
		time_base[i].minute = 0;
		time_base[i].second = 0;
	} */
	
	run = false;
	
}

/* void Timex::save(int id)
{
	time_base[id].time_id = id;
	time_base[id].hour = curr_time.hour;
	time_base[id].minute = curr_time.minute;
	time_base[id].second = curr_time.second;
} */

/* void Timex::action_once(  int num, int hour, int minute, int second, void (*callback)() )
{
	actions[num].callback = callback; 
	actions[num].hour = hour;
	actions[num].minute = minute;
	actions[num].second = second;
	actions[num].active = true;
	actions[num].every_time = false;
} */

/* void Timex::action_stop(int num)
{
	actions[num].active = false;
} */

/* void Timex::getTimeString(String& str)
{
	
}
 */
int Timex::getCurrentMinute()
{
	return curr_time.minute;
}

int Timex::getCurrentSecond()
{
	return curr_time.second;
}

int Timex::getCurrentHour()
{
	return curr_time.hour;
}

int Timex::getCurrentMinuteA()
{
	return all_time.minute;
}

int Timex::getCurrentSecondA()
{
	return all_time.second;
}

int Timex::getCurrentHourA()
{
	return all_time.hour;
}

int Timex::getPassedMinutes()
{
	return (curr_time.hour*60) + curr_time.minute;
}

int Timex::getPassedSeconds()
{
	return (curr_time.hour*3600) + (curr_time.minute*60) + curr_time.second;
}

int Timex::getPassedSecondsA()
{
	return (all_time.hour*3600) + (all_time.minute*60) + all_time.second;
}

int Timex::getPassedHours()
{
	return curr_time.hour;
}

void Timex::stop()
{
	run = false;
}

void Timex::pause()
{
	run = false;
}

void Timex::resume()
{
	run = true;
}

void Timex::start()
{
	curr_time.hour = 0; 
	curr_time.minute  = 0;
	curr_time.second  = 0;
	run = true;
}

void Timex::reset()
{
	curr_time.hour = 0; 
	curr_time.minute  = 0;
	curr_time.second  = 0;
}

void Timex::resetA()
{
	all_time.hour = 0; 
	all_time.minute  = 0;
	all_time.second  = 0;
}

/* void Timex::print_time()
{
	Serial.print("");
    Serial.print(curr_time.hour);
    Serial.print(":");
    Serial.print(curr_time.minute);
    Serial.print(":");
    Serial.print(curr_time.second);
} */

void volatile Timex::update()
{
	if(run == false) return;
	
	// Current time:
	curr_time.second++;
	if(curr_time.second == 60)
	{
		 curr_time.minute++;
		 curr_time.second = 0;
	}
	if(curr_time.minute == 60)
	{
		 curr_time.hour++;
		 curr_time.minute = 0;
	}
		
	// ALL time:
	all_time.second++;
	if(all_time.second == 60)
	{
	     all_time.minute++;
		 all_time.second = 0;
	}
	if(all_time.minute == 60)
	{
	     all_time.hour++;
		 all_time.minute = 0;
	}
	
	
	 //for(int i = 0;i<NUM_ACTIONS;i++)
	 //{
		//if(actions[i].active == true)
		//{
				//if( (actions[i].hour * 3600) +  (actions[i].minute * 60) + actions[i].second >= getPassedSeconds() )
				//{
						//if(actions[i].every_time == false)
						//{
							//actions[i].hour =    0;
							//actions[i].minute = 0;
							//actions[i].second = 0;
							//actions[i].active = false;
							//actions[i].callback = 0;
						//}
						//(*actions[i].callback)();  //execute action function and clear action
				//}
		//}	 
	 //}
	
	
}


