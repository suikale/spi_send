/// @author suikale
/// @version 23122020
///
/// reads pulse length from file, converts it to int 
/// and sends it through raspberry pi spi bus
/// works with numbers between 0-65535
/// input file contains one pulse length per line
/// depends: wiringPi
#include <wiringPiSPI.h>
#include <iostream>
#include <fstream>
#include <string>
#include <list>

#define SPI_CHANNEL 0
#define SPI_CLOCK_SPEED 2000000

using namespace std;

// send pulse length to transmitter
// input: int [0-65535]
// output: multiples of 256 [0-255], modulo [0-255]
void sendData(int intNumber) {
    int whole = intNumber / 256;
    int modulo = intNumber % 256;
    
    unsigned char full[2] = { whole, modulo };
    wiringPiSPIDataRW(SPI_CHANNEL, full, 2);
}


int main(int argc, char *argv[])
{       
    // spi device. exit if not found
    int fd = wiringPiSPISetupMode(SPI_CHANNEL, SPI_CLOCK_SPEED, 0);
    if (fd == -1) {
        cout << "SPI device not found";
        return -1;
    }

    // read input file line by line. exit if $1 is empty
    // TODO: check if file exists
    if (!argv[1]) {
        cout << "Give input file" << endl;
        return -1;
    }

    //open file
    ifstream infile(argv[1]);
    
    //for each row convert contents to int and store into a list   
    list<int> pulseLengths;
    string line;
    while (getline(infile, line)) {
        int i = stoi(line);
        if (!(i == 0)) {
            pulseLengths.push_back(i);
        }      
    }

    //close file
    infile.close();
    
    //for each int in array
    for(int i : pulseLengths)
    {
        //send it to spi bus
        sendData(i);
    }
    return 0;
}
