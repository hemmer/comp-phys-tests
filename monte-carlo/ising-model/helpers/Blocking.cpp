/*
 * =====================================================================================
 *
 *       Filename:  Blocking.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  19/07/11 21:25:59
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ewan Hemingway, ewan.hemingway@gmail.com
 *
 * =====================================================================================
 */
#include "Blocking.h"

Blocking::Blocking(string name, int blockSize, double prefactor) :
    name_(name), blockSize_(blockSize), prefactor_(prefactor) {

        resetAll();
    }


void Blocking::resetAll(){

    blocksTaken = dataTaken = 0;
    data = dataSquared = 0;
    meanData = meanDataSquared = 0;
    meanDataSum = meanDataSquaredSum = 0;
    derivative = 0;
    derivativeSum = derivativeSquaredSum = 0;
}


void Blocking::addData(double value){

    // track totals
    data += value;
    dataSquared += (value * value);

    // count each incoming data
    ++dataTaken;

    // and average over blocks of size 
    // blockSize
    if (dataTaken == blockSize_){

        // track number of averages taken
        ++blocksTaken;

        // take averages
        meanData = (double) data / (double) blockSize_;
        meanDataSquared = (double) dataSquared / (double) blockSize_;
        // and find the derivative
        //derivative =  (meanDataSquared - meanData*meanData);
        derivative = prefactor_ * (meanDataSquared - meanData*meanData);

        // and sum them
        meanDataSum += meanData;
        meanDataSquaredSum += (meanData*meanData);
        derivativeSum += derivative;
        derivativeSquaredSum += (derivative*derivative);

        // reset counters
        dataTaken = 0;
        data = dataSquared = 0;

    } else {
    }
}

void Blocking::readResults(double &dataMean, double &dataErr, 
        double &derivativeMean, double &derivativeError){

    dataMean = meanDataSum / (double) blocksTaken;
    dataErr = (meanDataSquaredSum / (double) blocksTaken) - (dataMean*dataMean);
    derivativeMean = derivativeSum / (double) blocksTaken;
    derivativeError = (derivativeSquaredSum / (double) blocksTaken) - (derivativeMean*derivativeMean);

    resetAll();
}
