# the compiler: gcc for C program, define as g++ for C++
CC = g++

# compiler flags:
#  -g     - this flag adds debugging information to the executable file
#  -Wall  - this flag is used to turn on most compiler warnings
CFLAGS  = -g -Wall -O3

all: grasp_roman

grasp_roman: roman.o solution.o
	$(CC) $(CFLAGS) -c grasp_roman.cpp
	$(CC) $(CFLAGS) -o grasp_roman grasp_roman.o roman.o solution.o
	$(RM) *.o

roman.o: roman.cpp
	$(CC) $(CFLAGS) -c roman.cpp

solution.o: solution.cpp
	$(CC) $(CFLAGS) -c solution.cpp

clean:
	$(RM) grasp_roman *.o
