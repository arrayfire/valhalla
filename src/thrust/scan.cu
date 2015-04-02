#include <algorithm>
#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include <thrust/scan.h>
#include <valhalla.hpp>

namespace vll {

    template<typename T>
    void scan(const int num, bool gen = true, int iter = 0)
    {
        static thrust::device_vector<T> d_in;
        static thrust::device_vector<T> d_out;

        if (gen) {
            thrust::host_vector<T> h_in(num);
            std::generate(h_in.begin(), h_in.end(), rand);
            d_in = h_in;
            d_out = thrust::device_vector<T>(num);
            cudaDeviceSynchronize();
        }

        for (int i = 0; i < iter; i++) {
            thrust::inclusive_scan(d_in.begin(), d_in.end(), d_out.begin());
        }

        cudaDeviceSynchronize();
    }

    template<typename T>
    void generate(const int num)
    {
        try {
            scan<T>(num, true, 0);
        } catch(const thrust::system_error &ex) {
            std::cout << ex.what() << std::endl;
        }
    }

    template<typename T>
    void run(const int iter)
    {
        try {
            scan<T>(-1, false, iter);
        } catch(const thrust::system_error &ex) {
            std::cout << ex.what() << std::endl;
        }
    }
}

int main(int argc, const char **args)
{
    vll::launch(argc, args);
}
