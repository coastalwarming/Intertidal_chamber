
void start_rtc() {
  //O RTC esta comunicavel?
  if (!Outrtc.begin()) {
    Outrtc_working = false;
    Serial.println("Something went wrong in Outrtc.begin(), check wiring.");
    return;
  }
  else {
    Outrtc_working = true;
    Acertar_registos_OUTRTC();
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  }
  //

  //Esta comunicavel, mas perdeu as horas?
  if (Outrtc_working && Outrtc.updateTime() && (Outrtc.getEpoch() > 1640995201)) {    //1640995201 = 1 January 2022 00:00:01
    RTC_Pos_2022 = true;
    //nowTimeUnix = Outrtc.getEpoch();
    nowTimeUnix = Outrtc.getEpoch() + (int)(UTC * 3600) + (int)(DST * 3600);
    Inrtc.setTime(nowTimeUnix); //Apenas para manter o timestamp nos files.
    //Serial.println("RTC is working and RTC time is after 01/01/2022.");
  } else {
    nowTimeUnix = 1640995201;
    Outrtc.setEpoch(nowTimeUnix);
    Serial.println("Outrtc_working but not prev to 01/01/2022.");
    minuto_mudou = true; //Para forçar a aparecer horas mesmo com relogio pre-2022.
    dia_mudou = true; //Para forçar a aparecer horas mesmo com relogio pre-2022.
    RTC_Pos_2022 = false;
  }


}

void Acertar_registos_OUTRTC() {

  //Desligar refresh
  //Serial.print("Registo 10h antes:"); printFullBin(Outrtc.readRegister_(0x10));
  Outrtc.writeBit_(0x10, 2, 1);
  delay(50);

  //Serial.print("Registo 10h apos:  "); printFullBin(Outrtc.readRegister_(0x10));
  //Mudar Registos na RAM
  Outrtc.writeRegister_(0xC0, 0b00100000);//Modo backup pilha com 3v<VDD(3.3v).
  delay(50);
  Outrtc.set24Hour(); //Uncomment this line if you'd like to set the RTC to 24 hour mode.
  delay(50);

  //Update eeprom
  //Serial.print("Registo 3Fh antes:"); printFullBin(Outrtc.readRegister_(0x3F));
  Outrtc.writeRegister_(0x3F, 0b00010001);
  delay(50);
  //Serial.print("Registo 3Fh apos:  "); printFullBin(Outrtc.readRegister_(0x3F));

  //Voltar a ligar refresh
  //Serial.print("Registo 10h antes:"); printFullBin(Outrtc.readRegister_(0x10));
  Outrtc.writeBit_(0x10, 2, 0);
  delay(50);

  //Serial.print("Registo 10h apos:  "); printFullBin(Outrtc.readRegister_(0x10));
  Outrtc.writeRegister_(0xC0, 0b00100000);//Modo backup pilha com 3v<VDD(3.3v).//Teste para ver se rtc nao perde as horas (nem sempre acontece).
  Outrtc.set24Hour();
  delay(50);
}
/*
  unsigned long getTime() {
  time_t now;
  struct tm timeinfo = {};
  byte counter;
  while ( !getLocalTime( &timeinfo, 0 ) ) { // wait for NTP to sync
    counter++;
    delay(1000);
    if (counter > 20) break;
  }

  time(&now);//O que que isto faz?!
  return now;
  }
*/


void NTP_adjust() {
  static bool Reset_after_time_adjust = false;
  //Serial.println("NTP_adjust()");
  //Wire.begin();//Precisa disto?
  if (Outrtc_working) {
    if (!Get_NTP_Time_Task_4_resumed) {
      Serial.println("Get_NTP_Time_Task_4_resumed.");
      Get_NTP_Time_Task_4_resumed = true;
      Get_Time_Task_finished = false;
      vTaskResume(Get_NTP_Time_Task_4_Handle);
    }


    //NTP_epochTime = getTime();
    if (Get_NTP_Time_Task_4_resumed && Get_Time_Task_finished) {
      Get_NTP_Time_Task_4_resumed = false;
      Get_Time_Task_finished = false;

      Serial.println("Get_Time_Task_finished");

      if (NTP_epochTime > 1640995201) {//1640995201=1 January 2022 00:00:01
        if (Outrtc.updateTime() && ( (NTP_epochTime > (Outrtc.getEpoch() + 60) ) ||    (NTP_epochTime < (Outrtc.getEpoch() - 60)) ) ) {
          Serial.printf("NTP_epochTime was %i\n While Outrtc.getEpoch() was  %i\n ", NTP_epochTime, Outrtc.getEpoch());

          Serial.println("Hence, NTP time difference was above 60 seconds with RTC time (not including DST or UTC) - need to reset!");
          Reset_after_time_adjust = true;
        }
        else {
          Serial.println("NTP time difference was below 60 seconds - no need to reset.");
          Serial.print("NTP_epochTime was:");   Serial.println(NTP_epochTime);
          Serial.print("Outrtc.getEpoch() was:");   Serial.println(Outrtc.getEpoch());
        }

        RTC_Pos_2022 = true;
        NTP_Time_adjusted = true;
        Outrtc.setEpoch(NTP_epochTime);

        minuto_mudou = true; //Para forçar a aparecer horas e dia a amarelo.
        dia_mudou = true; //Para forçar a aparecer horas e dia a amarelo.

        if (Outrtc_working && Outrtc.updateTime() && (Outrtc.getEpoch() > 1640995201))
        {
          nowTimeUnix = Outrtc.getEpoch() + (int)(UTC * 3600) + (int)(DST * 3600);//nowTimeUnix = Outrtc.getEpoch();
          Inrtc.setTime(nowTimeUnix);
          Serial.printf("NTP_epochTime was %i\nWhile nowTimeUnix with DST=%i and UTC=%i became ", NTP_epochTime, DST, UTC);
          Serial.println(nowTimeUnix);
        }
      } else Serial.println("NTP_epochTime came with Epoch: 1640995201=1 January 2022 00:00:01.");

      if (Reset_after_time_adjust) {
        SD.end();
        Turn_all_off(true);
        xSemaphoreTake(mutex, portMAX_DELAY);

        tft.fillScreen(BLACK);
        tft.setTextColor(WHITE);
        tft.setTextSize(3);
        drawCentreString("Clock time adjusted.", 240, 140, 18, 3);
        drawCentreString("Rebooting...", 240, 180, 12, 3);

        xSemaphoreGive(mutex);
        delay(3000);
        ESP.restart();
      }

    }



    //
  } else {
    Serial.println("Outrtc_working is not communicating, attempting start_rtc() again.");
    start_rtc();
  }
}
