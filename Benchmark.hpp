#ifndef Benchmarkhpp
#define Benchmarkhpp

#include <iostream>
#include <chrono>
#include <thread>

#define START(timename) auto timename = std::chrono::system_clock::now();
#define STOP(timename, elapsed) auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - timename).count();

using namespace std;

class Benchmark
{
    
private:
    long *us_elapsed;
    chrono::system_clock::time_point start;
    chrono::system_clock::time_point stop;
    string message;
    using usecs = chrono::microseconds;
    using msecs = chrono::milliseconds;
    
public:
    Benchmark(const string m);           // Constructor
    Benchmark(const string m, long *us); // Constructor
    ~Benchmark();                        // Deconstructor
};

#endif
