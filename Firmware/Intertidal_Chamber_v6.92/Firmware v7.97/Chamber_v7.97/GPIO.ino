void Toggle_GPIO(int Pin, Power_state estado) {
  xSemaphoreTake(i2c_mutex, portMAX_DELAY);
  switch (Pin) {
    case 7:
      if (Prev_Enter_WV_state != estado) {
        Prev_Enter_WV_state = estado;
        if (estado == On) {
          mcp_20.digitalWrite(7, HIGH);
          Serial.printf("Turned Enter_WV %s.\n", Power_label[estado]);
        }
        if (estado == Off) {
          mcp_20.digitalWrite(7, LOW);
          Serial.printf("Turned Enter_WV %s.\n", Power_label[estado]);
        }
      }
      break;

    case 8:
      if (Prev_Heat_Lamp_state != estado) {
        Prev_Heat_Lamp_state = estado;
        if (estado == On) {
          mcp_20.digitalWrite(8, HIGH);
          Serial.printf("Turned Heat_Lamp %s.\n", Power_label[estado]);
        }
        if (estado == Off) {
          mcp_20.digitalWrite(8, LOW);
          Serial.printf("Turned Heat_Lamp %s.\n", Power_label[estado]);
        }
      }
      break;

    case 9:
      if (Prev_Water_Cooler_state != estado) {
        Prev_Water_Cooler_state = estado;
        if (estado == On) {
          mcp_20.digitalWrite(9, HIGH);
          Serial.printf("Turned Water_Cooler %s.\n", Power_label[estado]);
          //18/12/2024 - 1_Chamber_v7.96_T1
          //delay(5000);
          //
          
          //06/01/2025 - 1_Chamber_v7.96_T3
          //delay(2000);
          //

         //06/01/2025 - 1_Chamber_v7.96_T3
          delay(500);
          //
        }
        if (estado == Off) {
          mcp_20.digitalWrite(9, LOW);
          Serial.printf("Turned Water_Cooler %s.\n", Power_label[estado]);
          //18/12/2024 - 1_Chamber_v7.96_T1
          //delay(5000);
          //
          
          //06/01/2025 - 1_Chamber_v7.96_T3
          //delay(2000);
          //

          //06/01/2025 - 1_Chamber_v7.96_T3
          delay(500);
          //
        }
      }
      break;

    case 10:
      if (Prev_Water_Heater_state != estado) {
        Prev_Water_Heater_state = estado;
        if (estado == On) {
          mcp_20.digitalWrite(10, HIGH);
          Serial.printf("Turned Water_Heater %s.\n", Power_label[estado]);
        }
        if (estado == Off) {
          mcp_20.digitalWrite(10, LOW);
          Serial.printf("Turned Water_Heater %s.\n", Power_label[estado]);
        }
      }
      break;

    case 11:
      if (Prev_Cooler_Pump_state != estado) {
        Prev_Cooler_Pump_state = estado;
        if (estado == On) {
          mcp_20.digitalWrite(11, HIGH);
          Serial.printf("Turned Cooler_Pump %s.\n", Power_label[estado]);
        }
        if (estado == Off) {
          mcp_20.digitalWrite(11, LOW);
          Serial.printf("Turned Cooler_Pump %s.\n", Power_label[estado]);
        }
      }
      break;

    case 12:
      if (Prev_Tide_Pump_state != estado) {
        Prev_Tide_Pump_state = estado;
        if (estado == On) {
          mcp_20.digitalWrite(12, HIGH);
          Serial.printf("Turned Tide_Pump %s.\n", Power_label[estado]);
        }
        if (estado == Off) {
          mcp_20.digitalWrite(12, LOW);
          Serial.printf("Turned Tide_Pump %s.\n", Power_label[estado]);
        }
      }
      break;

    case 13:
      if (Prev_Circle_FAN_state != estado) {
        Prev_Circle_FAN_state = estado;
        if (estado == On) {
          mcp_20.digitalWrite(13, HIGH);
          Serial.printf("Turned Circle_FAN %s.\n", Power_label[estado]);
        }
        if (estado == Off) {
          mcp_20.digitalWrite(13, LOW);
          Serial.printf("Turned Circle_FAN %s.\n", Power_label[estado]);
        }
      }
      break;

    case 14:
      if (Prev_Cool_FAN_state != estado) {
        Prev_Cool_FAN_state = estado;
        if (estado == On) {
          mcp_20.digitalWrite(14, HIGH);
          Serial.printf("Turned Cool_FAN %s.\n", Power_label[estado]);
        }
        if (estado == Off) {
          mcp_20.digitalWrite(14, LOW);
          Serial.printf("Turned Cool_FAN %s.\n", Power_label[estado]);
        }
      }
      break;

    case 15:
      if (Prev_Release_WV_state != estado) {
        Prev_Release_WV_state = estado;
        if (estado == On) {
          mcp_20.digitalWrite(15, HIGH);
          Serial.printf("Turned Release_WV %s.\n", Power_label[estado]);
        }
        if (estado == Off) {
          mcp_20.digitalWrite(15, LOW);
          Serial.printf("Turned Release_WV %s.\n", Power_label[estado]);
        }
      }
      break;
  }
  xSemaphoreGive(i2c_mutex);
}
