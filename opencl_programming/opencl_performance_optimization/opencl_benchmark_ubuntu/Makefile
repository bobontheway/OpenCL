all:
	g++ -c -o opencl.o opencl.cpp -I /opt/AMDAPPSDK-3.0/include/
	g++ -c -o main.o main.cpp -I /opt/AMDAPPSDK-3.0/include/
	g++ -o opencl_benchmakr opencl.o main.o -L \
		/opt/AMDAPPSDK-3.0/lib/x86_64/sdk/ -lOpenCL

clean:
	rm *.o *.png main
