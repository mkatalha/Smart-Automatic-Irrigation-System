#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

SoftwareSerial sim(10, 11);

String number = "+8801636286484";
String textMessage;
int state = 0;

const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define relayPin 8
#define motorPin 12
#define speedPin 9
#define soilSensor1 A4
#define soilSensor2 A5


void setup() {
  lcd.begin(16, 2);
  lcd.print("   Welcome To  ");
  lcd.setCursor(0, 1);
  lcd.print("My Project Show");
  Serial.begin(9600);
  Serial.println("System Started.....");
  sim.begin(9600);
  delay(1000);
  sim.print("AT+CNMI=2,2,0,0,0\r");
  delay(1000);

  lcd.clear();
  lcd.print("   Automatic   ");
  lcd.setCursor(0, 1);
  lcd.print("Iragation System");
  delay(2000);
  lcd.clear();

  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);

  pinMode(motorPin, OUTPUT);
  digitalWrite(motorPin, HIGH);

  pinMode(soilSensor1, INPUT);
  pinMode(soilSensor2, INPUT);
}

void loop() {
  readSensor();
}

void readSensor()
{
  int Sensor1 = digitalRead(soilSensor1); Serial.print(Sensor1); Serial.print("\t");
  int Sensor2 = digitalRead(soilSensor2); Serial.println(Sensor2);
  lcd.setCursor(0, 0);
  lcd.print("S1:");
  lcd.setCursor(8, 0);
  lcd.print("S2:");
  lcd.setCursor(0, 1);
  lcd.print("Motor: ");

  if (Sensor1 == 0) {
    lcd.setCursor(3, 0); lcd.print("Water");
  }
  else if (Sensor1 == 1) {
    lcd.setCursor(3, 0); lcd.print(" dry ");
  }

  if (Sensor2 == 0) {
    lcd.setCursor(11, 0); lcd.print("Water");
  }
  else if (Sensor2 == 1) {
    lcd.setCursor(11, 0); lcd.print(" dry ");
  }
  if (Sensor1 == 0 && Sensor2 == 0 && state == 0) {
    lcd.setCursor(7, 1); lcd.print("OFF ");
  }
  else if (Sensor1 == 0 && Sensor2 == 1 && state == 0) {
    lcd.setCursor(7, 1); lcd.print("OFF ");
  }
  else if (Sensor1 == 1 && Sensor2 == 0 && state == 0) {
    lcd.setCursor(7, 1); lcd.print("OFF ");
  }
  else if (Sensor1 == 1 && Sensor2 == 1 && state == 0) {
    state = 1;
     motorONsms(); delay(2000);
    state = 2;
    lcd.setCursor(7, 1); lcd.print(" ON  ");
    motorON();
  }
  if (Sensor1 == 0 && Sensor2 == 0 && state == 2) {
     motorOFFsms(); delay(2000);
    state = 0;
    lcd.setCursor(7, 1); lcd.print("OFF ");
    motorOFF();
  }
  delay(250);
  lcd.clear();
}
void motorON()
{
  digitalWrite(relayPin, LOW);
  analogWrite(speedPin, 100);
}
void motorOFF()
{
  digitalWrite(relayPin, HIGH);
  analogWrite(speedPin, 0);
}
void motorONsms()
{
  sim.println("AT+CMGF=1");
  delay(1000);
  sim.println("AT+CMGS=\"" + number + "\"\r");
  delay(1000);
  String SMS = "Motor ON";
  sim.println(SMS);
  delay(100);
  sim.println((char)26);
  delay(1000);
}

void motorOFFsms()
{
  sim.println("AT+CMGF=1");
  delay(1000);
  sim.println("AT+CMGS=\"" + number + "\"\r");
  delay(1000);
  String SMS = "Motor OFF";
  sim.println(SMS);
  delay(100);
  sim.println((char)26);
  delay(1000);
}
