#include <algorithm>
#include <vector>
#include <tbb/blocked_range.h>
#include <tbb/parallel_for.h>
#include <valhalla.hpp>
using namespace tbb;

template<typename T>
void ParallelSaxpy(T a, T *x, T *y, T *z, int n)
{
    tbb::parallel_for(
        tbb::blocked_range<int>(0, n),

        // lambda function
        [&](tbb::blocked_range<int> r) {
            for (auto i = r.begin(); i != r.end(); i++) {
                z[i] = a * x[i] + y[i];
            }
        }

        );
}

namespace vll {
    template<typename T>
    void saxpy(const int num, bool gen = true, int iter = 0)
    {
        static std::vector<T> x(1000);
        static std::vector<T> y(1000);
        static std::vector<T> out(1000);
        T a = 3.5;

        if (gen) {
            x.resize(num);
            y.resize(num);
            out.resize(num);
            std::generate(x.begin(), x.end(), rand);
            std::generate(y.begin(), y.end(), rand);
        }

        for (int i = 0; i < iter; i++) {
            ParallelSaxpy<T>(a, &x[0], &y[0], &out[0], x.size());
        }
    }

    template<typename T>
    void generate(const int num)
    {
        saxpy<T>(num, true, 0);
    }

    template<typename T>
    void run(const int iter)
    {
        saxpy<T>(-1, false, iter);
    }
}

int main(int argc, const char **args)
{
    vll::launch(argc, args);
}
