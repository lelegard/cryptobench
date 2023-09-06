//----------------------------------------------------------------------------
// CryptoBench - Copyright (c) 2023, Thierry Lelegard
// BSD 2-Clause License, see LICENSE file.
// Base class of all benchmarks.
//----------------------------------------------------------------------------

#include "bench.h"

//----------------------------------------------------------------------------
// Constructor.
//----------------------------------------------------------------------------

bench::bench(const std::string& name, int64_t min_usec, int64_t min_iterations) :
    _name(name),
    _min_usec(min_usec),
    _min_iterations(min_iterations)
{
}

//----------------------------------------------------------------------------
// Run the benchmark.
//----------------------------------------------------------------------------

void bench::run(std::ostream& out, const bench* reference, double factor)
{
    run();
    display(out, reference, factor);
}

void bench::run()
{
    _iterations = 0;
    _usec_total = 0;
    const int64_t bench_start = sys::cpu_time();

    // Run several iterations at a time. Start with the minimum number of iterations.
    int64_t more = _min_iterations;
    for (;;) {
        // Run the current chunk of iterations.
        for (int64_t i = 0; i < more; ++i) {
            one_iteration();
        }

        // Include these iterations in the counters.
        _iterations += more;
        _usec_total = sys::cpu_time() - bench_start;

        // Did we reach the minimum CPU time in microseconds?
        if (_usec_total >= _min_usec || _usec_total <= 0) {
            // Note: when _usec_total is zero, we could not measure some microseconds
            // in the first chunk of iteration, we cannot decide and give up.
            break;
        }

        // Compute the approximated number of additional iterations required to reach the
        // minimum CPU time in microseconds. Always add 2 iterations as safe margin.
        more = 2 + sys::div64(_iterations * (_min_usec - _usec_total), _usec_total);
    }

    // Now run the same number of empty iterations to evaluate the overhead to remove.
    // Use two levels of virtual calls since most benchmarks do so.
    const int64_t dummy_start = sys::cpu_time();
    for (int64_t i = 0; i < _iterations; ++i) {
        dummy_iteration_2();
    }
    const int64_t dummy_usec = sys::cpu_time() - dummy_start;
    _usec_total -= std::min(_usec_total, dummy_usec);
}

//----------------------------------------------------------------------------
// Dummy iterations, for calibration.
//----------------------------------------------------------------------------

// One level of virtual call.
void bench::dummy_iteration_1()
{
}

// Two levels of virtual calls.
void bench::dummy_iteration_2()
{
    dummy_iteration_1();
}

//----------------------------------------------------------------------------
// Compute the comparative score from a reference benchmarck.
//----------------------------------------------------------------------------

double bench::score(const bench& reference, double factor) const
{
    return _iterations == 0 || reference._usec_total == 0 ? 0.0 :
           (factor * (double)_usec_total * (double)reference._iterations) / ((double)_iterations * (double)reference._usec_total);
}

std::string bench::score_string(const bench& reference, double factor) const
{
    const double sc = score(reference, factor);
    if (sc < 0.1) {
        return sys::format("%.3lf", sc);
    }
    else if (sc < 1.0) {
        return sys::format("%.2lf", sc);
    }
    else if (sc < 20.0) {
        return sys::format("%.1lf", sc);
    }
    else {
        return sys::format("%ld", long(sc));
    }
}

//----------------------------------------------------------------------------
// Basic display of results.
//----------------------------------------------------------------------------

void bench::display(std::ostream& out, const bench* reference, double factor)
{
    if (!_name.empty()) {
        out << _name << ": ";
    }
    out << sys::format("iterations=%'" PRId64 ": usec-total=%'" PRId64 ": usec/iter=%'" PRId64, iterations(), total_usec(), unit_usec());
    if (reference != nullptr && reference->_iterations > 0) {
        out << ": score=" << score_string(*reference, factor);
    }
    out << std::endl;
}
