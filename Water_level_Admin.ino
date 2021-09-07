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
int timezone = 5.5 * 3600;  // indian time +5:30
int dst = 0;

const int V1pin = D5;
const int V2pin = D6;
const int V3pin = D7;
const int V4pin = D8;

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
  pinMode(V1pin, OUTPUT); 
  pinMode(V2pin, OUTPUT);
  pinMode(V3pin, OUTPUT);
  pinMode(V4pin, OUTPUT);
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
 


  Firebase.getInt(fbdo,"Status");
   int Status = fbdo.intData();

  if(Status==1111)
  {
    digitalWrite(V1pin, HIGH);
    digitalWrite(V2pin, HIGH);
    digitalWrite(V3pin, HIGH);
    digitalWrite(V4pin, HIGH);
  }
  if(Status==1112)
  {
    digitalWrite(V1pin, HIGH);
    digitalWrite(V2pin, HIGH);
    digitalWrite(V3pin, HIGH);
    digitalWrite(V4pin, HIGH);
  }
  if(Status==1123)
  {
    digitalWrite(V1pin, HIGH);
    digitalWrite(V2pin, HIGH);
    digitalWrite(V3pin, HIGH);
    digitalWrite(V4pin, LOW);
  }
  if(Status==1233)
  {
    digitalWrite(V1pin, HIGH);
    digitalWrite(V2pin, HIGH);
    digitalWrite(V3pin, LOW);
    digitalWrite(V4pin, LOW);
  }
    if(Status==2333)
  {
    digitalWrite(V1pin, HIGH);
    digitalWrite(V2pin, LOW);
    digitalWrite(V3pin, LOW);
    digitalWrite(V4pin, LOW);
  }
  if(Status==3333)
  {
    digitalWrite(V1pin, LOW);
    digitalWrite(V2pin, LOW);
    digitalWrite(V3pin, LOW);
    digitalWrite(V4pin, LOW);
  }


  
   lcd.setCursor(0, 0);
      lcd.print("STATUS          ");
      lcd.setCursor(0, 1);
      lcd.print(Status);
      lcd.print("                 ");
      delay(100);
}
