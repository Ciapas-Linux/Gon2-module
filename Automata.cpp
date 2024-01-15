
#include "Arduino.h"
#include "Automata.h"



Automata::Automata(FS* fs,
				   WebSocketsServer* wss,
				   PCF8574* ext, 
				   AutomataData* atd,
				   Timex* tmx,
				   RBD::Timer* tm_buz,
				   RBD::Timer* tm_zg)
{
	extender = ext;
	automata_data = atd;
	timex = tmx;
	timer_buzzer = tm_buz;
	timer_ZGON = tm_zg;
	websocket = wss;
	fileSystem = fs;
	
	//receive data struct
	receive_data.module_action_code  = 0;
    receive_data.gModulePresureBMP = 0;
    receive_data.gModuleTempBMP = 0.00;
    receive_data.DS_1_WODA = 0.00;
    receive_data.DS_2_BUFOR = 0.00;
    receive_data.DS_2_WOLNY = 0.00;
    receive_data.AnalogSensorValue = 0;
    
    //send data struct
    send_data.module_action_code = 0;
    
    //Next_Power_Click_Count = 1;
    //Current_Power_Click_Count = 1;
    
    timer_buzzer->stop();
    timer_ZGON->stop();
    
    status_code = SCODE_NIC;
    
       
    // MODULES COMM RX-TX
    ETin.begin(details(receive_data), &Serial);
    ETout.begin(details(send_data), &Serial);
      
}

void Automata::SetStatusCode(int scode)
{
	this->status_code = scode;
}

void Automata::file_print_log(String message)
{
	String sTmpStr;
	sTmpStr += String(automata_data->gCzasProcesuGodz) +
			   ":" + String(automata_data->gCzasProcesuMin) +
			   ":" + String(automata_data->gCzasProcesuSek);
	File LogFile = fileSystem->open("log.txt", "a");
	if (!LogFile)
	{
		//Serial.println("");
		//Serial.println("Error opening log.txt file for writing");
	}
	LogFile.print(sTmpStr + ":  " + message);

	LogFile.close();
}


void Automata::Beep(int time)
{
	extender->digitalWrite(BUZZER, HIGH);
    timer_buzzer->setTimeout(time);
    timer_buzzer->restart();
}

int Automata::GetModuleCntr()
{
	return module_cntr;
}		

bool Automata::IsStarted()
{
	if(status_code != SCODE_NIC && status_code != SCODE_KONIEC && status_code != TERMOSTAT)
	{
		return true;
	}else
	{
		return false;
	}
}   

void Automata::TempDnia()
{
  automata_data->gTempDnia = automata_data->gTempKolumna;
  automata_data->gCisnienieDnia = automata_data->gPresureBMP;
  
  // ?????????
  //.................................................
  //TempKolumnaValveOFF = gParametryPracy.gTempKolumna;
  //.................................................
    
  /* Serial.print(F("Tmp Dnia: "));
  Serial.print(automata_data->gTempKolumna);
  Serial.println(F(""));
  
  Serial.print(F("Cisnienie Dnia: "));
  Serial.print(automata_data->gPresureBMP);
  Serial.println(F("")); */

  //file_print_log("Automat > Tmp Dnia: " + String(automata_data->gTempKolumna) + "\r\nCisnienie Dnia: " + String(automata_data->gPresureBMP) + "\r\n");	

}

void Automata::ModuleUpdate()
{
	if(ETin.receiveData())
	{
			// Serial.printf("\n");
		    // Serial.println("Module data:");
			// Serial.println(receive_data.module_action_code);
			// Serial.println(receive_data.gModuleTempDS1);
            // Serial.println(receive_data.gModuleTempDS2);
            // Serial.println(receive_data.gModuleTempDS3);
            // Serial.printf("\n");
                        
            module_cntr++;
            
            switch (receive_data.module_action_code)
		   {
				 case MC_DS_ERROR:
				 {
					//Serial.println("Module ds error code 10");
					//Beep(300);
					automata_data->gPresureBMP = 0;
					automata_data->gTempModWoda = 0.0;
					automata_data->gTempModBufor = 0.0;
					automata_data->gTempModWolny = 0.0;
				}
				 break;
				 
				case  MC_OK:
				{
					automata_data->gPresureBMP = receive_data.gModulePresureBMP;
					
					automata_data->AnalogSensorValue = receive_data.AnalogSensorValue;
					
					if(receive_data.DS_1_WODA != -127.00)
					{
						automata_data->gTempModWoda = receive_data.DS_1_WODA;
					}else
					{
						automata_data->gTempModWoda = 0.00;
					}
					
					if(receive_data.DS_2_BUFOR != -127.00)
					{
						automata_data->gTempModBufor  = receive_data.DS_2_BUFOR;
					}else
					{
						automata_data->gTempModBufor  = 0.00;
					}
					
					if(receive_data.DS_2_WOLNY != -127.00)
					{
						automata_data->gTempModWolny  = receive_data.DS_2_WOLNY;
					}else
					{
						automata_data->gTempModWolny  = 0.00;
					}
							
				}
				break;
			}
 	}
}

void Automata::Pause()
{
	previous_status_code = status_code;
	status_code = SCODE_PAUSE;
	SavePowerState();
	SaveValvesState();
	timex->pause();
	Timer_G1 = false;
	//file_print_log("Automat: Pauza !!!\r\n");
}

void Automata::Resume()
{
	status_code = previous_status_code;

	//file_print_log("Automat: Wznowienie !!!\r\n");

	if(status_code == SCODE_NIC || status_code == SCODE_KONIEC)
		return;

	RestorePowerState();
	RestoreValvesState();
	timex->resume();
	
	if(automata_data->gPWR_MOD == true)
	{
			switch(status_code)
			{
				  case STABILIZACJA: 
				 {
					 Timer_G1 = true;
				 }
				 break;
				 
				  case PRZEDGON: 
				 {
					 Timer_G1 = true;
				 }
				 break;
				 
				 case STAB_PRZEDGONU: 
				 {
					 Timer_G1 = true;
				 }
				 break;
				 
				 case ODBIOR_GONU: 
				 {
					 Timer_G1 = true;
				 }
				 break;
			}
    }
}

//This is working function which must be executed inside main sketch loop 
void Automata::Run()
{
	ModuleUpdate();  // odczyt danych od modułu
	
	
	if(Timer_G1 == true)
	{
		Grupowe_Run();
	}
	

	switch(status_code)
	 {
		 case TERMOSTAT:
		 {
				ETAP_0_TERMOSTAT();
		 }
		 break;
		 
		 case ROZGRZEWANIE:
		 {
				ETAP_1_ROZGRZEWANIE();
		 }
		 break;
		 
		 case ZALEWANIE:
		 {
				ETAP_2_ZALEWANIE();
		 }
		 break;
		 
		 case STABILIZACJA:
		 {
				ETAP_3_STABILIZACJA();
		 }
		 break;
		 
		 case PRZEDGON:
		 {
				ETAP_4_PRZEDGON();
		 }
		 break;
		 case STAB_PRZEDGONU: 
		 {
				ETAP_5_STAB_PRZEDGON();
		 }
		 break;
		 
		 case PLUKANIE_OLM:
		 {
				ETAP_6_PLUKANIE_OLM();
		 }
		 break;
		 
		 case ODBIOR_GONU: // Z buforem lub bez:
		 {
			    if(automata_data->Bufor == true)
			    {
					Zawor_Krok_bufor();
					ETAP_7_ODBIOR_GONU_BUFOR();
				}else
				{
					Zawor_Krok();
					ETAP_7_ODBIOR_GONU();
				}
		 }
		 break;
		 
		 case ODBIOR_POGONU:
		 {
				ETAP_8_ODBIOR_POGONU();
		 }
		 break;
		 
		 case CHLODZENIE_GLOWICY:
		 {
				ETAP_9_CHLODZENIE_GLOWICY();
		 }
		 break;
		 
		  case SCODE_NIC:  // nic nie robienie czekamy aż naciśnie guzik
		 {
			
		 }
		 break;
		 
		  case SCODE_KONIEC:  // nic nie robienie koniec
		 {
			
		 }
		 break;
		 
		 case SCODE_PAUSE:  // nic nie robienie pauza
		 {
			
		 }
		 break;

		 case SCODE_ALARM_ZALANIA:  // nic nie robienie pauza
		 {
			
		 }
		 break;

		 
	 }
}

int Automata::Status()
{
	return status_code;
} 

void Automata::TermostatStart()
{
	timex->reset();
	timex->start();
	status_code = TERMOSTAT;
	//file_print_log("Automat > Termostat > START");
}

// czy grzałki mają się wyłączyć ???:
void Automata::TermostatStop()
{
	if(status_code != TERMOSTAT) return;
	timex->stop();
	timex->reset();
	status_code = SCODE_NIC;
	WszystkieGrzalki_OFF();
	//timex->save(TIMEX_SLOT_TERMOSTAT);
	//file_print_log("Termostat > STOP");
}

void Automata::ETAP_0_TERMOSTAT()
{
	if(automata_data->gTempKolumna > automata_data->gTempTermostat_stop)
	{
		WszystkieGrzalki_OFF();			 			    
	}
	      
	if(automata_data->gTempKolumna < automata_data->gTempTermostat_start)
	{
		WszystkieGrzalki_ON();	
	}
}

void Automata::Start() // Zaczynamy od rozgrzewania etap 1
{
	przerwa_zalania = false;
	przekroczenie_cisnienia = false;
	koniec = false;
	
	//alarm_zalania = false;
	start_zalania_alarm = 0;
	
	czas_przerwy_zalania = 0;
	koniec_przerwy_zalania = 0;
	
	CzasZakGReaded = false;
	
	AktualnyCyklPrzedgonu = 1;
	CyklPrzedgonuZawor  = false;
	
	czas_otw_ezglowica = 0;
	num_otw_ezglowica = 0;
	
	czas_zam_ezglowica = 0;
	num_zam_ezglowica = 0;
	
	pwr_on = 0;
	pwr_off = 0;
	oldtime = 0;
	
	bufor_speed  = false;
	bufor_speed_num = 1;
		
	previous_status_code = 0;
	
	automata_data->gKrok = 0;
	automata_data->gETAP = 1;
	automata_data->gTempDnia = 0.0;
	automata_data->gAktZalanie = 0;
	automata_data->gCisnienieDnia = 0.0;
	automata_data->gPresureBMP = 0;

	ClearAllValvesState();
	ClearPowerState();

	WszystkieGrzalki_ON();
			
	Timer_G1 = false;
	
	timex->resetA();
	timex->reset();
	timex->start();
	
	
	status_code = ROZGRZEWANIE;
	
	//Serial.println("Rozpoczynam rozgrzewanie...");

	//file_print_log("ROZGRZEWANIE -> START\r\n");
}

void Automata::Stop()
{
	przerwa_zalania = false;
	przekroczenie_cisnienia = false;
	koniec = false;

	//alarm_zalania = false;
	start_zalania_alarm = 0;
	
	czas_przerwy_zalania = 0;
	koniec_przerwy_zalania = 0;
	
	CzasZakGReaded = false;
	
	AktualnyCyklPrzedgonu = 1;
	CyklPrzedgonuZawor  = false;
	
	czas_otw_ezglowica = 0;
	num_otw_ezglowica = 0;
	
	czas_zam_ezglowica = 0;
	num_zam_ezglowica = 0;
	
	pwr_on = 0;
	pwr_off = 0;
	oldtime = 0;
	
	bufor_speed  = false;
	bufor_speed_num = 1;
		
	previous_status_code = 0;
	
	WszystkieGrzalki_OFF();
	
	// wody nie zamykamy:
	Zawor_OFF(EZ_GON);
	Zawor_OFF(EZ_GLOWICA);
	Zawor_OFF(EZ_PLUK_OLM);

	timer_buzzer->stop();
    timer_ZGON->stop();
	
	status_code = SCODE_NIC;
	
	automata_data->gKrok = 0;
	tCzasZakG = 0;
	PrzekroczenieTmpKolumna = false;
	
	timex->stop();
	timex->reset();
	timex->resetA();
	
	
	Timer_G1 = false;
		
	//file_print_log("Automat: STOP!\r\n");

}

void Automata::StopAutoAlarmZalania() // Zaczynamy od rozgrzewania etap 1
{
	przerwa_zalania = false;
	przekroczenie_cisnienia = false;
	koniec = false;

	start_zalania_alarm = 0;
	
	czas_przerwy_zalania = 0;
	koniec_przerwy_zalania = 0;
	
	CzasZakGReaded = false;
	
	AktualnyCyklPrzedgonu = 1;
	CyklPrzedgonuZawor  = false;
	
	czas_otw_ezglowica = 0;
	num_otw_ezglowica = 0;
	
	czas_zam_ezglowica = 0;
	num_zam_ezglowica = 0;
	
	pwr_on = 0;
	pwr_off = 0;
	oldtime = 0;
	
	bufor_speed  = false;
	bufor_speed_num = 1;
		
	previous_status_code = 0;
	
	WszystkieGrzalki_OFF();
	
	// wody nie zamykamy:
	Zawor_OFF(EZ_GON);
	Zawor_OFF(EZ_GLOWICA);
	Zawor_OFF(EZ_PLUK_OLM);

	timer_buzzer->stop();
    timer_ZGON->stop();
	
	status_code = SCODE_ALARM_ZALANIA;
	
	automata_data->gKrok = 0;
	tCzasZakG = 0;
	PrzekroczenieTmpKolumna = false;
	
	timex->stop();
	timex->reset();
	timex->resetA();

		
	Timer_G1 = false;

	websocket->broadcastTXT("#al_zal_start#");
		
	//file_print_log("Automat: STOP AZ!\r\n");
}

void Automata::ReStart()
{
	przerwa_zalania = false;
	przekroczenie_cisnienia = false;
	previous_status_code = 0;
	koniec = false;

	//alarm_zalania = false;
	
	pwr_on = 0;
	pwr_off = 0;
	
	bufor_speed  =false;
	bufor_speed_num = 1;

	AktualnyCyklPrzedgonu = 1;
	CyklPrzedgonuZawor  = false;
	
	Timer_G1 = false;
	WszystkieGrzalki_ON();
	status_code = ROZGRZEWANIE;
	automata_data->gETAP = 1;
	timex->reset();

	//file_print_log("Automat: RESTART!\r\n");
}


// tutaj użyto sprzętowego timera z esp8266
void volatile Automata::UpdateTime()  //must be run every second 
{
	if(status_code != SCODE_PAUSE || SCODE_NIC || SCODE_KONIEC)
	{
		timex->update();
		automata_data->gCzasProcesuSek = timex->getCurrentSecond();
		automata_data->gCzasProcesuMin = timex->getCurrentMinute();
		automata_data->gCzasProcesuGodz = timex->getCurrentHour();
		
		// All time
		automata_data->gCzasStartSek = timex->getCurrentSecondA();
		automata_data->gCzasStartMin = timex->getCurrentMinuteA();
		automata_data->gCzasStartGodz = timex->getCurrentHourA();
	}
}

void Automata::AskModule()  //must be run every second 
{
	send_data.module_action_code = MC_OK; // every second ask for more data from module
    ETout.sendData();
}

bool Automata::AlarmZalaniaStatus()
{
	if(this->status_code == SCODE_ALARM_ZALANIA)
	{
		return true;
	}else
	{
		return false;
	}
}

void Automata::AlarmZalaniaStop()
{
	//this->alarm_zalania = false;
	this->status_code = SCODE_NIC;
}

void Automata::AlarmZalania()
{
	     // ALARM ZALANIA -> Czy jest czujnik zalania:
		 if(automata_data->gCzujnikZalania == true)
		 {
			    // Czy nastąpiło zalanie ...
				if(automata_data->AnalogSensorValue > automata_data->gWartoscZalania)
				{
						// Zalało kolumnę:
						if(start_zalania_alarm == 0)
						{
							start_zalania_alarm = timex->getPassedSecondsA();
						}else if(start_zalania_alarm > 0)
						{
							    // czy upłyneło 10 sek wtedy ALARM:
								if(timex->getPassedSecondsA() > start_zalania_alarm + 10)
								{
										// Zalało w tym etapie dłużej niż 10 sekund
										// zatrzymujemy, trzeba zmienić wartość 
										// mocy i zacząć od nowa
										// wysłać informację na stronę
										
										//this->alarm_zalania = true;
										//file_print_log("ALARM ZALANIA\r\n");
										StopAutoAlarmZalania();
								}
						}
				  return;
				 }

				 // Zalanie było chwilę ale ustąpiło przed
				 // upływem 10 sek czas rozpoczęcia
				 // alarmu jest zerowany
				 // żeby można było go od nowa mierzyć: 	
				 if(start_zalania_alarm > 0)
				 {
					start_zalania_alarm = 0;
				 }

				 
		 }
}

void Automata::ETAP_1_ROZGRZEWANIE()
{
// Włączają się wszystkie grzałki! A wyłączają się po przekroczeniu zadanej temperatury (wyberanej w 
// menu) Termometr "KOLUMNA" + załacza się EZ-Woda i przechodzi do następnego etapu. 
// Grzałka 1 ON
// Grzałka 2 ON
// Grzałka 3 ON
// EZ-WODA  ON   na koniec rozgrzewania ???

	if(automata_data->gTempKolumna > automata_data->gTempStopRozgrzewania)
	{
		// Rozpoczynamy zalewanie etap 2
		status_code = ZALEWANIE;
		przerwa_zalania = false;
		automata_data->gAktZalanie = 1;
	    automata_data->gETAP = 2;
		Zawor_ON(EZ_WODA);
		WszystkieGrzalki_ON();
		//timex->save(TIMEX_SLOT_ROZGRZEWANIE);
		timex->reset();
		Beep(1000);
		//Serial.println("Rozpoczynam zalewanie...");
		//Serial.println("gCzasZalania_1:");
		//Serial.println(automata_data->gCzasZalania_1);
		websocket->broadcastTXT("#zalewanie_start#");
		//file_print_log("Automat: ZALEWANIE --> START\r\n");		 			    
	}
}

void Automata::ETAP_2_ZALEWANIE()
{
// Są 3 zalania każde według osobnych parametrów:
// gAktZalanie: aktualne zalanie 1 2 3
// gPrzerwaZalania_1
// gPrzerwaZalania_2
// gPrzerwaZalania_3
// gCzasZalania_1
// gCzasZalania_2
// gCzasZalania_3
// gCiscienieZalania_1
// gCiscienieZalania_2
// gCiscienieZalania_3
// Włączają się wszystkie grzałki na [ON] a wyłączą się po przekroczeniu 
// wartości ciśnienia (czujnik analogowy) ale w tym samym czasie odlicza zegar i jeżeli mine 8 minut a 
// ciśnienie nie przekroczy zadanego to przechodzi do przerwy z zalaniu (Wybrany czas w opcjach).

	
	if(przerwa_zalania == true)
	{
				 uint16_t   FloodBreak = 0; 
				 if(automata_data->gAktZalanie == 1)
				 {
					FloodBreak = automata_data->gPrzerwaZalania_1; // było * 60
				 }else if(automata_data->gAktZalanie == 2)
				 {
					FloodBreak= automata_data->gPrzerwaZalania_2; // było * 60
			     }else if(automata_data->gAktZalanie == 3)
				 {
					FloodBreak = automata_data->gPrzerwaZalania_3; // było * 60
				 }
			
				// czas rozpoczęcia przerwy zalania w sekundach
				// int przerwa_zal_seconds = timex->time_base[TIMEX_SLOT_ZALEWANIE_MEM].hour*3600 + 
														  //timex->time_base[TIMEX_SLOT_ZALEWANIE_MEM].minute*60 + 
														  //timex->time_base[TIMEX_SLOT_ZALEWANIE_MEM].second; 
													   
				// sprawdzenie czy nie upłyneła zadana  z menu przerwa w zalaniu
				// dla każdego zalania można ustawić dowolną przerwę				
				
												   
				if(timex->getPassedSeconds() >= czas_przerwy_zalania + FloodBreak)
				{
					// koniec przerwy
					przerwa_zalania = false;
					automata_data->gAktZalanie++;
					automata_data->gPrzerwaWzalaniu = false;
					WszystkieGrzalki_ON(); //  ???
					koniec_przerwy_zalania = timex->getPassedSeconds();
					//Serial.println("Koniec przerwy zalania");
					//Serial.println("Ponowne zalanie");
					//Serial.println(FloodBreak);
					//Serial.println("FloodBreak");
					//file_print_log("Automat: Koniec przerwy zalania\r\n");
					
					Beep(500);
				}			
		}else // przerwa_zalania = false  -->  brak przerwy: ###################
		{
			    // sprawdzenie kture jest zalanie jak 3 to dalej:
				 if(automata_data->gAktZalanie == 4)
			    {
							 automata_data->gAktZalanie = 3;
							 //timex->save(TIMEX_SLOT_ZALEWANIE);
							 timex->reset();
							 status_code = STABILIZACJA;
							 //start_zalania_alarm = 0;
							 automata_data->gETAP = 3;
							 set_et3_power();
							 websocket->broadcastTXT("#stab_start#");	
							 Beep(1000);
							 //file_print_log("Automat > STABILIZACJA > START\r\n");
							 //Serial.println("Koniec zalewania...");
							 //Serial.println("Rozpoczynam stabilizację..."); 	
				  return;
				  }
			
				 // ustalamy  czas zalania według numeru zalania
				 // bo dla każdego zalania przypisany jest  czas zalania:
				 uint16_t czas_zalania;
				 if(automata_data->gAktZalanie == 1)
				 {
					czas_zalania = automata_data->gCzasZalania_1; // było * 60
				 }else if(automata_data->gAktZalanie == 2)
				 {
					czas_zalania = automata_data->gCzasZalania_2; // było * 60
			     }else if(automata_data->gAktZalanie == 3)
				 {
					czas_zalania = automata_data->gCzasZalania_3; // było * 60
				 }
				 
								 
				 // sprawdzamy czas zalania, jeśli minoł:
				 if(timex->getPassedSeconds() >= czas_zalania + koniec_przerwy_zalania)
				 {
					 WszystkieGrzalki_OFF();
					 automata_data->gPrzerwaWzalaniu = true;
					 przerwa_zalania = true;
					 Beep(200);
					 czas_przerwy_zalania = timex->getPassedSeconds();
					 //Serial.println("Czas zalania upłynoł !");
					 //Serial.println("Przerwa w zalaniu:");
					 //Serial.println(timex->getPassedSeconds());
					 //Serial.println("czas zalania:");
					 //Serial.println(czas_zalania);
					 //timex->save(TIMEX_SLOT_ZALEWANIE_MEM);
					 //file_print_log("Czas zal.minol\r\n");
					 
				 }
				 
				 // Czujnik zalania
				 if(automata_data->gCzujnikZalania == true)
				 {
					// Zalało kolumnę:
					if(automata_data->AnalogSensorValue > automata_data->gWartoscZalania)
					{
						// kończymy zalanie:
						WszystkieGrzalki_OFF();
						automata_data->gPrzerwaWzalaniu = true;
						przerwa_zalania = true;
						Beep(200);
						czas_przerwy_zalania = timex->getPassedSeconds();
						//file_print_log("CZ.Prz.ZALANIA start\r\n");
					}
				 }
	
		}
}

void Automata::ETAP_3_STABILIZACJA()
{
// Grzałki włączą się według wyboru. Wyświetli się czas do końca etapu. Po tym czasie przechodzi do 
// następnego etapu 4.

		 // CZY NASTĄPIŁ ALARM ZALANIA
		AlarmZalania();
		 
		// sprawdzenie czy nie upłynął czas stabilizacji							   
		if(timex->getPassedSeconds() >= automata_data->gCzasStabilizacji*60) // w minutach więć * 60
		{
			//timex->save(TIMEX_SLOT_STABILIZACJA);
	        timex->reset();
			status_code = PRZEDGON;
			set_et4_power();
			automata_data->gETAP = 4;
			//start_zalania_alarm = 0;
			websocket->broadcastTXT("#przedg_start#");
			
			// Serial.println("Rozpoczynam przedgon...");
			// file_print_log("Automat > PRZEDGON > START\r\n");	
			
			// pierwszy cykl przedgonu
			if(automata_data->gCyklePrzedgonu > 0)
			{
				Zawor_ON(EZ_GLOWICA);
				CyklPrzedgonuZawor = true;
			}

			Beep(1000);
		}			
}

void Automata::ETAP_4_PRZEDGON()
{
// [1 cykl przedgonu] EZ-Głowicy otworzy się na wybrany czas i po tym czasie się zamknie na wybrany 
// czas a następnie idzie sobie do następnego cyklu. Jeżeli wybierzemy 0 w ustawieniach cykle przedgonu 
// to przechodzi do następnego etapu. W ostatnim cyklu przechodzi do następnego etapu 5 bez (czas 
// zamknięcia) Moc grzałek w zależności od konfiguracji.
// 1 cykl przedgonu to czas otwarcia i zamknięcia EZ_GLOWICA

	// Jeżeli wybierzemy 0 w ustawieniach cykle przedgonu to przechodzi do następnego etapu
	if(automata_data->gCyklePrzedgonu == 0)
	{
		//timex->save(TIMEX_SLOT_PRZEDGON);
		timex->reset();
		status_code = STAB_PRZEDGONU;
		//start_zalania_alarm = 0;
		automata_data->gETAP = 5;
		Zawor_OFF(EZ_GLOWICA); //ez gon w głowicy
		set_et7_power();
		//Serial.println("Stabilizacja przedgonu...");	
		websocket->broadcastTXT("#stb_przg_start#");
		//file_print_log("Automat > STB.PRZEDGONU > START\r\n");
		Beep(1000);
		return;
	}
	
	 // CZY NASTĄPIŁ ALARM ZALANIA
	AlarmZalania();
					
	if(Zawor(EZ_GLOWICA) == true)
	{
		if(timex->getPassedSeconds() == czas_otw_ezglowica + automata_data->gCzasOtwarciaZPrzedgonu) // było * 60
		{
			Zawor_OFF(EZ_GLOWICA);
			czas_zam_ezglowica = timex->getPassedSeconds();
			num_otw_ezglowica++;
			Beep(300);
     	}
    }else
	{
		if(timex->getPassedSeconds() == czas_zam_ezglowica + automata_data->gCzasZamknieciaZPrzedgonu) // było * 60
		{
			Zawor_ON(EZ_GLOWICA);
			czas_otw_ezglowica = timex->getPassedSeconds();
			num_zam_ezglowica++;
			Beep(300); 
		}			
	}
		
	if(automata_data->gCyklePrzedgonu == num_otw_ezglowica && num_zam_ezglowica)
	{
		//timex->save(TIMEX_SLOT_PRZEDGON);
		timex->reset();
		status_code = STAB_PRZEDGONU;
		//start_zalania_alarm = 0;
		automata_data->gETAP = 5;
		Zawor_OFF(EZ_GLOWICA);
		set_et7_power();
		websocket->broadcastTXT("#stb_przg_start#");
		//file_print_log("Automat > STB.PRZEDGONU > START\r\n");
		Beep(1000);	
	}
		
}

// moc z etapu 7
void Automata::ETAP_5_STAB_PRZEDGON()
{
// Po wybranym czasie odczytuje/zapisuje (temperaturę dnoia z termometru KOLUMNA) i przechodzi do 
// następnego etapu.

	// CZY NASTĄPIŁ ALARM ZALANIA
	AlarmZalania();

	if(timex->getPassedSeconds() >= automata_data->gCzasStabilizacjiPrzedgonu * 60)
	{
			//timex->save(TIMEX_SLOT_STAB_PRZEDGONU);
	        timex->reset();
			status_code = PLUKANIE_OLM;
			//start_zalania_alarm = 0;
			automata_data->gETAP = 6;
			TempDnia();
			Zawor_ON(EZ_PLUK_OLM);
			websocket->broadcastTXT("#olm_start#");
			//file_print_log("Automat > P.OLM > START\r\n");
			Beep(1000);	
	}			
}

void Automata::ETAP_6_PLUKANIE_OLM()
{
// EZ-Płukanie OLM otwiera się na wybrany czas i zamyka po zakończeniu liczenia. Po wybranym czasie 
// przechodzi do następnego etapu. Czas (0) spowoduje pominięcie płukania olm.

	// CZY NASTĄPIŁ ALARM ZALANIA
	AlarmZalania();

	 if(automata_data->gCzasPlukanieOLM == 0)
	 {
				//timex->save(TIMEX_SLOT_PLUKANIE_OLM);
				timex->reset();
				status_code = ODBIOR_GONU;
				//start_zalania_alarm = 0;
				automata_data->gETAP = 7;
				automata_data->gKrok = 0;
				zawor_krok_setup = true;
				zawor_close_time_min = 0;
				SpadekTempKolumna = false;
				//set_et7_power();
				//timer_ZGON->restart();
				Zawor_ON(EZ_GON);
				Zawor_OFF(EZ_PLUK_OLM);
				websocket->broadcastTXT("#gon_start#");
				//file_print_log("Automat > ODB.GON > START\r\n");
				Beep(1000);	
	 }else if(timex->getPassedSeconds() >= automata_data->gCzasPlukanieOLM)
	 {
				//timex->save(TIMEX_SLOT_PLUKANIE_OLM);
				timex->reset();
				status_code = ODBIOR_GONU;
				//start_zalania_alarm = 0;
				automata_data->gETAP = 7;
				zawor_krok_setup = true;
				zawor_close_time_min = 0;
				SpadekTempKolumna = false;
				//set_et7_power();
				//timer_ZGON->restart();
				Zawor_OFF(EZ_PLUK_OLM);
				Zawor_ON(EZ_GON);
				websocket->broadcastTXT("#gon_start#");
				//file_print_log("Automat > ODB.GON > START\r\n");
				Beep(1000);	
	 }		
}

// ************************
// *Cezary 🅳🆄🅿🅰* ODB.Destylatu 7  *
// ************************
//Odbiór destylatu:

// EZ-ON

// Czeka na wzrost temp

// Jeżeli temp. kolumna będzie większa od temp. dnia + temp. histerezy zamknięcia to elektrozawór 
// się zamyka. 

// Następnie: 1 krok
// Jeżeli temp. kolumna (spadnie) będzie mniejsza od temp. dnia + histereza otwarcia 
// to przechodzi do pierwszego kroku (8 sec. otwarty 2 sec. zamknięty).

// Następnie:
// Jeżeli temp. kolumna będzie większa od temp. dnia + temp. histerezy zamknięcia 
// to kroki przestają dzialać elektrozawór się zamyka. 

// Następnie: 2 krok
// Jeżeli temp. kolumna (spadnie) będzie mniejsza od temp. dnia + histereza otwarcia 
//  to przechodzi do drugiego kroku (8 sec. otwarty 2 sec. zamknięty).

// Następnie:
// Jeżeli temp. kolumna będzie większa od temp. dnia + temp. histerezy zamknięcia 
// to kroki przestają dzialać elektrozawór się zamyka. 

// Następnie: 3 krok
// Jeżeli temp. kolumna (spadnie) będzie mniejsza od temp. dnia + histereza otwarcia 
//  to przechodzi do trzeciego kroku (6 sec. otwarty 4 sec. zamknięty).

// Następnie:
// Jeżeli temp. kolumna będzie większa od temp. dnia + temp. histerezy zamknięcia
// to kroki przestają dzialać elektrozawór się zamyka. 

// Następnie: 4 krok
// Jeżeli temp. kolumna (spadnie) będzie mniejsza od temp. dnia + histereza otwarcia
// to przechodzi do czwartego kroku (4 sec. otwarty 6 sec. zamknięty).

// Następnie:
// Jeżeli temp. kolumna będzie większa od temp. dnia + temp. histerezy zamknięcia
// to kroki przestają dzialać elektrozawór się zamyka. 

// Następnie:  5 krok
// Jeżeli temp. kolumna (spadnie) będzie mniejsza od temp. dnia  + histereza otwarcia 
// to przechodzi do piątego kroku (2 sec. otwarty 8 sec. zamknięty).

// Następnie: 
// Jeżeli temp. kolumna będzie większa od temp. dnia + temp. histerezy zamknięcia 
//  to kroki przestają dzialać elektrozawór się zamyka. 

// Następnie: 
// Zostaje odliczany wybrany czas i jeżeli  w tym czasie temp.kolumna będzie mniejsza od
// temp. dnia + histereza otwarcia  w wybranym czasie to przechodzi do nastepnego etapu.

//  A -  poza tym gCzasZakonczeniaGon
//  Jeżeli temp. kolumna (spadnie) będzie mniejsza od temp. dnia  + histereza 
//  otwarcia to pozostaje w kroku piątym (2 sec. otwarty 8 sec. zamknięty) 

// odlicza czas ale jezeli czas nie zostanie odliczony do końca
// a temperatura na kolumnie spadnie poniżej temp dnia + tem histereza
// otwarcia to przechodzi do kroku 5


void Automata::ETAP_7_ODBIOR_GONU()
{
	
	// CZY NASTĄPIŁ ALARM ZALANIA
	AlarmZalania();
	
	// czy przekroczono: temp.kolumna > temp.dnia + H zamkniecia
	if(automata_data->gTempKolumna > 
	   automata_data->gTempDnia + automata_data->gHisterezaG_close 
	   && PrzekroczenieTmpKolumna == false)
	{
			//Serial.println("Tmp.kolumna przekroczyła tmp.dnia + hist.zamkniecia zaworu.");
			//file_print_log("Tmp.kolumna przekroczyła tmp.dnia + hist.zamkniecia zaworu.\r\n");
			timer_ZGON->stop();
			Zawor_OFF(EZ_GON);
			PrzekroczenieTmpKolumna = true;
	}
	
	
	// czekamy temp na kolumnie < tmp.dnia +  histereza otwarcia
	if(PrzekroczenieTmpKolumna == true)
	{
		if(automata_data->gTempKolumna < automata_data->gTempDnia + automata_data->gHisterezaG_open )
		{
			// krok mniejszy jak 5:
			if(automata_data->gKrok < 5)
			{
				Beep(500);	
			    //Serial.println("Krok:");
				//Serial.println(automata_data->gKrok);
				//file_print_log("Automat > KROK: " + String(automata_data->gKrok) + "\r\n");
				automata_data->gKrok++;
				timer_ZGON->restart();
				PrzekroczenieTmpKolumna = false;
			}
		}
		
		// ostatni krok odliczamy czas zakonczenia:
		//if(automata_data->gKrok == 5)
		//{
				if(tCzasZakG == 0)
				{
					tCzasZakG = 111;
					timex->reset();
					Beep(1000);	
				}
				
				// sprawdzenie czy nie upłynoł czas zakonczenia gon					   
				if(timex->getPassedSeconds() >= automata_data->gCzasZakonczeniaGon*60)
				{
						//timex->save(TIMEX_SLOT_ODBIOR_GONU);
						timex->reset();
						status_code = ODBIOR_POGONU;
						//start_zalania_alarm = 0;
						automata_data->gETAP = 8;
						Zawor_ON(EZ_GLOWICA);
						Zawor_OFF(EZ_GON);
						timer_ZGON->stop();
						websocket->broadcastTXT("#pogon_start#");
						//file_print_log("Automat > ODB.PO.GON > START\r\n");
					    Beep(1000);	
				}else if(timex->getPassedSeconds()  <= automata_data->gCzasZakonczeniaGon*60)
				{
						if(automata_data->gTempKolumna < automata_data->gTempDnia + automata_data->gHisterezaG_open )
						{
							      PrzekroczenieTmpKolumna = false;
							      tCzasZakG = 0;
							      timer_ZGON->restart();
							      Beep(500);
						}
				}
	
			//} 
				
	}
}



//  Odbiór destylatu z użyciem buforu.
void Automata::ETAP_7_ODBIOR_GONU_BUFOR()
{
	//  Pierwszy warunek:
    //  Zamknięcie EZ-OLM. Jeżeli temp. na kolumnie przekroczy temp dnia + histereza
    //  zamknięcia to  elektrozawór OLM się zamknie.
    //  Jeżeli jest w „speed” to „speed” musi się zatrzymać.
    
    // CZY NASTĄPIŁ ALARM ZALANIA
	AlarmZalania();
    
	if(automata_data->gTempKolumna > 
	   automata_data->gTempDnia + automata_data->gHisterezaG_close 
	   && PrzekroczenieTmpKolumna == false)
	{
			//Serial.println("Tmp.kolumna przekroczyła tmp.dnia + hist.zamkniecia zaworu.");
			//file_print_log("Tmp.kolumna przekroczyła tmp.dnia + hist.zamkniecia zaworu.\r\n");
							
			if(bufor_speed == true)
			{
				timer_ZGON->stop();
				//bufor_speed = false;
				
			}
			Zawor_OFF(EZ_GON);
			PrzekroczenieTmpKolumna = true;
	}
	
	
	// Jeżeli nie jest w SPEED:
	//if(bufor_speed == false)
	//{
		   //  Trzy spowolnienia odbioru destylatu
		   //  Załączają się po przekroczeniu wybranej z menu temperatury na
		   //  buforze: TBufor80 TBufor60 TBufor40
		   switch(bufor_speed_num)
		   {
			        //  Jak przekroczy na buforze zadaną z menu 
					//  temperaturę to załączy się SPEED I
			        //  18 sec. otwarty 4 sec. Zamknięty. „Komunikat” Speed 80%
					//  Wybrana temp. na buforze np. 80st.
					case	 1: 
					{
						if(automata_data->gTempModBufor > automata_data->TBufor80)
						{
							// KROK  I -> START
							Beep(500);
							bufor_speed = true;
							timer_ZGON->restart();
							//Serial.println("Speed: ");
							//Serial.println(automata_data->gKrok);
							//file_print_log("Automat > Speed: " + String(automata_data->gKrok) + "\r\n");
							
							bufor_speed_num = 2; // 2
						}
					}
					break;
					
					 //  Jak przekroczy na buforze zadaną z menu 
					 //  temperaturę to załączy się SPEED II
					 //  12 sec. otwarty 8 sec. Zamknięty. „Komunikat” Speed 60%
					 //  Wybrana temp. na buforze np. 85st.
					case	 2:
					{
						if(automata_data->gTempModBufor > automata_data->TBufor60)
						{
							Beep(500);	
							bufor_speed = true;
							automata_data->gKrok = 2; 
							timer_ZGON->restart();
							//Serial.println("Speed: ");
							//Serial.println(automata_data->gKrok);
							//file_print_log("Automat > Speed: " + String(automata_data->gKrok) + "\r\n");
							bufor_speed_num = 3;
						}
					}
					break;
					
					//  Jak przekroczy na buforze zadaną z menu 
					//  temperaturę to załączy się SPEED III
					//  4 sec. otwarty 6 sec. Zamknięty. „Komunikat” Speed 40%
					//  Wybrana temp. na buforze np. 90st.
					case	 3: 
					{
						if(automata_data->gTempModBufor > automata_data->TBufor40)
						{
							Beep(500);	
							bufor_speed = true;
							automata_data->gKrok = 3; 
							timer_ZGON->restart();
							//Serial.println("Speed: ");
							//Serial.println(automata_data->gKrok);
							//file_print_log("Automat > Speed: " + String(automata_data->gKrok) + "\r\n");
							bufor_speed_num = 4; 
							// koniec sprawdzania temp bufor !
						}
					}
					break;
			}
	

	//}
		
	// spadek temperatury ...
	// czekamy temp na kolumnie < tmp.dnia +  histereza otwarcia
	if(PrzekroczenieTmpKolumna == true)
	{
		    // Otwarcie EZ-OLM.
		    // Jeżeli temp. na kolumnie będzie mniejsza od temp. Dnia + histereza
		    // otwarcia to EZ-OLM się otworzy a jeżeli był w „speed”
		    // to powraca do poprzedniego „speed”.
			if(automata_data->gTempKolumna < automata_data->gTempDnia + automata_data->gHisterezaG_open )
			{
					// powraca do poprzedniego speed kroku
					if(bufor_speed == true)
					{
						 switch(automata_data->gKrok)
						{
								case	 1: // SPEED I kork 1
								{
								}
								break;
								case	 2: // SPEED II krok 2
								{
								}
								break;
								case	 3: // SPEED III krok 3
								{
								}
								break;
						}
					}else
					{
						Zawor_ON(EZ_GON);
				    }
					tCzasZakG = 0;
					PrzekroczenieTmpKolumna = false;
			}
			
			// zresetowanie czasu przy pierwszym wejściu
			if(tCzasZakG == 0)
			{
					tCzasZakG = 111;
					timex->reset();
					Beep(1000);	
		    }
				
			
				//  Zakończenie etapu:
				//  Jeżeli nie ustabilizuje się po wybranym czasie
				//  np. 10 min to przechodzi do następnego etapu.
				//  jezeli ez nie otworzy przez te 10 minut koniec etapu			   
				if(timex->getPassedSeconds() >= automata_data->gCzasZakonczeniaGon*60)
				{
					    // START > POGON 
						//timex->save(TIMEX_SLOT_ODBIOR_GONU);
						timex->reset();
						status_code = ODBIOR_POGONU;
						//start_zalania_alarm = 0;
						automata_data->gETAP = 8;
						Zawor_ON(EZ_GLOWICA);
						Zawor_OFF(EZ_GON);
						timer_ZGON->stop();
						websocket->broadcastTXT("#pogon_start#");
						//file_print_log("Automat > ODB.PO.GON > START\r\n");
					    Beep(1000);	
				}else if(timex->getPassedSeconds()  <= automata_data->gCzasZakonczeniaGon*60)
				{
						// w tym czasie:
				}
	
			
	}// if PrzekroczenieTmpKolumna == true
	
		
}



void Automata::ETAP_8_ODBIOR_POGONU()
{
// EZ-Głowicy Otwiera się a zamyka po przekroczeniu wybranej temperatury i przechodzi do następnego 
// etapu. Moc grzania opcjonalnie.
// [tem.] temperatura z czujnika kolumna.

	// CZY NASTĄPIŁ ALARM ZALANIA
	AlarmZalania();

	if(automata_data->gTempKolumna > automata_data->gTempStopPogonu)
	{
			//timex->save(TIMEX_SLOT_ODBIOR_POGONU);
			timex->reset();
			status_code = CHLODZENIE_GLOWICY;
			automata_data->gETAP = 9;
			Zawor_OFF(EZ_GLOWICA);
			Grupowe_OFF_G1();
			WszystkieGrzalki_OFF();
			websocket->broadcastTXT("#chlodz_start#");
			//file_print_log("Automat > CHLO.GLO > START\r\n");
			Beep(1000);	
	}
}

void Automata::ETAP_9_CHLODZENIE_GLOWICY()
{
    // Grzałki się wyłączają a EZ-Woda jest otwary na określony czas po tym czasie jest KONIEC PROCESU
	if(timex->getPassedSeconds() >= automata_data->gCzasChlodzeniaGlow*60)
	{
			//timex->save(TIMEX_SLOT_CHLODZENIE_GLOWICY);
	        //RestartTime();
	        timex->stop();
	        Zawor_OFF(EZ_WODA);
			status_code = SCODE_KONIEC;
			automata_data->gETAP = 10;
			websocket->broadcastTXT("#auto_end#");
			//file_print_log("Automat > ZAKONCZONO\r\n");
			Beep(1000);	
	}		
}

bool Automata::Zawor(int valve)
{
	switch(valve)
	 {
		 case EZ_WODA:
		 {
				if(	automata_data->gZaworWoda == true)
				{
					return true;
				}else
				{
					return false;
				}
		 }
		 break;
		 
		 case EZ_GON:
		 {
				if(	automata_data->gZaworGonu == true)
				{
					return true;
				}else
				{
					return false;
				}
		 }
		 break;
		 
		 case EZ_GLOWICA:
		 {
				if(	automata_data->gZaworGlowica_m== true)
				{
					return true;
				}else
				{
					return false;
				}
		 }
		 break;
		 
		case EZ_PLUK_OLM:
		 {
				if(	automata_data->gZaworPlukanie_OLM_m == true)
				{
					return true;
				}else
				{
					return false;
				}
		 }
		 break;
	 }
	 
	 return false;
}

 void Automata::ModuleStartWifi()
 {
	 send_data.module_action_code = MC_UPDATE_ENABLE;
     ETout.sendData();
 }  

void Automata::Zawor_ON(int valve)
{
	switch(valve)
	 {
		 case EZ_WODA:
		 {
				extender->digitalWrite(valve,LOW);
				automata_data->gZaworWoda = true;
				//Serial.println(F("Zawor Woda ON"));
				//file_print_log("Automat: Z.WOD ON\r\n");		 
		 }
		 break;
		 case EZ_GON:
		 {
				extender->digitalWrite(valve,LOW);
				automata_data->gZaworGonu = true;
				automata_data->gZaworGonCntr++;
				//Serial.println(F("Zawor Gon ON"));
				//file_print_log("Automat: Z.G ON\r\n");	
		 }
		 break;
		 case EZ_GLOWICA:
		 {
			automata_data->gZaworGlowica_m = true; 
			send_data.module_action_code = MC_EZ_GLOWICA_ON;
			ETout.sendData();
			//Serial.println(F("Zawor Glowica m ON"));
			//file_print_log("Automat: Z.GLO M ON\r\n");
		 }
		 break;
		 case EZ_PLUK_OLM:
		 {
			automata_data->gZaworPlukanie_OLM_m = true; 
			send_data.module_action_code = MC_EZ_PLUK_OLM_ON;
			//Serial.println(F("Zawor PlukanieOLM m ON"));
			ETout.sendData();
			//file_print_log("Automat: Z.POLM M ON\r\n");
		 }
		 break;
	 }

}

void Automata::Zawor_OFF(int valve)
{
	switch(valve)
	 {
		 case EZ_WODA:
		 {
				extender->digitalWrite(valve,HIGH);
				automata_data->gZaworWoda = false;
				//Serial.println(F("Zawor Woda OFF"));
				//file_print_log("Automat: Z.WOD M OFF\r\n");		 
		 }
		 break;
		 case EZ_GON:
		 {
				extender->digitalWrite(valve,HIGH);
				automata_data->gZaworGonu = false;
				//Serial.println(F("Zawor Gon OFF"));
				//file_print_log("Automat: Gon M OFF\r\n");	
		 }
		 break;
		 case EZ_GLOWICA:
		 {
			automata_data->gZaworGlowica_m = false; 
			send_data.module_action_code = MC_EZ_GLOWICA_OFF;
			ETout.sendData();
			//Serial.println(F("Zawor Glowica module OFF"));
			//file_print_log("Automat: Z.GLO M OFF\r\n");
		 }
		 break;
		 case EZ_PLUK_OLM:
		 {
			automata_data->gZaworPlukanie_OLM_m = false; 
			send_data.module_action_code = MC_EZ_PLUK_OLM_OFF;
			//Serial.println(F("Zawor PlukanieOLM module OFF"));
			//file_print_log("Automat: Z.P.OLM M OFF\r\n");
			ETout.sendData();
		 }
		 break;
	 }
}

void Automata::WszystkieGrzalki_ON()
{
	extender->digitalWrite(GRZALKA_1,LOW);
	extender->digitalWrite(GRZALKA_2,LOW);
	extender->digitalWrite(GRZALKA_3,LOW);
	automata_data->G1 = true;
	//Serial.println(F("Grzalka1 ON"));
	automata_data->G2 = true;
	//Serial.println(F("Grzalka2 ON"));
	automata_data->G3 = true;
	//Serial.println(F("Grzalka3 ON"));
	//file_print_log("Automat: ALL G ON\r\n");
}

void Automata::WszystkieGrzalki_OFF()
{
	extender->digitalWrite(GRZALKA_1,HIGH);
	extender->digitalWrite(GRZALKA_2,HIGH);
	extender->digitalWrite(GRZALKA_3,HIGH);
	automata_data->G1 = false;
	//Serial.println(F("Grzalka1 OFF"));
	automata_data->G2 =  false;
	//Serial.println(F("Grzalka2 OFF"));
	automata_data->G3 = false;
	//Serial.println(F("Grzalka3 OFF"));
	//file_print_log("Automat: ALL G OFF\r\n");
}

void Automata::SavePowerState()
{
    power_state.G1 = automata_data->G1;
    power_state.G2 = automata_data->G2;
    power_state.G3 = automata_data->G3;
}

void Automata::SaveValvesState()
{
    save_EZ_WODA = Zawor(EZ_WODA);
    save_EZ_GLOWICA  = Zawor(EZ_GLOWICA);
    save_EZ_PLUK_OLM = Zawor(EZ_PLUK_OLM);
    save_EZ_GON = Zawor(EZ_GON);
}

void Automata::ClearAllValvesState()
{
    save_EZ_WODA = false;
    save_EZ_GLOWICA  = false;
    save_EZ_PLUK_OLM = false;
    save_EZ_GON = false;
}

void Automata::ClearPowerState()
{
    power_state.G1 = false;
    power_state.G2 = false;
    power_state.G3 = false;
}


void Automata::RestoreValvesState()
{
	if(save_EZ_WODA == true)
	{
		Zawor_ON(EZ_WODA);
	}else
	{
		Zawor_OFF(EZ_WODA);
	}
	
	if(save_EZ_GLOWICA == true)
	{
		Zawor_ON(EZ_GLOWICA);
	}else
	{
		Zawor_OFF(EZ_GLOWICA);
	}
	
	if(save_EZ_PLUK_OLM == true)
	{
		Zawor_ON(EZ_PLUK_OLM);
	}else
	{
		Zawor_OFF(EZ_PLUK_OLM);
	}
	
	if(save_EZ_GON == true)
	{
		Zawor_ON(EZ_GON);
	}else
	{
		Zawor_OFF(EZ_GON);
	}

}

void Automata::RestorePowerState()
{
	if(power_state.G1 == true)
	{
		Grzalka_ON(GRZALKA_1);
	}else
	{
		Grzalka_OFF(GRZALKA_1);
	}
	
	if(power_state.G2 == true)
	{
		Grzalka_ON(GRZALKA_2);
	}else
	{
		Grzalka_OFF(GRZALKA_2);
	}
	
	if(power_state.G3 == true)
	{
		Grzalka_ON(GRZALKA_3);
	}else
	{
		Grzalka_OFF(GRZALKA_3);
	}
}

void Automata::Grzalka_ON(int grz)
{
 extender->digitalWrite(grz,LOW);
 
	 switch(grz)
	 {
		 case GRZALKA_1:
		 {
			automata_data->G1 = true;
			//Serial.println(F("Grzalka1 ON"));
			//file_print_log("Automat > G1 ON \r\n");
							
			 
		 }
		 break;
		 case GRZALKA_2:
		 {
			automata_data->G2 = true;
			//Serial.println(F("Grzalka2 ON"));
			//file_print_log("Automat > G2 ON \r\n");
		 }
		 break;
		 case GRZALKA_3:
		 {
			automata_data->G3 = true;
			//Serial.println(F("Grzalka3 ON"));
			//file_print_log("Automat > G3 ON \r\n");
		 }
		 break;
	 }
	  	  
 }
  
 bool Automata::Grzalka(int grz) //LOW state is ON
{
	uint8_t val =  extender->digitalRead(grz) ;
	if (val == LOW) 
	{
      	return true;	
	}else
	{
	    return false;
	}
}

bool Automata::Grzalka1() 
{
	if (automata_data->G1 == true) 
	{
      	return true;	
	}else
	{
	    return false;
	}
}

bool Automata::Grzalka2() 
{
	if (automata_data->G2 == true) 
	{
      	return true;	
	}else
	{
	    return false;
	}
}

bool Automata::Grzalka3() 
{
	if (automata_data->G3 == true) 
	{
      	return true;	
	}else
	{
	    return false;
	}
}
 
 bool Automata::Grzanie_hw()
 {
	if(Grzalka(GRZALKA_1) == true || Grzalka(GRZALKA_2) == true || Grzalka(GRZALKA_3) == true)
	{
		return true;
	}else
	{
		return false;
	}
}

bool Automata::Grzanie_sw()
 {
	if(automata_data->G1 == true || automata_data->G2 == true || automata_data->G3 == true)
	{
		return true;
	}else
	{
		return false;
	}
}
 
void Automata::Grzalka_OFF(int grz)
{
  extender->digitalWrite(grz,HIGH);
  
  switch(grz)
	 {
		 case GRZALKA_1:
		 {
			automata_data->G1 = false;
			//Serial.println(F("Grzalka1 OFF"));
			//file_print_log("Automat > G1 OFF \r\n");
		 }
		 break;
		 case GRZALKA_2:
		 {
			automata_data->G2 =false;
			//Serial.println(F("Grzalka2 OFF"));
			//file_print_log("Automat > G2 OFF \r\n");
		 }
		 break;
		 case GRZALKA_3:
		 {
			automata_data->G3 = false;
			//Serial.println(F("Grzalka3 OFF"));
			//file_print_log("Automat > G3 OFF \r\n");
		 }
		 break;
	 }
}

void Automata::Grupowe_Run()
{
	 // extender->digitalWrite(GRZALKA_1,LOW); // ON
	 // extender->digitalWrite(GRZALKA_1,HIGH); // OFF
	 
	 if(automata_data->G1 == true)
	 {
			//oldtime = millis();
			if ( (millis()-oldtime) > pwr_on)
			{
			   oldtime = millis();
			   extender->digitalWrite(GRZALKA_1,HIGH);
			   automata_data->G1 = false;	
			}
	 }else
	 {
		   // oldtime = millis();
			if ( (millis()-oldtime) > pwr_off)
			{
			   oldtime = millis();
			   extender->digitalWrite(GRZALKA_1,LOW);
			   automata_data->G1 = true;
			}
	 }

}

void Automata::Grupowe_ON_G1()
 {
	  switch(status_code)
	  {
			 case STABILIZACJA:
			 {
				pwr_on =  1000 * automata_data->gStb_G1pwm / 100;
				pwr_off =  1000 - pwr_on;
				Grzalka_ON(GRZALKA_1);
				Timer_G1 = true;
			 }
			 break;
							
			 case  PRZEDGON:  
			 {
				pwr_on =  1000 *  automata_data->gPrzg_G1pwm / 100;
				pwr_off =  1000 - pwr_on;
				Grzalka_ON(GRZALKA_1);
				Timer_G1 = true;
			 }
			 break;
			 
			 case  ODBIOR_GONU:  
			 {
				pwr_on =  1000 *  automata_data->gGon_G1pwm / 100;
				pwr_off =  1000 - pwr_on;
				Grzalka_ON(GRZALKA_1);
				Timer_G1 = true;
			 }
			 break;
			 
			 case  STAB_PRZEDGONU:  
			 {
				pwr_on =  1000 *  automata_data->gGon_G1pwm / 100;
				pwr_off =  1000 - pwr_on;
				Grzalka_ON(GRZALKA_1);
				Timer_G1 = true;
			 }
			 break;
	  }

 }
 
void Automata::Grupowe_OFF_G1()
 {
	 Grzalka_OFF(GRZALKA_1);
	 Timer_G1 = false;
 }

// stabilizacja power:
void Automata::set_et3_power()
{
	// jeśli moduł mocy jest regulowany:
	if(automata_data->gPWR_MOD == true)
	{
		if(automata_data->gStb_G1 == true)
		{
			Grupowe_ON_G1();
		}else
		{
			Grupowe_OFF_G1();
		}
	}else// nie jest:
	{
			if(automata_data->gStb_G1 == true)
			{
				Grzalka_ON(GRZALKA_1);
			}else
			{
				Grzalka_OFF(GRZALKA_1);
			}
	}
	
	if(automata_data->gStb_G2 == true)
	{
		Grzalka_ON(GRZALKA_2);
	}else
	{
	    Grzalka_OFF(GRZALKA_2);
	}
	
	if(automata_data->gStb_G3 == true)
	{
		Grzalka_ON(GRZALKA_3);
	}else
	{
		Grzalka_OFF(GRZALKA_3);
	}
	
}

// przedgon power:
void Automata::set_et4_power()
{
	// jeśli moduł mocy jest regulowany:
	if(automata_data->gPWR_MOD == true)
	{
		if(automata_data->gPrzg_G1 == true)
		{
			Grupowe_ON_G1();
		}else
		{
			Grupowe_OFF_G1();
		}
	}else// nie jest:
	{
		if(automata_data->gPrzg_G1 == true)
		{
			Grzalka_ON(GRZALKA_1);
		}else
		{
			Grzalka_OFF(GRZALKA_1);
		}
	}
	
	if(automata_data->gPrzg_G2 == true)
	{
		Grzalka_ON(GRZALKA_2);
	}else
	{
	    Grzalka_OFF(GRZALKA_2);
	}
	
	if(automata_data->gPrzg_G3 == true)
	{
		Grzalka_ON(GRZALKA_3);
	}else
	{
		Grzalka_OFF(GRZALKA_3);
	}
	
}

// gon power:
void Automata::set_et7_power()
{
	// jeśli moduł mocy jest regulowany:
	if(automata_data->gPWR_MOD == true)
	{
		if(automata_data->gGon_G1 == true)
		{
			Grupowe_ON_G1();
		}else
		{
			Grupowe_OFF_G1();
		}
	}else// nie jest:
	{
		if(automata_data->gGon_G1 == true)
		{
			Grzalka_ON(GRZALKA_1);
		}else
		{
			Grzalka_OFF(GRZALKA_1);
		}
	}
	
	if(automata_data->gGon_G2 == true)
	{
		Grzalka_ON(GRZALKA_2);
	}else
	{
	    Grzalka_OFF(GRZALKA_2);
	}
	
	if(automata_data->gGon_G3 == true)
	{
		Grzalka_ON(GRZALKA_3);
	}else
	{
		Grzalka_OFF(GRZALKA_3);
	}
	
}


void Automata::Zawor_Krok_bufor()
{
	 if(timer_ZGON->onRestart())
    {
		//Serial.print(F("Krok bufor: "));  
		//Serial.print(automata_data->gKrok);	  
		//Serial.println("");
		
		//  Trzy spowolnienia odbioru destylatu.
		//  1) 18 sec. otwarty 4 sec. Zamknięty. „Komunikat” Speed 80%
		//  Wybrana temp. na buforze np. 80st.

		//  2) 12 sec. otwarty 8 sec. Zamknięty. „Komunikat” Speed 60%
		//  Wybrana temp. na buforze np. 85st.

		//  3) 4 sec. otwarty 6 sec. Zamknięty. „Komunikat” Speed 40%
		//  Wybrana temp. na buforze np. 90st.
	  
    switch(automata_data->gKrok)
	{
		  case 0: 
		 {
		 
		 }
		 
		 // krok 1 zawór 18 sek otwarty potem 4 sek zamknięty
		 case 1: 
		 {
			  if(automata_data->gZaworGonu == true)
			  {
				Zawor_OFF(EZ_GON);
	            timer_ZGON->setTimeout(4000);
                timer_ZGON->restart(); 
			  }else
			  {
			    Zawor_ON(EZ_GON);
			    timer_ZGON->setTimeout(18000);
			    timer_ZGON->restart();
			  }
		 }
		 break;
		 
		 
		 // krok 2 zawór 8 sek otwarty potem 12 sek zamknięty
		 case 2: 
		 {  
		     if(automata_data->gZaworGonu == true)
			  {
				Zawor_OFF(EZ_GON); 
	            timer_ZGON->setTimeout(12000);
                timer_ZGON->restart(); 
			  }else
			  {
			    Zawor_ON(EZ_GON);
			    timer_ZGON->setTimeout(8000);
			    timer_ZGON->restart();
			  }
		 }
		 break;
		 		 
		  // krok 3 zawór 4 sek otwarty potem 6 sek zamknięty
		 case 3: 
		 { 
			 if(automata_data->gZaworGonu == true)
			  {
				Zawor_OFF(EZ_GON);  
	            timer_ZGON->setTimeout(6000);
                timer_ZGON->restart(); 
			  }else
			  {
			    Zawor_ON(EZ_GON);
			    timer_ZGON->setTimeout(4000);
			    timer_ZGON->restart();
			  }
		 }
		 break;
	
	}
  }
	
}

void Automata::Zawor_Krok()
{
  if(timer_ZGON->onRestart())
  {
	//Serial.print(F("Krok: "));  
	//Serial.print(automata_data->gKrok);	  
	//Serial.println("");
	  
    switch(automata_data->gKrok)
	{
		 // krok 1 zawór 8 sek otwarty potem 2 sek zamknięty
		 
		  case 0: 
		 {
		 
		 }
		 
		 case 1: 
		 {
			  if(automata_data->gZaworGonu == true)
			  {
				Zawor_OFF(EZ_GON);
	            timer_ZGON->setTimeout(2000);
                timer_ZGON->restart(); 
			  }else
			  {
			    Zawor_ON(EZ_GON);
			    timer_ZGON->setTimeout(8000);
			    timer_ZGON->restart();
			  }
		 }
		 break;
		 
		 
		 // krok 2 zawór 6 sek otwarty potem 4 sek zamknięty
		 case 2: 
		 {  
		     if(automata_data->gZaworGonu == true)
			  {
				Zawor_OFF(EZ_GON); 
	            timer_ZGON->setTimeout(4000);
                timer_ZGON->restart(); 
			  }else
			  {
			     Zawor_ON(EZ_GON);
			    timer_ZGON->setTimeout(6000);
			    timer_ZGON->restart();
			  }
		 }
		 break;
		 		 
		  // krok 3 zawór 4 sek otwarty potem 6 sek zamknięty
		 case 3: 
		 { 
			 if(automata_data->gZaworGonu == true)
			  {
				Zawor_OFF(EZ_GON);  
	            timer_ZGON->setTimeout(6000);
                timer_ZGON->restart(); 
			  }else
			  {
			    Zawor_ON(EZ_GON);
			    timer_ZGON->setTimeout(4000);
			    timer_ZGON->restart();
			  }
		 }
		 break;
		 
		 
		 // krok 4 zawór 2 sek otwarty potem 8 sek zamknięty
		 case 4: 
		 {  
			 if(automata_data->gZaworGonu == true)
			  {
				Zawor_OFF(EZ_GON); 
	            timer_ZGON->setTimeout(8000);
                timer_ZGON->restart(); 
			  }else
			  {
			    Zawor_ON(EZ_GON);
			    timer_ZGON->setTimeout(2000);
			    timer_ZGON->restart();
			  }
		 }
		 break;
		
				 
		 // krok 5 zawór 1 sek otwarty potem 9 sek zamknięty
		 case 5: 
		 {  
			 if(automata_data->gZaworGonu == true)
			  {
				Zawor_OFF(EZ_GON); 
	            timer_ZGON->setTimeout(9000);
                timer_ZGON->restart(); 
			  }else
			  {
			    Zawor_ON(EZ_GON);
			    timer_ZGON->setTimeout(1000);
			    timer_ZGON->restart();
			  }
		 }
		 break;
	}
  }
	
}

  /*  

void Automata::ETAP_2_ZALEWANIE()
{
// Są 3 zalania każde według osobnych parametrów:
// gAktZalanie: aktualne zalanie 1 2 3
// gPrzerwaZalania_1
// gPrzerwaZalania_2
// gPrzerwaZalania_3
// gCzasZalania_1
// gCzasZalania_2
// gCzasZalania_3
// gCiscienieZalania_1
// gCiscienieZalania_2
// gCiscienieZalania_3
// Włączają się wszystkie grzałki na [ON] a wyłączą się po przekroczeniu 
// wartości ciśnienia (czujnik analogowy) ale w tym samym czasie odlicza zegar i jeżeli mine 8 minut a 
// ciśnienie nie przekroczy zadanego to przechodzi do przerwy z zalaniu (Wybrany czas w opcjach).

	switch(przerwa_zalania)
	{
		// przekroczono zadane ciśnienie zalania i następuje przerwa:
		case true:
		{
 				uint16_t     FloodBreak = 0; // zadane ciśnienie
				 if(automata_data->gAktZalanie == 1)
				 {
					FloodBreak = automata_data->gPrzerwaZalania_1*60;
				 }else if(automata_data->gAktZalanie == 2)
				 {
					FloodBreak= automata_data->gPrzerwaZalania_2*60;
			     }else if(automata_data->gAktZalanie == 3)
				 {
					FloodBreak = automata_data->gPrzerwaZalania_3*60;
				 }
			
				// czas rozpoczęcia przerwy zalania w sekundach
				int przerwa_zal_seconds = timex->time_base[TIMEX_SLOT_ZALEWANIE_MEM].hour*3600 + 
														  timex->time_base[TIMEX_SLOT_ZALEWANIE_MEM].minute*60 + 
														  timex->time_base[TIMEX_SLOT_ZALEWANIE_MEM].second; 
													   
				// sprawdzenie czy nie upłyneła zadana  z menu przerwa w zalaniu
				// dla każdego zalania można ustawić dowolną przerwę									   
				if(timex->getPassedSeconds() >= przerwa_zal_seconds + FloodBreak)
				{
					// koniec przerwy
					przerwa_zalania = false;
					WszystkieGrzalki_ON(); //  ???
				}			
		}
		break;
		
		case false:
		{
				 // ustalamy ciśnienie zalania i czas zalania według numeru zalania
				 // bo dla każdego zalania przypisane jest ciśnienie i czas zalania:
				 int32_t      FloodPresure = 0; // zadane ciśnienie
				 uint8_t czas_zalania;
				 if(automata_data->gAktZalanie == 1)
				 {
					FloodPresure = automata_data->gCiscienieZalania_1;
					czas_zalania = automata_data->gCzasZalania_1*60;
				 }else if(automata_data->gAktZalanie == 2)
				 {
					FloodPresure = automata_data->gCiscienieZalania_2;
					czas_zalania = automata_data->gCzasZalania_2*60;
			     }else if(automata_data->gAktZalanie == 3)
				 {
					FloodPresure = automata_data->gCiscienieZalania_3;
					czas_zalania = automata_data->gCzasZalania_3*60;
				 }
				 
				 // gdy nie ma przerwy w zalaniu to sprawdzamy czy przekroczono
				 // ciśnienie zalania jeśli tak przechodzimy na kolejne zalanie
				 if(automata_data->gPresureBMP > FloodPresure)
				 {
					 // nastapilo zalanie
					 automata_data->gAktZalanie++;
					 WszystkieGrzalki_OFF();
					 przekroczenie_cisnienia = true;
					 przerwa_zalania = true;
					 
						 // trzecie ostatnie zalanie nastapiło przechodzimy do kolejnego etapu
						 // czy aby tutaj....
						 if(automata_data->gAktZalanie == 3)
						 {
							 set_et3_power();
							 timex->save(TIMEX_SLOT_ZALEWANIE);
							 RestartTime();
							 status_code = STABILIZACJA;
							 automata_data->gETAP = 3;
						  return;
				          }
				 }// nie przekroczono ciśnienia więć ---> dalej:
				 
				 // sprawdzamy czas zalania, jeśli minoł a nie nastąpiło
				 // zalanie, to zarządzona jest przerwa ale jeśli czas
				 // minoł i nastąpiło zalanie to co:
				  if(timex->getPassedSeconds() > czas_zalania)
				 {
					 //jak nie przekroczyło to przerwa
					 if(przekroczenie_cisnienia == false)
					 {
						przerwa_zalania = true;
						timex->save(TIMEX_SLOT_ZALEWANIE_MEM);
					 }
				 }
				 
				 
		}
		break;
	}
}
*/

/* 
void Automata::ETAP_7_ODBIOR_GONU()
{
// jezeli temperatura na kolumnie przekroczy temp dnia + histereza 
// to przechodzi do 1 go kroku potem ten sam warunek dla kolejnych kroków
// w ostatnim kroku jezeli temp kolumny pzekroczy temp dnia + hist i nie zejdzie ponizej temp dnia + hist

	if(SpadekTempKolumna == false) // nie ma spadku
   { 																  // 1.0	
		if(automata_data->gTempKolumna > automata_data->gTempDnia + automata_data->gHisterezaG_close )
		{
			Zawor_OFF(EZ_GON);
			TempKolumnaValveOFF = automata_data->gTempKolumna;
			zawor_close_time_min = (automata_data->gCzasProcesuGodz * 60) + automata_data->gCzasProcesuMin;
			SpadekTempKolumna   = true;
			Serial.print("Spadek T.Kolumna z: ");
			Serial.print(TempKolumnaValveOFF);
			Serial.print(" o: ");
			Serial.print(automata_data->gHisterezaG_open);
			Serial.print(" C");
			Serial.println("");
			Serial.print("TempKolumnaValveOFF: ");
			Serial.print(TempKolumnaValveOFF);
			Serial.println("");
			timer_ZGON->stop();
		}
   }else
   {															// 0.5
		if(TempKolumnaValveOFF > automata_data->gTempKolumna + automata_data->gHisterezaG_open)
		{
			
			if(zawor_krok_setup == false)
			{
				if(zawor_krok < 4)
			    {
				  zawor_krok++;
			    }
			}
						
			SpadekTempKolumna = false;
			zawor_krok_setup = false;
			timer_ZGON->restart();
			Serial.println(F("Nastepny krok >"));
		}
   }
   
   if(SpadekTempKolumna == true)
   {
	
		int zawor_close_now = (  automata_data->gCzasProcesuGodz * 60)
										 + automata_data->gCzasProcesuMin;
		
		if(zawor_close_now - zawor_close_time_min >=  automata_data->gCzasZakonczeniaGon) 
		{
			
			//if(zawor_krok == 4)  // w ostatnim kroku 0.1.2.3 i > 4 koniec dopiero jak przez 20 minut
                                            // nie otworzy się zawor gon
			
			  timex->save(TIMEX_SLOT_ODBIOR_GONU);
			  RestartTime();
			  status_code = ODBIOR_POGONU;
			  automata_data->gETAP = 8;
			  Zawor_ON(EZ_GLOWICA);
			  Zawor_OFF(EZ_GON);
			  timer_ZGON->stop();
			  
		   //}else
	    //{
			//Serial.println("");
			//Serial.print(F("Zawor nie otworzyl sie przez: "));
			//Serial.print(gParametryPracy.gCzasZakonczenia);
			//Serial.print(F("Krok: "));
			//Serial.print(zawor_krok);
			//Serial.println("");
		//}
		}
  }
}

 */
