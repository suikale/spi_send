/// @author suikale
/// @version 17062021
/// depends: wiringPi
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <iostream>
#include <cstring>
#include <string>
#include "buffer.h"

#define SPI_CHANNEL 0
#define SPI_CLOCK_SPEED 2000000
#define RESET_PIN 22
#define BUFFER_SIZE 128

unsigned char hex;
Buffer buffer{BUFFER_SIZE};

void spi_rw(unsigned char *data)
{
	wiringPiSPIDataRW(SPI_CHANNEL, data, 1);
	buffer.push(*data);
}

int main(int argc, char **argv)
{       
    if (argc < 2)
	{
		std::cout << "usage: " << argv[0] << " [COMMAND|reset (1-4)]" << std::endl;
		return -1;
	}
	 
    // setup
	wiringPiSetup();
    wiringPiSPISetup(SPI_CHANNEL, SPI_CLOCK_SPEED);
	pinMode(RESET_PIN, 1);

	// katso halutaanko resetoida
	if ((std::string)argv[1] == "reset")
	{			
		// spi_send reset: reset avr
		if (!argv[2])
		{
			digitalWrite(RESET_PIN, 1);
			digitalWrite(RESET_PIN, 0);
			std::cout << "avr reset" << std::endl;
			return 0;
		}
		
		// spi_send reset 1-4: reset pairing with 
		hex = 0x00;
		if (*argv[2] == '1') hex = 'A';
		if (*argv[2] == '2') hex = 'B';
		if (*argv[2] == '3') hex = 'C';
		if (*argv[2] == '4') hex = 'D';
		
		if (hex != 0x00)
		{
			spi_rw(&hex);
			std::cout << "pairing with " << *argv[2] << " reset" << std::endl;
		}
		else
		{
			std::cout << "usage:" << std::endl;
			std::cout << argv[0] << " reset to reset avr" << std::endl;
			std::cout << argv[0] << " reset 1-4 to reset pairing with device 1-4" << std::endl;
		}
		return 0;
	}

    // d on pointer lähtevään dataan
    unsigned char *d = nullptr;
    
	// lähetä kaikki parametrit kirjain kerrallaan
	int i = 1;

	while (i < argc)
	{
		d = (unsigned char *)argv[i];

		for (unsigned char *p = d; *p != '\0'; p++)
		{
    		// lähetä ja vastaanota dataa
    		spi_rw(p);
		}
		i++;
	}	
	
	// tyhjennä bufferi
	std::cout << "bufferissa: " << std::endl;
	while (!buffer.is_empty())
	{	
		std::cout << buffer.pop();
	}
	std::cout << std::endl;

    return 0;
}
