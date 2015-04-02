#include <vector>
#include <boost/compute/system.hpp>
#include <boost/compute/algorithm/copy.hpp>
#include <boost/compute/container/vector.hpp>
#include <boost/compute/algorithm/reduce.hpp>
#include <valhalla.hpp>

namespace compute = boost::compute;

namespace vll {

    template<typename T>
    void reduce(const int num, bool gen = true, int iter = 0)
    {
        static compute::device gpu;
        static compute::context context;
        static compute::command_queue queue;
        static compute::vector<T> d_vec;

        if (gen) {
            gpu = compute::system::default_device();
            context = compute::context(gpu);
            queue = compute::command_queue(context, gpu);
            d_vec = compute::vector<T>(num, context);

            std::vector<T> h_vec(num);
            std::generate(h_vec.begin(), h_vec.end(), rand);

            compute::copy(h_vec.begin(), h_vec.end(), d_vec.begin(), queue);
            queue.finish();
        }

        for (int i = 0; i < iter; i++) {
            T x = 0;
            compute::reduce(d_vec.begin(), d_vec.end(), &x, queue);
        }

        queue.finish();
    }

    template<typename T>
    void generate(const int num)
    {
        try {
            reduce<T>(num, true, 0);
        } catch(compute::opencl_error &err) {
            std::cout << err.what() << std::endl;
            throw;
        }
    }

    template<typename T>
    void run(const int iter)
    {
        try {
            reduce<T>(-1, false, iter);
        } catch(compute::opencl_error &err) {
            std::cout << err.what() << std::endl;
            throw;
        }
    }
}

int main(int argc, const char **args)
{
    vll::launch(argc, args);
}
