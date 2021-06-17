TARGET=spi_send
FLAGS=-Wall -I/usr/local/include -L/usr/local/lib -lwiringPi

all :
	g++ -o $(TARGET) $(TARGET).cpp $(FLAGS)
