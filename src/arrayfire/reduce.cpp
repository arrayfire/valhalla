#include <arrayfire.h>
#include <af/traits.hpp>
#include <valhalla.hpp>

namespace vll {

    using namespace af;

    array a;

    template<typename T>
    void generate(const int num)
    {
        try {
            a = randu(num, (af::dtype)dtype_traits<T>::af_type);
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
                T res = sum<T>(a);
            }
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
