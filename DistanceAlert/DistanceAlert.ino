/* Front */
#define echoPinF 2 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPinF 3 //attach pin D3 Arduino to pin Trig of HC-SR04
long frontDuration; // variable for the duration of sound wave travel
int frontDistance; // variable for the distance measurement
int frontScale = 50; // distance between sensor and vehicle head

/* Back */
// #define echoPinB 2
// #define trigPinB 3
// long backDuration;
// int backDistance;
// int backScale = 50;

#define buzzer 5
// alertDistance can calculate from max speed s=ut+(1/2)at^2 for safety or dynamic by speed 
long alertDistance = 50;

void setup() {
  Serial.begin(9600); // Serial Communication is starting with 9600 of baudrate speed

  /* Front */
  pinMode(trigPinF, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPinF, INPUT); // Sets the echoPin as an INPUT

  /* Back */
  // pinMode(trigPinB, OUTPUT);
  // pinMode(echoPinB, INPUT);

  pinMode(buzzer, OUTPUT); 
  Serial.println("Distance Alert"); // print some text in Serial Monitor
  Serial.println("Ultrasonic Sensor HC-SR04, Buzzer and Arduino UNO R3");
}

void loop() {

  //if wheelchair not running -> buzzer not working ?

  /* Front */
  // Clears the trigPin condition
  digitalWrite(trigPinF, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPinF, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinF, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  frontDuration = pulseIn(echoPinF, HIGH);
  // Calculating the distance
  frontDistance = (frontDuration/2) / 29.1; // Speed of sound wave divided by 2 (go and back)

  // Displays the distance on the Serial Monitor
  Serial.print("Front Distance : ");
  Serial.print(frontDistance);
  Serial.println(" cm");

  //Condition
  if(frontDistance < alertDistance + frontScale) {
    digitalWrite(buzzer, 1);
    delay(1000);
    digitalWrite(buzzer, 0);
    delay(50);
  } else {
    digitalWrite(buzzer, 0);
  }

  /* Back */
  // digitalWrite(trigPinB, LOW);
  // delayMicroseconds(2);
  // digitalWrite(trigPinB, HIGH);
  // delayMicroseconds(10);
  // digitalWrite(trigPinB, LOW);

  // backDuration = pulseIn(echoPinB, HIGH);
  // backDistance = (backDuration/2) / 29.1;

  // Serial.print("Back Distance : ");
  // Serial.print(backDistance);
  // Serial.println(" cm");

  // if(backDistance < alertDistance + backScale) {
  //   digitalWrite(buzzer, 1);
  //   delay(100);
  //   digitalWrite(buzzer, 0);
  //   delay(50);
  // } else {
  //   digitalWrite(buzzer, 0);
  // }
  
  Serial.println("");
  delay(400);

}