all:
	g++ -c -o rotate.o rotate.cpp -I /opt/AMDAPPSDK-3.0/include/ -I ./include/
	g++ -c -o OpenCLSampler.o OpenCLSampler.cpp -I /opt/AMDAPPSDK-3.0/include/ -I ./include/
	g++ -o OpenCLSampler rotate.o OpenCLSampler.o -L \
		/opt/AMDAPPSDK-3.0/lib/x86_64/sdk/ -lOpenCL -lfreeimage

clean:
	rm *.o *.png OpenCLSampler
