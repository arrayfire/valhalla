#pragma once
#include <iostream>
#include <cstdio>
#include <stdexcept>

#if __cplusplus <= 199711L
    #error "C++11 support is required"
#endif

#include <chrono>

namespace vll
{
    using std::chrono::system_clock;

    template<typename T> void run(const int num);
    template<typename T> void generate(const int iter);

    template<typename T>
    void bench_wrapper(const int start, const int end, const int step)
    {
        const int warmup_iter = 10;
        const int bench_iter = 1000;
        std::cout << "Size\t\tTime(us)\t" << std::endl;
        for (int num = start; num < end; num += step) {
            generate<T>(num);
            run<T>(warmup_iter); // warmup

            system_clock::time_point start = system_clock::now();
            run<T>(bench_iter);
            system_clock::time_point end = system_clock::now();
            auto u_sec = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / bench_iter;

            int repeat = 1;
            while (u_sec < 100 && repeat < 10 ) {
                start = system_clock::now();
                run<T>(bench_iter);
                end = system_clock::now();
                u_sec += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / bench_iter;
                repeat++;
            }

            std::cout << num << "\t\t" << u_sec / repeat << std::endl;
        }
    }

    static void launch(const int argc,
                       const char **args)
    {
        if (argc != 5) {
            printf("Usage: %s <data_type> <start> <end> <step>\n", args[0]);
            printf("data_type should be one of the following\n");
            printf("float\n");
            printf("double\n");
            throw std::runtime_error("Incorrect number of arguments");
        }

        try {
            const int start = atoi(args[2]);
            const int end  = atoi(args[3]);
            const int step = atoi(args[4]);

            if (args[1][0] == 'f') {
                bench_wrapper<float>(start, end, step);
            } else if (args[1][0] == 'd') {
                bench_wrapper<double>(start, end, step);
            } else {
                throw std::runtime_error("Data type not supported");
            }
        } catch(...) {
            std::cout << "Something went wrong" << std::endl;
        }
    }
}
