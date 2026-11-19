#pragma once

#include <tracy/Tracy.hpp>

// Two levels of Tracy zones (guides/benchmarking/08-tracy-profiling.md):
//
// - Coarse zones use plain ZoneScopedN directly. They are real zones in any
//   build configured with PROFILING=ON (scripts' --profiling) and compile to
//   nothing otherwise.
// - Fine zones use CORE_ZONE_FINE. They mark code that runs many times per event
//   (once per atom, per tECS operation, per live state), where a zone's own
//   cost (tens of ns) is comparable to the work it measures and would distort
//   the coarse breakdown. They are real zones only with PROFILING_FINE=ON
//   (--profiling-fine), which defines CORE_PROFILING_FINE.
#if defined(CORE_PROFILING_FINE)
#define CORE_ZONE_FINE(name) ZoneScopedN(name)
#else
#define CORE_ZONE_FINE(name)
#endif
