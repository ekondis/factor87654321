#include <CL/sycl.hpp>
#include <iostream>
#include <string>

using namespace cl;

int main(void) {
    std::cout << "SYCL device selection" << std::endl;

    std::vector<sycl::device> devices = sycl::device::get_devices();

    std::cout << "Total devices: " << devices.size() << std::endl;

    int iD = 1;
    for (const auto &dev : devices) {
        std::cout << iD++ << ". " << dev.get_info<sycl::info::device::name>();
        auto vendor = dev.get_info<sycl::info::device::vendor>();
        if (vendor != "")
            std::cout << '/' << dev.get_info<sycl::info::device::vendor>();
        std::cout << std::endl;
    }

    if (devices.size() > 1) {
        iD = 0;
        while (iD < 1 || iD > devices.size()) {
            std::cout << "Select device index: ";
            std::cin >> iD;
        }
    } else
        iD = 1;
    sycl::device dev = devices[iD - 1];

    std::cout << "Device:   " << dev.get_info<sycl::info::device::name>() << std::endl;

    sycl::queue queue(dev, sycl::property::queue::enable_profiling());

    std::cout << "Profiling enabled" << std::endl;

    {
        int buff_host[2];
        sycl::buffer<int, 1> buff(buff_host, sycl::range<1>(2));

        constexpr int limit = 15000;
        auto kop = queue.submit([&](sycl::handler &cgh) {
            auto buff_acc =
                buff.get_access<sycl::access::mode::discard_write>(cgh);

            cgh.parallel_for<class factor1_8>(
                sycl::nd_range<1>{sycl::range<1>{15040}, sycl::range<1>{64}},
                [=](sycl::nd_item<1> ndi) {
                    int i = ndi.get_global_linear_id();
                    unsigned int p1 = 0;
                    if (i <= limit)
                        for (int j = i + 1; j <= limit; j++)
                            if (i * j == 87654321) {
                                p1 = j;
                            }
                    if (p1 != 0) {
                        buff_acc[0] = i;
                        buff_acc[1] = p1;
                    }
                });
        });
        queue.wait_and_throw();

        auto elapsedTime =
            (kop.get_profiling_info<sycl::info::event_profiling::command_end>() -
             kop.get_profiling_info<sycl::info::event_profiling::command_start>()) /
            1000000.0;
        buff.get_access<sycl::access::mode::read>();

        std::cout << "i = " << buff_host[0] << ", "
                  << "j = " << buff_host[1] << std::endl
                  << "kernel execution time = " << elapsedTime << " msecs"
                  << std::endl;
    }

    return 0;
}
