#include <Wire.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>

SoftwareSerial bth(9, 10); //Tx,Rx
LiquidCrystal_I2C lcd(0x3F, 16, 2);

int Vo = A1;
int V_LED = 3;
int count = 0;

float Vo_value = 0;
float Voltage = 0;
float dust = 0;
float avg = 0;
float sum = 0;

int led_r = 6;
int led_y = 5;
int led_g = 4;
int relay_motor = 7;

void setup() {
  Serial.begin(9600);
  bth.begin(9600);
  lcd.init();
  lcd.backlight();
  pinMode(V_LED, OUTPUT);
  pinMode(Vo, INPUT);
  pinMode(led_r, OUTPUT);  
  pinMode(led_y, OUTPUT);  
  pinMode(led_g, OUTPUT); 
  pinMode(relay_motor, OUTPUT);  
  digitalWrite(relay_motor, LOW);
}

void loop() {
  digitalWrite(V_LED, LOW);
  delayMicroseconds(280);
  Vo_value = analogRead(Vo);
  delayMicroseconds(40);
  digitalWrite(V_LED, HIGH);
  delayMicroseconds(9680);

  Voltage = Vo_value * 5.0 / 1024.0;
  dust = (Voltage - 0.6) / 0.005;
  
  if(dust > 0) {
    Serial.print("Dust : ");
    Serial.print(dust);
    Serial.println(" ㎍/㎥");
  
    delay(1000);

    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("Air Quality");
    lcd.setCursor(3,1);
    lcd.print("Dust : ");
    lcd.print(dust);
  }
  
  
  if (dust > 0) {
    if (count >= 10) {
      avg = sum / count;
      Serial.print("Average : ");
      Serial.print(avg);
      Serial.println(" ㎍/㎥\n");
      
      bth.println(avg);
      
      sum = 0;
      count = 0;
    }
  else {
    sum = sum + dust;
    count++;
  }
}

  if (dust <= 15.0) {
    led_status(3);
  } else if (15.0 < dust && dust <= 50.0) {
    led_status(2);
  } else {
    led_status(1);
  }

  if (avg < 15) {
    relay(true);
  } else {
    relay(false);
  }
}

void led_status(int value) {
  if(value == 1) {
    digitalWrite(led_r, HIGH);
    digitalWrite(led_y, LOW);
    digitalWrite(led_g, LOW);
  } else if (value == 2) {
    digitalWrite(led_r, LOW);
    digitalWrite(led_y, HIGH);
    digitalWrite(led_g, LOW);
  } else if (value == 3) {
    digitalWrite(led_r, LOW);
    digitalWrite(led_y, LOW);
    digitalWrite(led_g, HIGH);
  }
}

void relay(bool value){
    if(value == true){
        digitalWrite(relay_motor, LOW);
    }else{
        digitalWrite(relay_motor, HIGH);
    }
  }

