#include <arrayfire.h>
#include <af/traits.hpp>
#include "../../include/common.hpp"


namespace vll {

    using namespace af;

    array a;

    template<typename T>
    void generate(const int num)
    {
        a = randu(num, (af::dtype)dtype_traits<T>::af_type);
    }

    template<typename T>
    void run(const int iter)
    {
        for (int i = 0; i < iter; i++) {
            T res = sum<T>(a);
        }
    }
}

int main(int argc, const char **args)
{
    vll::launch(argc, args);
}
