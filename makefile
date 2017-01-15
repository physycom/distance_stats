EXE1=distance_stats.exe
EXE2=global_stats.exe
EXE3=trip_stats.exe

all: dirtree
	$(CXX) -std=c++11 -Isrc/jsoncons/src -o bin/$(EXE1) src/distance_stats.cpp
	$(CXX) -std=c++11 -Isrc/jsoncons/src -o bin/$(EXE2) src/global_stats.cpp
	$(CXX) -std=c++11 -Isrc/jsoncons/src -o bin/$(EXE3) src/trip_stats.cpp

dirtree:
	@mkdir -p bin
	@mkdir -p obj

clean:
	rm -f *.o *.sdf *.ilk *.pdb *.iobj *.obj *.ipdb bin/*.exe

cleanall: clean
	rm -rf bin obj




