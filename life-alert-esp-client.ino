#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
String weekDays[7]={"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
String months[12]={"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

const char* ssid = "RobotZone";
const char* password = "RobotZone1";
const char* serverName = "http://192.168.1.106:1880/update-sensor";

//unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
//unsigned long timerDelay = 5000;

//Init button to be used for ON/OFF override swtich
const int on_off_switch = 4;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  timeClient.begin();
  timeClient.setTimeOffset(-21600);
}

void loop() {
   timeClient.update();
   //Time related variables here
   unsigned long epochTime = timeClient.getEpochTime();
   String formattedTime = timeClient.getFormattedTime();
   int currentHour = timeClient.getHours();
   int currentMinute = timeClient.getMinutes();
   int currentSecond = timeClient.getSeconds();
   String weekDay = weekDays[timeClient.getDay()];
   struct tm *ptm = gmtime ((time_t *)&epochTime);
   int monthDay = ptm->tm_mday;
   int currentMonth = ptm->tm_mon+1;
   String currentMonthName = months[currentMonth-1];
   int currentYear = ptm->tm_year+1900;
   String currentDate = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay);
  //Not sure which state is default -> could be LOW or HIGH
  //If switch is ON then continue logic to record 'events'
  if(digitalRead(on_off_switch) == HIGH){

    //Logic for event goes here

    
  }









  

    if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;
      http.begin(serverName);
      http.addHeader("Content-Type", "application/json");      
      int httpResponseCode = http.POST("{\"api_key\":\"tPmAT5Ab3j7F9\",\"sensor\":\"BME280\",\"value1\":\"24.25\",\"value2\":\"49.54\",\"value3\":\"1005.14\"}");
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      http.end();
    }
  }



  /* 
   Metrics to Track/Display 
   DAILY
   - Total events for the day
   WEEKLY
   - Total events for the week
   - Day with most events

   MONTHLY
   -Total events for the month
   -Day with the most events
   -Week with the most events



  
  */