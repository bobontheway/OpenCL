all:
	g++ -c -o rotate.o rotate.cpp -I /opt/AMDAPPSDK-3.0/include/ -I ./include/
	g++ -c -o OpenCLRotate.o OpenCLRotate.cpp -I /opt/AMDAPPSDK-3.0/include/ -I ./include/
	g++ -o OpenCLRotate rotate.o OpenCLRotate.o -L /opt/AMDAPPSDK-3.0/lib/x86_64/sdk/ -lOpenCL
