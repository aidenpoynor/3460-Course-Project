#include "Metrics.h"
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <optional>
#include <unordered_map>
#include <unistd.h>

namespace {

std::uint64_t total_ticks(const CpuCounters& c) {
    return c.user + c.nice + c.system + c.idle + c.iowait + c.irq + c.softirq + c.steal;
}

std::uint64_t idle_all_ticks(const CpuCounters& c) {
    return c.idle + c.iowait;
}

std::optional<double> overall_cpu(const CpuCounters& a, const CpuCounters& b) {
    const std::uint64_t total_a = total_ticks(a), total_b = total_ticks(b);
    const std::uint64_t idle_a = idle_all_ticks(a), idle_b = idle_all_ticks(b);
    if (total_b < total_a || idle_b < idle_a) return std::nullopt;

    const std::uint64_t delta_total = total_b - total_a;
    const std::uint64_t delta_idle = idle_b - idle_a;
    if (delta_total == 0 || delta_idle > delta_total) return std::nullopt;

    return 100.0 * static_cast<double>(delta_total - delta_idle)
                 / static_cast<double>(delta_total);
}

}  // namespace

void calculate_metrics(const SystemSnapshot& previous, SystemSnapshot& current) {
    static const long ticks_per_second = ::sysconf(_SC_CLK_TCK);  // Read once

    current.cpu_usage = overall_cpu(previous.cpu, current.cpu);

    // Actual elapsed time between snapshots, not the requested interval.
    const double elapsed = std::chrono::duration<double>(current.time - previous.time).count();

    std::unordered_map<int, const ProcessInfo*> prior;
    prior.reserve(previous.processes.size());
    for (const ProcessInfo& p : previous.processes) prior.emplace(p.pid, &p);

    for (ProcessInfo& b : current.processes) {
        b.cpu_usage.reset();
        const auto it = prior.find(b.pid);
        if (it == prior.end()) continue;  // New process: no delta yet.

        const ProcessInfo& a = *it->second;
        if (a.start_ticks != b.start_ticks) continue;  // Reused PID: different process.
        if (elapsed <= 0 || ticks_per_second <= 0 || b.cpu_ticks < a.cpu_ticks) continue;

        const double cpu_seconds = static_cast<double>(b.cpu_ticks - a.cpu_ticks)
                                 / static_cast<double>(ticks_per_second);
        b.cpu_usage = 100.0 * cpu_seconds / elapsed;
    }
}

bool compareByCpu(const ProcessInfo& a, const ProcessInfo& b) {
    const double cpu_a = a.cpu_usage.value_or(-1.0);
    const double cpu_b = b.cpu_usage.value_or(-1.0);
    if (cpu_a == cpu_b) {
        return a.pid < b.pid;  // Smaller PID first when CPU values tie.
    }
    return cpu_a > cpu_b;      // Higher CPU usage first.
}

bool compareByMemory(const ProcessInfo& a, const ProcessInfo& b) {
    const bool has_a = a.memory_kb.has_value();
    const bool has_b = b.memory_kb.has_value();
    if (has_a != has_b) return has_a;  // Unavailable values sort last.
    const std::uint64_t mem_a = a.memory_kb.value_or(0);
    const std::uint64_t mem_b = b.memory_kb.value_or(0);
    if (mem_a == mem_b) {
        return a.pid < b.pid;
    }
    return mem_a > mem_b;
}

void sort_and_limit(std::vector<ProcessInfo>& rows, SortMode mode, std::size_t top_n) {
    std::sort(rows.begin(), rows.end(),
              mode == SortMode::Cpu ? compareByCpu : compareByMemory);
    if (rows.size() > top_n) {
        rows.resize(top_n);
    }
}
