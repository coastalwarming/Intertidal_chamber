

void Rodar_e_Calculate_next_Future_Setpoint() {
  //Serial.println("Rodar_e_Calculate_next_Future_Setpoint");
  

  bool Projected_Tide_Setpoint = 0, Projected_TPA_Setpoint = 0;
  if (runStatus == Ongoing || runStatus == Waiting_to_start) {
    //Serial.print("runStatus"); Serial.println(runStatus);

    //Sera que impede falso setpoint de plotar quando esta em finished?
    //Calcular o proximo valor de setpoint a plotar:
    long unsigned int i = nowTimeUnix + 10800 - 90;

    if (i > profileTimes[Projected_target_Step_Index])Projected_target_Step_Index++;

    Proj_nowTimes[0] = profileTimes[Projected_target_Step_Index - 1];
    Proj_nowTimes[1] = profileTimes[Projected_target_Step_Index];
    Proj_nowTemps[0] = (profileTTL[Projected_target_Step_Index - 1] / 10000) / 10.0;
    Proj_nowTemps[1] = (profileTTL[Projected_target_Step_Index]   / 10000) / 10.0;

    Projected_Tide_Setpoint = (profileTTL[Projected_target_Step_Index - 1] / 1000)  % 10;
    Projected_TPA_Setpoint = (int)((profileTPA[Projected_target_Step_Index - 1]));

    if (Prev_Plot_TPA_Projected_target_Step_Index  != (Projected_target_Step_Index - 1)) {
     // Serial.println("1cnt reset");
     // Serial.print("Projected_target_Step_Index-1:");  Serial.println(Projected_target_Step_Index - 1);
      Prev_Plot_TPA_Projected_target_Step_Index  = Projected_target_Step_Index - 1;
      cnt_plot  = 0;
    }
    if (Projected_TPA_Setpoint == 1 && cnt_plot  < 4) {
    //  Serial.println("1cnt++");
      cnt_plot ++;
    }

    if ((Projected_TPA_Setpoint == 1) && cnt_plot  > 3) {
      //Serial.println("1Projected_TPA_Setpoint=0!!");
      Projected_TPA_Setpoint = 0;
    } //else Serial.println("1Projected_TPA_Setpoint=1!!");


    m = ((Proj_nowTemps[1] - Proj_nowTemps[0]) / (Proj_nowTimes[1] - Proj_nowTimes[0])); // m = (y2 - y1) / (x2 - x1)
    b = Proj_nowTemps[0]; // b = y1 - (m * x1)
    if (!isnan(m))Projected_Temp_Setpoint = (m * float(i - Proj_nowTimes[0] + 90)) + b; // Y = mX + b
    else {
      Projected_Temp_Setpoint = 0;
      //Serial.println("m is nan!");
    }


    //Serial.printf("Projected_Temp_Setpoint added on right of moving of plot:%f\n", Projected_Temp_Setpoint);

    char Debug[100];
    // if (((nowTemps[1] > nowTemps[0]) && (Projected_Temp_Setpoint > nowTemps[1])) ||
    //    ((nowTemps[1] < nowTemps[0]) && (Projected_Temp_Setpoint < nowTemps[1])))Projected_Temp_Setpoint = nowTemps[1];
    //Impede setpoint acima do objetivo (quero 15ºC mas aponta para 15.13ºC).

    if ((Proj_nowTemps[1] >= Proj_nowTemps[0]) && (Projected_Temp_Setpoint > Proj_nowTemps[1]) ) {

      Projected_Temp_Setpoint = Proj_nowTemps[1];
    }

    if ((Proj_nowTemps[1] <= Proj_nowTemps[0]) && (Projected_Temp_Setpoint < Proj_nowTemps[1])) {

      Projected_Temp_Setpoint = Proj_nowTemps[1];
    }
    /*
        Serial.printf("m:%f\n", m);
        Serial.printf("b:%f\n", b);
        Serial.printf("nowTemps[1]:%f\n", nowTemps[1]);
        Serial.printf("nowTemps[0]:%f\n", nowTemps[0]);
        Serial.printf("nowTimes[1]:%li\n", nowTimes[1]);
        Serial.printf("nowTimes[0]:%li\n", nowTimes[0]);
        Serial.printf("1Projected_Temp_Setpoint:%f\n", Projected_Temp_Setpoint);
    */


  } else {
    Projected_Temp_Setpoint = 0;
    //Serial.printf("2Projected_Temp_Setpoint:%f\n", Projected_Temp_Setpoint);
  }

  //Rodar vetor:
  for (int i = 0; i < 240; i++) {
    Prev_Temp_SetPoint[i] = Temp_SetPoints[i][0];
    if (i < 239) {
      Temp_SetPoints[i][0] = Temp_SetPoints[i + 1][0];
      Temp_SetPoints[i][1] = Temp_SetPoints[i + 1][1];
      Temp_SetPoints[i][2] = Temp_SetPoints[i + 1][2];
    }
    if (i == 239) {
      Temp_SetPoints[i][0] = Projected_Temp_Setpoint;
      Temp_SetPoints[i][1] = Projected_Tide_Setpoint ;
      Temp_SetPoints[i][2] = Projected_TPA_Setpoint ;
    }
    //Serial.print("Temp_SetPoints[i][0]:"); Serial.println(Temp_SetPoints[i][0]);
    //Serial.print("Prev_Temp_SetPoint[i]:"); Serial.println(Prev_Temp_SetPoint[i]);
  }
}

void Plotar_Temp(double dados[120], unsigned int cor, bool Linhas_intermedias_no_inicio, int limite) {
  //Serial.println("Chamar Graph em Plotar_");
  bool TPA = false;
  for (x = 0; x < limite; x += 1) {
    //if(  Plotar(Temp_SetPoints, RED, true, 360);
    Graph(tft, x,  dados[(int)x],
          225, 305, // gx = x graph location (lower left),  gy = y graph location (lower left)
          245, 170,// w = width of graph,  h = height of graph=240, 190
          0, 240,  // xlo = lower bound of x axis,  xhi = upper bound of x asis
          Min_Temp, Max_Temp, // ylo = lower bound of y axis, yhi = upper bound of y asis
          60, 5, // // xinc = division of x axis (distance not count),yinc = division of y axis (distance not count)
          "Climatic chamber", //title = title of graph
          " Time[h]", "T[C]",
          DKBLUE,
          RED,
          cor,
          WHITE,
          BLACK,
          false,//Refazer_legenda
          Linhas_intermedias_no_inicio,//Linhas_intermedias_no_inicio
          false,//Linhas_intermedias_no_final so no Limpar.
          TPA);//O 1o pixel desta linha é amarelo=TPA.
  }
}
void Rodar_e_acrescentar() {
  // if ((nowTimeUnix - 90) >= Times_of_Temp_Air[119]) {
  //Serial.println("Ainda não passaram 90seg.");
  //Rodar 1 para esquerda:
  for (int i = 0; i < 120; i++) {//240 = ha 240 pontos de 90segundos nas 6h passadas.
    Prev_Temp_Air[i] = Temp_Air[i];
    if (i < 119) {
      Temp_Air[i] = Temp_Air[i + 1];
      // Times_of_Temp_Air[i] = Times_of_Temp_Air[i + 1];
      //  if (Temp_Air[i] != 0)Serial.printf("--Times_of_Temp_Air[%i]:%i Temp_Air[%i]:%f \n", i, Times_of_Temp_Air[i], i, Temp_Air[i]);

    }
    if (i == 119) {
      //  if (Temp_Air[i] != 0)Serial.printf("--Times_of_Temp_Air[%i]:%i Temp_Air[%i]:%f \n", i, Times_of_Temp_Air[i], i, Temp_Air[i]);

      Temp_Air[i] = Air_Input_Temp_Avg;
      //   Times_of_Temp_Air[i] = nowTimeUnix;
    }

  }
  //for (int i = 0; i < 120; i++) Serial.printf("i:%d T:%f \n", i, Temp_Air[i]);


  /*
      //Gravar nova serie de 240:
      char log_1[20];

      //Serial.print("path:"); Serial.println(path);

      File file = SD_MMC.open("/Passed_values.txt", FILE_WRITE);
      if (!file) {
        Serial.println("Failed to open file for appending.");
        return;
      }

      for (int i = 0; i < 240; i++) {
        file.seek(15 * i);
        //if (Temp_Air[i] != 0)Serial.printf("Temp_Air[%i]:%f ", i, Temp_Air[i]);
        snprintf_P(log_1, sizeof(log_1), PSTR("%010i-%02i%.0f\n"), Times_of_Temp_Air[i], (int)Temp_Air[i], round((Temp_Air[i] - (int)Temp_Air[i]) * 10));
        // if (Temp_Air[i] != 0)Serial.printf("Linha %i log_1:%s", i, log_1);
        if (!file.print(log_1)) {
          Serial.println("Append failed.");
        } //Serial.println("Append suseful.");
      }


      file.close();
  */
  //  } //else Serial.println("Ainda não passaram 90seg.");
}

void Limpar_Plot(double dados[240], unsigned int cor, int limite) {
  //tft.fillRect(50+1, 280-242, 400, 242, BLACK);//(canto Sup.esquerdo x e ,y, largura,altura).
  //Serial.println("Chamar Graph in Limpar_Plot");
  bool TPA = false;
  new_plot = true;
  for (x = 0; x < limite; x += 1) {


    //Serial.printf("x = % i, y = % f\n", x, dados[x]);
    Graph(tft, x, dados[x],
          225, 305, // gx = x graph location (lower left),  gy = y graph location (lower left)
          245, 170,// w = width of graph,  h = height of graph
          0, 240,  // xlo = lower bound of x axis,  xhi = upper bound of x asis
          Min_Temp, Max_Temp, // ylo = lower bound of y axis, yhi = upper bound of y asis
          60, 5, // // xinc = division of x axis (distance not count),yinc = division of y axis (distance not count)
          "Climatic chamber", //title = title of graph
          " Time[h]", "T[C]",
          DKBLUE,
          RED,
          cor,
          WHITE,
          BLACK,
          false,//Refazer_legenda
          false,//Linhas_intermedias_no_inicio, apenas nos Setpoints.
          true,//Linhas_intermedias_no_final so no Limpar.
          TPA);//O 1o pixel desta linha é amarelo=TPA.
  }

}
void Plotar_Setpoint(double dados[240][3], unsigned int cor, bool Linhas_intermedias_no_inicio, int limite) {
  //Serial.println("Chamar Graph em Plotar");
  bool TPA = false;

  for (x = 0; x < limite; x += 1) {
    if (x > 0 && dados[x][1]) {
      //  if (x > 0 && dados[x-1][1]) {
      //Serial.printf("BLUE-dados[%i][0]:%f dados[%i][1]:%f  dados[%i][2]:%f\n", x, dados[x][0], x, dados[x][1], x, dados[x][2]);
      cor = BLUE;
    }
    else {
      //Serial.printf("RED-dados[%i][0]:%f dados[%i][1]:%f dados[%i][2]:%f\n", x, dados[x][0], x, dados[x][1], x, dados[x][2]);
      cor = RED;
    }

    if (x > 0 && dados[x][2] ) {

      //cor = YELLOW;
      TPA = true;
      //Serial.printf("YELLOW-dados[%i][0]:%f dados[%i][1]:%f dados[%i][2]:%f  \n", x, dados[x][0], x, dados[x][1], x, dados[x][2]);
    } else  TPA = false;

    //
    //if (dados[x][0] > 0)Serial.printf("dados[%i][0]:%f\n", x, dados[x][0]);
    Graph(tft, x,  dados[(int)x][0],
          225, 305, // gx = x graph location (lower left),  gy = y graph location (lower left)
          245, 170,// w = width of graph,  h = height of graph
          0, 240,  // xlo = lower bound of x axis,  xhi = upper bound of x asis
          Min_Temp, Max_Temp, // ylo = lower bound of y axis, yhi = upper bound of y asis
          60, 5, // // xinc = division of x axis (distance not count),yinc = division of y axis (distance not count)
          "Climatic chamber", //title = title of graph
          " Time[h]", "T[C]",
          DKBLUE,
          RED,
          cor,
          WHITE,
          BLACK,
          false,//Refazer_legenda
          Linhas_intermedias_no_inicio,//Linhas_intermedias_no_inicio
          false, //Linhas_intermedias_no_final so no Limpar.
          TPA);//O 1o pixel desta linha é amarelo=TPA.
  }
}

void Plot_empty_graph() {
  bool TPA = false;
  new_plot = true;
  //Serial.println("Chamar Graph in Plot_empty_graph");
  Graph(tft, x, y,
        225, 305, // gx = x graph location (lower left),  gy = y graph location (lower left)
        245, 170,// w = width of graph,  h = height of graph(estava 240,200)
        0, 240,  // xlo = lower bound of x axis,  xhi = upper bound of x asis
        Min_Temp, Max_Temp, // ylo = lower bound of y axis, yhi = upper bound of y asis
        60, 5, // // xinc = division of x axis (distance not count),yinc = division of y axis (distance not count)
        "Climatic chamber", //title = title of graph
        "Time[h]", "T[C]",
        DKBLUE,
        RED,
        GREEN,
        WHITE,
        BLACK,
        true,
        true,
        false,//Linhas_intermedias_no_final
        TPA);//O 1o pixel desta linha é amarelo=TPA.
}
void Graph(Adafruit_HX8357 &d, double x, double y, double gx, double gy, double w, double h, double xlo, double xhi,  double ylo, double yhi, double xinc, double yinc, String title, String xlabel, String ylabel, unsigned int gcolor, unsigned int acolor, unsigned int pcolor, unsigned int tcolor, unsigned int bcolor, boolean Refazer_legenda, boolean Linhas_intermedias_no_inicio, boolean Linhas_intermedias_no_final, bool TPA) {
  // if (y > yhi)y = yhi;//Limita y ao maximo do eixo dele. Isto causa a falsa ilusão que a linha se mantem no maximo.
  static bool Dado_nulo = false;
  static bool Anterior_era_nulo = false;


  if (y == 0 || y > yhi) {//teste a ver se grafico nao se mantem no topo quando passa o maximo.
    Dado_nulo = true;
    //Serial.println("Dado_nulo");
  }
  else {
    if (Dado_nulo) {
      Anterior_era_nulo = true;
      //Serial.println("Anterior_era_nulo");
    }
    Dado_nulo = false;
    //Serial.println("NAO Dado_nulo");
  }

  double ydiv, xdiv;
  double i;
  double temp;
  int rot, newrot;
  static bool Linhas_intermedias_no_final_ = false;
  //bool Linhas_intermedias_no_final = false;
  //if (linhas_eixos && (!Linhas_intermedias_no_inicio) && (x == 359))Linhas_intermedias_no_final = true;

  //Refazer linhas de eixos e marcacoes intermedias apenas se estiver no ultimo ponto do grafico e Refazer_eixos for true:
  if ((Linhas_intermedias_no_inicio) && (x == 0)) {
    //Serial.println("Fiz linhas de eixos intermedias no inicio.");
    //Redesenhar eixos:
    temp =  (yhi - ylo) * (gy - h - gy) / (yhi - ylo) + gy;
    d.drawLine(gx, temp, gx + w, temp, DARKGREY );
    //Serial.println("03Linhas medias no inicio ");
    for (int i = ylo; i <= (yhi - yinc); i += yinc) {
      temp =  (i - ylo) * (gy - h - gy) / (yhi - ylo) + gy;
      d.drawLine(gx, temp, gx + w, temp, DARKGREY );

      //Serial.printf("0temp:%f\n", temp);
    }
    /* draw x scale*/
    for (int i = xlo; i <= xhi; i += xinc) {
      temp =  (i - xlo) * ( w) / (xhi - xlo) + gx;
      if (temp == 350)d.drawLine(temp, gy, temp, gy - h, DARKGREY ); else d.drawLine(temp, gy, temp, gy - h, DARKGREY );
      // Serial.printf("0temp:%f gy:%f h:%f w:%f xlo:%f xhi:%f \n", temp, gy, h, w, xlo, xhi);
    }

  }
  //

  if (Refazer_legenda == true) {
    /* draw y scale*/
    //Valor Maximo de Y:
    temp =  (yhi - ylo) * (gy - h - gy) / (yhi - ylo) + gy;
    //   d.drawLine(gx, temp, gx + w, temp, DARKGREY );
    d.setTextSize(1);
    d.setTextColor(tcolor, bcolor);
    d.setCursor(gx - 14, temp - 2); //d.setCursor(gx - 40, temp);
    d.println(yhi, 0);

    //Valores dps do max de Y:
    for (int i = ylo; i <= (yhi - yinc); i += yinc) {
      temp =  (i - ylo) * (gy - h - gy) / (yhi - ylo) + gy;
      //Marcacao numerica intermedia em y:
      d.setTextSize(1);
      d.setTextColor(tcolor, bcolor);
      d.setCursor(gx - 14, temp - 2); //d.setCursor(gx - 40, temp);
      //Serial.println(i);
      // if (i != 0)d.println(i, 0);
      if (i != 0)d.println(i, 0);
      //


    }

    /* draw x scale*/
    for (i = xlo; i <= xhi; i += xinc) {
      temp =  (i - xlo) * ( w) / (xhi - xlo) + gx;
      //Marcacao numerica intermedia em x:
      d.setTextSize(1);
      d.setTextColor(tcolor, bcolor);
      // d.setCursor(temp, gy + 10);
      // precision is default Arduino--this could really use some format control
      if (i == 0) {
        d.setCursor(225, gy + 5);
        //  Serial.print("-6h:");  Serial.println( 110);
        d.println("-3h");
      }
      if (i == 120) {
        d.setCursor(345, gy + 5);
        // Serial.print("-3:");   Serial.println( 225);
        d.println("0h");
      }
      if (i == 240) {
        d.setCursor(460, gy + 5);
        // Serial.print("0h:");  Serial.println( 345);
        d.println("3h");
      }


      //d.println(i, 0);
    }
    /*
      //now draw the labels
      d.setTextSize(2);
      d.setTextColor(tcolor, bcolor);
      d.setCursor(gx , gy - h - 30);
      d.println(title);
    */
    /*
      d.setTextSize(1);
      d.setTextColor(acolor, bcolor);
      d.setCursor(gx , gy + 20);
      d.println(xlabel);
    */
    /*
      d.setTextSize(1);
      d.setTextColor(acolor, bcolor);
      d.setCursor(gx - 30, gy - h - 10);
      d.println(ylabel);
    */
    Refazer_legenda = false;
  }

  //graph drawn now plot the data
  // the entire plotting code are these few lines...
  // recall that ox and oy are initialized as static above
  if (new_plot) {
    ox = gx;
    oy = gy;
  }


  //x = 2 + (x - xlo) * ( w) / (xhi - xlo) + gx;
  //y = -2 + (y - ylo) * (gy - h - gy) / (yhi - ylo) + gy;
  //  static bool prim = false;
  if (x == 239) Linhas_intermedias_no_final_ = Linhas_intermedias_no_final;
  if (x == 0 || Anterior_era_nulo) {//Evita a primeira linha começar por zero.
    //Serial.printf("Anterior_era_nulo, agora é %f e cor", y); Serial.println(pcolor);
    x = (x - xlo) * ( w) / (xhi - xlo) + gx;
    y = (y - ylo) * (gy - h - gy) / (yhi - ylo) + gy;
    ox = x;
    oy = y;
    Anterior_era_nulo = false;
  } else {
    x = (x - xlo) * ( w) / (xhi - xlo) + gx;
    y = (y - ylo) * (gy - h - gy) / (yhi - ylo) + gy;
  }
  /*

    if (prim) {
    Serial.print("x: "); Serial.println(x);
    Serial.print("y: "); Serial.println(y);

    Serial.print("ox: "); Serial.println(ox);
    Serial.print("oy: "); Serial.println(oy);

    Serial.print("xlo: "); Serial.println(xlo);
    Serial.print("ylo: "); Serial.println(ylo);

    Serial.print("gx: "); Serial.println(gx);
    Serial.print("gy: "); Serial.println(gy);

    Serial.print("xhi: "); Serial.println(xhi);
    Serial.print("yhi: "); Serial.println(yhi);
    prim = false;
    }
  */
  /*
    Serial.print("ox: "); Serial.println(ox);
    Serial.print("oy: "); Serial.println(oy);
    Serial.print("x: "); Serial.println(x);
    Serial.print("y: "); Serial.println(y);
  */


  if (!Dado_nulo)d.drawLine(ox, oy, x, y, pcolor);//start: (ox, oy) end: (x, y), pcolor.
  if (TPA)d.drawPixel(ox, oy, YELLOW);
  new_plot = false;
  ox = x;
  oy = y;




  //Refazer linhas de eixos e marcacoes intermedias apenas se estiver no ultimo ponto do grafico e Refazer_eixos for true:
  if (Linhas_intermedias_no_final_) {
    //Serial.println("Fiz linhas de eixos intermedias no final.");
    //Redesenhar eixos:
    temp =  (yhi - ylo) * (gy - h - gy) / (yhi - ylo) + gy;
    d.drawLine(gx, temp, gx + w, temp, DARKGREY );





    //Serial.println("03Linhas medias no inicio ");
    for (int i = ylo; i <= (yhi - yinc); i += yinc) {
      temp =  (i - ylo) * (gy - h - gy) / (yhi - ylo) + gy;
      d.drawLine(gx, temp, gx + w, temp, DARKGREY );

      //Serial.println("04Linhas medias no inicio ");
    }
    /* draw x scale*/
    for (int i = xlo; i <= xhi; i += xinc) {
      temp =  (i - xlo) * ( w) / (xhi - xlo) + gx;
      if (temp == 350)d.drawLine(temp, gy, temp, gy - h, DARKGREY ); else d.drawLine(temp, gy, temp, gy - h, DARKGREY );
      //Serial.printf("1temp:%f gy:%f h:%f w:%f xlo:%f xhi:%f \n", temp, gy, h, w, xlo, xhi);
    }
    Linhas_intermedias_no_final_ = false;
  }
  //
}
