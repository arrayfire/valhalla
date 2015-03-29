#include <vector>
#define __CL_ENABLE_EXCEPTIONS
#include <bolt/cl/device_vector.h>
#include <bolt/cl/reduce.h>
#include <bolt/cl/copy.h>
#include <valhalla.hpp>

namespace Bolt = bolt::cl;

namespace vll {

    template<typename T>
    void reduce(const int num, bool gen = true, int iter = 0)
    {

        static Bolt::device_vector<T> d_vec;

        if (gen) {
            std::vector<T> h_vec(num,1);
            std::generate(h_vec.begin(), h_vec.end(), rand);

            d_vec = Bolt::device_vector<T>(num);
            Bolt::copy(h_vec.begin(), h_vec.end(), d_vec.begin());
            Bolt::control::getDefault().getCommandQueue().finish();
        }

        for (int i = 0; i < iter; i++) {
            T x = Bolt::reduce(d_vec.begin(), d_vec.end());
        }

        Bolt::control::getDefault().getCommandQueue().finish();
    }

    template<typename T>
    void generate(const int num)
    {
        try {
            reduce<T>(num, true, 0);
        } catch(cl::Error &err) {
            std::cout << err.err() << " " << err.what() << std::endl;
        } catch(...) {
            // do nothing
        }
    }

    template<typename T>
    void run(const int iter)
    {
        try {
            reduce<T>(-1, false, iter);
        } catch(cl::Error &err) {
            std::cout << err.err() << " " << err.what() << std::endl;
        } catch(...) {
            // do nothing
        }
    }
}

int main(int argc, const char **args)
{
    vll::launch(argc, args);
}
