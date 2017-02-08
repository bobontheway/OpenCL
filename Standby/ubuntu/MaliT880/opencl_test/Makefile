all:
	g++ -c -o rotateYUV.o rotateYUV.cpp
	g++ -c -o rotate.o rotate.cpp -I /home/xbdong/Project/Excise/OpenCL/MaliT880/opencl_test/lib
	g++ -c -o opencl.o opencl.cpp -I /home/xbdong/Project/Excise/OpenCL/MaliT880/opencl_test/lib/
	g++ -c -o main.o main.cpp -I /opt/AMDAPPSDK-3.0/include/ -I /home/xbdong/Project/Excise/OpenCL/MaliT880/opencl_test/lib
	g++ -o main main.o rotate.o opencl.o rotateYUV.o -L /opt/AMDAPPSDK-3.0/lib/x86_64/sdk/ -lOpenCL
#g++ -o main main.c -I /opt/AMDAPPSDK-3.0/include/ -L /opt/AMDAPPSDK-3.0/lib/x86_64/sdk/ -lOpenCL
