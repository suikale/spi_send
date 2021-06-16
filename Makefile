TARGET=spi_send
FLAGS=-Wall -lwiringPi

all :
	gcc -o $(TARGET) $(TARGET).cpp $(FLAGS)
