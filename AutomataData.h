



struct ip_adr
{
  uint8_t 		ip1 = 0;
  uint8_t 		ip2 = 0;
  uint8_t 		ip3 = 0;
  uint8_t 		ip4 = 0;
};


struct AutomataData
{
  // TMP. z DS18b20	
  float 	gTempKolumna 	= 00.00;
  float 	gTempBeczka 	= 00.00;
  float 	gTempGlowica 	= 00.00;
  
  // TMP. z MODULE
  float 	gTempModWoda 	= 00.00;
  float 	gTempModBufor = 00.00;
  float 	gTempModWolny = 00.00;
  
  // TMP.ALARMY:
  float 	gTempAlarmuGlowica 	= 70.00;
  float 	gTempAlarmuBeczka 	= 105.00;
  
  // TMP.DNIA:
  float 	    gTempDnia = 00.00;

  // CISNIENIE:
  int32_t     gCisnienieDnia    = 0;
  int32_t     gPresureBMP = 0;
  
  // Aktualny etap:
  uint8_t  	gETAP = 0;
  
  // TERMOSTAT:
  float		gTempTermostat_stop = 72.0; //st C
  float	  gTempTermostat_start = 70.0; //st C
    
  // ROZGRZEWANIE:
  float   gTempStopRozgrzewania   = 00.00; //st C
  
  // ZALANIE:
  // Analogowy pin A0 na module czujnik
  int 				AnalogSensorValue = 0;
  uint8_t  	  gAktZalanie = 0;
  bool        gCzujnikZalania = false;
  int32_t     gWartoscZalania = 0;
  bool			  gPrzerwaWzalaniu = false;
  //bool			gAlarmZalania = false;
  int32_t     gCisnienieZalania_1 = 0;
  int32_t     gCisnienieZalania_2 = 0;
  int32_t     gCisnienieZalania_3 = 0;
  uint16_t    gPrzerwaZalania_1 = 5; // 4 min
  uint16_t    gPrzerwaZalania_2 = 5; // 5 min
  uint16_t    gPrzerwaZalania_3 = 5; // 6 min
  uint16_t    gCzasZalania_1 = 5;  // 8 min A
  uint16_t    gCzasZalania_2 = 5;  // 5 min B
  uint16_t    gCzasZalania_3 = 5;  // 3 min C
  
  
  // moduł mocy regulowany:
  bool  gPWR_MOD = false;
  
  // STABILIZACJA:
  uint16_t    gCzasStabilizacji = 15; // 15 min
  uint16_t    gStb_G1pwm = 0;    // 0-100 %
  bool           gStb_G1 = false;
  bool           gStb_G2 = false;
  bool           gStb_G3 = false;
  
    
  // PRZEDGON:
  uint8_t  	gCyklePrzedgonu = 0;
  uint16_t  gCzasOtwarciaZPrzedgonu = 5;	
  uint16_t  gCzasZamknieciaZPrzedgonu = 5;	
  uint16_t  gPrzg_G1pwm = 0;    // 0-100 %
  bool      gPrzg_G1 = false;
  bool      gPrzg_G2 = false;
  bool      gPrzg_G3 = false;
  
  // STAB_PRZEDGONU:
  uint16_t    gCzasStabilizacjiPrzedgonu = 5;  // 5 minut
  
  // PLUK_OLM:
  uint16_t    gCzasPlukanieOLM = 5; // 0-300 sek tu 5 sek
  
  // GON:
  uint16_t    gCzasZakonczeniaGon = 15;  //Czas zakończenia etapu krokowego.
  float		    gHisterezaG_open  = 0.5; //st C
  float		    gHisterezaG_close  = 1.0; //st C
  uint16_t    gGon_G1pwm = 0;    // 0-2000 w pwm
  bool        gGon_G1 = false;
  bool        gGon_G2 = false;
  bool        gGon_G3 = false;
  uint8_t     gKrok = 0; 
  float       TBufor80 = 0.0;
  float       TBufor60 = 0.0;
  float       TBufor40 = 0.0;
  bool        Bufor = false;
  
  // POGON: 
  float		gTempStopPogonu = 99.00; //stopnie C
  
   // CHLODZENIE:
  uint16_t    gCzasChlodzeniaGlow = 5; // 5 minut
  
  
  // CZAS:
  uint16_t volatile	gCzasProcesuGodz     =	0;
  uint16_t volatile	gCzasProcesuMin      =	0; 
  uint16_t volatile	gCzasProcesuSek      =	0;
  
  //Czas startu:
  uint16_t volatile	gCzasStartGodz     =	0;
  uint16_t volatile gCzasStartMin      =	0; 
  uint16_t volatile	gCzasStartSek       =	0;
  
     
  //ZAWORY:
  bool             gZaworGonu = false;
  bool             gZaworWoda = false;
  bool             gZaworPlukanie_OLM_m = false;
  bool             gZaworGlowica_m = false;
  uint16_t      gZaworGonCntr  = 0;	
  
     
  // DANE SIECIOWE WIFI:
  uint8_t  gWIFI_MODE = 0;      // default statione=0 1=AP 2=AP-service
  long  	 gWIFI_RSSI   = 0;      // power wifi
  char gSSID[34]   = "brak";
  char gPASS[64]   = "brak";
  ip_adr ip_STA = {0,0,0,0};
  ip_adr ip_AP   = {0,0,0,0};
 
 
  //arduino cloud:
  //char gCloudLogin[64]   = "af3d3caf-b693-4e96-8aec-abdbad55c428";
  //char gCloudKey[64]   = "KAS7C1ZXAZZJVSCXJM0U";
  
  // Config File Name:
  char FileName[32]   = "ustawienia fabryczne";
   
  uint8_t  gTEMPERATURE_PRECISION = 12;
  long     gESP_CNTR  = 0;  
  
  // stan grzałek
  bool		G1  = false;
  bool		G2  = false;
  bool		G3  = false;
  
  // moce grzałek
  uint16_t gMocGrzaniaG1 = 1500;
  uint16_t gMocGrzaniaG2 = 2000;
  uint16_t gMocGrzaniaG3 = 2500;
  
  const uint32_t gStruct_ID = 4294967295;
   
};

