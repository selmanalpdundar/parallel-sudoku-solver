#include "Benchmark.hpp"

Benchmark::Benchmark(const string message)
{
    this->message = message;
    this->us_elapsed = (long *)NULL;
    start = chrono::system_clock::now();
}

Benchmark::Benchmark(const string message, long *us)
{
    this->message = message;
    this->us_elapsed = us;
    start = chrono::system_clock::now();
}

Benchmark::~Benchmark()
{
    stop = chrono::system_clock::now();
    chrono::duration<double> elapsed = stop - start;
    auto musec = chrono::duration_cast<chrono::microseconds>(elapsed).count();
    cout << message << " computed in " << musec << " usec " << endl;
    if (us_elapsed != NULL)
    {
        (*us_elapsed) = musec;
    }
}

