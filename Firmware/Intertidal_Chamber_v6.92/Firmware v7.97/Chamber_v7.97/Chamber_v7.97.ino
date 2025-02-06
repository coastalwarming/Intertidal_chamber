char Version[6] = "V7.97";
const int ledChannel = 0;
byte Blynk_ronda = 1;

//Na V3.8 acrescentei um 100nF entre reset e GND.
bool Fst_Blynk = true;
unsigned long Ultima_Tentativa = 0;
volatile bool MQTT_disconnected = true;
volatile bool Blynk_connected = false;
bool Top_sensors_func = false, Bot_Bot_sensors_func = false;
bool WS_incomplete = true;
//ADC
#include <i2c_adc_ads7828.h>
ADS7828 device(0, SINGLE_ENDED | REFERENCE_ON | ADC_ON, 0xFF);
ADS7828* adc = &device;
ADS7828Channel* Canal_1[8] = {adc->channel(0), adc->channel(1), adc->channel(2), adc->channel(3), adc->channel(4), adc->channel(5), adc->channel(6), adc->channel(7)};
int state[8] = {0, 0, 0, 0, 0, 0, 0, 0};
void Turn_all_off(bool Override_Manual_SW);
int Prev_Plot_TPA_Projected_target_Step_Index = -1;
int Prev_Prof_TPA_Projected_target_Step_Index = -1;
bool WEx_sw_ativated = false, Manual_Now_WEx = false;
volatile byte cnt_profile = 0;
volatile byte cnt_plot = 0;
byte TPA_state = 0;
#include <esp_wifi.h>//Para usar esp_wifi_disconnect(); .
volatile unsigned long Falhei_Blynk_login_time = 0;
volatile bool Falhei_Blynk_login_flag = false;
/*Libraries*/
SemaphoreHandle_t mutex;
SemaphoreHandle_t i2c_mutex;

//Watchdog libraries:
//#include "soc/rtc_wdt.h"//Tem que estar antes do BlynkEdgent.h porque o rtc_wdt_feed é evocado la dentro.
#include <esp_task_wdt.h>
//
#include <Update.h>
volatile int KILL = 0;
bool Was_KILL = false;
//TFT
#define TFT_CS 10
#define TFT_DC 19
#define TFT_RST -1 // RST can be set to -1 if you tie it to Arduino's reset
#define DKBLUE    0x000D
#define BLUE      0x001F
#define RED       0xF800
#define WHITE     0xFFFF
#define BLACK     0x0000
#define HX8357_BLACK 0x0000   ///< BLACK color for drawing graphics
#define HX8357_BLUE 0x001F    ///< BLUE color for drawing graphics
#define HX8357_RED 0xF800     ///< RED color for drawing graphics
#define HX8357_GREEN 0x07E0   ///< GREEN color for drawing graphics
#define HX8357_CYAN 0x07FF    ///< CYAN color for drawing graphics
#define HX8357_MAGENTA 0xF81F ///< MAGENTA color for drawing graphics
#define HX8357_YELLOW 0xFFE0  ///< YELLOW color for drawing graphics
#define HX8357_WHITE 0xFFFF   ///< WHITE color for drawing graphics
#define BLACK 0x0000       ///<   0,   0,   0
#define NAVY 0x000F        ///<   0,   0, 123
#define DARKGREEN 0x03E0   ///<   0, 125,   0
#define DARKCYAN 0x03EF    ///<   0, 125, 123
#define MAROON 0x7800      ///< 123,   0,   0
#define PURPLE 0x780F      ///< 123,   0, 123
#define OLIVE 0x7BE0       ///< 123, 125,   0
#define LIGHTGREY 0xC618   ///< 198, 195, 198
#define DARKGREY 0x7BEF    ///< 123, 125, 123
#define BLUE 0x001F        ///<   0,   0, 255
#define GREEN 0x07E0       ///<   0, 255,   0
#define CYAN 0x07FF        ///<   0, 255, 255
#define RED 0xF800         ///< 255,   0,   0
#define MAGENTA 0xF81F     ///< 255,   0, 255
#define YELLOW 0xFFE0      ///< 255, 255,   0
#define WHITE 0xFFFF       ///< 255, 255, 255
#define ORANGE 0xFD20      ///< 255, 165,   0
#define GREENYELLOW 0xAFE5 ///< 173, 255,  41
#define PINK 0xFC18        ///< 255, 130, 198


#include "Adafruit_GFX.h"    // Core graphics library
#include <SPI.h>
#include <Adafruit_HX8357.h>
//
//TFT
Adafruit_HX8357 tft = Adafruit_HX8357(TFT_CS, TFT_DC);
boolean new_plot = false;
double ox , oy ;
double y;
int x;
int Max_Temp = 60, Min_Temp = 0;
void drawCentreString(const String &buf, int x, int y, int caracteres = 0, int lettersize = 3);

bool Global_Profile_received = false, Global_Settings_received = false;
//
//WIFI
#include <nvs_flash.h>
#include <SimpleFTPServer.h>//FTP server.
#include <WiFi.h>
#include <ESP32Ping.h>
#include <WiFiAP.h>//Para HiveMQ.
#include <WebServer.h>//OTA por webpage.
#include <ESP2SOTA.h>//OTA por webpage.
#include <PubSubClient.h>//MQTT.
#include <WiFiClientSecure.h>//MQTT Secure para HiveMQ.
//#include <WiFiClient.h>//MQTT Secure para outros brokers que nao o HiveMQ - Ja é importado nas tabs do blynk.

char BLYNK_TEMPLATE_ID_[13];
char BLYNK_DEVICE_NAME_[35];//O BLYNK_DEVICE_NAME_ parece ser necessário mas irrelevante. Talvez seja visivel no modo de configuração via app de telemovel.
//O BLYNK_TEMPLATE_ID_ identifica o template que ira receber os dados.
//o auth permite acesso a conta de blynk
char auth[40] = "";//e9HjaqMDUICV_QexrlnGtW9yaEvRfdc5
#define BLYNK_TEMPLATE_ID "Chamber"
//O BLYNK_TEMPLATE_ID aqui definido não é usado para mandar dados online (esse é o BLYNK_TEMPLATE_ID_).
//Mas este define tem de ser igual entre todos os firmwares que se tente enviar remotamente.
//Portanto convem manter sempre um BLYNK_TEMPLATE_ID defined basico em todos os firmwares da chamber.

//Se nao definir BLYNK_TEMPLATE_ID, mesmo que em branco, surge este erro:error: 'BLYNK_FIRMWARE_TYPE' was not declared in this scope BLYNK_FIRMWARE_TYPE,
//Apenas o BLYNK_TEMPLATE_ID aqui definido permite fazer OTA. Posso sempre fazer OTA onde mudo este BLYNK_TEMPLATE_ID .
//#define BLYNK_DEVICE_NAME "Chamber"//Nao preciso definir BLYNK_DEVICE_NAME, basta a variavel BLYNK_DEVICE_NAME_ que vem no cartao e esta na EEProm.
//char auth[35] = "e9HjaqMDUICV_QexrlnGtW9yaEvRfdc5";
#define BLYNK_PRINT Serial
#define BLYNK_FIRMWARE_VERSION "0.0.1"
#include <Preferences.h>//Eeprom access.
Preferences preferences;
Preferences preferences_PID;

const char* remote_host = "www.google.com";
volatile bool  Online = false;
volatile bool Blynk_connected_display = false;
bool Attempted_STA_connect = false, Wifi_available = false, AP_set = false; //Necessario antes de BlynkEdgent.h.
#include "BlynkEdgent.h"//Online IOT cloud.
//

//Temperature libraries:
#include <OneWireNg.h>
#include <DallasTemperature.h>
//

//SD card:
#include <Wire.h>
//#include <SPI.h>//Already inserted.
#include "FS.h"
#include "SD_MMC.h"
//#include <SD.h>
//
//MCP23008:
#include <Adafruit_MCP23X17.h>
//

/**/

/*Global variables:*/
//Temperature sensors:
const byte Sensor_1_Pin = 4;
const byte Sensor_2_Pin = 6;
const byte Sensor_3_Pin = 16;
const byte Sensor_4_Pin = 5;
const byte Sensor_5_Pin = 15;
const byte Sensor_6_Pin = 7;

OneWire oneWire_Sensor_1(Sensor_1_Pin);
OneWire oneWire_Sensor_2(Sensor_2_Pin);
OneWire oneWire_Sensor_3(Sensor_3_Pin);
OneWire oneWire_Sensor_4(Sensor_4_Pin);
OneWire oneWire_Sensor_5(Sensor_5_Pin);
OneWire oneWire_Sensor_6(Sensor_6_Pin);


DallasTemperature Air_Temp_Sensor_1 (&oneWire_Sensor_1);
DallasTemperature Air_Temp_Sensor_2 (&oneWire_Sensor_2);
DallasTemperature Air_Temp_Sensor_3 (&oneWire_Sensor_3);
DallasTemperature Water_Temp_Sensor_1 (&oneWire_Sensor_4);
DallasTemperature Water_Temp_Sensor_2 (&oneWire_Sensor_5);
DallasTemperature Room_Temp_Sensor (&oneWire_Sensor_6);

DeviceAddress Air_Temp_1_Address;
DeviceAddress Air_Temp_2_Address;
DeviceAddress Air_Temp_3_Address;
DeviceAddress Water_Temp_1_Address;
DeviceAddress Water_Temp_2_Address;
DeviceAddress Room_Temp_Address;

volatile float Temp_Setpoint = 0, Air_Input_Temp_Avg = 0, Air_Temp_1 = 0, Air_Temp_2 = 0, Air_Temp_3 = 0,
               Water_Temp_Setpoint = 0, Water_Input_Temp_Avg = 0, Water_Temp_1 = 0, Water_Temp_2 = 0, Room_Temp = 0,
               Top_Gap = 0, Bottom_Gap = 0, Water_Heater_delta = 0.15, Water_Cooler_delta = 0.15;
bool Air_Temp_1_ok = true, Air_Temp_2_ok = true, Air_Temp_3_ok = true, Room_Temp_ok = true,
     Water_Temp_1_ok = true, Water_Temp_2_ok = true,  Air_Temp_requested = false,
     Water_Temp_requested = false, Air_Temp_sensors_functional = false, Water_Temp_sensors_functional = false;
int  Temp_Set_Direction = 0;
unsigned long  Time_Cooler_powerOFF = 0;
double Temp_Air[240] = {0}, Prev_Temp_Air[240] = {0};
byte tempo = 0;
//
//Heating/cooling states:
enum Temp_state {
  Idle,
  Heating,
  Cooling,
  In,
  Out,
  Halt,
  none_
};
volatile Temp_state Water_temp_state = Idle;

enum Power_state {
  On,
  Off,
  none
};
volatile Power_state Prev_Heat_Lamp_state = none, Heat_Lamp_state = none,
                     Prev_Water_Heater_state = none, Water_Heater_state = none,
                     Prev_Water_Cooler_state = none, Water_Cooler_state = none,
                     Prev_Tide_Pump_state = none, Tide_Pump_state = none,
                     Prev_Cooler_Pump_state = none, Cooler_Pump_state = none,
                     Prev_Circle_FAN_state = none, Circle_FAN_state = none,
                     Prev_Cool_FAN_state = none, Cool_FAN_state = none,
                     Prev_Release_WV_state = none, Release_WV_state = none,
                     Prev_Enter_WV_state = none, Enter_WV_state = none;

const char* Power_label[3] = {"On", "Off", "none"};

bool Wait_low_tide = false;
//

//Water sensors:
byte WS_1 = 2, WS_2 = 2, WS_3 = 2, WS_4 = 2, WS_5 = 2, WS_6 = 2;
bool Water_top_detected = false, Water_BOt_top_detected = false, Water_BOt_mid_detected = false, Water_BOt_bot_detected = false, Top_sensors = true, Bottom_sensors = true;
//

//Timing:
long unsigned int Water_start_release_instant = 0, Water_start_fill_instant = 0;
unsigned int  Water_release_period = 0, Water_fill_period = 30;
unsigned long previousMillis = 0, Instant_Tried_wifi_sta_connect = 0,
              previousMillis_100ms = 0, previousMillis_1seg = 0, previousMillis_2seg = 0;
const long interval_search_wifi = 1000 * 60;
byte segundos = 0;
bool Ja_sei_next_Water_Setpoint = false;
bool Acao_2s = false, Acao_4s = false, Acao_10s = false;
float UTC = 0;
bool DST = false;
long unsigned int FAN_instant = 0;
//


//WiFi:
bool No_wifi = true, No_MQTT = false, No_Blynk = false;

const char* Wifi_ssid;
const char* Wifi_pass;
//const char* ssid     = "Meuwifi_2";
//const char* password = "passdowifi";
const long unsigned int Scan_wifi_delay = 30000 * 2;
bool Servers_state_configured = false;
//const char* AP_ssid = "Chamber";
const char* AP_pass = "";//"Chamber12345";
IPAddress IP;
IPAddress AP_IP(10, 10, 10, 10);
IPAddress gateway(10, 10, 10, 10);
IPAddress subnet(255, 255, 255, 0);
volatile WiFiEventId_t ID_STA_disc, ID_AP_con, ID_STA_GOTIP, ID_STA_LOST_IP;
volatile bool Ongoing_wifi_scan = false, wifi_list_checked = false;
void WiFi_connected_AP(WiFiEvent_t event, WiFiEventInfo_t info) {
  Serial.println("WiFi connected as AP event!");
  IP = WiFi.softAPIP();
}
bool reset_ = false;
//

//Servers(FTP and HTTP for OTA webpage):
FtpServer ftpSrv;
WebServer server_OTA(80);
//

//SD card:
bool Reset = false, sd_exist = false, sd_back = false, Profile_changed = false, read_settings = false;
char File_name[35];
char Device_name[13] = {"Chamber"};
char MQTT_Host_temp[60], MQTT_ID_temp[20], MQTT_pass_temp[20], AP_pass_temp[21], Treatment[20] = {"Treatment"}, Treatment_temp[20], Device_name_temp[13], Wifi_ssid_temp[15], Wifi_pass_temp[31], NTP_TIMEZONE_[50];
//
bool Fst_MQTT_attempt = true;
//Client to MQTT server:
//Mosquitto
//const char* mqtt_server = "188.83.9.101";
//const int mqtt_port = 1883;
//HiveMQ
//char* MQTT_Host = "ea8c328d98194b26a4395ab3f42fd0ac.s2.eu.hivemq.cloud";
//char* MQTT_ID = "LuisPereira";
//char* MQTT_pass = "6hvZAGJyLhM4wg#";
char* MQTT_Host = "";
char* MQTT_ID = "";
char* MQTT_pass = "";
const int mqtt_port = 8883;
//Mosca/Aedes
//const char* mqtt_server = "188.83.9.101";
//const int mqtt_port = 1883;
//const char* mqtt_username = "Luis";
//const char* mqtt_password = "Luis_pass";
//
//WiFiClient espClient;
//WiFiClient espClient;//Para outros que nao o HiveMQTT.
WiFiClientSecure espClient;//Para o HiveMQTT.
PubSubClient client(espClient);

//#include <Arduino_FreeRTOS.h>
#include "freertos/FreeRTOS.h"
TaskHandle_t MQTT_Connect_Task_1_Handle;
TaskHandle_t Update_Time_Task_2_Handle;
TaskHandle_t Ping_Task_3_Handle;
TaskHandle_t Get_NTP_Time_Task_4_Handle;
TaskHandle_t Ler_ADCs_Task_5_Handle;
TaskHandle_t Ligar_Blynk_Task_6_Handle;
//Interrupções:
void WiFiGotIP_STA(WiFiEvent_t event, WiFiEventInfo_t info);
void WiFi_disconnected_STA(WiFiEvent_t event, WiFiEventInfo_t info);

/*ESTE CERTIFICADO NAO TEM QUE MUDAR,E NECESSARIO PARA AO HIVEMQ MAS BASTA MUDAR O HOST, USER E PASSWORD E DA PARA TODOS*/
static const char *root_ca PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4
WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu
ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY
MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc
h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+
0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U
A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW
T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH
B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC
B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv
KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn
OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn
jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw
qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI
rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV
HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq
hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL
ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ
3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK
NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5
ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur
TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC
jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc
oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq
4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA
mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d
emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=
-----END CERTIFICATE-----
)EOF";

//Now_Tide:
//0=Low Tide
//1=HighTide



//RTC:
#include <SparkFun_RV3032.h>//External clock.
#include <TimeLib.h>//Convercao de epoch em horas e minutos.
#include "time.h"//NTP Time.
#include <ESP32Time.h>//Internal clock;
//
bool Fst_derivative = true;
float Previous_Error = 0;
//Profile variables:
char End[18] = {}, Start[18] = {};
double PID_Kp = 0, PID_Ki = 0, PID_Kd = 0;
float Error_1 = 0, Error_Integral_1 = 0, Error_Derivative_1 = 0;
double Kp = 0, Ki = 0 , Kd = 0;//14_Kp2_ki_0018
double Temp_SetPoints[240][3] = {0}, Prev_Temp_SetPoint[240] = {0};
float PID_Output_1 = 0;
byte PID_RATE_SECS = 10;
unsigned int profileTimes[1002];
unsigned int profileTTL[1002];
bool profileTPA[5002];
//volatile long unsigned profileTimes[1002];
//volatile long unsigned profileTTL[1002];
//volatile int unsigned profileTPA[1002];
volatile long unsigned No_water_instant=0, Tide_change_instant = 0, nowTimes[2], Next_tide_change_moment = 0, Proj_nowTimes[2];
volatile double nowTemps[2], m, b, Projected_Temp_Setpoint = 0, Proj_nowTemps[2];
bool Receiving_Profile = false, Got_a_new_Profile = false, Profile_ok = false, Calculated_Setpoints = false;
int profileLength = 0, Projected_target_Step_Index = 0;
int Now_WEx=0, Now_Tide  = 0, Prev_Tide = 0, target_Step_Index = 0, Last_WEx_TimeStep=-1,  Wait_before_air_heat = 120, Wait_Open_Valve=120, Chiller_wait=180;
bool Water_change = false, Valve_open=false;
static const char *Estados[] = { "UnPositioned", "Waiting_to_start", "Ongoing", "Finished", "Unset"};
int Now_Light=0;
const byte LightsPin = 21;
const int Freq_PWM = 200;
const int Res_PWM = 8;

//Permite usar Estados[runStatus] em prints e obter UnPositioned ou outros.
enum Status {
  UnPositioned,
  Waiting_to_start,
  Ongoing,
  Finished, 
  KILL_
};
enum Status runStatus = UnPositioned;
Status Check_runStatus();
bool No_more_tide_change = false, Waiting_for_cooler = false;
//

//RTC variables:
RV3032 Outrtc;//RTC externo.
ESP32Time Inrtc;//Para ter timestamp nos files tenho de ter o rtc interno ajustado.
bool Outrtc_working = false, RTC_Pos_2022 = false, NTP_Time_adjusted=false, minuto_mudou = false, dia_mudou = false;
volatile unsigned long int nowTimeUnix = 0, NTP_epochTime = 0;
long  gmtOffset_sec = 0;
int   daylightOffset_sec = 0;
const char* ntpServer = "pool.ntp.org";
//

//MCP23008:
Adafruit_MCP23X17  mcp_20;
Adafruit_MCP23X17  mcp_21;

//

//Switches_e_interrupcoes:
volatile bool Int_dispultada_1 = false;
void IRAM_ATTR Ext_INT1_ISR();
enum Switch {
  Auto,
  Manual,
  ON,
  OFF,
  Unknown
};
enum Switch Main_SW_State = Unknown;
enum Switch Prev_Main_SW_State = Unknown;
enum Switch A_Heat_sw_state = Unknown, W_Heat_sw_state = Unknown, Cool_sw_state = Unknown,
            Pump_sw_state = Unknown, Tide_sw_state = Unknown, Light_sw_state = Unknown, Fan_sw_state = Unknown, WEx_sw_state= Unknown;


//
bool WEx_ongoing_F=false;
bool Get_Time_Task_finished=true;
bool Get_NTP_Time_Task_4_resumed=false;
/**/
void Send_Reset_info() {
  if (!reset_ && (BlynkState::get() == MODE_RUNNING)) {
    Blynk.virtualWrite(1, "Reset\n");
     Blynk.virtualWrite(2, 1);
    String ino = __FILE__;
    Serial.println(ino);

    ino = (ino.substring( (ino.indexOf(".")), (ino.lastIndexOf("\\")) + 1) );//

    Blynk.virtualWrite(1, Version);

    Blynk.virtualWrite(4, Device_name);
    Blynk.virtualWrite(5, Treatment);
    Blynk.virtualWrite(6, Start);
    Blynk.virtualWrite(7, End);



   // Serial.printf("Sent Reset, Device_name and Treatment.\n");
    reset_ = true;
    Blynk_ronda++;
  }
}

void Retrieve_PID_Eeprom();

void Say_on_Serial_which_firmware_I_have () {
  String the_path = __FILE__;
  int slash_loc = the_path.lastIndexOf('/');
  String the_cpp_name = the_path.substring(slash_loc + 1);
  int dot_loc = the_cpp_name.lastIndexOf('.');
  String the_sketchname = the_cpp_name.substring(0, dot_loc);

  Serial.print("\nArduino is running Sketch: ");
  Serial.println(the_sketchname);
  Serial.print("Compiled on: ");
  Serial.print(__DATE__);
  Serial.print(" at ");
  Serial.print(__TIME__);
  Serial.print("\n");
}

void setup() {

  // Create mutex before starting tasks
  mutex = xSemaphoreCreateMutex();
  i2c_mutex = xSemaphoreCreateMutex();

  //Serial:
  Serial.begin(115200);
  delay(100);
  Serial.println("Inicio setup().");
  Say_on_Serial_which_firmware_I_have();
  //


  
  //EEprom
  Retrieve_PID_Eeprom();
  //

  //Watchdog:
  //Set_RTC_Watchdog(60);//Seconds.
  //

  //TFT:
  tft_init();
  tft_main_frame();
  tft_initial_tags();
  //

//Luz
  ledcAttach(LightsPin,Freq_PWM,Res_PWM); //ledcAttach(uint8_tpin,uint32_tfreq,uint8_tresolution); for ESP core V3

  //ledcSetup(ledChannel, 1000, 8);//ledChannel, freq, resolution
  //ledcAttachPin(LightsPin, ledChannel);
  
  //MCP's:
  Wire.begin();
  Wire.setClock(10000);
  //Serial.println(Wire.getClock());
  Config_mcp_outputs();
  Config_mcp_switches();
  //Int_dispultada_0 = true;
  Int_dispultada_1 = true;
  //Int_dispultada_2 = true;
  Update_switchs_display();
  
//  bool setClock(uint32_t frequency);
 // bool getClock();


 //

  //DS18b20
  Configure_temp_sensors();
  //
  

  //

  //RTC:
  //Wire.begin();
  start_rtc();
  
  //

  //Wifi:
  //nvs_flash_erase();//Impede wifi.scan de terminar - Nunca se liga ao wifi.
  //WiFi.persistent(false);
  //WiFi.setSleep(false);
  
  WiFi.mode(WIFI_AP_STA);//WIFI_AP_STA//WIFI_STA
  
  //WiFi.disconnect(true,true);
  //WiFi.softAPdisconnect(wifioff)
  //
  


      //NTP_Task
  xTaskCreatePinnedToCore(
    Get_NTP_Time_Task_4,
    "Get_NTP_Time_Task_4",// nome da tarefa 
    5000, // número de palavras a serem alocadas para uso com a pilha da tarefa 
    NULL,//parâmetro de entrada para a tarefa (pode ser NULL) 
    1,// prioridade da tarefa (0 a N) 
    &Get_NTP_Time_Task_4_Handle,// referência para a tarefa (pode ser NULL) 
    0);// Núcleo que executará a tarefa 
  
  vTaskSuspend(Get_NTP_Time_Task_4_Handle);



 //Ping
  xTaskCreatePinnedToCore(
    Ping_Task_3,
    "Ping_Task_3",// nome da tarefa 
    5000, // número de palavras a serem alocadas para uso com a pilha da tarefa 
    NULL,// parâmetro de entrada para a tarefa (pode ser NULL) 
    1,// prioridade da tarefa (0 a N) 
    &Ping_Task_3_Handle,// referência para a tarefa (pode ser NULL) 
    0);// Núcleo que executará a tarefa 
//


//Task 5 ADC
xSemaphoreTake(mutex, portMAX_DELAY);

ADS7828::begin();
  xTaskCreatePinnedToCore(
    Ler_ADCs_Task_5,   // função que implementa a tarefa 
    "Ler_ADCs_Task_5", // nome da tarefa 
    10000,      // número de palavras a serem alocadas para uso com a pilha da tarefa 
    NULL,       // parâmetro de entrada para a tarefa (pode ser    'NULL,'   ou    '(void *)&qualquernome),'    
    2,          // prioridade da tarefa (0 a N) 
    &Ler_ADCs_Task_5_Handle,       // referência para a tarefa (pode ser NULL) 
    0);         // Núcleo que executará a tarefa 
    
   xSemaphoreGive(mutex);
//


  //SD:

  start_sd();
  if (sd_exist){
    updateFromFS(SD_MMC);
    Profile_ok = GetProfile_SD(SD_MMC, "/Profile.txt");
    readFile(SD_MMC, "/Settings.txt");
    readFile_KILL(SD_MMC, "/KILL.txt");
    } else {
      Serial.println("No SD card!");
      }
  read_settings=true;
  Set_Start_End_Name_TFT();
  //



xTaskCreatePinnedToCore(
    Update_Time_Task_2,
    "Update_Time_Task_2",// nome da tarefa 
    5000, // número de palavras a serem alocadas para uso com a pilha da tarefa 
    NULL,// parâmetro de entrada para a tarefa (pode ser NULL) 
    2,// prioridade da tarefa (0 a N) 
    &Update_Time_Task_2_Handle,// referência para a tarefa (pode ser NULL) 
    0);// Núcleo que executará a tarefa 


  //client.setServer(mqtt_server, 1883);
  espClient.setCACert(root_ca);//Para o HiveMQ.
  client.setServer(MQTT_Host, mqtt_port);//Para todos os brokers.
  client.setCallback(MQTT_callback);//Para todos os brokers.
  //
  
  
  //TFT final:
  Plot_empty_graph();
  if (Profile_ok && RTC_Pos_2022) {
    Set_state_n_intial_tide();
    Calculate_Past_n_Future_Temp_Setpoints();
    Plotar_Setpoint(Temp_SetPoints, RED, false, 240);
    Calculated_Setpoints = true;
  }



  Serial.println("Final setup()!");
  Serial.println();


  ID_STA_GOTIP = WiFi.onEvent(WiFiGotIP_STA, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
  ID_STA_disc = WiFi.onEvent(WiFi_disconnected_STA, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);

   Int_dispultada_1 = true;

   tft.setTextSize(2);
    tft.setCursor(245, 117);
    tft.setTextColor(BLUE);
    tft.print("Reading sensors...");
    delay(4000);
    tft.setCursor(245, 117);
    tft.setTextColor(BLACK);
    tft.print("Reading sensors...");

    Int_dispultada_1 = true;
}

/*__________________________________________________________________________*/

void loop() {
  //ledcWrite(LightsPin, 200); //mcp_20.digitalWrite(5, HIGH);

  
  //Warn reset
  Send_Reset_info();
  //



  //esp_task_wdt_reset();
  Update_switchs_display();
  Update_Water_sensors_display();
  Manage_wifi_state_n_display();
  if (minuto_mudou)Update_Time_Display();

  if (Online && !NTP_Time_adjusted) {
   //Serial.println("0-NTP_adjust!");
    NTP_adjust();
  }

if(Fst_MQTT_attempt){ MQTT_Connect_Task_1();}

  if (!Calculated_Setpoints && RTC_Pos_2022) {
    Set_state_n_intial_tide();
    Calculate_Past_n_Future_Temp_Setpoints();
    Plotar_Setpoint(Temp_SetPoints, RED, false, 240);
    Calculated_Setpoints = true;
    Serial.println("1Calculated_Setpoints!");
  }

  unsigned long currentMillis = millis();

  //Corre a cada 100ms
  if ((currentMillis - previousMillis_100ms >= 100) && (runStatus == Ongoing)) {
    previousMillis_100ms = currentMillis;
    if(!KILL && runStatus!=UnPositioned)Apply_Air_Heat();
  }
  //

  //Corre a cada segundo:
  if (currentMillis - previousMillis_1seg >= 1000) {
    previousMillis_1seg = currentMillis;
    segundos++;
     //Serial.println("Task_1s start->");

     //esp_task_wdt_reset();

    if (Profile_ok && RTC_Pos_2022) runStatus = Check_runStatus();

    Write_status_tft();

    if (!Air_Temp_requested)Request_Air_Temperatures();

    if (!Water_Temp_requested)Request_Water_Temperatures();

    if(!KILL && runStatus!=UnPositioned)Apply_Light();

    if(!KILL && runStatus!=UnPositioned)Apply_Pumps();

    if(!KILL && runStatus!=UnPositioned)Apply_Fans();

    if(!KILL && runStatus!=UnPositioned)Apply_WExValves();

    Acao_2s = true;
      //Serial.println("Task_1s end");
  }

  //Corre a cada 2segundos:
  if ( ((segundos % 2) == 0) && segundos > 1 && Acao_2s) {
    //Serial.println("Task_2s start->");
    Acao_2s = false;
    if (Air_Temp_requested)Get_Air_Temperatures();

    if (Water_Temp_requested)Get_Water_Temperatures();

    Update_Temp_Sensors_display();
    //Serial.println("Task_2s end");
    Acao_4s = true;
  }

  //Corre a cada 4segundos:
  if ( ((segundos % 4) == 0) && segundos > 3 && Acao_4s) {
    //Serial.println("Task_4s start->");
    Acao_4s = false;
    Calculate_Temp_Setpoint();
    Calculate_Air_PID();

    if(!KILL && runStatus!=UnPositioned)Apply_Water_Heat();
    //Serial.println("Task_4s end");

    Acao_10s = true;
  }

  if ( ((segundos % 10) == 0) && segundos > 9 && Acao_10s) {
    /*
      Serial.printf("Water_top_detected:%i\n Water_BOt_top_detected:%i\n Water_BOt_mid_detected:%i\n Water_BOt_bot_detected:%i\n", 
  Water_top_detected, 
  Water_BOt_top_detected, 
  Water_BOt_mid_detected, 
  Water_BOt_bot_detected);
*/
  
    //Serial.print("Task_10s start->");
if(!sd_exist) Religar();

if(runStatus==Ongoing || runStatus==Waiting_to_start)Gravar_SD();else SenseSD();
    Postar_online();
  
  
    Acao_10s = false;
    //Serial.println("Task_10s end");

  }

  if (segundos == 90) {

      
    // Serial.print("Task_90s start->");
 //unsigned long pre_mqtt= millis();
    MQTT_Connect_Task_1();
 //Serial.println(millis()-pre_mqtt);
    
    //Atualizar Set point plot:
    if (runStatus != UnPositioned) {
      Rodar_e_Calculate_next_Future_Setpoint();
      Limpar_Plot(Prev_Temp_SetPoint, BLACK, 240);
      Plotar_Setpoint(Temp_SetPoints, RED, true, 240);
    }
    //

    //Atualizar data point plot:
    Rodar_e_acrescentar();//240 = ha 240 pontos de 90segundos nas 6h passadas.
    Limpar_Plot(Prev_Temp_Air, BLACK, 120);
    Plotar_Temp(Temp_Air, GREEN, false, 120);
    //
    segundos = 0;
    //  Serial.println("Task_90s end");
    //Serial.println("___________90s_____________");
  }

  Check_Serial_input();
  
}
