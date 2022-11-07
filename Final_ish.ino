#include <Servo.h>

Servo servo[8];

int width = 4; // number of pixels in width
int height = 2; // number of pixels in height
int minPos[8] = {74, 76, 75, 84, 92, 80, 100, 98};
int maxPos[8] = {114, 108, 119, 50, 133, 122, 60, 60};
int midPos[8] = {90, 91, 92, 69, 107, 95, 85, 84};

int maxVal = 255; // maximum brightness-value (HSV)
int minVal = 80; // minimum brightness-value (HSV) (mirror does not show darker than this value)

String val; // read from serial
String pixelValue; // value for each pixel
int value; // value for each pixel as int
int posRet; // returning position from valToPos
int pos; // position for each servo


int arr[8];
String returnArray = "";

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(1);
  for(int i = 0; i < (width*height); i++){ // for each servo
    servo[i].attach(i+3); // attach servo to pin
    servo[i].write(midPos[i]); // set servo to midPos
  }
}

void loop() {
  
  if (Serial.available() > 0) { // wait for serial communication from python
    // ----- 1. Read and store the string from Python, format: (XX:XX:XX:XX:XX:XX:XX:XX) -----
    val = Serial.readString();

    
    for (int i = 0; i < (width*height); i++) { // for each servo
      // ----- 2. Take out each brightness-value and convert to an angle of rotation -----
      pixelValue = getValue(val, ':', i); // get specific value on index i
      value = pixelValue.toInt(); // convert value from String to int
      pos = valToPos(value, minPos[i], maxPos[i]); // convert brightness-value (0-255) to position (in this case: minpos-maxpos)

      // ----- 3. Write servo to matching angle -----
      if(i != 3 && i != 6 && i != 7){ // servo 3, 6, 7 are converted and have a higher minPos than maxPos
        if(pos <= maxPos[i] && pos >= minPos[i]){ // only move the servo if desired position is in interval (minPos-maxPos)
          servo[i].write(pos);
        }
      }
      else{
        if(pos >= maxPos[i] && pos <= minPos[i]){ // only move the servo if desired position is in interval (maxPos-minPos)
          servo[i].write(pos);
        }
      }
    }
  }

  // ----- 4. Add time delay to achieve desired frame rate ----
  delay(40);
  // returnArray = "";
}

// function that convert brightness-value, interval: (0 - 255), to an angle of rotation, interval: (minmum angle - maximum angle) ------
int valToPos(int valIn, int minPos, int maxPos){
  int posRet = minPos;
  if(valIn > minVal){ // ignores the values under minVal
    float tmp = (((float)(maxPos-minPos)/((float)maxVal))*(float)valIn) + (float)minPos;
    posRet = round(tmp);
  }
  return posRet;
}


// function that get data on specific index from string with separators (recieved online)
String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
