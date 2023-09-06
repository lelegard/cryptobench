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
// Start the benchmark.
//----------------------------------------------------------------------------

void bench::start()
{
    _iterations = 0;
    _usec_total = 0;
    _usec_start = sys::cpu_time();
}

//----------------------------------------------------------------------------
// Add iterations in a running benchmark.
//----------------------------------------------------------------------------

void bench::add_iterations(int64_t more_iterations)
{
    _iterations += more_iterations;
    _usec_total = sys::cpu_time() - _usec_start;
}

//----------------------------------------------------------------------------
// Compute the approximated number of additional iterations required
// to reach a given CPU time in microseconds. Negative means unknown.
//----------------------------------------------------------------------------

int64_t bench::need_iterations() const
{
    if (_usec_total >= _min_usec) {
        return std::max<int64_t>(0, _min_iterations - _iterations);
    }
    else if (_usec_total <= 0) {
        return -1; // not enough data to decide
    }
    else {
        // Always add 2 iterations as safe margin.
        return 2 + sys::div64(_iterations * (_min_usec - _usec_total), _usec_total);
    }
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
    start();

    int64_t more = min_iterations();
    for (int64_t i = 0; i < more; ++i) {
        one_iteration();
    }
    add_iterations(more);

    while ((more = need_iterations()) > 0) {
        for (int64_t i = 0; i < more; ++i) {
            one_iteration();
        }
        add_iterations(more);
    }
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
