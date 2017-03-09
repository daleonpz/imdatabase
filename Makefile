CC=g++
OBJ= main.o db_interface.o parse.o

OPT_1= -Wall -Wextra 
OPT_2= -O2
DEBUG= -O0 -g
CFLAGS= $(OPT_1) $(DEBUG) -c
LFLAGS= $(OPT_1) $(DEBUG)
LIB=-lpq

main: $(OBJ)
	$(CC) $(LFLAGS) $(OBJ) -o main $(LIB)

#------------------------------------------------------- 
TARGET1= main.o
SRC1=main.cc 
OBJ1=headers/db_interface.h

$(TARGET1): $(OBJ1)  
	$(CC) $(CFLAGS) $(SRC1) $(OBJ1)  

#-------------------------------------------------------
TARGET2= db_interface.o
SRC2= headers/db_interface.cc
OBJ2= headers/db_interface.h $(SRC2)  headers/parse.h

$(TARGET2): $(OBJ2)  
	$(CC) $(CFLAGS) $(SRC2)   

#-------------------------------------------------------
TARGET3= parse.o
SRC3= headers/parse.cc
OBJ3= headers/parse.h $(SRC3)

$(TARGET3): $(OBJ3)
	$(CC) $(CFLAGS) $(SRC3) 
#-------------------------------------------------------
clean:
	rm -rf *~
	rm -rf *.o


