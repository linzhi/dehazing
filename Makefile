OUT = dehaze
CXX = g++
IDIR = include/
CXXFALGS = -Wall -g -I/home/linzhi/software/include -I/usr/include/eigen3
OBJS = dehaze.o
LIBS = -lopencv_highgui -lopencv_core -lopencv_contrib -lopencv_imgproc -lopencv_legacy -llapack

dehaze:$(OBJS)
	$(CXX) -o $(OUT) $(OBJS) $(LIBS)

dehaze.o:dehaze.cpp $(IDIR)
	$(CXX) -c  dehaze.cpp $(CXXFALGS)




.PHONY:clean

clean:
	rm -f $(OBJS)
