
CC = gcc
CFLAGS= -DH5_USE_16_API -g

OBJECTS = writeData.o getData.o main.o
TARGET = main
LIBS =  -lm -lhdf5
INCLUDES = dataStructs.h

$(TARGET): $(OBJECTS) 
	$(CC) $(CFLAGS)  $(OBJECTS) -o $(TARGET) $(LIBS)

$(OBJECTS) : $(INCLUDES) Makefile

.PHONY: clean
clean:
	rm -rf *.o $(TARGET)
