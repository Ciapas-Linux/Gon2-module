

//#define NUM_ACTIONS 32
#define MAX_TIME_BASE 4

/* struct action
{
   int  hour = 0;
   int  minute = 0;
   int  second = 0;
   bool every_time = false;
   bool active = false;
   void (*callback)(void);
}; */

struct time_tmx
{
   uint8_t  time_id;
   uint16_t hour;
   uint16_t minute;
   uint16_t second;
};

class Timex
{

public:
  Timex(void);

  //void action_once( int num, int hour, int minute,int second, void (*callback)() );
  //void action_stop(int num);
  void start();
  void pause();
  void resume();
  void stop();
  //void save(int id);
  void volatile update();
  void reset();
  void resetA();
  //void print_time();
  //void getTimeString(String &str);
  int getCurrentMinute();
  int getCurrentSecond();
  int getCurrentHour();
  int getCurrentMinuteA();
  int getCurrentSecondA();
  int getCurrentHourA();
  int getPassedMinutes();
  int getPassedSeconds();
  int getPassedSecondsA();
  int getPassedHours();
  
  //time_tmx time_base[MAX_TIME_BASE];

  
private:
	
	time_tmx curr_time = {0,0,0,0};
	time_tmx all_time = {0,0,0,0};
	
	bool volatile run = false;
   //char print_buff[10];
   //action actions[NUM_ACTIONS];
   
};
