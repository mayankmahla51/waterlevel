#include "ESP8266WiFi.h" 
#include <FirebaseESP8266.h> 
#include <time.h>
#include <Wire.h>  // This library is already built in to the Arduino IDE
#include <LiquidCrystal_I2C.h> //This library you can add via Include Library > Manage Library > 

FirebaseData fbdo;
 
// Set these to run example.
//#define FIREBASE_HOST "water-level-alert-default-rtdb.firebaseio.com"
//#define FIREBASE_AUTH "3CPW1R3leqftxAGkegguFz9KtttIJCgH0gRhVkgu"
#define FIREBASE_HOST "waterlevelsensor-1474f-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "BrkHGzoGNZxSbBXzub1NLLogoiAF8MzNVicKV9tT"
#define WIFI_SSID "MAYANK"
#define WIFI_PASSWORD "45454545"
int sensorHeight= 50;       // Verticle height of sensor from bottom in cm
int timezone = 5.5 * 3600;  // indian time +5:30
int dst = 0;
const int trigPin = D5; 
const int echoPin = D6; 

int tUpdate=0;
int updateInterval=10;
int updated[6]={1, 1, 1, 1, 1 ,1};

long duration;
int distance;
 
LiquidCrystal_I2C lcd(0x27, 16, 2);
 
void setup()
{
   WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  lcd.init();   // initializing the LCD
  lcd.backlight(); // Enable or Turn On the backlight 
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600); // Starts the serial communication

 //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    int n = WiFi.scanNetworks();

 if (n == 0)
 {
  

     lcd.setCursor(0, 0);
  lcd.print("no networks found"); // Start Print text to Line 1
  lcd.setCursor(0, 1);      
  lcd.print("--------"); // Start Print Test to Line 2
}
 
  else
  {
     lcd.setCursor(0, 0);
    lcd.print(n);
    lcd.print(" Networks Found ");
    delay(2000);
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      lcd.setCursor(0, 0);
       lcd.print("                ");
         lcd.setCursor(0, 0);
      lcd.print(i + 1);
      lcd.print(": ");
      lcd.setCursor(0, 1);
        lcd.print("                ");
       lcd.setCursor(0, 1);  
      lcd.print(WiFi.SSID(i));
      /*
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");*/
      delay(2000); }
      
  }
    
  //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
   // connect to wifi.
 
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED)
      {
    Serial.print(".");
    delay(500);
      }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  
   configTime(timezone, dst, "in.pool.ntp.org","time.nist.gov");
  Serial.println("\nWaiting for Internet time");

  while(!time(nullptr)){
     Serial.print("*");
     delay(1000);
  }
  

}
 
void loop()
{
  //                                                 Time Print
 
   
  time_t now = time(nullptr);
  struct tm* p_tm = localtime(&now);


  Serial.print(p_tm->tm_mday);
  Serial.print("/");
  Serial.print(p_tm->tm_mon + 1);
  Serial.print("/");
  Serial.print(p_tm->tm_year + 1900);
  Serial.print(" ");
  Serial.print(p_tm->tm_hour);
  Serial.print(":");
  Serial.print(p_tm->tm_min);
  Serial.print(":");
  Serial.println(p_tm->tm_sec);
  int ss = p_tm->tm_sec;
  
//  ultrasonicDistance();

  // Clears the trigPin
digitalWrite(trigPin, LOW);
delayMicroseconds(2);

// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);

// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);

// Calculating the distance
distance= duration*0.034/2;
int waterlevel=sensorHeight-distance;
// Prints the distance on the Serial Monitor
Serial.print("Distance: ");
Serial.println(distance);
lcd.setCursor(0, 0);
lcd.print("Water Level: ");
lcd.print(waterlevel);
lcd.print("    ");
lcd.setCursor(0, 1);
lcd.print("Time: XX:XX:XX      ");
Firebase.setFloat(fbdo,"Water_Level",waterlevel);
delay(100);

   if(waterlevel<25)
   {
    Firebase.setInt(fbdo,"Status",1111);
   }
   if(waterlevel>=25 && waterlevel<30)
   {
    Firebase.setInt(fbdo,"Status",1112);
   }
   if(waterlevel>=30 && waterlevel<35)
   {
    Firebase.setInt(fbdo,"Status",1123);
   }
    if(waterlevel>=35 && waterlevel<40)
   {
    Firebase.setInt(fbdo,"Status",1233);
   }
    if(waterlevel>=40 && waterlevel<45)
   {
    Firebase.setInt(fbdo,"Status",2333);
   }
   if(waterlevel>=45 && waterlevel<50)
   {
    Firebase.setInt(fbdo,"Status",3333);
   }

  int check =  updated[5]+10*updated[4] + 100*updated[3] + 1000*updated[2] + 10000*updated[1] + 100000*updated[0] ;
Firebase.setInt(fbdo,"Array",check);
  if(ss>0 && ss<10 && updated[0]==1 )
  {
   updated[0]=0;
    Firebase.setInt(fbdo,"Graph_Data/10",waterlevel);
//       Firebase.setInt(fbdo,"Graph_Data/20",0);
//       Firebase.setInt(fbdo,"Graph_Data/30",0);
//       Firebase.setInt(fbdo,"Graph_Data/40",0);
//       Firebase.setInt(fbdo,"Graph_Data/50",0);
//       Firebase.setInt(fbdo,"Graph_Data/60",0);
//    
  }
  if(ss>10 && ss<20 && updated[1]==1 )
  {
   updated[1]=0;
    Firebase.setInt(fbdo,"Graph_Data/20",waterlevel);
  }
  if(ss>20 && ss<30 && updated[2]==1 )
  {
   updated[2]=0;
    Firebase.setInt(fbdo,"Graph_Data/30",waterlevel);
  }
  if(ss>30 && ss<40 && updated[3]==1 )
  {
   updated[3]=0;
    Firebase.setInt(fbdo,"Graph_Data/40",waterlevel);
  }
  if(ss>40 && ss<50 && updated[4]==1 )
  {
   updated[4]=0;
   updated[5]=1;
    Firebase.setInt(fbdo,"Graph_Data/50",waterlevel);
  }
  if(ss>50 && ss<60 && updated[5]==1 )
  {
   updated[5]=0;
   updated[0]=1;
   updated[1]=1;
   updated[2]=1;
   updated[3]=1;
   updated[4]=1;
    Firebase.setInt(fbdo,"Graph_Data/60",waterlevel);
  }
 
}

void ultrasonicDistance()
{
 
}
