//including necessary libraries
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <gamma.h>
#include <LiquidCrystal.h>
#include <Servo.h>
#include <CapacitiveSensor.h>

//servo motor stuff
Servo myServo;

//making a new matrix
Adafruit_NeoMatrix matrix1 = Adafruit_NeoMatrix(8, 8, 2,
                             NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
                             NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE,
                             NEO_GRB            + NEO_KHZ800);

//making a new matrix
Adafruit_NeoMatrix matrix2 = Adafruit_NeoMatrix(8, 8, 3,
                             NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
                             NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE,
                             NEO_GRB            + NEO_KHZ800);

//distance sensor vars
int trigPin1 = 9;
int echoPin1 = 10;
int trigPin2 = 11;
int echoPin2 = 12;

//timer vars
int interval = 500;
unsigned long startTime;

//capacative touch stuff
CapacitiveSensor   cs_4_2 = CapacitiveSensor(4, 5);
long   prevVal =  cs_4_2.capacitiveSensor(30);
long   currentVal =  cs_4_2.capacitiveSensor(30);

//***************************************************************

void setup() {
  projectSetup();
}

//***************************************************************

void loop() {
  projectLoop();
}


//***************************************************************

float getDistance(int anEcho, int aTrig) {
  //variables for how far away something is
  //and for how long it takes for the sound to return
  float distance;
  float duration;

  //the sensor is not sending out sound
  digitalWrite(aTrig, LOW);
  delayMicroseconds(5);

  //the sensor is sending out sound
  digitalWrite(aTrig, HIGH);
  delayMicroseconds(10);

  //recording the time between in and out
  digitalWrite(aTrig, LOW);
  duration = pulseIn(anEcho, HIGH);

  //calculating the exact distance with the distance formula
  distance = duration / 2 * .0344;

  //returning how far away the object is (in cm)
  return (distance);
}

//***************************************************************

//function to make the tail wiggle when you "pet" its head
void tailWag() {
  if (abs(prevVal - currentVal) > 30) {
    Serial.println("right");
    myServo.write(60);
    delay(500);
    Serial.println("left");
    myServo.write(50);
    delay(500);
  }
}

//***************************************************************

//function for the eyes looking at what's closer
void eyesFollow() {
  //if something is closer on the left, the eyes look left
  if (getDistance(echoPin1, trigPin1) > getDistance(echoPin2, trigPin2)) {
    lookingLeft();
    matrix1.clear();
    matrix2.clear();
  }

  //if something is closer on the right, the eyes look right
  if (getDistance(echoPin2, trigPin2) > getDistance(echoPin1, trigPin1)) {
    lookingRight();
    matrix1.clear();
    matrix2.clear();
  }

}



//***************************************************************

//function for the eyes looking left
void lookingLeft() {
  // This 8x8 array represents the LED matrix pixels.
  // A value of 1 means the pixel turns blurple
  int eyes[8][8] = {
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 1, 0, 0, 0, 0, 1, 0},
    {1, 1, 1, 1, 0, 0, 0, 0},
    {1, 1, 1, 1, 1, 0, 0, 1},
    {1, 1, 0, 1, 1, 0, 0, 1},
    {0, 0, 1, 1, 0, 0, 1, 1},
    {0, 1, 0, 0, 0, 0, 1, 0},
    {0, 0, 1, 1, 1, 1, 0, 0}
  };

  for (int row = 0; row < 8; row++) {
    for (int column = 0; column < 8; column++) {
      if (eyes[row][column] == 1) {
        matrix1.drawPixel(row, column, matrix1.Color(100, 0, 255));
        matrix2.drawPixel(row, column, matrix1.Color(100, 0, 255));
      }
    }
  }
  matrix1.show();
  matrix2.show();
}

//***************************************************************

//function for the eyes looking right
void lookingRight() {
  // This 8x8 array represents the LED matrix pixels.
  int eyes[8][8] = {
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 1, 0, 0, 0, 0, 1, 0},
    {1, 1, 0, 0, 1, 1, 0, 0},
    {1, 0, 0, 1, 1, 1, 1, 1},
    {1, 0, 0, 1, 0, 1, 1, 1},
    {0, 0, 0, 0, 1, 1, 1, 1},
    {0, 1, 0, 0, 0, 0, 1, 0},
    {0, 0, 1, 1, 1, 1, 0, 0}
  };

  for (int row = 0; row < 8; row++) {
    for (int column = 0; column < 8; column++) {
      if (eyes[row][column] == 1) {
        matrix1.drawPixel(row, column, matrix1.Color(100, 0, 255));
        matrix2.drawPixel(row, column, matrix1.Color(100, 0, 255));
      }
    }
  }
  matrix1.show();
  matrix2.show();
}

//***************************************************************

void projectSetup() {
  Serial.begin(9600);

  //beginning, setting brightness, clearing old code, and showing the clear for matrix 1
  matrix1.begin();
  matrix1.setBrightness(50);
  matrix1.clear();
  matrix1.show();

  //beginning, setting brightness, clearing old code, and showing the clear for matrix 2
  matrix2.begin();
  matrix2.setBrightness(50);
  matrix2.clear();
  matrix2.show();

  //setting pinmodes for distance sensors
  pinMode(9, OUTPUT);
  pinMode(10, INPUT);
  pinMode(11, OUTPUT);
  pinMode(12, INPUT);

  //setting servo pin
  myServo.attach(6);

  //setting up my timer
  startTime = millis();

  //setting up capacative touch
  cs_4_2.set_CS_AutocaL_Millis(0xFFFFFFFF);
}

//***************************************************************

void projectLoop() {
  //run functions
  //add timer for reading the eye direction
  if (millis() - startTime >= interval) {
    //having eyes follow only every half second so they don't flicker
    eyesFollow();
    //reset
    startTime = millis();
  }

  //more capacative sensor stuff
  currentVal =  cs_4_2.capacitiveSensor(20);

  tailWag();

  Serial.println(currentVal);

  //capacative touch debouncing stuff
  prevVal = currentVal;

}
