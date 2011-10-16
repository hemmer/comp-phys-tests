/*
 * =====================================================================================
 *
 *       Filename:  Blocking.h
 *
 *    Description:  General purpose class for taking averages of quantities and returning
 *                  the associated variances
 *
 *        Version:  1.0
 *        Created:  19/07/11 21:11:02
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ewan Hemingway, ewan.hemingway@gmail.com
 *
 * =====================================================================================
 */
#include <string>
using std::string;

class Blocking {

    public:

        void addData(double value);   

        // getters / setters
        void readResults(double &dataMean, double &dataErr, double &derivativeMean, double &derivativeError);
        void updatePrefactor(double prefactor){ prefactor_ = prefactor; }

        // constructor
        Blocking(string name, int blockSize, double prefactor);

    private:

        string name_;
        int blockSize_;

        double data, dataSquared;
        double meanData, meanDataSquared;
        double meanDataSum, meanDataSquaredSum;

        // factor for finding higher order 
        // quantities such as heat capacity
        // referred to here as "derivative"
        double prefactor_;
        double derivative, derivativeSum, derivativeSquaredSum;
       
        // counters for the averages
        int dataTaken, blocksTaken;
        void resetAll();    // reset counters, data arrays etc
};

