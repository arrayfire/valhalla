//---------------------------------------------------------------------------//
// Copyright (c) 2013-2014 Kyle Lutz <kyle.r.lutz@gmail.com>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
// See http://boostorg.github.com/compute for more information.
//---------------------------------------------------------------------------//

#include <algorithm>
#include <vector>
#include <tbb/blocked_range.h>
#include <tbb/parallel_reduce.h>
#include <valhalla.hpp>

template<class T>
struct Sum {
    T value;
    Sum() : value(0) {}
    Sum( Sum& s, tbb::split ) {value = 0;}
    void operator()( const tbb::blocked_range<T*>& r ) {
        T temp = value;
        for( T* a=r.begin(); a!=r.end(); ++a ) {
            temp += *a;
        }
        value = temp;
    }
    void join( Sum& rhs ) {value += rhs.value;}
};

template<class T>
T ParallelSum( T array[], size_t n ) {
    Sum<T> total;
    tbb::parallel_reduce( tbb::blocked_range<T*>( array, array+n ),
                     total );
    return total.value;
}

namespace vll {
    template<typename T>
    void reduce(const int num, bool gen = true, int iter = 0)
    {
        static std::vector<T> vec(1000);

        if (gen) {
            vec.resize(num);
            std::generate(vec.begin(), vec.end(), rand);
        }

        for (int i = 0; i < iter; i++) {
            ParallelSum<T>(&vec[0], vec.size());
        }
    }

    template<typename T>
    void generate(const int num)
    {
        reduce<T>(num, true, 0);
    }

    template<typename T>
    void run(const int iter)
    {
        reduce<T>(-1, false, iter);
    }
}
int main(int argc, const char **args)
{
    vll::launch(argc, args);
}
