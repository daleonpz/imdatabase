CC=g++

OPT_1= -Wall -Wextra 
OPTIONS= $(OPT_1) -O0 -g 
CFLAGS= $(OPTIONS) 
LIB=-lpq
DEPS= db_interface.cc parse.cc

#-------------------------------------------------------
OBJ=  main.o
TARGET= test

$(TARGET): $(OBJ)  
	$(CC) $(CFLAGS) -o $(TARGET)  $(OBJ) $(DEPS) $(LIB) 


#-------------------------------------------------------
clean:
	rm -rf *~
	rm -rf *.o
	rm -rf $(TARGET)


