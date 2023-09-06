//----------------------------------------------------------------------------
// CryptoBench - Copyright (c) 2023, Thierry Lelegard
// BSD 2-Clause License, see LICENSE file.
// Base class of all benchmarks.
//----------------------------------------------------------------------------

#pragma once
#include "sys.h"

class bench
{
public:
    // Benchmark name.
    std::string name() const { return _name; }

    // Get the number of iterations and CPU time.
    int64_t iterations() const { return _iterations; }
    int64_t min_iterations() const { return _min_iterations; }
    int64_t total_usec() const { return _usec_total; }
    int64_t unit_usec() const { return _iterations <= 0 ? 0 : sys::div64(_usec_total, _iterations); }

    // Run the benchmark.
    void run();

    // Compute the comparative score from a reference benchmarck.
    double score(const bench& reference, double factor = 1.0) const;
    std::string score_string(const bench& reference, double factor = 1.0) const;

    // Basic display of results, including score based on a reference benchmark.
    void display(std::ostream& out = std::cout, const bench* reference = nullptr, double factor = 1.0);

    // Run and display.
    void run(std::ostream& out, const bench* reference = nullptr, double factor = 1.0);

protected:
    // Constructor.
    bench() = delete;
    bench(const std::string& name, int64_t min_usec, int64_t min_iterations);

    // Start the benchmark.
    void start();

    // Add iterations in a running benchmark.
    void add_iterations(int64_t more_iterations);

    // Compute the approximated number of additional iterations required
    // to reach the minimum CPU time in microseconds. Negative means unknown.
    // May not be accurate. Need to be rechecked after these iterations.
    int64_t need_iterations() const;

    // One iteration.
    virtual void one_iteration() = 0;

private:
    const std::string _name;            // display name
    const int64_t _min_usec = 0;        // minimum CPU time in microseconds
    const int64_t _min_iterations = 0;  // minimum number of iterations
    int64_t       _iterations = 0;      // number of iterations
    int64_t       _usec_start = 0;      // start CPU time in microseconds
    int64_t       _usec_total = 0;      // total execution CPU time in microseconds
};
