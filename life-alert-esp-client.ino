#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
String weekDays[7]={"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
String months[12]={"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

const char* ssid = "Zach's Wi-Fi Network";
const char* password = "Jayhawk20";
const char* serverName = "http://192.168.1.106:1880/update-sensor";

//Init button to be used for ON/OFF override swtich
const int on_off_switch = 4;
//Init pin to read the sensor value from
const int soundSensor = A0;
//Set the threshold value that triggers an event
const int eventThreshold = 300;

//Initial delay period to weed out non-events
long delayBuffer = 30000;
long eventBuffer = 60000;
long previousTime = 0;
long previousEventTime = 0;
//Store integer to represent the different modes 
//0 = sentry
//1 = buffer
//2 = recording
int eventMode = 0;
String potentialStartTime;
String potentialEndTime;
  //Times triggered is an int to store the amount of times the threshold value for an event is triggered during an event. AKA how many times did it get very loud
  //Starts at 2 becuase by the time we get to eventMode 2 then the sensor has been triggered 2 times
  int timesTriggered = 2;
  
  //MaxdB is an int to store the peak dB value recorded during an event 
  int maxdB = 0;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {delay(500);Serial.print(".");}
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  timeClient.begin();
  timeClient.setTimeOffset(-21600);
  


 
}

void loop() {
  unsigned long currentTime = millis();
   timeClient.update();
  if(eventMode == 0){ //Sentry Mode
    delay(500);
    if(analogRead(soundSensor) > eventThreshold){
      Serial.println("Sensor triggered, going to event mode 1");
      //If sensor is tripped the first time, set the event mode to buffer status
     eventMode = 1;
     previousTime = currentTime;
    }
    Serial.println("Sensor not triggered");
  }
  if(eventMode == 1){ //Buffer Mode
    delay(500);
    potentialStartTime = timeClient.getFormattedTime(); //May need to record more variations of date/time for POST request to server
       Serial.println("starting buffer period of 30 seconds");
       //If sensor is triggered and the time from start is less than 30 seconds
       if((analogRead(soundSensor) > eventThreshold)&& currentTime-previousTime <= delayBuffer){
        previousTime = currentTime;
        eventMode = 2;
        Serial.println("Going to eventMode 2");
       //Else if the timer expires before another trigger of the sensor then switch back to sentry mode
       }else if(currentTime-previousTime >= delayBuffer){
        eventMode = 0;
        Serial.println("Going back to eventMode 0");
       }
    
  }
  if(eventMode == 2){//Recording Mode
    delay(500);
    Serial.println("Now in event mode 2");
    int sensorVal = analogRead(soundSensor);
    if(sensorVal> eventThreshold && currentTime - previousTime <= eventBuffer){
      Serial.println("triggered in timer period,");
      //Store the potential end time of the event
      potentialEndTime = timeClient.getFormattedTime();
      //Since sensor has been triggered, increase the triggered count
        timesTriggered++;
      // If the current reading of the sensor is greater than the current maxdB, update the maxdB to the current sensorVal
      if(sensorVal > maxdB){
        maxdB = sensorVal;
      }
      //Sensor has been triggered so update the eventBuffer
      Serial.println("Restarting eventMode 2 loop and updating timer");
      previousTime = currentTime;
      }else if(currentTime - previousTime > eventBuffer){
        //reset and post values
        Serial.println("Timer ran out");
        eventMode = 0;
        timesTriggered = 2;
        Serial.println("Going back to eventMode 0");
        Serial.println(potentialStartTime);
        Serial.println("---------");
        Serial.println(potentialEndTime);
        Serial.println("----------");
        Serial.println(timesTriggered);
//      if(WiFi.status()== WL_CONNECTED){
//      HTTPClient http;
//      http.begin(serverName);
//      http.addHeader("Content-Type", "application/json");      
//      int httpResponseCode = http.POST("{\"api_key\":\"tPmAT5Ab3j7F9\",\"sensor\":\"BME280\",\"value1\":\"24.25\",\"value2\":\"49.54\",\"value3\":\"1005.14\"}");
//      Serial.print("HTTP Response code: ");
//      Serial.println(httpResponseCode);
//      http.end();
//    }
      }
    
  }}














  //   unsigned long epochTime = timeClient.getEpochTime();
//   String formattedTime = timeClient.getFormattedTime();
//   int currentHour = timeClient.getHours();
//   int currentMinute = timeClient.getMinutes();
//   int currentSecond = timeClient.getSeconds();
//   String weekDay = weekDays[timeClient.getDay()];
//   struct tm *ptm = gmtime ((time_t *)&epochTime);
//   int monthDay = ptm->tm_mday;
//   int currentMonth = ptm->tm_mon+1;
//   String currentMonthName = months[currentMonth-1];
//   int currentYear = ptm->tm_year+1900;
//   String currentDate = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay);
