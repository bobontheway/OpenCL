all:
	g++ -c -o util.o ../libs/util.cpp -I /opt/AMDAPPSDK-3.0/include/ -I ../include/
	g++ -c -o OpenCLAsyncCopy.o OpenCLAsyncCopy.cpp -I /opt/AMDAPPSDK-3.0/include/ -I ../include/
	g++ -o OpenCLAsyncCopy util.o OpenCLAsyncCopy.o -L /opt/AMDAPPSDK-3.0/lib/x86_64/sdk/ -lOpenCL

clean:
	rm -rf *.o OpenCLAsyncCopy
