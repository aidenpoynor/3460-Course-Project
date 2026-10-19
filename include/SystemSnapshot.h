#pragma once
#include <chrono>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

struct MemoryInfo {
    std::uint64_t total_kb{};
    std::uint64_t available_kb{};
    std::uint64_t used_kb{};
};

struct CpuCounters {
    std::uint64_t user{}, nice{}, system{}, idle{};
    std::uint64_t iowait{}, irq{}, softirq{}, steal{};
};

struct ProcessInfo {
    int pid{};                                // Keep from M1
    std::string name;                         // Keep from M1
    std::uint64_t cpu_ticks{}, start_ticks{}; // New raw counters
    std::optional<double> cpu_usage;          // New derived CPU%
    std::optional<std::uint64_t> memory_kb;   // New current VmRSS
};

struct SystemSnapshot {
    MemoryInfo memory;                      // Keep from M1
    std::vector<ProcessInfo> processes;     // Keep; use enriched records
    CpuCounters cpu;                        // New M2 counters
    std::chrono::steady_clock::time_point time;
    std::optional<double> cpu_usage;        // New overall CPU%
};
