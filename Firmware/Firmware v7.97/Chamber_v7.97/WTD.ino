void Set_RTC_Watchdog(int segundos) {

    // Set up the watchdog timer
  esp_task_wdt_config_t wdt_config = {
    .timeout_ms = segundos*1000, // Set timeout in milliseconds
  };

  esp_task_wdt_init(&wdt_config);  // Pass the config object

  //O esp32S3 estava assim antes do board update do v2 para v3:
  //esp_task_wdt_init(segundos, true); //enable panic so ESP32 restarts
  //esp_task_wdt_add(NULL); //add current thread to WDT watch

  /*
    rtc_wdt_protect_off();      //Disable RTC WDT write protection
    rtc_wdt_set_stage(RTC_WDT_STAGE0, RTC_WDT_STAGE_ACTION_RESET_RTC);
    rtc_wdt_set_time(RTC_WDT_STAGE0, segundos * 1000);
    rtc_wdt_enable();           //Start the RTC WDT timer
    rtc_wdt_protect_on();       //Enable RTC WDT write protection
  */
}
