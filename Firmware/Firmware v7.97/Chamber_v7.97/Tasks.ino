
void MQTT_Connect_Task_1() {
  static byte Falhou_mqtt = 0;
  if (Online && !client.connected() && !No_MQTT) {
    //Serial.printf("0-client.state:%i\n", client.state());
    client.disconnect();
    //Serial.printf("1-client.state:%i\n", client.state());
    if (Fst_MQTT_attempt)Fst_MQTT_attempt = false;
    Serial.println("MQTT_Connect_Task_1 - client.connect()");
    Serial.print("Treatment:"); Serial.println(Treatment);
    //Teste a ver se o nome random para com o falhanço a ligar ao mqtt.
    String clientId = "mqtt"; // Create a random client ID
    clientId += String(random(0xffff), HEX);

    Serial.printf("clientId:%s\n", clientId);

    bool Mqtt_ligou = client.connect(clientId.c_str(), MQTT_ID, MQTT_pass); //Estava assim!10/03/2023
    if (Mqtt_ligou) {
      Serial.println("MQTT_LIGOU");
      Falhou_mqtt = 0;
    } else {
      Serial.printf("Falhou 4-client.state:%i\n", client.state());
      Falhou_mqtt++;
      if (Falhou_mqtt > 10) {
        Serial.println("Falhei MQTT mais de 10x, fazer reset wifi!");
        Falhou_mqtt = 0;
        //bool disconnect(bool wifioff = false, bool eraseap = false);
        WiFi.disconnect(true, true);

      }
    }
  }
}


/*
  // MQTT task running on Core 0
  void WIFI_TASK_1(void * pvParameters) {
  for (;;) {

    if (Online && !client.connected()) {
      client.connect("esp32", "Luis", "Luis_pass");
    }
    vTaskDelay(1000);//vTaskDelay permite que outras threads no mesmo core continuem.
    //(ver https://www.dobitaobyte.com.br/esp32-delay-vtaskdelay-vtaskdelayuntil-millis/)
  }
  }

  /*
  // MQTT task running on Core 0
  //void MQTT_Connect_Task_1(void * pvParameters) {

  //Opcoes para evitar watchdog trigger em tasks:
  //Link com discussão: https://github.com/espressif/arduino-esp32/issues/595
  //1-vTaskDelay.
  //2-Baixar prioridade da task que estiver a consumir stack.
  //3- // TIMERG0.wdt_wprotect=TIMG_WDT_WKEY_VALUE;
  // TIMERG0.wdt_feed=1;
  // TIMERG0.wdt_wprotect=0;
  //4- //disableCore0WDT();
  //


  // String clientId = "ESP32Client - "; // Create a random client ID
  // clientId += String(random(0xffff), HEX);


  for (;;) {
    //Serial.println("Task 1");

    if (Online && !client.connected()) {
      //Adicionei o Ping.ping(remote_host) para ver se evita o erro:" E (1022944) task_wdt: Task watchdog got triggered."
      //  if (Ping.ping(remote_host)) {
      Serial.println("MQTT_Connect_Task_1 - client.connect()");

  //    Serial.print("0 MQTT_Connect_Task_1:");
   //   Serial.println(uxTaskGetStackHighWaterMark( NULL ));//Ou imprimir diretamente a função.

      //client.connect("test", mqtt_username, mqtt_password);
      Serial.print("Treatment:"); Serial.println(Treatment);
      client.connect(Treatment, MQTT_ID, MQTT_pass);
      //client.connect("esp32", "Luis", "Luis_pass");


      //}
    }

  //  Serial.print("1 MQTT_Connect_Task_1:");
  //  Serial.println(uxTaskGetStackHighWaterMark( NULL ));//Ou imprimir diretamente a função.

    vTaskDelay(10000);//vTaskDelay permite que outras threads no mesmo core continuem.
    //(ver https://www.dobitaobyte.com.br/esp32-delay-vtaskdelay-vtaskdelayuntil-millis/)
  }
  }
*/
/*
  // MQTT task running on Core 0
  void WIFI_TASK_1(void * pvParameters) {
  for (;;) {

    if (Online && !client.connected()) {
      client.connect("esp32", "Luis", "Luis_pass");
    }
    vTaskDelay(1000);//vTaskDelay permite que outras threads no mesmo core continuem.
    //(ver https://www.dobitaobyte.com.br/esp32-delay-vtaskdelay-vtaskdelayuntil-millis/)
  }
  }
*/
void Update_Time_Task_2(void * pvParameters) {
  TickType_t xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount();
  for ( ;; )
  {
    xSemaphoreTake(i2c_mutex, portMAX_DELAY);

    /*
      Serial.print("0 Update_Time_Task_2: ");
      Serial.println(uxTaskGetStackHighWaterMark( NULL ));//Ou imprimir diretamente a função.
    */

    static int Prev_minute = 0;
    static int Prev_day = 0;

    // if (Outrtc_working && Outrtc.updateTime() && RTC_Pos_2022) {
    if (Outrtc_working && Outrtc.updateTime()) {
      //nowTimeUnix = Outrtc.getEpoch();
      nowTimeUnix = Outrtc.getEpoch() + (int)(UTC * 3600) + (int)(DST * 3600);

      if (minute(nowTimeUnix) != Prev_minute) {
        Prev_minute = minute(nowTimeUnix);
        minuto_mudou = true;
        //Serial.println("minuto_mudou");
      }

      if (day(nowTimeUnix) != Prev_day) {
        Prev_day = day(nowTimeUnix);
        dia_mudou = true;
        //Serial.println("dia_mudou");
      }

    } else Serial.println("Task Core 0: Update_Time_Task_2(): RTC failed to update time variables.");
    /*
        Serial.print("1 Update_Time_Task_2: ");
        Serial.println(uxTaskGetStackHighWaterMark( NULL ));//Ou imprimir diretamente a função.
    */
    xSemaphoreGive(i2c_mutex);
    vTaskDelayUntil( &xLastWakeTime, ( 1000 / portTICK_PERIOD_MS ) );
  }
}

void Ping_Task_3(void * pvParameters) {
  for (;;) {
    /*
      Serial.print("0 Ping_Task_3: ");
      Serial.println(uxTaskGetStackHighWaterMark( NULL ));//Ou imprimir diretamente a função.
    */
    //Serial.println("Task 3");
    if (Online) {
      // Serial.println("Task 3__");
      if (!Ping.ping(remote_host, 3)) {
        Serial.println("Falhou ping!");
        WiFi.disconnect();
      } //else Serial.println("Online!");
    }
    /*
        Serial.print("1 Ping_Task_3: ");
        Serial.println(uxTaskGetStackHighWaterMark( NULL ));//Ou imprimir diretamente a função.
    */
    vTaskDelay(10000);
  }
}

void Get_NTP_Time_Task_4(void * pvParameters) {
  static unsigned long NTP_request_instant = 0;
  static bool NTP_requested = false;
  time_t now;
  struct tm timeinfo = {};
  byte counter = 0;
  for (;;) {
    /*
      Serial.print("0 Get_NTP_Time_Task_4: ");
      Serial.println(uxTaskGetStackHighWaterMark( NULL ));//Ou imprimir diretamente a função.
    */
    if (Online && !NTP_Time_adjusted && !NTP_requested) {
      Serial.println("NTP_requested in Get_NTP_Time_Task_4()");
      configTime(0, 0, ntpServer);
      getLocalTime( &timeinfo);
      NTP_request_instant = millis();
      NTP_requested = true;
    }

    if (Online && !NTP_Time_adjusted && NTP_requested && (millis() > (NTP_request_instant + 60000))) {
      Serial.println("60seconds passed since NTP_request, retrieving NTP_epochTime:");
      NTP_requested = false;
      Serial.println(1);
      time(&now);//O que que isto faz?!
      Serial.println(2);

      NTP_epochTime = now;
      Serial.println(3);

      Get_Time_Task_finished = true;
      Serial.println(4);

      vTaskSuspend(NULL);
      Serial.println(5);

    }
    /*
      Serial.print("1 Get_NTP_Time_Task_4: ");
      Serial.println(uxTaskGetStackHighWaterMark( NULL ));//Ou imprimir diretamente a função.
    */
    vTaskDelay(10000);
  }//else Serial.println("NOT Online && !NTP_Time_adjusted");

}

void Ler_ADCs_Task_5( void * pvParameters )
{
  static byte Disc_cnt[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  static byte LOW_cnt[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  static byte HIGH_cnt[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  static int leituras_atual[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  //static int state[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  static int Prev_state[8] = { -1, -1, -1, -1, -1, -1, -1, -1};

  for ( ;; )
  {
    xSemaphoreTake(i2c_mutex, portMAX_DELAY);

    ADS7828::updateAll();
    //Serial.println(Canal_1->value());
    //Serial.println(smooth());
    for (byte i = 0; i < 8; i++) {
      leituras_atual[i] = Canal_1[i]->value();

      if ((leituras_atual[i] < 500)) {
        if (state[i] != 1)LOW_cnt[i]++;
        else {
          if (HIGH_cnt[i] > 0)HIGH_cnt[i]--;
          if (Disc_cnt[i] > 0)Disc_cnt[i] = 0;
        }
      }
      else if ((leituras_atual[i] > 3406) ) {
        if (state[i] != 3) HIGH_cnt[i]++;
        else {
          if (LOW_cnt[i] > 0)LOW_cnt[i]--;
          if (Disc_cnt[i] > 0)Disc_cnt[i] = 0;
        }

      }
      else if ((leituras_atual[i] >= 500) && (leituras_atual[i] <= 3406) ) {
        if (state[i] != 0) Disc_cnt[i]++;
        else {
          if (LOW_cnt[i] > 0)LOW_cnt[i]--;
          if (HIGH_cnt[i] > 0)HIGH_cnt[i]--;
        }
        Disc_cnt[i]++;
      //  Serial.printf("Disc_cnt[i]++ : %d \n", Disc_cnt[i]);
      }

      if (LOW_cnt[i] > 5 && state[i] != 1) {
        state[i] = 1;
        LOW_cnt[i] = 0;
        HIGH_cnt[i] = 0;
        Disc_cnt[i] = 0;
        //Serial.println("1 - Disc_cnt[i] zerou");
      }
      if (HIGH_cnt[i] > 5 && state[i] != 3) {
        state[i] = 3;
        LOW_cnt[i] = 0;
        HIGH_cnt[i] = 0;
        Disc_cnt[i] = 0;
        //  Serial.println("3 - Disc_cnt[i] zerou");
      }
      if (Disc_cnt[i] > 250 && state[i] != 0) {
        state[i] = 0;
        LOW_cnt[i] = 0;
        HIGH_cnt[i] = 0;
        Disc_cnt[i] = 0;
        //  Serial.println("0 - Disc_cnt[i] zerou");
      }

      if (Prev_state[i] != state[i]) {
        Prev_state[i] = state[i];
        /*
                if (state[i] == 0) Serial.printf("Sensor %d Disconnected \n", i + 1);
                if (state[i] == 1) Serial.printf("Sensor %d LOW\n", i + 1);
                if (state[i] == 2) Serial.printf("Sensor %d HIGH\n", i + 1);
        */
      }

      //50ms funciona bem com 20 cnt.

      //100ms funciona bem com 10 cnt leva 5 segundos a detetar mudança de estado.
      //100ms com 5 cnt deteta disconnected no intermedio.
      //200ms funciona bem com 10 cnt leva 6 segundos a detetar mudança de estado.(MELHOR!!!)
      //300ms funciona bem com 10 cnt leva 9 segundos a detetar mudança de estado.
      //300ms funciona bem com 5 cnt leva ?? segundos a detetar mudança de estado.
      //200ms com 5 cnt deteta disconnected no intermedio.

      //Leituras com PCB CHamber Back v1.2:
      //Sem sensor:2717
      //Com sensor LOW:3
      //Com sensor HIGH:4095

    }

/*
    Serial.printf("%d %d %d %d %d %d %d %d \n", leituras_atual[0],
                  leituras_atual[1], leituras_atual[2],
                  leituras_atual[3], leituras_atual[4],
                  leituras_atual[5], leituras_atual[6],
                  leituras_atual[7]);
*/

    xSemaphoreGive(i2c_mutex);
    vTaskDelay(200);


  }

}

void Ligar_Blynk_Task_6( void * pvParameters )
{
  for ( ;; )
  { BlynkState::set(MODE_CONNECTING_CLOUD);
    //Blynk.config(configStore.cloudToken, configStore.cloudHost, configStore.cloudPort);//Estava assim->Permite provisioning de credenciasi pela app 'Blynk IoT'.
    Blynk.config(auth, configStore.cloudHost, configStore.cloudPort);
    Blynk.connect(0);

    unsigned long timeoutMs = millis() + WIFI_CLOUD_CONNECT_TIMEOUT;
    while ((timeoutMs > millis()) && (!Blynk.isTokenInvalid()) && (Blynk.connected() == false))
      //if((!Blynk.isTokenInvalid()) && (Blynk.connected() == false))
    {
      for (int i = 0; i < 300; i++) {
        //Ultima_Tentativa=millis();//V51
        //Serial.println("Tentativa login Blynk");
        delay(10);
        Blynk.run();
        app_loop();
        if (!BlynkState::is(MODE_CONNECTING_CLOUD)) {
          Serial.println("MEU:Blynk got to Blynk.disconnect();");

          Blynk.disconnect();
          return;
        }
      }
    }

    if (millis() > timeoutMs) {
      Serial.println("Blynk got to Timeout");

      DEBUG_PRINT("Timeout");
    }


    if (Blynk.isTokenInvalid()) {
      Serial.println("Blynk.isTokenInvalid()");
      //    config_set_last_error(BLYNK_PROV_ERR_TOKEN);
      //  BlynkState::set(MODE_WAIT_CONFIG);
    } else if (Blynk.connected()) {
      Serial.println("MEU: Blynk.connected()");
      BlynkState::set(MODE_RUNNING);

      if (!configStore.getFlag(CONFIG_FLAG_VALID)) {
        configStore.last_error = BLYNK_PROV_ERR_NONE;
        configStore.setFlag(CONFIG_FLAG_VALID, true);
        config_save();
      }

    } else {
      Serial.println("Blynk Token is valid() but did not connect");
      // config_set_last_error(BLYNK_PROV_ERR_CLOUD);//V51
      //  BlynkState::set(MODE_ERROR);//V51
    }
    vTaskDelay(30000);
  }

}
