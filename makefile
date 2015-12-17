EXE=distance_stats.exe

all:
	$(CXX) -std=c++11 -I. -o $(EXE) distance_stats.cpp 
