/// @author suikale
/// @version 22032022
/// depends: wiringPi

#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <iostream>
#include <string>

#define SPI_CHANNEL 	0
#define SPI_CLOCK_SPEED 250000
#define RESET_PIN 		22

void spi_rw(unsigned char *data, int len)
{
	wiringPiSPIDataRW(SPI_CHANNEL, data, len);
}

int main(int argc, char **argv)
{       
    if (argc < 2)
	{
		std::cout << "usage: " << std::endl;
		std::cout << "  set state: " << argv[0] << " [state] [id] [group] [remote]" << std::endl;
		std::cout << "TODO:  reset avr: " << argv[0] << " hr" << std::endl;
		return -1;
	}
	 
    // setup
	wiringPiSetup();
    wiringPiSPISetup(SPI_CHANNEL, SPI_CLOCK_SPEED);
	pinMode(RESET_PIN, 1);

	// hard reset
	if ((std::string)argv[1] == "hr")
	{			
		digitalWrite(RESET_PIN, 1);
		digitalWrite(RESET_PIN, 0);
		std::cout << "avr reset" << std::endl;
		return 0;
	}
	
	// hardcoded values for remote 1, device 0, group 0, state 1
	unsigned char d[5];
	d[0] = 0x00;
	d[1] = 0x86;
	d[2] = 0x80;
	d[3] = 0x2a;
	d[4] = 0x81;
	spi_rw(d, 5);	
	
	printf("sent %d bytes\n", sizeof(d));
	
	return 0;
}
