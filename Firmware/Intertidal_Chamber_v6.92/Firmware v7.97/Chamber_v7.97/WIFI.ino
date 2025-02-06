void Manage_wifi_state_n_display() {

  if (!AP_set && !Online && !Attempted_STA_connect && !No_wifi) {
    //Serial.println("1- !AP_set && !Online && !Attempted_STA_connect");
    if (!Ongoing_wifi_scan) {
      Start_wifi_search();
    }
    if (Ongoing_wifi_scan && (WiFi.scanComplete() >= 0))Check_wifi_list();
    //if (Ongoing_wifi_scan && (WiFi.scanComplete() == -1)) Serial.println("scanComplete in progress.");

    // if (Ongoing_wifi_scan && (WiFi.scanComplete() == -2)) Serial.println("WiFi.scanComplete() == -2 -> Scan not triggered mas Ongoing_wifi_scan=true.");
    if (wifi_list_checked) {
      if (Wifi_available)Attempt_STA_connect();
      else Set_Wifi_AP();
    }
  } else {
    /*
      Serial.print("AP_set: "); Serial.println(AP_set);
      Serial.print("Online: "); Serial.println(Online);
      Serial.print("Attempted_STA_connect: "); Serial.println(Attempted_STA_connect);
    */
  }

  if (Attempted_STA_connect && (millis() >= Instant_Tried_wifi_sta_connect + 20000)) {
    Serial.println("2- Attempted_STA_connect && (millis() >= Instant_Tried_wifi_sta_connect + 20000)");

    Attempted_STA_connect = false;
    WiFi.disconnect();
    Serial.println("4Attempted_STA_connect set to false");

    if (!AP_set) {
      Set_Wifi_AP();
      Serial.println("Attempted_STA_connect passed 20s, calling Set_Wifi_AP();");
    } else Serial.println("Attempted_STA_connect passed 20s, but AP_set is already true.");
  }

  if (No_wifi && !AP_set) {
    Serial.println("No_wifi is true, calling Set_Wifi_AP();");
    Set_Wifi_AP();
  }

  if (AP_set && !No_wifi) {
    if (millis() - previousMillis >= Scan_wifi_delay) {
      previousMillis = millis();
      Serial.println("Currently on AP but 60s passed, doing a Search_wifi().");
      if (!Ongoing_wifi_scan)Start_wifi_search();
    }
    if (Ongoing_wifi_scan && (WiFi.scanComplete() >= 0)) {
      //WiFi.scanComplete():If scan is not done, then returned value is < 0 as follows:
      //* Scanning still in progress: -1
      //* Scanning has not been triggered: -2
      Check_wifi_list();

    }

    if (wifi_list_checked && Wifi_available && !Attempted_STA_connect) {
      wifi_list_checked = false;
      Wifi_available = false;
      Attempt_STA_connect();
    }
  }

  if (Servers_state_configured)Keep_server_mode();

  if (Online) {
    /*
      if (!No_Blynk && !Falhei_Blynk_login_flag )BlynkEdgent.run();//V51
      if (!No_Blynk && Falhei_Blynk_login_flag  && Falhei_Blynk_login_time  < millis()) {
      Falhei_Blynk_login_flag = false;
      Serial.println("Voltar a fazer blynk connect.");
      BlynkState::set(MODE_CONNECTING_CLOUD);
      }
    */
    //if (Blynk_connected)BlynkEdgent.run();//V51
    BlynkEdgent.run();
    if (!No_MQTT)Keep_MQTT();
  }
}

void Start_wifi_search() {
  Serial.println("1Start_wifi_search()");
  //while (WiFi.scanComplete() == -2) {

  if (WiFi.scanComplete() == -2) {    //Teste a ver se em vez de ficar aqui preso até conseguir iniciar o scan, simplesmente tenta iniciar scan.

    WiFi.scanDelete();
    //WiFi.scanComplete() -> On scan completion function returns the number of discovered networks.
    //If scan is not done, then returned value is < 0 as follows:
    //* Scanning still in progress: -1
    //* Scanning has not been triggered: -2
    /*
        Serial.println("2 tentar Start_wifi_search()");
        Wifi_available = false;
        Ongoing_wifi_scan = true;
        wifi_list_checked = false;
        Serial.println("Scan started!");
    */
    int n = WiFi.scanNetworks(true); //WiFi.scanNetworks(true) -> true para async ou false/default para esperar aqui.

    //Teste a ver se em vez de ficar aqui preso até conseguir iniciar o scan, simplesmente tenta iniciar scan.
    if (WiFi.scanComplete() == -1) {
      Serial.println("Conseguiu Start_wifi_search()!");

      Wifi_available = false;
      Ongoing_wifi_scan = true;
      wifi_list_checked = false;
    } else Serial.println("Falhou Start_wifi_search()");

    //  Serial.printf("passei o WiFi.scanNetworks(true);? : %i\n", n);
  }
}

void Check_wifi_list() {
  //Serial.println("Entrei em Check_wifi_list()");
  static int n = -5;

  n = WiFi.scanComplete();

  if (n >= 0) {
    Ongoing_wifi_scan = false;

    // Serial.print("Scan done -> ");
    if (n == 0)Serial.println("No networks found.");
    else {
      //Serial.printf("%i networks found!\n", n);
      for (int i = 0; i < n; ++i) {
        //Serial.printf("%d: %s, Ch:%d (%ddBm)\n", i + 1, WiFi.SSID(i).c_str(), WiFi.channel(i), WiFi.RSSI(i));
        if ((String(WiFi.SSID(i)) == String(Wifi_ssid))) {
          Serial.print("Given ssid found in wifi list:"); Serial.println(String(WiFi.SSID(i)));
          Wifi_available = true;
        } else {
          // Serial.print(String(WiFi.SSID(i))); Serial.print("!=");Serial.println(Wifi_ssid);
        }
      }
      if (!Wifi_available) Serial.println("Given ssid not found in wifi list.");
    }
    WiFi.scanDelete();
    wifi_list_checked = true;
  } else {
    Serial.print("n in Check_wifi_list is <0 | ");
  }
}

void Set_Wifi_AP() {
  Serial.println("Set_Wifi_AP()");

  Serial.println("Entrei em Set_Wifi_AP();");
  Servers_state_configured = false;
  // ftpSrv.end();

  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(Device_name, AP_pass);//AP_ssid
  //ID_AP_con = WiFi.onEvent(WiFi_connected_AP, WiFiEvent_t::ARDUINO_EVENT_WIFI_AP_START);

  IPAddress Ip(10, 10, 10, 10); //setto IP Access Point same as gateway
  IPAddress NMask(255, 255, 255, 0);
  WiFi.softAPConfig(Ip, Ip, NMask);
  IP = WiFi.softAPIP();
  Serial.print("IP:"); Serial.println(IP);

  //Display
  xSemaphoreTake(mutex, portMAX_DELAY);

  tft.setTextSize(2);
  tft.setTextColor(BLACK); tft.setCursor(115, 3); tft.print("W");
  tft.setTextColor(DARKGREY); tft.setCursor(115, 3); tft.print("A");
  //

  //Display
  tft.setTextSize(2);
  tft.setTextColor(GREEN); tft.setCursor(115, 3); tft.print("A");

  tft.setTextColor(BLACK);
  //tft.setCursor(4, 5); tft.print("   ---   ");
  drawCentreString("   ---   ", 56, 5, 9, 2);
  tft.setTextColor(WHITE);

  char sub_Device_name[10];
  memcpy(sub_Device_name, Device_name, 9);
  sub_Device_name[9] = '\0';
  drawCentreString(sub_Device_name, 56, 5, strlen(sub_Device_name), 2);//AP_ssid

  //tft.setCursor(4, 5); tft.print(AP_ssid);
  tft.setTextSize(1);
  drawCentreString(WiFi.softAPIP().toString(), 56, 26, WiFi.softAPIP().toString().length(), 1);
  //tft.setCursor(4, 26); tft.print(IP);
  //
  xSemaphoreGive(mutex);

  AP_set = true;
  Attempted_STA_connect = false;
  Wifi_available = false;

  if (sd_exist) {
    //FTP
    ftpSrv.setCallback(_callback);
    ftpSrv.setTransferCallback(_transferCallback);
    ftpSrv.begin("Chamber_EB", "Chamber_EB", true);//True para AP, false para STA .
    //
  }

  //OTA
  /*OTA*/
  server_OTA.on("/test", HTTP_GET, []() {
    server_OTA.sendHeader("Connection", "close");
    server_OTA.send(200, "text/plain", "The Chamber server is working.");
  });
  ESP2SOTA.begin(&server_OTA);
  server_OTA.begin();
  //http://192.168.4.1/myurl
  //http://192.168.4.1/update
  //
  /**/
  Servers_state_configured = true;
  //
}

void Attempt_STA_connect() {
  Serial.println("Attempt_STA_connect();");
  //Crio evento de obter IP:
  /*
    ID_STA_GOTIP = WiFi.onEvent(WiFiGotIP_STA, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
    Serial.printf("Criei evento ID_STA_GOTIP:%i\n", ID_STA_GOTIP);
  */
  // ID_STA_disc = WiFi.onEvent(WiFi_disconnected_STA, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
  //  Serial.printf("1Created WiFi disconnected event:%i \n", ID_STA_disc);
  WiFi.setSleep(WIFI_PS_NONE);
  WiFi.begin(Wifi_ssid, Wifi_pass);//ssid, password
  Attempted_STA_connect = true;

  Instant_Tried_wifi_sta_connect = millis();
  Serial.print("Instant_Tried_wifi_sta_connect:"); Serial.println(Instant_Tried_wifi_sta_connect);
}

void Keep_server_mode() {
  // esp_task_wdt_reset();//Ainda nao implementei o rtc.
  if (sd_exist) {
    ftpSrv.handleFTP();//Handle do FTP server.
    if (Global_Profile_received) {
      Serial.println("Global_Profile_received TRUE");
      Did_Profile_changed(SD_MMC, "/Profile.txt");
      Global_Profile_received = false;
    }
    if (Global_Settings_received) {
      Serial.println("Global_Settings_received TRUE");
      Global_Settings_received = false;


      Serial.println("Settings received!");
      Turn_all_off(true);
      //Display
      xSemaphoreTake(mutex, portMAX_DELAY);

      tft.fillScreen(BLACK);
      tft.setTextColor(WHITE);
      tft.setTextSize(3);
      drawCentreString("Received new Settings.txt", 240, 140, 25, 3);
      drawCentreString("Rebooting...", 240, 180, 12, 3);

      xSemaphoreGive(mutex);

      delay(3000);
      ESP.restart();

    }


    //Serial.println(ftpSrv.handleFTP());
  }

  server_OTA.handleClient();//Handle do OTA website/server.
}

void Keep_MQTT() {
  /*
    client.state():
    -4 : MQTT_CONNECTION_TIMEOUT - the server didn't respond within the keepalive time
    -3 : MQTT_CONNECTION_LOST - the network connection was broken
    -2 : MQTT_CONNECT_FAILED - the network connection failed
    -1 : MQTT_DISCONNECTED - the client is disconnected cleanly
    0 : MQTT_CONNECTED - the client is connected
    1 : MQTT_CONNECT_BAD_PROTOCOL - the server doesn't support the requested version of MQTT
    2 : MQTT_CONNECT_BAD_CLIENT_ID - the server rejected the client identifier
    3 : MQTT_CONNECT_UNAVAILABLE - the server was unable to accept the connection
    4 : MQTT_CONNECT_BAD_CREDENTIALS - the username/password were rejected
    5 : MQTT_CONNECT_UNAUTHORIZED - the client was not authorized to connect
  */
  /*
    if (!client.connected()) {
    Serial.println("MQTT not connected.");
    reconnect();//MQTT
    }
  */

  if (client.connected()) {
    //Serial.printf("2-client.state:%i\n",client.state());
    if (MQTT_disconnected) {
      //Serial.printf("3-client.state:%i\n", client.state());
      xSemaphoreTake(mutex, portMAX_DELAY);
      tft.setTextSize(2);
      tft.setTextColor(GREEN); tft.setCursor(115, 23); tft.print("M");
      xSemaphoreGive(mutex);
      Serial.println("MQTT is connected again!Subscribing to topics once more!");
      client.subscribe("esp32/LED");
      client.subscribe(Treatment);

      MQTT_disconnected = false;
    }
    client.loop();
  } else if (!MQTT_disconnected) {
    xSemaphoreTake(mutex, portMAX_DELAY);
    tft.setTextSize(2);
    tft.setTextColor(DARKGREY); tft.setCursor(115, 23); tft.print("M");
    xSemaphoreGive(mutex);
    Serial.println("MQTT disconnected!");
    MQTT_disconnected = true;
  }

}
/*
  void reconnect() {

  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    //if (client.connect("esp32")) {//Sem credenciais necessarias ao mqtt (anonymous).
  //   if (client.connect("Esp32", mqtt_username, mqtt_password)) {//Para HIveMQ.
  if (client.connect("Esp32")) {
      Serial.println("Connected to MQTT server.");
      // Subscribe
      client.subscribe("esp32/LED");
      client.subscribe("CIIMAR_Tests");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
  }
*/
void MQTT_callback(char* topic, byte* message, unsigned int length) {
  if (sd_exist) {
    //Serial.print("Message arrived on topic: ");
    //Serial.println(topic);
    //Serial.print(". Message: ");
    String messageTemp;

    for (int i = 0; i < length; i++) {
      //Serial.print((char)message[i]);
      messageTemp += (char)message[i];
    }
    //Serial.println();



    //Filtrar mensagem recebida por topicos:
    //esp32/LED

    if (String(topic) == "esp32/LED") {
      if (messageTemp == "on") {
        Serial.println("on");
        // digitalWrite(ledPin, HIGH);
      }
      else if (messageTemp == "off") {
        Serial.println("off");
        //digitalWrite(ledPin, LOW);
      }
    }

    //esp32/Profile
    //if (String(topic) == "CIIMAR_Tests") {
    if (String(topic) == String(Treatment)) {
      static int lines_count = 0;
      //Receber em char:
      char messageBuffer[30];
      memcpy(messageBuffer, message, length);  //copy the payload to a buffer
      messageBuffer[length] = '\0';  //terminate with a zero to convert to a C style string
      //Serial.print("messageBuffer:"); Serial.println(messageBuffer);
      //Serial.print("strlen(messageBuffer):"); Serial.println(strlen(messageBuffer));

      if ((strcmp(messageBuffer, "END") != 0) && (strlen(messageBuffer) == 20)) //==0 iguais, !=0 diferentes.
      {
        char buf1[11] = {messageBuffer[0], messageBuffer[1], messageBuffer[2], messageBuffer[3], messageBuffer[4], messageBuffer[5], messageBuffer[6], messageBuffer[7], messageBuffer[8], messageBuffer[9], '\0'};
        //profileTimes[lines_count] = atol(buf1);
        //Serial.printf("Time_Step %i:%i ", i, profileTimes[i]);

        char buf2[8] = {messageBuffer[11], messageBuffer[12], messageBuffer[13], messageBuffer[14], messageBuffer[15], messageBuffer[16], messageBuffer[17], '\0'};
        //profileTTL[lines_count] = atol(buf2);
        //Serial.printf("TTL_Step %i:%i \n", i, profileTTL[i]);

        char buf3[2] = {messageBuffer[18], '\0'};

        lines_count++;
        Serial.println(lines_count);

        if (!Receiving_Profile) {
          lines_count = 1;
          Receiving_Profile = true;
          writeFile(SD_MMC, "/Profile.txt", messageBuffer,1);
        } else appendFile(SD_MMC, "/Profile.txt", messageBuffer, 0);
      } else if (strcmp(messageBuffer, "END") == 0) {
        Serial.printf("Recebi END e li %i linhas.\n", lines_count);
        Receiving_Profile = false;
        Did_Profile_changed(SD_MMC, "/Profile.txt");//New_Profile();//Falta trazer esta função mas requer bibliotecas e funcoes do LCD.
      }
      //
    }
  } else Serial.println("Recebi MQTT mas estou sem SD card.");
}

void WiFiGotIP_STA(WiFiEvent_t event, WiFiEventInfo_t info)
{
  /*
    WiFi.removeEvent(ID_STA_GOTIP);
    Serial.printf("Desligar evento ID_STA_GOTIP:%i\n", ID_STA_GOTIP);
  */
  xSemaphoreTake(mutex, portMAX_DELAY);

  Serial.println("Got IP address event!");



  Serial.println(IPAddress(info.got_ip.ip_info.ip.addr));
  IP = WiFi.localIP();
  /*
    ID_STA_disc = WiFi.onEvent(WiFi_disconnected_STA, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
    Serial.printf("Criei evento ID_STA_disc:%i\n", ID_STA_disc);
    ID_STA_LOST_IP = WiFi.onEvent(WiFi_Lost_IP, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_LOST_IP);
    Serial.printf("Criei evento ID_STA_LOST_IP:%i\n", ID_STA_LOST_IP);
  */
  if (Ping.ping("www.google.com", 3)) {//Teste enviar 3 pings apena para ver se impede RTCWDT_RTC_RESET no ping em fraca rede.


    Serial.println("Ping was a success!");
    Online = true;

    //Display
    tft.setTextSize(2);
    if (AP_set) {
      tft.setTextColor(BLACK);
      tft.setCursor(115, 3); tft.print("A");
      char sub_Device_name[10];
      memcpy(sub_Device_name, Device_name, 9);
      sub_Device_name[9] = '\0';
      drawCentreString(sub_Device_name, 56, 5, strlen(sub_Device_name), 2);//AP_ssid
      tft.setTextSize(1);
      drawCentreString(WiFi.softAPIP().toString(), 56, 26, WiFi.softAPIP().toString().length(), 1);
      //tft.setCursor(4, 5); tft.print(AP_ssid);
    } else {
      tft.setTextColor(BLACK);
      tft.setCursor(115, 3); tft.print("W");
      drawCentreString("   ---   ", 56, 5, 9, 2);
      // tft.setCursor(4, 5); tft.print("   ---   ");
    }
    tft.setTextSize(2);
    tft.setTextColor(GREEN); tft.setCursor(115, 3); tft.print("W");
    tft.setTextColor(WHITE);

    char sub_Wifi_ssid[10];
    memcpy(sub_Wifi_ssid, Wifi_ssid, 9);
    sub_Wifi_ssid[9] = '\0';
    drawCentreString(sub_Wifi_ssid, 56, 5, strlen(sub_Wifi_ssid), 2);

    // Serial.print("strlen(ssid):"); Serial.println(strlen(ssid));


    // tft.print(ssid);
    tft.setTextSize(1);
    drawCentreString(WiFi.localIP().toString(), 56, 26, WiFi.localIP().toString().length(), 1);
    //tft.setCursor(4, 26); tft.print(IP);
    //

    Servers_state_configured = false;
    // ftpSrv.end();
    // WiFi.softAPdisconnect (true);
    AP_set = false;
    Attempted_STA_connect = false;


    if (sd_exist) {
      //FTP
      ftpSrv.setCallback(_callback);
      ftpSrv.setTransferCallback(_transferCallback);
      ftpSrv.begin("Chamber_EB", "Chamber_EB", false);//True para AP, false para STA .
      //
    }


    //OTA
    server_OTA.on("/test", HTTP_GET, []() {
      server_OTA.sendHeader("Connection", "close");
      server_OTA.send(200, "text/plain", "The Chamber server is working.");
    });
    ESP2SOTA.begin(&server_OTA);
    server_OTA.begin();

    //http://192.168.4.1/myurl
    //http://192.168.4.1/update
    //
    /**/
    Servers_state_configured = true;
    //

    //Blynk startup:
    BlynkEdgent.begin();
    //

  } else {
    Serial.println("Ping failed.");

  }
  xSemaphoreGive(mutex);
}
void WiFi_Lost_IP(WiFiEvent_t event, WiFiEventInfo_t info)
{
  Serial.println("Lost IP!");
}
void WiFi_disconnected_STA(WiFiEvent_t event, WiFiEventInfo_t info)
{
  /*
    WiFi.removeEvent(ID_STA_disc);
    Serial.printf("Desliguei evento ID_STA_disc:%i\n", ID_STA_disc);
  */

  NTP_Time_adjusted = false; //Para forçar o NTP update quando a net voltar.
  xSemaphoreTake(mutex, portMAX_DELAY);

  Serial.println("WiFi disconnected event!");
  Online = false;
  Fst_MQTT_attempt = true;
  Blynk_connected_display = false;
  Blynk_connected = false;
  if (!AP_set) {
    //Display
    tft.setTextSize(2);
    tft.setTextColor(DARKGREY); tft.setCursor(115, 3); tft.print("W");
    tft.setTextColor(DARKGREY); tft.setCursor(145, 3); tft.print("B");
    tft.setTextColor(DARKGREY); tft.setCursor(115, 23); tft.print("M");

    tft.setTextColor(BLACK);
    char sub_Wifi_ssid[10];
    memcpy(sub_Wifi_ssid, Wifi_ssid, 9);
    sub_Wifi_ssid[9] = '\0';
    drawCentreString(sub_Wifi_ssid, 56, 5, strlen(sub_Wifi_ssid), 2);

    tft.setTextSize(1);
    drawCentreString(IP.toString(), 56, 26, IP.toString().length(), 1);

    tft.setTextSize(2);
    tft.setTextColor(WHITE);
    drawCentreString("   ---   ", 56, 5, 9, 2);

    //tft.setCursor(4, 26); tft.print(IP);
    //
  }

  xSemaphoreGive(mutex);

  /*
    //Teste v35:
    bool stop_ = WiFi.Meu_espWiFiStop();
    Serial.print("stop_:"); Serial.println(stop_);

    //esp_wifi_disconnect();
    //esp_wifi_stop();
    //esp_wifi_deinit();
    bool mode_ = WiFi.mode(WIFI_AP_STA);
    Serial.print("Mode after disconnect:"); Serial.println(mode_);
    //esp_wifi_init();
  */
  //
  Serial.println("Final - WiFi disconnected event!");

}
