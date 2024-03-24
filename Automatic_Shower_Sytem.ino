#include <Servo.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL3JpjUqGKq"
#define BLYNK_TEMPLATE_NAME "servochatgpt"

char auth[] = "B8lhFemTa_6O40zbMiIMV7ZREYkX1bl-";  // Replace with your Blynk authentication token
char ssid[] = "Sneha";   // Replace with your WiFi credentials
char pass[] = "sneha1807";

Servo servo1;
Servo servo2;
#define echoPin 15               // CHANGE PIN NUMBER HERE IF YOU WANT TO USE A DIFFERENT PIN
#define trigPin 4
#define relay 5
long duration, distance;
int t=0;
int st=0;


void setup() {
  Serial.begin(115200);
  pinMode(relay,OUTPUT);
  digitalWrite(relay,HIGH);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT); 
  Blynk.begin(auth, ssid, pass,"blynk.cloud",80);
  int t=0;
  
  
  servo1.attach(2);  // cold water
  servo2.attach(13); // Hot water
}
void pump(int t){
  if(distance<20 and t==1){
      digitalWrite(relay,LOW);
    }
    else{
      digitalWrite(relay,HIGH);
      }
   }
void ultrasonic(){
   digitalWrite(trigPin, LOW);
  delayMicroseconds(4);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  distance = duration / 58.2;
  String disp = String(distance);
 
  Serial.print("Distance: ");
  Serial.print(disp);
  Serial.println(" cm");
  delay(1000);
  }
  
BLYNK_WRITE(V3){
  int switch1=param.asInt();
  if(switch1==1){
    st=1;
  }
    else{
      st=0;
      t=0;
      digitalWrite(relay,HIGH);
        servo1.write(0);
        servo2.write(0);
        Blynk.virtualWrite(V2,0);
        Blynk.virtualWrite(V1,0);
        Blynk.virtualWrite(V0,0);
      }
  }
 BLYNK_WRITE(V0) {//Cold Water
      int switchState = param.asInt();
       ultrasonic();
       if(distance<=20 and switchState==1 and st==1){
              servo1.write(200);
              delay(1000);
              digitalWrite(relay,LOW);
              t=1;
      }
  }
BLYNK_WRITE(V1) {//HOT Water
          int switchState = param.asInt();
          ultrasonic();
          if(distance<=20 and switchState==1 and st==1){
              servo1.write(200);
              servo2.write(300);
              delay(1000);
              digitalWrite(relay,LOW);
              t=1;
            }
           
          }

BLYNK_WRITE(V2) {//normal Water
     int switchState = param.asInt();
      ultrasonic();
      if(distance<=20 and switchState==1 and st==1){
          servo1.write(370);
          servo2.write(120);
          delay(1000);
          t=1;
      }
        
      }

void loop() {
  ultrasonic();
  Blynk.run();
  if(t==1 and st==1){
    pump(t);
    }
   else{
    pump(0);
    }
}
