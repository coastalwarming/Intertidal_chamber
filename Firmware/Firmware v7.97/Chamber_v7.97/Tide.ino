bool Quando_sera_proxima_tide_change() {
  for (int i = target_Step_Index; i < profileLength - 1; i++) {
    byte Next_Tide = ((profileTTL[i] / 1000)  % 10);
    //Serial.print("TIDE profileTTL[i]:"); Serial.println(profileTTL[i]);
    //Serial.print("TIDE profileTTL[i]/ 10:"); Serial.println((profileTTL[i] / 10));
    //Serial.print("TIDE profileTTL[i]/ 10)  mod 10:"); Serial.println(((profileTTL[i] / 10)  % 10));

    //    Serial.print("TIDE profileTTL[i]/ 1000:"); Serial.println((profileTTL[i] / 1000));
    // Serial.print("TIDE profileTTL[i]/ 1000)  mod 10:"); Serial.println(((profileTTL[i] / 1000)  % 10));

    if (Now_Tide != Next_Tide) {
      Serial.printf("Tide will change from %i to %i at %02i:%02i\n", Now_Tide, Next_Tide, hour(profileTimes[i]), minute(profileTimes[i]));
      Next_tide_change_moment = profileTimes[i];
      //Serial.println(hour(Next_tide_change_moment));
      //Serial.println(minute(Next_tide_change_moment));
      //printf("Date1: %4d-%02d-%02d %02d:%02d:%02d\n", year(t_unix_date1), month(t_unix_date1), day(t_unix_date1), hour(t_unix_date1), minute(t_unix_date1), second(t_unix_date1));

      return true;
    }
  }
  return false;
}

bool Houve_mare_cheia_10min() {
  Serial.println("Houve_mare_cheia_10min()");

  //Estou em mare vazia e e este é o 1o passo:
  if (!Now_Tide && target_Step_Index == 0) {
    Serial.println("Houve_mare_cheia_10min() : Estou no passso 0");
    return false;
  }
  //Se não estou no primeiro passo, verifico se a ultima mare cheia foi ha menos de 10 minutos:
  for (int i = target_Step_Index; i >= 0; i--) {

    byte Prev__Tide = ((profileTTL[i] / 1000)  % 10);

    Serial.print("For target_Step_Index: "); Serial.println(i);
    Serial.println("Prev__Tide: "); Serial.println(Prev__Tide);
    Serial.println("profileTimes[i]: "); Serial.println(profileTimes[i]);
    if (Prev__Tide) Serial.printf("High tide %i seconds ago.\n", nowTimeUnix - profileTimes[i]);

    if (Prev__Tide &&
        ((nowTimeUnix - profileTimes[i]) < 600)) {
      Serial.printf("Houve mare cheia ha %i segundos, por isso espero.\n", nowTimeUnix - profileTimes[i]);
      return true;
    }
  }

  Serial.println("Não houve mare cheia a menos de 10 minutos.");

  return false;
}
