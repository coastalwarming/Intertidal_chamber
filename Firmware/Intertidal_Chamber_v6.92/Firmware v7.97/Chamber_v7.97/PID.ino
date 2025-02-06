void Calculate_Air_PID() {

  // Serial.printf("Calculated new Temp_Setpoint in Calculate_Temp_Setpoint:%.2f .\n", Temp_Setpoint);
  //Aquecer so para manter seco:
  if ((Main_SW_State  == Manual && (A_Heat_sw_state == Auto)) ||
      Main_SW_State  == Auto) {
    //if (Now_Tide == 1 || runStatus != Ongoing)PID_Output_1 = 5;
    if (Now_Tide == 1)PID_Output_1 = 10;//Teste fora de experiecia 
  }

  //HEAT LAMPS:
  if ( (runStatus == Ongoing) &&
       !Now_Tide &&
       Air_Temp_sensors_functional &&
       !Water_top_detected &&
       !Wait_low_tide &&
       Top_sensors_func)
  {

    if ((Main_SW_State  == Manual && (A_Heat_sw_state == Auto)) ||
        Main_SW_State  == Auto) PID_Compute_1(Air_Input_Temp_Avg, Temp_Setpoint);//Atualiza PID_Output_1.
  } else {
    //PID_Output_1 = 0;  //Aquecer so para manter seco:
    //Cool_FAN_state = Off;//Pke desligar as cooling fans quando não posso calcular PID?
    //Serial.println("10 Cool_FAN_state = Off;");
    Error_1 = 0;
    Previous_Error = 0;
    Error_Integral_1 = 0;
    Error_Derivative_1 = 0;
    Fst_derivative = true;

    Heat_Lamp_state = Off;
    Toggle_GPIO(8, Heat_Lamp_state);
  }

  if ((Main_SW_State  == Manual) && (Tide_sw_state == OFF)) {
    Serial.println("On manual tide - no HEAT LAMPS.");

    //PID_Output_1 = 0;  //Aquecer so para manter seco:
    Cool_FAN_state = Off;

    Error_1 = 0;
    Previous_Error = 0;
    Error_Integral_1 = 0;
    Error_Derivative_1 = 0;
    Fst_derivative = true;
    Serial.println("PID:A mare baixa é manual, não vou aquecer!");
  }
  //

}

void PID_Compute_1(float T_Input, float T_Setpoint)
{

  /*
    Ki = (T_Setpoint/40);
    if (T_Setpoint < 30)Ki = 0.5;
    if (T_Setpoint > 40)Ki = 1;
  */



  Error_1 = T_Setpoint - T_Input;

  // unsigned long now = millis();

  //INTEGRAL - Limit to +/-50  or 100 ?
  Error_Integral_1 += Error_1;

  if (Error_Integral_1 > (100 / Ki))Error_Integral_1 = 100 / Ki;
  if (Error_Integral_1 < 0)Error_Integral_1 = 0;

  //DERIVATIVE
  Error_Derivative_1 = (Error_1 - Previous_Error);
  Previous_Error = Error_1;
  //O 1o Error_Derivative_1 da infinito.
  if (Fst_derivative) {
    Error_Derivative_1 = 0; Fst_derivative = false;
  }
  //PID_Output


  // Ki = (0.013 * T_Setpoint + 0.195)/100;


  PID_Kp = Kp * Error_1;
  PID_Ki = Ki * Error_Integral_1;

  PID_Kd = Kd * Error_Derivative_1;
  PID_Output_1 = PID_Kp + PID_Ki + PID_Kd;
  /*
    Serial.printf("Kp:%f\n", Kp);
    Serial.printf("Ki:%f\n", Ki);
    Serial.printf("Kd:%f\n", Kd);
  */
  //PID_Output_1 = (Kp * Error_1) + (Ki * Error_Integral_1) + (Kd * Error_Derivative_1);
  //Serial.printf("Error_1:%f  PID_1:%f = %f + %f + %f\n", Error_1, PID_Output_1, PID_Kp, PID_Ki, PID_Kd);



  if (PID_Output_1 > 100) PID_Output_1 = 100;
  else if (PID_Output_1 < 0) PID_Output_1 = 0;


}
