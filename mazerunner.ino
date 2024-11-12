// Pin definitions 
#define trigPinLeft 7
#define echoPinLeft 6
#define trigPinFront 5
#define echoPinFront 4
#define trigPinRight 3
#define echoPinRight 2

// Motor control pins
#define motorLeftForward 8
#define motorLeftBackward 9
#define motorRightForward 10
#define motorRightBackward 11

// ir sensor pins 
int IRsen=1;
int IRstate;

// Threshold distance for walls (in cm)
int threshold = 7;

// Path memory (to record movements)
char path[100];    // Array to store up to 100 moves
int pathIndex = 0; // Index to track the current move in the path

// Position tracking (basic for loop detection)
int posX = 0, posY = 0; // X, Y coordinates of the robot in the maze
int direction = 0;      // 0: North, 1: East, 2: South, 3: West (relative to starting point)

void setup() {
  Serial.begin(9600);
  //setup for ultrasonic sensors
  pinMode(trigPinLeft, OUTPUT);
  pinMode(echoPinLeft, INPUT);
  pinMode(trigPinFront, OUTPUT);
  pinMode(echoPinFront, INPUT);
  pinMode(trigPinRight, OUTPUT);
  pinMode(echoPinRight, INPUT);

//setup for motor drivers
  pinMode(motorLeftForward, OUTPUT);
  pinMode(motorLeftBackward, OUTPUT);
  pinMode(motorRightForward, OUTPUT);
  pinMode(motorRightBackward, OUTPUT);

//setup for ir sensors
  pinMode(IRsen,INPUT);
}

void loop() {
  int distanceLeft = getDistance(trigPinLeft, echoPinLeft);
  int distanceFront = getDistance(trigPinFront, echoPinFront);
  int distanceRight = getDistance(trigPinRight, echoPinRight);
  IRstate=digitalRead(IRsen);

  // Decision-making based on sensor readings
  if (distanceLeft > threshold) {
    turnLeft();
    recordMove('L');  // Record a left turn
  } else if (distanceFront > threshold) {
    moveForward();
    recordMove('F');  // Record moving forward
  } else if (distanceRight > threshold) {
    turnRight();
    recordMove('R');  // Record a right turn
  } else {
    turnAround();
    recordMove('B');  // Record a U-turn
  }

  delay(100); // Delay for stability

  // After completing one run, optimize the path (once an exit is found)
  if (IRstate == 1/* condition for reaching the end */) {
    stop();
    delay(10000);
    optimizePath();  // Optimize the path based on learned data
  }
}

// Function to measure distance using ultrasonic sensors
int getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2;

  return distance;
}

// Movement functions
void moveForward() {
  digitalWrite(motorLeftForward, HIGH);
  digitalWrite(motorLeftBackward, LOW);
  digitalWrite(motorRightForward, HIGH);
  digitalWrite(motorRightBackward, LOW);
  updatePosition(1);  // Move forward in the current direction
}

void turnLeft() {
  digitalWrite(motorLeftForward, LOW);
  digitalWrite(motorLeftBackward, HIGH);
  digitalWrite(motorRightForward, HIGH);
  digitalWrite(motorRightBackward, LOW);
  delay(300);
  updateDirection(-1);  // Turn 90 degrees left
}

void turnRight() {
  digitalWrite(motorLeftForward, HIGH);
  digitalWrite(motorLeftBackward, LOW);
  digitalWrite(motorRightForward, LOW);
  digitalWrite(motorRightBackward, HIGH);
  delay(300);
  updateDirection(1);  // Turn 90 degrees right
}

void turnAround() {
  digitalWrite(motorLeftForward, LOW);
  digitalWrite(motorLeftBackward, HIGH);
  digitalWrite(motorRightForward, HIGH);
  digitalWrite(motorRightBackward, LOW);
  delay(600);  // Longer delay for a U-turn
  updateDirection(2);  // U-turn
}
void stop(){
  digitalWrite(motorLeftForward, LOW);
  digitalWrite(motorLeftBackward, LOW);
  digitalWrite(motorRightForward, LOW);
  digitalWrite(motorRightBackward, LOW);

}

// Function to record each move
void recordMove(char move) {
  if (pathIndex < 100) {  // Avoid overflow
    path[pathIndex++] = move;
  }
}

// Function to optimize the path after multiple attempts
void optimizePath() {
  Serial.println("Optimizing path...");
  // Implement path optimization logic here
  // Example: Remove U-turns or redundant turns
  int i = 0;
  while (i < pathIndex - 1) {
    // If the robot makes unnecessary back-and-forth moves (e.g., L, R)
    if ((path[i] == 'L' && path[i+1] == 'R') || (path[i] == 'R' && path[i+1] == 'L')) {
      // Remove both movements
      removePathElement(i);
      removePathElement(i);
    } else {
      i++;
    }
  }
}

// Function to remove an element from the path
void removePathElement(int index) {
  for (int i = index; i < pathIndex - 1; i++) {
    path[i] = path[i + 1];
  }
  pathIndex--;  // Decrease path length
}

// Update the robot's position based on movement
void updatePosition(int step) {
  switch (direction) {
    case 0: posY += step; break;  // North
    case 1: posX += step; break;  // East
    case 2: posY -= step; break;  // South
    case 3: posX -= step; break;  // West
  }
}

// Update the robot's direction after a turn
void updateDirection(int turn) {
  direction = (direction + turn + 4) % 4;  // Ensure direction stays within 0-3
}