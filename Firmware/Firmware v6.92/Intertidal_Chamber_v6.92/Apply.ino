void Apply_Air_Heat() {
  static bool Prev_HeatersON = false,  HeatersON = false ;
  static bool Fst_boot = true;
  unsigned long HeatersONtime = 0;
  //Não estou em maré baixa:
  //if (Now_Tide == 1 || runStatus != Ongoing) {//Teste nao ligar lampadas quando fora de experiencia.
  if (Now_Tide == 1){
    unsigned long elapsed = millis() % (PID_RATE_SECS * 1000);//PID_RATE_SECS= 15seg; % = Resto = 76000 % 10000ms = 6000ms ou 71000 % 10000ms = 1000ms;
    HeatersONtime = (unsigned long)(PID_Output_1 * float((PID_RATE_SECS * 1000) / 100.0));//Exemplo: Se output=20% e PID_RATE_SECS=5segundos, HeatersONtime = 0.2*5=1segundo.

    //Serial.print("1-HeatersONtime:"); Serial.println(HeatersONtime);
    //Serial.print("1-elapsed:"); Serial.println(elapsed);
    HeatersON = HeatersONtime > elapsed;// 0 ou 1
    //Serial.print("2-HeatersONtime:"); Serial.println(HeatersON);



    if ((Main_SW_State  == Auto) || (Main_SW_State  == Manual && (A_Heat_sw_state == Auto))) {
      if (HeatersON)Heat_Lamp_state = On; else Heat_Lamp_state = Off;//É suposto estar a ligar?
    } else {
      if (Heat_Lamp_state == On) {
        Serial.println("Força Heat_Lamp_state = Off; por estar em Manual-Off .");
        Heat_Lamp_state = Off;
      }
    }

    //Serial.print("1-HeatersON:"); Serial.println(HeatersON);
    //Serial.print("1-Prev_HeatersON:"); Serial.println(Prev_HeatersON);
    if (Prev_HeatersON != HeatersON) {
      Serial.print("3-Heat_Lamp_state:");
      if (Heat_Lamp_state == On)Serial.println("On");
      if (Heat_Lamp_state == Off)Serial.println("Off");

      Serial.print("3-HeatersON:"); Serial.println(HeatersON);
      Prev_HeatersON = HeatersON;
      Toggle_GPIO(8, Heat_Lamp_state);
    }

  }

  //Recomeça contagem caso volte a haver água no topo ou falhe sensores:
  if (!Now_Tide &&
      ((Water_top_detected && Top_sensors_func) || !Top_sensors_func) &&
      (No_water_instant > 0) &&
      Wait_low_tide) {
    No_water_instant = 0;
    Wait_low_tide = false;
    Serial.println("Voltei a ter água no topo ou os sensores do topo falharam.");
  }

  //Guardar tempo em que fiquei sem agua:
  if (!Now_Tide && !Water_top_detected && (No_water_instant == 0) && !Wait_low_tide && Top_sensors_func) {
    //Caso o profile comece em mare vazia ou a mare anterior tenha sido cheia mas ocorrido ha mais de 10minutos, não espera 120segundos:
    if (Fst_boot && !Houve_mare_cheia_10min()) {
      Fst_boot = false;
      Serial.println("Não houve mare cheia a meos de 10 minutos, vou ignorar a espera.");
      No_water_instant = nowTimeUnix - Wait_before_air_heat;
      Wait_low_tide = false;
    } else {
      No_water_instant = nowTimeUnix;
      Wait_low_tide = true;
      Serial.printf("Já não tenho água no topo, agora vou esperar %i segundos antes de aquecer.\n", Wait_before_air_heat);
    }
  }

  //Verificar se já passou X tempo apos ter ficado sem agua:
  if (!Now_Tide &&
      ((nowTimeUnix - No_water_instant) > Wait_before_air_heat) &&
      Wait_low_tide &&
      (No_water_instant > 0) &&
      !Water_top_detected &&
      Top_sensors_func
     ) {
    Wait_low_tide = false;
    Serial.printf("Já passou %i tempo desde que não tenho água no topo.\n", Wait_before_air_heat);
  }// else if (Wait_low_tide) Serial.printf("Falta %i\n", Wait_before_air_heat - (nowTimeUnix - No_water_instant));


  //HEAT LAMPS:
  if ( (runStatus == Ongoing) &&
       !Now_Tide &&
       Air_Temp_sensors_functional &&
       !Water_top_detected &&
       !Wait_low_tide &&
       Top_sensors_func)
  {
    // Serial.println("Já posso aquecer.");

    //Pega na parcela atual de 10seg (Ex: 79000 millis ja so falta 1seg dessa parcela), e liga caso HeatersONtime for >90%.
    unsigned long elapsed = millis() % (PID_RATE_SECS * 1000);//PID_RATE_SECS= 15seg; % = Resto = 76000 % 10000ms = 6000ms ou 71000 % 10000ms = 1000ms;

    HeatersONtime = (unsigned long)(PID_Output_1 * float((PID_RATE_SECS * 1000) / 100.0));//Exemplo: Se output=20% e PID_RATE_SECS=5segundos, HeatersONtime = 0.2*5=1segundo.
    static unsigned long passou = 0;
    HeatersON = HeatersONtime > elapsed;// 0 ou 1

    if (Prev_HeatersON != HeatersON) {
      Prev_HeatersON = HeatersON;

      if ((Main_SW_State  == Manual) && (Tide_sw_state == OFF)) {
        Serial.println("Apply:A mare baixa é manual, não vou aquecer!");
      } else {
        if (!Top_sensors || (Top_sensors && !Water_top_detected && Top_sensors_func)) {

          if (Main_SW_State  == Auto) {
            if (HeatersON)Heat_Lamp_state = On; else Heat_Lamp_state = Off;
            Toggle_GPIO(8, Heat_Lamp_state);
          }
          if (Main_SW_State  == Manual && (A_Heat_sw_state == Auto)) {

            if (HeatersON)Heat_Lamp_state = On; else Heat_Lamp_state = Off;
            Toggle_GPIO(8, Heat_Lamp_state);
          }
        } else {
          Heat_Lamp_state = Off;
          Toggle_GPIO(8, Heat_Lamp_state);
        }
      }
    }

  } else {
    //Heat_Lamp_state = Off;//So comentei no teste para as manetr sempre quentes.
    // Toggle_GPIO(8, Heat_Lamp_state);
    // mcp_20.digitalWrite(8, LOW);//230v_1=Heat lamps}
  }
  if ((Main_SW_State  == Manual) && (Tide_sw_state == OFF)) {
    if (Wait_low_tide) {
      Wait_low_tide = false;
      Serial.println("Apply:A mare baixa é manual, Wait_low_tide a false.");
    }
  }
  //
}

void Apply_Light() {
  if (runStatus == Ongoing ) {
    //  Serial.printf("profileTTL[%i]:", target_Step_Index - 1); Serial.println(profileTTL[target_Step_Index - 1]);
    // Serial.printf("profileTTL[%i]mod 100:", target_Step_Index - 1); Serial.println(profileTTL[target_Step_Index - 1]% 100);
    // Serial.printf("profileTTL[%i]mod 1000:", target_Step_Index - 1); Serial.println(profileTTL[target_Step_Index - 1]% 1000);


    Now_Light   =  profileTTL[target_Step_Index - 1]   % 1000;
    //Serial.printf("Now_Light[0-100]:%i  --  ", Now_Light);
    // Now_Light = map(Now_Light, 0, 100, 0, 255);
    //Serial.printf("Now_Light[0-255]:%i\n", Now_Light);


    if (Main_SW_State == Auto)ledcWrite(0, Now_Light);//mcp_20.digitalWrite(5, Now_Light);//mcp_20.5 = 12v_1 = Lights
    if (Main_SW_State  == Manual)if (Light_sw_state  == Auto)ledcWrite(0, map(Now_Light, 0, 100, 0, 255));//mcp_20.digitalWrite(5, Now_Light);//mcp_20.5 = 12v_1 = Lights

  }

  if (Main_SW_State == Auto && runStatus != Ongoing) {
    Now_Light = 0;
    ledcWrite(0, 0); //mcp_20.digitalWrite(5, LOW);
  }
  if (Main_SW_State == Manual && Light_sw_state == ON) {
    Now_Light = 100;
    ledcWrite(0, map(Now_Light, 0, 100, 0, 255)); //mcp_20.digitalWrite(5, HIGH);
  }
  if (Main_SW_State == Manual && Light_sw_state == OFF) {
    Now_Light = 0;
    ledcWrite(0, 0); //mcp_20.digitalWrite(5, LOW);
  }
  if (Main_SW_State == Manual && Light_sw_state == Auto && runStatus != Ongoing) {
    Now_Light = 0;
    ledcWrite(0, 0); //mcp_20.digitalWrite(5, LOW);
  }

}

void Apply_WExValves() {
  //TPA_state -> =0(Idle), =1(releasing), =2(Entering), =3(Sensor_Order_Error)
  static byte Prev_Now_WEx = 2;



  //Avalia se ha Water change no profile:
  if (runStatus == Ongoing && (target_Step_Index < profileLength) &&
      Last_WEx_TimeStep != ( target_Step_Index - 1) &&
      (nowTimeUnix < (profileTimes[target_Step_Index - 1] + 60))) {
    Last_WEx_TimeStep = target_Step_Index - 1;

    Now_WEx = (byte)((profileTPA[target_Step_Index - 1]));
    if (Prev_Now_WEx != Now_WEx) {
      Prev_Now_WEx = Now_WEx;
      if (Now_WEx)Serial.println("Now_WEx ativated!"); else Serial.println("Now_WEx false!");
    }
  }

  //Avalia se ha Water change no Manual:
  if (WEx_sw_ativated && !Manual_Now_WEx) {
    WEx_sw_ativated = false;
    Serial.print("WCh_sw_ativated, ");

    if (TPA_state == 0) {
      Serial.println("and Manual_Now_WEx set true.");
      Manual_Now_WEx = true;
    } else {
      Serial.println("and Manual_Now_WEx set false because WEx already ongoing.");
      Manual_Now_WEx = false;
    }
  }
  //WEx_sw_state//ON, OFF, Auto


  //1-Passar de Idle a Releasing:
  if (TPA_state == 0 && ((Now_WEx && !WEx_ongoing_F ) || Manual_Now_WEx) && !WS_incomplete) {
    if (Manual_Now_WEx)Manual_Now_WEx = false;


    WEx_ongoing_F = true;
    TPA_state = 1; //TPA_state=0(Idle, =1(releasing), =2(Entering)
    Water_start_release_instant = nowTimeUnix;
    Serial.printf("Initiate Release at %02i:%02i and ", hour(nowTimeUnix), minute(nowTimeUnix));
    if (!Top_sensors && !Bottom_sensors) Serial.printf("will close at %02i:%02i .\n", hour(nowTimeUnix +  Water_release_period), minute(nowTimeUnix + Water_release_period));
    if (Top_sensors && Bottom_sensors) Serial.println("will close when Bottom sensors dont sense water.");
    if (!Top_sensors && Bottom_sensors)Serial.println("will close when Bottom sensors dont sense water.");
    if (Top_sensors && !Bottom_sensors)Serial.println("will close when Top sensor dont sense water.");

    Serial.printf("Now_WEx:%i WEx_ongoing_F:%i Last_WEx_TimeStep:%i ( target_Step_Index - 1):%i Manual_Now_WEx:%i\n", Now_WEx, WEx_ongoing_F, Last_WEx_TimeStep, target_Step_Index - 1, Manual_Now_WEx );


  } else if (Manual_Now_WEx && WS_incomplete) {
    Manual_Now_WEx = false;
    Serial.println("Manual_Now_WEx set false because Water sensors network is incomplete.");
  }

  //



  //2-Passar de Releasing a Entering:
  if (TPA_state == 1) {
    //Esvazia até:
    //- Até NÂO sentir agua no Bot_bot.
    //OPCAO Desligar sensores: Esvazia até ultrapassar o Water_release_period com Top_sensors false.
    //Cancela experiência se durante o esvaziamento:
    //- Nao detetar agua no Water_BOt_bot_detected antes do Water_BOt_mid_detected;
    //- Nao detetar agua no Water_BOt_mid_detected antes do Water_BOt_top_detected;
    if ((!Water_BOt_bot_detected && Bot_Bot_sensors_func)
        || !Bottom_sensors && (nowTimeUnix > Water_start_release_instant + Water_release_period)
       ) {
      TPA_state = 2; //TPA_state -> =0(Idle), =1(releasing), =2(Entering)
      Water_start_fill_instant = nowTimeUnix;

      Serial.printf("Initiate Enter at % 02i:%02i and ", hour(nowTimeUnix), minute(nowTimeUnix));
      Serial.printf("will close at %02i:%02i.\n", hour(nowTimeUnix +  Water_fill_period), minute(nowTimeUnix + Water_fill_period));
      if (Bottom_sensors) Serial.println("Or when Bottom-top sensor sense water.");
    }

    //Vias de cancelamento do esvaziar de agua (validas para Bottom_sensors=true (default):
    if (Bottom_sensors) {
      if ((!Water_BOt_mid_detected && Water_BOt_top_detected)
          || (!Water_BOt_bot_detected && Water_BOt_mid_detected) ||
          !Bot_Bot_sensors_func ||
          !Top_sensors_func) {
        Serial.println("Cancelar esvaziamento por falta de logica nos sensores:");
        /*    Serial.printf("Water_top_detected:%i\nWater_BOt_top_detected:%i\nWater_BOt_mid_detected:%i\nWater_BOt_bot_detected:%i\n",
                          Water_top_detected,
                          Water_BOt_top_detected,
                          Water_BOt_mid_detected,
                          Water_BOt_bot_detected);
        */
        TPA_state = 3;//State 3 é erro nos sensores.
      }
    }
  }

  //2-Passar de Entering a Idle:
  if (TPA_state == 2) {
    //Enche até:
    //1.1- Ultrapassar o Water_fill_period.
    //1.2- Se tem agua no Water_top_detected: até sentir agua no Water_BOt_mid_detected.
    //1.3- Se nao tem agua no Water_top_detected: até sentir agua no Water_BOt_top_detected .
    //Cancela experiência se durante o enchimento:
    //1.4- Demorar mais que X minutos ter Water_BOt_bot_detected=true;
    //1.5- Demorar mais que X minutos ter Water_BOt_mid_detected=true;
    //1.6- Deteta agua no Water_BOt_mid_detected antes do Water_BOt_bot_detected;
    //1.7- Deteta agua no Water_BOt_top_detected antes do Water_BOt_mid_detected;

    if ((!Water_top_detected && Water_BOt_top_detected && Bottom_sensors && Bot_Bot_sensors_func) //1.3
        || (Water_top_detected && Water_BOt_mid_detected && Bottom_sensors && Top_sensors_func && Bot_Bot_sensors_func) //1.2
        || (nowTimeUnix > Water_start_fill_instant + Water_fill_period)//1.1
       ) {

      WEx_ongoing_F = false;

      TPA_state = 0; //TPA_state=0(Idle, =1(releasing), =2(Entering)
      Now_WEx = false;

      if (Bottom_sensors && Water_BOt_top_detected)Serial.println("Initiated Idle because Bottom-top sensed water.");
      if (Bottom_sensors && Water_BOt_mid_detected)Serial.println("Initiated Idle because Bottom-mid sensed water.");
      else Serial.printf("Reached Idle Water_fill_period of %i seconds was reached at %i:%i.\n", Water_fill_period, hour(nowTimeUnix), minute(nowTimeUnix));
    }

    //Vias de cancelamento do encher de agua (validas para Bottom_sensors=true (default):
    if (Bottom_sensors) {
      if ((!Water_BOt_mid_detected && Water_BOt_top_detected) ||
          (!Water_BOt_bot_detected && Water_BOt_mid_detected)          ) {
        Serial.println("Cancelar enchimento por falta de logica nos sensores:");
        /*    Serial.printf("Water_top_detected:%i\nWater_BOt_top_detected:%i\nWater_BOt_mid_detected:%i\nWater_BOt_bot_detected:%i\n",
                          Water_top_detected,
                          Water_BOt_top_detected,
                          Water_BOt_mid_detected,
                          Water_BOt_bot_detected);
        */
        TPA_state = 3;//State 3 é erro nos sensores.
      }
    }
  }

  //coloca em estado Halt caso falhe algum sensor:
  if (TPA_state > 0 && WS_incomplete && TPA_state != 3) {
    //Serial.println("Iria por Set to  TPA_state = 3;");

    TPA_state = 3; //State 3 é erro nos sensores.
  }

  //WEx_sw_state//ON, OFF, Auto
  if (Main_SW_State != OFF && WEx_sw_state != OFF) {
    if (TPA_state == 0) {
      Release_WV_state = Off;
      Enter_WV_state = Off;
      Toggle_GPIO(15, Release_WV_state);
      Toggle_GPIO(7, Enter_WV_state);
      //mcp_20.digitalWrite(15, LOW);//12v_4=Close Release water valve.
      //mcp_20.digitalWrite(7, LOW);//12v_5=Close Enter water valve.
    }
    if (TPA_state == 1) {
      Release_WV_state = On;
      Enter_WV_state = Off;
      Toggle_GPIO(15, Release_WV_state);
      Toggle_GPIO(7, Enter_WV_state);

      //   mcp_20.digitalWrite(15, HIGH);//12v_4=Close Release water valve.
      //  mcp_20.digitalWrite(7, LOW);//12v_5=Open Enter water valve.
    }
    if (TPA_state == 2) {
      Release_WV_state = Off;
      Enter_WV_state = On;
      Toggle_GPIO(15, Release_WV_state);
      Toggle_GPIO(7, Enter_WV_state);

      // mcp_20.digitalWrite(15, LOW);//12v_4=Close Release water valve.
      //mcp_20.digitalWrite(7, HIGH);//12v_5=Open Enter water valve.
    }
    if (TPA_state == 3) {
      Release_WV_state = Off;
      Enter_WV_state = Off;
      Toggle_GPIO(15, Release_WV_state);
      Toggle_GPIO(7, Enter_WV_state);

      //mcp_20.digitalWrite(15, LOW);//12v_4=Close Release water valve.
      //mcp_20.digitalWrite(7, LOW);//12v_5=Close Enter water valve.
      //Desligar Aquecedor de agua.
      //Desligar Cooler.
      //Desligar Tide Pump.
      //Desligar Cooler Pump.
    }

  } else {
    //Aqui forço parar a troca de agua e limpo o estado de Halt!
    TPA_state = 0;
    Now_WEx = false;
    Release_WV_state = Off;
    Enter_WV_state = Off;
    Toggle_GPIO(15, Release_WV_state);
    Toggle_GPIO(7, Enter_WV_state);

    //mcp_20.digitalWrite(15, LOW);//12v_4=Close Release water valve.
    //mcp_20.digitalWrite(7, LOW);//12v_5=Close Enter water valve.

  }

}

void Apply_Fans() {
  //FANS:



  //In Low-Tide:
  if ((runStatus == Ongoing) && (Now_Tide == 0) && Top_sensors_func && !Water_top_detected) {

    if ((Main_SW_State  == Auto) || (Main_SW_State  == Manual && Fan_sw_state == Auto)) {

      if (Circle_FAN_state == Off) {
        Circle_FAN_state = On; //mcp_20.digitalWrite(13, HIGH); //12v_2=Circle FAN
        Serial.println("1 Circle_FAN_state = On");
      }

      float tolerancia = (Temp_Setpoint * 0.0075) - 0.1375;
      if (Air_Input_Temp_Avg > (Temp_Setpoint + tolerancia)) {
        if (Cool_FAN_state == Off) {
          Cool_FAN_state = On;
          Serial.println("2 Cool_FAN_state = On");
        }
      } else {
        if (Cool_FAN_state == On) {
          Cool_FAN_state = Off;
          Serial.println("3 Cool_FAN_state = Off;");
        }
      }
    }
  } else {
    //Serial.println("Else FANS OFF");
    if (Circle_FAN_state == On) {
      Circle_FAN_state = Off;//Versao5 coling hotbulbs
      //Cool_FAN_state = Off;
      //
      //Serial.println("4 Circle_FAN_state = Off;");
    }
  }

  //In High-Tide:
  //Teste se switchs permitem ligar:
  if ((Main_SW_State  == Auto) || (Main_SW_State  == Manual && Fan_sw_state == Auto))
  {
     //if (Now_Tide == 1 || runStatus != Ongoing) {//Teste nao ligar lampadas quando fora de experiencia.
  if (Now_Tide == 1){

      if ((millis() - FAN_instant) > 30000) {

        if (Cool_FAN_state == On) {
          Cool_FAN_state = Off;
          Serial.println("6 Cool_FAN_state = Off");
        }
        else {
          Cool_FAN_state = On;
          Serial.println("7 Cool_FAN_state = On");
        }
/*
 * //Versao5 coling hotbulbs
        if (Circle_FAN_state == On) {
          Circle_FAN_state = Off;
          Serial.println("6 Circle_FAN_state = Off");
        }
        else {
          Circle_FAN_state = On;
          Serial.println("7 Circle_FAN_state = On");
        }
*/
        FAN_instant = millis();
      }
    }
  } else {
    //if (Cool_FAN_state == On || Circle_FAN_state == On) {//Versao5 coling hotbulbs
      if (Cool_FAN_state == On){
        Cool_FAN_state = Off;
      //Circle_FAN_state = Off;//Versao5 coling hotbulbs
      Serial.println("Both fans off pk switch nao permitem Circle_FAN_state = On");
    }
  }
  //In Manual FAN:
  if (Main_SW_State  == Manual && Fan_sw_state == ON) {
    // Serial.print("Manual Apply Cool_FAN_state == On ");
    Circle_FAN_state = On;
    Cool_FAN_state = On;
  } else if (Main_SW_State  == Manual && Fan_sw_state == OFF) {
    Serial.println("FANs estao manual OFF.");
    Circle_FAN_state = Off;
    Cool_FAN_state = Off;
  } else if (Main_SW_State  == Manual && Fan_sw_state == Auto && runStatus != Ongoing) {
    Serial.println("FANs estao manual Auto mas fora de experiencia.");
    Circle_FAN_state = Off;
    Cool_FAN_state = Off;
  } else if (Now_Tide == 1 && !((Main_SW_State  == Manual && Fan_sw_state == ON))) {
    // Serial.println("Mare alta e FANs n estao manual ON.");
    if (Circle_FAN_state == On) {
      //Circle_FAN_state = Off;
      //Cool_FAN_state = Off;
      //Serial.println("8 Circle_FAN_state = Off;");
    }
  }



  Toggle_GPIO(13, Circle_FAN_state);
  Toggle_GPIO(14, Cool_FAN_state);

}

void Apply_Water_Heat() {

  //static byte Prev_Water_temp_state = 0;
  static float Temperatura_input_agua_usada = 0;
  Top_Gap = Air_Input_Temp_Avg - Temp_Setpoint;
  Bottom_Gap = Water_Input_Temp_Avg - Water_Temp_Setpoint;

  if (TPA_state == 0 && Water_temp_state != Idle)Water_temp_state = Idle;

  /*Calculo da margem que vai decidir se liga ou não:*/
  Temperatura_input_agua_usada = Water_Input_Temp_Avg;
  //Target acima de 20ºC:
  if (Water_Temp_Setpoint >= 20) {
    if (Temp_Set_Direction == 1) {
      Water_Heater_delta = (-0.0159 * Water_Temp_Setpoint) + 0.3218;
      if (Top_Gap >= 0.2)Water_Cooler_delta = 0.2; else Water_Cooler_delta = 0.5;
    }
    if (Temp_Set_Direction == -1) {
      if (Top_Gap <= -0.1)Water_Heater_delta = 0.1; else Water_Heater_delta = 0.5;
      Water_Cooler_delta = (0.025 * Water_Temp_Setpoint) - 0.775;
    }
    if (Temp_Set_Direction == 0) {
      Water_Heater_delta = 0.1;//(-0.0159 * Water_Temp_Setpoint) + 0.3218;
      Water_Cooler_delta = 0.1;//0.2;
    }
  }

  //Serial.print("Water_Temp_Setpoint::::"); Serial.println(Water_Temp_Setpoint);

  //Target abaixo de 20ºC:
  if (Water_Temp_Setpoint < 20) {
    if (Temp_Set_Direction == 1) {
      Water_Heater_delta = (-0.0159 * Water_Temp_Setpoint) + 0.3218;
      if (Top_Gap >= 0.1)Water_Cooler_delta = -Top_Gap; else Water_Cooler_delta = 0.5;
    }
    if (Temp_Set_Direction == -1) {
      Water_Cooler_delta = (0.025 * Water_Temp_Setpoint) - 0.775;
      if (Top_Gap <= -0.1)Water_Heater_delta = 0.1; else Water_Heater_delta = 0.5;
    }
    if (Temp_Set_Direction == 0) {
      Water_Cooler_delta = (0.0495 * Water_Temp_Setpoint) - 0.923;//(0.025 * Water_Temp_Setpoint) - 0.775;
      Water_Heater_delta = 0.5;
    }
  }
  /**/


  /*Deterinar Water_temp_state:*/
  //Se tiver a decorrer troca de água desliga o aquecimento/arrefecimento da água:
  if (TPA_state > 0) {
    //Desligar Pump de Cooler no Apply_Pumps.
    if (Water_Cooler_state != Off) {
      //Desligar Cooler:
      Time_Cooler_powerOFF = millis();
      Serial.print("1-Atualizei Time_Cooler_powerOFF: "); Serial.println(Time_Cooler_powerOFF);

      Water_Cooler_state = Off;
      Toggle_GPIO(9, Water_Cooler_state);
      //mcp_20.digitalWrite(9, LOW);//mcp20.1 = 230v_2 = Cooler
    }
    if (Water_Heater_state != Off) {
      //Desligar WHeater:
      Water_Heater_state = Off;
      Toggle_GPIO(10, Water_Heater_state);

      // mcp_20.digitalWrite(10, LOW);//mcp20.10 = 230v_3 = Water Heater

    }
  }


  if (Profile_ok && ((runStatus != Finished)) && Water_Temp_sensors_functional && TPA_state == 0) {
    //Determinar estado:
    //Se nao detetar agua em Water_BOt_bot_detected ou estiver com mare alta manual, mantem o bot em idle.
    // if ((Bottom_sensors && !Water_BOt_bot_detected && Bot_Bot_sensors_func) ||
    if ((Main_SW_State  == Manual && Tide_sw_state == ON)) {
      if (TPA_state == 0)Water_temp_state = Idle;
    } else {
      if (!No_more_tide_change || Now_Tide == 1) { //Ainda não estou na ultima mare.
        if ((Temperatura_input_agua_usada <= ( Water_Temp_Setpoint - Water_Heater_delta)) && Water_temp_state != Heating) { //Water_temp_state -> 0 nada, 1 aquecer, 2 arrefecer.
          Water_temp_state = Heating;
          //Serial.printf("Water_temp_state = Heating | Temperatura_input_agua_usada: % f Water_Temp_Setpoint: % f Water_Heater_delta: % f\n", Temperatura_input_agua_usada, Water_Temp_Setpoint, Water_Heater_delta);
        }
        if ((Temperatura_input_agua_usada >= ( Water_Temp_Setpoint + Water_Cooler_delta)) && Water_temp_state != Cooling) {
          Water_temp_state = Cooling;
          //Serial.printf("Water_temp_state = Cooling | Temperatura_input_agua_usada: % f Water_Temp_Setpoint: % f Water_Cooler_delta: % f\n", Temperatura_input_agua_usada, Water_Temp_Setpoint, Water_Cooler_delta);
        }
      }
    }
    /*
      if ((Main_SW_State  == Manual) && (Tide_sw_state == ON)) {
      Water_temp_state = Idle;//Nao aquecer/arrefecer em mare manual.
      }
    */



    /*Aplicar caso Idle*/
    if (Water_temp_state == Idle) {
      // Serial.println("Entrei em Idle");
      Water_Heater_state = Off;
      Toggle_GPIO(10, Water_Heater_state);

      //   if (Water_Cooler_state == On || Time_Cooler_powerOFF == 0) {
      if (Water_Cooler_state == On) {
        Time_Cooler_powerOFF = millis();
        Serial.print("Atualizei Time_Cooler_powerOFF: "); Serial.println(Time_Cooler_powerOFF);
        Water_Cooler_state = Off;
        Toggle_GPIO(9, Water_Cooler_state);
      }
    }
    /**/

    /*Aplicar caso Heating:*/
    if (Water_temp_state == Heating) {
      //Aquecedor:
      //Water_Heater_state = On;
      if ((Bottom_sensors && !Water_BOt_bot_detected) || !Bot_Bot_sensors_func) {
        Water_Heater_state = Off;
        Toggle_GPIO(10, Water_Heater_state);
        Serial.println("É suposto estar a aquecer mas falta deteção de água no nivel mais inferior.");
      } else {
        if (Main_SW_State  == Auto) {
          Water_Heater_state = On;
          Toggle_GPIO(10, Water_Heater_state);
        }
        if (Main_SW_State  == Manual && (W_Heat_sw_state == Auto) && (Pump_sw_state  == Auto)) {
          Water_Heater_state = On;
          Toggle_GPIO(10, Water_Heater_state);
        }
        if (Main_SW_State  == Manual && (W_Heat_sw_state == OFF)) {
          Water_Heater_state = Off;
          Toggle_GPIO(10, Water_Heater_state);


          if (TPA_state == 0)Water_temp_state = Idle;
        }
        if (Main_SW_State  == Manual && (Pump_sw_state == OFF)) {
          Water_Heater_state = Off;
          Toggle_GPIO(10, Water_Heater_state);

          if (TPA_state == 0)Water_temp_state = Idle;
        }
      }

      //Cooler:
      if (Water_Cooler_state == On) {
        Time_Cooler_powerOFF = millis();
        Serial.print("Atualizei Time_Cooler_powerOFF: "); Serial.println(Time_Cooler_powerOFF);
        Water_Cooler_state = Off;
        Toggle_GPIO(9, Water_Cooler_state);
      }
    }
    /**/

    /*Aplicar caso Cooling:*/
    if (Water_temp_state == Cooling) {

      //Assegura que dentro do cooling, o heating esta desligado:
      Water_Heater_state = Off;
      Toggle_GPIO(10, Water_Heater_state);
      //

      //Condições de segurança para desligar o cooler:
      if ((Bottom_sensors && !Water_BOt_bot_detected)
          || (Main_SW_State  == Manual && (Cool_sw_state == OFF))
          || (Main_SW_State  == Manual && (Pump_sw_state == OFF))
          || !Bot_Bot_sensors_func) {

        if (Water_Cooler_state == On) {
          Time_Cooler_powerOFF = millis();
          Water_Cooler_state = Off;
          Toggle_GPIO(9, Water_Cooler_state);
          Waiting_for_cooler = false;
          Serial.println("Desliguei Cooler por falta de circulação de água e atualizaei Time_Cooler_powerOFF.");
        }
      }
      //

      //É seguro ligar Cooler?(nao esteve ligado a menos de 3 minutos?)
      if (((Water_Cooler_state == Off) &&
           ((Bottom_sensors && Water_BOt_bot_detected && Bot_Bot_sensors_func) || !Bottom_sensors) &&
           ((millis()) > Time_Cooler_powerOFF + (1000 * Chiller_wait))) ||
          Time_Cooler_powerOFF == 0) {


        if (Main_SW_State  == Auto ) {
          Serial.println("1 - Liguei Cooler!");
          Water_Cooler_state = On;
          Toggle_GPIO(9, Water_Cooler_state);
          Waiting_for_cooler = false;
        }
        if (Main_SW_State  == Manual && (Cool_sw_state == Auto) && (Pump_sw_state  == Auto)) {
          Serial.println("2 - Liguei Cooler!");
          Water_Cooler_state = On;
          Toggle_GPIO(9, Water_Cooler_state);
          Waiting_for_cooler = false;
        }

        if ( Water_Cooler_state == On && Time_Cooler_powerOFF == 0) {
          Time_Cooler_powerOFF = millis();
          Serial.println("Iniciei Time_Cooler_powerOFF pela 1a vez. ");
        } else {
          Serial.println("É seguro ligar Cooler pk ja passaram Chiller_wait segundos desde que o desliguei.");
          Serial.print((millis()));
          Serial.print(" > ");
          Serial.println(Time_Cooler_powerOFF + (1000 * Chiller_wait));
        }
      } else if (Water_Cooler_state == Off && !(Bottom_sensors && Water_BOt_bot_detected && Bot_Bot_sensors_func)) {
        Serial.println("É suposto estar a arrefecer mas falta deteção de água no nivel mais inferior.");
      } else if (Water_Cooler_state == Off && Time_Cooler_powerOFF != 0 && ((millis()) < Time_Cooler_powerOFF + (1000 * Chiller_wait))) {
        Serial.print("É suposto estar a arrefecer mas o Cooler está em repouso de"); Serial.printf(" %i segundos.\n", Chiller_wait);
      }

      //Apenas para registar na variavel Waiting_for_cooler que o Cooler devia estar ligado mas ainda está em espera.
      if (((Water_Cooler_state == Off) && ((millis()) < Time_Cooler_powerOFF + (1000 * Chiller_wait))))Waiting_for_cooler = true;
      //

    }
  } else {

    if (TPA_state == 0) {
      if (Water_temp_state != Idle)Water_temp_state = Idle;
      //Serial.println("Water_temp_state = Idle;");
    }

    if (Water_Cooler_state == On) {
      Time_Cooler_powerOFF = millis();
      Water_Cooler_state = Off;
      Toggle_GPIO(9, Water_Cooler_state);
      Waiting_for_cooler = false;
      Serial.println("||Desliguei Cooler por falta de circulação de água e atualizaei Time_Cooler_powerOFF.");
    }
    Water_Heater_state = Off;
    Toggle_GPIO(10, Water_Heater_state);
  }



  //if (Main_SW_State == OFF || (!Water_Temp_sensors_functional && TPA_state == 0)) {
  if (Main_SW_State == OFF) {

    if (TPA_state == 0)Water_temp_state = Idle;
    // if (!Water_Temp_sensors_functional)Serial.println("2Water_Temp_sensors_functional is FALSE!");
  }

  //Colocar  Water_temp_state = Halt; caso seja suposto estar a aquecer ou arrefecer mas falta sensores:
  if ((Now_Tide == 1 && (Bottom_sensors && !Water_BOt_bot_detected)) ||
      !Bot_Bot_sensors_func ||
      !Water_Temp_sensors_functional) {
    Water_temp_state = Halt;
    /*
      Serial.println("HALLLT");
      Serial.println(Bot_Bot_sensors_func);
      Serial.println(Water_Temp_sensors_functional);
      Serial.println((Now_Tide && (Bottom_sensors && !Water_BOt_bot_detected)));
      Serial.println(Now_Tide);
      Serial.println(Bottom_sensors);
      Serial.println(Water_BOt_bot_detected);
    */
  }
  //

  //Caso esteja em troca de agua, não faz controlo de temperatura de água:
  static byte Prev_TPA_state = 4; //So vai de 0 a 3;
  if (Prev_TPA_state != TPA_state) {
    Prev_TPA_state = TPA_state;
    if (TPA_state == 1) {
      Water_temp_state = Out;
      Serial.println("Water_temp_state = Out");
    }
    if (TPA_state == 2) {
      Water_temp_state = In;
      Serial.println("Water_temp_state = In;");
    }
    if (TPA_state == 3) {
      Water_temp_state = Halt;
      Serial.println("Water_temp_state = Halt;");
      Serial.printf("TPA_state:%i \n"), TPA_state;
    }
  }

  //Serial.print("0Water_temp_state:"); Serial.println(Water_temp_state);

  //
  /*Update water_Temp_state TFT display:*/
  //Water cooling state
  static Temp_state Prev_Water_temp_state = Idle;
  if (Water_temp_state != Prev_Water_temp_state) {
    //Serial.print("0Prev_Water_temp_state: "); Serial.println(Prev_Water_temp_state);

    xSemaphoreTake(mutex, portMAX_DELAY);
    //tft.setCursor(110, 224);
    tft.setTextColor(BLACK);
    if (Prev_Water_temp_state == Idle)drawCentreString("Idle", 130, 224, 4, 2);//tft.print("Idle");
    if (Prev_Water_temp_state == Heating)drawCentreString("Heating", 130, 224, 7, 2);//tft.print("Heating");
    if (Prev_Water_temp_state == Cooling)drawCentreString("Cooling", 130, 224, 7, 2);//tft.print("Cooling");
    if (Prev_Water_temp_state == Out)drawCentreString("WCh Out", 130, 224, 7, 2);//tft.print("WCh Out);
    if (Prev_Water_temp_state == In)drawCentreString("WCh In", 130, 224, 6, 2);//tft.print("W-Ex In");
    if (Prev_Water_temp_state == Halt)drawCentreString("Halt!", 130, 224, 5, 2);//tft.print("W-Ex In");

    if (Water_temp_state == Heating) {
      tft.setTextColor(ORANGE);
      drawCentreString("Heating", 130, 224, 7, 2);
      //tft.print("Heating");
    }
    if (Water_temp_state == Cooling) {
      tft.setTextColor(BLUE);
      drawCentreString("Cooling", 130, 224, 7, 2);
      //tft.print("Cooling");
    }
    if (Water_temp_state == Idle) {
      tft.setTextColor(WHITE);
      drawCentreString("Idle", 130, 224, 4, 2);
      //tft.print("Idle");
    }

    if (Water_temp_state == Out) {
      tft.setTextColor(WHITE);
      drawCentreString("WCh Out", 130, 224, 7, 2);
      //tft.print("Idle");
    }

    if (Water_temp_state == In) {
      tft.setTextColor(WHITE);
      drawCentreString("WCh In", 130, 224, 6, 2);
      //tft.print("Idle");
    }

    if (Water_temp_state == Halt) {
      tft.setTextColor(RED);
      drawCentreString("Halt!", 130, 224, 5, 2);
      //tft.print("Idle");
    }
    xSemaphoreGive(mutex);
    Prev_Water_temp_state = Water_temp_state;
  }

}

void Apply_Pumps() {
  /*Ambas as Pumps: Se não tiver em All_off ou Manual+Pump_off:*/
  //1-Cooler_Pump_state:
  //1.1- Liga se tiver a decorrer uma experiencia e houver agua no Bot_bot e nao estiver em modo Halte houver mares cheia endiante..
  //Opcao sem sensores:
  //1.2- Liga se tiver a decorrer uma experiencia e houver mares cheia endiante.
  //(Ponderação não implementada)1.3- Desliga se não for a ter mais marés cheias (Lowtide e No_more_tide_change).

  //2-Tide_Pump_state:
  //2.1- Liga se for mare cheia e houver agua no Bot_bot e nao estiver em modo Halt.
  //2.2- Liga se tiver em Manual-HighTide e houver agua no Bot_bot e nao estiver em modo Halt.
  //2.3- Desliga se tiver em Manual-LowTide .
  //2.4- Desliga se deixar de ter agua no Bot_bot e nao estiver em modo Halt.
  //Opcao sem sensores:
  //2.5- Liga se for mare cheia.
  //2.6- Liga se tiver em Manual-HighTide.

  /*Atualizo o estado da mare para este momento e maré Manual:*/
  if (runStatus == Ongoing && (target_Step_Index < profileLength)) {
    //Serial.printf("target_Step_Index:%i profileLength:%i\n", target_Step_Index, profileLength);
    Now_Tide = (byte)((profileTTL[target_Step_Index - 1] / 1000)  % 10);
    //Serial.printf("Now_Tide by profile set to: %i .\n", Now_Tide);
  } else if (runStatus == Finished || (target_Step_Index >= profileLength)) {
    //Serial.println("Now_Tide set to -1 pk acabou experiencia");
    Now_Tide = -1;
  } else if (Main_SW_State  != Manual || Tide_sw_state != ON) {
    Now_Tide = 0;
    //Serial.println("Now_Tide set to 0 pk nao esta em experiencia e os switches nao forçam mare manual.");
  }

  if (Now_Tide == 1 && No_water_instant != 0)No_water_instant = 0; //Para voltar a guardar o instante em que os sensores de agua de cima ja nao detetam agua na mare baixa.

  if (Main_SW_State  == Manual && Tide_sw_state == OFF)Now_Tide = 0;
  if (Main_SW_State  == Manual && Tide_sw_state == ON)Now_Tide = 1;



  if ((Main_SW_State != OFF) &&
      !(Main_SW_State == Manual &&
        Pump_sw_state == OFF) &&
      Now_Tide > -1) {
    //Se não tiver em All_off ou Manual+Pump_off:
    if ((runStatus == Waiting_to_start || runStatus ==  Ongoing )
        && (Bottom_sensors
            && Water_BOt_bot_detected
            && Bot_Bot_sensors_func)
        && TPA_state != 3) {
      //Liga se tiver a decorrer uma experiencia, ou para decorrer ainda, e houver agua em no Bot_bot ou estiver em não modo Halt.
      Cooler_Pump_state = On;//1.1
      if (Now_Tide == 1)Tide_Pump_state = On;//2.1
    } else {
      //1.2- Liga se tiver a decorrer uma experiencia.
      if ((runStatus == Waiting_to_start || runStatus ==  Ongoing ) && !Bottom_sensors) Cooler_Pump_state = On;
      else Cooler_Pump_state = Off;
    }

    if (Now_Tide == 1 && (Bottom_sensors && Water_BOt_bot_detected && Bot_Bot_sensors_func) && TPA_state != 3)Tide_Pump_state = On; //2.2

    if (Now_Tide == 0) Tide_Pump_state = Off; //2.3
    if (Now_Tide == 1 && (Bottom_sensors && !Water_BOt_bot_detected) && TPA_state != 3)Tide_Pump_state = Off; //2.4

    //   if (!Bottom_sensors && Now_Tide == 1) Tide_Pump_state = On;//2.5 e 2.6

  } else {
    //Serial.printf("Entrei em desligar bombas com Now_Tide:%i.\n", Now_Tide);
    Cooler_Pump_state = Off;
    Tide_Pump_state = Off;
  }


  /*Caso tenha feito reboot agora:*/
  static bool Fst_tide = true;
  if (Fst_tide) {
    Prev_Tide = Now_Tide;
    Fst_tide = false;
  }
  /**/

  /*Mare mudou? Automatico ou Manual:*/
  if (Prev_Tide != Now_Tide) {
    //Mare mudou:
    Ja_sei_next_Water_Setpoint = false;//Causa a chamada de Qual_sera_Water_Temp_Setpoint().
    Serial.printf("Tide changed from %i to %i at %i seconds and Now_TPA:%i.\n", Prev_Tide, Now_Tide, nowTimeUnix, Now_WEx);
    Prev_Tide = Now_Tide;
    Serial.print("Adjusted Tide_change_instant from "); Serial.print(Tide_change_instant);
    Tide_change_instant = nowTimeUnix;
    Serial.print(" to "); Serial.println(Tide_change_instant);

    if (!Quando_sera_proxima_tide_change()) {
      No_more_tide_change = true;
      if (Now_Tide == 0) {
        Water_Temp_Setpoint = 0;
        Cooler_Pump_state = Off;
        Serial.println("No_more_tide_change set True and Water_Temp_Setpoint set 0 and  Cooler_Pump_state = Off;");
      }
      Serial.println("No_more_tide_change set True ");
    } else No_more_tide_change = false;
    //
  }

  if (Water_Temp_Setpoint == 0 && !No_more_tide_change && !(Main_SW_State == Manual &&  Tide_sw_state == ON)) {
    Qual_sera_Water_Temp_Setpoint();
    // Serial.printf("Calculei de novo o Water_Temp_Setpoint:%f.\n", Water_Temp_Setpoint);
  } //else Serial.printf("Nao Calculei de novo o Water_Temp_Setpoint:%f.\n", Water_Temp_Setpoint);



  //Desliga Cooler pump se nao houver mares cheia à vista:
  if ((Bottom_sensors && Water_BOt_bot_detected) && (Main_SW_State != OFF) && (runStatus == Waiting_to_start || runStatus ==  Ongoing ) && (Water_Temp_Setpoint != 0) && !(Main_SW_State == Manual && Pump_sw_state == OFF)) { //nao testei com !(Main_SW_State == Manual && Pump_sw_state == On)
    Cooler_Pump_state = On;
    //Serial.println("Cooler_Pump_state = On; !");
    // Serial.print("1Water_Temp_Setpoint:::"); Serial.println(Water_Temp_Setpoint);
  } else {
    Cooler_Pump_state = Off;
    //Serial.println("Cooler_Pump_state = Off; !");
    //Serial.print("2Water_Temp_Setpoint:::"); Serial.println(Water_Temp_Setpoint);
  }

  if (Main_SW_State == Manual && Pump_sw_state == OFF) {
    //Serial.println("Cooler_Pump_state = Offfffff; !");
    Cooler_Pump_state = Off;
    Tide_Pump_state = Off;
  } else {
    //Serial.println("Nao esta off???");
    //Serial.println(Main_SW_State);
    //Serial.println(Pump_sw_state);

  }


  //Serial.print("Now_Tide em Apply_Pumps:"); Serial.println(Now_Tide);

  //Realmente ligar ou não ligar:
  Toggle_GPIO(11, Cooler_Pump_state);
  Toggle_GPIO(12, Tide_Pump_state);
}
