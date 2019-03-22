/*
 * ==============================================================================
 * Copyright (c) 2019 Yao Yiheng
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * ==============================================================================
 * 
 * Z-Score Filtering Algorithm for time-continuous peak detection in signal
 * Based on: https://stackoverflow.com/questions/22583391/peak-signal-detection-in-realtime-timeseries-data
 * 
 * Implementation as library allows one to attach raw input from arbitrary sensor
 * data to this algorithm and filter for peaks.
 * 
 */

#include <math.h>
#include <stdlib.h>

class zFilter{
  public:
    ~zFilter();                       //Free up malloc
    zFilter();                        //Default Constructor
    zFilter(int);                     //Initialize with lag length
    zFilter(double*);                 //Initialize with data array
    void configure(int, int, double); //Configure z-Score Filtering parameters
    int filter(double);               //Adds a new data point and returns peak status
  private:
    int idx;
    int HISTORY, lag, threshold;
    double influence;
    double* data;
    double* avg;
    double* std;
    double getM1(int, int);           //Get First Moment, also Mean
    double getM2(int, int);           //Get Second Moment
    double getStd(int, int);          //Calculate standard deviation via moments
};
