#include <Servo.h>

#define SERVO_PIN 9
#define TRIG_PIN 2 
#define ECHO_PIN 4 

Servo myServo;

int minTriggerDistance = 20;
int maxTriggerDistance = 50;
long waitBetweenActions = 5000;
long lastActionTime = millis();
long randomAction, duration, distance;
//String command = "*?2-10";  //trigger servo randomly b/t 2 and 10 sec
String command = "*1200,@200,1500?2-10";  //trigger servo randomly b/t 2 and 10 sec
boolean booRandom = false;
byte minWait, maxWait;
String actions[10];

//-------------------------------------------------------------------
void setup() 
{  
  myServo.attach(SERVO_PIN);
  myServo.writeMicroseconds(1500);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  Serial.begin(9600);
  randomSeed(analogRead(A0));
  Serial.println("nanoHCSR04");

  //parse the command string telling the controller what to do
  parseCommand(command);
  
  Serial.print("Will trigger action when object within ");
  Serial.print(minTriggerDistance);
  Serial.print("-");
  Serial.print(maxTriggerDistance);
  Serial.println(" cm.");
  Serial.print("Will reset trigger ");
  Serial.print(waitBetweenActions/1000);
  Serial.println(" sec after previous action.");
}

//-------------------------------------------------------------------
void loop() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = (duration/2) / 29.1;

  if ((distance > minTriggerDistance) && (distance < maxTriggerDistance)) {
    if (millis() > lastActionTime + waitBetweenActions) {
      lastActionTime = millis();
      waitBetweenActions = random(minWait, maxWait)*1000;

      //make frog hop
      executeActions();

      Serial.print(distance);
      Serial.print(" cm");
      Serial.print(", next: ");
      Serial.println(waitBetweenActions);
    }
  }

  delay(1000); 
}

//-------------------------------------------------------------------
void executeActions() {
  char code;
  String strTime;
  int iTime;
  
  byte i=0;
  while (actions[i] != 0) {
    code = actions[i][0];
    strTime = actions[i].substring(1);
    iTime = strTime.toInt();
//    Serial.print("code:"); Serial.print(code); Serial.print(", time:"); Serial.println(iTime);

    switch (code) {
      case '*':
        myServo.writeMicroseconds(iTime);
        break;
      case '@':
        delay(iTime);
        break;
    }
    i++;
  }
}

//-------------------------------------------------------------------
void parseCommand(String cmd) {
  //"*1200,@200,*1500?2-10"
  actions[0] = "*1200";
  actions[1] = "@200";
  actions[2] = "*1500";
  booRandom = true;
  minWait = 2;
  maxWait = 10;
}

