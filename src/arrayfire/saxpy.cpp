#include <arrayfire.h>
#include <af/traits.hpp>
#include <valhalla.hpp>

namespace vll {

    using namespace af;

    float a;
    array x, y;

    template<typename T>
    void generate(const int num)
    {
        try {
            a = 3.5;
            x = randu(num, (af::dtype)dtype_traits<T>::af_type);
            y = randu(num, (af::dtype)dtype_traits<T>::af_type);
            af::sync();
        } catch(const af::exception &ex) {
            std::cout << ex.what() << std::endl;
            throw;
        }
    }

    template<typename T>
    void run(const int iter)
    {
        try {
            for (int i = 0; i < iter; i++) {
                y = a * x + y;
                y.eval();
            }
            af::sync();
        } catch(const af::exception &ex) {
            std::cout << ex.what() << std::endl;
            throw;
        }
    }
}

int main(int argc, const char **args)
{
    vll::launch(argc, args);
}
