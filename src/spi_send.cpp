/// @author suikale
/// @version 01112021
/// depends: wiringPi
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <iostream>
#include <string>

#define SPI_CHANNEL 	0
#define SPI_CLOCK_SPEED 2000000
#define RESET_PIN 		22
#define MAX_ARGS 		3

void spi_rw(unsigned char *data, int len)
{
	wiringPiSPIDataRW(SPI_CHANNEL, data, len);
}

int main(int argc, char **argv)
{       
    if (argc < 2)
	{
		std::cout << "usage: " << std::endl;
		std::cout << "  set single socket state: " << argv[0] << " 0/1 [id] [group]" << std::endl;
		std::cout << "  set group state:         " << argv[0] << " 2/3 [group]" << std::endl;
		std::cout << "  reset socket:            " << argv[0] << " r [id] [group]" << std::endl;
		std::cout << "  reset avr:               " << argv[0] << " hr" << std::endl;
		std::cout << "    where [id/group] = 0 ... 3" << std::endl;
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

	
	char c[MAX_ARGS];
	for (int i = 0; i < MAX_ARGS; i++)
		c[i] = 0;
	
	for (int i = 1; i < argc && i <= MAX_ARGS; i++)
		c[i - 1] = *argv[i];
	
	for (int i = 0; i < argc; i++)
	{
		unsigned char j = (unsigned char) *(c + i);
		spi_rw(&j, 1);
	}
	
	std::cout << "sent " << argc - 1 << " bytes, received: " << c << std::endl;
	
	return 0;
}
