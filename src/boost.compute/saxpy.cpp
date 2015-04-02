#include <vector>
#include <boost/compute/system.hpp>
#include <boost/compute/algorithm/copy.hpp>
#include <boost/compute/container/vector.hpp>
#include <boost/compute/algorithm/transform.hpp>
#include <boost/compute/lambda.hpp>
#include <valhalla.hpp>

namespace compute = boost::compute;

namespace vll {

    template<typename T>
    void reduce(const int num, bool gen = true, int iter = 0)
    {
        static compute::device gpu;
        static compute::context context;
        static compute::command_queue queue;
        static compute::vector<T> x;
        static compute::vector<T> y;
        static compute::vector<T> res;
        static T alpha = 3.5;

        using compute::lambda::_1;
        using compute::lambda::_2;

        if (gen) {
            gpu = compute::system::default_device();
            context = compute::context(gpu);
            queue = compute::command_queue(context, gpu);

            x = compute::vector<T>(num, context);
            std::vector<T> h_x(num);
            std::generate(h_x.begin(), h_x.end(), rand);
            compute::copy(h_x.begin(), h_x.end(), x.begin(), queue);

            y = compute::vector<T>(num, context);
            std::vector<T> h_y(num);
            std::generate(h_y.begin(), h_y.end(), rand);
            compute::copy(h_y.begin(), h_y.end(), y.begin(), queue);

            res = compute::vector<T>(num, context);
            queue.finish();
        }

        for (int i = 0; i < iter; i++) {
            compute::transform(x.begin(), x.end(),
                               y.begin(), res.begin(),
                               alpha * _1 + _2,
                               queue);
        }

        queue.finish();
    }

    template<typename T>
    void generate(const int num)
    {
        try {
            reduce<T>(num, true, 0);
        } catch(...) {
            // do nothing
        }
    }

    template<typename T>
    void run(const int iter)
    {
        try {
            reduce<T>(-1, false, iter);
        } catch(...) {
            // do nothing
        }
    }
}

int main(int argc, const char **args)
{
    vll::launch(argc, args);
}
