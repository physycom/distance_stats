EXE1=distance_stats.exe
EXE2=trip_stats.exe

all:
	$(CXX) -std=c++11 -Isrc -o $(EXE1) src/distance_stats.cpp
	$(CXX) -std=c++11 -Isrc -o $(EXE2) src/trip_stats.cpp

clean:
	rm -rf *.o x64 Debug Release *.sdf *.ilk *.pdb *.iobj *.obj *.ipdb src/Debug src/Release src/x64

cleanall: clean
	rm -rf *.exe
