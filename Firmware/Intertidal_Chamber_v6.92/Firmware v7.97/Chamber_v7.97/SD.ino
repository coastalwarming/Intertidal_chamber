void readFile_KILL(fs::FS & fs, const char * path) {
  //Serial.println("readFile_KILL()");

  char *p;

  File file = fs.open(path);
  if (!file) {
    Serial.println("Failed to open file /KILL.txt for reading.");
    return;
  }

  char buffer_[10];
  memset(buffer_, 0, sizeof(buffer_));//Sem isto eu via muito lixo no buffer.

  uint8_t i = 0;

  while (file.available())    // note how this also prevents the buffer from overflowing (18 max to leave space for '\0'!)
  {


    buffer_[i] = file.read();
    i++;
    //Serial.println(buffer_);
  }
  //Serial.print("linhas:"); Serial.println(linhas);
  //Serial.println(buffer);
  if (strstr(buffer_, "=") != nullptr) {
    //KILL:
    if (strstr(buffer_, "KILL=") != nullptr) {
      p = strtok(buffer_, "=");
      p = strtok(NULL, "=");

      if (p != NULL) {
        KILL = atoi(p);
        if (KILL == 1)Serial.println("KILL in SD=1");
        //if (KILL == 0) Serial.println("KILL in SD=0");
      } else {
        KILL = 1;

        Serial.println(" KILL = 1; by else");
      }

      Serial.print("KILL in SD card:"); Serial.println(KILL);
    }

  }

  if (KILL)runStatus = KILL_;


  file.close();
}

void readFile(fs::FS & fs, const char * path) {
  char *p;

  File file = fs.open(path);
  if (!file) {
    Serial.println("Failed to open file for reading.");
    return;
  }

  char buffer[100];
  memset(buffer, 0, sizeof(buffer));//Sem isto eu via muito lixo no buffer.

  uint8_t i = 0;
  int linhas = 0;
  byte len = 0;
  //Serial.println("");


  //char Label_device_[13], S1[13], S2[13], S3[13], S4[13], S5[13], S6[13], S7[13], S8[13], S9[13], S10[13];

  while (file.available())    // note how this also prevents the buffer from overflowing (18 max to leave space for '\0'!)
  {

    buffer[i] = file.read();
    i++;
    //  Serial.print("buffer:");
    //  Serial.println(buffer);
    //  Serial.print("buffer[i]:");
    // Serial.println(buffer[i]);
    if (file.peek() == '\n') {

      linhas++;
      if (linhas > 19)break;
      //  Serial.print("linhas:"); Serial.println(linhas);
      // Serial.println(buffer);
      if (strstr(buffer, "=") != nullptr) {

        //Device_name:
        if (strstr(buffer, "Device_name=") != nullptr) {
          p = strtok(buffer, "=");
          p = strtok(NULL, "=");
          if (p != NULL) {
            len = strlen(p);
          } else {
            //Serial.println("Device is NULL, setting to default: Pulse.");
            p = "Chamber";
            len = strlen(p);
          }
          if (len > 10)len = 10;//Limite de caracteres.
          strncpy(Device_name_temp, p, len);
          Device_name_temp[len] = '\0';
          strcpy(Device_name, Device_name_temp);
          // Serial.print("Device_name="); Serial.println(Device_name);
        }

        //Treatment - MQTT label para receber novo profile:
        if (strstr(buffer, "Treatment=") != nullptr) {
          p = strtok(buffer, "=");
          p = strtok(NULL, "=");
          if (p != NULL) {
            len = strlen(p);
          } else {
            //Serial.println("Device is NULL, setting to default: Pulse.");
            p = "Treatment";
            len = strlen(p);
          }
          if (len > 10)len = 10;//Limite de caracteres.
          strncpy(Treatment_temp, p, len);
          Treatment_temp[len] = '\0';
          strcpy(Treatment, Treatment_temp);
          Serial.print("Treatment="); Serial.println(Treatment);
        }

        //Wifi_ssid:
        if (strstr(buffer, "Wifi_ssid=") != nullptr) {
          p = strtok(buffer, "=");
          p = strtok(NULL, "=");
          if (p != NULL) {
            strncpy(Wifi_ssid_temp, p, (strlen(p)));
            Wifi_ssid_temp[strlen(p)] = '\0';
            Wifi_ssid = &Wifi_ssid_temp[0];
            No_wifi = false;
          } else {
            Serial.println("Wifi_ssid is NULL, No_wifi set to true.");
            No_wifi = true;
          }
          Serial.print("Wifi_ssid:"); Serial.println(Wifi_ssid);
          //Serial.print("strlen(p):"); Serial.println(strlen(p));
        }
        //Wifi_pass:
        if (strstr(buffer, "Wifi_pass=") != nullptr) {
          p = strtok(buffer, "=");
          p = strtok(NULL, "=");
          if (p != NULL) {
            strncpy(Wifi_pass_temp, p, (strlen(p)));
            Wifi_pass_temp[strlen(p)] = '\0';
            Wifi_pass = &Wifi_pass_temp[0];
          } else {
            Serial.println("Wifi_pass is NULL, Wifi_pass set to empty.");
            Wifi_pass = "";
          }
          Serial.print("Wifi_pass:"); Serial.println(Wifi_pass);
          //Serial.print("strlen(p):"); Serial.println(strlen(p));
        }

        //AP_pass:
        if (strstr(buffer, "AP_pass=") != nullptr) {
          p = strtok(buffer, "=");
          p = strtok(NULL, "=");
          if (p != NULL) {
            strncpy(AP_pass_temp, p, (strlen(p)));
            AP_pass_temp[strlen(p)] = '\0';
            AP_pass = &AP_pass_temp[0];
          } else {
            Serial.println("AP_pass is NULL, AP_pass set to empty.");
            AP_pass = "";
          }
          Serial.print("AP_pass:"); Serial.println(AP_pass);
        }
        //Local_time_zone:
        if (strstr(buffer, "Local_time_zone=") != nullptr) {
          p = strtok(buffer, "=");
          p = strtok(NULL, "=");
          if (p != NULL) {
            UTC = atof(p);
            if (UTC < -12 || UTC > 12) {
              UTC = 0;
              //     Serial.println("UTC failed -12:12 filter, setting to default: 0.");
            }
          } else {
            UTC = 0;
            //  Serial.println("UTC is NULL, setting to default: 0.");
          }
          Serial.print("UTC:"); Serial.println(UTC);
        }

        //DST:
        if (strstr(buffer, "DST=") != nullptr) {
          p = strtok(buffer, "=");
          p = strtok(NULL, "=");
          if (p != NULL) {
            if (p[0] == 'Y' || p[0] == 'y')DST = true; else {
              //    Serial.println("DST is not Y or y, DST set to false.");
              DST = false;
            }
          } else {
            // Serial.println("DST is NULL, DST set to No.");
            DST = false;
          }
          Serial.print("DST:"); Serial.println(DST);
        }

        //Wait_before_air_heat:
        if (strstr(buffer, "Heat_wait=") != nullptr) {
          p = strtok(buffer, "=");
          p = strtok(NULL, "=");
          if (p != NULL) {
            Wait_before_air_heat = atoi(p);
            if (Wait_before_air_heat < 0) {
              Wait_before_air_heat = 120;
              Serial.println("Wait_before_air_heat failed -0:121 filter, setting to default: 120.");
            }
          } else {
            Wait_before_air_heat = 120;
            //  Serial.println("Wait_before_air_heat is NULL, setting to default: 120.");
          }
          Serial.print("Wait_before_air_heat:"); Serial.println(Wait_before_air_heat);
        }

        //Chiller_wait:
        if (strstr(buffer, "Chiller_wait=") != nullptr) {
          p = strtok(buffer, "=");
          p = strtok(NULL, "=");
          if (p != NULL) {
            Chiller_wait = atoi(p);
            if (Chiller_wait < 0) {
              Chiller_wait = 180;
              Serial.println("Chiller_wait failed -0:121 filter, setting to default: 180.");
            }
          } else {
            Chiller_wait = 180;
            //  Serial.println("Chiller_wait is NULL, setting to default: 180.");
          }
          Serial.print("Chiller_wait:"); Serial.println(Chiller_wait);
        }
        //Top_sensors:
        if (strstr(buffer, "Top_sensors=") != nullptr) {
          p = strtok(buffer, "=");
          p = strtok(NULL, "=");
          if (p != NULL) {
            if (p[0] == 'Y' || p[0] == 'y')Top_sensors = true; else {
              //    Serial.println("Top_sensors is not Y or y, Top_sensors set to false.");
              Top_sensors = false;
            }
          } else {
            // Serial.println("Top_sensors is NULL, DST set to No.");
            Top_sensors = true;
          }
          Serial.print("Top_sensors:"); Serial.println(Top_sensors);
        }

        //Bottom_sensors:
        if (strstr(buffer, "Bottom_sensors=") != nullptr) {
          p = strtok(buffer, "=");
          p = strtok(NULL, "=");
          if (p != NULL) {
            if (p[0] == 'Y' || p[0] == 'y')Bottom_sensors = true; else {
              //    Serial.println("DST is not Y or y, DST set to false.");
              Bottom_sensors = false;
            }
          } else {
            // Serial.println("Bottom_sensors is NULL, Bottom_sensors set to No.");
            Bottom_sensors = true;
          }
          Serial.print("Bottom_sensors:"); Serial.println(Bottom_sensors);
        }

        //Water_release:
        if (strstr(buffer, "Water_release=") != nullptr) {
          p = strtok(buffer, "=");
          p = strtok(NULL, "=");
          if (p != NULL) {
            Water_release_period = atoi(p);
            if ( Water_release_period < 5) Water_release_period = 5;
          } else {
            Water_release_period = 600;
            // Serial.println(" Water_release_period is NULL, setting to default: 600seconds.");
          }

          Serial.print("Water_release_period:"); Serial.println( Water_release_period);
        }

        //Water_fill:
        if (strstr(buffer, "Water_fill=") != nullptr) {
          p = strtok(buffer, "=");
          p = strtok(NULL, "=");
          if (p != NULL) {
            Water_fill_period = atoi(p);
            if (Water_fill_period < 5)Water_fill_period = 5;
          } else {
            Water_fill_period = 300;
            // Serial.println("Water_fill_period is NULL, setting to default: 30seconds.");
          }

          Serial.print("Water_fill_period:"); Serial.println(Water_fill_period);
        }

        //MQTT_Host:
        if (strstr(buffer, "MQTT_Host=") != nullptr) {
          p = strtok(buffer, "=");
          p = strtok(NULL, "=");
          if (p != NULL) {
            strncpy(MQTT_Host_temp, p, (strlen(p)));
            MQTT_Host_temp[strlen(p)] = '\0';
            MQTT_Host = &MQTT_Host_temp[0];
          } else {
            Serial.println("MQTT_Host is NULL, No_MQTT set True.");
            No_MQTT = true;
          }
          Serial.print("MQTT_Host:"); Serial.println(MQTT_Host);
          //Serial.print("strlen(p):"); Serial.println(strlen(p));
        }

        //MQTT_ID:
        if (strstr(buffer, "MQTT_ID=") != nullptr) {
          p = strtok(buffer, "=");
          p = strtok(NULL, "=");
          if (p != NULL) {
            strncpy(MQTT_ID_temp, p, (strlen(p)));
            MQTT_ID_temp[strlen(p)] = '\0';
            MQTT_ID = &MQTT_ID_temp[0];
          } else {
            Serial.println("MQTT_ID_ssid is NULL, No_MQTT set True.");
            No_MQTT = true;
          }
          Serial.print("MQTT_ID:"); Serial.println(MQTT_ID);
          //Serial.print("strlen(p):"); Serial.println(strlen(p));
        }

        //MQTT_pass:
        if (strstr(buffer, "MQTT_pass=") != nullptr) {
          p = strtok(buffer, "=");
          p = strtok(NULL, "=");
          if (p != NULL) {
            strncpy(MQTT_pass_temp, p, (strlen(p)));
            MQTT_pass_temp[strlen(p)] = '\0';
            MQTT_pass = &MQTT_pass_temp[0];
          } else {
            Serial.println("MQTT_pass is NULL, No_MQTT set True.");
            No_MQTT = true;
          }
          Serial.print("MQTT_pass:"); Serial.println(MQTT_pass);
          //Serial.print("strlen(p):"); Serial.println(strlen(p));
        }

        //BLYNK_TEMPLATE_ID:
        if (strstr(buffer, "BLYNK_TEMPLATE_ID") != nullptr) {
          p = strtok(buffer, "=");
          p = strtok(NULL, "=");
          if (p != NULL) {
            strncpy(BLYNK_TEMPLATE_ID_, p, sizeof BLYNK_TEMPLATE_ID_);
            BLYNK_TEMPLATE_ID_[sizeof BLYNK_TEMPLATE_ID_] = '\0';//Forma de copiar char* do strtok para char global.
          } else {
            Serial.println("BLYNK_TEMPLATE_ID_ is NULL, No_Blynk set True.");
            No_Blynk = true;
          }
          Serial.print("BLYNK_TEMPLATE_ID_:"); Serial.println(BLYNK_TEMPLATE_ID_);
        }

        //BLYNK_DEVICE_NAME: (para que serve?)
        /*
                if (strstr(buffer, "BLYNK_DEVICE_NAME") > 0) {
                  p = strtok(buffer, "=");
                  p = strtok(NULL, "=");
                  strncpy(BLYNK_DEVICE_NAME_, p, sizeof BLYNK_DEVICE_NAME_);
                  BLYNK_DEVICE_NAME_[sizeof BLYNK_DEVICE_NAME_] = '\0';//Forma de copiar char* do strtok para char global.
                  Serial.print("BLYNK_DEVICE_NAME_:"); Serial.println(BLYNK_DEVICE_NAME_);
                }
        */

        //auth:
        if (strstr(buffer, "BLYNK_AUTH_TOKEN") != nullptr) {
          p = strtok(buffer, "=");
          p = strtok(NULL, "=");
          if (p != NULL) {
            strncpy(auth, p, (strlen(p)));
            auth[sizeof auth] = '\0';
          } else {
            Serial.println("BLYNK_AUTH_TOKEN  is NULL, No_Blynk set True.");
            No_Blynk = true;
          }
          Serial.print("BLYNK_AUTH_TOKEN:"); Serial.println(auth);
        }



      }
      i = 0;
      memset(buffer, 0, sizeof buffer);
    }
  }
  file.close();

  Serial.println();
}

void Create_n_Log_data_to_SD() {

  char log_1[80];

  Wire.begin();//Precisa disto?
  /*//Isto nao esta bem.
    if (Outrtc_working) {
    //Serial.println("Outrtc_working=true.");
    if (RTC_Pos_2022) {
      Inrtc.setTime(nowTimeUnix); //Apenas para manter o timestamp nos files.
    } else {
      if (Online) {
        NTP_epochTime = getTime();
        if (NTP_epochTime > 1640995201) {//1640995201=1 January 2022 00:00:01
          RTC_Pos_2022 = true;
          Outrtc.setEpoch(NTP_epochTime);
          Inrtc.setTime(NTP_epochTime);
          Serial.printf("NTP_epochTime set on Outrtc and Inrtc: %i\n", NTP_epochTime);
        } else Serial.println("NTP_epochTime came with Epoch: 1640995201=1 January 2022 00:00:01.");
      } else Serial.println("RTC_Pos_2022 is false but I'm not Online.");

      Serial.println("Outrtc was not working. Set Inrtc and Outrtc according to wifi - retrieved NTP_epochTime: "); Serial.println(NTP_epochTime);
    }
    //
    } else {
    Serial.println("Outrtc_working is not communicating, attempting start_rtc() again.");
    start_rtc();
    }
  */
  //Daqui para baixo, conto ter as horas certas:

  //Criar ficheiro com nome do dia atual caso não exista:
  String lLog = "Data/";
  snprintf_P(File_name, sizeof(File_name), PSTR("/Data/%i_%02i_%02i__%02ih.CSV"), year(nowTimeUnix), month(nowTimeUnix), day(nowTimeUnix), hour(nowTimeUnix));
  //Serial.print("File name:"); Serial.println(File_name);
  //
  //Opening or creating file:
  File file = SD_MMC.open(File_name);
  //

  //O file existe ou tenho de o criar?
  if (!file) {//Nao vejo o file mas estava ligado ao cartão?
    //Serial.println("File doesn't exist.");
    //Serial.println("Creating file:"); Serial.println(File_name);
    //Definir cabeçalho:
    Serial.print("Creating file named:"); Serial.println(File_name);
    writeFile(SD_MMC, File_name, "", 0);
    //
  }
  else {
    //Serial.print("File:"); Serial.print(File_name); Serial.println(" already exists.");
  }
  file.close();
  //

  //Ficheiro já existe, vou fazer append do log:
  if (!Reset) {
    appendFile(SD_MMC, File_name , "Reset", 1);
    Reset = true;
  }

  if (sd_back) {
    appendFile(SD_MMC, File_name , "SD reconnected", 1);
    sd_back = false;
  }

  char Log_data[400];
  //Serial.println("Preencher Log_data_M");
  snprintf_P(Log_data, sizeof(Log_data), PSTR("%d-%02d-%02d,%02d:%02d:%02d,%d.%02d,%d.%02d,%d.%02d,%d.%02d,%d.%02d,%d.%02d,%d.%02d,%d.%02d,%d.%02d,%d.%02d,%d,"),
             year(nowTimeUnix),  month(nowTimeUnix), day(nowTimeUnix),
             hour(nowTimeUnix), minute(nowTimeUnix), second(nowTimeUnix),

             (int)Air_Temp_1, (int)(Air_Temp_1 * 100) % 100,
             (int)Air_Temp_2, (int)(Air_Temp_2 * 100) % 100,
             (int)Air_Temp_3, (int)(Air_Temp_3 * 100) % 100,
             (int)Water_Temp_1, (int)(Water_Temp_1 * 100) % 100,
             (int)Water_Temp_2, (int)(Water_Temp_2 * 100) % 100,
             (int)Room_Temp, (int)(Room_Temp * 100) % 100,

             (int)Air_Input_Temp_Avg, (int)(Air_Input_Temp_Avg * 100) % 100,
             (int)Temp_Setpoint, (int)(Temp_Setpoint * 100) % 100,

             (int)Water_Input_Temp_Avg, (int)(Water_Input_Temp_Avg * 100) % 100,
             (int)Water_Temp_Setpoint, (int)(Water_Temp_Setpoint * 100) % 100,
             Now_Tide
            );


  for (int i = 0; i < 8; i++) {
    if (state[i] == 0) strcat(Log_data, "-1,");
    if (state[i] == 1) strcat(Log_data, "0,");
    if (state[i] == 3) strcat(Log_data, "1,");
  }

  char novo[23] = {};
  sprintf(novo, "%.02f,", PID_Output_1);
  strcat(Log_data, novo);
  memset(novo, 0, sizeof novo);

  if (Water_Cooler_state == On)strcat(Log_data, "1,"); else strcat(Log_data, "0,");
  if (Water_Heater_state == On)strcat(Log_data, "1,"); else strcat(Log_data, "0,");
  if (Cooler_Pump_state == On)strcat(Log_data, "1,"); else strcat(Log_data, "0,");
  if (Tide_Pump_state == On)strcat(Log_data, "1,"); else strcat(Log_data, "0,");

  sprintf(novo, "%03d,", Now_Light);
  strcat(Log_data, novo);
  memset(novo, 0, sizeof novo);

  if (Circle_FAN_state == On)strcat(Log_data, "1,"); else strcat(Log_data, "0,");
  if (Cool_FAN_state == On)strcat(Log_data, "1,"); else strcat(Log_data, "0,");
  if (Release_WV_state == On)strcat(Log_data, "1,"); else strcat(Log_data, "0,");
  if (Enter_WV_state == On)strcat(Log_data, "1,"); else strcat(Log_data, "0,");

  sprintf(novo, "%.03f,%.03f,%.03f", Kp, Ki, Kd);
  strcat(Log_data, novo);
  memset(novo, 0, sizeof novo);


  //Serial.print("Log_data:"); Serial.println(Log_data);

  appendFile(SD_MMC, File_name , Log_data, 1);
  memset(Log_data, 0, sizeof Log_data);

  //Para exemplo:
  //sprintf(novo, ",TSH%i_0,TSH%i_1,TSH%i_2", i, i, i);
  //strcat(header, novo);
}

bool GetProfile_SD(fs::FS & fs, const char * path) {
  int i = 0;
  char buf[20], Prev_buf[20];

  Serial.printf("Reading file: %s\n", path);

  File Profile = fs.open(path);
  if (!Profile) {
    Serial.println("Failed to open file for reading.");
    return false;
  }

  //Serial.println("Read from file in GetProfile_SD(): ");

  while (Profile.available()) {


    //int max_len = Profile.available();
    //byte read_len = Profile.readBytesUntil('\n', buf, 18); //17 read to buffer, \n character is discarded from buffer.//'\n'
    //Profile.readBytesUntil('\n', buf, 17);//Necessario para nao repetir cada linha 2 vezes, mas com o filtro das 17 linhas parece que nao e necessario.
    Profile.readBytesUntil('\n', buf, 20);

    //Serial.printf("0Conteudo:%s \n", buf);
    buf[19] = '\0';//Se nao terminar o array com 0, ele imprime coisas estranhas no final.

    byte read_len = strlen(buf);
    //Serial.printf("0Conteudo[%i]:%s \n", i, buf);
    //Serial.print("read_len:"); Serial.println(read_len);

    //Prev_buf[19] = '\0';//Se nao terminar o array com 0, ele imprime coisas estranhas no final.
    /*
      Serial.print("Prev_buf:"); Serial.println(Prev_buf);
      Serial.print("buf:"); Serial.println(buf);
      Serial.print("read_len_buf:"); Serial.println(strlen(buf));
      Serial.print("read_len_Prev_buf:"); Serial.println(strlen(Prev_buf));
    */
    if ((strcmp(buf, Prev_buf) == 0) ) {
      // strings are equal
      //      Serial.printf("Strings are equal: |%s| == |%s| \n", Prev_buf, buf );
    } //else  Serial.printf("Strings are not equal: %s == %s \n", Prev_buf, buf );



    if (read_len == 19 && (strcmp(buf, Prev_buf) != 0)) {
      //Serial.printf("\nLinha %i:\n", i);
      //Serial.print(i); Serial.print("   "); Serial.println(buf);
      //Serial.printf("buf[0]:%c, buf[1]:%c, buf[2]:%c, buf[3]:%c, buf[4]:%c, buf[5]:%c, buf[6]:%c, buf[7]:%c, buf[8]:%c, buf[9]:%c,buf[10]:%c,buf[11]:%c, buf[12]:%c, buf[13]:%c, buf[14]:%c, buf[15]:%c,buf[16]:%c,buf[17]:%c,buf[18]:%c,buf[19]:%c\n", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7], buf[8], buf[9], buf[10], buf[11], buf[12], buf[13], buf[14], buf[15], buf[16], buf[17], buf[18], buf[19]);

      //Serial.printf("Bytes read:%i .\n",read_len);
      // Serial.printf("1Conteudo:%s \n", buf);
      char buf1[11] = {buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7], buf[8], buf[9], '\0'};
      profileTimes[i] = atol(buf1);
      //Serial.printf("profileTimes %i:%i \n", i, profileTimes[i]);

      char buf2[8] = {buf[11], buf[12], buf[13], buf[14], buf[15], buf[16], buf[17], '\0'};
      profileTTL[i] = atol(buf2);
      //Serial.printf("0profileTTL %i:%i \n", i, profileTTL[i]);

      char buf3[2] = {buf[18], '\0'};
      profileTPA[i] = atoi(buf3);
      //Serial.printf("0profileTPA %i:%i \n", i, profileTPA[i]);

      i++;




    } else  {

      //Serial.printf("Line without 19 chars at Profile.txt line %i, or duplicated line. ( GetProfile_SD() ) \n", i + 1);
      //Serial.printf("2Conteudo:%s \n", buf);
      //Serial.printf("strlen(buf):%i .\n", strlen(buf));
    }

    strncpy(Prev_buf, buf, 20);
    // Serial.printf("\n\n");
  }
  profileLength = i; //Guarda o numero de passos do perfil.
  Serial.printf("ProfileLength:"); Serial.println(profileLength);
  /*
    for (i = 0; i < profileLength; i++) {
      Serial.printf("1profileTimes[%i]:", i); Serial.println(profileTimes[i]);
      Serial.printf("1profileTTL[%i]:", i); Serial.println(profileTTL[i]);
      Serial.printf("1profileTPA[%i]:", i); Serial.println(profileTPA[i]);
    }
  */
  //Imprimir primeiro e ultimo:
  Serial.printf("1profileTimes[%i]:", 0); Serial.println(profileTimes[0]);
  Serial.printf("1profileTTL[%i]:", 0); Serial.println(profileTTL[0]);
  Serial.printf("1profileTPA[%i]:", 0); Serial.println(profileTPA[0]);
  Serial.println();
  Serial.printf("1profileTimes[%i]:", profileLength-1); Serial.println(profileTimes[profileLength-1]);
  Serial.printf("1profileTTL[%i]:", profileLength-1); Serial.println(profileTTL[profileLength-1]);
  Serial.printf("1profileTPA[%i]:", profileLength-1); Serial.println(profileTPA[profileLength-1]);

  Profile.close();

  if (profileLength == 0) {
    return false;
  } else return true;
}

void Did_Profile_changed(fs::FS & fs, const char * path) {
  int i = 0;
  char buf[20], Prev_buf[20];
  Profile_changed = false;

  //GetProfile_SD(SD_MMC, "/Profile.txt");
  Serial.printf("Reading file: %s\n", path);

  File Profile = fs.open(path);
  if (!Profile) {
    Serial.println("Did_Profile_changed(): Failed to Profile.txt for reading.");
    Profile_changed = true;
  }

  Serial.println("Read Profile.txt in Did_Profile_changed():");

  while (Profile.available()) {
    Profile.readBytesUntil('\n', buf, 20);

    //  Serial.printf("0Conteudo:%s \n", buf);
    buf[19] = '\0';//Se nao terminar o array com 0, ele imprime coisas estranhas no final.

    byte read_len = strlen(buf);

    if (read_len == 19 && (strcmp(buf, Prev_buf) != 0)) {
      // Serial.printf("\nLinha %i:\n", i);

      //Serial.printf("Linha:%i 0Conteudo:%s \n", i, buf);
      // buf[17] = '\0';//Se nao terminar o array com 0, ele imprime coisas estranhas no final.
      //Serial.printf("buf[0]:%c, buf[1]:%c, buf[2]:%c, buf[3]:%c, buf[4]:%c, buf[5]:%c, buf[6]:%c, buf[7]:%c, buf[8]:%c, buf[9]:%c,buf[10]:%c,buf[11]:%c, buf[12]:%c, buf[13]:%c, buf[14]:%c, buf[15]:%c,buf[16]:%c\n", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7], buf[8], buf[9],buf[10],buf[11], buf[12], buf[13], buf[14], buf[15],buf[16]);

      //Serial.printf("Bytes read:%i .\n",read_len);
      //Serial.printf("1Conteudo:%s \n", buf);
      char buf1[11] = {buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7], buf[8], buf[9], '\0'};
      if (profileTimes[i] != atol(buf1)) {
        //Serial.printf("profileTimes[%i] different:%i!=%i\n ", i, profileTimes[i], atol(buf1));
        Profile_changed = true;
      } //else Serial.println("The same:profileTimes[i]");
      //Serial.printf("Time_Step %i:%i ", i, profileTimes[i]);

      char buf2[8] = {buf[11], buf[12], buf[13], buf[14], buf[15], buf[16], buf[17], '\0'};
      // profileTTL[i] = atol(buf2);
      if (profileTTL[i] != atol(buf2)) {
        //Serial.printf("profileTTL[%i] different:%i!=%i\n ", i, profileTTL[i], atol(buf2));
        Profile_changed = true;
      } //else Serial.println("The same:profileTTL[i]");

      char buf3[2] = {buf[18], '\0'};
      // profileTTL[i] = atol(buf2);
      if (profileTPA[i] != atoi(buf3)) {
        //Serial.printf("profileTPA[%i] different:%i!=%i\n ", i, profileTPA[i], atoi(buf3));
        Profile_changed = true;
      } //else Serial.println("The same:profileTTL[i]");

      //Serial.printf("TTL_Step %i:%i \n", i, profileTTL[i]);

      i++;
    } else  {

      //Serial.printf("Line without 19 chars at Profile.txt line %i, or duplicated line. ( GetProfile_SD() ) \n", i + 1);
      //Serial.printf("2Conteudo:%s \n", buf);
      //Serial.printf("strlen(buf):%i .\n", strlen(buf));
    }

    strncpy(Prev_buf, buf, 20);
    // Serial.printf("\n\n");
  }
  if (profileLength != i) {
    Serial.printf("profileLength:%i != i:%i\n ", profileLength, i);
    Profile_changed = true;
  };//Guarda o numero de passos do perfil.
  // Serial.print("profileLength:"); Serial.println(profileLength);

  Profile.close();

  if (Profile_changed) {
    Serial.println("Profile changed!");
    Turn_all_off(true);
    //Display
    xSemaphoreTake(mutex, portMAX_DELAY);

    tft.fillScreen(BLACK);
    tft.setTextColor(WHITE);
    tft.setTextSize(3);
    if (sd_exist)drawCentreString("Received new Profile.txt.", 240, 140, 25, 3);
    else drawCentreString("Detected SD card.", 240, 140, 17, 3);
    drawCentreString("Rebooting...", 240, 180, 12, 3);


    xSemaphoreGive(mutex);

    delay(3000);
    ESP.restart();
    //
  } else Serial.println("Profile did not change.");


}

void Gravar_SD() {
  if (sd_exist) {
    //Serial.println("In Gravar_SD()->sd_exist=true.");
    Create_n_Log_data_to_SD();
  }
  else {
    Serial.println("Não consegui logar pk nao detetei o cartão, vou tentar relogar.");
    if (Religar())Create_n_Log_data_to_SD();
  }
}

bool Religar() {
  char log_1[80];
  SD_MMC.end();//Teste - necessario?
  InitSDCard();
  if (SenseSD()) {
    Serial.println("Ja consegui ligar ao cartão.");

    Turn_all_off(true);
    // tft.begin(HX8357D);
    // tft.setRotation(3);
    xSemaphoreTake(mutex, portMAX_DELAY);

    tft.fillScreen(BLACK);
    tft.setTextColor(WHITE);
    tft.setTextSize(3);
    drawCentreString("SD card detected.", 240, 140, 18, 3);
    drawCentreString("Rebooting...", 240, 180, 12, 3);

    xSemaphoreGive(mutex);
    delay(3000);
    ESP.restart();

    return true;
  } else {
    Serial.println("Ainda não consegui ligar ao cartão.");
    return false;
  }
}

bool SenseSD(void) {
  //Serial.println("SenseSD()");

  if (SD_MMC.open("/test.txt", "w"))
  {
    SD_MMC.remove("/test.txt");
    //Serial.println("Succesfuly created and removed.");
    //Serial.println("SD card detected");


    if (!sd_exist) {
      Serial.println("SD card redetected!");
      xSemaphoreTake(mutex, portMAX_DELAY);
      tft.setTextColor(GREEN); tft.setCursor(140, 23); tft.print("SD");
      xSemaphoreGive(mutex);
      Did_Profile_changed(SD_MMC, "/Profile.txt");
      sd_exist = true;
      sd_back = true;
    }

    return true;
  } else  {
    if (sd_exist) {
      sd_exist = false;
      sd_back = false;
      xSemaphoreTake(mutex, portMAX_DELAY);

      tft.setTextColor(DARKGREY); tft.setCursor(140, 23); tft.print("SD");
      xSemaphoreGive(mutex);
      //Serial.println("Error in creating file");
      Serial.println("SD card not detected in SenseSD()");
    }
    return false;
  }
}

void start_sd() {
  if (InitSDCard()) {

    //Testar gravar num file:
    File file_2 = SD_MMC.open("/test_2.txt", FILE_WRITE);
    if (file_2 && file_2.println("test")) {
      file_2.close();
      // listDir(SD_MMC, "/", 0);
      SD_MMC.remove("/test_2.txt");
      //listDir(SD_MMC, "/", 0);
      Serial.println("Passou no teste de escrita no start_sd()_1");
      //

      //Serial.println("InitSDCard() true.");
      xSemaphoreTake(mutex, portMAX_DELAY);
      tft.setTextColor(GREEN); tft.setCursor(140, 23); tft.print("SD");
      xSemaphoreGive(mutex);
      sd_exist = true;
      createDir(SD_MMC, "/Data");
    } else {

      Serial.println("Falhou no teste de escrita no start_sd()_1");
      sd_exist = false;

    }

  } else {
    Serial.println("SD card not detected in start_sd()_2");
    sd_exist = false;
  }
}

bool InitSDCard() {
  if (!SD_MMC.setPins(42, 41, 2, 1, 39, 40)) {//bool setPins(int clk, int cmd, int d0, int d1, int d2, int d3);// NO DEVKitM S3 o 33 e 34 nao tem breakout
    Serial.println("Failed pin setup");
  }


  if (!SD_MMC.begin()) {
    for (int i = 0; i < 10; i++) {
      // if (SD_MMC.begin(5, spi, 8000000))return 1; else {
      if (SD_MMC.begin())return 1; else {
        SD_MMC.end();
        Serial.printf("Card Mount Failed, % i.\n", i );
        delay(500);
      }
    }
    Serial.println("Returning 0 in InitSDCard().");
    return 0;
  } else {
    //Serial.println("Card Mount Successful.");
    return 1;
  }
  return 0;
  //if (!SD_MMC.begin() && tentativas >= 10) return 0; else return 0;
}

void createDir(fs::FS & fs, const char * path) {
  // Serial.printf("Creating Dir: % s\n", path);

  if (fs.mkdir(path)) {
    //Serial.println("Dir created.");
  } else {
    Serial.println("mkdir failed.");
    Serial.println("SD card not detected in createDir()");

    sd_exist = false;
  }


}

void writeFile(fs::FS & fs, const char * path, const char * message, bool isKILL_or_Profile) {
  Serial.printf("Writing file: % s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing - > sd_exist set to false.");
    sd_exist = false;
    sd_back = false;
    xSemaphoreTake(mutex, portMAX_DELAY);

    tft.setTextColor(DARKGREY); tft.setCursor(140, 23); tft.print("SD");
    xSemaphoreGive(mutex);
    return;
  }
  if (isKILL_or_Profile) {
    file.print(message);
    file.close();
    Serial.println("Saved KILL file.");
  } else {
    char log_[30] = {};
    char log_2[30] = {};
    memset(log_, 0, sizeof(log_));//Sem isto eu via muito lixo no buffer.

    file.println("------------------------------,--------------------");
    file.println("www.electricblue.eu, Portugal");
    file.println("------------------------------,--------------------");
    file.printf("Intertidal version,V%.1f\n", Version);
    file.println("------------------------------,--------------------");
    file.printf("Device,%s\n", Device_name);
    file.printf("Treatment,%s\n", Treatment);
    file.printf("UTC,%.2f\n", UTC);
    if (DST)file.printf("Day light saving time,Yes\n"); else file.printf("Day light saving time,No\n");
    file.printf("Heat_wait,%d\n", Wait_before_air_heat);
    file.printf("Chiller_wait,%d\n", Chiller_wait);
    file.printf("Water_release,%d\n", Water_release_period);
    file.printf("Water_fill,%d\n", Water_fill_period);
    file.printf("Experiment_Start,%s\n", Start);
    file.printf("Experiment_End,%s\n", End);
    file.println("------------------------------,--------------------");
    file.println("Date,Time,T1,T2,T3,T4,T5,T6,Top_avg,Top_setpoint,Base_avg,Base_setpoint,Tide,WS1,WS2,WS3,WS4,WS5,WS6,WS7,WS8,Heat_Lamps_%,Water_Cooler_state,Water_Heater_state,Cooler_pump_state,Tide_pump_state,LED_intensity_%,Circle_fan_state,Cool_fan_state,Outlet_valve_state,Inlet_valve_state,Kp,Ki,Kd");


    file.close();
  }
}

void appendFile(fs::FS & fs, const char * path, char message[], bool paragrafo) {
  //Wire.begin();//Teste2
  char log_1[80];
  //const char * message

  //Serial.print("path : "); Serial.println(path);

  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    Serial.println("Failed to open file for appending.");
    return;
  }
  if (paragrafo) {
    if (!file.println(message)) {
      snprintf_P(log_1, sizeof(log_1), PSTR("Append failed.\n")); Serial.print(log_1);
      Serial.println("Append with paragrafo failed.");
    } else {
      //Serial.print("0Appended : "); Serial.println(message);
    }
  } else {
    if (!file.print(message)) {
      snprintf_P(log_1, sizeof(log_1), PSTR("Append failed.\n")); Serial.print(log_1);
      Serial.println("Append without paragrafo failed.");
    } else {
      //Serial.print("1Appended : "); Serial.println(message);
    }
  }

  file.close();
}

void listDir(fs::FS &fs, const char * dirname, uint8_t levels) {
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if (!root) {
    Serial.println("Failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels) {
        listDir(fs, file.name(), levels - 1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}
