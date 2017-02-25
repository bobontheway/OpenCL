all:
	file_name
	g++ -o OpenCLCompile OpenCLCompile.cpp -I /opt/AMDAPPSDK-3.0/include/ -L /opt/AMDAPPSDK-3.0/lib/x86_64/sdk/ -lOpenCL


all:
	g++ -c -o util.o libs/util.cpp -I /opt/AMDAPPSDK-3.0/include/ -I ./include/
	g++ -c -o rotate.o rotate.cpp -I /opt/AMDAPPSDK-3.0/include/ -I ./include/
	g++ -c -o OpenCLSampler.o OpenCLSampler.cpp -I /opt/AMDAPPSDK-3.0/include/ -I ./include/
	g++ -o OpenCLSampler util.o rotate.o OpenCLSampler.o -L \
		/opt/AMDAPPSDK-3.0/lib/x86_64/sdk/ -lOpenCL -lfreeimage
