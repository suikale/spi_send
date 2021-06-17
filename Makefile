TARGET=spi_send
FLAGS=-Wall -I/usr/local/include -L/usr/local/lib -lwiringPi
DEPS=src/buffer.cpp
all :
	g++ -o $(TARGET) src/$(TARGET).cpp $(DEPS) $(FLAGS)
