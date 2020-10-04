/// @author suikale
/// @version 04102020
///
/// reads pulse length from csv, converts it to char 
/// and sends it through raspberry pi spi bus
/// works with numbers between 0-65535
/// depends: wiringPi
///
/// TODO: make it work

#include <iostream>
#include <fstream>
#include <wiringPiSPI.h>

#define SPI_CHANNEL 0
#define SPI_CLOCK_SPEED 2000000

using namespace std;

int main(int argc, char **argv)
{   
    int i = 0;
    // spi device
    int fd = wiringPiSPISetupMode(SPI_CHANNEL, SPI_CLOCK_SPEED, 0);
    // values read from file
    string values;
    // converted to int
    int intValue;
    // int array
    int intArray[i];

    // exit if $1 is empty
    // TODO: check if file exists
    if (argv[1] == "") {
        cout << "tarvii filen";
        return -1;
    }
    
    // exit if spi device not found
    if (fd == -1) {
        cout << "SPI device not found";
        return -1;
    }
    
    // read $1
    ifstream intValues(argv[1]);

    // reads every line from text file, 
    // extracts pulse length and stores it in array
    // TODO: make this as a method
    while (getline(intValues, values)) {
        // still a string, format 1234567,123,1 (timestamp, pulse length, state)
        values;
        // extract the pulse length
        ;
        // convert to int
        ;
        // store to array place i
        ;
        // raise i
        i++;
    }

    // close the file
    intValues.close();

    // send data through spi
    // TODO: maybe send whole array instead of single int at a time
    for (i = 0; i < sizeof(intArray); i++) {
        SendData(intArray[i]);
    }
   return 0;
}

/// input: int [0-65535]
/// output: multiples of 256 [0-255], modulo [0-255]
void SendData(int intNumber) {
    int whole = intNumber / 256;
    int modulo = intNumber % 256;
    
    unsigned char full[2] = { whole, modulo };
    wiringPiSPIDataRW(SPI_CHANNEL, full, 2);
}