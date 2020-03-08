#include <Servo.h>
#include <SoftwareSerial.h>

SoftwareSerial MyBlue(2,3); // RX | TX
Servo rotateServo;
Servo flipServo;
Servo holdServo;

int flag = 0;
int pflag = 0;
const int LED = 8;
const int ledpin = 4;
const int trigPin = 10;
const int echoPin = 9;
const int sensorPin = 0;
const int rtpin = 7;
const int flpin = 6;
const int hdpin = 5;

int lightLevel;
int calibratedlightLevel; // used to store the scaled / calibrated lightLevel
int maxThreshold = 0;     // used for setting the "max" light level
int minThreshold = 1023;   // used for setting the "min" light level

unsigned long startTime;
unsigned long period = 1000;

const byte positions[] = {120, 90, 180, 90, 20};
const byte numberOfPositions = sizeof(positions);
byte currentPosition = 0;

void stopMotor(Servo servo);
void run();
float getDistance();
float getPulseDuration();

void setup()
{
  Serial.begin(9600);

  MyBlue.begin(38400);

  Serial.println("Ready to connect\nDefualt password is 1234 or 000");

  rotateServo.attach(rtpin);  
  flipServo.attach(flpin);
  holdServo.attach(hdpin);

  pinMode(LED, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  stopMotor(rotateServo);
}

void loop()
{

    float distance = getDistance();
    Serial.println(distance);
    lightLevel = analogRead(sensorPin);  // reads the voltage on the sensorPin
    //Serial.println(lightLevel);
    calibratedlightLevel = map(lightLevel, 0, 1023, 0, 255);
    //Serial.println(calibratedlightLevel);
    delay(50);
    if(distance<38.0){
      digitalWrite(LED, HIGH);
    }else{
      digitalWrite(LED, LOW);
    }

    if (MyBlue.available())
      flag = MyBlue.read();
    if(flag!=pflag){
      //Serial.println(flag);
      pflag=flag;
    }
    if (flag == 120)
    {
      run();
      //Serial.println("LED On");
    }
    else if (flag == 0)
    {
      //Serial.println("LED Off");
    }
    
}

void run(){

  holdServo.write(0);
  stopMotor(rotateServo);
  rotateServo.attach(rtpin);
  rotateServo.write(0);
  delay(750);
  stopMotor(rotateServo);
  flipServo.write(180);
  holdServo.write(180);
  delay(2500);
  flipServo.write(180);
  delay(1000);
}

void stopMotor(Servo servo){
  servo.write(90);
  servo.detach();
}

float getDistance(){
  return getPulseDuration()*0.034/2;
}

float getPulseDuration(){
  digitalWrite(trigPin, LOW);
  //delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  float duration = pulseIn(echoPin, HIGH);

  return duration;
}
