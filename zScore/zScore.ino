#include "zFilter.h"
zFilter* peak;
const int FILTER_LEN = 8;
double avgFilter[FILTER_LEN];
int filter_count = 0;

int inputPin = A0;
long lastPoll = 0;

double getSmoothedData(){
  if (filter_count < FILTER_LEN){
    return 0.0;
  }
  double SUM = 0.0;
  for (int i=0;i<FILTER_LEN;++i) SUM += avgFilter[i];
  return SUM/FILTER_LEN;
}

void setup() {
  // put your setup code here, to run once:
  peak = new zFilter();
  peak->configure(10, 2, 0.9);
  Serial.begin(9600);
  pinMode(inputPin, INPUT);
}

void loop() {
  //Loops at 40Hz
  if (millis() > lastPoll + 25){
    //Measure Reading
    double phonyData = (double)analogRead(inputPin)/512 - 1.0;

    //Implement Average Smoothening First
    avgFilter[filter_count%FILTER_LEN] = phonyData;
    filter_count++;
    if (filter_count > 16000) filter_count = FILTER_LEN + filter_count%FILTER_LEN;
    
    Serial.print(getSmoothedData());
    Serial.print(",");

    //Apply Z-Score Peak Filtering
    Serial.println(peak->filter(getSmoothedData()));

    lastPoll = millis();
  }
}
