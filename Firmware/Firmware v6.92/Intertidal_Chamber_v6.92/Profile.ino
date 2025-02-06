

void Set_state_n_intial_tide () {

  //tft.setTextSize(2); tft.setTextColor(WHITE);//Precisa de estara aqui?

  if (Profile_ok && RTC_Pos_2022) {
    //Serial.println("Setup: runStatus = Check_runStatus();");

    runStatus = Check_runStatus();

    if (runStatus == Ongoing) {

      //Initial tide concerns:
      Now_Tide = (bool)((profileTTL[target_Step_Index - 1] / 1000)  % 10);
      Serial.printf("Setup_Now_Tide:%d\n", Now_Tide);


      if (!Now_Tide) {
        int i = 0;
        for ( i = target_Step_Index; i >= 0; i--) {
          //Serial.printf("i=%d - Tide:%d\n", i, (bool)((profileTTL[i - 1] / 10)  % 10));
          if ((bool)((profileTTL[i - 1] / 1000)  % 10))break;
        }

        Serial.printf("0 Adjusted Tide_change_instant from %i to ", Tide_change_instant);
        Tide_change_instant = profileTimes[i];
        Serial.println(Tide_change_instant);

        //Tide_change_instant = nowTimeUnix - Wait_before_air_heat; //Isto é para nao comecar em mare vazia e mesmo assim esperar Chiller_wait segundos para comecar a aquecer.
      }
      //Serial.printf("Adjusted Tide_change_instant to %i.\n", Tide_change_instant);
      //

    }

    Serial.printf("Tide started as: %d\n", Now_Tide);

    //Dados do setpoint de outros instantes:
    //Calculate_Past_n_Future_Temp_Setpoints();
    if (!Quando_sera_proxima_tide_change()) {
      Serial.printf("No_more_tide_change is true on all profile. Set_state_n_intial_tide()\n");
      No_more_tide_change = true;
    }
    // Plotar(Temp_SetPoints, RED, true, 240);//XXX
    //


  }
}
Status Check_runStatus() {
  switch (runStatus)
  {
    case UnPositioned:
      {
        Where_am_I_in_profile();
        return runStatus;
      }
    case Waiting_to_start: // Start time has not been reached yet.
      {
        //Serial.printf("runStatus in zero\n");
        if (nowTimeUnix > profileTimes[0])
        {
          target_Step_Index = 1;
          runStatus = Ongoing;
          Serial.printf("runStatus changed from Waiting_to_start to Ongoing.\n");
        }

        //actuateHeaters_1(0, 1, 0, 0);
        //actuateHeaters_2(0, 1, 0, 0);
        return runStatus;
      }
    case Ongoing: // Profile is running.
      {
        if (target_Step_Index >= profileLength) {
          runStatus = Finished;

          //PID_Output_1 = 0;  //Aquecer so para manter seco.
          Cool_FAN_state = Off;

          Serial.printf("runStatus changed from Ongoing to Finished.\n");
          return runStatus;
          //break;
        } else if (nowTimeUnix > profileTimes[target_Step_Index]) target_Step_Index += 1;
        /*
                // update targets
                nowTimes[0] = profileTimes[targetStep - 1];

                nowTimes[1] = profileTimes[targetStep];
                nowTemps[0] = (profileTTLs[targetStep - 1] / 100) / 10.0;
                nowTemps[1] = (profileTTLs[targetStep]   / 100) / 10.0;

                if (!Manual_TIDE.b) {//So atualiza o nowTides pelo ficheiro se o dipPin2 estiver low(para cima).
                  nowTides    = (profileTTLs[targetStep - 1] / 10)  % 10;
                  if (nowTides != Prev_Tide) {
                    Tide_change_instant = nowTime.unixtime();
                    //Serial.print("nowTides:"); Serial.print(nowTides); Serial.print("   Prev_Tide:"); Serial.println(Prev_Tide);
                    //Serial.print("Instante de inicio de mare nova:"); Serial.println( nowTime.unixtime());
                    Prev_Tide = nowTides;
                  }
                }

                nowLights   =  profileTTLs[targetStep - 1]   % 10;

                // find the temperature setpoint, i.e,
                // the exact temperature along the temperature profile ramp at present time
                if (nowTemps[0] == nowTemps[1])
                {
                  T_Setpoint = nowTemps[1];
                } else {
                  m = ((nowTemps[1] - nowTemps[0]) / (nowTimes[1] - nowTimes[0])); // m = (y2 - y1) / (x2 - x1)
                  b = nowTemps[0]; // b = y1 - (m * x1)
                  T_Setpoint = (m * float(nowTimeUnix - nowTimes[0] + PID_RATE_SECS)) + b; // Y = mX + b

                }



                float tempError_1 = T_Input_1 - T_Setpoint;
                float tempError_2 = T_Input_2 - T_Setpoint;


                if (pidNow)
                {
                  PID_Compute_1(T_Input_1, T_Setpoint, Kp,  Ki, Kd, nowTides);
                  PID_Compute_2(T_Input_2, T_Setpoint, Kp,  Ki, Kd, nowTides);
                  pidNow = 0;
                }

                if (killOutput_1) PID_Output_1 = 0;
                if (killOutput_2) PID_Output_2 = 0;


                // Do work.
                if (nowTides) {
                  PID_Output_1 = 0;
                  PID_Output_2 = 0;
                }
                actuateHeaters_1(PID_Output_1, killOutput_1, nowTides, nowLights);
                actuateHeaters_2(PID_Output_2, killOutput_2, nowTides, nowLights);


                //Create and/or log data in a new file every hour.
                Generate_Log();
        */
        return runStatus;

      }

    case Finished: // Experiment is over.
      {
        //Serial.printf("runStatus in 3\n");

        // actuateHeaters_1(0, 1, 0, nowLights);
        // actuateHeaters_2(0, 1, 0, nowLights);

        return runStatus;
      }
    case KILL_: // Experiment is over.
      {
        //Serial.println("KILL_ in Check_runStatus().");

        // actuateHeaters_1(0, 1, 0, nowLights);
        // actuateHeaters_2(0, 1, 0, nowLights);

        return runStatus;
      }
      /*
        default:
        {
        Where_am_I_in_profile();
        //Serial.println("default returned 5.");
        return runStatus;
        //return Unknown;
        }
      */
  }
}




void Calculate_Past_n_Future_Temp_Setpoints() {
  bool Projected_Tide_Setpoint = 0, Projected_TPA_Setpoint = 0;

  //Serial.println("Calculate_Past_n_Future_Temp_Setpoints");
  static long int Projected_nowTimeUnix = nowTimeUnix - 10800;//Projected_nowTimeUnix -> O instante de ha 6h atras. 21600=60seg*60min*6h (6h em segundos)
  static bool Encontreime = false, primeiro_valor = true;
  int j = 0;

  //Serial.print("Projected_nowTimeUnix:");  Serial.println(Projected_nowTimeUnix);
  //Serial.print("profileLength:");  Serial.println(profileLength);
  //Serial.print("(Projected_nowTimeUnix + 32400):");  Serial.println((Projected_nowTimeUnix + 32400));//Instante daqui a 3h (partindo de 6h atras).

  //Ha setpoints de à 3h atras até daqui a 3h?:
  for (long unsigned int i = Projected_nowTimeUnix; i < (Projected_nowTimeUnix + 21600); i = i + 90) {//32400=6h passadas + 3h futuras=60seg*60min*9h
    //1-Percorro todos os 90segundos entre 3h atras e 3h de agora:
    //Serial.print("i:");  Serial.println(i);
    if (!Encontreime) {
      for (long int A = 0; A < profileLength; A++)
      {
        //Serial.print("profileTimes[A]:");  Serial.println(profileTimes[A]);
        if ((i > profileTimes[A]) && (i <= profileTimes[A + 1]))
        {
          Projected_target_Step_Index = (int)(A + 1);
          Encontreime = true;
          // if (Encontreime)Serial.printf("Em Calculate_Past_n_Future_Temp_Setpoints(): Encontrei-me no passo:%i \n", A);
          break;
        }
      }
    }
    if (Encontreime) {
      //Serial.println("Encontreime");
      //Passa ao proximo Projected_target_Step_Index:
      if ((i > profileTimes[Projected_target_Step_Index]) && (Projected_target_Step_Index < profileLength))Projected_target_Step_Index++;
      //



      //Serial.print("Projected_target_Step_Index:");  Serial.println(Projected_target_Step_Index);

      if (Projected_target_Step_Index < profileLength) {

        nowTimes[0] = profileTimes[Projected_target_Step_Index - 1];
        nowTimes[1] = profileTimes[Projected_target_Step_Index];
        nowTemps[0] = (profileTTL[Projected_target_Step_Index - 1] / 10000) / 10.0;
        nowTemps[1] = (profileTTL[Projected_target_Step_Index]   / 10000) / 10.0;
        Projected_Tide_Setpoint = (int)(profileTTL[Projected_target_Step_Index - 1] / 1000)  % 10;
        Projected_TPA_Setpoint = (int)((profileTPA[Projected_target_Step_Index - 1]));

        if (Prev_Prof_TPA_Projected_target_Step_Index  != (Projected_target_Step_Index - 1)) {
          //   Serial.println("cnt reset");
          Prev_Prof_TPA_Projected_target_Step_Index  = Projected_target_Step_Index - 1;
          cnt_profile  = 0;
        }
        if (Projected_TPA_Setpoint == 1 && cnt_profile  < 4) {
          //Serial.println("cnt++");
          cnt_profile ++;
        }

        if ((Projected_TPA_Setpoint == 1) && cnt_profile  > 3) {
          // Serial.println("Projected_TPA_Setpoint=0!!");
          Projected_TPA_Setpoint = 0;
        } //else Serial.println("Projected_TPA_Setpoint=1!!");

        m = ((nowTemps[1] - nowTemps[0]) / (nowTimes[1] - nowTimes[0])); // m = (y2 - y1) / (x2 - x1)
        /*
                Serial.printf("m:%f\n", m);
                Serial.printf("nowTemps[1]:%f\n", nowTemps[1]);
                Serial.printf("nowTemps[0]:%f\n", nowTemps[0]);
                Serial.printf("nowTimes[1]:%li\n", nowTimes[1]);
                Serial.printf("nowTimes[0]:%li\n", nowTimes[0]);
        */
        b = nowTemps[0]; // b = y1 - (m * x1)
        Projected_Temp_Setpoint = (m * float(i - nowTimes[0] + 90)) + b; // Y = mX + b
        //Serial.print("0 Projected_Temp_Setpoint: "); Serial.println(Projected_Temp_Setpoint);


        char Debug[100];
        //  if (((nowTemps[1] > nowTemps[0]) && (Projected_Temp_Setpoint > nowTemps[1])) || ((nowTemps[1] < nowTemps[0]) && (Projected_Temp_Setpoint < nowTemps[1])))Projected_Temp_Setpoint = nowTemps[1];//Impede setpoint acima do objetivo (quero 15ºC mas aponta para 15.13ºC).
        //Impede setpoint acima do objetivo (quero 15ºC mas aponta para 15.13ºC).
        if ((nowTemps[1] >= nowTemps[0]) && (Projected_Temp_Setpoint > nowTemps[1])) {
          Projected_Temp_Setpoint = nowTemps[1];
        }

        if ((nowTemps[1] <= nowTemps[0]) && (Projected_Temp_Setpoint < nowTemps[1])) {
          Projected_Temp_Setpoint = nowTemps[1];
        }
        //     Serial.printf("1Projected_Temp_Setpoint:%f\n", Projected_Temp_Setpoint);
        /*
           if (Projected_Temp_Setpoint > Max_Temp) {
             Max_Temp = Projected_Temp_Setpoint;
           }
           if ((Projected_Temp_Setpoint > 0) && (Projected_Temp_Setpoint < Min_Temp)) {
             Min_Temp = Projected_Temp_Setpoint;
           }
        */
      } else Projected_Temp_Setpoint = 0;
    }
    //Temp_SetPoints[j][0] = Projected_Temp_Setpoint;
    if (primeiro_valor && nowTimes[0] > 0) {
      primeiro_valor = false;
      Temp_SetPoints[j][0] = ((float)((int)(nowTemps[0] * 10))) / 10;//Forma de reduzir a float com 1 decimal.
      Temp_SetPoints[j][1] = Projected_Tide_Setpoint;//Serial.print(Projected_Tide_Setpoint); Serial.printf(" %i ", j); Serial.println(Temp_SetPoints[j][1]);
      Temp_SetPoints[j][2] = Projected_TPA_Setpoint;//Serial.print(Projected_Tide_Setpoint); Serial.printf(" %i ", j); Serial.println(Temp_SetPoints[j][1]);

      //Serial.printf("1!-Temp_SetPoints[j][0]:%f\n", Temp_SetPoints[j][0]);
      //Serial.printf("1!-Temp_SetPoints[%i][1]:%f\n",  j,Temp_SetPoints[j][1]);

    } else if (!primeiro_valor) {
      Temp_SetPoints[j][0] = ((float)((int)(Projected_Temp_Setpoint * 10))) / 10;//Forma de reduzir a float com 1 decimal.
      Temp_SetPoints[j][1] = Projected_Tide_Setpoint;//Serial.print(Projected_Tide_Setpoint); Serial.printf(" %i ", j); Serial.println(Temp_SetPoints[j][1]);
      Temp_SetPoints[j][2] = Projected_TPA_Setpoint;//Serial.print(Projected_Tide_Setpoint); Serial.printf(" %i ", j); Serial.println(Temp_SetPoints[j][1]);


      //Serial.printf("2!-Temp_SetPoints[j][0]:%f\n", Temp_SetPoints[j][0]);
      //Serial.printf("2!-Temp_SetPoints[%i][1]:%f\n",  j,Temp_SetPoints[j][1]);
    }

    j++;
  }

  //Serial.println("Setpoints:");
  for (int i = 0; i < 240; i++) {
    /*
      Serial.print(Temp_SetPoints[i][0]); Serial.print(" ");
      Serial.print(Temp_SetPoints[i][1]); Serial.print(" ");
      Serial.println(Temp_SetPoints[i][2]);
    */
  }

}

void Where_am_I_in_profile() {
  //Serial.println("Running Where_am_I_in_profile();");

  // Identify profile step
  //runStatus starts as 0 until a state is choosen. Fisrt time Check_runStatus() calls Where_am_I_in_profile() to select a runStatus.
  // targetStep = 0 means profile start time has not yet been reached (runStatus = 1)
  // targetStep = profileLength means the profile has been completed (runStatus = 3)
  // 0 < targetStep < profileLength means operation is being resumed (runStatus = 2)
  if (nowTimeUnix > profileTimes[0])
  {
    if (nowTimeUnix > profileTimes[profileLength - 1])
    {
      target_Step_Index = profileLength;
      runStatus = Finished;
    } else {
      for (long i = 1; i < profileLength; i++)
      {
        if (nowTimeUnix > profileTimes[i - 1] & nowTimeUnix <= profileTimes[i])
        {
          target_Step_Index = (int)(i);
          runStatus = Ongoing;
          break;
        }
      }
    }
  } else {
    runStatus = Waiting_to_start;//Esperava que isto desse merda quando estou em estado UnPositioned por nao ter Profile.txt ou este estar vazio.
  }

   //Falta ver como nao ter erros se estiver no 1o passo e nao existir target_Step_Index - 1.
  //Assim como a intervanção dos sensores.
  Prev_Tide = (profileTTL[target_Step_Index - 1] / 1000)  % 10; //Faz com que mare anterior seja a mare lida no momento.
  //Isto evita que ele encha de novo quando ja estava em mare cheia.

  //Serial.printf("Prev_Tide:%i \n", Prev_Tide);

}
void Turn_all_off(bool Override_Manual_SW) {
  Serial.println("Turn_all_off()");
  Heat_Lamp_state = Off;
  Toggle_GPIO(8, Heat_Lamp_state);

  Water_Cooler_state = Off;
  Toggle_GPIO(9, Water_Cooler_state);

  Cooler_Pump_state = Off;
  Toggle_GPIO(11, Cooler_Pump_state);

  Water_Heater_state = Off;
  Toggle_GPIO(10, Water_Heater_state);


  //Coisas que desliga mesmo sem o Override_Manual_SW:
  //mcp_20.digitalWrite(8, LOW);//230v_1=Heat lamps
  //mcp_20.digitalWrite(9, LOW);//230v_2=Cooler
  //mcp_20.digitalWrite(11, LOW);//230v_4=Cooler Pump
  //A tide pump pode estar dependente do manual. As ventoinhas tbm.
  //mcp_20.digitalWrite(10, LOW);//230v_3=Water Heater

  if (Main_SW_State != Manual || Override_Manual_SW) {
    Serial.println("Turn_all_off()->Main_SW_State != Manual");
    //230v:

    Tide_Pump_state = Off;
    Toggle_GPIO(12, Tide_Pump_state);
    //mcp_20.digitalWrite(12, LOW);//230v_5=Tide Pump


    //Outputs.digitalWrite(Outputs.eGPA0, LOW);//Lampadas Aquecimento
    //Outputs.digitalWrite(Outputs.eGPA1, LOW);//Bomba que mantem mare cheia.
    //Outputs.digitalWrite(Outputs.eGPA2, LOW);//Aquecimento agua.
    //Outputs.digitalWrite(Outputs.eGPA3, LOW);//Bomba que refrigera agua.
    //Outputs.digitalWrite(Outputs.eGPA4, LOW);//Bomba esta sempre a enviar para o Cooler.

    //12v:
    ledcWrite(0, 0);// mcp_20.digitalWrite(5, LOW);//12v_1=Lights

    Circle_FAN_state = Off;
    Toggle_GPIO(13, Circle_FAN_state);

    Cool_FAN_state = Off;
    Toggle_GPIO(14, Cool_FAN_state);
    Serial.println("9 Cool_FAN_state = Off;");

    Release_WV_state = Off;
    Toggle_GPIO(15, Release_WV_state);

    Enter_WV_state = Off;
    Toggle_GPIO(7, Enter_WV_state);

    //mcp_20.digitalWrite(13, LOW);//12v_2=Circle FAN
    //mcp_20.digitalWrite(14, LOW);//12v_3=Cooling Fan
    //mcp_20.digitalWrite(15, LOW);//12v_4=Close Outlet water valve.
    //mcp_20.digitalWrite(7, LOW);//12v_5=Close Inlet water valve.

    //delay(500);//Sera que precisa?
    //Outputs.digitalWrite(Outputs.eGPB0, LOW);//Lights
    //Outputs.digitalWrite(Outputs.eGPB1, HIGH);//Valvula solenoide->Abrir_agua.
    //Outputs.digitalWrite(Outputs.eGPB2, LOW);//Valvula solenoide->Fechar agua.
    //Outputs.digitalWrite(Outputs.eGPB3, LOW);//Ventoinha que homogeniza.
    //Outputs.digitalWrite(Outputs.eGPB4, LOW);//Ventoinha que extrai ar quente.
  }
}

void Calculate_Temp_Setpoint() {
  //Top tank
  if (runStatus == Ongoing ) { //era (runStatus!=Finished).
    static int Prev_target_Step_Index = -1;

    nowTimes[0] = profileTimes[target_Step_Index - 1];
    nowTimes[1] = profileTimes[target_Step_Index];

    if (Prev_target_Step_Index != target_Step_Index) {
      Prev_target_Step_Index = target_Step_Index;
      //Serial.print(""); Serial.printf("Prev_target_Step_Index:%i != %i:target_Step_Index\n", Prev_target_Step_Index, target_Step_Index);
      nowTemps[0] = (profileTTL[target_Step_Index - 1] / 10000) / 10.0;
      nowTemps[1] = (profileTTL[target_Step_Index]   / 10000) / 10.0;
    }
    /*
        Serial.print("nowTimes[0]"); Serial.print(nowTimes[0]);
        Serial.print("  nowTimes[1]"); Serial.println(nowTimes[1]);
    */
    /*
          Serial.printf("profileTTL[%i]:",target_Step_Index - 1); Serial.println(profileTTL[target_Step_Index - 1]);
          Serial.printf("profileTTL[%i]:",target_Step_Index); Serial.println(profileTTL[target_Step_Index]);
    */
    /*
        Serial.printf("profileTTL[%i] / 10000:", target_Step_Index - 1); Serial.print(profileTTL[target_Step_Index - 1] / 10000);
        Serial.printf("  profileTTL[%i] / 10000:", target_Step_Index); Serial.println(profileTTL[target_Step_Index] / 10000);

        Serial.print("nowTemps[0]"); Serial.print(nowTemps[0]);
        Serial.print("  nowTemps[1]"); Serial.println(nowTemps[0]);
    */

    if (nowTemps[1] == nowTemps[0] || nowTimes[1] == nowTimes[0] ) {
      Temp_Setpoint = nowTemps[1];
      Temp_Set_Direction = 0;
      /*
        Serial.print("Equal values: nowTemps[0]:"); Serial.print(nowTemps[0]); Serial.print(" nowTemps[1]:"); Serial.print(nowTemps[1]);
        Serial.print(" E-Temp_Setpoint:"); Serial.println(Temp_Setpoint);
      */
    } else {
      if (nowTemps[0] > nowTemps[1]) Temp_Set_Direction = -1;
      if (nowTemps[0] < nowTemps[1]) Temp_Set_Direction = 1;
      if (nowTemps[0] == nowTemps[1]) Temp_Set_Direction = 0;


      m = ((nowTemps[1] - nowTemps[0]) / (nowTimes[1] - nowTimes[0])); // m = (y2 - y1) / (x2 - x1)
      b = nowTemps[0]; // b = y1 - (m * x1)
      Temp_Setpoint = (m * float(nowTimeUnix - nowTimes[0] + PID_RATE_SECS)) + b; // Y = mX + b
      /*
            Serial.print("Calculated by y=mx curve!\n m:"); Serial.print(m); Serial.print(" b:"); Serial.print(b);
            Serial.print(" nowTemps[0]:"); Serial.print(nowTemps[0]); Serial.print(" nowTemps[1]:"); Serial.print(nowTemps[1]);
            Serial.print(" nowTimes[0]:"); Serial.print(nowTimes[0]); Serial.print(" nowTimes[1]:"); Serial.println(nowTimes[1]);
            Serial.print("C-Temp_Setpoint:"); Serial.println(Temp_Setpoint);
      */
      char Debug[100];
      //Impede setpoint acima do objetivo (quero 15ºC mas aponta para 15.13ºC).
      if ((nowTemps[1] > nowTemps[0]) && (Temp_Setpoint > nowTemps[1])) {

        Temp_Setpoint = nowTemps[1];
      }

      if ((nowTemps[1] < nowTemps[0]) && (Temp_Setpoint < nowTemps[1])) {
        Temp_Setpoint = nowTemps[1];
      }

    }
  } else Temp_Setpoint = 0;



  //Base Tank

  if (runStatus == Ongoing && Now_Tide == 1 && !(Main_SW_State  == Manual && Tide_sw_state == ON)) {
    //Serial.printf("aquii Water_Temp_Setpoint:%f.\n", Water_Temp_Setpoint);
    Water_Temp_Setpoint = Temp_Setpoint;
  }
  else {//Se não estou em maré cheia, aponto a Water_Temp_Setpoint para a proxima maré:
    if (!Ja_sei_next_Water_Setpoint) {
      if (runStatus != Finished) {
        //Ja_sei_next_Water_Setpoint é posto a true em Qual_sera_Water_Temp_Setpoint() .
        Serial.println("Fui calcular Qual_sera_Water_Temp_Setpoint() e atualizar o bool No_more_tide_change");
        No_more_tide_change = Qual_sera_Water_Temp_Setpoint();
        Serial.print("Water_Temp_Setpoint apos:"); Serial.println(Water_Temp_Setpoint);
        if (No_more_tide_change)Serial.println("No_more_tide_change is true. Calculate_Temp_Setpoint()"); else Serial.println("No_more_tide_change is false");

      } else {
        //Serial.println("Não calculo Water_Temp_Setpoint pk a experiencia terminou.");
      }

    }
  }

  //Isto é para o target da agua ficar "----" em vez de estar igual ao target do tanque de baixo
  //quando estou em mare alta manual, quer antes estivesse em mare alta ou mais grave, em mare baixa.
  if (Main_SW_State  == Manual && Tide_sw_state == ON) {
    Serial.println("COloquei Water_Temp_Setpoint=0");
    Ja_sei_next_Water_Setpoint = false;//Causa a chamada de Qual_sera_Water_Temp_Setpoint().
    Water_Temp_Setpoint = 0;
  }

  /*
    Serial.print("  |||m:"); Serial.print(m); Serial.print(" b:"); Serial.println(b);
    Serial.print("  |||Temp_Setpoint:"); Serial.println(Temp_Setpoint);
    Serial.print("  |||Water_Temp_Setpoint:"); Serial.println(Water_Temp_Setpoint);
    Serial.println();
  */

}

bool Qual_sera_Water_Temp_Setpoint() {
  Ja_sei_next_Water_Setpoint = true;
  for (int i = target_Step_Index; i < profileLength - 1; i++) {
    if (((profileTTL[i] / 1000)  % 10) > 0) {//Procura instante futuro de mare cheia.
      Water_Temp_Setpoint = (profileTTL[i]   / 10000) / 10.0;

      Serial.printf("Na mare a começar a %02i/%02i/%i %02i:%02i terei Water_Temp_Setpoint:%.02f \n", day(profileTimes[i]),
                    month(profileTimes[i]), year(profileTimes[i]), hour(profileTimes[i]), minute(profileTimes[i]), Water_Temp_Setpoint);
      return false;//So volta true se já não encontrar marés cheias.
    }
  }
  //Serial.println("Nao havera mais maré cheia");
  return true;//Retorna true se não encontrar marés cheias.
}
