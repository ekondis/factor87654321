#define __CL_ENABLE_EXCEPTIONS

#include <iostream>  
#include <string>  
#include <CL/cl.hpp>
  
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
		pl->getDevices(CL_DEVICE_TYPE_ALL, &devices);
		iD = 1;
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
	cl::Device dev = devices[iD-1];

	pl.getInfo(CL_PLATFORM_NAME, &tmp);  
	std::cout << "Platform: " << tmp << std::endl;

	dev.getInfo(CL_DEVICE_NAME, &tmp);  
	std::cout << "Device:   " << tmp << std::endl;

	cl::Context context = cl::Context(VECTOR_CLASS<cl::Device>(1, dev));  
	cl::CommandQueue queue = cl::CommandQueue(context, dev, CL_QUEUE_PROFILING_ENABLE);  

	std::string src_string = 
		"__kernel void factor8to1(unsigned int limit, global int *results)  \n"
		"{                                    \n"
		"	int i = get_global_id(0);         \n"
		"	                                  \n"
		"	unsigned int p1 = 0;              \n"
		"   if( i<=limit )                    \n"
		"		for(int j=i+1; j<=limit; j++) \n"
		"			if( i*j==87654321 ){      \n"  // change to mul24 if 32bit multiplication is too slow
		"				p1 = j;               \n"
		"			}                         \n"
		"	if( p1!=0 ){                      \n"
		"		results[0] = i;               \n"
		"		results[1] = p1;              \n"
		"	}                                 \n"
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

	kernel.setArg(0, (cl_int)15000);
	kernel.setArg(1, buff);

	cl::Event timeEvent;
	queue.enqueueNDRangeKernel(kernel, cl::NullRange, globalSize, localSize, NULL, &timeEvent);
	cl_ulong ev_t_start, ev_t_finish;
	timeEvent.wait();
	timeEvent.getProfilingInfo(CL_PROFILING_COMMAND_START, &ev_t_start);
	timeEvent.getProfilingInfo(CL_PROFILING_COMMAND_END, &ev_t_finish);
	double elapsedTime = (ev_t_finish-ev_t_start)/1000000.0;

	map = (int*)queue.enqueueMapBuffer(buff, CL_TRUE, CL_MAP_READ, 0, 2*sizeof(int));  
	std::cout << "i = " << map[0] << ", "
		<< "j = " << map[1] << std::endl
		<< "kernel execution time = " << elapsedTime << " msecs" << std::endl;
	queue.enqueueUnmapMemObject(buff, map);  

	return 0;  
}
