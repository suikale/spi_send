/// @author suikale
/// @version 17062021
/// depends: wiringPi
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <iostream>
#include <cstring>
#include <string>

#define SPI_CHANNEL 0
#define SPI_CLOCK_SPEED 2000000
#define RESET_PIN 22

void spi_rw(unsigned char *data)
{
	wiringPiSPIDataRW(SPI_CHANNEL, data, 1);
}

int main(int argc, char **argv)
{       
    if (argc < 2)
        return -1;
	 
    // setup
	wiringPiSetup();
    wiringPiSPISetup(SPI_CHANNEL, SPI_CLOCK_SPEED);
	pinMode(RESET_PIN, 1);

	// katso halutaanko resetoida
	if ((std::string)argv[1] == "reset")
	{		
		digitalWrite(RESET_PIN, 1);
		digitalWrite(RESET_PIN, 0);
		std::cout << "reset" << std::endl;
		return 0;
	}

    // d on pointer lähtevään dataan
    unsigned char *d = nullptr;
    
	// lähetä kaikki parametrit kirjain kerrallaan
	int i = 1;
	char c;

	while (i < argc)
	{
		std::cout << "param :" << i << std::endl;
		// ota i:s parametri
		d = (unsigned char *)argv[i];

		for (unsigned char *p = d; *p != '\0'; p++)
		{
			// tulosta lähtevä data
			std::cout << "\tTX: " << *p << std::endl;
		
    		// lähetä ja vastaanota dataa
    		spi_rw(p);
    
    		//tulosta vastaanotettu data
			std::cout << "\tRX: " << *p << std::endl;
			std::cout << std::endl;
		}

		// käsky päättyy, lähetä NUL
		c = 0x00;
		spi_rw((unsigned char *)&c);		

		i++;
	}
	
	// EOT
	c = 0x04;
	spi_rw((unsigned char *)&c);

    return 0;
}
