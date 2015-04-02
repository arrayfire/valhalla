#include <vector>
#define __CL_ENABLE_EXCEPTIONS
#include <bolt/cl/device_vector.h>
#include <bolt/cl/scan.h>
#include <bolt/cl/copy.h>
#include <valhalla.hpp>

namespace Bolt = bolt::cl;

namespace vll {

    template<typename T>
    void scan(const int num, bool gen = true, int iter = 0)
    {

        static Bolt::device_vector<T> d_in ;
        static Bolt::device_vector<T> d_out;

        if (gen) {
            std::vector<T> h_in(num,1);
            std::generate(h_in.begin(), h_in.end(), rand);

            d_in = Bolt::device_vector<T>(num);
            d_out = Bolt::device_vector<T>(num);

            Bolt::copy(h_in.begin(), h_in.end(), d_in.begin());
            Bolt::control::getDefault().getCommandQueue().finish();
        }

        for (int i = 0; i < iter; i++) {
            Bolt::inclusive_scan(d_in.begin(), d_in.end(), d_out.begin());
        }

        Bolt::control::getDefault().getCommandQueue().finish();
    }

    template<typename T>
    void generate(const int num)
    {
        try {
            scan<T>(num, true, 0);
        } catch(cl::Error &err) {
            std::cout << err.err() << " " << err.what() << std::endl;
            throw;
        }
    }

    template<typename T>
    void run(const int iter)
    {
        try {
            scan<T>(-1, false, iter);
        } catch(cl::Error &err) {
            std::cout << err.err() << " " << err.what() << std::endl;
            throw;
        }
    }
}

int main(int argc, const char **args)
{
    vll::launch(argc, args);
}
