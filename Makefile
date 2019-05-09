GCC=g++
CFLAGS=-Wall -g
EXE_SERVER=server 
SRC_SERVER=server.cpp

$(EXE_SERVER):$(SRC_SERVER)
	$(GCC) -o $(EXE_SERVER) $(SRC_SERVER) $(CFLAGS)

clean:
	rm $(EXE_SERVER)  *.o -rf
