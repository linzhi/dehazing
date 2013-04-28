OUT = dehaze
CXX = g++
IDIR = include/
CXXFALGS = -Wall -g
OBJS = dehaze.o
LIBS = -lopencv_highgui -lopencv_core -lopencv_contrib -lopencv_imgproc -lopencv_legacy

dehaze:$(OBJS)
	$(CXX) -o $(OUT) $(OBJS) $(LIBS)

dehaze.o:dehaze.cpp $(IDIR)
	$(CXX) -c $(CXXFLAGS) dehaze.cpp




.PHONY:clean

clean:
	rm -f $(OBJS)
