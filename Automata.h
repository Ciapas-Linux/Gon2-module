
#include "PCF8574.h"
#include <EasyTransfer.h>
#include <RBD_Timer.h>
#include "Timex.h"
//#include <Ticker.h>
#include <WebSocketsServer.h>
#include "AutomataData.h"

// sterownik
#define BUZZER         P4
#define GRZALKA_1    P6    
#define GRZALKA_2    P3
#define GRZALKA_3    P7    
#define BUTTON_1      D0
#define BUTTON_2      D5
#define DS_ZBIORNIK D4
#define DS_KOLUMNA  D6
#define DS_GLOWICA  D7
#define EZ_WODA        P2  // sterownik   
#define EZ_GON           P5  // kolumna sterownik

//#define BUTTON_1      D0
//#define BUTTON_2      D5


// są cztery zawory 2 w sterowniku i dwa w module
// pogon i przedgon to jest jeden ten sam zawór

// jedna dioda na module jest od informacja o połączeninu ze sterownikiem
// zamiast tego jednego zbędnego EZ.

// moduł
#define EZ_GLOWICA    100  // głowica  pogon
//#define EZ_PRZEDGON  101 //  głowica przedgon
#define EZ_PLUK_OLM   102 //  kolumna
// #define DS_1_WODA  103
// #define DS_2_BUFOR  104
// #define DS_2_WOLNY  105

// status kod
#define TERMOSTAT 0
#define ROZGRZEWANIE 1
#define ZALEWANIE 2
#define STABILIZACJA 3
#define PRZEDGON 4
#define STAB_PRZEDGONU 5
#define PLUKANIE_OLM 6
#define ODBIOR_GONU 7
#define ODBIOR_POGONU 8
#define CHLODZENIE_GLOWICY 9
#define SCODE_NIC  10
#define SCODE_KONIEC 11
#define SCODE_PAUSE 12
#define SCODE_ALARM_ZALANIA 13

// module kody
#define MC_OK 239 
#define MC_DS_ERROR 10
#define MC_EZ_GLOWICA_ON 11
#define MC_EZ_GLOWICA_OFF 12
#define MC_EZ_PLUK_OLM_ON 13
#define MC_EZ_PLUK_OLM_OFF 14
#define MC_UPDATE_ENABLE 15
  
  // sloty pamięci dla timex
#define TIMEX_SLOT_TERMOSTAT 0
#define TIMEX_SLOT_ROZGRZEWANIE 1
#define TIMEX_SLOT_ZALEWANIE_MEM 2
#define TIMEX_SLOT_ZALEWANIE 3
#define TIMEX_SLOT_STABILIZACJA 4
#define TIMEX_SLOT_PRZEDGON 5
#define TIMEX_SLOT_STAB_PRZEDGONU 6
#define TIMEX_SLOT_PLUKANIE_OLM 7
#define TIMEX_SLOT_ODBIOR_GONU 8
#define TIMEX_SLOT_ODBIOR_POGONU 9
#define TIMEX_SLOT_CHLODZENIE_GLOWICY 10

//*****************************************
// MODULE RX-TX
//create two objects
struct RECEIVE_DATA_STRUCTURE
{
  int         module_action_code;
  float 	    DS_1_WODA 	           = 00.00;
  float 	    DS_2_BUFOR 	           = 00.00;
  float 	    DS_2_WOLNY 	       = 00.00;	
  float       gModuleTempBMP      = 00.00;
  int32_t     gModulePresureBMP   = 0;
  int 			  AnalogSensorValue = 0;
};

struct SEND_DATA_STRUCTURE
{
  int module_action_code = 0;
};
//******************************************

struct POWER
{
 bool G1 = false;
 bool G2 = false;
 bool G3 = false;
};


class Automata
{

public:
  Automata(FS* fileSystem,WebSocketsServer* wss, PCF8574* ext, AutomataData* atd, Timex* tmx, RBD::Timer* tm_buz,RBD::Timer* tm_zg);
    
  void Run();
  void ModuleUpdate();
  void Start();
  void StopAutoAlarmZalania();
  void Stop();
  void Pause();
  void Resume();
  void ReStart();
  void volatile UpdateTime();
  void TempDnia();
  int  Status(); 
  bool IsStarted();

  void SetStatusCode(int scode);
  
  void ModuleStartWifi();   
  
  bool Grzalka(int grz);
  bool Grzalka1();
  bool Grzalka2();
  bool Grzalka3();
  bool Grzanie_hw();
  bool Grzanie_sw();
  void Grzalka_ON(int grz);
  void Grzalka_OFF(int grz);
  void WszystkieGrzalki_OFF();
  void WszystkieGrzalki_ON();
  void SavePowerState();
  void RestorePowerState();
  void ClearPowerState();
  void set_et3_power();
  void set_et4_power();
  void set_et7_power();
    
  void Zawor_ON(int valve);
  void Zawor_OFF(int valve);
  void Zawor_Krok();
  void Zawor_Krok_bufor();
  bool Zawor(int valve);
  void SaveValvesState();
  void RestoreValvesState();
  void ClearAllValvesState();

 
 void TermostatStart();
 void TermostatStop();
 
 void Grupowe_ON_G1();
 void Grupowe_OFF_G1();
 void Grupowe_Run();
 
 bool AlarmZalaniaStatus();
 void AlarmZalania();
 void AlarmZalaniaStop();

 
 void AskModule();

 void ETAP_0_TERMOSTAT();
 void ETAP_1_ROZGRZEWANIE();
 void ETAP_2_ZALEWANIE();	
 void ETAP_3_STABILIZACJA();	
 void ETAP_4_PRZEDGON();	
 void ETAP_5_STAB_PRZEDGON();	
 void ETAP_6_PLUKANIE_OLM();	
 void ETAP_7_ODBIOR_GONU();
 void ETAP_7_ODBIOR_GONU_BUFOR();	
 void ETAP_8_ODBIOR_POGONU();	
 void ETAP_9_CHLODZENIE_GLOWICY();
  
 int  GetModuleCntr();
 void Beep(int time);
 void file_print_log(String message);		
  
private:
	PCF8574* extender;
	AutomataData* automata_data;
	Timex* timex;
  FS* fileSystem;
	
	WebSocketsServer* websocket;
	
	POWER power_state;
		
	RBD::Timer* timer_buzzer;
	RBD::Timer* timer_ZGON;
	
	bool przerwa_zalania = false;
	bool przekroczenie_cisnienia = false;
	bool koniec = false;
	
	uint8_t volatile status_code = 0;
	uint8_t  previous_status_code = 0;
			
	uint8_t AktualnyCyklPrzedgonu = 1;
	bool CyklPrzedgonuZawor = false;
	
	int   zawor_close_time_min = 0;
	bool  zawor_krok_setup = true;
	bool  SpadekTempKolumna = false;
	uint16_t czas_przerwy_zalania = 0;
	uint16_t koniec_przerwy_zalania = 0;
	bool PrzekroczenieTmpKolumna = false;
	uint16_t  tCzasZakG = 0;
	bool CzasZakGReaded = false;
	
	// zalanie w innych etapach po przyłożeniu za dużej mocy
	uint16_t start_zalania_alarm = 0;
	//bool alarm_zalania = false;
	
	
	uint16_t czas_otw_ezglowica = 0;
	uint16_t num_otw_ezglowica = 0;
	
	uint16_t czas_zam_ezglowica = 0;
	uint16_t num_zam_ezglowica = 0;
	
	bool bufor_speed = false;
	uint8_t  bufor_speed_num = 1;
		
		
	bool Timer_G1 = false;
	uint32_t oldtime = 0;
	uint16_t pwr_on = 0;
	uint16_t pwr_off = 0;
	  			
	EasyTransfer ETin, ETout; 
	RECEIVE_DATA_STRUCTURE receive_data;
	SEND_DATA_STRUCTURE 	send_data;

	bool ModuleLive = false;
	long  module_cntr = 0;
	
	//valve state for saving:
	bool     save_EZ_WODA = false;
  bool     save_EZ_GLOWICA = false;
  bool     save_EZ_PLUK_OLM = false;
  bool     save_EZ_GON = false;
    
  uint16_t work_hour = 0;
  uint16_t work_minute = 0;
  uint16_t work_second = 0;
	
		
   
};

//Ticker grupowe:

// Sposób 1, moja wersja regulacja czasem:
// polega na tym że jak byśmy podłączyli żarówkę to będzie mrygała.
// przy małej wartości zadanej np 3% będzie większość czasu zgaszona i zapali się zgodnie z obliczeniem
// tylko na 30 milisekund. A przy dużej wartości np 87% będzie większośc czasu włączona
// czyli będzie palić się aż 870 ms.

// okno czasowe przyjołem 1 sekundę 1000 ms 
// Wartość mocy zadana w procentach 


//  ile czasu ms ma być zapalone przy zadanych np: 50% 
//  i ile czasu ms być zgaszone przy 50%


// Wzór:
// Czas świecenia ms = 1000 * Zadany procent / 100
// Czas wyłączenia ms = 1000 - Czas świecenia ms

// dla wartości przykładowo 15%
// czas świecenia to 150 ms
// czas nie świecenia to 850 ms

// 40% 400ms
// 50% 500ms
// 60% 600ms
// 70% 700ms
// 80% 800ms
// 90% 900 ms
//100% 1000ms





// Sposób 2 regulacja częstotliwością.
// można przyjac że jak mrygać żarówką z częstotliwością 50 razy na sekundę( 50hz )
// to będzie owe 100% to tak jakby cały czas była włączona, a 1% to będzie jak mrygnie raz na sekundę.

// 100% = 50 hz tak jakby cały czas włączone dosyć szybko mryga
// 1% = 1hz
// 0% = 0hz  wyłączone

// dla zadanej wartości w % to ile razy na sekundę ma się włączyć
// i wyłączyć żarówka skoro wiadomo że 100% to jest 50hz

// wzór:
// częstotliwość  = 50hz * zadany procent / 100 

// dla 5% wychodzi 2.5 hz czyli ma mrygać 2,5 razy na sekundę
// dla 50% wychodzi 25 hz czyli zapala się i gaśnie 25 razy na sekundę
// dla 88% wychodzi  44 hz czyli ma mrygać 44 razy w ciągu sekundy


