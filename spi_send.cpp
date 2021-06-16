/// @author suikale
/// @version 13062021
/// depends: wiringPi
#include <wiringPiSPI.h>
#include <cstring>

#define SPI_CHANNEL 0
#define SPI_CLOCK_SPEED 2000000

int main(int argc, char *argv[])
{       
    if (!argv[1])
        return -1;

    wiringPiSPISetup(SPI_CHANNEL, SPI_CLOCK_SPEED);
    wiringPiSPIDataRW(SPI_CHANNEL, (unsigned char *)argv[1], 1);
    return 0;
}
