#include <vector>
#include <boost/compute/system.hpp>
#include <boost/compute/algorithm/copy.hpp>
#include <boost/compute/container/vector.hpp>
#include <boost/compute/algorithm/inclusive_scan.hpp>
#include <valhalla.hpp>

namespace compute = boost::compute;

namespace vll {

    template<typename T>
    void scan(const int num, bool gen = true, int iter = 0)
    {
        static compute::device gpu;
        static compute::context context;
        static compute::command_queue queue;
        static compute::vector<T> d_in;
        static compute::vector<T> d_out;

        if (gen) {
            gpu = compute::system::default_device();
            context = compute::context(gpu);
            queue = compute::command_queue(context, gpu);
            d_in = compute::vector<T>(num, context);
            d_out = compute::vector<T>(num, context);

            std::vector<T> h_in(num);
            std::generate(h_in.begin(), h_in.end(), rand);

            compute::copy(h_in.begin(), h_in.end(), d_in.begin(), queue);
            queue.finish();
        }

        for (int i = 0; i < iter; i++) {
            compute::inclusive_scan(d_in.begin(), d_in.end(), d_out.begin(), queue);
        }

        queue.finish();
    }

    template<typename T>
    void generate(const int num)
    {
        try {
            scan<T>(num, true, 0);
        } catch(...) {
            // do nothing
        }
    }

    template<typename T>
    void run(const int iter)
    {
        try {
            scan<T>(-1, false, iter);
        } catch(...) {
            // do nothing
        }
    }
}

int main(int argc, const char **args)
{
    vll::launch(argc, args);
}
