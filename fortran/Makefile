
FF = h5fc
FFLAGS= -lhdf5 -lhdf5_fortran -g

OBJECTS = read_galaxies.f90
TARGET = read_galaxies
LIBS = -L/usr/lib/libhdf5_fortran.la -L/usr/lib/gcc/x86_64-linux-gnu/4.6/libgcc.a -I/usr/include/ -I/usr/lib/
INCLUDES =

$(TARGET): $(OBJECTS) 
	$(FF) $(FFLAGS)  $(OBJECTS) -o $(TARGET) $(INCLUDES) $(LIBS)

$(OBJECTS) : $(INCLUDES)  Makefile

.PHONY: clean
clean:
	rm -rf *.o $(TARGET)
