/*
	remote id: -rid int (0 - 67108863)
    remote: -r char (1 - 3)
    group:  -g char (0 - 3)
    device: -d char (0 - 3)
    state:  -s char (0 - 3)
    reset:  -hr
    TODO: 0-3 or 1-4 ???

    argument parsing from https://stackoverflow.com/a/868894

	TODO: clean up if's?
	TODO: add support also for "-r12" style options ???
	TODO: adjust delay to hard reset??
	TODO: doesn't work
*/

#include <wiringPiSPI.h>
#include <wiringPi.h>
#include <algorithm>
#include <iostream>
#include <unistd.h>
#include <string>

#define SPI_CHANNEL 	0
#define SPI_CLOCK_SPEED 250000
#define RESET_PIN 		22
#define SLEEP_DELAY		1

void printUsage()
{
    std::cout << "usage: ./main args" << std::endl;
    std::cout << "  where args is a combination of" << std::endl;
    std::cout << "    -rid (0 - 67108863) | for setting remote id" << std::endl;
    std::cout << "    -r   (1 - 255)      | for selecting hardcoded remote" << std::endl;
    std::cout << "    -g   (0 - 3)        | for selecting group" << std::endl;
    std::cout << "    -d   (0 - 3)        | for selecting device" << std::endl;
    std::cout << "    -s   (0 - 3)        | for selecting state" << std::endl;
    std::cout << "  or just -hr for hard resetting the avr" << std::endl;
    std::cout << "  defaults every value to 0" << std::endl;
}

char* getCmdOption(char ** begin, char ** end, const std::string & option)
{
    char ** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return *itr;
    }
    return 0;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option)
{
    return std::find(begin, end, option) != end;
}

void spiRW(unsigned char * data, int len)
{
    printf("data sent:\n");
    for (int i = 0; i < len; i++)
    {
        printf("  0x%02x\n", data[i]);
    }
	
	wiringPiSPIDataRW(SPI_CHANNEL, data, len);
}

void hardResetAVR()
{
	digitalWrite(RESET_PIN, 1);
	sleep(SLEEP_DELAY);
	digitalWrite(RESET_PIN, 0);
    std::cout << "hard resetting" << std::endl;
}

void setupSPI()
{
	wiringPiSetup();
    wiringPiSPISetup(SPI_CHANNEL, SPI_CLOCK_SPEED);
	pinMode(RESET_PIN, 1);
}

int main(int argc, char ** argv)
{
    if (argc < 2)
    {
        printUsage();
        return 0;
    }

    if (cmdOptionExists(argv, argv + argc, "-hr"))
    {
        hardResetAVR();
        return 0;
    }

    int remoteId = 0, remote = 0, group = 0, device = 0, state = 0;
    char *rid, *r, *g, *d, *s;
    
    rid = getCmdOption(argv, argv + argc, "-rid");
    r = getCmdOption(argv, argv + argc, "-r");
    g = getCmdOption(argv, argv + argc, "-g");
    d = getCmdOption(argv, argv + argc, "-d");
    s = getCmdOption(argv, argv + argc, "-s");

    if (rid)
    {
        remoteId = atoi(rid);
        if (remoteId < 0 || 67108863 < remoteId)
        {
            std::cout << "please use remote id between 0 and 67108863" << std::endl;
            return 0;
        }
    }

    if (r) 
    { 
        remote = atoi(r);
        if (remote < 1 || 255 < remote)
        {
            std::cout << "please use hardcoded remote id between 1 and 255" << std::endl;
            return 0;
        }
    }
    
    if (g)
    {
        group = atoi(g);
        if (group < 0 || 3 < group)
        {
            std::cout << "please use group id between 0 and 3" << std::endl;
            return 0;
        }
    }
    
    if (d)
    {
        device = atoi(d);
        if (device < 0 || 3 < device)
        {
            std::cout << "please use device id between 0 and 3" << std::endl;
            return 0;
        }
    }
    
    if (s)
    {
        state = atoi(s); 
        if (state < 0 || 3 < state)
        {
            std::cout << "please use state between 0 and 3" << std::endl;
            return 0;
        }
    }

    if (rid && r)
    {
        std::cout << "please select either hard coded remote id (-r) OR remote id (-rid)" << std::endl;
        return 0;
    }
    
    if (!(d && s))
    {
        std::cout << "please select device id (-d) AND preferred state (-s)" << std::endl;
        return 0;
    }

	if (r || rid) setupSPI();

    if (r)
    {
        /*    
            using hardcoded remote ID
            1: [rrrrrrrr]
            2: [00ggddss]
        */

        unsigned char d[2];
        d[0] = (unsigned char)(remote & 0xff);
        d[1] = (unsigned char)(0 | (state & 3) | ((device & 3) << 2) | ((group & 3) << 4));
        spiRW(d, sizeof(d));
    }

    if (rid)
    {
        /*
            using custom remote ID
            1: [00000000]
            2: [rrrrrrrr]
            3: [rrrrrrrr]
            4: [rrrrrrrr]
            5: [rrggddss] 
        */

        unsigned char d[5];
        d[0] = 0;
        d[1] = (unsigned char)((remoteId >> 18) & 0xff);
        d[2] = (unsigned char)((remoteId >> 10) & 0xff);
        d[3] = (unsigned char)((remoteId >> 2) & 0xff);
        d[4] = (unsigned char)((state & 3) | ((device & 3) << 2) | ((group & 3) << 4) | ((remoteId & 3) << 6));
        spiRW(d, sizeof(d));
    }

    return 0;
}
