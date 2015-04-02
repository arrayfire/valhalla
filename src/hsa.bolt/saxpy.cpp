#include <vector>
#define __CL_ENABLE_EXCEPTIONS
#include <bolt/cl/device_vector.h>
#include <bolt/cl/transform.h>
#include <bolt/cl/copy.h>
#include <valhalla.hpp>

BOLT_FUNCTOR(SaxpyFunctor,
             struct SaxpyFunctor
             {
                 float _a;
                 SaxpyFunctor(float a) : _a(a) {};

                 float operator() (const float &xx, const float &yy) const
                 {
                     return _a * xx + yy;
                 };
             };
    );

namespace Bolt = bolt::cl;

namespace vll {

    template<typename T>
    void saxpy(const int num, bool gen = true, int iter = 0)
    {

        static Bolt::device_vector<T> x;
        static Bolt::device_vector<T> y;
        static Bolt::device_vector<T> res;
        SaxpyFunctor s(3.5);

        if (gen) {
            std::vector<T> h_x(num,1);
            std::vector<T> h_y(num,1);

            x   = Bolt::device_vector<T>(num);
            y   = Bolt::device_vector<T>(num);
            res = Bolt::device_vector<T>(num);

            std::generate(h_x.begin(), h_x.end(), rand);
            std::generate(h_y.begin(), h_y.end(), rand);

            Bolt::copy(h_x.begin(), h_x.end(), x.begin());
            Bolt::copy(h_y.begin(), h_y.end(), y.begin());

            Bolt::control::getDefault().getCommandQueue().finish();
        }

        for (int i = 0; i < iter; i++) {
            Bolt::transform(x.begin(), x.end(), y.begin(), res.begin(), s);
        }

        Bolt::control::getDefault().getCommandQueue().finish();
    }

    template<typename T>
    void generate(const int num)
    {
        try {
            saxpy<T>(num, true, 0);
        } catch(cl::Error &err) {
            std::cout << err.err() << " " << err.what() << std::endl;
            throw;
        }
    }

    template<typename T>
    void run(const int iter)
    {
        try {
            saxpy<T>(-1, false, iter);
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
