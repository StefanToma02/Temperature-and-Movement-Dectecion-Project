//the time we give the sensor to calibrate (10-60 secs according to the datasheet)
int calibrationTime = 10;        

//the time when the sensor outputs a low impulse
long unsigned int lowIn;         

//the amount of milliseconds the sensor has to be low 
//before we assume all motion has stopped
long unsigned int pause = 5000;  

boolean lockLow = true;
boolean takeLowTime;  

int pirPin = 3;    //the digital pin connected to the PIR sensor's output
int ledPin = 6;

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SimpleDHT.h>

// for DHT11, 
//      VCC: 5V or 3V
//      GND: GND
//      DATA: 2
int pinDHT11 = 2;
SimpleDHT11 dht11(pinDHT11);

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
//LiquidCrystal_PCF8574 lcd(0x27);  // set the LCD address to 0x27 for a 16 chars and 2 line display

int contor = 0;

void setup() {
 Serial.begin(9600);
  // LCD
  Serial.println("LCD...");
  Wire.begin();
  Wire.beginTransmission(0x27);
  Serial.println("Check for LCD");
  int error = Wire.endTransmission();
  Serial.print("Error: ");
  Serial.print(error);
  if (error == 0) {
    Serial.println(": LCD found.");
    lcd.begin(); // initialize the lcd
    lcd.setBacklight(255);
    lcd.home(); 
    lcd.clear();
    lcd.print("Hello LCD");
    delay(1000);
  } else {
    Serial.println(": LCD not found.");
  } // if else
}

void loop() {
  // put your main code here, to run repeatedly:
  // DHT11
  // start working...
  Serial.println("=================================");
  Serial.println("Sample DHT11...");
  
  // read without samples.
  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed, err="); Serial.println(err);delay(1000);
    return;
  }
  
  Serial.print("Sample OK: ");
  Serial.print((int)temperature); Serial.print(" *C, "); 
  Serial.print((int)humidity); Serial.println(" H");
  
  // LCD
  lcd.clear();
  lcd.setCursor(0,0);  //coloana - linie
  lcd.print("Temperatura:");
    lcd.print((int)temperature);

  //lcd.setCursor(0,1);  //coloana - linie
  //lcd.print("T: ");
  //lcd.setCursor(3,1);  //coloana - linie
  //lcd.print((int)temperature);
  //lcd.setCursor(5,1);  //coloana - linie
  lcd.print(" C");
  lcd.setCursor(1,1);  //coloana - linie
  lcd.print("Umiditate:");
  lcd.print((int)humidity);
  //lcd.setCursor(13,1);  //coloana - linie
  lcd.print(" %");
  contor++;
  delay(1200);






  if(digitalRead(pirPin) == HIGH){
       digitalWrite(ledPin, HIGH);   //the led visualizes the sensors output pin state
       if(lockLow){  
         //makes sure we wait for a transition to LOW before any further output is made:
         lockLow = false;            
         Serial.println("---");
         Serial.print("motion detected at ");
         Serial.print(millis()/1000);
         Serial.println(" sec"); 
         delay(50);
         }         
         takeLowTime = true;
       }

     if(digitalRead(pirPin) == LOW){       
       digitalWrite(ledPin, LOW);  //the led visualizes the sensors output pin state

       if(takeLowTime){
        lowIn = millis();          //save the time of the transition from high to LOW
        takeLowTime = false;       //make sure this is only done at the start of a LOW phase
        }
       //if the sensor is low for more than the given pause, 
       //we assume that no more motion is going to happen
       if(!lockLow && millis() - lowIn > pause){  
           //makes sure this block of code is only executed again after 
           //a new motion sequence has been detected
           lockLow = true;                        
           Serial.print("motion ended at ");      //output
           Serial.print((millis() - pause)/1000);
           Serial.println(" sec");
           delay(50);
           }
       }

}
