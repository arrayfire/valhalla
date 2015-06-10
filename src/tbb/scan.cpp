#include <algorithm>
#include <vector>
#include <tbb/blocked_range.h>
#include <tbb/parallel_scan.h>
#include <valhalla.hpp>
using namespace tbb;

template<typename T>
class Body {
    T sum;
    T* const y;
    const T* const z;
public:
    Body( T y_[], const T z_[] ) : sum(0), z(z_), y(y_) {}
    T get_sum() const {return sum;}

    template<typename Tag>
    void operator()( const blocked_range<int>& r, Tag ) {
        T temp = sum;
        for( int i=r.begin(); i<r.end(); ++i ) {
            temp = temp + z[i];
            if( Tag::is_final_scan() )
                y[i] = temp;
        }
        sum = temp;
    }
    Body( Body& b, split ) : z(b.z), y(b.y), sum(0) {}
    void reverse_join( Body& a ) { sum = a.sum + sum;}
    void assign( Body& b ) {sum = b.sum;}
};

template<typename T>
float ParallelScan( T y[], const T z[], int n ) {
    Body<T> body(y,z);
    parallel_scan( blocked_range<int>(0,n), body );
    return body.get_sum();
}

namespace vll {
    template<typename T>
    void scan(const int num, bool gen = true, int iter = 0)
    {
        static std::vector<T> in(1000);
        static std::vector<T> out(1000);

        if (gen) {
            in.resize(num);
            out.resize(num);
            std::generate(in.begin(), in.end(), rand);
        }

        for (int i = 0; i < iter; i++) {
            ParallelScan<T>(&in[0], &out[0], in.size());
        }
    }

    template<typename T>
    void generate(const int num)
    {
        scan<T>(num, true, 0);
    }

    template<typename T>
    void run(const int iter)
    {
        scan<T>(-1, false, iter);
    }
}

int main(int argc, const char **args)
{
    vll::launch(argc, args);
}
