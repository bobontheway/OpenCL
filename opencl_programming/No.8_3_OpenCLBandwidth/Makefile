all:
	g++ -c -o util.o ../libs/util.cpp -I /opt/AMDAPPSDK-3.0/include/ -I ../include/
	g++ -c -o OpenCLTimeCount.o OpenCLTimeCount.cpp -I /opt/AMDAPPSDK-3.0/include/ -I ../include/
	g++ -o OpenCLTimeCount util.o OpenCLTimeCount.o -L /opt/AMDAPPSDK-3.0/lib/x86_64/sdk/ -lOpenCL

clean:
	rm -rf *.o OpenCLTimeCount
