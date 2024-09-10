// Define pin numbers for ultrasonic sensor and motor driver connections
#define TRIGGER 11 // Pin connected to the trigger pin of the ultrasonic sensor
#define ECHO 10    // Pin connected to the echo pin of the ultrasonic sensor
#define M1_IN1 3   // Pin connected to the first input pin of motor 1
#define M1_IN2 5   // Pin connected to the second input pin of motor 1
#define M2_IN3 6   // Pin connected to the first input pin of motor 2
#define M2_IN4 9   // Pin connected to the second input pin of motor 2

// Flag to keep track of the turn state
bool turnFlag = false; // Initially set to false, indicating no turn is in progress

// Setup function - runs once at the beginning
void setup() {
  Serial.begin(9600);           // Initialize serial communication at 9600 baud rate

  pinMode(TRIGGER, OUTPUT);      // Set TRIGGER pin as output for sending trigger pulses
  pinMode(ECHO, INPUT);          // Set ECHO pin as input for receiving echo pulses

  pinMode(M1_IN1, OUTPUT);       // Set motor driver pins as output
  pinMode(M1_IN2, OUTPUT);
  pinMode(M2_IN3, OUTPUT);
  pinMode(M2_IN4, OUTPUT);
}

// Loop function - runs repeatedly
void loop() {
  float distObstacle = ultraSonic(); // Call ultraSonic function to measure distance
  Serial.println(distObstacle);      // Print the measured distance to the serial monitor

  if (distObstacle >= 300) {         // If distance is greater than or equal to 300 mm
    analogWrite(M1_IN1, 127);        // Move forward
    analogWrite(M1_IN2, 0);
    analogWrite(M2_IN3, 127);
    analogWrite(M2_IN4, 0);
  } else if (distObstacle < 300) {   // If distance is less than 300 mm (obstacle detected)
    if (turnFlag == false) {         // If no left turn has been made
      analogWrite(M1_IN1, 85);       // Turn left
      analogWrite(M1_IN2, 0);
      analogWrite(M2_IN3, 0);
      analogWrite(M2_IN4, 85);
      delay(750);                    // Delay 750ms for the turn to complete
      turnFlag = !turnFlag;          // Update turnFlag to indicate left turn completed
    } else if (turnFlag == true) {   // If already turned left once
      analogWrite(M1_IN1, 0);        // Turn right
      analogWrite(M1_IN2, 85);
      analogWrite(M2_IN3, 85);
      analogWrite(M2_IN4, 0);
      delay(900);                    // Delay 900ms for the turn to complete
      turnFlag = !turnFlag;          // Update turnFlag to indicate right turn completed
    }
  }
}

// Function to measure distance using ultrasonic sensor
float ultraSonic() {
  digitalWrite(TRIGGER, HIGH);       // Send a high pulse to trigger the sensor
  delayMicroseconds(10);             // Wait for 10 microseconds
  digitalWrite(TRIGGER, LOW);        // Send a low pulse to stop the trigger

  float soundTime = pulseIn(ECHO, HIGH); // Measure the duration of the high pulse on ECHO pin
  float distanceMm = soundTime / 2.94 / 2; // Calculate distance using speed of sound (2.94 ms per mm)

  delayMicroseconds(200);            // Wait for 200 microseconds before next measurement

  return distanceMm;                 // Return the calculated distance
}