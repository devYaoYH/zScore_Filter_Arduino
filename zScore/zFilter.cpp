#include "zFilter.h"
using namespace std;
const int DEFAULT_HISTORY = 32;
const int DEFAULT_LAG = 10;
const int DEFAULT_THRESHOLD = 2;
const double DEFAULT_INFLUENCE = 0.2;
const double EPSILON = 0.001;

double zFilter::getM1(int start, int len) {
  double m1 = 0.0;
  for (int i=0;i<len;++i){
    m1 += data[(start+i)%HISTORY];
  }
  return m1/len;
}

double zFilter::getM2(int start, int len) {
  double m2 = 0.0;
  for (int i=0;i<len;++i){
    m2 += data[(start+i)%HISTORY] * data[(start+i)%HISTORY];
  }
  return m2/len;
}

double zFilter::getStd(int start, int len) {
  double mu = getM1(start, len);
  double std = getM2(start, len) - mu*mu;
  if (std > -EPSILON && std < EPSILON) return 0.0;
  else return sqrt(getM2(start, len) - mu*mu);
}

zFilter::zFilter(): idx(0), HISTORY(DEFAULT_HISTORY), lag(DEFAULT_LAG), threshold(DEFAULT_THRESHOLD), influence(DEFAULT_INFLUENCE) {
  data = (double*) malloc(sizeof(double)*(HISTORY+1));
  avg = (double*) malloc(sizeof(double)*(HISTORY+1));
  std = (double*) malloc(sizeof(double)*(HISTORY+1));
  for (int i=0;i<HISTORY;++i){
    data[i] = 0.0;
    avg[i] = 0.0;
    std[i] = 0.0;
  }
}

zFilter::zFilter(int i): idx(0), HISTORY(i), lag(DEFAULT_LAG), threshold(DEFAULT_THRESHOLD), influence(DEFAULT_INFLUENCE) {
  data = (double*) malloc(sizeof(double)*(HISTORY+1));
  avg = (double*) malloc(sizeof(double)*(HISTORY+1));
  std = (double*) malloc(sizeof(double)*(HISTORY+1));
  for (int i=0;i<HISTORY;++i){
    data[i] = 0.0;
    avg[i] = 0.0;
    std[i] = 0.0;
  }
}

zFilter::zFilter(double* init_data): idx(0), HISTORY(0), lag(DEFAULT_LAG), threshold(DEFAULT_THRESHOLD), influence(DEFAULT_INFLUENCE) {
  HISTORY = sizeof(init_data)/sizeof(double);
  data = (double*) malloc(sizeof(double)*(HISTORY+1));
  avg = (double*) malloc(sizeof(double)*(HISTORY+1));
  std = (double*) malloc(sizeof(double)*(HISTORY+1));
  //Copies data values and populates filters
  for (int i=0;i<HISTORY;++i) data[i] = init_data[i];
  for (int i=0;i<lag;++i){
    avg[i] = 0.0;
    std[i] = 0.0;
  }
  for (int i=lag;i<HISTORY;++i){
    avg[i] = getM1((i-lag+1+HISTORY)%HISTORY, lag);
    std[i] = getStd((i-lag+1+HISTORY)%HISTORY, lag);
  }
}

void zFilter::configure(int lag, int threshold, double influence){
  this->lag = lag;
  this->threshold = threshold;
  this->influence = influence;
}

int zFilter::filter(double dataPt){
  int sig = 0, curIdx = idx%HISTORY;
  double cur_deviation = dataPt - avg[curIdx];
  if (cur_deviation > -EPSILON && cur_deviation < EPSILON){
    data[(idx+1)%HISTORY] = dataPt;
    sig = 0;
  }
  else if (cur_deviation > threshold*std[curIdx]){
    data[(idx+1)%HISTORY] = influence*dataPt + (1.0-influence)*data[curIdx];
    sig = 1;
  }
  else if (cur_deviation < -threshold*std[curIdx]){
    data[(idx+1)%HISTORY] = influence*dataPt + (1.0-influence)*data[curIdx];
    sig = -1;
  }
  else{
    data[(idx+1)%HISTORY] = dataPt;
  }
  avg[(idx+1)%HISTORY] = getM1((idx-lag+1+HISTORY)%HISTORY, lag);
  std[(idx+1)%HISTORY] = getStd((idx-lag+1+HISTORY)%HISTORY, lag);
  idx++;    //Increment index
  if (idx >= 16383) idx = HISTORY + idx%HISTORY;    //Prevent int overflow
  return sig;
}

zFilter::~zFilter(){
  delete data;
  delete avg;
  delete std;
}
