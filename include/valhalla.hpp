#pragma once
#include <iostream>
#include <iomanip>
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
    void bench_wrapper(const int start, const int end, const int step, const bool ismul)
    {
        const int warmup_iter = 10;
        const int bench_iter = 1000;
        std::cout << "Size\t\tTime(us)\t" << std::endl;
        for (int num = start; num <= end; ismul ? num *= step : num += step) {
            generate<T>(num);
            run<T>(warmup_iter); // warmup

            system_clock::time_point begin = system_clock::now();
            run<T>(bench_iter);
            system_clock::time_point now = system_clock::now();
            auto u_sec = std::chrono::duration_cast<std::chrono::microseconds>(now - begin).count() / bench_iter;

            int repeat = 1;
            while (u_sec < 10000 && repeat < 10 ) {
                begin = system_clock::now();
                run<T>(bench_iter);
                now = system_clock::now();
                u_sec += std::chrono::duration_cast<std::chrono::microseconds>(now - begin).count() / bench_iter;
                repeat++;
            }

            std::cout << std::setw(10) << std::left << num << "\t" << u_sec/repeat << std::endl;
        }
    }

    static void launch(const int argc,
                       const char **args)
    {
        if (argc < 3) {
            printf("Usage: %s <data_type> <start> [<end>  [<step>]]\n", args[0]);
            printf("data_type should be one of the following\n");
            printf("float\n");
            printf("double\n");
            throw std::runtime_error("Incorrect number of arguments");
        }

        try {
            const int start = atoi(args[2]);
            const int end  = argc > 3 ? atoi(args[3]) : start;
            bool ismul = false;
            int step = start;

            if (argc > 4) {
                ismul = args[4][0] == 'x' || args[4][0] == 'X';
                step = atoi(args[4] + 1);
            }

            if (args[1][0] == 'f') {
                bench_wrapper<float>(start, end, step, ismul);
            } else if (args[1][0] == 'd') {
                bench_wrapper<double>(start, end, step, ismul);
            } else {
                throw std::runtime_error("Data type not supported");
            }
        } catch(...) {
            std::cout << "Something went wrong" << std::endl;
        }
    }
}
