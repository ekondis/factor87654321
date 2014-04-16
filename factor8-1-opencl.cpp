#define __CL_ENABLE_EXCEPTIONS

#include <iostream>  
#include <string>  
#include <CL/cl.hpp>
#include <sys/time.h>

//using namespace std;

struct timeval time_check(void){
	struct timeval t;
	gettimeofday(&t, NULL);
	return t;
}

double time_done(struct timeval t){
	struct timeval t2;
	double elapsedTime;
	gettimeofday(&t2, NULL);

	elapsedTime = (t2.tv_sec - t.tv_sec) * 1000.0;
	elapsedTime += (t2.tv_usec - t.tv_usec) / 1000.0;
	return elapsedTime;
}
  
int main(void) {  
	VECTOR_CLASS<cl::Platform> platforms;
	VECTOR_CLASS<cl::Device> devices;
	std::cout << "OpenCL platform/device selection" << std::endl;
	cl::Platform::get(&platforms);
	std::cout << "Total platforms: " << platforms.size() << std::endl;
	std::string tmp;
	int iP = 1, iD;
	for(VECTOR_CLASS<cl::Platform>::iterator pl = platforms.begin(); pl != platforms.end(); ++pl) {
		if( platforms.size()>1 )
			std::cout << iP++ << ". ";
		std::cout << pl->getInfo<CL_PLATFORM_NAME>() << std::endl;
//		cout << "Devices:" << endl;
		pl->getDevices(CL_DEVICE_TYPE_ALL, &devices);
		iD = 1;
//		cout << "print dev" << endl;
		for(VECTOR_CLASS<cl::Device>::iterator dev = devices.begin(); dev != devices.end(); ++dev)
			std::cout << '\t' << iD++ << ". " << dev->getInfo<CL_DEVICE_NAME>() << '/' << dev->getInfo<CL_DEVICE_VENDOR>() << std::endl;
	}
	if( platforms.size()>1 ){
		std::cout << "Select platform index: ";
		std::cin >> iP;
	} else
		iP = 1;
	cl::Platform pl = platforms[iP-1];
	pl.getDevices(CL_DEVICE_TYPE_ALL, &devices);
	
	if( devices.size()>1 ){
		std::cout << "Select device index: ";
		std::cin >> iD;
	} else
		iD = 1;
	cl::Device dev = devices[iD-1];//.back();//[0];
//	cl::Device dev = devices.back();//[0];

	pl.getInfo(CL_PLATFORM_NAME, &tmp);  
	std::cout << "Platform: " << tmp << std::endl;

	dev.getInfo(CL_DEVICE_NAME, &tmp);  
	std::cout << "Device:   " << tmp << std::endl;

	cl::Context context = cl::Context(VECTOR_CLASS<cl::Device>(1, dev));  
	cl::CommandQueue queue = cl::CommandQueue(context, dev);  

	std::string src_string = 
		"__kernel void factor8to1(unsigned int limit, global int *results)  \n"
		"{                                    \n"
		"	int i = get_global_id(0);         \n"
		"	                                  \n"
		"   if( i<=limit )                    \n"
		"		for(int j=i+1; j<=limit; j++) \n"
		"			if( i*j==87654321 ){      \n"
//		"			if( mul24(i,j)==87654321 ){ \n"
		"				results[0] = i;       \n"
		"				results[1] = j;       \n"
		"			}                         \n"
		"}                                    \n";  

	cl::Program::Sources src(1, std::make_pair(src_string.c_str(), src_string.size()));  
	cl::Program program(context, src);
	std::cout << "Building OpenCL program" << std::endl;
	program.build(VECTOR_CLASS<cl::Device>(1, dev));  

	std::string buildLog; 
	program.getBuildInfo(dev, CL_PROGRAM_BUILD_LOG, &buildLog);  
	std::cout << "Build log:" << std::endl  
		  << " ******************** " << std::endl  
		  << buildLog << std::endl  
		  << " ******************** " << std::endl;

	cl::Kernel kernel(program, "factor8to1");
	cl::NDRange globalSize(15040), localSize(64);
	cl::Buffer buff(context, CL_MEM_WRITE_ONLY, 2*sizeof(int));
	int* map = (int*)queue.enqueueMapBuffer(buff, CL_TRUE, CL_MAP_WRITE, 0, 2*sizeof(int));
	map[0] = map[1] = 0;
	queue.enqueueUnmapMemObject(buff, map);  
	struct timeval t;

	kernel.setArg(0, (cl_int)15000);
	kernel.setArg(1, buff);

    // Dummy calls
//	cl::Event timeEvent;
//	queue.enqueueNDRangeKernel(kernel, cl::NullRange, globalSize, localSize, NULL, NULL);
//	queue.finish();
	t = time_check();
	queue.enqueueNDRangeKernel(kernel, cl::NullRange, globalSize, localSize, NULL, NULL);
//	queue.enqueueTask(kernel);
	queue.finish();
	double elapsedTime = time_done(t);

	map = (int*)queue.enqueueMapBuffer(buff, CL_TRUE, CL_MAP_READ, 0, 2*sizeof(int));  
	std::cout << "i = " << map[0] << ", "
		<< "j = " << map[1] << std::endl
		<< "kernel execution time = " << elapsedTime << " msecs" << std::endl;
	queue.enqueueUnmapMemObject(buff, map);  

	return 0;  
}
