void Configure_temp_sensors() {
  Air_Temp_Sensor_1.begin();
  Air_Temp_Sensor_1.getAddress(Air_Temp_1_Address, 0);
  Air_Temp_Sensor_1.setResolution(Air_Temp_1_Address, 12);
  Air_Temp_Sensor_1.setWaitForConversion(false);

  Air_Temp_Sensor_2.begin();
  Air_Temp_Sensor_2.getAddress(Air_Temp_2_Address, 0);
  Air_Temp_Sensor_2.setResolution(Air_Temp_2_Address, 12);
  Air_Temp_Sensor_2.setWaitForConversion(false);

  Air_Temp_Sensor_3.begin();
  Air_Temp_Sensor_3.getAddress(Air_Temp_3_Address, 0);
  Air_Temp_Sensor_3.setResolution(Air_Temp_3_Address, 12);
  Air_Temp_Sensor_3.setWaitForConversion(false);

  Room_Temp_Sensor.begin();
  Room_Temp_Sensor.getAddress(Room_Temp_Address, 0);
  Room_Temp_Sensor.setResolution(Room_Temp_Address, 12);
  Room_Temp_Sensor.setWaitForConversion(false);

  Water_Temp_Sensor_1.begin();
  Water_Temp_Sensor_1.getAddress(Water_Temp_1_Address, 0);
  Water_Temp_Sensor_1.setResolution(Water_Temp_1_Address, 12);
  Water_Temp_Sensor_1.setWaitForConversion(false);

  Water_Temp_Sensor_2.begin();
  Water_Temp_Sensor_2.getAddress(Water_Temp_2_Address, 0);
  Water_Temp_Sensor_2.setResolution(Water_Temp_2_Address, 12);
  Water_Temp_Sensor_2.setWaitForConversion(false);
}

void Request_Air_Temperatures() {

  //Serial.println("");
  //Serial.println("Request Air Temperature.");
  Air_Temp_Sensor_1.requestTemperatures();

  Air_Temp_Sensor_2.requestTemperatures();

  Air_Temp_Sensor_3.requestTemperatures();

  Room_Temp_Sensor.requestTemperatures();

  Air_Temp_requested = true;
  //Serial.println("Requested Air temperatures.");
}
void Request_Water_Temperatures() {
  //Serial.println("");
  //Serial.println("Request Water Temperature.");
  Water_Temp_Sensor_1.requestTemperatures();
  Water_Temp_Sensor_2.requestTemperatures();
  Water_Temp_requested = true;
  //Serial.println("Requested Water temperatures.");
}
void Get_Air_Temperatures() {

  for (int i = 0; i < 3; i++) {
    //Serial.println(i);
    Air_Temp_1 = Air_Temp_Sensor_1.getTempCByIndex(0);
    if (Air_Temp_1 > -126)break;
  }
  for (int i = 0; i < 3; i++) {
    //Serial.println(i);
    Air_Temp_2 = Air_Temp_Sensor_2.getTempCByIndex(0);
    if (Air_Temp_2 > -126)break;
  }

  for (int i = 0; i < 3; i++) {
    //Serial.println(i);
    Room_Temp = Room_Temp_Sensor.getTempCByIndex(0);
    if (Room_Temp > -126)break;
  }

  for (int i = 0; i < 3; i++) {
    //Serial.println(i);
    Air_Temp_3 = Air_Temp_Sensor_3.getTempCByIndex(0);
    if (Air_Temp_3 > -126)break;
  }

  Air_Temp_1_ok = true;
  Air_Temp_2_ok = true;
  Air_Temp_3_ok = true;
  Room_Temp_ok = true;

  int divider = 0;

  if (Air_Temp_1 < -126 || Air_Temp_1 > 84) {
    Air_Temp_1_ok = false;
    Air_Temp_1 = 0;
  } else divider++;

  if (Air_Temp_2 < -126 || Air_Temp_2 > 84) {
    Air_Temp_2_ok = false;
    Air_Temp_2 = 0;
  } else divider++;

  if (Air_Temp_3 < -126 || Air_Temp_3 > 84) {
    Air_Temp_3_ok = false;
    Air_Temp_3 = 0;
  } else divider++;

  if (Room_Temp < -126 || Room_Temp > 84) {
    Room_Temp_ok = false;
    Room_Temp = 0;
  }


  if (divider == 0)Air_Temp_sensors_functional = false; else Air_Temp_sensors_functional = true;

  if (!Air_Temp_1_ok && !Air_Temp_2_ok && !Air_Temp_3_ok) {
    Air_Temp_sensors_functional = false;
  }

  if (Air_Temp_sensors_functional) Air_Input_Temp_Avg = (Air_Temp_1 + Air_Temp_2 + Air_Temp_3) / divider;
  else Air_Input_Temp_Avg = 0;


  Air_Temp_requested = false;
}

void Get_Water_Temperatures() {

  for (int i = 0; i < 3; i++) {
    //Serial.println(i);
    Water_Temp_1 = Water_Temp_Sensor_1.getTempCByIndex(0);
    if (Water_Temp_1 > -126)break;
  }
  for (int i = 0; i < 3; i++) {
    //Serial.println(i);
    Water_Temp_2 = Water_Temp_Sensor_2.getTempCByIndex(0);
    if (Water_Temp_2 > -126)break;
  }

  Water_Temp_1_ok = true;
  Water_Temp_2_ok = true;
  int divider = 0;

  if (Water_Temp_1 < -126 || Water_Temp_1 > 84) {
    Water_Temp_1_ok = false;
    Water_Temp_1 = 0;
  } else divider++;

  if (Water_Temp_2 < -126 || Water_Temp_2 > 84) {
    Water_Temp_2_ok = false;
    Water_Temp_2 = 0;
  } else divider++;

  if (divider == 0) {
    Water_Temp_sensors_functional = false;
    //Serial.println("0Water_Temp_sensors_functional = false;");
  } else Water_Temp_sensors_functional = true;

  if (!Water_Temp_1_ok && !Water_Temp_2_ok) {
    //Serial.println("1Water_Temp_sensors_functional = false;");
    Water_Temp_sensors_functional = false;
  }

  if (Water_Temp_sensors_functional) Water_Input_Temp_Avg = (Water_Temp_1 + Water_Temp_2 ) / divider;
  else Water_Input_Temp_Avg = 0;

  Water_Temp_requested = false;
}
