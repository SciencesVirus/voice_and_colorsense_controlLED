#include "Wire.h"
#include "Adafruit_TCS34725.h"

#define buttonPin 2
#define PIN_R 4
#define PIN_G 16
#define PIN_B 17
#define PIN_12 5
#define PIN_MIC 15
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_300MS, TCS34725_GAIN_4X);
int buttonState = 1;
int lastButtonState = 1;
const int threshold = 100;
const int numReadings = 5;
float r, g, b;
int readings[numReadings];
int index1 = 0;
int total = 0;
int bt_val = 0;
int voice_val = 0;

void setup(void) {
  Serial.begin(115200);

  pinMode(PIN_R, OUTPUT);
  pinMode(PIN_G, OUTPUT);
  pinMode(PIN_B, OUTPUT);
  pinMode(PIN_12, OUTPUT);
  pinMode(PIN_MIC, INPUT);
  

  for (int i = 0; i < numReadings; i++) {
    readings[i] = 0;  // 初始化數值陣列
  }
    if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1);
  }
}

void loop(void) {
  //button_state();
  //voice_color();
  sense_color();
  delay(600);
}


void sense_color() {

  float redValue, greenValue, blueValue;
  tcs.getRGB(&redValue, &greenValue, &blueValue);
  if (greenValue < 55 && redValue >=174 && blueValue < 75){
    redValue = 255;
    greenValue = 0;
    blueValue = 0;
  }
  else if (greenValue > 110 && redValue <45 && blueValue < 75){
    redValue = 0;
    greenValue = 255;
    blueValue = 0;
  }
  else if (greenValue < 86 && redValue > 45 && blueValue > 121){
    redValue = 255;
    greenValue = 0;
    blueValue = 200;
  }
  else if (greenValue > 145 && redValue > 50 && blueValue > 50){
    redValue = 0;
    greenValue = 150;
    blueValue = 0;
  }
  else if (greenValue > 121 && redValue > 85){
    redValue = 255;
    greenValue = 200;
    blueValue = 0;
  }
  else if (greenValue > 80 && redValue > 100){
    redValue = 255;
    greenValue = 100;
    blueValue = 0;
  }
  else if (blueValue > 170){
    redValue = 0;
    greenValue = 0;
    blueValue = 255;
  }
  else if (blueValue < 45){
    blueValue = 0;
  }
  else if (greenValue < 45){
    greenValue = 0;
  }
  else if (redValue < 40){
    redValue = 0;
  }

  float pwm_valR = map((int)redValue, 0, 255, 255, 0);
  float pwm_valG = map((int)greenValue, 0, 255, 255, 0);
  float pwm_valB = map((int)blueValue, 0, 255, 255, 0);
  Serial.print("Red = ");
  Serial.print((int)redValue);   /*Print Red Color Value on Serial Monitor*/
  Serial.print("    ");
  Serial.print("Green = ");
  Serial.print((int)greenValue); /*Print Green Color Value on Serial Monitor*/
  Serial.print("    ");
  Serial.print("Blue = ");
  Serial.println((int)blueValue);/*Print Blue Color Value on Serial Monitor*/

  analogWrite(PIN_12, 255);
  analogWrite(PIN_G, pwm_valG);
  analogWrite(PIN_R, pwm_valR);
  analogWrite(PIN_B, pwm_valB);

  Serial.flush();


}

void blue_light() {
  analogWrite(PIN_12, 255);
  analogWrite(PIN_G, 255);
  analogWrite(PIN_R, 255);
  analogWrite(PIN_B, 0);
}
void lightgreen_light() {
  float pwm_valR = map(144, 0, 255, 255, 0);
  float pwm_valG = map(238, 0, 255, 255, 0);
  float pwm_valB = map(144, 0, 255, 255, 0);
  analogWrite(PIN_12, 255);
  analogWrite(PIN_G, pwm_valG);
  analogWrite(PIN_R, pwm_valR);
  analogWrite(PIN_B, pwm_valB);
}
void green_light() {
  analogWrite(PIN_12, 255);
  analogWrite(PIN_G, 0);
  analogWrite(PIN_R, 255);
  analogWrite(PIN_B, 255);
}
void orange_light() {
  float pwm_valR = map(255, 0, 255, 255, 0);
  float pwm_valG = map(165, 0, 255, 255, 0);
  float pwm_valB = map(0, 0, 255, 255, 0);
  analogWrite(PIN_12, 255);
  analogWrite(PIN_G, pwm_valG);
  analogWrite(PIN_R, pwm_valR);
  analogWrite(PIN_B, pwm_valB);
}
void yellow_light() {
  float pwm_valR = map(255, 0, 255, 255, 0);
  float pwm_valG = map(255, 0, 255, 255, 0);
  float pwm_valB = map(0, 0, 255, 255, 0);
  analogWrite(PIN_12, 255);
  analogWrite(PIN_G, pwm_valG);
  analogWrite(PIN_R, pwm_valR);
  analogWrite(PIN_B, pwm_valB);
}
void red_light() {
  analogWrite(PIN_12, 255);
  analogWrite(PIN_G, 255);
  analogWrite(PIN_R, 0);
  analogWrite(PIN_B, 255);
}

void voice_color() {
  int val = analogRead(PIN_MIC);
  total = total - readings[index1];    // 減去最舊的數值
  readings[index1] = val;              // 更新數值陣列
  total = total + readings[index1];    // 加上最新的數值
  index1 = (index1 + 1) % numReadings;  // 移動索引，循環使用陣列

  int average = total / numReadings;  // 計算平均值

  int valueChange = abs(val - average);  // 計算與平均值的差異

  if (valueChange > threshold) {
    // 在此處執行當數值變化超過閾值時要進行的操作
    Serial.println("Significant value change detected!");
    voice_val++;
    if (voice_val >= 5) {
      voice_val = 5;
    }
  } else {
    voice_val--;
    if (voice_val <= 0) {
      voice_val = 0;
    }
  }

  Serial.println(val);
  Serial.println(voice_val);
  switch (voice_val) {
    case 0:
      blue_light();
      break;

    case 1:
      lightgreen_light();
      break;

    case 2:
      green_light();
      break;

    case 3:
      yellow_light();
      break;

    case 4:
      orange_light();
      break;

    case 5:
      red_light();
      break;
  }
}
void button_state() {
  buttonState = digitalRead(buttonPin);
  while(1){
  if (buttonState != 1) {
    Serial.println(buttonState);
    sense_color();
  } else if (buttonState == 1) {
    Serial.println(buttonState);
    analogWrite(PIN_12, 0);
    analogWrite(PIN_G, 255);
    analogWrite(PIN_R, 255);
    analogWrite(PIN_B, 255);
  }
  }
}