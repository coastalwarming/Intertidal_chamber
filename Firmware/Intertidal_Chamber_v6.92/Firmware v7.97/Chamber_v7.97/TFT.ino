
void Set_Start_End_Name_TFT() {
  xSemaphoreTake(mutex, portMAX_DELAY);

  tft.setTextColor(RED);
  tft.setTextSize(1);
  tft.setCursor(5, 91);
  tft.print(Version);
  //Serial.print("Version:"); Serial.println(Version);
  tft.setTextSize(2);
  tft.drawRoundRect(215, 93, 265, 20, 8, RED);//Retangulo do ip,start e data atual.

  tft.setTextColor(WHITE);

  drawCentreString(Device_name, 347 - (((strlen(Device_name) * 12) / 2) + 6), 95, strlen(Device_name), 2);

  tft.setTextColor(YELLOW);
  tft.setCursor(344, 95);
  tft.print("-");

  tft.setTextColor(WHITE);
  drawCentreString(Treatment, 341 + 12 + (((strlen(Treatment) * 12) / 2) + 6), 95, strlen(Treatment), 2);
  xSemaphoreGive(mutex);


  if (Profile_ok) {
    //Display datas de inicio e fim
    //Profile times:
    time_t start_ = profileTimes[0];
    //Serial.print("start_:"); Serial.println(start_);
    if (start_ != 0)snprintf (Start, 17, "S:%02d:%02d %02d/%02d/%d", hour(start_), minute(start_), day(start_), month(start_), year(start_) - 2000);
    //Serial.print("Start:"); Serial.println(Start);
    xSemaphoreTake(mutex, portMAX_DELAY);
    tft.setCursor(170, 3);
    tft.print(Start);
    xSemaphoreGive(mutex);
    time_t end_ = profileTimes[profileLength - 1];
    //Serial.print("end_:"); Serial.println(end_);



    if (end_ != 0)snprintf (End, 17, "E:%02d:%02d %02d/%02d/%d",  hour(end_), minute(end_), day(end_), month(end_), year(end_) - 2000);

    //Serial.print("End:"); Serial.println(End);
    xSemaphoreTake(mutex, portMAX_DELAY);

    tft.setCursor(170, 23);
    tft.print(End);
    xSemaphoreGive(mutex);
    //
  }
  else {
    xSemaphoreTake(mutex, portMAX_DELAY);

    tft.setCursor(170, 3);
    snprintf (Start, 17, "S:--:-- --/--/--");
    tft.print(Start);

    snprintf(End, 17, "E:--:-- --/--/--");
    tft.setCursor(170, 23);
    tft.print(End);
    xSemaphoreGive(mutex);
    Serial.println("runStatus = UnPositioned");
  }
}

void Write_status_tft() {

  static enum Status Prev_runStatus = UnPositioned;
  static char Titulo[30] = {}, Prev_Titulo[30] = {};
  static byte Local_Prev_Tide = 3;
  static byte Prev_Water_BOt_tot_detected  = 3;
  static byte Prev_Water_BOt_bot_detected  = 3;
  static byte Prev_Water_top_detected  = 3;
  static Switch Prev_Tide_sw_state = Unknown;
  static char Tide_text_1[30] = {};
  static char Tide_text_2[30] = {};
  static char Prev_Tide_text_1[30] = {};
  static char Prev_Tide_text_2[30] = {};
  static bool No_sd_card_drawn = false, No_experiment_drawn = false, No_RTC_drawn = false, sensor_halted_F = false;
  static uint16_t Color_Tide_text_1 = 0;
  static uint16_t Color_Tide_text_2 = 0;

  sensor_halted_F = false;

  if (runStatus == UnPositioned || runStatus == KILL_) {
    //UnPositioned= !sd_exist ou No experiment. ou Clock not adjusted.

    if (runStatus == KILL_) {
      snprintf (Tide_text_1, sizeof(Tide_text_1), "KILL is Active.");
      memset(Tide_text_2, 0, sizeof Tide_text_2);
      Color_Tide_text_1 = RED;
      Color_Tide_text_2 = BLACK;
    } else if (!sd_exist && !No_sd_card_drawn) {
      No_sd_card_drawn = true;
      No_experiment_drawn = false;
      No_RTC_drawn = false;
      //Serial.println("No SD card detected!");
      snprintf (Tide_text_1, sizeof(Tide_text_1), "No SD card detected!");
      memset(Tide_text_2, 0, sizeof Tide_text_2);
      Color_Tide_text_1 = RED;
      Color_Tide_text_2 = BLACK;
      //tft.print(Titulo);
      //
    } else if (sd_exist && !No_experiment_drawn && RTC_Pos_2022) {
      No_experiment_drawn = true;
      No_sd_card_drawn = false;
      No_RTC_drawn = false;
      //Serial.println("No experiment.");
      snprintf (Tide_text_1, sizeof(Tide_text_1), "No experiment.");
      memset(Tide_text_2, 0, sizeof Tide_text_2);

      Color_Tide_text_1 = RED;
      Color_Tide_text_2 = BLACK;
      // tft.print(Titulo);
      //
    } else if (sd_exist && !RTC_Pos_2022 && !No_RTC_drawn) {
      No_experiment_drawn = true;
      No_sd_card_drawn = false;
      No_RTC_drawn = true;

      snprintf (Tide_text_1, sizeof(Tide_text_1), "Clock not adjusted.");
      memset(Tide_text_2, 0, sizeof Tide_text_2);

      Color_Tide_text_1 = RED;
      Color_Tide_text_2 = BLACK;
      //tft.print(Titulo);
      //
    }
  } else if ((Main_SW_State == Manual) && ((Tide_sw_state == ON) || (Tide_sw_state == OFF))) {

    //Mare:
    if (Tide_sw_state == ON) {
      //Serial.printf("0Now_Tide:%d\n", Now_Tide);
      //Serial.printf("0Local_Prev_Tide:%d\n", Local_Prev_Tide);
      snprintf (Tide_text_1, sizeof(Tide_text_1), "High Tide");
      Color_Tide_text_1 = BLUE;
    }
    if (Tide_sw_state == OFF) {
      //Serial.printf("00Now_Tide:%d\n", Now_Tide);
      //Serial.printf("00Local_Prev_Tide:%d\n", Local_Prev_Tide);
      snprintf (Tide_text_1, sizeof(Tide_text_1), "Low Tide");
      Color_Tide_text_1 = RED;
    }
    //Serial.printf("000Tide_text_1:%s.\n", Tide_text_1);
    //Estado Halted:
    if ((Now_Tide && (Bottom_sensors && !Water_BOt_bot_detected)) ||
        (!Now_Tide && (Top_sensors && Water_top_detected)) ||
        (!Now_Tide && !Top_sensors_func) ||
        (Now_Tide && !Bot_Bot_sensors_func)) {
      snprintf (Tide_text_2, sizeof(Tide_text_2), " halted!");
      Color_Tide_text_2 = RED;
      //Serial.println("000Tide_text_2: sensor halt!");
    } else {
      snprintf (Tide_text_2, sizeof(Tide_text_2), " - Manual");
      Color_Tide_text_2 = ORANGE;
      // Serial.println("000Manual");
    }



  } else if (runStatus == Waiting_to_start) {
    // snprintf (Titulo, sizeof(Titulo), "Waiting to start.");
    snprintf (Tide_text_1, sizeof(Tide_text_1), "Waiting to start.");
    memset(Tide_text_2, 0, sizeof Tide_text_2);
    Color_Tide_text_1 = YELLOW;
  } else if (runStatus == Ongoing) {
    //Se a experiencia esta a decorrer, e não ha modo kill, NoSd, Nortc, nem mares manuais:
    //Mare:
    if (Now_Tide == 1) {
      //Serial.printf("1Now_Tide:%d\n", Now_Tide);
      //Serial.printf("1Local_Prev_Tide:%d\n", Local_Prev_Tide);
      snprintf (Tide_text_1, sizeof(Tide_text_1), "High Tide");
      Color_Tide_text_1 = BLUE;

    } else {
      //Serial.printf("11Now_Tide:%d\n", Now_Tide);
      //Serial.printf("11Local_Prev_Tide:%d\n", Local_Prev_Tide);
      snprintf (Tide_text_1, sizeof(Tide_text_1), "Low Tide");
      Color_Tide_text_1 = RED;
    }
    //
    //Horas final ou estado Halted:
    //Estado Halted:
    if ((Now_Tide && (Bottom_sensors && !Water_BOt_bot_detected)) ||
        (!Now_Tide && (Top_sensors && Water_top_detected)) ||
        (!Now_Tide && !Top_sensors_func) ||
        (Now_Tide && !Bot_Bot_sensors_func)) {
      snprintf (Tide_text_2, sizeof(Tide_text_2), " halted!");
      Color_Tide_text_2 = RED;
      //Serial.println("000Tide_text_2: sensor halt!");
    } else {
      //Nao ha modo halted:
      if (!No_more_tide_change) {
        snprintf (Tide_text_2, sizeof(Tide_text_2), " until %02d:%02d", hour(Next_tide_change_moment), minute(Next_tide_change_moment));
        Color_Tide_text_2 = WHITE;
        //Serial.println("until XX:XX");
      }
      else if (No_more_tide_change) {
        snprintf (Tide_text_2, sizeof(Tide_text_2), " until end.");
        Color_Tide_text_2 = WHITE;
        //Serial.println("until end ");
      }
    }

    strcpy(Titulo, Tide_text_1 );
    strcat(Titulo, Tide_text_2);
    //
  } else if (runStatus == Finished) {
    // snprintf (Titulo, sizeof(Titulo), "Experiment finished.");
    snprintf (Tide_text_1, sizeof(Tide_text_1), "Experiment finished.");
    memset(Tide_text_2, 0, sizeof Tide_text_2);
    Color_Tide_text_1 = RED;
    //Serial.println("Experiment finished.");
  }

  //Agora que decidi o titulo, escrevo:
  //if (strcmp(Prev_Titulo, Titulo) != 0) {
  if ((strcmp(Prev_Tide_text_1, Tide_text_1) != 0) || (strcmp(Prev_Tide_text_2, Tide_text_2) != 0)) {
    xSemaphoreTake(mutex, portMAX_DELAY);

    tft.setTextSize(2);
    tft.setCursor(225, 117);
    tft.setTextColor(BLACK);
    tft.print(Prev_Tide_text_1);
    tft.print(Prev_Tide_text_2);

    tft.setCursor(225, 117);
    tft.setTextColor(Color_Tide_text_1);
    tft.print(Tide_text_1);
    tft.setTextColor(Color_Tide_text_2);
    tft.print(Tide_text_2);

    strcpy(Prev_Tide_text_1, Tide_text_1 );
    strcpy(Prev_Tide_text_2, Tide_text_2 );


    xSemaphoreGive(mutex);
  }
  //
}

void Update_Time_Display() {
  xSemaphoreTake(mutex, portMAX_DELAY);

  //Apagar Hora:
  tft.setTextColor(BLACK);
  tft.setTextSize(2);
  tft.setCursor(390, 23);
  static char Hora[6] = {};
  tft.print(Hora);

  //Escolher Cor:
  if (NTP_Time_adjusted)tft.setTextColor(GREEN); else tft.setTextColor(WHITE);

  //Escrever Hora:
  tft.setCursor(390, 23);
  snprintf ( Hora, sizeof(Hora), "%02d:%02d", hour(nowTimeUnix), minute(nowTimeUnix));
  tft.print(Hora);
  /*
    Serial.println("Em Update_Time_Display:");
    Serial.printf("nowTimeUnix:%i.\n", nowTimeUnix);
    Serial.printf("Horas:%i:%i.\n", hour(nowTimeUnix), minute(nowTimeUnix));
  */
  minuto_mudou = false;

  //Apagar data:
  if (dia_mudou) {
    tft.setTextSize(2);
    tft.setTextColor(BLACK);
    tft.setCursor(375, 5);
    static char Data[10] = {};
    tft.print(Data);

    //Escolher cor:
    if (NTP_Time_adjusted)tft.setTextColor(GREEN); else tft.setTextColor(WHITE);

    //Escrever Data:
    tft.setCursor(375, 5);
    snprintf ( Data, sizeof(Data), "%02d/%02d/%2d", day(nowTimeUnix), month(nowTimeUnix), year(nowTimeUnix) - 2000);
    tft.print(Data);
    dia_mudou = false;
  }
  xSemaphoreGive(mutex);
}

void tft_init() {
  xSemaphoreTake(mutex, portMAX_DELAY);
  //18/12/2024
  tft.begin(1000000);//4000000 (dia 29/12/2024 era 2700000 .
  //Dia 06/01/2025 passei de 100KHz para 1MHz (100000 para 1000000 ).
  //
  tft.fillScreen(BLACK);
  tft.setRotation(3);
  xSemaphoreGive(mutex);
}

void tft_main_frame() {
  xSemaphoreTake(mutex, portMAX_DELAY);

  //Retangulos
  //void drawRoundRect(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, uint16_t radius, uint16_t color);
  tft.drawRoundRect(0, 0, 480, 40, 8, RED);//Retangulo do ip,start e data atual.
  tft.drawRoundRect(0, 100, 175, 100, 8, RED);//Retangulo do ar
  tft.drawRoundRect(0, 200, 175, 80, 8, RED);//Retangulo da agua
  tft.drawRoundRect(0, 280, 175, 40, 8, RED); //Retangulo da RT e LED
  /*
    tft.setTextSize(2);
    tft.setTextColor(YELLOW);
    drawCentreString("Wlvl", 191, 140, 4, 2);
  */
  tft.drawRect(176, 156, 30, 164, LIGHTGREY);//Retangulo nivel de agua
  tft.drawRect(177, 157, 28, 162, LIGHTGREY);//Retangulo nivel de agua

  tft.fillRect(176, 196, 30, 5, LIGHTGREY);//Juncao dos tanques

  //Secao Top:
  tft.fillRect(178, 159, 8, 36, RED);//104
  tft.fillRect(178 + 9, 159, 8, 36, RED); //
  tft.fillRect(178 + 9 + 9, 159, 8, 36, RED); //


  //Secao Bot_Top:
  tft.fillRect(178, 202, 26, 36, RED);//

  //Secao Bot_Mid:
  tft.drawLine(178, 240, 178 + 5, 240, LIGHTGREY);
  tft.drawLine(178 + 7, 240, 178 + 7 + 5, 240, LIGHTGREY);
  tft.drawLine(178 + 7 + 7, 240, 178 + 7 + 7 + 5, 240, LIGHTGREY);
  tft.drawLine(178 + 7 + 7 + 7, 240, 178 + 7 + 7 + 7 + 5, 240, LIGHTGREY);

  tft.fillRect(178, 242, 26, 36, RED);//

  //Secao Bot_Bot:
  tft.drawLine(178, 280, 178 + 5, 280, LIGHTGREY);
  tft.drawLine(178 + 7, 280, 178 + 7 + 5, 280, LIGHTGREY);
  tft.drawLine(178 + 7 + 7, 280, 178 + 7 + 7 + 5, 280, LIGHTGREY);
  tft.drawLine(178 + 7 + 7 + 7, 280, 178 + 7 + 7 + 7 + 5, 280, LIGHTGREY);

  tft.fillRect(178, 282, 8, 35, RED);//
  tft.fillRect(178 + 9, 282, 8, 35, RED); //
  tft.fillRect(178 + 9 + 9, 282, 8, 35, RED); //


  //Linhas horizontais:
  //( x0,  y0,  x1,  y1,  color);

  //tft.drawLine(0, 120, 199, 120, RED);
  tft.drawLine(0, 120, 174, 120, RED);
  tft.drawLine(0, 180, 74, 180, RED);

  //tft.drawLine(100, 160, 199, 160, RED);
  // tft.drawLine(100, 240, 199, 240, RED);
  tft.drawLine(85, 160, 174, 160, RED);
  tft.drawLine(0, 220, 174, 220, RED);

  tft.drawLine(85, 240, 174, 240, RED);
  tft.drawLine(0, 260, 85, 260, RED);
  tft.drawLine(0, 300, 174, 300, RED);



  //Linhas vertcais:
  //( x0,  y0,  x1,  y1,  color);
  tft.drawLine(110, 0, 110, 39, RED);
  tft.drawLine(165, 0, 165, 39, RED);
  tft.drawLine(365, 0, 365, 39, RED);
  tft.drawLine(85, 120, 85, 200, RED);
  tft.drawLine(85, 220, 85, 280, RED);
  tft.drawLine(85, 280, 85, 320, RED);
  /*
    tft.drawLine(100, 120, 100, 200, RED);
    tft.drawLine(100, 220, 100, 280, RED);
    tft.drawLine(100, 280, 100, 320, RED);
  */
  //tft.drawLine(50, 300, 50, 320, RED);

  //Texto sinalizador
  tft.setTextSize(2);
  tft.setTextColor(DARKGREY); tft.setCursor(115, 3); tft.print("W");
  tft.setTextColor(DARKGREY); tft.setCursor(145, 3); tft.print("B");
  tft.setTextColor(DARKGREY); tft.setCursor(115, 23); tft.print("M");
  tft.setTextColor(DARKGREY); tft.setCursor(140, 23); tft.print("SD");
  // drawRoundRect( x0,  y0,  w,  h,  radius,  color);
  //Outline dos Botoes
  tft.drawRoundRect(0, 43, 50, 45, 8, WHITE);  //Manual

  tft.drawRoundRect(51, 43, 60, 45, 8, DARKGREY);  //A-Heat

  tft.drawRoundRect(112, 43, 50, 45, 8, DARKGREY);  //Cool

  tft.drawRoundRect(163, 43, 50, 45, 8, DARKGREY);  //Pump

  tft.drawRoundRect(214, 43, 62, 45, 8, DARKGREY);  //W-Heat

  tft.drawRoundRect(277, 43, 50, 45, 8, DARKGREY);  //Tide

  tft.drawRoundRect(328, 43, 50, 45, 8, DARKGREY);  //WV

  tft.drawRoundRect(379, 43, 50, 45, 8, DARKGREY);  //Light

  tft.drawRoundRect(430, 43, 50, 45, 8, DARKGREY);//Fan
  /*
    tft.drawRoundRect(0, 43, 69, 45, 8, WHITE);  //Manual

    tft.drawRoundRect(70, 43, 68, 45, 8, DARKGREY);  //A-Heat

    tft.drawRoundRect(139, 43, 51, 45, 8, DARKGREY);  //Cool

    tft.drawRoundRect(191, 43, 52, 45, 8, DARKGREY);  //Pump

    tft.drawRoundRect(244, 43, 70, 45, 8, DARKGREY);  //W-Heat

    tft.drawRoundRect(315, 43, 51, 45, 8, DARKGREY);  //Tide

    tft.drawRoundRect(367, 43, 62, 45, 8, DARKGREY);  //Light

    tft.drawRoundRect(430, 43, 50, 45, 8, DARKGREY);//Fan
  */
  xSemaphoreGive(mutex);
}

void tft_initial_tags() {
  xSemaphoreTake(mutex, portMAX_DELAY);

  //Texto
  tft.setTextSize(2);

  tft.setTextColor(YELLOW);
  drawCentreString("Top Tank", 85, 104, 8, 2);
  //tft.setCursor(50, 104);
  //tft.print("Air Temp");

  tft.setTextColor(WHITE);
  tft.setCursor(10, 124);
  tft.print("    "); tft.print((char)9); tft.print("C");

  // tft.setCursor(115, 124);
  tft.setTextColor(YELLOW);
  drawCentreString("IR-Heat", 130, 124, 7, 2);
  // tft.print("PID");

  //tft.setCursor(130, 144);
  //tft.setTextColor(WHITE);
  //tft.print(PID_Output_1, 0); tft.print("%");

  //Serial.print("PID_Output_1:"); Serial.println(PID_Output_1, 0);

  tft.setCursor(95, 164);
  tft.setTextColor(YELLOW);
  tft.print("Target");
  tft.setCursor(95, 184);
  tft.setTextColor(WHITE);
  tft.print("----"); tft.print((char)9); tft.print("C");

  tft.setCursor(10, 144);
  tft.print("    "); tft.print((char)9); tft.print("C");
  tft.setCursor(10, 164);
  tft.print("    "); tft.print((char)9); tft.print("C");

  tft.setTextColor(WHITE);
  tft.setCursor(10, 184);
  tft.print("    "); tft.print((char)9); tft.print("C");

  tft.setTextColor(YELLOW);
  drawCentreString("Base Tank", 85, 204, 9, 2);
  //tft.setCursor(50, 204);
  //tft.print("Water Temp");

  tft.setTextColor(WHITE);
  tft.setCursor(10, 224);
  tft.print("    "); tft.print((char)9); tft.print("C");
  tft.setCursor(10, 244);
  tft.print("    "); tft.print((char)9); tft.print("C");

  //tft.setCursor(110, 224);
  if (Water_temp_state == Idle) drawCentreString("Idle", 130, 224, 4, 2);//tft.print("Idle");//tft.print("Idle");

  tft.setCursor(95, 244);
  tft.setTextColor(YELLOW);
  tft.print("Target");//target na agua.
  tft.setCursor(95, 264);
  tft.setTextColor(WHITE);
  tft.print("----"); tft.print((char)9); tft.print("C");

  tft.setTextColor(WHITE);
  tft.setCursor(10, 264);
  tft.print("    "); tft.print((char)9); tft.print("C");
  /*
    tft.setTextColor(YELLOW);
    tft.setCursor(4, 284); tft.print("Water");
    tft.setCursor(65, 284);
    tft.print("lvl");
  */
  /*
    tft.setCursor(22, 302);
    tft.setTextColor(WHITE);
    tft.print("-");
  */
  /*
    tft.setCursor(72, 302);
    tft.setTextColor(WHITE);
    tft.print("-");
  */

  //RT
  //tft.setCursor(105, 284);
  tft.setCursor(10, 284);
  tft.setTextColor(YELLOW);
  tft.print("RoomT");
  //tft.setCursor(120, 304);
  tft.setCursor(10, 304);
  tft.setTextColor(WHITE);
  tft.print("    "); tft.print((char)9); tft.print("C");

  //LED
  tft.setTextColor(YELLOW);
  drawCentreString("LED", 130, 284, 3, 2);



  /*
    tft.setTextColor(WHITE);
    tft.setCursor(14, 302);
    tft.print("S1:");
    tft.setTextColor(BLUE);
    tft.print("High");
    tft.setTextColor(WHITE);

    tft.setCursor(114, 302);
    tft.print("S2:");
    tft.setTextColor(GREENYELLOW);
    tft.print("Low");
  */


  //Wifi
  //Simbolof
  tft.setTextColor(WHITE);
  drawCentreString("   ---   ", 56, 5, 9, 2);
  // tft.setTextSize(2);tft.setCursor(4, 5); tft.print("   ---   ");
  //tft.setTextSize(1); tft.setCursor(4, 26);  tft.print("192.168.132.201");

  //tft.drawBitmap(105, 4, Line2, 45, 45, 0xffff);//wifi
  /*
    tft.setTextSize(2);
    tft.setCursor(170, 3);
    tft.print("S:15:30 20/06/22");
    tft.setCursor(170, 23);
    tft.print("E:15:30 21/06/22");
  */
  /*
    //Horas
    tft.setTextColor(YELLOW);
    tft.setTextSize(2);
    tft.setCursor(375, 5);
    tft.print("01/01/22");
    tft.setCursor(390, 23);
    tft.print("16:10");
  */
  xSemaphoreGive(mutex);
}

void drawCentreString(const String &buf, int x, int y, int caracteres, int letter_size) {
  int16_t x1, y1;
  uint16_t w, h;
  tft.getTextBounds(buf, x, y, &x1, &y1, &w, &h); //calc width of new string


  if (caracteres > 0)w = (letter_size * 6) * caracteres; //lettersize=3 da caracteres de 18 largura.
  /*
    Serial.print("x:");  Serial.println(x);
    Serial.print("w:");  Serial.println(w);
    Serial.print("x - w / 2:");  Serial.println(x - w / 2);
  */
  tft.setCursor(x - w / 2, y);
  tft.print(buf);
}

void Update_Temp_Sensors_display() {
  xSemaphoreTake(mutex, portMAX_DELAY);

  static int Prev_Now_Light = -1;
  static char LED_text[20] = {};
  static char Prev_LED_text[20] = {};

  if (Prev_Now_Light != Now_Light) {
    //Serial.print("Update_Temp_Sensors_display:"); Serial.println(Now_Light);
    Prev_Now_Light = Now_Light;

    tft.setTextColor(BLACK);
    drawCentreString(Prev_LED_text, 130, 304, strlen(Prev_LED_text), 2);

    snprintf(LED_text, sizeof(LED_text), "%d%%", Now_Light);
    tft.setTextColor(WHITE);
    drawCentreString(LED_text, 130, 304, strlen(LED_text), 2);

    memcpy(Prev_LED_text, LED_text, sizeof(Prev_LED_text));

  }

  /*Sensores*/
  tft.setTextSize(2);
  tft.setTextColor(WHITE);
  static float Prev_Air_Temp_1 = -1;
  static float Prev_Air_Temp_2 = -1;
  static float Prev_Air_Temp_3 = -1;
  static float Prev_Room_Temp = -1;

  static float Prev_Air_Input_Temp_Avg = -1;
  static float Prev_PID_Output_1 = 0.1;
  static float Prev_Air_Target_Temp = 0.0;

  static float Prev_Water_Temp_1 = -1;
  static float Prev_Water_Temp_2 = -1;
  static float Prev_Water_Input_Temp_Avg = -1;
  static float Prev_Water_Temp_Setpoint = 0.0;

  static byte Prev_Water_Sensor_1 = 0;
  static byte Prev_Water_Sensor_2 = 0;

  // static byte Prev_WS_1 = 2, Prev_WS_2 = 2, Prev_WS_3 = 2, Prev_WS_4 = 2;
  static byte Prev_WS_1 = 2, Prev_WS_2 = 2, Prev_WS_3 = 2
                                         ;
  static char Prev_PID_text[20] = {};
  static char PID_text[20] = {};
  static char PID_text_1[5] = {};
  static char PID_text_2[7] = {};
  static char lol[36] = {};
  static bool Prev_Wait_low_tide = 0;
  static bool Prev_Air_Temp_1_ok = false, Prev_Air_Temp_2_ok = false, Prev_Air_Temp_3_ok = false, Prev_Room_Temp_ok = false, Prev_Air_Temp_sensors_functional = false,
              Prev_Water_Temp_1_ok = false, Prev_Water_Temp_2_ok = false, Prev_Water_Temp_sensors_functional = false;

  //Air Temp sensors:
  if (Air_Temp_1 != Prev_Air_Temp_1) {
    if (Air_Temp_1_ok) {

      tft.setCursor(10, 124);
      tft.setTextColor(BLACK);
      if (Prev_Air_Temp_1_ok)tft.print(Prev_Air_Temp_1, 1); else tft.print("----");
      tft.setCursor(10, 124);
      tft.setTextColor(WHITE);
      tft.print(Air_Temp_1, 1);
      Prev_Air_Temp_1 = Air_Temp_1;
      Prev_Air_Temp_1_ok = true;
    } else {
      Prev_Air_Temp_1_ok = false;
      tft.setCursor(10, 124);
      tft.setTextColor(BLACK);
      tft.print(Prev_Air_Temp_1, 1);
      tft.setCursor(10, 124);
      tft.setTextColor(RED);
      tft.print("----");
    }
  }


  if (Air_Temp_2 != Prev_Air_Temp_2) {
    if (Air_Temp_2_ok) {

      tft.setCursor(10, 144);
      tft.setTextColor(BLACK);
      if (Prev_Air_Temp_2_ok)tft.print(Prev_Air_Temp_2, 1); else tft.print("----");
      tft.setCursor(10, 144);
      tft.setTextColor(WHITE);
      tft.print(Air_Temp_2, 1);
      Prev_Air_Temp_2 = Air_Temp_2;
      Prev_Air_Temp_2_ok = true;
    } else {
      Prev_Air_Temp_2_ok = false;
      tft.setCursor(10, 144);
      tft.setTextColor(BLACK);
      tft.print(Prev_Air_Temp_2, 1);
      tft.setCursor(10, 144);
      tft.setTextColor(RED);
      tft.print("----");
    }
  }

  if (Air_Temp_3 != Prev_Air_Temp_3) {
    if (Air_Temp_3_ok) {

      tft.setCursor(10, 164);
      tft.setTextColor(BLACK);
      if (Prev_Air_Temp_3_ok)tft.print(Prev_Air_Temp_3, 1); else tft.print("----");
      tft.setCursor(10, 164);
      tft.setTextColor(WHITE);
      tft.print(Air_Temp_3, 1);
      Prev_Air_Temp_3 = Air_Temp_3;
      Prev_Air_Temp_3_ok = true;
    } else {
      Prev_Air_Temp_3_ok = false;
      tft.setCursor(10, 164);
      tft.setTextColor(BLACK);
      tft.print(Prev_Air_Temp_3, 1);
      tft.setCursor(10, 164);
      tft.setTextColor(RED);
      tft.print("----");
    }
  }

  if (Room_Temp != Prev_Room_Temp) {
    if (Room_Temp_ok) {
      // tft.setCursor(120, 304);
      tft.setCursor(10, 304);
      tft.setTextColor(BLACK);
      if (Prev_Room_Temp_ok)tft.print(Prev_Room_Temp, 1); else tft.print("----");
      //tft.setCursor(120, 304);
      tft.setCursor(10, 304);

      tft.setTextColor(WHITE);
      tft.print(Room_Temp, 1);
      Prev_Room_Temp = Room_Temp;
      Prev_Room_Temp_ok = true;
    } else {
      Prev_Room_Temp_ok = false;
      //tft.setCursor(120, 304);
      tft.setCursor(10, 304);

      tft.setTextColor(BLACK);
      tft.print(Prev_Room_Temp, 1);
      //tft.setCursor(120, 304);
      tft.setCursor(10, 304);

      tft.setTextColor(RED);
      tft.print("----");
    }
  }

  if (Air_Input_Temp_Avg != Prev_Air_Input_Temp_Avg) {
    if (Air_Temp_sensors_functional) {
      tft.setCursor(10, 184);
      tft.setTextColor(BLACK);
      if (Prev_Air_Temp_sensors_functional)tft.print(Prev_Air_Input_Temp_Avg, 1); else tft.print("----");
      tft.setCursor(10, 184);
      tft.setTextColor(ORANGE);
      tft.print(Air_Input_Temp_Avg, 1);
      Prev_Air_Input_Temp_Avg = Air_Input_Temp_Avg;
      Prev_Air_Temp_sensors_functional = true;
    } else {
      Prev_Air_Temp_sensors_functional = false;
      tft.setCursor(10, 184);
      tft.setTextColor(BLACK);
      tft.print(Prev_Air_Input_Temp_Avg, 1);
      tft.setCursor(10, 184);
      tft.setTextColor(RED);
      tft.print("----");
    }
  }



  if ((PID_Output_1 != Prev_PID_Output_1) || Wait_low_tide || strlen(PID_text) > 4) {
    Prev_Wait_low_tide = Wait_low_tide;
    //drawCentreString(const String &buf, int x, int y, int caracteres, int letter_size)
    //Serial.print("PID_Output_1:"); Serial.println(PID_Output_1);
    //Serial.print("Prev_PID_Output_1:"); Serial.println(Prev_PID_Output_1);
    // Serial.print("Wait_low_tide:"); Serial.println(Wait_low_tide);

    tft.setTextColor(BLACK);
    drawCentreString(Prev_PID_text, 130, 144, strlen(Prev_PID_text), 2);
    //  Serial.print("Prev_PID_text:"); Serial.println(Prev_PID_text);
    //  Serial.print("sizeof(Prev_PID_text):"); Serial.println(sizeof(Prev_PID_text));
    // Serial.print("strlen(Prev_PID_text):"); Serial.println(strlen(Prev_PID_text));

    byte tempo = 0;

    if ((Wait_low_tide && !Now_Tide) || (Prev_Wait_low_tide != Wait_low_tide)) {
      tempo = (int)(Wait_before_air_heat - (nowTimeUnix - No_water_instant));
      //Desliga ventoinha de frio caso tenha ficado ligada durante os ciclos de descondensação:
      if (Cool_FAN_state == ON) {
        Cool_FAN_state = Off;
        Toggle_GPIO(14, Cool_FAN_state);
      }
      //
      snprintf(PID_text, sizeof(PID_text), "%d%%-%is", (int)PID_Output_1, tempo);
      /*
        Serial.print("PID_text:"); Serial.println(PID_text);
        Serial.print("Prev_Wait_low_tide:"); Serial.println(Prev_Wait_low_tide);
        Serial.print("Wait_low_tide:"); Serial.println(Wait_low_tide);
        Serial.print("Now_Tide:"); Serial.println(Now_Tide);
        Serial.print("strlen(PID_text):"); Serial.println(strlen(PID_text));
      */
      tft.setTextColor(WHITE);
      drawCentreString(PID_text, 130, 144, strlen(PID_text), 2);

    } else {
      snprintf(PID_text, sizeof(PID_text), "%d%%", (int)PID_Output_1);
      tft.setTextColor(WHITE);
      drawCentreString(PID_text, 130, 144, strlen(PID_text), 2);
    }
    //Prev_PID_text = PID_text;
    //strncpy(Prev_PID_text, PID_text, 18);


    //  Serial.print("0Prev_PID_text:"); Serial.println(Prev_PID_text);
    //  Serial.print("0PID_text:"); Serial.println(PID_text);
    memcpy(Prev_PID_text, PID_text, sizeof(Prev_PID_text));
    //  Serial.print("1Prev_PID_text:"); Serial.println(Prev_PID_text);
    //  Serial.print("1PID_text:"); Serial.println(PID_text);
    //  Serial.println();

    Prev_PID_Output_1 = PID_Output_1;
  }


  static bool Fst_Water_target = true, Fst_Air_target = true;

  if (Temp_Setpoint != Prev_Air_Target_Temp) {
    //Serial.print("0Temp_Setpoint:"); Serial.println(Temp_Setpoint);

    if (Temp_Setpoint > -126 && Temp_Setpoint < 84 && Air_Temp_sensors_functional) {
      tft.setCursor(95, 184);
      tft.setTextColor(BLACK);
      if (Fst_Air_target) {
        tft.print("----");
        Fst_Air_target = false;
      } else tft.print(Prev_Air_Target_Temp, 1);

      tft.setCursor(95, 184);
      if (runStatus != Finished) {
        tft.setTextColor(ORANGE);
        tft.print(Temp_Setpoint, 1);
      } else {
        tft.setTextColor(WHITE);
        tft.print("----");
      }
      Prev_Air_Target_Temp = Temp_Setpoint;
    } else {
      tft.setCursor(95, 184);
      tft.setTextColor(BLACK);
      tft.print(Prev_Air_Target_Temp, 1);

      tft.setCursor(95, 184);
      tft.setTextColor(WHITE);
      tft.print("----");
    }

  }



  //Water Temp sensors:
  if (Water_Temp_1 != Prev_Water_Temp_1) {
    if (Water_Temp_1_ok) {
      tft.setCursor(10, 224);
      tft.setTextColor(BLACK);
      if (Prev_Water_Temp_1_ok)tft.print(Prev_Water_Temp_1, 1); else tft.print("----");
      tft.setCursor(10, 224);
      tft.setTextColor(WHITE);
      tft.print(Water_Temp_1, 1);
      Prev_Water_Temp_1 = Water_Temp_1;
      Prev_Water_Temp_1_ok = true;
    } else {
      Prev_Water_Temp_1_ok = false;
      tft.setCursor(10, 224);
      tft.setTextColor(BLACK);
      tft.print(Prev_Water_Temp_1, 1);
      tft.setCursor(10, 224);
      tft.setTextColor(RED);
      tft.print("----");
    }
  }

  if (Water_Temp_2 != Prev_Water_Temp_2) {
    if (Water_Temp_2_ok) {
      tft.setCursor(10, 244);
      tft.setTextColor(BLACK);
      if (Prev_Water_Temp_2_ok)tft.print(Prev_Water_Temp_2, 1); else tft.print("----");
      tft.setCursor(10, 244);
      tft.setTextColor(WHITE);
      tft.print(Water_Temp_2, 1);
      Prev_Water_Temp_2 = Water_Temp_2;
      Prev_Water_Temp_2_ok = true;
    } else {
      Prev_Water_Temp_2_ok = false;
      tft.setCursor(10, 244);
      tft.setTextColor(BLACK);
      tft.print(Prev_Water_Temp_2, 1);
      tft.setCursor(10, 244);
      tft.setTextColor(RED);
      tft.print("----");
    }
  }

  if (Water_Input_Temp_Avg != Prev_Water_Input_Temp_Avg) {
    if (Water_Temp_sensors_functional) {
      tft.setCursor(10, 264);
      tft.setTextColor(BLACK);
      if (Prev_Water_Temp_sensors_functional)tft.print(Prev_Water_Input_Temp_Avg, 1); else tft.print("----");
      tft.setCursor(10, 264);
      tft.setTextColor(ORANGE);
      tft.print(Water_Input_Temp_Avg, 1);
      Prev_Water_Input_Temp_Avg = Water_Input_Temp_Avg;
      Prev_Water_Temp_sensors_functional = true;
    } else {
      Prev_Water_Temp_sensors_functional = false;
      tft.setCursor(10, 264);
      tft.setTextColor(BLACK);
      tft.print(Prev_Water_Input_Temp_Avg, 1);
      tft.setCursor(10, 264);
      tft.setTextColor(RED);
      tft.print("----");
    }
  }


  if (Water_Temp_Setpoint != Prev_Water_Temp_Setpoint) {
    //Serial.print("Water_Temp_Setpoint:"); Serial.println(Water_Temp_Setpoint);

    if (Water_Temp_Setpoint > -126 && Water_Temp_Setpoint < 84 && Water_Temp_sensors_functional) {
      tft.setCursor(95, 264);
      tft.setTextColor(BLACK);

      if (Fst_Water_target || Prev_Water_Temp_Setpoint == 0) {
        tft.print("----");
        Fst_Water_target = false;
      } else {
        tft.print(Prev_Water_Temp_Setpoint, 1);
      }

      tft.setCursor(95, 264);
      if (runStatus != Finished && (!No_more_tide_change
                                    || (No_more_tide_change && Now_Tide && !(Main_SW_State  == Manual && Tide_sw_state == ON)))) {
        //Adicionei && !No_more_tide_change para apagar o watersetpoint quando a experiencia acaba em mare vazia.
        //Adicionei && !(Main_SW_State  == Manual && Tide_sw_state == ON) Para ao meter mare alta manual nao aquecer a agua
        //so pk a mare baixa antes de forcar alta manual estava a aquecer.
        tft.setTextColor(ORANGE);
        if (Water_Temp_Setpoint == 0)tft.print("----"); else tft.print(Water_Temp_Setpoint, 1);

      } else {
        Fst_Water_target = true;
        tft.setTextColor(WHITE);
        tft.print("----");
      }

      Prev_Water_Temp_Setpoint = Water_Temp_Setpoint;

    } else {
      tft.setCursor(95, 264);
      tft.setTextColor(BLACK);
      if (Prev_Water_Temp_Setpoint == 0)tft.print("----"); else tft.print(Prev_Water_Temp_Setpoint, 1);

      tft.setCursor(95, 264);
      tft.setTextColor(WHITE);
      tft.print("----");
    }

  }




  /*
    tft.setCursor(40, 302);
    tft.setTextColor(BLUE);
    tft.print("H-H");

    tft.setCursor(140, 302);
    tft.setTextColor(GREENYELLOW);
    tft.print("H-L");
  */
  //
  xSemaphoreGive(mutex);
}
