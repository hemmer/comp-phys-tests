#include <iostream>
#include "LatticeModel.h"
#include "helpers/Blocking.h"
#include <cmath>

using std::cout;
using std::cin;
using std::endl;

int main() {

    int rows, cols;
    rows = cols = 50;

    int warmup = 40000;

    double energyMean = 0.0, energyErr = 0.0, cvMean = 0.0, cvErr = 0.0;
    double magMean = 0.0, magErr = 0.0, chiMean = 0.0, chiErr = 0.0;
    int blockSize = 1000;

    double initialTemp = 0.2;

    // choose which data you want to output
    enum OutputType { ENERGY, MAGNETISATION, C_V, CHI, ALL};
    int dataDisplayed = MAGNETISATION;

    // setup and initialise a model
    LatticeModel ising = LatticeModel(rows, cols);
    ising.setTemp(initialTemp);

    // track and average observables
    Blocking energyBlocker("energy", blockSize, 0.0);
    Blocking magnetisationBlocker("magnetisation", blockSize, 0.0);

    cout << "model created with:" << endl;
    cout << "rows: " << ising.rows() << ", cols: " << ising.cols() << endl;

    // let the system warmup to reach equilibrium
    for (int i = 0; i < warmup; ++i) {
        ising.monteCarloStep();
    }

    // print header
    switch(dataDisplayed) {
        case ENERGY:
            cout << "temperature \t energy \t error" << endl;
            break;
        case MAGNETISATION:
            cout << "temperature \t magnetisation \t error" << endl;
            break;
        case C_V:
            cout << "temperature \t c_v \t error" << endl;
            break;
        case CHI:
            cout << "temperature \t chi \t error" << endl;
            break;
        case ALL:
            cout << "temperature \t energy \t magnetisation \t c_v \t chi" << endl;
            break;
    }





    // main experiment: iterate over various temperatures
    for (double temperature = initialTemp; temperature < 4.0; temperature += 0.05) {

        // update model temperature
        ising.setTemp(temperature);
        // update the blocking code with the new prefactors
        energyBlocker.updatePrefactor( (1.0 / (temperature * temperature) ) );
        magnetisationBlocker.updatePrefactor( (1.0 / temperature ) );

        // take averages every monte carlo step
        for (int i = 0; i < 10000; ++i) {
            ising.monteCarloStep();
            energyBlocker.addData( ising.currentEnergy() );
            magnetisationBlocker.addData( ising.currentMagnetisation() );
        }

        energyBlocker.readResults(energyMean, energyErr, cvMean, cvErr);
        magnetisationBlocker.readResults(magMean, magErr, chiMean, chiErr);

        // only print relevant info
        switch(dataDisplayed) {
            case ENERGY:
                cout << temperature << "\t" << energyMean << "\t" << sqrt(energyErr) << endl;
                break;
            case MAGNETISATION:
                cout << temperature << "\t" << magMean << "\t" << sqrt(magErr) << endl;
                break;
            case C_V:
                cout << temperature << "\t" << cvMean << "\t" << sqrt(cvErr) << endl;
                break;
            case CHI:
                cout << temperature << "\t" << chiMean << "\t" << sqrt(chiErr) << endl;
                break;
            case ALL:
                cout << temperature << "\t" << energyMean << "\t" << magMean << "\t" << cvMean << "\t" << chiMean << endl;
                break;
        }

    }

    return 0;

}

