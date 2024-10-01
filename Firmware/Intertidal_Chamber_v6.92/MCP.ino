void Update_Water_sensors_display() {
  //state =0 ->Disconnected
  //state =1 ->LOW
  //state =3 ->HIGH
  xSemaphoreTake(mutex, portMAX_DELAY);

  static int Prev_state[8] = { -1, -1, -1, -1, -1, -1, -1, -1};
  byte Top_sensors_ok = 0;
  byte Bot_Bot_sensors_ok = 0;

  if (Prev_state[0] != state[0]) {
    Prev_state[0] = state[0];
    Serial.printf("Sensor 1 changed state %d\n", state[0]);
    if (state[0] == 0)tft.fillRect(178, 159, 8, 36,  RED);
    if (state[0] == 1)tft.fillRect(178, 159, 8, 36,  BLACK);
    if (state[0] == 3)tft.fillRect(178, 159, 8, 36,  BLUE);
  }

  if (Prev_state[1] != state[1]) {
    Prev_state[1] = state[1];
    Serial.printf("Sensor 2 changed state %d\n", state[1]);
    if (state[1] == 0)tft.fillRect(178 + 9, 159, 8, 36,  RED);
    if (state[1] == 1)tft.fillRect(178 + 9, 159, 8, 36,  BLACK);
    if (state[1] == 3)tft.fillRect(178 + 9, 159, 8, 36,  BLUE);
  }

  if (Prev_state[2] != state[2]) {
    Prev_state[2] = state[2];
    Serial.printf("Sensor 3 changed state %d\n", state[2]);
    if (state[2] == 0)tft.fillRect(178 + 9 + 9, 159, 8, 36, RED);
    if (state[2] == 1)tft.fillRect(178 + 9 + 9, 159, 8, 36, BLACK);
    if (state[2] == 3)tft.fillRect(178 + 9 + 9, 159, 8, 36, BLUE);
  }

  if (Prev_state[3] != state[3]) {
    Prev_state[3] = state[3];
    Serial.printf("Sensor 4 changed state %d\n", state[3]);
    if (state[3] == 0)tft.fillRect(178, 202, 26, 36, RED);
    if (state[3] == 1)tft.fillRect(178, 202, 26, 36, BLACK);
    if (state[3] == 3)tft.fillRect(178, 202, 26, 36, BLUE);
  }

  if (Prev_state[4] != state[4]) {
    Prev_state[4] = state[4];
    Serial.printf("Sensor 5 changed state %d\n", state[4]);
    if (state[4] == 0)tft.fillRect(178, 242, 26, 36, RED);
    if (state[4] == 1)tft.fillRect(178, 242, 26, 36, BLACK);
    if (state[4] == 3)tft.fillRect(178, 242, 26, 36, BLUE);

  }

  if (Prev_state[5] != state[5]) {
    Prev_state[5] = state[5];
    Serial.printf("Sensor 6 changed state %d\n", state[5]);
    if (state[5] == 0)tft.fillRect(178, 282, 8, 35, RED);
    if (state[5] == 1)tft.fillRect(178, 282, 8, 35, BLACK);
    if (state[5] == 3)tft.fillRect(178, 282, 8, 35, BLUE);
  }

  if (Prev_state[6] != state[6]) {
    Prev_state[6] = state[6];
    Serial.printf("Sensor 7 changed state %d\n", state[6]);
    if (state[6] == 0)tft.fillRect(178 + 9, 282, 8, 35, RED);
    if (state[6] == 1)tft.fillRect(178 + 9, 282, 8, 35, BLACK);
    if (state[6] == 3)tft.fillRect(178 + 9, 282, 8, 35, BLUE);
  }

  if (Prev_state[7] != state[7]) {
    Prev_state[7] = state[7];
    Serial.printf("Sensor 8 changed state %d\n", state[7]);
    if (state[7] == 0)tft.fillRect(178 + 9 + 9, 282, 8, 35, RED);
    if (state[7] == 1)tft.fillRect(178 + 9 + 9, 282, 8, 35, BLACK);
    if (state[7] == 3)tft.fillRect(178 + 9 + 9, 282, 8, 35, BLUE);
  }

  //Contabilizar sensores bons:
  if (state[0] != 0)Top_sensors_ok++;
  if (state[1] != 0)Top_sensors_ok++;
  if (state[2] != 0)Top_sensors_ok++;
  if (state[5] != 0)Bot_Bot_sensors_ok++;
  if (state[6] != 0)Bot_Bot_sensors_ok++;
  if (state[7] != 0)Bot_Bot_sensors_ok++;

  if (Bot_Bot_sensors_ok >= 2)Bot_Bot_sensors_func = true; else Bot_Bot_sensors_func = false;
  if (Top_sensors_ok >= 2)Top_sensors_func = true; else Top_sensors_func = false;

  //Versão 8 sensores:
  if ((state[2] + state[1] + state[0]) >= 6)Water_top_detected  = true; else Water_top_detected  = false;
  if (state[3] == 3)Water_BOt_top_detected = true; else Water_BOt_top_detected = false;
  if (state[4] == 3)Water_BOt_mid_detected = true; else Water_BOt_mid_detected = false;

  if ((state[7] + state[6] + state[5]) >= 6)Water_BOt_bot_detected = true; else Water_BOt_bot_detected = false;

  if (!WS_incomplete && (!state[3] || !state[4] || !Bot_Bot_sensors_func || !Top_sensors_func)) {
    WS_incomplete = true;
    Serial.println("WS_incomplete is true.");
  }
  if (WS_incomplete && state[3] && state[4] && Bot_Bot_sensors_func && Top_sensors_func) {
    WS_incomplete = false;
    Serial.println("WS_incomplete is false.");
  }

  xSemaphoreGive(mutex);
}

void Update_switchs_display() {
  //if (Int_dispultada_1 || Int_dispultada_2) {
  if (Int_dispultada_1) {
    Serial.println("Int_dispultada_1");
    Atualizar_mcp21_n_display_n_power();
    mcp_21.clearInterrupts();
    //mcp_21.getLastInterruptPin();
    //mcp_23.getLastInterruptPin();
    //mcp_27.getLastInterruptPin();
  }
}

void Atualizar_mcp21_n_display_n_power() {
  xSemaphoreTake(mutex, portMAX_DELAY);
  tft.setTextSize(2);
  tft.setTextColor(WHITE);

  static byte Prev_mcp_21_0 = 2, Prev_mcp_21_1 = 2, Prev_mcp_21_2 = 2,
              Prev_mcp_21_3 = 2, Prev_mcp_21_4 = 2, Prev_mcp_21_5 = 2,
              Prev_mcp_21_8 = 2, Prev_mcp_21_9 = 2, Prev_mcp_21_10 = 2,
              Prev_mcp_21_11 = 2, Prev_mcp_21_12 = 2, Prev_mcp_21_13 = 2,
              Prev_mcp_21_14 = 2, Prev_mcp_21_15 = 2;

  byte mcp_21_0, mcp_21_1, mcp_21_2,
       mcp_21_3, mcp_21_4, mcp_21_5,
       mcp_21_8, mcp_21_9, mcp_21_10,
       mcp_21_11, mcp_21_12, mcp_21_13,
       mcp_21_14, mcp_21_15;


  if (Int_dispultada_1) {
    Serial.println("Int_1 set!");
    delay(5);
    mcp_21_0 = mcp_21.digitalRead(0);
    mcp_21_1 = mcp_21.digitalRead(1);
    mcp_21_2 = mcp_21.digitalRead(2);
    mcp_21_3 = mcp_21.digitalRead(3);
    mcp_21_4 = mcp_21.digitalRead(4);
    mcp_21_5 = mcp_21.digitalRead(5);
    //mcp_21_6 = mcp_21.digitalRead(6);//NOT_USED
    //mcp_21_7 = mcp_21.digitalRead(7);//NOT_USED
    mcp_21_8 = mcp_21.digitalRead(8);
    mcp_21_9 = mcp_21.digitalRead(9);
    mcp_21_10 = mcp_21.digitalRead(10);
    mcp_21_11 = mcp_21.digitalRead(11);
    mcp_21_12 = mcp_21.digitalRead(12);
    mcp_21_13 = mcp_21.digitalRead(13);
    mcp_21_14 = mcp_21.digitalRead(14);
    mcp_21_15 = mcp_21.digitalRead(15);

  }

  if (Int_dispultada_1) {

    if (Prev_mcp_21_8 != mcp_21_8 || Prev_mcp_21_9 != mcp_21_9) {
      Prev_mcp_21_8 = mcp_21_8;
      Prev_mcp_21_9 = mcp_21_9;

      //Serial.printf("Mudou mcp_21_8 para %i\n", mcp_21_8);
      if (mcp_21_8 == 0 && mcp_21_9 == 1) {
        Main_SW_State = OFF;
        // tft.fillRoundRect(1, 44, 67, 43, 8, RED );//All_off
        tft.fillRoundRect(1, 44, 47, 43, 8, RED );//Auto

        drawCentreString("All", 26, 50, 3, 2);
        drawCentreString("OFF", 26, 69, 3, 2);
        //tft.setCursor(5, 50); tft.print("All"); tft.setCursor(5, 69); tft.print("OFF");//All_off//18
      }
      if (mcp_21_8 == 1 && mcp_21_9 == 0) {
        Main_SW_State = Manual;
        tft.fillRoundRect(1, 44, 47, 43, 8, ORANGE  );//Manual
        //tft.setCursor(5, 60); tft.print("Man");//Manual
        drawCentreString("Man", 26, 60, 3, 2);
        //
      }

      if (mcp_21_8 == 1 && mcp_21_9 == 1)  {
        Main_SW_State = Auto;
        tft.fillRoundRect(1, 44, 47, 43, 8, DARKGREEN );//Auto

        drawCentreString("Auto", 26, 60, 4, 2);
      }
    }
    // if (Int_dispultada_1)Int_dispultada_1 = false;
  }



  if ((Prev_Main_SW_State != Main_SW_State) && Main_SW_State == Auto) {
    Prev_Main_SW_State = Main_SW_State;

    Prev_mcp_21_0 = 2;
    Prev_mcp_21_1 = 2;
    Prev_mcp_21_2 = 2;
    Prev_mcp_21_3 = 2;
    Prev_mcp_21_4 = 2;
    Prev_mcp_21_5 = 2;
    //Prev_mcp_21_6 = 2;
    //Prev_mcp_21_7 = 2;
    //Prev_mcp_21_8 = 2;
    //Prev_mcp_21_9 = 2;
    Prev_mcp_21_10 = 2;
    Prev_mcp_21_11 = 2;
    Prev_mcp_21_12 = 2;
    Prev_mcp_21_13 = 2;
    Prev_mcp_21_14 = 2;
    Prev_mcp_21_15 = 2;

    //A-Heat
    tft.fillRoundRect(51, 44, 59, 43, 8, DARKGREEN );//Auto
    drawCentreString("AHeat", 81, 50, 5, 2);
    drawCentreString("---", 81, 69, 3, 2);
    //
    //Cool
    tft.fillRoundRect(113, 44, 48, 43, 8, DARKGREEN );//Cool
    drawCentreString("Cool", 139, 50, 4, 2);//137
    drawCentreString("---", 139, 69, 3, 2);
    //
    //Pump
    tft.fillRoundRect(164, 44, 48, 43, 8, DARKGREEN );//Pump
    drawCentreString("Pump", 189, 50, 4, 2);//188
    drawCentreString("---", 189, 69, 3, 2);
    //
    //W-Heat
    tft.fillRoundRect(215, 44, 48 + 12, 43, 8, DARKGREEN ); //W-Heat
    drawCentreString("WHeat", 246, 50, 5, 2);//245
    drawCentreString("---", 246, 69, 3, 2);
    //
    //Tide
    tft.fillRoundRect(278, 44, 48, 43, 8, DARKGREEN );//Tide
    drawCentreString("Tide", 303, 50, 4, 2);//302
    drawCentreString("---", 303, 69, 3, 2);
    //

    //W-Ex
    tft.fillRoundRect(329, 44, 48, 43, 8, DARKGREEN );//W-EX
    drawCentreString("W-Ex", 354, 50, 4, 2);//353
    drawCentreString("---", 354, 69, 3, 2);
    //

    //LED
    tft.fillRoundRect(380, 44, 48, 43, 8, DARKGREEN );//LED
    drawCentreString("LED", 405, 50, 3, 2);//404
    drawCentreString("---", 405, 69, 3, 2);
    //
    //Fan
    tft.fillRoundRect(431, 44, 48, 43, 8, DARKGREEN );//Fan
    drawCentreString("Fan", 456, 50, 3, 2);//455
    drawCentreString("---", 456, 69, 3, 2);
    //
    //

    if (runStatus == Finished) {
      if (TPA_state == 0) {
        Water_temp_state = Idle; //estava comentado pke?

      }

      //PID_Output_1 = 0;  //Aquecer so para manter seco.
      Cool_FAN_state = Off;
      Serial.println("12 Cool_FAN_state = Off;");
      Error_1 = 0;
      Previous_Error = 0;
      Error_Integral_1 = 0;
      Error_Derivative_1 = 0;
      Fst_derivative = true;

      Turn_all_off(false);
    }



  }

  if ((Prev_Main_SW_State != Main_SW_State) && Main_SW_State == OFF) {
    Prev_Main_SW_State = Main_SW_State;
    Turn_all_off(true);

    Prev_mcp_21_0 = 2;
    Prev_mcp_21_1 = 2;
    Prev_mcp_21_2 = 2;
    Prev_mcp_21_3 = 2;
    Prev_mcp_21_4 = 2;
    Prev_mcp_21_5 = 2;
    //Prev_mcp_21_6 = 2;//NOT_USED
    //Prev_mcp_21_7 = 2;//NOT_USED
    //Prev_mcp_21_8 = 2;//Manual
    //Prev_mcp_21_9 = 2;//All_OFF
    Prev_mcp_21_10 = 2;
    Prev_mcp_21_11 = 2;
    Prev_mcp_21_12 = 2;
    Prev_mcp_21_13 = 2;
    Prev_mcp_21_14 = 2;
    Prev_mcp_21_15 = 2;

    //A-Heat
    PID_Output_1 = 0;
    Error_1 = 0;
    Previous_Error = 0;
    Error_Integral_1 = 0;
    Error_Derivative_1 = 0;
    Fst_derivative = true;


    tft.fillRoundRect(51, 44, 59, 43, 8, RED );//Auto
    //tft.fillRoundRect(71, 44, 66, 43, 8, RED );//A-Heat
    drawCentreString("AHeat", 81, 50, 5, 2);
    drawCentreString("---", 81, 69, 3, 2);
    //

    //Cool
    tft.fillRoundRect(113, 44, 48, 43, 8, RED );//Cool
    drawCentreString("Cool", 139, 50, 4, 2);//137
    drawCentreString("---", 139, 69, 3, 2);
    //
    //Pump
    tft.fillRoundRect(164, 44, 48, 43, 8, RED );//Cool
    drawCentreString("Pump", 189, 50, 4, 2);//188
    drawCentreString("---", 189, 69, 3, 2);
    //
    //W-Heat
    tft.fillRoundRect(215, 44, 48 + 12, 43, 8, RED ); //W-Heat
    drawCentreString("WHeat", 246, 50, 5, 2);//245
    drawCentreString("---", 246, 69, 3, 2);
    //
    //Tide
    tft.fillRoundRect(278, 44, 48, 43, 8, RED );//Tide
    drawCentreString("Tide", 303, 50, 4, 2);//302
    drawCentreString("---", 303, 69, 3, 2);
    //

    //W-EX
    tft.fillRoundRect(329, 44, 48, 43, 8, RED );//W-EX
    drawCentreString("W-Ex", 354, 50, 4, 2);//353
    drawCentreString("---", 354, 69, 3, 2);
    //

    //LED
    tft.fillRoundRect(380, 44, 48, 43, 8, RED );//LED
    drawCentreString("LED", 405, 50, 3, 2);//404
    drawCentreString("---", 405, 69, 3, 2);
    //

    //Fan
    tft.fillRoundRect(431, 44, 48, 43, 8, RED );//Fan
    drawCentreString("Fan", 456, 50, 3, 2);//455
    drawCentreString("---", 456, 69, 3, 2);
    //

  }

  if (Main_SW_State == Manual) {
    // Serial.println("Entrei em Main_SW_State == Manual .");
    Prev_Main_SW_State = Main_SW_State;

    // Int_dispultada_1 = false;
    if (Int_dispultada_1) {
      if (Prev_mcp_21_10 != mcp_21_10) {
        Prev_mcp_21_10 = mcp_21_10;
        //Serial.printf("Mudou mcp_21_10 para %i\n", mcp_21_10);
        if (mcp_21_10 == 0) {

          A_Heat_sw_state = OFF;

          Heat_Lamp_state = Off;
          Toggle_GPIO(8, Heat_Lamp_state);

          // mcp_20.digitalWrite(8, LOW);//230v_1=Heat lamps

          PID_Output_1 = 0;
          //Cool_FAN_state = Off; //Pke desligar as coolings fans quando desligo as heat lamps?
          //Serial.println("11 Cool_FAN_state = Off;");
          Previous_Error = 0;
          Error_1 = 0;
          Error_Integral_1 = 0;
          Error_Derivative_1 = 0;
          Fst_derivative = true;



          tft.fillRoundRect(51, 44, 59, 43, 8, RED );//A-Heat
          drawCentreString("AHeat", 81, 50, 5, 2);
          drawCentreString("OFF", 81, 69, 3, 2);

        } else {
          A_Heat_sw_state = Auto;
          tft.fillRoundRect(51, 44, 59, 43, 8, DARKGREEN );//A-Heat
          drawCentreString("AHeat", 81, 50, 5, 2);
          drawCentreString("Auto", 81, 69, 4, 2);

        }
      }

      if (Prev_mcp_21_11 != mcp_21_11) {
        Prev_mcp_21_11 = mcp_21_11 ;
        //Serial.printf("Mudou mcp_21_11 para %i\n", mcp_21_11);
        if (mcp_21_11 == 0) {
          Water_Cooler_state = Off;
          Toggle_GPIO(9, Water_Heater_state);
          //mcp_20.digitalWrite(9, LOW);//230v_2=Cooler
          Cool_sw_state = OFF;
          tft.fillRoundRect(113, 44, 48, 43, 8, RED );//Cool
          drawCentreString("Cool", 139, 50, 4, 2);//137
          drawCentreString("OFF", 139, 69, 3, 2);
        } else {
          Cool_sw_state = Auto;


          tft.fillRoundRect(113, 44, 48, 43, 8, DARKGREEN );//Cool
          drawCentreString("Cool", 139, 50, 4, 2);//137
          drawCentreString("Auto", 139, 69, 4, 2);
        }
      }

      if (Prev_mcp_21_12 != mcp_21_12) {
        Prev_mcp_21_12 = mcp_21_12;
        //Serial.printf("Mudou mcp_21_12 para %i\n", mcp_21_12);
        if (mcp_21_12 == 0) {
          Tide_Pump_state = Off;
          Toggle_GPIO(12, Tide_Pump_state);
          Cooler_Pump_state = Off;
          Toggle_GPIO(11, Cooler_Pump_state);
          //mcp_20.digitalWrite(11, LOW);//230v_4=Cooler Pump
          //mcp_20.digitalWrite(12, LOW);//230v_5=Tide Pump
          Pump_sw_state = OFF;

          tft.fillRoundRect(164, 44, 48, 43, 8, RED );//Cool
          drawCentreString("Pump", 189, 50, 4, 2);//188
          drawCentreString("OFF", 189, 69, 3, 2);
        } else {
          Pump_sw_state = Auto;
          tft.fillRoundRect(164, 44, 48, 43, 8, DARKGREEN );//Cool
          drawCentreString("Pump", 189, 50, 4, 2);//188
          drawCentreString("Auto", 189, 69, 4, 2);
        }
      }
      if (Prev_mcp_21_13 != mcp_21_13) {
        Prev_mcp_21_13 = mcp_21_13;
        //Serial.printf("Mudou mcp_21_13 para %i\n", mcp_21_13);
        if (mcp_21_13 == 0) {
          Water_Heater_state = Off;
          Toggle_GPIO(10, Water_Heater_state);
          //mcp_20.digitalWrite(10, LOW);//230v_3=Water Heater
          W_Heat_sw_state = OFF;
          tft.fillRoundRect(215, 44, 48 + 12, 43, 8, RED ); //W-Heat
          drawCentreString("WHeat", 246, 50, 5, 2);//245
          drawCentreString("OFF", 246, 69, 3, 2);
        } else {
          W_Heat_sw_state = Auto;
          tft.fillRoundRect(215, 44, 48 + 12, 43, 8, DARKGREEN ); //W-Heat
          drawCentreString("WHeat", 246, 50, 5, 2);//245
          drawCentreString("Auto", 246, 69, 4, 2);
        }
      }
      if (Prev_mcp_21_14 != mcp_21_14 || Prev_mcp_21_15 != mcp_21_15) {
        Prev_mcp_21_14 = mcp_21_14;
        Prev_mcp_21_15 = mcp_21_15;
        if (mcp_21_14 && !mcp_21_15) {

          Serial.println("Entrei em mare alta manual");
          Tide_sw_state = ON;
          tft.fillRoundRect(278, 44, 48, 43, 8, BLUE );//Tide
          drawCentreString("Tide", 303, 50, 4, 2);//302
          drawCentreString("HIGH", 303, 69, 4, 2);
        }
        if (mcp_21_15 && !mcp_21_14) {
          Tide_sw_state = OFF;
          Serial.println("Entrei em baixa  manual");

          Now_Tide = 0;/*Sera assim?*/
          tft.fillRoundRect(278, 44, 48, 43, 8, RED );//Tide
          drawCentreString("Tide", 303, 50, 4, 2);//302
          drawCentreString("LOW", 303, 69, 3, 2);
        }
        if (mcp_21_14 && mcp_21_15) {
          Tide_sw_state = Auto;
          //Serial.println("Entrei em mare auto.");
          tft.fillRoundRect(278, 44, 48, 43, 8, DARKGREEN );//Tide
          drawCentreString("Tide", 303, 50, 4, 2);//302
          drawCentreString("Auto", 303, 69, 4, 2);
        }
      }
      Int_dispultada_1 = false;
    }

    //W-Ch
    if (Prev_mcp_21_4 != mcp_21_4 || Prev_mcp_21_5 != mcp_21_5) {
      Prev_mcp_21_4 = mcp_21_4;
      Prev_mcp_21_5 = mcp_21_5;
      if (mcp_21_5 && !mcp_21_4) {
        WEx_sw_state = ON;
        WEx_sw_ativated = true;
        tft.fillRoundRect(329, 44, 48, 43, 8, BLUE );//W-EX
        drawCentreString("W-Ex", 354, 50, 4, 2);//353
        drawCentreString("ON", 354, 69, 2, 2);
      }
      if (mcp_21_4 && !mcp_21_5) {
        WEx_sw_state = OFF;
        tft.fillRoundRect(329, 44, 48, 43, 8, RED );//W-EX
        drawCentreString("W-Ex", 354, 50, 4, 2);//353
        drawCentreString("OFF", 354, 69, 3, 2);
      }
      if (mcp_21_4 && mcp_21_5) {
        WEx_sw_state = Auto;
        tft.fillRoundRect(329, 44, 48, 43, 8, DARKGREEN );//W-EX
        drawCentreString("W-Ex", 354, 50, 4, 2);//353
        drawCentreString("Auto", 354, 69, 4, 2);
      }
    }
    //
    //LED
    if (Prev_mcp_21_3 != mcp_21_3 || Prev_mcp_21_2 != mcp_21_2) {
      Prev_mcp_21_3 = mcp_21_3;
      Prev_mcp_21_2 = mcp_21_2;
      if (mcp_21_3 && !mcp_21_2) {
        //mcp_20.digitalWrite(5, HIGH);//12v_1=Lights
        Light_sw_state = ON;
        tft.fillRoundRect(380, 44, 48, 43, 8, BLUE );//LED
        drawCentreString("LED", 405, 50, 3, 2);//404
        drawCentreString("ON", 405, 69, 2, 2);
      }
      if (mcp_21_2 && !mcp_21_3) {
        //mcp_20.digitalWrite(5, LOW);//12v_1=Lights
        Light_sw_state = OFF;
        tft.fillRoundRect(380, 44, 48, 43, 8, RED );//LED
        drawCentreString("LED", 405, 50, 3, 2);//404
        drawCentreString("OFF", 405, 69, 3, 2);
      }
      if (mcp_21_3 && mcp_21_2) {
        Light_sw_state = Auto;
        //if (runStatus != Ongoing)mcp_20.digitalWrite(5, LOW);//12v_1=Lights
        tft.fillRoundRect(380, 44, 48, 43, 8, DARKGREEN );//LED
        drawCentreString("LED", 405, 50, 3, 2);//404
        drawCentreString("Auto", 405, 69, 4, 2);
      }
    }

    //FAN
    if (Prev_mcp_21_1 != mcp_21_1 || Prev_mcp_21_0 != mcp_21_0) {
      Prev_mcp_21_1 = mcp_21_1;
      Prev_mcp_21_0 = mcp_21_0;
      if (mcp_21_1 && !mcp_21_0) {
        //mcp_20.digitalWrite(13, HIGH);//12v_2=Circle FAN
        //mcp_20.digitalWrite(14, HIGH);//12v_3=Cooling Fan
        Fan_sw_state = ON;
        tft.fillRoundRect(431, 44, 48, 43, 8, BLUE );//Fan
        drawCentreString("Fan", 456, 50, 3, 2);//455
        drawCentreString("ON", 456, 69, 2, 2);
      }
      if (mcp_21_0 && !mcp_21_1) {
        // mcp_20.digitalWrite(13, LOW);//12v_2=Circle FAN
        // mcp_20.digitalWrite(14, LOW);//12v_3=Cooling Fan
        Fan_sw_state = OFF;
        tft.fillRoundRect(431, 44, 48, 43, 8, RED );//Fan
        drawCentreString("Fan", 456, 50, 3, 2);//455
        drawCentreString("OFF", 456, 69, 3, 2);
      }
      if (mcp_21_1 && mcp_21_0) {
        Fan_sw_state = Auto;
        if (runStatus != Ongoing) {
          // mcp_20.digitalWrite(13, LOW);//12v_2=Circle FAN
          // mcp_20.digitalWrite(14, LOW);//12v_3=Cooling Fan
        }
        tft.fillRoundRect(431, 44, 48, 43, 8, DARKGREEN );//Fan
        drawCentreString("Fan", 456, 50, 3, 2);//455
        drawCentreString("Auto", 456, 69, 4, 2);
      }
    }


  }
  if (Int_dispultada_1)Int_dispultada_1 = false;
  //  if (Int_dispultada_2)Int_dispultada_2 = false;

  xSemaphoreGive(mutex);
}

void Config_mcp_switches() {
  if (!mcp_21.begin_I2C(0x21)) {
    //0x21 -> Front Panel
    Serial.println("Error on mcp_21.begin_I2C(0x21) - Switches");

  } else Serial.println("MCP23017 0x21 Front Panel initiated.");

  //mcp21:
  mcp_21.pinMode(0, INPUT_PULLUP);//Fan_Manual_OFF
  mcp_21.pinMode(1, INPUT_PULLUP);//Fan_Manual_ON
  mcp_21.pinMode(2, INPUT_PULLUP);//LED_Manual_ON
  mcp_21.pinMode(3, INPUT_PULLUP);//LED_Manual_OFF
  mcp_21.pinMode(4, INPUT_PULLUP);//Valves_Manual_OFF
  mcp_21.pinMode(5, INPUT_PULLUP);//Valves_Manual_ON
  mcp_21.pinMode(6, INPUT_PULLUP);//NOT_USED
  mcp_21.pinMode(7, INPUT_PULLUP);//NOT_USED

  mcp_21.pinMode(8, INPUT_PULLUP);//All_Manual
  mcp_21.pinMode(9, INPUT_PULLUP);//All_OFF
  mcp_21.pinMode(10, INPUT_PULLUP);//Heat_Lamps_Manual_OFF
  mcp_21.pinMode(11, INPUT_PULLUP);//Water_Cooler_Manual_OFF
  mcp_21.pinMode(12, INPUT_PULLUP);//Water_Cooler_Pump_Manual_OFF
  mcp_21.pinMode(13, INPUT_PULLUP);//Water_Heat_Manual_OFF
  mcp_21.pinMode(14, INPUT_PULLUP);//Tide_Manual_HIGH
  mcp_21.pinMode(15, INPUT_PULLUP);//Tide_Manual_LOW

  mcp_21.setupInterrupts(true, false, LOW);

  // enable interrupt on button_pin
  mcp_21.setupInterruptPin(0, CHANGE);//(Pin do mcp (0 a 17),  CHANGE ou LOW ou HIGH) Se nao for em CHANGE esta sempre a ativar!
  mcp_21.setupInterruptPin(1, CHANGE);//(Pin do mcp (0 a 17),  CHANGE ou LOW ou HIGH) Se nao for em CHANGE esta sempre a ativar!
  mcp_21.setupInterruptPin(2, CHANGE);//(Pin do mcp (0 a 17),  CHANGE ou LOW ou HIGH) Se nao for em CHANGE esta sempre a ativar!
  mcp_21.setupInterruptPin(3, CHANGE);//(Pin do mcp (0 a 17),  CHANGE ou LOW ou HIGH) Se nao for em CHANGE esta sempre a ativar!
  mcp_21.setupInterruptPin(4, CHANGE);//(Pin do mcp (0 a 17),  CHANGE ou LOW ou HIGH) Se nao for em CHANGE esta sempre a ativar!
  mcp_21.setupInterruptPin(5, CHANGE);//(Pin do mcp (0 a 17),  CHANGE ou LOW ou HIGH) Se nao for em CHANGE esta sempre a ativar!
  //mcp_21.setupInterruptPin(6, CHANGE);//(Pin do mcp (0 a 17),  CHANGE ou LOW ou HIGH) Se nao for em CHANGE esta sempre a ativar!
  //mcp_21.setupInterruptPin(7, CHANGE);//(Pin do mcp (0 a 17),  CHANGE ou LOW ou HIGH) Se nao for em CHANGE esta sempre a ativar!
  mcp_21.setupInterruptPin(8, CHANGE);//(Pin do mcp (0 a 17),  CHANGE ou LOW ou HIGH) Se nao for em CHANGE esta sempre a ativar!
  mcp_21.setupInterruptPin(9, CHANGE);//(Pin do mcp (0 a 17),  CHANGE ou LOW ou HIGH) Se nao for em CHANGE esta sempre a ativar!
  mcp_21.setupInterruptPin(10, CHANGE);//(Pin do mcp (0 a 17),  CHANGE ou LOW ou HIGH) Se nao for em CHANGE esta sempre a ativar!
  mcp_21.setupInterruptPin(11, CHANGE);//(Pin do mcp (0 a 17),  CHANGE ou LOW ou HIGH) Se nao for em CHANGE esta sempre a ativar!
  mcp_21.setupInterruptPin(12, CHANGE);//(Pin do mcp (0 a 17),  CHANGE ou LOW ou HIGH) Se nao for em CHANGE esta sempre a ativar!
  mcp_21.setupInterruptPin(13, CHANGE);//(Pin do mcp (0 a 17),  CHANGE ou LOW ou HIGH) Se nao for em CHANGE esta sempre a ativar!
  mcp_21.setupInterruptPin(14, CHANGE);//(Pin do mcp (0 a 17),  CHANGE ou LOW ou HIGH) Se nao for em CHANGE esta sempre a ativar!
  mcp_21.setupInterruptPin(15, CHANGE);//(Pin do mcp (0 a 17),  CHANGE ou LOW ou HIGH) Se nao for em CHANGE esta sempre a ativar!

  attachInterrupt(27, Ext_INT1_ISR, CHANGE);//RISING, FALLING, CHANGE, LOW .(LOW nao funciona)

  Atualizar_mcp21_n_display_n_power();
}
void Config_mcp_outputs() {
  //As saidas e sensores de agua:
  mcp_20.begin_I2C(0x20);
  //mcp20:
  mcp_20.pinMode(7, OUTPUT);//12V_5 -> Enter Water Valve.

  mcp_20.pinMode(8, OUTPUT);//230V_1 -> Heat Lamps
  mcp_20.pinMode(9, OUTPUT);//230V_2 -> Cooler
  mcp_20.pinMode(10, OUTPUT);//230V_3 -> Water_Heater
  mcp_20.pinMode(11, OUTPUT);//230V_4 -> Cooler_Pump
  mcp_20.pinMode(12, OUTPUT);//230V_5 -> Tide_Pump
  //pinMode(4, OUTPUT);//12V_1 -> LED

  mcp_20.pinMode(13, OUTPUT);//12V_2 -> Inside_Fan
  mcp_20.pinMode(14, OUTPUT);//12V_3 -> Cooling_Fan
  mcp_20.pinMode(15, OUTPUT);//12V_4 -> Release Water Valve.

  //for (int i = 0; i < 16; i++)mcp_20.digitalWrite(i, 0);//Desligar tudo.

  Heat_Lamp_state = Off;
  Water_Heater_state = Off;
  Water_Cooler_state = Off;
  Tide_Pump_state = Off;
  Cooler_Pump_state = Off;
  Circle_FAN_state = Off;
  Cool_FAN_state = Off;
  Release_WV_state = Off;
  Enter_WV_state = Off;

  Toggle_GPIO(7, Heat_Lamp_state);
  Toggle_GPIO(8, Enter_WV_state);
  Toggle_GPIO(9, Water_Cooler_state);
  Toggle_GPIO(10, Water_Heater_state);
  Toggle_GPIO(11, Cooler_Pump_state);
  Toggle_GPIO(12, Tide_Pump_state);
  Toggle_GPIO(13, Circle_FAN_state);
  Toggle_GPIO(14, Cool_FAN_state);
  Toggle_GPIO(15, Release_WV_state);
}


void IRAM_ATTR Ext_INT1_ISR()
{
  Int_dispultada_1 = true;
}
