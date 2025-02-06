/*
  BLYNK_WRITE(V24) {//Receber do terminal pela datastreams V24(string)
  String string = param.asStr();
  }

  BLYNK_WRITE(V21)
  {
  if (param.asInt() == 1) {
    Serial.printf("Recebi um em reset");
  }
  }

*/

BLYNK_WRITE(V44)
{
  Serial.printf("Kp recebido pelo BLYNK:%f\n", param.asFloat());
  if (param.asFloat() != Kp) {
    Kp = param.asFloat();
    Blynk.virtualWrite(41, Kp);
    Serial.printf("Kp reenviado ao BLYNK:%f\n", Kp);

    preferences_PID.begin("ESPChamber", false);

    preferences_PID.putDouble("proportional", Kp);

    preferences_PID.end();

  } else Serial.println("Não tive de atualizar a epprom do Kp");
}

BLYNK_WRITE(V45)
{
  Serial.printf("Ki recebido pelo BLYNK:%f\n", param.asFloat());
  if (param.asFloat() != Ki) {
    Ki = param.asFloat();
    Blynk.virtualWrite(42, Ki);
    Serial.printf("Ki reenviado ao BLYNK:%f\n", Ki);


    preferences_PID.begin("ESPChamber", false);

    preferences_PID.putDouble("integral", Ki);

    preferences_PID.end();

  } else Serial.println("Não tive de atualizar a epprom do Ki");
}

BLYNK_WRITE(V46)
{
  Serial.printf("Kd recebido pelo BLYNK:%f\n", param.asFloat());
  if (param.asFloat() != Kd) {
    Kd = param.asFloat();
    Blynk.virtualWrite(43, Kd);
    Serial.printf("Kd reenviado ao BLYNK:%f\n", Kd);


    preferences_PID.begin("ESPChamber", false);
    preferences_PID.putDouble("derivative", Kd);
    preferences_PID.end();

  } else Serial.println("Não tive de atualizar a epprom do Kd");
}



BLYNK_WRITE(V3)
{
  Serial.println("Recebi update do KILL em Blynk V3.");
  if (param.asInt() == 1) {
    runStatus = KILL_;
    KILL = 1;
    Was_KILL = true;
    Turn_all_off(true);
    if (sd_exist)writeFile(SD, "/KILL.txt", "KILL=1", 1);
    Serial.println("Recebi um em kill=ON");
  }
  if (param.asInt() == 0) {
    runStatus = UnPositioned;
    KILL = 0;
    if (sd_exist)writeFile(SD, "/KILL.txt", "KILL=0", 1);
    Serial.println("Recebi um em kill=OFF");
  }
}
void Postar_online() {
  /*
    enum Power_state {
    On,
    Off,
    none
    };

    enum Temp_state {
    Idle,
    Heating,
    Cooling,
    In,
    Out,
    Halt,
    none_
    };
  */


  if (Online && Blynk.connected()) {
    static float Prev_PID_Output_1 = -1, Prev_Kp = -1, Prev_Ki = -1, Prev_Kd = -1;
    static int Prev_Now_Tide_ = -2, Prev_Now_Light = -1;
    static int Prev_state_[8] = { -1, -1, -1, -1, -1, -1, -1, -1};
    static Temp_state Prev_Water_temp_state = none_;//Vai de 0 a 5)

    volatile Power_state Prev_Water_Cooler_state = none, Prev_Water_Heater_state = none, Prev_Tide_pump_state = none,
                         Prev_Circle_fan_state = none, Prev_Cool_fan_state = none, Prev_Release_WV_state_ = none,
                         Prev_Enter_WV_state_ = none, Prev_Cooler_pump_state = none;


    if (Blynk_ronda == 2) {
      if (runStatus == Ongoing)Blynk.virtualWrite(8, Temp_Setpoint);
      if (runStatus == Ongoing || runStatus == Waiting_to_start)Blynk.virtualWrite(9, Water_Temp_Setpoint);
      Blynk.virtualWrite(10, Air_Temp_1);
      Blynk.virtualWrite(11, Air_Temp_2);
      Blynk.virtualWrite(12, Air_Temp_3);
      Blynk.virtualWrite(13, Air_Input_Temp_Avg);
      Blynk.virtualWrite(14, Water_Temp_1);
      Blynk.virtualWrite(15, Water_Temp_2);
      Blynk.virtualWrite(16, Water_Input_Temp_Avg);
      Blynk.virtualWrite(47, Room_Temp);
    }

    if (Blynk_ronda == 3) {
      //Water sensors 1 to 5 and Water sections 1 to 3:
      //Water sensors 1 to 5:
      for (int i = 0; i < 5; i ++) {
        if (state[i] == 0)Blynk.virtualWrite(18 + i, -1);
        if (state[i] == 1)Blynk.virtualWrite(18 + i, 0);
        if (state[i] == 3)Blynk.virtualWrite(18 + i, 1);
      }
      //Water sections 1 to 3:
      if (Top_sensors_func && Water_top_detected) Blynk.virtualWrite(26, 1);
      if (Top_sensors_func && !Water_top_detected) Blynk.virtualWrite(26, 0);
      if (!Top_sensors_func ) Blynk.virtualWrite(26, -1);

      if (state[3] == 0)Blynk.virtualWrite(27, -1);
      if (state[3] == 1)Blynk.virtualWrite(27, 0);
      if (state[3] == 3)Blynk.virtualWrite(27, 1);

      if (state[4] == 0)Blynk.virtualWrite(28, -1);
      if (state[4] == 1)Blynk.virtualWrite(28, 0);
      if (state[4] == 3)Blynk.virtualWrite(28, 1);
    }

    if (Blynk_ronda == 4) {
      //Water sensors 6 to 8 and Water section 4:
      //Water sensors 6 to 8:
      for (int i = 5; i < 8; i ++) {
        if (state[i] == 0)Blynk.virtualWrite(18 + i, -1);
        if (state[i] == 1)Blynk.virtualWrite(18 + i, 0);
        if (state[i] == 3)Blynk.virtualWrite(18 + i, 1);
      }

      //Water section 4:
      if (Bot_Bot_sensors_func  && Water_BOt_bot_detected) Blynk.virtualWrite(29, 1);
      if (Bot_Bot_sensors_func  && !Water_BOt_bot_detected) Blynk.virtualWrite(29, 0);
      if (!Bot_Bot_sensors_func ) Blynk.virtualWrite(29, -1);

      Blynk.virtualWrite(17, Now_Tide);
      if (Tide_Pump_state == On)Blynk.virtualWrite(35, 1); else Blynk.virtualWrite(35, 0);
      if (Release_WV_state == On)Blynk.virtualWrite(39, 1); else Blynk.virtualWrite(39, 0);
      if (Enter_WV_state == On)Blynk.virtualWrite(40, 1); else Blynk.virtualWrite(40, 0);
    }

    if (Blynk_ronda == 5) {
      Blynk.virtualWrite(30, PID_Output_1);
      if (Water_Cooler_state == On)Blynk.virtualWrite(31, 1); else Blynk.virtualWrite(31, 0);
      if (Water_Cooler_state == On)Blynk.virtualWrite(32, 1); else Blynk.virtualWrite(32, 0);

      //Water_state

      if (Water_temp_state == Idle)Blynk.virtualWrite(33, "Idle");
      if (Water_temp_state == Heating)Blynk.virtualWrite(33, "Heating");
      if (Water_temp_state == Cooling)Blynk.virtualWrite(33, "Cooling");
      if (Water_temp_state == In)Blynk.virtualWrite(33, "Water_Inlet");
      if (Water_temp_state == Out)Blynk.virtualWrite(33, "Water_Outlet");
      if (Water_temp_state == Halt)Blynk.virtualWrite(33, "Halt!");
      if (Water_temp_state == none_)Blynk.virtualWrite(33, "Undefined");

      if (Cooler_Pump_state == On)Blynk.virtualWrite(34, 1); else Blynk.virtualWrite(34, 0);
      Blynk.virtualWrite(36, Now_Light);
      if (Circle_FAN_state == On)Blynk.virtualWrite(37, 1); else Blynk.virtualWrite(37, 0);
      if (Cool_FAN_state == On)Blynk.virtualWrite(38, 1); else Blynk.virtualWrite(38, 0);
    }


    if (Prev_Kp != Kp) {
      Blynk.virtualWrite(41, Kp);
      Prev_Kp = Kp;
    }
    if (Prev_Ki != Ki) {
      Blynk.virtualWrite(42, Ki);
      Prev_Ki = Ki;
    }
    if (Prev_Kd != Kd) {
      Blynk.virtualWrite(43, Kd);
      Prev_Kd = Kd;
    }
    if (Blynk_ronda == 5)Blynk_ronda = 2; else Blynk_ronda++;
  }
}

/*Opcoes;*/
//  int virtual_pin_value = param.asInt();
//  String virtual_pin_value = param.asStrng()();
//  float virtual_pin_value = param.asFloat();

//Blynk.logEvent("Ola", "Mensagem_Ola");
//Blynk.virtualWrite(V1,0);//Envia 0 pelo V1
//Blynk.syncVirtual(V1);
/**/


//.getFloat
//.putFloat
