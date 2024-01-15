

//******************************************************************
//  "Sprawiła [wreszcie], że wszystkim małym i wielkim, bogatym i ubogim,			*
//  wolnym i niewolnikom wyciśnięto znamię na prawej ręce lub na czole.				*
//  Od tego czasu nikt, kto nie miał znaku imienia Bestii lub liczby jej imienia			*
//  nic już nie mógł ani kupić, ani sprzedać.Oto tu [potrzeba] mądrości!					*
//  Kto czuje się dostatecznie mądrym, niech wyliczy liczbę Bestii.							*
//  Jest to liczba jakiegoś człowieka,																		*
//  a liczbą tą jest sześćset sześćdziesiąt sześć”.														*
//  Apokalipsa 13: 11-18																						*
//******************************************************************

#include "include.h"

// Websocket events
void ON_WS_Event(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
	switch (type)
	{
	case WStype_DISCONNECTED:
	{
		//Serial.printf("[%u] Disconnected!\n", num);
	}
	break;

	case WStype_CONNECTED:
	{
		IPAddress ip = webSocket.remoteIP(num);
		//Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
	}
	break;

	case WStype_TEXT:
	{
		msg = (const char *)payload;
		Proces_WS_message();
	}
	break;

	case WStype_BIN:
	{
		//Serial.printf("[%u] get binary length: %u\n", num, length);
		hexdump(payload, length);
	}
	break;
	}
}

void Beep(int time)
{
	pcf8574.digitalWrite(BUZZER, HIGH);
	timer_buzzer.setTimeout(time);
	timer_buzzer.restart();
}

void EEPROM_RESET()
{
	// gAutomat.Zawor_OFF(EZ_GON);
	// gAutomat.WszystkieGrzalki_OFF();

	disable_lcd_update = true;
	gParametryPracy.gCzasProcesuGodz = 0;
	gParametryPracy.gCzasProcesuMin = 0;
	gParametryPracy.gCzasProcesuSek = 0;
	gParametryPracy.gCzasStartGodz = 0;
	gParametryPracy.gCzasStartMin = 0;
	gParametryPracy.gCzasStartSek = 0;
	gParametryPracy.gESP_CNTR = 0;
	gParametryPracy.gHisterezaG_open = 0.5;	 // st C
	gParametryPracy.gHisterezaG_close = 1.0; // st C
	gParametryPracy.gTempAlarmuBeczka = 105.00;
	gParametryPracy.gTempAlarmuGlowica = 70.00;
	gParametryPracy.gMocGrzaniaG1 = 1500;
	gParametryPracy.gMocGrzaniaG2 = 2000;
	gParametryPracy.gMocGrzaniaG3 = 2500;
	gParametryPracy.G1 = false;
	gParametryPracy.G2 = false;
	gParametryPracy.G3 = false;
	gParametryPracy.gTEMPERATURE_PRECISION = 12;

	gParametryPracy.gZaworGonu = false;
	gParametryPracy.gZaworWoda = false;
	gParametryPracy.gZaworPlukanie_OLM_m = false;
	gParametryPracy.gZaworGlowica_m = false;

	gParametryPracy.gWIFI_MODE = 0;
	gParametryPracy.gZaworGonCntr = 0;
	gParametryPracy.gTempTermostat_stop = 72.0;	 // st C
	gParametryPracy.gTempTermostat_start = 70.0; // st C
	gParametryPracy.gCzasZakonczeniaGon = 20;
	gParametryPracy.gTempBeczka = 0;
	gParametryPracy.gTempKolumna = 0;
	gParametryPracy.gTempGlowica = 0;
	gParametryPracy.gCzasZakonczeniaGon = 20;
	gParametryPracy.gTempModWoda = 0.0;
	gParametryPracy.gTempModBufor = 0.0;
	gParametryPracy.gTempModWolny = 0.0;
	gParametryPracy.gCisnienieZalania_1 = 0.0;
	gParametryPracy.gCisnienieZalania_2 = 0.0;
	gParametryPracy.gCisnienieZalania_3 = 0.0;
	gParametryPracy.gCzasZalania_1 = 120; // sekund
	gParametryPracy.gCzasZalania_2 = 120;
	gParametryPracy.gCzasZalania_3 = 120;
	gParametryPracy.gPrzerwaZalania_1 = 60;
	gParametryPracy.gPrzerwaZalania_2 = 60;
	gParametryPracy.gPrzerwaZalania_3 = 60;
	gParametryPracy.gCzasStabilizacji = 5;
	gParametryPracy.gCzasStabilizacjiPrzedgonu = 5;
	gParametryPracy.gCzasPlukanieOLM = 5;
	gParametryPracy.gCzasChlodzeniaGlow = 5;
	gParametryPracy.gCyklePrzedgonu = 5;
	gParametryPracy.gCzasOtwarciaZPrzedgonu = 60;
	gParametryPracy.gCzasZamknieciaZPrzedgonu = 60;
	gParametryPracy.gAktZalanie = 0;
	gParametryPracy.gCzujnikZalania = false;
	gParametryPracy.gWartoscZalania = 700;
	gParametryPracy.AnalogSensorValue = 0;

	gParametryPracy.gTempStopPogonu = 99.0;
	gParametryPracy.gTempStopRozgrzewania = 80.0;

	gParametryPracy.gPWR_MOD = false;

	gParametryPracy.gCisnienieDnia = 0;
	gParametryPracy.gTempDnia = 0.00;

	gParametryPracy.gStb_G1pwm = 0; // 0-100 %
	gParametryPracy.gStb_G1 = false;
	gParametryPracy.gStb_G2 = false;
	gParametryPracy.gStb_G3 = false;

	gParametryPracy.gPrzg_G1pwm = 0; //  0-100 %
	gParametryPracy.gPrzg_G1 = false;
	gParametryPracy.gPrzg_G2 = false;
	gParametryPracy.gPrzg_G3 = false;

	gParametryPracy.gGon_G1pwm = 0; //  0-100 %
	gParametryPracy.gGon_G1 = false;
	gParametryPracy.gGon_G2 = false;
	gParametryPracy.gGon_G3 = false;

	gParametryPracy.TBufor80 = 0.0;
	gParametryPracy.TBufor60 = 0.0;
	gParametryPracy.TBufor40 = 0.0;
	gParametryPracy.Bufor = false;

	memset(gParametryPracy.gSSID, 0, sizeof(gParametryPracy.gSSID));
	memset(gParametryPracy.gPASS, 0, sizeof(gParametryPracy.gPASS));

	const char *strFrom = "brak";
	// void *memcpy (void* dest, const void* src, size_t size);
	memcpy(gParametryPracy.gSSID, strFrom, 4);
	memcpy(gParametryPracy.gPASS, strFrom, 4);

	memset(gParametryPracy.FileName, 0, sizeof(gParametryPracy.FileName));
	memcpy(gParametryPracy.FileName, "ustawienia fabryczne", 20);

	// Clear EEPROM cells
	for (int i = 0; i < sizeof(AutomataData); EEPROM.write(i++, 0));

	EEPROM.put(0, gParametryPracy);
	EEPROM.commit();
	EEPROM.end();

	//boolean ok = EEPROM.commitReset();
	
	//Serial.println("EEPROM reset...");
	//Serial.println((ok) ? "Save OK" : "Save failed");
	//Serial.println("");

	if ( !LittleFS.format() )
	{
		Serial.println("LittleFS format failed");
	}

	LittleFS.end();
	
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("RESET");
	lcd.setCursor(0, 1);
	lcd.print("Konfiguracji");
	lcd.setCursor(6, 3);
	lcd.print("OK");
	delay(5000);
	lcd.clear();
	disable_lcd_update = false;

	ESP.restart();
}

uint32_t getFlashChipRealSize(void)
{
	return (1 << ((spi_flash_get_id() >> 16) & 0xFF));
}

void setup_eeprom()
{
	EEPROM.begin(sizeof(AutomataData));
	delay(5);
	if (EEPROM.percentUsed() >= 0)
	{
		EEPROM.get(0, gParametryPracy);
		//Serial.println("Loading EEPROM data...");
		//Serial.println("");
		//Serial.print(EEPROM.percentUsed());
		//Serial.println("% of ESP flash space currently used");

		gParametryPracy.gTempDnia = 0.0;
		gParametryPracy.gCisnienieDnia = 0.0;

		gParametryPracy.gCzasProcesuGodz = 0;
		gParametryPracy.gCzasProcesuMin = 0;
		gParametryPracy.gCzasProcesuSek = 0;
		gParametryPracy.gCzasStartGodz = 0;
		gParametryPracy.gCzasStartMin = 0;
		gParametryPracy.gCzasStartSek = 0;
		
		gParametryPracy.gZaworGonCntr = 0;
		
		gParametryPracy.gZaworGonu = false;
		gParametryPracy.gZaworWoda = false;
		
		gParametryPracy.gZaworPlukanie_OLM_m = false;
		gParametryPracy.gZaworGlowica_m = false;
		
		gParametryPracy.G1 = false;
		gParametryPracy.G2 = false;
		gParametryPracy.G3 = false;
		
		gParametryPracy.gTempBeczka = 0.0;
		gParametryPracy.gTempKolumna = 0.0;
		gParametryPracy.gTempGlowica = 0.0;
		gParametryPracy.gTempModBufor = 0.0;
		gParametryPracy.gTempModWoda = 0.0;
		gParametryPracy.gTempModWolny = 0.0;

		gParametryPracy.gAktZalanie = 0;
		gParametryPracy.AnalogSensorValue = 0; // czujnik zalania
		gParametryPracy.gKrok = 0;
		gParametryPracy.gPresureBMP = 0;  //coś. atmosferyczne
		
		// Serial.println("");
		// Serial.println("##########-->");
		// Serial.println("gCzasZalania_1:");
		// Serial.println(gParametryPracy.gCzasZalania_1);
		// Serial.println("gCzasZalania_2:");
		// Serial.println(gParametryPracy.gCzasZalania_2);
		// Serial.println("gCzasZalania_3:");
		// Serial.println(gParametryPracy.gCzasZalania_3);
		//Serial.println("");
	}
	else
	{
		//Serial.println("resetting EEPROM data.");
		EEPROM_RESET();
		return;
	}

	if (gParametryPracy.gStruct_ID != 4294967295)
	{
		//Serial.println("Struct ID not match, resetting EEPROM data.");
		EEPROM_RESET();
		return;
	}
}

void file_list_send()
{
	// Serial.println("Send saved files list to > browser");
	// Serial.println("");

	String str_file_list = "file_list;";

	// LittleFS.begin();

	Dir dir = LittleFS.openDir("/");
	bool no_files = true;
	// String fileName;
	size_t fileSize;
	while (dir.next())
	{
		// fileName = dir.fileName();
		fileSize = dir.fileSize();
		// Serial.printf("FS File: %s, size: %s\n", fileName.c_str(), FormatBytes(fileSize).c_str());
		if (dir.fileName() != "log.txt")
		{
			str_file_list += dir.fileName();
			str_file_list += ";";
		}
		no_files = false;
	}

	// LittleFS.gc();
	// LittleFS.end();

	if (no_files == false)
	{
		str_file_list = str_file_list.substring(0, str_file_list.length() - 1);
	}

	webSocket.broadcastTXT(str_file_list.c_str());

	// Serial.println("");
}

int file_number()
{
	int numfiles = 0;

	// LittleFS.begin();

	Dir dir = LittleFS.openDir("/");
	byte cnt = 0;
	while (dir.next())
	{
		numfiles++;
	}

	// LittleFS.end();

	return numfiles;
}

void file_system_status()
{
	FSInfo fs_info;

	//Serial.print("\n");
	//Serial.println("type: LittleFS");
	//Serial.printf("total bytes: %s\n", FormatBytes(fs_info.totalBytes).c_str());
	//Serial.printf("used bytes: %s\n", FormatBytes(fs_info.usedBytes).c_str());

	//Serial.print("\n");
}

void file_list_print()
{
	//Serial.println("");
	//Serial.println("File list:");
	//Serial.println("");

	// LittleFS.begin();

	Dir dir = LittleFS.openDir("/");
	byte cnt = 0;
	while (dir.next())
	{
		String fileName = dir.fileName();
		size_t fileSize = dir.fileSize();
		delay(200);
		//Serial.printf("FS File: %s, size: %s\n", fileName.c_str(), FormatBytes(fileSize).c_str());
	}
	//Serial.printf_P(PSTR("\n"));

	// LittleFS.gc();

	// LittleFS.end();
		
}

void Akt_Temp()
{
	gParametryPracy.gTempKolumna = sensor_1.getTempCByIndex(0);
	gParametryPracy.gTempBeczka = sensor_2.getTempCByIndex(0);
	gParametryPracy.gTempGlowica = sensor_3.getTempCByIndex(0);

	// gParametryPracy.gTempKolumna = truncatef(gParametryPracy.gTempKolumna);
	// Serial.println(gParametryPracy.gTempKolumna);
	// Serial.println(gParametryPracy.gTempBeczka);
	// Serial.println(gParametryPracy.gTempGlowica);

	// AWARIA DS:
	if (gParametryPracy.gTempKolumna == -127.00 || gParametryPracy.gTempBeczka == -127.00 || gParametryPracy.gTempGlowica == -127.00)
	{
		// Beep(2000);
		/* Serial.println(F("Awaria DS"));
		Serial.println("--------");
		Serial.print(F("Kolumna: "));
		Serial.println(gParametryPracy.gTempKolumna);
		Serial.print(F("Beczka: "));
		Serial.println(gParametryPracy.gTempBeczka);
		Serial.print(F("Głowica: "));
		Serial.println(gParametryPracy.gTempGlowica);
		Serial.println("--------"); */

		if (gParametryPracy.gTempKolumna == -127.00)
		{
			DS18b20_1 = false;
			//file_print_log("DS error: kolumna\r\n");
			gParametryPracy.gTempKolumna = 0.00;
		}
		if (gParametryPracy.gTempBeczka == -127.00)
		{
			DS18b20_1 = false;
			//file_print_log("DS error: zbiornik\r\n");
			gParametryPracy.gTempBeczka = 0.00;
		}
		if (gParametryPracy.gTempGlowica == -127.00)
		{
			DS18b20_1 = false;
			//file_print_log("DS: error glowica\r\n");
			gParametryPracy.gTempGlowica = 0.00;
		}
		AwariaDS_number++;
	}

	// TEMP OK
	if (gParametryPracy.gTempKolumna != -127.00 && gParametryPracy.gTempBeczka != -127.00 && gParametryPracy.gTempGlowica != -127.00)
	{
		// Beep(1000);
		// Serial.println(F("DS znowu dziala..."));
		// file_print_log("DS OK: kol zbi glo\r\n");
		DS18b20_1 = true;
		DS18b20_2 = true;
		DS18b20_3 = true;
		AwariaDS_number = 0;
		AWARIA_DS = false;
	}

	if (gParametryPracy.gTempBeczka > gParametryPracy.gTempAlarmuBeczka)
	{
		// Beep(2000);
		//Serial.println(F("Alarm temperatury beczka..."));

		if (Alarm_temperaturyB == false)
		{
			// gAutomat.SavePowerState();
			// gAutomat.WszystkieGrzalki_OFF();
			gAutomat.Stop();
			Alarm_temperaturyB = true;
			//file_print_log("Przekroczono alarm temperatury beczka\r\n");
		}
		// Do_power_off = false;
	}

	if (gParametryPracy.gTempGlowica > gParametryPracy.gTempAlarmuGlowica)
	{
		// Beep(2000);
		//Serial.println(F("Alarm temperatury glowica..."));

		if (Alarm_temperaturyG == false)
		{
			// gAutomat.SavePowerState();
			// gAutomat.WszystkieGrzalki_OFF();
			gAutomat.Stop();
			Alarm_temperaturyG = true;
			//file_print_log("Przekroczono alarm temperatury glowica\r\n");
		}

		// Do_power_off = false;
	}

	if (gParametryPracy.gTempBeczka < gParametryPracy.gTempAlarmuBeczka && gParametryPracy.gTempGlowica < gParametryPracy.gTempAlarmuGlowica)
	{
		if (Alarm_temperaturyG == true || Alarm_temperaturyB == true)
		{
			Alarm_temperaturyB = false;
			Alarm_temperaturyG = false;
			//file_print_log("Temperatura spadla wiec alarm wylaczono\r\n");
			// gAutomat.RestorePowerState();
		}
	}

	sensor_1.requestTemperaturesnodelay();
	sensor_2.requestTemperaturesnodelay();
	sensor_3.requestTemperaturesnodelay();

	// Czy jest alarm zalania:

	
	// Update arduino cloud code:
	// ArduinoCloud.update();
}

void LCD_Update()
{
	if (disable_lcd_update == true)
		return;
	if (lcd_wait > 0)
	{
		lcd_wait--;
		if (lcd_wait == 0)
		{
			lcd.clear();
		}
		else
		{
			return;
		}
	}
	LCD_MAIN_Screen();
}

void LCD_draw_IP()
{
	String sTmpStr;

	lcd.setCursor(0, 0);
	if (gParametryPracy.gWIFI_MODE == 0) // STA
	{

		sTmpStr = "IP: ";
		sTmpStr += WiFi.localIP().toString();

		uint8_t slength = sTmpStr.length();
		for (uint8_t cntr = 0; cntr < (20 - slength); cntr++)
		{
			sTmpStr += " ";
		}
		lcd.print(sTmpStr.c_str());
	}
	else if (gParametryPracy.gWIFI_MODE == 1 || gParametryPracy.gWIFI_MODE == 2) // AP
	{
		sTmpStr = "IP: ";
		sTmpStr += WiFi.softAPIP().toString();
		uint8_t slength = sTmpStr.length();
		for (uint8_t cntr = 0; cntr < (20 - slength); cntr++)
		{
			sTmpStr += " ";
		}
		lcd.print(sTmpStr.c_str());
	}
}

void LCD_draw_Power()
{
	String sTmpStr;
	lcd.setCursor(0, 0);
	if (gAutomat.IsStarted() == true)
	{
		sTmpStr += "MOC:  ";
		sTmpStr += "   ";
	}
	else
	{
		sTmpStr += "MOC: ";
		sTmpStr += "   ";
	}

	int pwr = 0;
	if (gParametryPracy.G1 == true)
		pwr += gParametryPracy.gMocGrzaniaG1;
	if (gParametryPracy.G2 == true)
		pwr += gParametryPracy.gMocGrzaniaG2;
	if (gParametryPracy.G3 == true)
		pwr += gParametryPracy.gMocGrzaniaG3;

	sTmpStr += String(pwr);
	sTmpStr += "  W";

	uint8_t slength = sTmpStr.length();
	for (uint8_t cntr = 0; cntr < (20 - slength); cntr++)
	{
		sTmpStr += " ";
	}
	lcd.print(sTmpStr.c_str());
}

void LCD_draw_status()
{
	String sTmpStr;
	switch (gAutomat.Status())
	{
	case 0:
	{
		lcd.setCursor(0, 0);
		sTmpStr = "TERMOSTAT";
		sTmpStr += "  0";
		uint8_t slength = sTmpStr.length();
		for (uint8_t cntr = 0; cntr < (20 - slength); cntr++)
		{
			sTmpStr += " ";
		}
		lcd.print(sTmpStr.c_str());
	}
	break;

	case 1:
	{
		lcd.setCursor(0, 0);
		sTmpStr = "ROZGRZEWANIE";
		sTmpStr += "  1";
		uint8_t slength = sTmpStr.length();
		for (uint8_t cntr = 0; cntr < (20 - slength); cntr++)
		{
			sTmpStr += " ";
		}
		lcd.print(sTmpStr.c_str());
	}
	break;

	case 2:
	{
		lcd.setCursor(0, 0);
		sTmpStr = "ZALEWANIE";
		sTmpStr += "  ";
		if (gParametryPracy.gPrzerwaWzalaniu == true)
		{
			sTmpStr += "  P";
		}
		else
		{
			memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
			itoa(gParametryPracy.gAktZalanie, TempCharBuffer, 10);
			sTmpStr += String(TempCharBuffer);
		}
		uint8_t slength = sTmpStr.length();
		for (uint8_t cntr = 0; cntr < (20 - slength); cntr++)
		{
			sTmpStr += " ";
		}
		lcd.print(sTmpStr.c_str());
	}
	break;

	case 3:
	{
		lcd.setCursor(0, 0);
		sTmpStr = "STABILIZACJA";
		sTmpStr += "  3";
		uint8_t slength = sTmpStr.length();
		for (uint8_t cntr = 0; cntr < (20 - slength); cntr++)
		{
			sTmpStr += " ";
		}
		lcd.print(sTmpStr.c_str());
	}
	break;

	case 4:
	{
		lcd.setCursor(0, 0);
		sTmpStr = "PRZEDGON";
		sTmpStr += "  4";
		uint8_t slength = sTmpStr.length();
		for (uint8_t cntr = 0; cntr < (20 - slength); cntr++)
		{
			sTmpStr += " ";
		}
		lcd.print(sTmpStr.c_str());
	}
	break;

	case 5:
	{
		lcd.setCursor(0, 0);
		sTmpStr = "STB.PRZEDGONU";
		sTmpStr += "  5";
		uint8_t slength = sTmpStr.length();
		for (uint8_t cntr = 0; cntr < (20 - slength); cntr++)
		{
			sTmpStr += " ";
		}
		lcd.print(sTmpStr.c_str());
	}
	break;

	case 6:
	{
		lcd.setCursor(0, 0);
		sTmpStr = "PLUKANIE.OLM";
		sTmpStr += "  6";
		uint8_t slength = sTmpStr.length();
		for (uint8_t cntr = 0; cntr < (20 - slength); cntr++)
		{
			sTmpStr += " ";
		}
		lcd.print(sTmpStr.c_str());
	}
	break;

	case 7:
	{
		lcd.setCursor(0, 0);
		sTmpStr = "ODB.DESTYLATU";
		sTmpStr += "  7";
		uint8_t slength = sTmpStr.length();
		for (uint8_t cntr = 0; cntr < (20 - slength); cntr++)
		{
			sTmpStr += " ";
		}
		lcd.print(sTmpStr.c_str());
	}
	break;

	case 8:
	{
		lcd.setCursor(0, 0);
		sTmpStr = "ODB.POZOSTALYCH";
		sTmpStr += "  8";
		uint8_t slength = sTmpStr.length();
		for (uint8_t cntr = 0; cntr < (20 - slength); cntr++)
		{
			sTmpStr += " ";
		}
		lcd.print(sTmpStr.c_str());
	}
	break;

	case 9:
	{
		lcd.setCursor(0, 0);
		sTmpStr = "CHLODZENIE";
		sTmpStr += "  9";
		uint8_t slength = sTmpStr.length();
		for (uint8_t cntr = 0; cntr < (20 - slength); cntr++)
		{
			sTmpStr += " ";
		}
		lcd.print(sTmpStr.c_str());
	}
	break;

	case 10:
	{
		lcd.setCursor(0, 0);
		sTmpStr = "NIC NIE ROBIE";
		uint8_t slength = sTmpStr.length();
		for (uint8_t cntr = 0; cntr < (20 - slength); cntr++)
		{
			sTmpStr += " ";
		}
		lcd.print(sTmpStr.c_str());
	}
	break;

	case 11:
	{
		lcd.setCursor(0, 0);
		sTmpStr = "ZAKONCZONO";
		uint8_t slength = sTmpStr.length();
		for (uint8_t cntr = 0; cntr < (20 - slength); cntr++)
		{
			sTmpStr += " ";
		}
		lcd.print(sTmpStr.c_str());
	}
	break;

	case 12:
	{
		lcd.setCursor(0, 0);
		sTmpStr = "TRYB MANUALNY";
		uint8_t slength = sTmpStr.length();
		for (uint8_t cntr = 0; cntr < (20 - slength); cntr++)
		{
			sTmpStr += " ";
		}
		lcd.print(sTmpStr.c_str());
	}
	break;
	}
}

void LCD_draw_Temp1()
{
	// Głowica:
	String sTmpStr;
	lcd.setCursor(0, 1);
	lcd.print("GLO");
	lcd.setCursor(4, 1);
	if (gParametryPracy.gTempGlowica != -127.0)
	{
		memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
		dtostrf(gParametryPracy.gTempGlowica, 5, 1, TempCharBuffer);
		sTmpStr = TempCharBuffer;
		sTmpStr.trim();
		sTmpStr += " ";
		lcd.print(sTmpStr.c_str());
	}
	else
	{
		lcd.print("brak");
	}

	// Kolumna:
	lcd.setCursor(11, 1);
	lcd.print("KOL");
	lcd.setCursor(15, 1);
	if (gParametryPracy.gTempKolumna != -127.0)
	{
		memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
		dtostrf(gParametryPracy.gTempKolumna, 5, 1, TempCharBuffer);
		sTmpStr = TempCharBuffer;
		sTmpStr.trim();
		sTmpStr += " ";
		lcd.print(sTmpStr.c_str());
	}
	else
	{
		lcd.print("brak");
	}

	// Zbiornik:
	lcd.setCursor(0, 2);
	lcd.print("ZBI");
	lcd.setCursor(4, 2);
	if (gParametryPracy.gTempBeczka != -127.00)
	{
		memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
		dtostrf(gParametryPracy.gTempBeczka, 5, 1, TempCharBuffer);
		sTmpStr = TempCharBuffer;
		sTmpStr.trim();
		sTmpStr += " ";
		lcd.print(sTmpStr.c_str());
	}
	else
	{
		lcd.print("brak");
	}
}

void LCD_draw_TempDnia()
{
	String sTmpStr;
	lcd.setCursor(11, 2);
	lcd.print("TDN");
	lcd.setCursor(15, 2);
	memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
	dtostrf(gParametryPracy.gTempDnia, 5, 1, TempCharBuffer);
	sTmpStr = TempCharBuffer;
	sTmpStr.trim();
	lcd.print(sTmpStr.c_str());
}

void LCD_draw_time()
{
	String sTmpStr = "        ";
	sTmpStr += String(gParametryPracy.gCzasProcesuGodz) +
			   ":" + String(gParametryPracy.gCzasProcesuMin) +
			   ":" + String(gParametryPracy.gCzasProcesuSek);

	for (uint8_t lop = 0; lop < 20 - sTmpStr.length(); lop++)
	{
		sTmpStr += " ";
	}
	lcd.setCursor(0, 3);
	lcd.print(sTmpStr.c_str());
}

void LCD_draw_Temp2()
{
	// Bufor:
	String sTmpStr;
	lcd.setCursor(0, 1);
	lcd.print("BUF");
	lcd.setCursor(4, 1);
	if (gParametryPracy.gTempModBufor != 0.0 && gParametryPracy.gTempModBufor != -127.0)
	{
		memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
		dtostrf(gParametryPracy.gTempModBufor, 5, 1, TempCharBuffer);
		sTmpStr = TempCharBuffer;
		sTmpStr.trim();
		sTmpStr += " ";
		lcd.print(sTmpStr.c_str());
	}
	else
	{
		lcd.print("brak");
	}

	// Woda:
	lcd.setCursor(11, 1);
	lcd.print("WOD");
	lcd.setCursor(15, 1);
	if (gParametryPracy.gTempModWoda != 0.0 && gParametryPracy.gTempModWoda != -127.0)
	{
		memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
		dtostrf(gParametryPracy.gTempModWoda, 5, 1, TempCharBuffer);
		sTmpStr = TempCharBuffer;
		sTmpStr.trim();
		sTmpStr += " ";
		lcd.print(sTmpStr.c_str());
	}
	else
	{
		lcd.print("brak");
	}

	// Wolny:
	lcd.setCursor(0, 2);
	lcd.print("WOL");
	lcd.setCursor(4, 2);
	if (gParametryPracy.gTempModWolny != 0.0 && gParametryPracy.gTempModWolny != -127.0)
	{
		memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
		dtostrf(gParametryPracy.gTempModWolny, 5, 1, TempCharBuffer);
		sTmpStr = TempCharBuffer;
		sTmpStr.trim();
		sTmpStr += " ";
		lcd.print(sTmpStr.c_str());
	}
	else
	{
		lcd.print("brak");
	}
}

void LCD_draw_alarmy()
{
	// if((Alarm_temperaturyB || Alarm_temperaturyG) == true)
	// {
	lcd.setCursor(0, 3);
	lcd.print("ALARM TEMPERATURY ");
	// }
}

void LCD_draw_DSerror()
{
	// if(AWARIA_DS == true)
	//{
	lcd.setCursor(0, 3);
	lcd.print("DS.ERROR    !!!    ");
	// }
}

void LCD_draw_tmodule()
{
	// termostat T
	if (gAutomat.Status() == TERMOSTAT)
	{
		if (blinkTermostat == true)
		{
			lcd.setCursor(3, 3);
			lcd.print("T");
			blinkTermostat = false;
		}
		else
		{
			lcd.setCursor(3, 3);

			if (module_live == true) // termostat T i ewent. moduł M
			{
				lcd.print("M");
			}
			else
			{
				lcd.print(" ");
			}

			blinkTermostat = true;
		}
	}
	else
	{
		lcd.setCursor(3, 3);
		if (module_live == true) //  moduł M
		{
			lcd.print("M");
		}
		else
		{
			lcd.print(" ");
		}
	}

	// Start > blink mark
	if (gAutomat.IsStarted() == true)
	{
		if (blinkTermostat == true)
		{
			lcd.setCursor(0, 3);
			lcd.print(">");
			blinkTermostat = false;
		}
		else
		{
			lcd.setCursor(0, 3);
			lcd.print(" ");
			blinkTermostat = true;
		}
	}
}

void LCD_MAIN_Screen()
{
	// Alarm zalania:
	if (gAutomat.AlarmZalaniaStatus() == true)
	{
		// disable_lcd_update = true;
		lcd.clear();
		lcd.setCursor(3, 0);
		lcd.print(F("ALARM ZALANIA"));
		lcd.setCursor(3, 1);
		lcd.print("ZMNIEJSZ MOC !");
		lcd.setCursor(3, 2);
		lcd.print("W USTAWIENIACH");
		lcd.setCursor(2, 3);
		lcd.print("ZACZNIJ OD NOWA");
		// lcd_wait = 4;
		// disable_lcd_update = false;
		return;
	}

	// Blink First line:
	if (ip_blink_cnt <= 6)
	{
		LCD_draw_Power();
	}
	else if (ip_blink_cnt >= 6 && ip_blink_cnt <= 16)
	{
		LCD_draw_IP();
	}
	else if (ip_blink_cnt >= 16)
	{
		LCD_draw_status();
	}
	if (ip_blink_cnt >= 26)
		ip_blink_cnt = 0;

	// Blink temperatures:
	if (temp_blink_cnt <= 6)
	{
		LCD_draw_Temp1();
	}
	else if (temp_blink_cnt >= 6 && temp_blink_cnt <= 16)
	{
		LCD_draw_Temp2();
	}
	if (temp_blink_cnt >= 16)
		temp_blink_cnt = 0;

	// lcd.clear();

	// tmp dnia:
	LCD_draw_TempDnia();

	// alarmy:
	if ((Alarm_temperaturyB || Alarm_temperaturyG) == true)
	{
		LCD_draw_alarmy();
	}
	else
	{
		if (AWARIA_DS == true)
		{
			// DS error:
			LCD_draw_DSerror();
		}
		else
		{
			// time:
			LCD_draw_time();

			// termostat and module mark
			LCD_draw_tmodule();
		}
	}

	ip_blink_cnt++;
	temp_blink_cnt++;
}

void WIFI_Check_Connection_timer()
{
	if (gAP_CONNECTED == true)
	{
		gParametryPracy.ip_AP.ip1 = WiFi.softAPIP()[0];
		gParametryPracy.ip_AP.ip2 = WiFi.softAPIP()[1];
		gParametryPracy.ip_AP.ip3 = WiFi.softAPIP()[2];
		gParametryPracy.ip_AP.ip4 = WiFi.softAPIP()[3];
		return;
	}

	// GET STATUS FROM LIBS:
	uint8_t wifi_status = WiFi.status();
	if (wifi_status == WL_CONNECTED || wifi_status == WL_SCAN_COMPLETED)
	{
		//Serial.println(F(""));
		//Serial.println(F("IDF: Polaczenie z ruterem OK !"));
		//Serial.println(F(""));
	}
	else
	{
		if (router_error_cntr == 10)
		{
			//Serial.println(F(""));
			//Serial.println(F("IDF: Przekroczono limit prob polaczenia do rutera wifi !"));
			WIFI_start_AP();
			router_error_cntr = 0;
		}
		router_error_cntr++;
		WiFi.reconnect();
		//Serial.println(F(""));
		//Serial.println(F("IDF: Wykryto brak polaczenia z ruterem wifi !"));
		//Serial.println(F("IDF: Prubuję ponownie polaczyc sie z ruterem wifi !"));
		return;
	}

	// Ping router:
	//Serial.printf("\n\nPinging gateway with IP: %s\n", WiFi.gatewayIP().toString().c_str());

	if (pinger.Ping(WiFi.gatewayIP()) == false)
	{
		//Serial.println(F(""));
		//Serial.println(F("PING: Brak polaczenia z ruterem wifi !"));

		if (ping_error_cntr == 10)
		{
			//Serial.println(F(""));
			//Serial.println(F("IDF: Przekroczono limit prob ping do rutera wifi !"));
			//Serial.println(F("IDF: Prubuję ponownie polaczyc sie z ruterem wifi !"));
			WiFi.reconnect();
			ping_error_cntr = 0;
			return;
		}
		ping_error_cntr++;
	}
	else
	{
		Serial.println(F(""));
		Serial.println(F("PING: Polaczenie z ruterem OK !"));
	}

	// if(pinger.Ping("cloud.arduino.cc",8,2000) == false)
	//{
	// Serial.println(F("PING: Brak polaczenia z cloud.arduino.cc !"));
	//}else
	//{
	// Serial.println(F("PING: Polaczenie z cloud.arduino.cc OK !"));
	//}

	// IP obtained from router:
	gParametryPracy.ip_STA.ip1 = WiFi.localIP()[0];
	gParametryPracy.ip_STA.ip2 = WiFi.localIP()[1];
	gParametryPracy.ip_STA.ip3 = WiFi.localIP()[2];
	gParametryPracy.ip_STA.ip4 = WiFi.localIP()[3];

	// CLOUD:
	// ArduinoCloud.printDebugInfo();

	ESP.wdtFeed();
}

void LCD_Auto_start()
{
	disable_lcd_update = true;
	//Serial.println(F("Start AUTO ...."));
	lcd.clear();
	lcd.setCursor(6, 1);
	lcd.print("START >>>");
	lcd_wait = 4;
	disable_lcd_update = false;
}

void LCD_manual_start()
{
	disable_lcd_update = true;
	//Serial.println(F("Start manual"));
	lcd.clear();
	lcd.setCursor(4, 1);
	lcd.print("MANUAL >>>");
	lcd_wait = 4;
	disable_lcd_update = false;
}

void LCD_muka()
{
	disable_lcd_update = true;
	//Serial.println(F("Muka ..."));
	lcd.clear();
	lcd.setCursor(3, 1);
	lcd.print("?.?.?.?.?");
	lcd_wait = 4;
	disable_lcd_update = false;
}

void LCD_manual_stop()
{
	disable_lcd_update = true;
	//Serial.println(F("Stop manual"));
	lcd.clear();
	lcd.setCursor(2, 1);
	lcd.print("STOP MANUAL >>>");
	lcd_wait = 4;
	disable_lcd_update = false;
}

void LCD_Auto_stop()
{
	disable_lcd_update = true;
	//Serial.println(F("Stop AUTO ...."));
	lcd.clear();
	lcd.setCursor(6, 1);
	lcd.print("STOP >>>");
	lcd_wait = 4;
	disable_lcd_update = false;
}

void Awaria_DS18b20()
{

	lcd.setCursor(0, 0);
	lcd.print("AWARIA !!!");
	lcd.setCursor(0, 1);
	lcd.print("Czujnika temperatury");
	lcd.setCursor(6, 2);
	lcd.print("DS18b20");

	if (DS18b20_1 == false)
	{
		lcd.setCursor(0, 3);
		lcd.print("KOL");
	}

	if (DS18b20_2 == false)
	{
		lcd.setCursor(5, 3);
		lcd.print("ZBI");
	}

	if (DS18b20_3 == false)
	{
		lcd.setCursor(9, 3);
		lcd.print("GLO");
	}

	pcf8574.digitalWrite(BUZZER, LOW);
	delay(500);
	lcd.clear();
	pcf8574.digitalWrite(BUZZER, HIGH);
	delay(500);

	Display_AWARIA_DS_cntr++;

	if (DS18b20_1 != false && DS18b20_2 != false && DS18b20_3 != false)
	{
		AWARIA_DS = false;
		Display_AWARIA_DS = false;
		Display_AWARIA_DS_cntr = 0;
		return;
	}
}

void Decode_Json()
{
	// EXAMPLE:
	// char json[] = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";
	// const char* sensor = doc["sensor"];
	// long time             = doc["time"];
	// double latitude     = doc["data"][0];
	// double longitude  = doc["data"][1];

	// data_save:{"TempKolumna":"15.8","TempBeczka":"15.1","TempGlowica":"15.9","TempModWoda":"0.0","TempModBufor":"0.0","TempModWolny":"0.0","TempAlarmuGlowica":"70","TempAlarmuBeczka":"105","TempDnia":"0.0","ETAP":0,"Status":"OK","AutomataStatus":"10","TempTermostat_stop":72,"TempTermostat_start":70,"Termostat_status":"OFF","TempStopRozgrzewania":80,"AktZalanie":0,"PresureBMP":0,"CisnienieZalania_1":788,"CisnienieZalania_2":877,"CisnienieZalania_3":993,"PrzerwaZalania_1":5,"PrzerwaZalania_2":5,"PrzerwaZalania_3":5,"CzasZalania_1":8,"CzasZalania_2":5,"CzasZalania_3":3,"CzasStabilizacji":15,"Stb_PWR_MOD":false,"Stb_G1pwm":1000,"Stb_G2":true,"Stb_G3":true,"CyklePrzedgonu":5,"CzasOtwarciaZPrzedgonu":5,"CzasZamknieciaZPrzedgonu":5,"Przg_PWR_MOD":false,"Przg_G1pwm":1200,"Przg_G2":true,"Przg_G3":true,"CzasStabilizacjiPrzedgonu":5,"CzasPlukanieOLM":5,"CzasZakonczeniaGon":20,"HisterezaG_open":0.5,"HisterezaG_close":1,"Gon_PWR_MOD":false,"Gon_G1pwm":500,"Gon_G2":true,"Gon_G3":true,"TempStopPogonu":0,"CzasChlodzeniaGlow":5,"CzasGodz":0,"CzasMin":0,"CzasSek":0,"ZaworGonu":"OFF","ZaworWoda":"OFF","ZaworPlukanie_OLM_m":"OFF","ZaworGlowica_m":"OFF","ZaworPrzedgon_m":"OFF","ZaworGonCntr":0,"WIFI_SSID":"bubu1","WIFI_PASS":"prolongatum2020","WIFI_RSSI":-60,"WIFI_MODE":0,"ESP_CNTR":1479,"MODULE":false,"G1":false,"G2":false,"G3":false,"MocGrzaniaG1":2000,"MocGrzaniaG2":3000,"MocGrzaniaG3":400}

	//Serial.println(msg.c_str());

	msg.remove(0, 10);

	DynamicJsonDocument doc(1024 * 3);
	deserializeJson(doc, msg.c_str());
	JsonObject obj = doc.as<JsonObject>();

	// WIFI:
	const char *strFrom = "brak";
	String tmp_str = obj["WIFI_PASS"];

	if (tmp_str.length() != 0)
	{
		memset(gParametryPracy.gPASS, 0, sizeof(gParametryPracy.gPASS));
		memcpy(gParametryPracy.gPASS, tmp_str.c_str(), tmp_str.length());
	}
	else
	{
		memset(gParametryPracy.gPASS, 0, sizeof(gParametryPracy.gPASS));
		memcpy(gParametryPracy.gPASS, strFrom, 4);
	}

	tmp_str = obj["WIFI_SSID"].as<String>();
	if (tmp_str.length() != 0)
	{
		memset(gParametryPracy.gSSID, 0, sizeof(gParametryPracy.gSSID));
		memcpy(gParametryPracy.gSSID, tmp_str.c_str(), tmp_str.length());
	}
	else
	{
		memset(gParametryPracy.gSSID, 0, sizeof(gParametryPracy.gSSID));
		memcpy(gParametryPracy.gSSID, strFrom, 4);
	}

	// CLOUD DATA Device login and key:
	// tmp_str = (String)obj["CloudLogin"];
	// if(tmp_str.length() != 0)
	//{
	// memset(gParametryPracy.gCloudLogin,0,sizeof(gParametryPracy.gCloudLogin));
	// memcpy(gParametryPracy.gCloudLogin,tmp_str.c_str(),tmp_str.length());
	//}else
	//{
	// memset(gParametryPracy.gCloudLogin,0,sizeof(gParametryPracy.gCloudLogin));
	// memcpy(gParametryPracy.gCloudLogin,strFrom,4);
	//}

	// tmp_str = (String)obj["CloudKey"];
	// if(tmp_str.length() != 0)
	//{
	// memset(gParametryPracy.gCloudKey,0,sizeof(gParametryPracy.gCloudKey));
	// memcpy(gParametryPracy.gCloudKey,tmp_str.c_str(),tmp_str.length());
	//}else
	//{
	// memset(gParametryPracy.gCloudKey,0,sizeof(gParametryPracy.gCloudKey));
	// memcpy(gParametryPracy.gCloudKey,strFrom,4);
	//}

	// Config file name:
	tmp_str = obj["FileName"].as<String>();
	if (tmp_str.length() != 0)
	{
		memset(gParametryPracy.FileName, 0, sizeof(gParametryPracy.FileName));
		memcpy(gParametryPracy.FileName, tmp_str.c_str(), tmp_str.length());
	}
	else
	{
		memset(gParametryPracy.FileName, 0, sizeof(gParametryPracy.FileName));
		memcpy(gParametryPracy.FileName, strFrom, 4);
	}

	// alarmy:
	gParametryPracy.gTempAlarmuGlowica = doc["TempAlarmuGlowica"];
	gParametryPracy.gTempAlarmuBeczka = doc["TempAlarmuBeczka"];

	// termostaty:
	gParametryPracy.gTempTermostat_stop = doc["TempTermostat_stop"];
	gParametryPracy.gTempTermostat_start = doc["TempTermostat_start"];

	// rozgrzewanie:
	gParametryPracy.gTempStopRozgrzewania = doc["TempStopRozgrzewania"];

	// zalewanie:
	gParametryPracy.gCisnienieZalania_1 = doc["CisnienieZalania_1"];
	gParametryPracy.gCisnienieZalania_2 = doc["CisnienieZalania_2"];
	gParametryPracy.gCisnienieZalania_3 = doc["CisnienieZalania_3"];

	gParametryPracy.gPrzerwaZalania_1 = doc["PrzerwaZalania_1"];
	gParametryPracy.gPrzerwaZalania_2 = doc["PrzerwaZalania_2"];
	gParametryPracy.gPrzerwaZalania_3 = doc["PrzerwaZalania_3"];

	gParametryPracy.gCzasZalania_1 = doc["CzasZalania_1"];
	gParametryPracy.gCzasZalania_2 = doc["CzasZalania_2"];
	gParametryPracy.gCzasZalania_3 = doc["CzasZalania_3"];

	// czujnik zalania:
	gParametryPracy.gCzujnikZalania = doc["CzujnikZalania"];
	gParametryPracy.gWartoscZalania = doc["WartoscZalania"];

	// power module regulowany:
	gParametryPracy.gPWR_MOD = doc["PWR_MOD"];

	// stabilizacja
	gParametryPracy.gCzasStabilizacji = doc["CzasStabilizacji"];
	gParametryPracy.gStb_G1pwm = doc["Stb_G1pwm"];
	gParametryPracy.gStb_G1 = doc["Stb_G1"];
	gParametryPracy.gStb_G2 = doc["Stb_G2"];
	gParametryPracy.gStb_G3 = doc["Stb_G3"];

	// przedgony:
	gParametryPracy.gCyklePrzedgonu = doc["CyklePrzedgonu"];
	gParametryPracy.gCzasOtwarciaZPrzedgonu = doc["CzasOtwarciaZPrzedgonu"];
	gParametryPracy.gCzasZamknieciaZPrzedgonu = doc["CzasZamknieciaZPrzedgonu"];
	gParametryPracy.gPrzg_G1pwm = doc["Przg_G1pwm"];
	gParametryPracy.gPrzg_G1 = doc["Przg_G1"];
	gParametryPracy.gPrzg_G2 = doc["Przg_G2"];
	gParametryPracy.gPrzg_G3 = doc["Przg_G3"];

	// stab.przedgonu:
	gParametryPracy.gCzasStabilizacjiPrzedgonu = doc["CzasStabilizacjiPrzedgonu"];

	// płukanie OLM:
	gParametryPracy.gCzasPlukanieOLM = doc["CzasPlukanieOLM"];

	// odb destylatu:
	gParametryPracy.gCzasZakonczeniaGon = doc["CzasZakonczeniaGon"];
	gParametryPracy.gHisterezaG_open = doc["HisterezaG_open"];
	gParametryPracy.gHisterezaG_close = doc["HisterezaG_close"];
	gParametryPracy.gGon_G1pwm = doc["Gon_G1pwm"];
	gParametryPracy.gGon_G1 = doc["Gon_G1"];
	gParametryPracy.gGon_G2 = doc["Gon_G2"];
	gParametryPracy.gGon_G3 = doc["Gon_G3"];
	gParametryPracy.TBufor80 = doc["TBufor80"];
	gParametryPracy.TBufor60 = doc["TBufor60"];
	gParametryPracy.TBufor40 = doc["TBufor40"];
	gParametryPracy.Bufor = doc["Bufor"];

	// pogonny:
	gParametryPracy.gTempStopPogonu = doc["TempStopPogonu"];

	// chlodzenie:
	gParametryPracy.gCzasChlodzeniaGlow = doc["CzasChlodzeniaGlow"];

	// power: MocGrzania G1 // G2 // G3
	gParametryPracy.gMocGrzaniaG1 = doc["MocGrzaniaG1"];
	gParametryPracy.gMocGrzaniaG2 = doc["MocGrzaniaG2"];
	gParametryPracy.gMocGrzaniaG3 = doc["MocGrzaniaG3"];

	// Serial.println(obj["MocGrzaniaG1"].as<String>());
	// Serial.println(doc["MocGrzaniaG2"]);
	// Serial.println(doc["MocGrzaniaG3"]);

	// Serial.println(gParametryPracy.gMocGrzaniaG1);
	// Serial.println(gParametryPracy.gMocGrzaniaG2);
	// Serial.println(gParametryPracy.gMocGrzaniaG3);
}

void Build_TXT()
{
	XML = "";
	XML += "esp8266;";

	String sTmpStr;

	// gParametryPracy.gTempKolumna
	memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
	dtostrf(gParametryPracy.gTempKolumna, 5, 1, TempCharBuffer);
	sTmpStr = TempCharBuffer;
	sTmpStr.trim();
	XML += sTmpStr + ";";

	// gParametryPracy.gTempBeczka
	memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
	dtostrf(gParametryPracy.gTempBeczka, 5, 1, TempCharBuffer);
	sTmpStr = TempCharBuffer;
	sTmpStr.trim();
	XML += sTmpStr + ";";

	// gParametryPracy.gTempGlowica
	memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
	dtostrf(gParametryPracy.gTempGlowica, 5, 1, TempCharBuffer);
	sTmpStr = TempCharBuffer;
	sTmpStr.trim();
	XML += sTmpStr + ";";

	// module DS data:
	memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
	dtostrf(gParametryPracy.gTempModWoda, 5, 1, TempCharBuffer);
	sTmpStr = TempCharBuffer;
	sTmpStr.trim();
	XML += sTmpStr + ";";

	memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
	dtostrf(gParametryPracy.gTempModBufor, 5, 1, TempCharBuffer);
	sTmpStr = TempCharBuffer;
	sTmpStr.trim();
	XML += sTmpStr + ";";

	memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
	dtostrf(gParametryPracy.gTempModWolny, 5, 1, TempCharBuffer);
	sTmpStr = TempCharBuffer;
	sTmpStr.trim();
	XML += sTmpStr + ";";

	// gTempStopRozgrzewania
	memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
	dtostrf(gParametryPracy.gTempStopRozgrzewania, 5, 1, TempCharBuffer);
	sTmpStr = TempCharBuffer;
	sTmpStr.trim();
	XML += sTmpStr + ";";

	// gTempStopPogonu 		   = 99.00;
	memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
	dtostrf(gParametryPracy.gTempStopPogonu, 5, 1, TempCharBuffer);
	sTmpStr = TempCharBuffer;
	sTmpStr.trim();
	XML += sTmpStr + ";";

	// TempDnia
	memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
	dtostrf(gParametryPracy.gTempDnia, 5, 1, TempCharBuffer);
	sTmpStr = TempCharBuffer;
	sTmpStr.trim();
	XML += sTmpStr + ";";

	// ALARMA BECZKA:
	memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
	dtostrf(gParametryPracy.gTempAlarmuBeczka, 5, 1, TempCharBuffer);
	sTmpStr = TempCharBuffer;
	sTmpStr.trim();
	XML += sTmpStr + ";";

	// ALARMA GLOWICA:
	memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
	dtostrf(gParametryPracy.gTempAlarmuGlowica, 5, 1, TempCharBuffer);
	sTmpStr = TempCharBuffer;
	sTmpStr.trim();
	XML += sTmpStr + ";";

	// CZAS GODZINKI:
	memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
	itoa(gParametryPracy.gCzasProcesuGodz, TempCharBuffer, 10);
	sTmpStr = TempCharBuffer;
	sTmpStr.trim();
	XML += sTmpStr + ";";

	// MINUTKI:
	memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
	itoa(gParametryPracy.gCzasProcesuMin, TempCharBuffer, 10);
	sTmpStr = TempCharBuffer;
	sTmpStr.trim();
	XML += sTmpStr + ";";

	// SEKUNDY:
	memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
	itoa(gParametryPracy.gCzasProcesuSek, TempCharBuffer, 10);
	sTmpStr = TempCharBuffer;
	sTmpStr.trim();
	XML += sTmpStr + ";";

	// ZAWORECZKI: GONU
	if (gParametryPracy.gZaworGonu == true)
	{
		sTmpStr = "ON";
	}
	else
	{
		sTmpStr = "OFF";
	}
	XML += sTmpStr + ";";

	// WODA:
	if (gParametryPracy.gZaworWoda == true)
	{
		sTmpStr = "ON";
	}
	else
	{
		sTmpStr = "OFF";
	}
	XML += sTmpStr + ";";

	// module zawory:
	if (gParametryPracy.gZaworPlukanie_OLM_m == true)
	{
		sTmpStr = "ON";
	}
	else
	{
		sTmpStr = "OFF";
	}
	XML += sTmpStr + ";";

	// GLOWICA M
	if (gParametryPracy.gZaworGlowica_m == true)
	{
		sTmpStr = "ON";
	}
	else
	{
		sTmpStr = "OFF";
	}
	XML += sTmpStr + ";";

	// WIFI + base64 wifi credentialse:
	// SSID:
	int inputStringLength = strlen(gParametryPracy.gSSID);
	int encodedLength = Base64.encodedLength(inputStringLength);
	char SSID_encodedString[encodedLength];
	Base64.encode(SSID_encodedString, gParametryPracy.gSSID, inputStringLength);
	XML += String(SSID_encodedString) + ";";

	// PASS:
	inputStringLength = strlen(gParametryPracy.gPASS);
	encodedLength = Base64.encodedLength(inputStringLength);
	char PASS_encodedString[encodedLength];
	Base64.encode(PASS_encodedString, gParametryPracy.gPASS, inputStringLength);
	XML += String(PASS_encodedString) + ";";

	// WIFI MODE:
	XML += String(gParametryPracy.gWIFI_MODE) + ";";

	// RSSI:
	XML += String(WiFi.RSSI()) + ";";

	// TERM.HIST:
	XML += String(gParametryPracy.gHisterezaG_close) + ";";
	XML += String(gParametryPracy.gHisterezaG_open) + ";";

	// ESP LIVE LICZNIK:
	XML += String(gParametryPracy.gESP_CNTR) + ";";

	// STATUSIK:
	if (AWARIA_DS == true)
	{
		XML += String("Awaria DS") + ";";
	}
	else if ((Alarm_temperaturyB && Alarm_temperaturyG) == true)
	{
		XML += String("Alarm T.ZG") + ";";
	}
	else if (Alarm_temperaturyB == true)
	{
		XML += String("Alarm TZ") + ";";
	}
	else if (Alarm_temperaturyG == true)
	{
		XML += String("Alarm TG") + ";";
	}
	else if (gAutomat.Status() == SCODE_KONIEC)
	{
		XML += String("KONIEC") + ";";
	}
	else
	{
		XML += String("OK") + ";";
	}

	// POWER G1:
	if (gParametryPracy.G1 == true)
	{
		sTmpStr = "ON";
	}
	else
	{
		sTmpStr = "OFF";
	}
	XML += sTmpStr + ";";

	// G2
	if (gParametryPracy.G2 == true)
	{
		sTmpStr = "ON";
	}
	else
	{
		sTmpStr = "OFF";
	}
	XML += sTmpStr + ";";

	// G3
	if (gParametryPracy.G3 == true)
	{
		sTmpStr = "ON";
	}
	else
	{
		sTmpStr = "OFF";
	}
	XML += sTmpStr + ";";

	// POWER NUMBER WATT:
	// G1 gMocGrzaniaG1
	memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
	itoa(gParametryPracy.gMocGrzaniaG1, TempCharBuffer, 10);
	sTmpStr = TempCharBuffer;
	sTmpStr.trim();
	XML += sTmpStr + ";";

	// G2 gMocGrzaniaG2
	memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
	itoa(gParametryPracy.gMocGrzaniaG2, TempCharBuffer, 10);
	sTmpStr = TempCharBuffer;
	sTmpStr.trim();
	XML += sTmpStr + ";";

	// G3 gMocGrzaniaG3
	memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
	itoa(gParametryPracy.gMocGrzaniaG3, TempCharBuffer, 10);
	sTmpStr = TempCharBuffer;
	sTmpStr.trim();
	XML += sTmpStr + ";";

	// TERMOSTACIK ON OFF:
	if (gAutomat.Status() == TERMOSTAT)
	{
		sTmpStr = "ON";
	}
	else
	{
		sTmpStr = "OFF";
	}
	XML += sTmpStr + ";";

	// TERMOSTAT TEMPERATURES:
	memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
	dtostrf(gParametryPracy.gTempTermostat_stop, 5, 1, TempCharBuffer);
	sTmpStr = TempCharBuffer;
	sTmpStr.trim();
	XML += sTmpStr + ";";

	memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
	dtostrf(gParametryPracy.gTempTermostat_start, 5, 1, TempCharBuffer);
	sTmpStr = TempCharBuffer;
	sTmpStr.trim();
	XML += sTmpStr + ";";

	// CZAS ZAKONCZENIA: GON
	memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
	itoa(gParametryPracy.gCzasZakonczeniaGon, TempCharBuffer, 10);
	sTmpStr = TempCharBuffer;
	sTmpStr.trim();
	XML += sTmpStr + ";";

	// ETAP:
	memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
	itoa(gParametryPracy.gETAP, TempCharBuffer, 10);
	sTmpStr = TempCharBuffer;
	sTmpStr.trim();
	XML += sTmpStr + ";";

	// ZALANIE:
	memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
	itoa(gParametryPracy.gAktZalanie, TempCharBuffer, 10);
	sTmpStr = TempCharBuffer;
	sTmpStr.trim();
	XML += sTmpStr + ";";

	// CISNIENIE Z CZUJNIKA:
	memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
	itoa(gParametryPracy.gPresureBMP, TempCharBuffer, 10);
	sTmpStr = TempCharBuffer;
	sTmpStr.trim();
	XML += sTmpStr + ";";

	// CIS. ZALANIA:
	memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
	itoa(gParametryPracy.gCisnienieZalania_1, TempCharBuffer, 10);
	sTmpStr = TempCharBuffer;
	sTmpStr.trim();
	XML += sTmpStr + ";";

	memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
	itoa(gParametryPracy.gCisnienieZalania_2, TempCharBuffer, 10);
	sTmpStr = TempCharBuffer;
	sTmpStr.trim();
	XML += sTmpStr + ";";

	memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
	itoa(gParametryPracy.gCisnienieZalania_3, TempCharBuffer, 10);
	sTmpStr = TempCharBuffer;
	sTmpStr.trim();
	XML += sTmpStr + ";";

	// PRZERWY ZALEW:
	memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
	itoa(gParametryPracy.gPrzerwaZalania_1, TempCharBuffer, 10);
	sTmpStr = TempCharBuffer;
	sTmpStr.trim();
	XML += sTmpStr + ";";

	memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
	itoa(gParametryPracy.gPrzerwaZalania_2, TempCharBuffer, 10);
	sTmpStr = TempCharBuffer;
	sTmpStr.trim();
	XML += sTmpStr + ";";

	memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
	itoa(gParametryPracy.gPrzerwaZalania_3, TempCharBuffer, 10);
	sTmpStr = TempCharBuffer;
	sTmpStr.trim();
	XML += sTmpStr + ";";

	// 3 CZASY ZALANIA:
	memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
	itoa(gParametryPracy.gCzasZalania_1, TempCharBuffer, 10);
	sTmpStr = TempCharBuffer;
	sTmpStr.trim();
	XML += sTmpStr + ";";

	memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
	itoa(gParametryPracy.gCzasZalania_2, TempCharBuffer, 10);
	sTmpStr = TempCharBuffer;
	sTmpStr.trim();
	XML += sTmpStr + ";";

	memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
	itoa(gParametryPracy.gCzasZalania_3, TempCharBuffer, 10);
	sTmpStr = TempCharBuffer;
	sTmpStr.trim();
	XML += sTmpStr + ";";

	// STAB:
	memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
	itoa(gParametryPracy.gCzasStabilizacji, TempCharBuffer, 10);
	sTmpStr = TempCharBuffer;
	sTmpStr.trim();
	XML += sTmpStr + ";";

	memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
	itoa(gParametryPracy.gCzasStabilizacjiPrzedgonu, TempCharBuffer, 10);
	sTmpStr = TempCharBuffer;
	sTmpStr.trim();
	XML += sTmpStr + ";";

	memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
	itoa(gParametryPracy.gCzasPlukanieOLM, TempCharBuffer, 10);
	sTmpStr = TempCharBuffer;
	sTmpStr.trim();
	XML += sTmpStr + ";";

	memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
	itoa(gParametryPracy.gCzasChlodzeniaGlow, TempCharBuffer, 10);
	sTmpStr = TempCharBuffer;
	sTmpStr.trim();
	XML += sTmpStr + ";";

	memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
	itoa(gParametryPracy.gCyklePrzedgonu, TempCharBuffer, 10);
	sTmpStr = TempCharBuffer;
	sTmpStr.trim();
	XML += sTmpStr + ";";

	// Z PRZEDGON CZASY OTW ZAMKN:
	memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
	itoa(gParametryPracy.gCzasOtwarciaZPrzedgonu, TempCharBuffer, 10);
	sTmpStr = TempCharBuffer;
	sTmpStr.trim();
	XML += sTmpStr + ";";

	memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
	itoa(gParametryPracy.gCzasZamknieciaZPrzedgonu, TempCharBuffer, 10);
	sTmpStr = TempCharBuffer;
	sTmpStr.trim();
	XML += sTmpStr + ";";

	// AUTOMATA STATUS:
	memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
	itoa(gAutomat.Status(), TempCharBuffer, 10);
	sTmpStr = TempCharBuffer;
	sTmpStr.trim();
	XML += sTmpStr + ";";

	// ZAWOR GON CNTR:
	memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
	itoa(gParametryPracy.gZaworGonCntr, TempCharBuffer, 10);
	sTmpStr = TempCharBuffer;
	sTmpStr.trim();
	XML += sTmpStr + ";";

	if (module_live == true)
	{
		sTmpStr = "ON";
	}
	else
	{
		sTmpStr = "OFF";
	}
	XML += sTmpStr + ";";

	// new elements: **********************

	// Moduł mocy default false regulowany moduł mocy:
	if (gParametryPracy.gPWR_MOD == true)
	{
		XML += "1";
	}
	else
	{
		XML += "0";
	}
	XML += ";";

	// stabilizacja grzanie:
	// gStb_G1pwm = 0;    //  0-2000 w pwm dla stabilizacji:
	memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
	itoa(gParametryPracy.gStb_G1pwm, TempCharBuffer, 10);
	sTmpStr = TempCharBuffer;
	sTmpStr.trim();
	XML += sTmpStr + ";";

	// czy G1 dla stabilizacji jest aktywna gStb_G1:
	if (gParametryPracy.gStb_G1 == true)
	{
		XML += "1";
	}
	else
	{
		XML += "0";
	}
	XML += ";";

	// czy G2 dla stabilizacji jest aktywna gStb_G2:
	if (gParametryPracy.gStb_G2 == true)
	{
		XML += "1";
	}
	else
	{
		XML += "0";
	}
	XML += ";";

	// czy G3 dla stabilizacji jest aktywna gStb_G3:
	if (gParametryPracy.gStb_G3 == true)
	{
		XML += "1";
	}
	else
	{
		XML += "0";
	}
	XML += ";";
	// ------------------------------------------------------------------

	// przedgon grzanie:
	// gPrzg_G1pwm = 0;    //  0-2000 w pwm dla przedgonu:
	memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
	itoa(gParametryPracy.gPrzg_G1pwm, TempCharBuffer, 10);
	sTmpStr = TempCharBuffer;
	sTmpStr.trim();
	XML += sTmpStr + ";";

	// czy G1 dla przedgonu jest aktywna gPrzg_G1:
	if (gParametryPracy.gPrzg_G1 == true)
	{
		XML += "1";
	}
	else
	{
		XML += "0";
	}
	XML += ";";

	// czy G2 dla przedgonu jest aktywna gPrzg_G2:
	if (gParametryPracy.gPrzg_G2 == true)
	{
		XML += "1";
	}
	else
	{
		XML += "0";
	}
	XML += ";";

	// czy G3 dla przedgon jest aktywna gPrzg_G3:
	if (gParametryPracy.gPrzg_G3 == true)
	{
		XML += "1";
	}
	else
	{
		XML += "0";
	}
	XML += ";";
	// ------------------------------------------------------------------

	// gon grzanie:
	// gGon_G1pwm = 0;    //  0-2000 w pwm dla gonu:
	memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
	itoa(gParametryPracy.gGon_G1pwm, TempCharBuffer, 10);
	sTmpStr = TempCharBuffer;
	sTmpStr.trim();
	XML += sTmpStr + ";";

	// czy G1 dla gonu jest aktywna gGon_G1:
	if (gParametryPracy.gGon_G1 == true)
	{
		XML += "1";
	}
	else
	{
		XML += "0";
	}
	XML += ";";

	// czy G2 dla gonu jest aktywna gGon_G2:
	if (gParametryPracy.gGon_G2 == true)
	{
		XML += "1";
	}
	else
	{
		XML += "0";
	}
	XML += ";";

	// czy G3 dla gon jest aktywna gGon_G3:
	if (gParametryPracy.gGon_G3 == true)
	{
		XML += "1";
	}
	else
	{
		XML += "0";
	}
	XML += ";";
	// ------------------------------------------------------------------

	// przerwa w zalaniu:
	if (gParametryPracy.gPrzerwaWzalaniu == true)
	{
		sTmpStr = "1";
	}
	else
	{
		sTmpStr = "0";
	}
	XML += sTmpStr + ";";

	// zawór krok:
	memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
	itoa(gParametryPracy.gKrok, TempCharBuffer, 10);
	sTmpStr = TempCharBuffer;
	sTmpStr.trim();
	XML += sTmpStr + ";";

	// Analog read z modułu:
	memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
	itoa(gParametryPracy.AnalogSensorValue, TempCharBuffer, 10);
	sTmpStr = TempCharBuffer;
	sTmpStr.trim();
	XML += sTmpStr + ";";

	// bufor temperatury:
	memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
	dtostrf(gParametryPracy.TBufor80, 5, 1, TempCharBuffer);
	sTmpStr = TempCharBuffer;
	sTmpStr.trim();
	XML += sTmpStr + ";";

	memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
	dtostrf(gParametryPracy.TBufor60, 5, 1, TempCharBuffer);
	sTmpStr = TempCharBuffer;
	sTmpStr.trim();
	XML += sTmpStr + ";";

	memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
	dtostrf(gParametryPracy.TBufor40, 5, 1, TempCharBuffer);
	sTmpStr = TempCharBuffer;
	sTmpStr.trim();
	XML += sTmpStr + ";";

	// Bufor:
	if (gParametryPracy.Bufor == true)
	{
		sTmpStr = "1";
	}
	else
	{
		sTmpStr = "0";
	}
	XML += sTmpStr + ";";

	// free memory:
	memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
	itoa(system_get_free_heap_size(), TempCharBuffer, 10);
	sTmpStr = TempCharBuffer;
	sTmpStr.trim();
	XML += sTmpStr + ";";

	// Preserve start automatic process time millis()
	memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
	itoa(gParametryPracy.gCzasStartGodz, TempCharBuffer, 10);
	sTmpStr = TempCharBuffer;
	sTmpStr.trim();
	XML += sTmpStr + ";";

	// MINUTKI:
	memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
	itoa(gParametryPracy.gCzasStartMin, TempCharBuffer, 10);
	sTmpStr = TempCharBuffer;
	sTmpStr.trim();
	XML += sTmpStr + ";";

	// SEKUNDY:
	memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
	itoa(gParametryPracy.gCzasStartSek, TempCharBuffer, 10);
	sTmpStr = TempCharBuffer;
	sTmpStr.trim();
	XML += sTmpStr + ";";

	// File Name:
	XML += String(gParametryPracy.FileName) + ";";

	// Czujnik zalania:
	if (gParametryPracy.gCzujnikZalania == true)
	{
		sTmpStr = "1";
	}
	else
	{
		sTmpStr = "0";
	}
	XML += sTmpStr + ";";

	// Wartość zalania:
	memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
	itoa(gParametryPracy.gWartoscZalania, TempCharBuffer, 10);
	sTmpStr = TempCharBuffer;
	sTmpStr.trim();
	XML += sTmpStr + ";";

	// CISNIENIE Dnia:
	memset(TempCharBuffer, 0, sizeof(TempCharBuffer));
	itoa(gParametryPracy.gCisnienieDnia, TempCharBuffer, 10);
	sTmpStr = TempCharBuffer;
	sTmpStr.trim();
	XML += sTmpStr + ";";

	// Alarm zalania:
	/* if (gAutomat.AlarmZalaniaStatus() == true)
	{
		sTmpStr = "1";
	}
	else
	{
		sTmpStr = "0";
	}
	XML += sTmpStr + ";"; */
}

void file_delete(const char *filename)
{
	//Serial.println("Usiłuje usunąć plik ...");
	// setup_timer1_disable();
	// LittleFS.begin();
	String file_name(filename);
	String str_file = "/" + file_name;
	LittleFS.remove(str_file.c_str());
	// LittleFS.end();
	// setup_timer1();
	Serial.println(str_file.c_str());
}

bool Save_conf_to_eeprom()
{
	EEPROM.put(0, gParametryPracy);
	boolean ok = EEPROM.commitReset();
	
	//Serial.println("Save options to eeprom");
	//Serial.println((ok) ? "Save OK" : "Save failed");
	//Serial.println("");
	return ok;
}

bool Save_Load_conf_file(const char *filename)
{
	// String file_name(filename);
	String str_file = "/" + String(filename);

	//Serial.println("");
	//Serial.println(F("Usiluje odczytac plik konfiguracji"));
	//Serial.println();
	//Serial.println(str_file.c_str());
	//Serial.println("");

	// LittleFS.begin();

	File ConfFile = LittleFS.open(str_file.c_str(), "r");

	if (!ConfFile)
	{
		//Serial.println(F("ERROR! odczytu pliku konfiguracji !"));
		// LittleFS.end();
		return false;
	}
	else
	{
		int readSize = ConfFile.read((byte *)&gParametryPracy, sizeof(gParametryPracy));
		//Serial.println("");
		//Serial.print("Readed file size: ");
		//Serial.print(readSize);
		//Serial.println("");

		ConfFile.close();
		// LittleFS.end();
		return true;
	}

	// LittleFS.end();
	return false;
}

// int readSize = file.readBytes((byte*) myConfigStruct, sizeof(myConfigStruct));
void Save_conf_to_new_file()
{
	// int num_files = file_number();

	//Serial.println("");
	//Serial.println("Usiłuje zapisac konfiguracje do pliku");
	String file_name(gParametryPracy.FileName);

	// file_name += ".str";
	// file_name += String(num_files + 1);

	memset(gParametryPracy.FileName, 0, sizeof(gParametryPracy.FileName));
	memcpy(gParametryPracy.FileName, file_name.c_str(), file_name.length());

	setup_timer1_disable();

	if (file_name.length() > 0)
	{
		// LittleFS.begin();

		String str_file = "/" + file_name;
		File configFile = LittleFS.open(str_file.c_str(), "w+");
		//Serial.println("Nazwa pliku: " + file_name);
		if (!configFile)
		{
			//Serial.println(F("Błąd otwarcia pliku do zapisu ..."));
		}
		else
		{
			//Serial.println(F("Zapisuje plik ...."));
			//Serial.printf("Start Position =%u \n", configFile.position());
			// unsigned char * data = reinterpret_cast<unsigned char*>(&gParametryPracy);
			// size_t bytes = configFile.write(data, sizeof(gParametryPracy)); // C++ way
			size_t bytes = configFile.write((byte *)&gParametryPracy, sizeof(gParametryPracy));
			// size_t bytes = configFile.write((byte*)&gParametryPracy, sizeof(gParametryPracy));
			// F.write((byte*) Cnf, sizeof(Cnf));
			//Serial.printf("END Position =%u \n", configFile.position());

			configFile.close();
		}

		// LittleFS.end();

		// enable interrupt
		setup_timer1();

		webSocket.broadcastTXT("#filesaveok#");
	}
	else
	{
		//Serial.println(F("Error! Pusta nazwa pliku ..."));
		//Serial.println(F("podczas zapisu do pliku !!"));
	}
}

// int readSize = file.readBytes((byte*) myConfigStruct, sizeof(myConfigStruct));
void Save_conf_to_file()
{
	//Serial.println("");
	//Serial.println("Usiłuje zapisac konfiguracje do pliku");
	String file_name(gParametryPracy.FileName);

	memset(gParametryPracy.FileName, 0, sizeof(gParametryPracy.FileName));
	memcpy(gParametryPracy.FileName, file_name.c_str(), file_name.length());

	setup_timer1_disable();

	if (file_name.length() > 0)
	{
		// LittleFS.begin();

		String str_file = "/" + file_name;
		File configFile = LittleFS.open(str_file.c_str(), "w+");
		//Serial.println("Nazwa pliku: " + file_name);
		if (!configFile)
		{
			//Serial.println(F("Błąd otwarcia pliku do zapisu ..."));
		}
		else
		{
			//Serial.println(F("Zapisuje plik ...."));
			//Serial.printf("Start Position =%u \n", configFile.position());
			// unsigned char * data = reinterpret_cast<unsigned char*>(&gParametryPracy);
			// size_t bytes = configFile.write(data, sizeof(gParametryPracy)); // C++ way
			size_t bytes = configFile.write((byte *)&gParametryPracy, sizeof(gParametryPracy));
			// size_t bytes = configFile.write((byte*)&gParametryPracy, sizeof(gParametryPracy));
			// F.write((byte*) Cnf, sizeof(Cnf));
			//Serial.printf("END Position =%u \n", configFile.position());

			configFile.close();
		}

		// LittleFS.end();

		// enable interrupt
		setup_timer1();

		webSocket.broadcastTXT("#filesaveok#");
	}
	else
	{
		//Serial.println(F("Error! Pusta nazwa pliku ..."));
		//Serial.println(F("podczas zapisu do pliku !!"));
	}
}

bool Proces_WS_message()
{
	// WIFI SCAN:
	if (msg == "#wifiscan#")
	{
		//file_print_log("Start --> ws... scan...\r\n");
		// Serial.println("Start --> ws... scan...");
		WiFi.scanNetworksAsync(WIFI_Scan_WS_Done);
		return true;
	}

	// Web browser user confirm Alarm Zalania OK:
	if (msg == "#al.zal.ok#")
	{
		gAutomat.AlarmZalaniaStop();
		lcd.clear();
		webSocket.broadcastTXT("#al_zal_stop#");
		//file_print_log("Alarm Zalania STOP\r\n");
		// Serial.println("Start --> ws... scan...");
     	return true;
	}
	
	// MANUAL:
	if (msg == "#manual#")
	{
		if (gAutomat.Status() == SCODE_PAUSE)
		{
			gAutomat.Resume();
			LCD_manual_stop();
			// Serial.println("MANUAL MODE Stop...");
			//file_print_log("MANUAL MODE OFF\r\n");
		}
		else
		{
			gAutomat.Pause();
			LCD_manual_start();
			//Serial.println("MANUAL MODE Start...");
			//file_print_log("MANUAL MODE START\r\n");
		}
		Beep(600);
		return true;
	}

	// MAN z G1:
	if (msg == "#man-g1#")
	{
		if (gAutomat.Status() == SCODE_PAUSE)
		{
			if (gAutomat.Grzalka1() == true)
			{
				gAutomat.Grzalka_OFF(GRZALKA_1);
				//file_print_log("MAN G1 OFF\r\n");
			}
			else
			{
				gAutomat.Grzalka_ON(GRZALKA_1);
				//file_print_log("MAN G1 ON\r\n");
			}
		}
		Beep(600);
		return true;
	}

	// MAN z G2:
	if (msg == "#man-g2#")
	{
		if (gAutomat.Status() == SCODE_PAUSE)
		{
			if (gAutomat.Grzalka2() == true)
			{
				gAutomat.Grzalka_OFF(GRZALKA_2);
				//file_print_log("MAN G2 OFF\r\n");
			}
			else
			{
				gAutomat.Grzalka_ON(GRZALKA_2);
				//file_print_log("MAN G2 ON\r\n");
			}
		}
		Beep(600);
		return true;
	}

	// MAN z G3:
	if (msg == "#man-g3#")
	{
		if (gAutomat.Status() == SCODE_PAUSE)
		{
			if (gAutomat.Grzalka3() == true)
			{
				gAutomat.Grzalka_OFF(GRZALKA_3);
				//file_print_log("MAN G3 OFF\r\n");
			}
			else
			{
				gAutomat.Grzalka_ON(GRZALKA_3);
				//file_print_log("MAN G3 ON\r\n");
			}
		}
		Beep(600);
		return true;
	}

	// MAN z woda:
	if (msg == "#man-zwoda#")
	{
		if (gAutomat.Status() == SCODE_PAUSE)
		{
			if (gAutomat.Zawor(EZ_WODA) == true)
			{
				gAutomat.Zawor_OFF(EZ_WODA);
				//file_print_log("MAN EZ.WODA OFF\r\n");
			}
			else
			{
				gAutomat.Zawor_ON(EZ_WODA);
				//file_print_log("MAN EZ.WODA ON\r\n");
			}
		}
		Beep(600);
		return true;
	}

	// MAN z głowica:
	if (msg == "#man-zglowica#")
	{
		if (gAutomat.Status() == SCODE_PAUSE)
		{
			if (gAutomat.Zawor(EZ_GLOWICA) == true)
			{
				gAutomat.Zawor_OFF(EZ_GLOWICA);
				//file_print_log("MAN EZ.GLOWICA OFF\r\n");
			}
			else
			{
				gAutomat.Zawor_ON(EZ_GLOWICA);
				//file_print_log("MAN EZ.GLOWICA ON\r\n");
			}
		}
		Beep(600);
		return true;
	}

	// MAN z płukanie olm:
	if (msg == "#man-zplolm#")
	{
		if (gAutomat.Status() == SCODE_PAUSE)
		{
			if (gAutomat.Zawor(EZ_PLUK_OLM) == true)
			{
				gAutomat.Zawor_OFF(EZ_PLUK_OLM);
				//file_print_log("MAN EZ.POLM OFF\r\n");
			}
			else
			{
				gAutomat.Zawor_ON(EZ_PLUK_OLM);
				//file_print_log("MAN EZ.POLM ON\r\n");
			}
		}
		Beep(600);
		return true;
	}

	// MAN z gonu:
	if (msg == "#man-zgon#")
	{
		if (gAutomat.Status() == SCODE_PAUSE)
		{
			if (gAutomat.Zawor(EZ_GON) == true)
			{
				gAutomat.Zawor_OFF(EZ_GON);
				//("MAN EZ.GON OFF\r\n");
			}
			else
			{
				gAutomat.Zawor_ON(EZ_GON);
				//file_print_log("MAN EZ.GON ON\r\n");
			}
		}
		Beep(600);
		return true;
	}

	// START MODULE WIFI:
	if (msg == "#modulewifi#")
	{
		//Serial.println("Włączam wifi w module rozszerzenia...");
		gAutomat.ModuleStartWifi();
		webSocket.broadcastTXT("#modulewifion#");
		//file_print_log("ENABLE MODULE WIFI\r\n");
		Beep(600);
		return true;
	}

	// STOP AUTOMATA:
	if (msg == "#autostop#")
	{
		//Serial.println(F("STOP AUTOMATA"));
		if (gAutomat.IsStarted() == false)
		{
			LCD_muka();
		}
		else
		{
			LCD_Auto_stop();
			gAutomat.Stop();
			//file_print_log("AUTO STOP\r\n");
			webSocket.broadcastTXT("#startoff#");
		}
		Beep(1000);
		return true;
	}

	// START AUTOMATA
	if (msg == "#autostart#")
	{
		if (gAutomat.Status() == SCODE_NIC || gAutomat.Status() == SCODE_KONIEC)
		{
			if (DS18b20_1 == false || DS18b20_2 == false || DS18b20_3 == false)
			{
				disable_lcd_update = true;
				//Serial.println(F("AWARIA DS Auto wylaczone ...."));
				lcd.clear();
				lcd.setCursor(0, 0);
				lcd.print("AWARIA DS!");
				lcd.setCursor(0, 1);
				lcd_wait = 4;
				disable_lcd_update = false;
			}
			else
			{
				LCD_Auto_start();

				AwariaDS_number = 0;
				disable_lcd_update = false;
				Beep(1000);

				gAutomat.TermostatStop();
				gAutomat.Start();
				webSocket.broadcastTXT("#starton#");
				//file_print_log("AUTO START\r\n");
			}
		}
		else
		{
			LCD_muka();
		}

		return true;
	}

	// TMP.DNIA:
	if (msg == "#tmpdnia#")
	{
		gAutomat.TempDnia();
		Beep(500);
		webSocket.broadcastTXT("#tmpdnia#");
		//file_print_log("Ustalono Temp.Dnia\r\n");
		return true;
	}

	// TERMOSTAT:
	if (msg == "#termostat#")
	{

		if (gAutomat.Status() == TERMOSTAT)
		{
			gAutomat.TermostatStop();
			LCD_termostat_stop();
			//file_print_log("Termostat Stop\r\n");
			Beep(1000);
		}

		return true;
	}

	// ZAWORKI-->EZGON:
	if (msg == "#ezgon#")
	{
		//Serial.println(F("receive #ezgon#"));
		webSocket.broadcastTXT("#ezgon#");
		return true;
	}

	// SEND FILE LIST TO CLIENT BROWSER:
	if (msg == "#filelist#")
	{
		file_list_send();
		//file_print_log("Wyslano liste plikow ...\r\n");
		return true;
	}

	// SAVE CONF TO LittleFS FILE if exist owerwrite old file:
	if (msg == "#save_config#")
	{
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("ZAPIS >>>");
		lcd.setCursor(2, 2);
		lcd.print("konfiguracji...");

		Save_conf_to_file();
		Save_conf_to_eeprom();

		//file_print_log("Zapisano konfiguracje do pliku\r\n");

		Beep(700);
		lcd_wait = 4;
	}

	// New config new file and save to eeprom:
	if (msg.substring(0, 12) == "#new_config:")
	{
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("ZAPIS >>>");
		lcd.setCursor(2, 2);
		lcd.print("konfiguracji...");

		msg = msg.substring(msg.lastIndexOf(':'));
		msg.remove(0, 1);

		memset(gParametryPracy.FileName, 0, sizeof(gParametryPracy.FileName));
		memcpy(gParametryPracy.FileName, msg.c_str(), msg.length());

		Save_conf_to_new_file();
		
		Save_conf_to_eeprom();

		file_list_send();

		//file_print_log("Utworzono nowy plik\r\n");

		Beep(700);
		lcd_wait = 4;
	}

	// DELETE LittleFS FILE:
	if (msg.substring(0, 10) == "file_dele:")
	{
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("KASUJE >>>");
		lcd.setCursor(2, 2);
		lcd.print("pliki ...");

		msg.remove(0, 10);

		//Serial.println("");
		//Serial.println(msg.c_str());
		//Serial.println("");

		int i = 0;
		String filename = "";
		// data example: name1;name2;name3;
		int from = 0;

		if (msg.indexOf(";") == -1)
		{
			file_delete(msg.c_str());
		}
		else
		{
			while (i <= msg.length())
			{
				if (msg[i] == ';')
				{
					filename = msg.substring(from, i);
					file_delete(filename.c_str());
					// Serial.println(filename.c_str());
					from = i + 1;
				}
				i++;
			}
		}

		file_list_send();

		//file_print_log("Usunieto jakies pliki\r\n");

		Beep(700);
		lcd_wait = 4;
	}

	// NOT USED NOW!!!
	// SAVE JSON CONFIG TO EEPROM:
	if (msg.substring(0, 10) == "data_save:")
	{
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("ZAPIS >>>");
		lcd.setCursor(2, 2);
		lcd.print("konfiguracji");
		//Decode_Json();

		// Serial.println(msg.c_str());
		bool ok = Save_conf_to_eeprom();

		Beep(700);
		lcd_wait = 4;

		if (ok == true)
		{
			webSocket.broadcastTXT("#confsaveok#");
		}
		else
		{
			webSocket.broadcastTXT("#confsaveerror#");
		}

		return true;
	}

	// LOAD CONFIG FROM FILE :
	if (msg.substring(0, 10) == "file_load:")
	{
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("ODCZYT >>>");
		lcd.setCursor(2, 2);
		lcd.print("konfiguracji");
		lcd.setCursor(2, 3);
		lcd.print("z pliku");

		msg.remove(0,10);

		//Decode_Json();

		bool ret = Save_Load_conf_file(msg.c_str());

		// Serial.println(msg.c_str());
		bool ok = Save_conf_to_eeprom();

		Beep(1000);
		lcd_wait = 4;

		if (ret == true && ok == true)
		{
			webSocket.broadcastTXT("#confloadok#");
			//file_print_log("Zaladowano plik\r\n");
		}
		else
		{
			webSocket.broadcastTXT("#confloaderror#");
			//file_print_log("Error: Nie zaladowano pliku\r\n");
		}

		return true;
	}

	// DOWNLOAD CONFIG FROM MCU FILE TO BROWSER:
	if (msg.substring(0, 14) == "file_download:")
	{
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("POBRANIE >>>");
		lcd.setCursor(2, 2);
		lcd.print("konfiguracji");
		lcd.setCursor(2, 3);
		lcd.print("z pliku ...");

		msg.remove(0,14);

		//Decode_Json();

		bool ret = Save_Load_conf_file(msg.c_str());

		// Serial.println(msg.c_str());

		Beep(500);
		lcd_wait = 4;

		/* if(ret  == true && ok == true)
		{
				webSocket.broadcastTXT("#confloadok#");
		}else
		{
				webSocket.broadcastTXT("#confloaderror#");
		} */

		return true;
	}

	// RESET TO FACTORY SETTINGS
	if (msg == "#resetmem#")
	{
		EEPROM_RESET();
		webSocket.broadcastTXT("#resetfactok#");
		return true;
	}

	if (msg == "#elegohome#")
	{
		//Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
		//Udp.write("#gohome#");
		//Udp.endPacket();
		udp.broadcast("#gohome#");
		Beep(600);
		return true;
	}

	//**********************************************
	//**********************************************
	//**********************************************

	// Płynna zmiana opcji:
	// -----------------------
	// USTAWIENIA PODSTAWOWE:

	// Change options alarm zbiornik:
	if (msg.substring(0, 9) == "#alarmZB:")
	{
		msg = msg.substring(msg.lastIndexOf(':'));
		msg.remove(0, 1);
		gParametryPracy.gTempAlarmuBeczka = msg.toFloat();
		Beep(300);
		return true;
	}

	// Change options alarm glowica:
	if (msg.substring(0, 10) == "#alarmGLO:")
	{
		msg = msg.substring(msg.lastIndexOf(':'));
		msg.remove(0, 1);
		gParametryPracy.gTempAlarmuGlowica = msg.toFloat();
		Beep(300);
		return true;
	}

	// Change options G1 value:
	if (msg.substring(0, 7) == "#G1val:")
	{
		msg = msg.substring(msg.lastIndexOf(':'));
		msg.remove(0, 1);
		gParametryPracy.gMocGrzaniaG1 = msg.toInt();
		Beep(300);
		return true;
	}

	// Change options G2 value:
	if (msg.substring(0, 7) == "#G2val:")
	{
		msg = msg.substring(msg.lastIndexOf(':'));
		msg.remove(0, 1);
		gParametryPracy.gMocGrzaniaG2 = msg.toInt();
		Beep(300);
		return true;
	}

	// Change options G3 value:
	if (msg.substring(0, 7) == "#G3val:")
	{
		msg = msg.substring(msg.lastIndexOf(':'));
		msg.remove(0, 1);
		gParametryPracy.gMocGrzaniaG3 = msg.toInt();
		Beep(300);
		return true;
	}

	// Change options wartość zalania #WZalvalue:
	if (msg.substring(0, 11) == "#WZalvalue:")
	{
		msg = msg.substring(msg.lastIndexOf(':'));
		msg.remove(0, 1);
		gParametryPracy.gWartoscZalania = msg.toInt();
		// Serial.println(msg);
		Beep(300);
		return true;
	}

	//#ModReg:
	// Change options moduł mocy regulowany:
	if (msg.substring(0, 8) == "#ModReg:")
	{
		msg = msg.substring(msg.lastIndexOf(':'));
		msg.remove(0, 1);
		if (msg == "false")
		{
			gParametryPracy.gPWR_MOD = false;
		}
		else if (msg == "true")
		{
			gParametryPracy.gPWR_MOD = true;
		}
		Beep(300);
		return true;
	}

	//#CzujZal:
	// Change options czujnik zalania:
	if (msg.substring(0, 9) == "#CzujZal:")
	{
		msg = msg.substring(msg.lastIndexOf(':'));
		msg.remove(0, 1);
		if (msg == "false")
		{
			gParametryPracy.gCzujnikZalania = false;
		}
		else if (msg == "true")
		{
			gParametryPracy.gCzujnikZalania = true;
		}
		Beep(300);
		// Serial.println(msg);
		return true;
	}

	// TERMOSTAT:
	// Change options termostat stop value:
	if (msg.substring(0, 9) == "#TerStop:")
	{
		msg = msg.substring(msg.lastIndexOf(':'));
		msg.remove(0, 1);
		gParametryPracy.gTempTermostat_stop = msg.toFloat();
		Beep(300);
		return true;
	}

	// Change options termostat start value:
	if (msg.substring(0, 10) == "#TerStart:")
	{
		msg = msg.substring(msg.lastIndexOf(':'));
		msg.remove(0, 1);
		gParametryPracy.gTempTermostat_start = msg.toFloat();
		Beep(300);
		return true;
	}

	// ROZGRZEWANIE:

	// gTempStopRozgrzewania
	// #TmpSRgz:
	// Change options temp. kolumny czyli stop rozgrzewania value:
	if (msg.substring(0, 9) == "#TmpSRgz:")
	{
		msg = msg.substring(msg.lastIndexOf(':'));
		msg.remove(0, 1);
		gParametryPracy.gTempStopRozgrzewania = msg.toFloat();
		Beep(300);
		return true;
	}

	// ZALEWANIE:

	// A
	if (msg.substring(0, 10) == "#CzasZalA:")
	{
		msg = msg.substring(msg.lastIndexOf(':'));
		msg.remove(0, 1);
		gParametryPracy.gCzasZalania_1 = msg.toInt();
		Beep(300);
		return true;
	}
	if (msg.substring(0, 13) == "#PrzerwaZalA:")
	{
		msg = msg.substring(msg.lastIndexOf(':'));
		msg.remove(0, 1);
		gParametryPracy.gPrzerwaZalania_1 = msg.toInt();
		Beep(300);
		return true;
	}

	// B
	if (msg.substring(0, 10) == "#CzasZalB:")
	{
		msg = msg.substring(msg.lastIndexOf(':'));
		msg.remove(0, 1);
		gParametryPracy.gCzasZalania_2 = msg.toInt();
		Beep(300);
		return true;
	}
	if (msg.substring(0, 13) == "#PrzerwaZalB:")
	{
		msg = msg.substring(msg.lastIndexOf(':'));
		msg.remove(0, 1);
		gParametryPracy.gPrzerwaZalania_2 = msg.toInt();
		Beep(300);
		return true;
	}

	// C
	if (msg.substring(0, 10) == "#CzasZalC:")
	{
		msg = msg.substring(msg.lastIndexOf(':'));
		msg.remove(0, 1);
		gParametryPracy.gCzasZalania_3 = msg.toInt();
		Beep(300);
		return true;
	}
	if (msg.substring(0, 13) == "#PrzerwaZalC:")
	{
		msg = msg.substring(msg.lastIndexOf(':'));
		msg.remove(0, 1);
		gParametryPracy.gPrzerwaZalania_3 = msg.toInt();
		Beep(300);
		return true;
	}

	// STABILIZACJA
	// gCzasStabilizacji
	if (msg.substring(0, 9) == "#CzasStb:")
	{
		msg = msg.substring(msg.lastIndexOf(':'));
		msg.remove(0, 1);
		gParametryPracy.gCzasStabilizacji = msg.toInt();
		Beep(300);
		return true;
	}

	// Wartość gStb_G1pwm
	if (msg.substring(0, 12) == "#gStb_G1pwm:")
	{
		msg = msg.substring(msg.lastIndexOf(':'));
		msg.remove(0, 1);
		gParametryPracy.gCzasStabilizacji = msg.toInt();
		Beep(300);
		return true;
	}

	// bool gStb_G1 = false;
	if (msg.substring(0, 9) == "#gStb_G1:")
	{
		msg = msg.substring(msg.lastIndexOf(':'));
		msg.remove(0, 1);
		if (msg == "false")
		{
			gParametryPracy.gStb_G1 = false;
		}
		else if (msg == "true")
		{
			gParametryPracy.gStb_G1 = true;
		}
		Beep(300);
		return true;
	}

	// bool gStb_G2 = false;
	if (msg.substring(0, 9) == "#gStb_G2:")
	{
		msg = msg.substring(msg.lastIndexOf(':'));
		msg.remove(0, 1);
		if (msg == "false")
		{
			gParametryPracy.gStb_G2 = false;
		}
		else if (msg == "true")
		{
			gParametryPracy.gStb_G2 = true;
		}
		Beep(300);
		return true;
	}

	// bool gStb_G3 = false;
	if (msg.substring(0, 9) == "#gStb_G3:")
	{
		msg = msg.substring(msg.lastIndexOf(':'));
		msg.remove(0, 1);
		if (msg == "false")
		{
			gParametryPracy.gStb_G3 = false;
		}
		else if (msg == "true")
		{
			gParametryPracy.gStb_G3 = true;
		}
		Beep(300);
		return true;
	}

	// PRZEDGON:
	// gCyklePrzedgonu
	if (msg.substring(0, 9) == "#CklPrzg:")
	{
		msg = msg.substring(msg.lastIndexOf(':'));
		msg.remove(0, 1);
		gParametryPracy.gCyklePrzedgonu = msg.toInt();
		Beep(300);
		return true;
	}

	// gCzasOtwarciaZPrzedgonu
	if (msg.substring(0, 11) == "#CzasO_EZG:")
	{
		msg = msg.substring(msg.lastIndexOf(':'));
		msg.remove(0, 1);
		gParametryPracy.gCzasOtwarciaZPrzedgonu = msg.toInt();
		Beep(300);
		return true;
	}

	// gCzasZamknieciaZPrzedgonu
	if (msg.substring(0, 11) == "#CzasZ_EZG:")
	{
		msg = msg.substring(msg.lastIndexOf(':'));
		msg.remove(0, 1);
		gParametryPracy.gCzasZamknieciaZPrzedgonu = msg.toInt();
		Beep(300);
		return true;
	}

	// gPrzg_G1pwm
	if (msg.substring(0, 13) == "#gPrzg_G1pwm:")
	{
		msg = msg.substring(msg.lastIndexOf(':'));
		msg.remove(0, 1);
		gParametryPracy.gPrzg_G1pwm = msg.toInt();
		Beep(300);
		return true;
	}

	// gPrzg_G1
	if (msg.substring(0, 10) == "#gPrzg_G1:")
	{
		msg = msg.substring(msg.lastIndexOf(':'));
		msg.remove(0, 1);
		if (msg == "false")
		{
			gParametryPracy.gPrzg_G1 = false;
		}
		else if (msg == "true")
		{
			gParametryPracy.gPrzg_G1 = true;
		}
		Beep(300);
		return true;
	}

	// gPrzg_G2
	if (msg.substring(0, 10) == "#gPrzg_G2:")
	{
		msg = msg.substring(msg.lastIndexOf(':'));
		msg.remove(0, 1);
		if (msg == "false")
		{
			gParametryPracy.gPrzg_G2 = false;
		}
		else if (msg == "true")
		{
			gParametryPracy.gPrzg_G2 = true;
		}
		Beep(300);
		return true;
	}

	// gPrzg_G3
	if (msg.substring(0, 10) == "#gPrzg_G3:")
	{
		msg = msg.substring(msg.lastIndexOf(':'));
		msg.remove(0, 1);
		if (msg == "false")
		{
			gParametryPracy.gPrzg_G3 = false;
		}
		else if (msg == "true")
		{
			gParametryPracy.gPrzg_G3 = true;
		}
		Beep(300);
		return true;
	}

	// STABILIZACJA PRZEDGONU:
	// gCzasStabilizacjiPrzedgonu
	if (msg.substring(0, 15) == "#gCzas_StbPrzg:")
	{
		msg = msg.substring(msg.lastIndexOf(':'));
		msg.remove(0, 1);
		gParametryPracy.gCzasStabilizacjiPrzedgonu = msg.toInt();
		Beep(300);
		return true;
	}

	// PLUKANIE OLM:
	// gCzasPlukanieOLM
	if (msg.substring(0, 12) == "#gCzas_POLM:")
	{
		msg = msg.substring(msg.lastIndexOf(':'));
		msg.remove(0, 1);
		gParametryPracy.gCzasPlukanieOLM = msg.toInt();
		Beep(300);
		return true;
	}

	//********************************************
	// ODBIOR GONU:
	// gCzasZakonczeniaGon
	if (msg.substring(0, 12) == "#gCzas_ZAKG:")
	{
		msg = msg.substring(msg.lastIndexOf(':'));
		msg.remove(0, 1);
		gParametryPracy.gCzasZakonczeniaGon = msg.toInt();
		Beep(300);
		return true;
	}

	// gHisterezaG_open
	if (msg.substring(0, 12) == "#gHisG_open:")
	{
		msg = msg.substring(msg.lastIndexOf(':'));
		msg.remove(0, 1);
		gParametryPracy.gHisterezaG_open = msg.toFloat();
		Beep(300);
		return true;
	}

	// gHisterezaG_close
	if (msg.substring(0, 13) == "#gHisG_close:")
	{
		msg = msg.substring(msg.lastIndexOf(':'));
		msg.remove(0, 1);
		gParametryPracy.gHisterezaG_close = msg.toFloat();
		Beep(300);
		return true;
	}

	// gGon_G1pwm
	if (msg.substring(0, 12) == "#gGon_G1pwm:")
	{
		msg = msg.substring(msg.lastIndexOf(':'));
		msg.remove(0, 1);
		gParametryPracy.gGon_G1pwm = msg.toInt();
		Beep(300);
		return true;
	}

	// gGon_G1
	if (msg.substring(0, 9) == "#gGon_G1:")
	{
		msg = msg.substring(msg.lastIndexOf(':'));
		msg.remove(0, 1);
		if (msg == "false")
		{
			gParametryPracy.gGon_G1 = false;
		}
		else if (msg == "true")
		{
			gParametryPracy.gGon_G1 = true;
		}
		Beep(300);
		return true;
	}

	// gGon_G2
	if (msg.substring(0, 9) == "#gGon_G2:")
	{
		msg = msg.substring(msg.lastIndexOf(':'));
		msg.remove(0, 1);
		if (msg == "false")
		{
			gParametryPracy.gGon_G2 = false;
		}
		else if (msg == "true")
		{
			gParametryPracy.gGon_G2 = true;
		}
		Beep(300);
		return true;
	}

	// gGon_G3
	if (msg.substring(0, 9) == "#gGon_G3:")
	{
		msg = msg.substring(msg.lastIndexOf(':'));
		msg.remove(0, 1);
		if (msg == "false")
		{
			gParametryPracy.gGon_G3 = false;
		}
		else if (msg == "true")
		{
			gParametryPracy.gGon_G3 = true;
		}
		Beep(300);
		return true;
	}

	// Bufor
	if (msg.substring(0, 8) == "#gBufor:")
	{
		msg = msg.substring(msg.lastIndexOf(':'));
		msg.remove(0, 1);
		if (msg == "false")
		{
			gParametryPracy.Bufor = false;
		}
		else if (msg == "true")
		{
			gParametryPracy.Bufor = true;
		}
		Beep(300);
		return true;
	}

	// TBufor80
	if (msg.substring(0, 10) == "#TBufor80:")
	{
		msg = msg.substring(msg.lastIndexOf(':'));
		msg.remove(0, 1);
		gParametryPracy.TBufor80 = msg.toFloat();
		Beep(300);
		return true;
	}

	// TBufor60
	if (msg.substring(0, 10) == "#TBufor60:")
	{
		msg = msg.substring(msg.lastIndexOf(':'));
		msg.remove(0, 1);
		gParametryPracy.TBufor60 = msg.toFloat();
		Beep(300);
		return true;
	}

	// TBufor40
	if (msg.substring(0, 10) == "#TBufor40:")
	{
		msg = msg.substring(msg.lastIndexOf(':'));
		msg.remove(0, 1);
		gParametryPracy.TBufor40 = msg.toFloat();
		Beep(300);
		return true;
	}

	// ODBIOR POGONU:
	// gTempStopPogonu
	if (msg.substring(0, 9) == "#TmpSPGN:")
	{
		msg = msg.substring(msg.lastIndexOf(':'));
		msg.remove(0, 1);
		gParametryPracy.gTempStopPogonu = msg.toFloat();
		Beep(300);
		return true;
	}

	// CHLODZENIE GLOWICY:
	// gCzasChlodzeniaGlow
	if (msg.substring(0, 12) == "#gCzas_CHGL:")
	{
		msg = msg.substring(msg.lastIndexOf(':'));
		msg.remove(0, 1);
		gParametryPracy.gCzasChlodzeniaGlow = msg.toInt();
		Beep(300);
		return true;
	}

	// WIFI DATA:
	// gSSID
	if (msg.substring(0, 7) == "#gSSID:")
	{
		msg = msg.substring(msg.lastIndexOf(':'));
		msg.remove(0, 1);
		memset(gParametryPracy.gSSID, 0, sizeof(gParametryPracy.gSSID));
		memcpy(gParametryPracy.gSSID, msg.c_str(), msg.length());
		Beep(300);
		return true;
	}

	// gPASS
	if (msg.substring(0, 7) == "#gPASS:")
	{
		msg = msg.substring(msg.lastIndexOf(':'));
		msg.remove(0, 1);
		memset(gParametryPracy.gPASS, 0, sizeof(gParametryPracy.gPASS));
		memcpy(gParametryPracy.gPASS, msg.c_str(), msg.length());
		Beep(300);
		return true;
	}

	return false;
}

// 20 sek
void TimerCallback_CheckWifi(void *pArg)
{
	TimerCheckWifi_occured = true;
}

void TimerCallback_UpdTemp(void *pArg)
{
	TimerUpdTemp_occured = true;
}

void TimerCallback_UDP(void *pArg)
{
	TimerUDP_occured = true;
}

void TimerCallback_ALARM_ZAL(void *pArg)
{
	TimerALARM_ZAL_occured = true;
}


void TimerCallback_LCD(void *pArg)
{
	TimerLCD_occured = true;
}

void TimerUpdTemp_update()
{
	if (TimerUpdTemp_occured == true)
	{
		TimerUpdTemp_occured = false;

		gParametryPracy.gESP_CNTR++;

		gAutomat.AskModule();
		Akt_Temp();
		

		if (gAutomat.AlarmZalaniaStatus() == true)
		{
			Beep(300);
		}

		
		//Serial.println("Send txt data to client ...");
		//Serial.println(F(""));
		//Serial.println(F("xxxxxxxxxx OK !"));

		//Serial.println(XML.c_str());
		// Serial.println(gParametryPracy.gPrzerwaZalania_1);

		Build_TXT();
		bool ret = webSocket.broadcastTXT(XML.c_str());
		//Serial.println(ret);

		

		// fake memory eating:
		// fake_ptr = (byte*)malloc(128);
	}

}

void TimerLCD_update()
{
	if (TimerLCD_occured == true)
	{
		TimerLCD_occured = false;
		LCD_Update();
		//Serial.println(F("LCD"));
	}
}

void TimerCheckWifi_update()
{
	if (TimerCheckWifi_occured == true)
	{
		TimerCheckWifi_occured = false;
		WIFI_Check_Connection_timer();

		if (AwariaDS_number >= 10)
		{
			AWARIA_DS = true;
			//file_print_log("Awaria DS trwa dluzej jak 10 sek\r\n");
			Beep(300);
		}
		AwariaDS_number = 0;

		if (gAutomat.GetModuleCntr() > module_cntr)
		{
			module_live = true;
			//Serial.println("Module is live...");
		}
		else
		{
			module_live = false;
			//Serial.println("Module is off line...");
		}

		module_cntr = gAutomat.GetModuleCntr();
	}
}

void TimerUDP_update()
{
	if (TimerUDP_occured == true)
	{
		TimerUDP_occured = false;
		//UDP_broadcast_self();
		udp.broadcast("esp");
	}
}

void TimerALARM_ZAL_update()
{
	if(gAutomat.AlarmZalaniaStatus() == true)
	{
		webSocket.broadcastTXT("#al_zal_start#");
	}
}


void notFound(AsyncWebServerRequest *request)
{
	request->send(404, "text/plain", "Not found");
}

void onUpload(AsyncWebServerRequest *request)
{
	AsyncWebServerResponse *response = request->beginResponse_P(200, MIMETYPE_HTML, upload_html_gz, upload_html_gz_len);
	response->addHeader("Content-Encoding", "gzip");
	request->send(response);
}

void onReact(AsyncWebServerRequest *request)
{
	AsyncWebServerResponse *response = request->beginResponse_P(200, MIMETYPE_HTML, react_html_gz, react_html_gz_len);
	response->addHeader("Content-Encoding", "gzip");
	request->send(response);
}

void onFile(AsyncWebServerRequest *request)
{
	String str_tmp = "/" + String(gParametryPracy.FileName);
	AsyncWebServerResponse *response = request->beginResponse(LittleFS, str_tmp.c_str(), "application/octet-stream");
	request->send(response);
}

void onHandleUpdate(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
{
	LittleFS.end();

	static unsigned long startTimer;
	if (!index)
	{
		// disable interrupt timer
		setup_timer1_disable();

		startTimer = millis();
		const char *FILESIZE_HEADER{"FileSize"};

		//Serial.printf("UPLOAD: Receiving: '%s'\n", filename.c_str());

		if (!request->hasHeader(FILESIZE_HEADER))
		{
			request->send(400, MIMETYPE_HTML, "No filesize header present!");
			request->client()->close();
			//Serial.printf("UPLOAD: Aborted upload because missing filesize header.\n");
			return;
		}

		//Serial.printf("UPLOAD: fileSize: %s\n", request->header(FILESIZE_HEADER));

		if (request->header(FILESIZE_HEADER).toInt() >= MAX_FILESIZE)
		{
			request->send(400, MIMETYPE_HTML,
						  "Too large. (" + humanReadableSize(request->header(FILESIZE_HEADER).toInt()) +
							  ") Max size is " + humanReadableSize(MAX_FILESIZE) + ".");

			request->client()->close();
			//Serial.printf("UPLOAD: Aborted upload because filesize limit.\n");
			return;
		}

		uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;

		// This method unmounts the file system.
		// Use this method before updating the file system using OTA.
		LittleFS.end();

		Update.runAsync(true);

		if (!Update.begin(maxSketchSpace))
		{
			Update.printError(Serial);
		}
	}

	// Store or do something with the data...
	// Serial.printf("file: '%s' received %i bytes\ttotal: %i\n", filename.c_str(), len, index + len);

	if (!Update.hasError())
	{
		if (Update.write(data, len) != len)
		{
			Update.printError(Serial);
		}
	}

	if (final)
	{
		//Serial.printf("UPLOAD: Done. Received %i bytes in %.2fs which is %.2f kB/s.\n",
					//  index + len,
					//  (millis() - startTimer) / 1000.0,
					//  1.0 * (index + len) / (millis() - startTimer));

		if (Update.end(true))
		{
			//Serial.printf("Udało się: %u bytes\n", index + len);
			//Serial.print(F("OK: MCU restarting now ..."));
			// output: LittleFS format failed
			
			EEPROM_RESET();

			
		}
		else
		{
			Update.printError(Serial);
		}
	}
}

void onHome(AsyncWebServerRequest *request)
{
	// if (request->header("If-Modified-Since").equals(last_modified))
	// {
	//     request->send(304);
	//  } else
	//  {
	AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", index_html_gz, index_html_gz_len);
	response->addHeader("Content-Encoding", "gzip");
	// response->addHeader("Last-Modified", last_modified);
	request->send(response);
	// }
}

void onGraph(AsyncWebServerRequest *request)
{
	AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", graph_html_gz, graph_html_gz_len);
	response->addHeader("Content-Encoding", "gzip");
	request->send(response);
}

// function executed after async scan network done:
void WIFI_Scan_WS_Done(int networksFound)
{
	String json = "#wifiscan#";
	for (int i = 0; i < networksFound; i++)
	{
		json += ":";
		json += WiFi.SSID(i);
		json += ":";
		json += String(WiFi.RSSI(i));
		json += ":";

		switch (WiFi.encryptionType(i))
		{
		case 2:
		{
			json += "TKIP(WPA)";
		}
		break;
		case 5:
		{
			json += "WEP";
		}
		break;
		case 4:
		{
			json += "CCMP(WPA)";
		}
		break;
		case 7:
		{
			json += "NONE";
		}
		break;
		case 8:
		{
			json += "AUTO";
		}
		break;
		}
	}

	WiFi.scanDelete();
	//Serial.println(json.c_str());
	webSocket.broadcastTXT(json.c_str());
	Beep(300);
}

void WIFI_scan_complete()
{
	if (next_scan == true)
	{
		int n = WiFi.scanComplete();
		if (n >= 0)
		{
			String json = "#wifiscan#"; // było ""
			for (int i = 0; i < n; ++i)
			{
				json += ":";
				json += WiFi.SSID(i);
				json += ":";
				json += String(WiFi.RSSI(i));
				json += ":";

				switch (WiFi.encryptionType(i))
				{
				case 2:
				{
					json += "TKIP(WPA)";
				}
				break;
				case 5:
				{
					json += "WEP";
				}
				break;
				case 4:
				{
					json += "CCMP(WPA)";
				}
				break;
				case 7:
				{
					json += "NONE";
				}
				break;
				case 8:
				{
					json += "AUTO";
				}
				break;
				}
			}
			//Serial.println(json.c_str());
			next_scan = false;
			WiFi.scanDelete();
			webSocket.broadcastTXT(json.c_str());
		}
	}
}

void setup_Servers()
{
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("system");
	lcd.setCursor(0, 1);
	lcd.print("uruchamia");
	lcd.setCursor(0, 3);
	lcd.print("server www");
	delay(1000);

	server.on(
		"/update", HTTP_POST, [](AsyncWebServerRequest *request)
		{ request->send(200); },
		[](AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final)
		{
			onHandleUpdate(request, filename, index, data, len, final);
		});

	server.on("/", HTTP_OPTIONS, [](AsyncWebServerRequest *request)
			  {
        AsyncWebServerResponse* response = request->beginResponse(204);
        response->addHeader("Access-Control-Allow-Methods", "PUT,POST,GET,OPTIONS");
        response->addHeader("Access-Control-Allow-Headers", "Accept, Content-Type, Authorization, FileSize");
        response->addHeader("Access-Control-Allow-Credentials", "true");
        request->send(response); });

	//Serial.println("");
	//Serial.print(F("Uruchomiono server aktualizacji... "));

	server.on("/", HTTP_GET, onHome);
	server.on("/graph.html", HTTP_GET, onGraph);
	server.on("/react", HTTP_GET, onReact);
	server.on("/file", HTTP_GET, onFile);
	server.on("/upload.htm", HTTP_GET, onUpload);

	server.serveStatic("/", LittleFS, "/");

	server.onNotFound(notFound);
	server.begin();

	//Serial.println("");
	//Serial.println(F("ASYNC HTTP server started"));
	//Serial.println("");

	webSocket.begin();
	webSocket.onEvent(ON_WS_Event);

	//Serial.println(F("Websocket server started"));
	//Serial.println("");
}

void WIFI_ScanOnly()
{
	lcd.clear();
	lcd.setCursor(0, 0); // lcd.setCursor(col, row)
	lcd.print("szukam");
	lcd.setCursor(0, 1);
	lcd.print("wifi...");
	//Serial.println(F("Wifi scan start:"));
	//Serial.println(F(""));
	int n = WiFi.scanNetworks();
	//Serial.println(F(""));
	//Serial.println(F("scan done"));
	if (n == 0)
	{
		//Serial.println(F("no networks found"));
	}
	else
	{
		//Serial.print(n);
		//Serial.println(F(" networks found"));
		for (int i = 0; i < n; ++i)
		{
			lcd.clear();
			lcd.setCursor(0, 0);
			String ssid = String(WiFi.SSID(i));
			ssid = ssid.substring(0, 17);
			lcd.print(ssid.c_str());
			lcd.setCursor(0, 2);
			lcd.print("sygnal: ");
			lcd.setCursor(8, 2);
			lcd.print(WiFi.RSSI(i));
			lcd.setCursor(12, 2);
			lcd.print(" dbm ");

			//Serial.print(i + 1);
			//Serial.print(": ");
			//Serial.print(WiFi.SSID(i));
			//Serial.print(" (");
			//Serial.print(WiFi.RSSI(i));
			//Serial.print(")");
			//Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
			delay(1500);
		}
		WiFi.scanDelete();
	}
	Serial.println("");
}

void setup_UDP()
{
	//Serial.printf("");
	//Serial.printf("UDP async server on port %d\n", localPort);
	//Serial.printf("");
	
	IPAddress broadcastIP;
	if (gParametryPracy.gWIFI_MODE == 2)
	{
		//  #--AP--#
		// sTmpStr += WiFi.softAPIP().toString();
		
		//broadcastIP = IPAddress(192, 168, 4, 255);
	}
	else if (gParametryPracy.gWIFI_MODE == 0)
	{
		//  #--STA--#
		// sTmpStr += WiFi.localIP().toString();
		broadcastIP = ~uint32_t(WiFi.subnetMask()) | (uint32_t(WiFi.gatewayIP()));
	}
	
	// async udp:
	// udp.listenMulticast(IPAddress(239,1,2,3), localPort)
	/* if(udp.listenMulticast(broadcastIP, localPort))
	{
        Serial.print("UDP Listening on IP: ");
        Serial.println(WiFi.localIP());

        udp.onPacket([](AsyncUDPPacket packet)
		{
            Serial.print("UDP Packet Type: ");
            Serial.print(packet.isBroadcast()?"Broadcast":packet.isMulticast()?"Multicast":"Unicast");
            Serial.print(", From: ");
            Serial.print(packet.remoteIP());
            Serial.print(":");
            Serial.print(packet.remotePort());
            Serial.print(", To: ");
            Serial.print(packet.localIP());
            Serial.print(":");
            Serial.print(packet.localPort());
            Serial.print(", Length: ");
            Serial.print(packet.length());
            Serial.print(", Data: ");
            Serial.write(packet.data(), packet.length());
            Serial.println();
            //reply to the client
            packet.printf("Got %u bytes of data", packet.length());
        });
        //Send multicast
        //udp.print("Hello!");
    } */

	if(udp.listen(localPort))
	{
        //Serial.print("UDP Listening on IP: ");
        Serial.println(WiFi.localIP());
        udp.onPacket([](AsyncUDPPacket packet) {
            Serial.print("UDP Packet Type: ");
            Serial.print(packet.isBroadcast()?"Broadcast":packet.isMulticast()?"Multicast":"Unicast");
            Serial.print(", From: ");
            Serial.print(packet.remoteIP());
            Serial.print(":");
            Serial.print(packet.remotePort());
            Serial.print(", To: ");
            Serial.print(packet.localIP());
            Serial.print(":");
            Serial.print(packet.localPort());
            Serial.print(", Length: ");
            Serial.print(packet.length());
            Serial.print(", Data: ");
            Serial.write(packet.data(), packet.length());
            Serial.println();
            //reply to the client
            //packet.printf("Got %u bytes of data", packet.length());
        });
    }
}

bool setup_WIFI()
{
	// WiFi.disconnect();
	// WiFi.softAPdisconnect();
	// delay(50);
	// WiFi.setPhyMode(WIFI_PHY_MODE_11B);
	// WiFi.setOutputPower(20.5);//20.5

	ESP.eraseConfig();

	String strPASS(gParametryPracy.gPASS);
	// String strPASS = "prolongatum2021";
	// String strPASS = "prolongatum2022sx";
	if (strPASS.equals("brak") == true)
	{
		strPASS = "";
	}

	WiFi.setAutoConnect(true);
	WiFi.setAutoReconnect(true);
	// WiFi.persistent(true);

	WiFi.mode(WIFI_STA); // WIFI_STA or  WIFI_AP_STA

	WiFi.begin((const char *)gParametryPracy.gSSID, strPASS.c_str());
	// WiFi.begin("bubu_2",strPASS.c_str());

	// delay(100);

	wifi_station_set_hostname((char *)"sterowanie-destylatorem");

	//Serial.println("Dane WIFI:");
	//Serial.print("SSID: ");
	//Serial.println(gParametryPracy.gSSID);
	//Serial.print("PASS: ");
	//Serial.println(gParametryPracy.gPASS);
	//Serial.println("");

	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print(F("Laczenie"));
	lcd.setCursor(0, 1);
	lcd.print(F("do sieci..."));
	delay(500);

	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("CONNECTING WIFI");

	// long previousMillis = 0;
	// long interval = 500;
	// int    ledState = HIGH;
	byte retry = 0;

	while (WiFi.status() != WL_CONNECTED)
	{
		lcd.setCursor(retry, 1);
		lcd.print((char)1);
		delay(500);
		// FastDigitalWrite(LED_BUILTIN,HIGH);
		pcf8574.digitalWrite(BUZZER, LOW);
		//Serial.println(F("Connecting..."));
		delay(500);
		// FastDigitalWrite(LED_BUILTIN,LOW);
		pcf8574.digitalWrite(BUZZER, HIGH);
		retry++;

		Serial.println(retry);
		if (retry == 18)
		{
			if (WiFi.status() != WL_CONNECTED)
			{
				//Serial.println(F("Connect Error!"));
				pcf8574.digitalWrite(BUZZER, LOW);
				lcd.clear();
				lcd.setCursor(3, 0);
				lcd.print("Blad");
				lcd.setCursor(3, 1);
				lcd.print(F("polaczenia"));
				delay(400);
			}
			break;
		}
	}

	pcf8574.digitalWrite(BUZZER, LOW);

	bool sta_con = false;

	if (WiFi.status() == WL_CONNECTED)
	{
		/* Serial.println("");
		Serial.print(F("Connected to network:  "));
		Serial.print(gParametryPracy.gSSID);
		Serial.println("");
		Serial.println(F("STA IP address: "));
		Serial.print(WiFi.localIP());
		Serial.println("");
		Serial.printf("Gataway IP: %s\n", WiFi.gatewayIP().toString().c_str());
		Serial.println("");
		Serial.print("Subnet mask: ");
		Serial.println(WiFi.subnetMask());
		Serial.print("DNS #1, #2 IP: ");
		WiFi.dnsIP().printTo(Serial);
		Serial.print(", ");
		WiFi.dnsIP(1).printTo(Serial);
		Serial.println();
		Serial.printf("RSSI: %d dBm\n", WiFi.RSSI());
		Serial.println(); */

		gParametryPracy.ip_STA.ip1 = WiFi.localIP()[0];
		gParametryPracy.ip_STA.ip2 = WiFi.localIP()[1];
		gParametryPracy.ip_STA.ip3 = WiFi.localIP()[2];
		gParametryPracy.ip_STA.ip4 = WiFi.localIP()[3];

		gParametryPracy.gWIFI_MODE = 0; // 0 = STA
		gParametryPracy.gWIFI_RSSI = WiFi.RSSI();

		//======================================= ????????????????????
		//.....................................................................................
		// wifi_station_set_auto_connect(false);
		// wifi_station_set_reconnect_policy(true); // było odremowane
		// WiFi.persistent(false);
		//.....................................................................................
		//=======================================

		sta_con = true;

		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print(F("Polaczono z:"));
		lcd.setCursor(0, 1);
		String ssid = String(gParametryPracy.gSSID);
		ssid = ssid.substring(0, 19);
		lcd.print(ssid.c_str());
		lcd.setCursor(0, 2);
		lcd.print(F("Adres ip:"));
		lcd.setCursor(0, 3);
		lcd.print(WiFi.localIP());
		delay(2000);

		//Serial.println(F("Polaczono z siecia wifi"));

		//Serial.println("");
	}
	else // AP MODE
	{
		//Serial.println("");
		//Serial.print(F("Connect to station failed...."));
		//Serial.println("");
		sta_con = false;
		WiFi.disconnect();
		WiFi.setAutoConnect(false);
		WiFi.setAutoReconnect(false);
		WiFi.mode(WIFI_AP);
		// IPAddress ip(192,168,4,1);
		// IPAddress gateway(192,168,4,1);
		// IPAddress subnet(255,255,255,0);
		// WiFi.softAPConfig(ip,gateway,subnet);

		// hasło do wifi AP
		// hasło do strony admin = destylacja
		gAP_CONNECTED = WiFi.softAP("DST2AIR", "destylacja");

		//Serial.println("");
		//Serial.println(F("Uruchamiam punkt dostępu wifi ..."));
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print(F("Tworzenie"));
		lcd.setCursor(0, 1);
		lcd.print(F("Punktu dostepu ..."));
		lcd.setCursor(0, 2);
		lcd.print(F("WIFI"));

		delay(1000);

		if (gAP_CONNECTED == true)
		{
			//Serial.println("");
			//Serial.println("AP Ready");
			if (sta_con == false)
				gParametryPracy.gWIFI_MODE = 2;

			disable_lcd_update = true; // #!
			lcd.clear();
			lcd.setCursor(0, 0);
			lcd.print("Utworzono");
			lcd.setCursor(0, 1);
			lcd.print("Punktu dostepu ");
			lcd.setCursor(0, 2);
			lcd.print(WiFi.softAPIP());
			lcd.setCursor(0, 3);
			lcd.print("DST2AIR");
			lcd_wait = 2;				// #!
			disable_lcd_update = false; // #!
										// delay(2000);
		}
		else
		{
			//Serial.println("");
			//Serial.println("AP Failed!");
			lcd.clear();
			lcd.setCursor(0, 0);
			lcd.print("ERROR !!!");
			lcd.setCursor(0, 1);
			lcd.print("WIFI");
			delay(1000);
		}

		/* Serial.println("");
		Serial.println(F("AP IP address: "));
		Serial.print(WiFi.softAPIP());
		Serial.println("");
		Serial.printf("Gataway IP: %s\n", WiFi.gatewayIP().toString().c_str());
		Serial.println("");
		Serial.print("Subnet mask: ");
		Serial.println(WiFi.subnetMask());
		Serial.print("DNS #1, #2 IP: ");
		WiFi.dnsIP().printTo(Serial);
		Serial.print(", ");
		WiFi.dnsIP(1).printTo(Serial);
		Serial.println();
		Serial.printf("RSSI: %d dBm\n", WiFi.RSSI());
		Serial.println(); */

		gParametryPracy.ip_AP.ip1 = WiFi.softAPIP()[0];
		gParametryPracy.ip_AP.ip2 = WiFi.softAPIP()[1];
		gParametryPracy.ip_AP.ip3 = WiFi.softAPIP()[2];
		gParametryPracy.ip_AP.ip4 = WiFi.softAPIP()[3];
	}

	//Serial.println("");
	//WiFi.printDiag(Serial);
	//Serial.println("");

	if ((sta_con == false) && (gAP_CONNECTED == false))
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool WIFI_start_AP()
{
	WiFi.disconnect();
	WiFi.softAPdisconnect();
	WiFi.setPhyMode(WIFI_PHY_MODE_11B);
	WiFi.setOutputPower(20.5); // 20.5
	WiFi.setSleepMode(WIFI_NONE_SLEEP, 0);

	pcf8574.digitalWrite(BUZZER, LOW);

	WiFi.mode(WIFI_AP);

	// IPAddress ip(192,168,4,1);
	// IPAddress gateway(192,168,4,1);
	// IPAddress subnet(255,255,255,0);
	// WiFi.softAPConfig(ip,gateway,subnet);

	gAP_CONNECTED = WiFi.softAP("DST2AIR", "destylacja");

	//Serial.println("");
	//Serial.println(F("Uruchamiam punkt dostępu wifi ..."));
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("Tworzenie");
	lcd.setCursor(0, 1);
	lcd.print("Punktu dostepu ...");

	if (gAP_CONNECTED == true)
	{
		/* Serial.println("");
		Serial.println("AP Ready");

		Serial.println("");
		Serial.println(F("AP IP address: "));
		Serial.print(WiFi.softAPIP());
		Serial.println("");
		Serial.println(""); */

		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("Utworzono");
		lcd.setCursor(0, 1);
		lcd.print("Punktu dostepu ");
		lcd.setCursor(3, 3);
		lcd.print(WiFi.softAPIP());

		gParametryPracy.ip_AP.ip1 = WiFi.softAPIP()[0];
		gParametryPracy.ip_AP.ip2 = WiFi.softAPIP()[1];
		gParametryPracy.ip_AP.ip3 = WiFi.softAPIP()[2];
		gParametryPracy.ip_AP.ip4 = WiFi.softAPIP()[3];

		gParametryPracy.gWIFI_MODE = 2;

		lcd.clear();

		return true;
	}
	else
	{
		//Serial.println("");
		//Serial.println("AP Failed!");
		gParametryPracy.ip_AP.ip1 = 0;
		gParametryPracy.ip_AP.ip2 = 0;
		gParametryPracy.ip_AP.ip3 = 0;
		gParametryPracy.ip_AP.ip4 = 0;
		lcd.clear();
		lcd.setCursor(1, 0);
		lcd.print("Network Error !");
		lcd.setCursor(3, 1);
		lcd.print("Blad sieci ... ");
		lcd.setCursor(3, 2);
		lcd.clear();
		return false;
	}
}

// format bytes
String FormatBytes(size_t bytes)
{
	if (bytes < 1024)
	{
		return String(bytes) + "B";
	}
	else if (bytes < (1024 * 1024))
	{
		return String(bytes / 1024.0) + "KB";
	}
	else if (bytes < (1024 * 1024 * 1024))
	{
		return String(bytes / 1024.0 / 1024.0) + "MB";
	}
	else
	{
		return String(bytes / 1024.0 / 1024.0 / 1024.0) + "GB";
	}
}

// akt czas IRAM_ATTR lub  ICACHE_RAM_ATTR
void IRAM_ATTR onTimer1_ISR()
{
	gAutomat.UpdateTime();
}

void setup_timer1()
{
	// TIM_DIV1                         for shortest and most accurate timer
	// TIM_DIV16                       for medium time and medium accurate timer
	// TIM_DIV256                     for longest timer but least accurate. Default

	timer1_disable();
	timer1_attachInterrupt(onTimer1_ISR);
	timer1_isr_init();
	timer1_enable(TIM_DIV16, TIM_EDGE, TIM_LOOP);
	timer1_write(5000000); //        5000000 / 5 ticks per us from TIM_DIV16 == 1000,000 us interval = 1s
}

void setup_timer1_disable()
{
	timer1_detachInterrupt();
	timer1_disable();
}

// serial formatowanie wyjścia
//------------------------------------------
// Serial.println(10, BIN);
// Serial.println(10, HEX);
// Serial.println(10, OCT);
// Serial.println(10, DEC);

void setup_pcf8574()
{
   pcf8574.pinMode(EZ_GON, OUTPUT);      // zwor    - dioda
   pcf8574.pinMode(GRZALKA_1 , OUTPUT);  // grzalka - dioda
   pcf8574.pinMode(GRZALKA_2 , OUTPUT);
   pcf8574.pinMode(GRZALKA_3 , OUTPUT);
   pcf8574.pinMode(BUZZER, OUTPUT);    // buzzer
   pcf8574.pinMode(EZ_WODA, OUTPUT);    // woda
}

void setup_pcf8574_2()
{
   //----------expander
  pcf8574.begin( pcf8574_addr);
  pcf8574.digitalWrite(BUZZER, LOW);
  pcf8574.digitalWrite(EZ_GON, HIGH);
  pcf8574.digitalWrite(GRZALKA_1 , HIGH);
  pcf8574.digitalWrite(GRZALKA_2 , HIGH);
  pcf8574.digitalWrite(GRZALKA_3 , HIGH);
  pcf8574.digitalWrite(EZ_WODA , HIGH);	
}

void setup_DS()
{
	// DS18b20
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print(F("szukam..."));
	lcd.setCursor(0, 2);
	lcd.print(F("termometrow"));
	//Serial.println(F("Szukam termometrow...."));

	delay(1000);

	sensor_1.begin();
	if (sensor_1.getDeviceCount() != 1)
	{
		//Serial.println(F("Czujnik temperatury nr: 1 nie podlaczony !!!"));
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("DS18B20: 1");
		lcd.setCursor(0, 2);
		lcd.print("KOLUMNA");
		lcd.setCursor(9, 2);
		lcd.print("BRAK !!!");
		DS18b20_1 = false;
		pcf8574.digitalWrite(BUZZER, HIGH);
		delay(500);
		pcf8574.digitalWrite(BUZZER, LOW);
		delay(1000);
	}
	else
	{
		//Serial.println(F("Czujnik temperatury nr: 1 OK"));
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("DS18B20: 1");
		lcd.setCursor(0, 2);
		lcd.print("KOLUMNA");
		lcd.setCursor(9, 2);
		lcd.print("OK");
		DS18b20_1 = true;
		pcf8574.digitalWrite(BUZZER, HIGH);
		delay(500);
		pcf8574.digitalWrite(BUZZER, LOW);
		delay(500);
	}

	sensor_2.begin();
	if (sensor_2.getDeviceCount() != 1)
	{
		//Serial.println(F("Czujnik temperatury nr: 2 nie podlaczony !!!"));
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("DS18b20: 2");
		lcd.setCursor(0, 2);
		lcd.print("ZBIORNIK");
		lcd.setCursor(9, 2);
		lcd.print("BRAK !!!");
		DS18b20_2 = false;
		pcf8574.digitalWrite(BUZZER, HIGH);
		delay(500);
		pcf8574.digitalWrite(BUZZER, LOW);
		delay(1000);
	}
	else
	{
		//Serial.println(F("Czujnik temperatury nr: 2 OK"));
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("DS18B20: 2");
		lcd.setCursor(0, 2);
		lcd.print("ZBIORNIK");
		lcd.setCursor(9, 2);
		lcd.print("OK");
		DS18b20_2 = true;
		pcf8574.digitalWrite(BUZZER, HIGH);
		delay(500);
		pcf8574.digitalWrite(BUZZER, LOW);
		delay(500);
	}

	sensor_3.begin();
	if (sensor_3.getDeviceCount() != 1)
	{
		//Serial.println(F("Czujnik temperatury nr: 3 nie podlaczony !!!"));
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("DS18B20: 3");
		lcd.setCursor(0, 2);
		lcd.print("GLOWICA");
		lcd.setCursor(9, 2);
		lcd.print("BRAK !!!");
		DS18b20_3 = false;
		pcf8574.digitalWrite(BUZZER, HIGH);
		delay(500);
		pcf8574.digitalWrite(BUZZER, LOW);
		delay(1000);
	}
	else
	{
		//Serial.println(F("Czujnik temperatury nr: 3 OK"));
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("DS18B20: 3");
		lcd.setCursor(0, 2);
		lcd.print("GLOWICA");
		lcd.setCursor(9, 2);
		lcd.print("OK");
		DS18b20_3 = true;
		pcf8574.digitalWrite(BUZZER, HIGH);
		delay(500);
		pcf8574.digitalWrite(BUZZER, LOW);
		delay(500);
	}

	//Serial.println(F(""));

	lcd.clear();

	if (DS18b20_1 == false || DS18b20_2 == false || DS18b20_3 == false)
	{
		for (int i = 0; i < 7; i++)
		{
			lcd.clear();
			lcd.setCursor(0, 0);
			lcd.print("AWARIA !!!");
			lcd.setCursor(0, 1);
			lcd.print("Czujnika temperatury");
			lcd.setCursor(6, 2);
			lcd.print("DS18b20");

			if (DS18b20_1 == false)
			{
				lcd.setCursor(0, 3);
				lcd.print("KOLUMNA");
			}

			if (DS18b20_2 == false)
			{
				lcd.setCursor(5, 3);
				lcd.print("ZBIORNIK");
			}

			if (DS18b20_3 == false)
			{
				lcd.setCursor(9, 3);
				lcd.print("GLOWICA");
			}

			pcf8574.digitalWrite(BUZZER, LOW);
			delay(800);
			lcd.clear();
			pcf8574.digitalWrite(BUZZER, HIGH);
			delay(800);
		}
		pcf8574.digitalWrite(BUZZER, LOW);
	}

	sensor_1.setWaitForConversion(false);
	sensor_1.setResolution(12);
	sensor_2.setWaitForConversion(false);
	sensor_2.setResolution(12);
	sensor_3.setWaitForConversion(false);
	sensor_3.setResolution(12);
}

void setup_I2C()
{
	byte address, error;
	int nDevices = 0;

	// expander
	// 0x38 = 56
	// 0x20 = 32

	// lcd
	// ox3F
	Serial.println(" Skanowanie   I2C");

	Serial.println("");

	Wire.begin();

	for (address = 1; address < 127; address++)
	{
		Wire.beginTransmission(address);
		error = Wire.endTransmission();

		if (error == 0)
		{
			if ((address == 32) || (address == 56))
			{
				Serial.print("I2C:  pcf8574 expander:  ");
				Serial.println(address, HEX);
				pcf8574_addr = address;
			}

			if ((address != 32) && (address != 56)) // 0x20 = 32 albo 0x38 = 56 --->  pcf8574 expander
			{
				Serial.print("I2C:  LCD display:   ");
				Serial.println(address, HEX);
				lcd_addr = address;
				// break;
			}

			nDevices++;
		}
	}

	// pcf8574

	if (nDevices == 0)
		Serial.println("Wogule nie znaleziono urządzeń I2C \n");

	if (lcd_addr == 0)
		Serial.println("Nie znaleziono I2C LCD wyświetlacza\n");

	if (pcf8574_addr == 0)
		Serial.println("Nie znaleziono PCF8574 expandera\n");
}

void setup_LCD()
{
	// --.LCD.--  lcd characters localized
	lcd.begin(16, 2);
	lcd.init(lcd_addr);
	lcd.backlight();
	lcd.createChar(0, heart);
	lcd.createChar(1, invert);
	lcd.createChar(2, smiley);
	lcd.createChar(3, char_ą);
	lcd.createChar(4, char_ó);
	lcd.createChar(5, smile1);
	lcd.createChar(6, smile2);
	lcd.createChar(7, hglass);
	lcd.clear();
}

String file_load_from_file(String file_name)
{
  String result = "";
  
  File this_file = LittleFS.open(file_name, "r");
  if (!this_file) { // failed to open the file, retrn empty result
    return result;
  }
  while (this_file.available()) {
      result += (char)this_file.read();
  }
  
  this_file.close();
  return result;
}

bool file_write_to_file(String file_name, String contents)
{  
  File this_file = LittleFS.open(file_name, "w+");
  if (!this_file) { // failed to open the file, return false
    return false;
  }
  int bytesWritten = this_file.print(contents);
 
  if (bytesWritten == 0) { // write failed
      return false;
  }
   
  this_file.close();
  return true;
}

/* void file_print_log(String message)
{
	String sTmpStr;
	sTmpStr += String(gParametryPracy.gCzasProcesuGodz) +
			   ":" + String(gParametryPracy.gCzasProcesuMin) +
			   ":" + String(gParametryPracy.gCzasProcesuSek);
	File LogFile = fileSystem->open("log.txt", "a");
	if (!LogFile)
	{
		Serial.println("");
		Serial.println("Error opening log.txt file for writing");
	}
	LogFile.print(sTmpStr + ":  " + message);

	LogFile.close();
} */

void setup_filesystem()
{
	// LittleFS.begin();

	// SPIFFS.end() or LittleFS.end() This method unmounts the file system.
	// Use this method before updating the file system using OTA



	fsOK = LittleFS.begin();
	Serial.println(fsOK ? F("Filesystem initialized.") : F("Filesystem init failed!"));
	file_system_status();
	file_list_print();

	
	/* Serial.println(F("Create fresh log file ..."));
	Serial.println("");

	File LogFile = fileSystem->open("log.txt", "w");
	if (!LogFile)
	{
		Serial.println("");
		Serial.println("Error opening fresh log.txt file for writing");
	}
	LogFile.close(); */
	
}

void setup_Buttons()
{
	// START i STOP
	// ---------------
	pinMode(BUTTON_1, INPUT); // BUTTON_1
	button1.init(BUTTON_1, LOW);
	adjustableButtonConfig1.setEventHandler(handleEvent1);
	adjustableButtonConfig1.setFeature(ButtonConfig::kFeatureLongPress);
	adjustableButtonConfig1.setFeature(ButtonConfig::kFeatureSuppressAfterLongPress);
	adjustableButtonConfig1.setLongPressDelay(4000);

	// EPROM RESET + TERMOSTAT START STOP
	// ---------------------------------------------------------------------------------------------
	pinMode(BUTTON_2, INPUT);
	button2.init(BUTTON_2, LOW);
	adjustableButtonConfig2.setEventHandler(handleEvent2);
	adjustableButtonConfig2.setFeature(ButtonConfig::kFeatureLongPress);
	adjustableButtonConfig2.setFeature(ButtonConfig::kFeatureSuppressAfterLongPress);
	adjustableButtonConfig2.setLongPressDelay(10000);
	// ----------------------------------------------------------------------------------------------
}

void setup_Timers()
{
	// Check wifi connection after some seconds
	TimerCheckWifi_occured = false;
	os_timer_setfn(&myTimer_CheckWifi, TimerCallback_CheckWifi, NULL);
	os_timer_arm(&myTimer_CheckWifi, 35000, true);

	// Get temperatures and send data to clients 
	// typical every 1s
	TimerUpdTemp_occured = false;
	os_timer_setfn(&myTimer_UpdTemp, TimerCallback_UpdTemp, NULL);
	os_timer_arm(&myTimer_UpdTemp, 1000, true);

	// broadcast IP every few seconds
	TimerUDP_occured = false;
	os_timer_setfn(&myTimer_UDP, TimerCallback_UDP, NULL);
	os_timer_arm(&myTimer_UDP, 15000, true);

	// LCD update typical every 1s
	TimerLCD_occured = false;
	os_timer_setfn(&myTimer_LCD, TimerCallback_LCD, NULL);
	os_timer_arm(&myTimer_LCD, 1000, true);

	// AlarmZalania update typical every few
	TimerALARM_ZAL_occured = false;
	os_timer_setfn(&myTimer_ALARM_ZAL, TimerCallback_ALARM_ZAL, NULL);
	os_timer_arm(&myTimer_ALARM_ZAL, 5000, true);

	// Hardware timer for time management
	setup_timer1(); // IRQ timer


	// ITimer.attachInterruptInterval(1000, onTimer1_ISR);
}

void setup_Info()
{
	/* Serial.println(F(""));
	Serial.println(F(""));
	Serial.print(F("Free Memory: "));
	Serial.print(system_get_free_heap_size());
	Serial.println(F(""));

	Serial.print(F("Flash Memory size: "));
	Serial.print(getFlashChipRealSize());
	Serial.println(F(""));
	delay(150);

	Serial.println("");
	Serial.print(F("ChipID:  "));
	Serial.print(ESP.getChipId(), DEC);
	Serial.println("");
	Serial.print(F("Flash ChipID:  "));
	Serial.print(ESP.getFlashChipId(), DEC);
	Serial.println("");
	Serial.print(F("Flash SIZE:  "));
	Serial.print(ESP.getFlashChipSize());
	Serial.println("");
	Serial.print(F("Flash chip speed:  "));
	Serial.print(ESP.getFlashChipSpeed());
	Serial.println("");
	Serial.print(F("\n"));
	delay(150); */
}

void setup_Mdns()
{
	/* if (MDNS.begin("sterowanie"))
	{
		Serial.println(F("MDNS started with name: esp"));
	} */

	// Add service to MDNS-SD
	// MDNS.addService("http", "tcp", 80);
}

void setup_ArduinoCloud()
{
	// Serial.println(F("CLOUD: comunicate with cloud.arduino.cc"));
	// Serial.println(gParametryPracy.gCloudLogin);
	// Serial.println(gParametryPracy.gCloudKey);
	// Serial.println("");
	// ArduinoCloud.setBoardId(gParametryPracy.gCloudLogin);
	// ArduinoCloud.setSecretDeviceKey(gParametryPracy.gCloudKey);
	// ArduinoCloud.addProperty(gParametryPracy.gTempKolumna, READ, 1 * SECONDS, NULL);
	// ArduinoIoTPreferredConnection.begin("","");
	// ArduinoCloud.begin(ArduinoIoTPreferredConnection,false);
	// setDebugMessageLevel(4);
	// ArduinoCloud.printDebugInfo();
	// Serial.println("");
}

void setup()
{
	system_update_cpu_freq(160); // 160 albo 80 Mhz

	Serial.begin(115200); // 115200
	delay(100);         
   
	timer_buzzer.stop();
	timer_ZGON.stop();
	setup_pcf8574();
	setup_I2C();
	setup_pcf8574_2();
	setup_LCD();

	lcd.setCursor(5, 0);
	lcd.print("STEROWANIE");
	lcd.setCursor(4, 1);
	lcd.print("DESTYLATOREM");
	lcd.setCursor(2, 3);
	lcd.print("2022 (c) v 2.3.5");
	delay(4000);

	setup_filesystem();
	//file_print_log("Setup file system ...\r\n");
	//file_print_log("Init pcf8574 ...\r\n");
	//file_print_log("Init pcf8574 stage 2\r\n");
	//file_print_log("Init I2C ...\r\n");
	//file_print_log("Init LCD display\r\n");
	
	//file_print_log("Get system info ...\r\n");
	// setup_Info();
	
	//file_print_log("Init DS temp.sensors\r\n");
	setup_DS();
	
	//file_print_log("Init eeprom memory data\r\n");
	setup_eeprom();
	
	memset(TempCharBuffer, 0, sizeof(TempCharBuffer));

	pinMode(LED_BUILTIN, OUTPUT); // LED_BUILTIN is a esp blue led
	delay(150);

	lcd.clear();

	//file_print_log("Scan for WIFI networks\r\n");
	WIFI_ScanOnly();

	//file_print_log("Init WIFI ...\r\n");
	setup_WIFI();

	//file_print_log("Setup servers ...\r\n");
	setup_Servers();
	
	//file_print_log("Init UDP ...\r\n");
	setup_UDP();

	//file_print_log("Init hardware buttons ...\r\n");
	setup_Buttons();

	lcd.clear();

	delay(200);
	Beep(300);

	sensor_1.requestTemperatures();
	sensor_2.requestTemperatures();
	sensor_3.requestTemperatures();

	//file_print_log("Init pinger ...\r\n");
	pinger.OnReceive([](const PingerResponse &response)
					 {
    if (response.ReceivedResponse)
    {
      Serial.printf(
        "Reply from %s: bytes=%d time=%lums TTL=%d\n",
        response.DestIPAddress.toString().c_str(),
        response.EchoMessageSize - sizeof(struct icmp_echo_hdr),
        response.ResponseTime,
        response.TimeToLive);
    }
    else
    {
      Serial.printf("Request timed out.\n");
    }

    // Return true to continue the ping sequence.
    // If current event returns false, the ping sequence is interrupted.
    return false; });

	// setup_Mdns();

	//file_print_log("Init timers ...\r\n");
	setup_Timers();

	// Set up ESP watchdog
	//file_print_log("Setup watchdog ...\r\n");
	ESP.wdtDisable();
	ESP.wdtEnable(WDTO_8S);

	// setup_ArduinoCloud();

	Serial.println("");
	Serial.println(F("Sterownik gotowy do roboty !!!"));
	Serial.println("");
	Serial.println("");

	//file_print_log("\r\n|********************|\r\n|******* SYSTEM *****|\r\n|******** LOG *******|\r\n|********************|\r\nSterownik gotowy ...\r\n");
		
}

void loop()
{
	gAutomat.Run();

	
	TimerUpdTemp_update();
	TimerCheckWifi_update();
	TimerUDP_update();
	TimerLCD_update();
	TimerALARM_ZAL_update();

	button1.check();
	button2.check();
	
	// Buzzer
	if (timer_buzzer.onRestart())
	{
		pcf8574.digitalWrite(BUZZER, LOW);
	}
	
	if (AWARIA_DS == true)
	{
		timer_ZGON.stop();
	}

	delay(0);

	// Proces_UDP_message();
	// MDNS.update();
	// ArduinoCloud.update();
}

// przycisk start
void handleEvent1(AceButton *, uint8_t eventType, uint8_t buttonState)
{
	switch (eventType)
	{
	// załacza start automata
	case AceButton::kEventReleased:
	{
		Beep(1000);
		if(gAutomat.AlarmZalaniaStatus() == true)
		{
			gAutomat.AlarmZalaniaStop();
			lcd.clear();
			break;
		}

		if (gAutomat.Status() == SCODE_NIC || gAutomat.Status() == TERMOSTAT)
		{
			if (DS18b20_1 == false || DS18b20_2 == false || DS18b20_3 == false)
			{
				disable_lcd_update = true;
				//Serial.println(F("AWARIA DS Auto wylaczone ...."));
				lcd.clear();
				lcd.setCursor(0, 0);
				lcd.print("AWARIA DS!");
				lcd.setCursor(0, 1);
				lcd_wait = 4;
				disable_lcd_update = false;
			}
			else
			{
				LCD_Auto_start();
				AwariaDS_number = 0;
				disable_lcd_update = false;
				//Serial.println(F("Automat START ...."));
				gAutomat.TermostatStop();
				gAutomat.Start();
			}
		}
		else
		{
			LCD_muka();
		}
	}
	break;

		// długie naciśnięcie 4 sek przycisku start wykonuje stop auto
	case AceButton::kEventLongPressed:
	{
		//Serial.println(F("STOP AUTOMATA"));
		LCD_Auto_stop();
		gAutomat.Stop();
		Beep(1000);
	}
	break;
	}
}

// TERMOSTAT +  10.sek reset EEPROMU
void handleEvent2(AceButton *, uint8_t eventType, uint8_t buttonState)
{
	switch (eventType)
	{
	// krótkie szybkie
	// załącza termostat  lub wyłącza termostacik
	case AceButton::kEventReleased:
	{
		switch (gAutomat.Status())
		{
		case TERMOSTAT:
		{
			Beep(1000);
			gAutomat.TermostatStop();
			//Serial.println(F("Termostat STOP"));
			webSocket.broadcastTXT("#stoptermo#");
			LCD_termostat_stop();
		}
		break;

		case SCODE_NIC: // nic nie robienie czekamy aż naciśnie guzik
		{
			Beep(1000);
			gAutomat.TermostatStart();
			//println(F("Termostat START"));
			webSocket.broadcastTXT("#starttermo#");
			LCD_termostat_start();
		}
		break;
		}
		break;
	}

	// długie naciśnięcie około 8 sekund resetuje pamięć ustawień sterownika sterownika
	case AceButton::kEventLongPressed:
	{
		if (gAutomat.IsStarted() == false)
		{
			webSocket.broadcastTXT("#epromreset#");
			Beep(2000);
			EEPROM_RESET();
		}
		break;
	}
	}
}

void LCD_Grzanie()
{
	disable_lcd_update = true;
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("Moc: ");
	lcd.setCursor(5, 0);

	int pwr = 0;
	if (gParametryPracy.G1 == true)
		pwr += gParametryPracy.gMocGrzaniaG1;
	if (gParametryPracy.G2 == true)
		pwr += gParametryPracy.gMocGrzaniaG2;
	if (gParametryPracy.G3 == true)
		pwr += gParametryPracy.gMocGrzaniaG3;

	lcd.print(pwr);
	lcd.setCursor(10, 0);
	lcd.print("W");
	lcd.setCursor(2, 2);
	lcd.print(F("GRZANIE"));
	lcd_wait = 2;
	disable_lcd_update = false;
}

void LCD_GX_start()
{
	disable_lcd_update = true;
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print(F("Moc: "));
	lcd.setCursor(5, 0);

	int pwr = 0;
	if (gParametryPracy.G1 == true)
		pwr += gParametryPracy.gMocGrzaniaG1;
	if (gParametryPracy.G2 == true)
		pwr += gParametryPracy.gMocGrzaniaG2;
	if (gParametryPracy.G3 == true)
		pwr += gParametryPracy.gMocGrzaniaG3;

	lcd.print(pwr);
	lcd.setCursor(10, 0);
	lcd.print("W");
	lcd.setCursor(2, 2);
	lcd.print(F("GRZANIE    START"));
	lcd_wait = 6;
	disable_lcd_update = false;
}

void LCD_GX_stop()
{
	disable_lcd_update = true;
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print(F("Moc: "));
	lcd.setCursor(5, 0);

	int pwr = 0;
	if (gParametryPracy.G1 == true)
		pwr += gParametryPracy.gMocGrzaniaG1;
	if (gParametryPracy.G2 == true)
		pwr += gParametryPracy.gMocGrzaniaG2;
	if (gParametryPracy.G3 == true)
		pwr += gParametryPracy.gMocGrzaniaG3;

	lcd.print(pwr);
	lcd.setCursor(10, 0);
	lcd.print("W");
	lcd.setCursor(2, 2);
	lcd.print(F("GRZANIE   STOP"));
	lcd_wait = 6;
	disable_lcd_update = false;
}

void LCD_G1_start()
{
	disable_lcd_update = true;
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print(F("Moc: "));
	lcd.setCursor(5, 0);

	int pwr = 0;
	if (gParametryPracy.G1 == true)
		pwr += gParametryPracy.gMocGrzaniaG1;
	if (gParametryPracy.G2 == true)
		pwr += gParametryPracy.gMocGrzaniaG2;
	if (gParametryPracy.G3 == true)
		pwr += gParametryPracy.gMocGrzaniaG3;

	lcd.print(pwr);
	lcd.setCursor(10, 0);
	lcd.print("W");
	lcd.setCursor(2, 2);
	lcd.print(F("GRZALKA 1  START"));
	lcd_wait = 6;
	disable_lcd_update = false;
}

void LCD_blank()
{
	disable_lcd_update = true;
	lcd.clear();
	lcd_wait = 1;
	disable_lcd_update = false;
}

void LCD_G1_stop()
{
	disable_lcd_update = true;
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print(F("Moc: "));
	lcd.setCursor(5, 0);

	int pwr = 0;
	if (gParametryPracy.G1 == true)
		pwr += gParametryPracy.gMocGrzaniaG1;
	if (gParametryPracy.G2 == true)
		pwr += gParametryPracy.gMocGrzaniaG2;
	if (gParametryPracy.G3 == true)
		pwr += gParametryPracy.gMocGrzaniaG3;

	lcd.print(pwr);
	lcd.setCursor(10, 0);
	lcd.print("W");
	lcd.setCursor(2, 2);
	lcd.print(F("GRZALKA 1  STOP"));
	lcd_wait = 6;
	disable_lcd_update = false;
}

void LCD_G2_start()
{
	disable_lcd_update = true;
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print(F("Moc: "));
	lcd.setCursor(5, 0);

	int pwr = 0;
	if (gParametryPracy.G1 == true)
		pwr += gParametryPracy.gMocGrzaniaG1;
	if (gParametryPracy.G2 == true)
		pwr += gParametryPracy.gMocGrzaniaG2;
	if (gParametryPracy.G3 == true)
		pwr += gParametryPracy.gMocGrzaniaG3;

	lcd.print(pwr);
	lcd.setCursor(10, 0);
	lcd.print("W");
	lcd.setCursor(2, 2);
	lcd.print(F("GRZALKA 2  START"));
	lcd_wait = 6;
	disable_lcd_update = false;
}

void LCD_G2_stop()
{
	disable_lcd_update = true;
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print(F("Moc: "));
	lcd.setCursor(5, 0);

	int pwr = 0;
	if (gParametryPracy.G1 == true)
		pwr += gParametryPracy.gMocGrzaniaG1;
	if (gParametryPracy.G2 == true)
		pwr += gParametryPracy.gMocGrzaniaG2;
	if (gParametryPracy.G3 == true)
		pwr += gParametryPracy.gMocGrzaniaG3;

	lcd.print(pwr);
	lcd.setCursor(10, 0);
	lcd.print("W");
	lcd.setCursor(2, 2);
	lcd.print(F("GRZALKA 2  STOP"));
	lcd_wait = 6;
	disable_lcd_update = false;
}

void LCD_G3_start()
{
	disable_lcd_update = true;
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print(F("Moc: "));
	lcd.setCursor(5, 0);
	int pwr = 0;
	if (gParametryPracy.G1 == true)
		pwr += gParametryPracy.gMocGrzaniaG1;
	if (gParametryPracy.G2 == true)
		pwr += gParametryPracy.gMocGrzaniaG2;
	if (gParametryPracy.G3 == true)
		pwr += gParametryPracy.gMocGrzaniaG3;
	lcd.print(pwr);
	lcd.setCursor(10, 0);
	lcd.print("W");
	lcd.setCursor(2, 2);
	lcd.print(F("GRZALKA 3  START"));
	lcd_wait = 6;
	disable_lcd_update = false;
}

void LCD_G3_stop()
{
	disable_lcd_update = true;
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print(F("Moc: "));
	lcd.setCursor(5, 0);

	int pwr = 0;
	if (gParametryPracy.G1 == true)
		pwr += gParametryPracy.gMocGrzaniaG1;
	if (gParametryPracy.G2 == true)
		pwr += gParametryPracy.gMocGrzaniaG2;
	if (gParametryPracy.G3 == true)
		pwr += gParametryPracy.gMocGrzaniaG3;

	lcd.print(pwr);
	lcd.setCursor(10, 0);
	lcd.print("W");
	lcd.setCursor(2, 2);
	lcd.print(F("GRZALKA 3  STOP"));
	lcd_wait = 6;
	disable_lcd_update = false;
}

void LCD_zawor_ON()
{
	disable_lcd_update = true;
	lcd.clear();
	lcd.setCursor(2, 1);
	lcd.print(F("ZAWOR ON"));
	lcd.setCursor(11, 1);
	lcd.print((char)0);
	lcd.setCursor(2, 3);
	lcd.print(gParametryPracy.gZaworGonCntr);
	lcd_wait = 2;
	disable_lcd_update = false;
}

void LCD_zawor_OFF()
{
	disable_lcd_update = true;
	lcd.clear();
	lcd.setCursor(2, 1);
	lcd.print(F("ZAWOR OFF"));
	lcd.setCursor(12, 1);
	lcd.print((char)0);
	lcd.setCursor(2, 3);
	lcd.print(gParametryPracy.gZaworGonCntr);
	lcd_wait = 2;
	disable_lcd_update = false;
}

void LCD_termostat_start()
{
	disable_lcd_update = true;
	lcd.clear();
	lcd.setCursor(2, 1);
	lcd.print(F("TERMOSTAT  START"));

	lcd.setCursor(0, 3);
	lcd.print(F("Moc: "));
	lcd.setCursor(5, 3);

	int pwr = 0;
	// if(gParametryPracy.G1 == true)
	pwr += gParametryPracy.gMocGrzaniaG1;
	// if(gParametryPracy.G2 == true)
	pwr += gParametryPracy.gMocGrzaniaG2;
	// if(gParametryPracy.G3 == true)
	pwr += gParametryPracy.gMocGrzaniaG3;

	lcd.print(pwr);
	lcd.setCursor(10, 3);
	lcd.print("W");

	lcd_wait = 4;
	disable_lcd_update = false;

	termo_run_lcd_once = true;
}

void LCD_KONIEC()
{
	disable_lcd_update = true;
	lcd.clear();
	lcd.setCursor(4, 1);
	lcd.print(F("ZAKONCZONO"));
	lcd_wait = 12;
	disable_lcd_update = false;
}

void LCD_grzalki()
{
	// disable_lcd_update = true;
	// lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print(F("GRZALKI:"));

	lcd.setCursor(2, 1);
	lcd.print("G1:  ");
	if (gParametryPracy.G1 == true)
	{
		lcd.setCursor(8, 1);
		lcd.print("ON ");
	}
	else
	{
		lcd.setCursor(8, 1);
		lcd.print("OFF");
	}

	lcd.setCursor(2, 2);
	lcd.print("G2:  ");
	if (gParametryPracy.G2 == true)
	{
		lcd.setCursor(8, 2);
		lcd.print("ON ");
	}
	else
	{
		lcd.setCursor(8, 2);
		lcd.print("OFF");
	}

	lcd.setCursor(2, 3);
	lcd.print("G3:");
	if (gParametryPracy.G3 == true)
	{
		lcd.setCursor(8, 3);
		lcd.print("ON ");
	}
	else
	{
		lcd.setCursor(8, 3);
		lcd.print("OFF");
	}

	// Start > blink mark
	if (gAutomat.IsStarted() == true)
	{
		if (blinkTermostat == true)
		{
			lcd.setCursor(19, 3);
			lcd.print(">");
			blinkTermostat = false;
		}
		else
		{
			lcd.setCursor(19, 3);
			lcd.print(" ");
			blinkTermostat = true;
		}
	}
	//   disable_lcd_update = false;
}

void LCD_grzalka1()
{
	lcd.setCursor(0, 0);
	lcd.print(F("GRZALKA:   1"));

	lcd.setCursor(2, 2);
	lcd.print("G1:  ");
	if (gParametryPracy.G1 == true)
	{
		lcd.setCursor(8, 2);
		lcd.print("ON ");
	}
	else
	{
		lcd.setCursor(8, 2);
		lcd.print("OFF");
	}

	// Start > blink mark
	if (gAutomat.IsStarted() == true)
	{
		if (blinkTermostat == true)
		{
			lcd.setCursor(19, 3);
			lcd.print(">");
			blinkTermostat = false;
		}
		else
		{
			lcd.setCursor(19, 3);
			lcd.print(" ");
			blinkTermostat = true;
		}
	}
}

void LCD_grzalka2()
{
	lcd.setCursor(0, 0);
	lcd.print(F("GRZALKA:  2"));

	lcd.setCursor(2, 2);
	lcd.print("G2:  ");
	if (gParametryPracy.G2 == true)
	{
		lcd.setCursor(8, 2);
		lcd.print("ON ");
	}
	else
	{
		lcd.setCursor(8, 2);
		lcd.print("OFF");
	}

	if (gAutomat.IsStarted() == true)
	{
		if (blinkTermostat == true)
		{
			lcd.setCursor(19, 3);
			lcd.print(">");
			blinkTermostat = false;
		}
		else
		{
			lcd.setCursor(19, 3);
			lcd.print(" ");
			blinkTermostat = true;
		}
	}
}

void LCD_grzalka3()
{
	lcd.setCursor(0, 0);
	lcd.print(F("GRZALKA:  3"));

	lcd.setCursor(2, 2);
	lcd.print("G3:  ");
	if (gParametryPracy.G3 == true)
	{
		lcd.setCursor(8, 2);
		lcd.print("ON ");
	}
	else
	{
		lcd.setCursor(8, 2);
		lcd.print("OFF");
	}

	if (gAutomat.IsStarted() == true)
	{
		if (blinkTermostat == true)
		{
			lcd.setCursor(19, 3);
			lcd.print(">");
			blinkTermostat = false;
		}
		else
		{
			lcd.setCursor(19, 3);
			lcd.print(" ");
			blinkTermostat = true;
		}
	}
}

void LCD_termostat_stop()
{
	disable_lcd_update = true;
	lcd.clear();
	lcd.setCursor(2, 1); // lcd.setCursor(col, row)
	lcd.print(F("TERMOSTAT  STOP"));
	lcd_wait = 2;
	disable_lcd_update = false;
}


/* 
void UDP_broadcast_self()
{

	// if(udp_connected == true) return;

	String sTmpStr = "esp";
	IPAddress broadcastIP;
	if (gParametryPracy.gWIFI_MODE == 2)
	{
		//  #--AP--#
		// sTmpStr += WiFi.softAPIP().toString();
		broadcastIP = IPAddress(192, 168, 4, 255);
	}
	else if (gParametryPracy.gWIFI_MODE == 0)
	{
		//  #--STA--#
		// sTmpStr += WiFi.localIP().toString();
		broadcastIP = ~uint32_t(WiFi.subnetMask()) | (uint32_t(WiFi.gatewayIP()));
	}

	// EncodeNumberString(sTmpStr);

	Udp.beginPacket(broadcastIP, localPort);
	Udp.write(sTmpStr.c_str());
	Udp.endPacket();

	// Async:
	 udp.print(sTmpStr.c_str());

}
 */


void EncodeNumberString(String &str_in)
{
	int i = 0;
	int num = 0;
	int strlen = str_in.length();
	char buff[3];

	for (i = 0; i < strlen; i++)
	{
		memset(buff, 0, sizeof(buff));
		buff[0] = str_in[i];

		// encoded:  *%$@:!)@.!&(.!.#
		// OBTAIN-->  *%$@:192.168.1.3

		num = atoi(buff);
		// printf ("%d",num);

		switch (num)
		{
		case 1:
			str_in[i] = '!';
			break;

		case 2:
			str_in[i] = '@';
			break;

		case 3:
			str_in[i] = '#';
			break;

		case 4:
			str_in[i] = '%';
			break;

		case 5:
			str_in[i] = '^';
			break;

		case 6:
			str_in[i] = '&';
			break;

		case 7:
			str_in[i] = '*';
			break;

		case 8:
			str_in[i] = '(';
			break;

		case 9:
			str_in[i] = ')';
			break;

		case 0:

			break;
		}
	}
}

/***************************************************************************
 * Load configuration data from the SPIFF file, return 0 if OK
 * SPIFFS open function needs path and mode:
 * mode = "r", "w", "a", "r+", "w+", "a+" (text file) or "rb", "wb", "ab", "rb+", "wb+", "ab+" (binary)
 * where r = read, w = write, a = append
 * + means file is open for update (read and write)
 * b means the file os open for binary operations
 *
 * Returns 0 if OK else:
 * -1 = No SPIFFS file system
 * -2 = File does not exist
 * -3 = File too short
 * -4 = Checksum does not compare
 ***************************************************************************/
// int SaveLoadConfigFromFile(AutomataData *Conf, char * configfile)
//{
// FSInfo fs_info;
// File F;
// int retcode = 0;
// unsigned int readSize;
// unsigned int chk;

// AutomataData Cnf;

// LittleFS.begin();

// if (!LittleFS.info(fs_info))
// retcode = -1;	//File system not initialized?
// else
//{
// if (!LittleFS.exists(configfile))
// retcode = -2;	//Could not find config file
// else
//{
// F = LittleFS.open(configfile, "rb");
// F.setTimeout(0);
// readSize = F.readBytes((char*) Cnf, sizeof(Cnf)); //cast changed from byte*
// if (!(readSize == sizeof(Cnf)))
// retcode = -3;
// else
//{
////chk = Cnf.checksum;
////Cnf.checksum = 0;
////if (CheckSum(&Cnf, sizeof(Cnf)) == chk) //Validated checksum
////{
////Cnf.checksum = chk;
////*Conf = Cnf;
////}
////else
////retcode = -4;
//}
// F.close();
//}
//}
// LittleFS.end();

// return retcode;
//}

/*********************************************************************
 * Write configuration into a SPIFF file at address 0
 * SPIFFS open function needs path and mode:
 * mode = "r", "w", "a", "r+", "w+", "a+" (text file) or "rb", "wb", "ab", "rb+", "wb+", "ab+" (binary)
 * where r = read, w = write, a = append
 * + means file is open for update (read and write)
 * b means the file os open for binary operations
 *
 * Returns 0 if OK else:
 * -1 = No SPIFFS file system
 * -3 = All bytes could not be written
 *********************************************************************/
// int SaveConfigToFile(AutomataData *Conf, char* configfile)
//{
// AutomataData Cnf;  //Temp struct
// FSInfo fs_info;
// File F;
// unsigned int writeSize;
// int retcode = 0;

// Cnf = *Conf;
////UpdateChecksum(&Cnf);  //Set checksum member to sum of all other bytes

// F2.begin();

// if (!LittleFS.info(fs_info))
// retcode = -1;	//File system not initialized?
// else
//{
// F = LittleFS.open(configfile, "wb");
// writeSize = F.write((byte*) Cnf, sizeof(Cnf));
// if (!(writeSize == sizeof(Cnf)))
// retcode = -3;
// F.close();
//}
// LittleFS.end();

// return retcode;
//}

void Setup_SPIFFS()
{
	FSInfo fs_info;

	// SerialDebug.println("SPIFFS check T=" + String(millis()));
	LittleFS.begin();

	if (!LittleFS.info(fs_info))
	{
		// SerialDebug.println("SPIFFS needs formatting T=" + String(millis()));
		if (LittleFS.format())
		{
			// SerialDebug.println("File System formatted T=" + String(millis()));
		}
		else
		{
			// SerialDebug.println("File System formatting Error T=" + String(millis()));
		}
	}
	else
	{
		// SerialDebug.print("Done! T=" + String(millis()));
		// SerialDebug.print(" Size=" + String(fs_info.totalBytes));
		// SerialDebug.println(" Used=" + String(fs_info.usedBytes));

		Dir dir = LittleFS.openDir("/");
		while (dir.next())
		{
			// SerialDebug.print(dir.fileName());
			File f = dir.openFile("r");
			// SerialDebug.println("  " + String(f.size()));
			f.close();
		}
	}

	LittleFS.end();
}

// spiffs write struct
// long start_time_spiffs = millis();

// File configFile = SPIFFS.open("/lights.conf", "w+");

// if (!configFile)
//{
// Serial.println(F("Failed to open test.conf"));
//} else {

// Serial.println(F("Opened Hue_conf.txt for UPDATE...."));
// Serial.printf("Start Position =%u \n", configFile.position());

// unsigned char * data = reinterpret_cast<unsigned char*>(Lights); // use unsigned char, as uint8_t is not guarunteed to be same width as char...
// size_t bytes = configFile.write(data, sizeof(HueLight) * _LightCount ); // C++ way

// Serial.printf("END Position =%u \n", configFile.position());
// configFile.close();

//}

// read spiffs

// File LightsFile = SPIFFS.open("/lights.conf", "r");

// if (!LightsFile)
//{
// Serial.println(F("Failed to open lights.conf"));
//} else {
// Serial.println(F("Opened lights.conf"));
// Serial.print(F("CONFIG FILE CONTENTS----------------------"));
// Serial.println();

// uint8_t light[sizeof(HueLight)];
// size_t size = LightsFile.size();
// uint8_t counter = 0;

// for(int i=0; i<size; i+=sizeof(HueLight)){

// for(int j=0;j<sizeof(HueLight);j++){
// light[j] = LightsFile.read();
//}

// HueLight *thelight = (HueLight *)light;
// HueLight *currentlight = &Lights[counter];
// memcpy (currentlight , thelight , sizeof(HueLight));

// Serial.printf( "Saved Light:%3u, %15s, State = %u, HSB(%5u,%3u,%3u) \n",
// counter, thelight->Name, thelight->State, thelight->Hue, thelight->Sat, thelight->Bri);

// counter++;
// }

// LightsFile.close();
//}

// void ScanDS(void)
//{
// byte i;
////byte present = 0;
////byte data[12];
// byte addr[8];
// int aindex = 0;

// Serial.print("Szukam DS...\n\r");

// bool commit = false;

// while(sensors.getAddress(addr, aindex))
//{

// Serial.print("\n\rZnalazlem \'1-Wire\' urzadzenie o adresie:\n\r");
// for( i = 0; i < 8; i++)
//{
// Serial.print("0x");
// if (addr[i] < 16)
//{
// Serial.print('0');
//}

// if(gParametryPracy.Termo[aindex].Termo_x[i] != addr[i])
//{
// gParametryPracy.Termo[aindex].Termo_x[i] = addr[i];
// commit = true;
//}

// Serial.print(addr[i], HEX);
// if (i < 7)
//{
// Serial.print(" ");
//}
//}
// if ( OneWire::crc8( addr, 7) != addr[7])
//{
// Serial.print("CRC error!\n");
// return;
//}

// aindex++;
//}

// if(commit == true)
//{
// EEPROM.put(0, gParametryPracy);
// boolean ok = EEPROM.commit();
// SETUP_DS = true;
// delay(200);
// Serial.println("");
// Serial.println("Znaleziono nowy DS18b20");
// Serial.println("");
// Serial.println("Zapis w pamieci eeprom");
// Serial.println((ok) ? "Zapis OK" : "Blad zapisu");
// Serial.println("");

//}
// Serial.print("\n\r\n\r");
////sensors.reset_search();
// return;
//}

//{
// Dir dir = SPIFFS.openDir("/");
// byte cnt = 0;
// while (dir.next())
//{
// String fileName = dir.fileName();
// size_t fileSize = dir.fileSize();
////lcd.clear();
// lcd.setCursor(0, 0); //lcd.setCursor(col, row)
// lcd.print("                    ");
// lcd.setCursor(0, 0);
// lcd.print(fileName.c_str());
// lcd.setCursor(0, 1);
// lcd.print("                    ");
// lcd.setCursor(0, 1);
// lcd.print(FormatBytes(fileSize).c_str());
// for(byte cnt2 = 0;cnt2 < cnt;cnt2++)
//{
// lcd.setCursor(0 + cnt2, 3);
// lcd.print((char)1);
//}
// cnt++;
// delay(200);
// Serial.printf("FS File: %s, size: %s\n", fileName.c_str(), FormatBytes(fileSize).c_str());
//}
// Serial.printf_P(PSTR("\n"));
//}

// SPIFFS.gc();

// File f = LittleFS.open("/AxsUjabu.dat", "r");

// if (!f)
//{
// Serial.println("Brak pliku kontrolnego!");
// while(1)
//{
// pcf8574.digitalWrite(BUZZER, LOW);
// lcd.clear();
// lcd.setCursor(2, 1);
// lcd.print("AWARIA");
// lcd.setCursor(2, 2);
// lcd.print("SYSTEMU !!!");
// delay(500);
// lcd.clear();
// pcf8574.digitalWrite(BUZZER, HIGH);
// delay(500);
//}

//}else
//{
// String strData = "";
// for( byte i=0; i < f.size() - 1; i++)
//{
// strData += char(f.read());
//}
// f.close();

// if(  strData =="afah.?foncoUnrnBcenAJuWqPMnaasUtqpnxcrxnp")
//{
// Serial.println("WERYFICATION COMPLETE!");
//}else
//{
// Serial.println("WERYFICATION ERROR!");
// lcd.clear();
// lcd.setCursor(0, 0);
// lcd.print("AWARIA");
// lcd.setCursor(0, 1);
// lcd.print("SYSTEMU !!!");
// while(1)
//{
////delay(0);
// pcf8574.digitalWrite(BUZZER, HIGH);
// delay(400);
// pcf8574.digitalWrite(BUZZER, LOW);
// delay(400);
//}
//}
//}

// long start_time_spiffs = millis();

// File configFile = SPIFFS.open("/lights.conf", "w+");

// if (!configFile)
//{
// Serial.println(F("Failed to open test.conf"));
//} else {

// Serial.println(F("Opened Hue_conf.txt for UPDATE...."));
// Serial.printf("Start Position =%u \n", configFile.position());

// unsigned char * data = reinterpret_cast<unsigned char*>(Lights); // use unsigned char, as uint8_t is not guarunteed to be same width as char...
// size_t bytes = configFile.write(data, sizeof(HueLight) * _LightCount ); // C++ way

// Serial.printf("END Position =%u \n", configFile.position());
// configFile.close();

//}

// int readSize = file.readBytes((byte*) myConfigStruct, sizeof(myConfigStruct));

// File LightsFile = SPIFFS.open("/lights.conf", "r");

// if (!LightsFile)
//{
// Serial.println(F("Failed to open lights.conf"));
//} else {
// Serial.println(F("Opened lights.conf"));
// Serial.print(F("CONFIG FILE CONTENTS----------------------"));
// Serial.println();

// uint8_t light[sizeof(HueLight)];
// size_t size = LightsFile.size();
// uint8_t counter = 0;

// for(int i=0; i<size; i+=sizeof(HueLight)){

// for(int j=0;j<sizeof(HueLight);j++){
// light[j] = LightsFile.read();
//}

// HueLight *thelight = (HueLight *)light;
// HueLight *currentlight = &Lights[counter];
// memcpy (currentlight , thelight , sizeof(HueLight));

// Serial.printf( "Saved Light:%3u, %15s, State = %u, HSB(%5u,%3u,%3u) \n",
// counter, thelight->Name, thelight->State, thelight->Hue, thelight->Sat, thelight->Bri);

// counter++;
// }

// LightsFile.close();
//}
