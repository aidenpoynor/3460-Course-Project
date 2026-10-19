#include "Reporter.h"
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

namespace {

std::string format_cpu(const std::optional<double>& value) {
    if (!value) return "--";
    std::ostringstream out;
    out << std::fixed << std::setprecision(1) << *value;
    return out.str();
}

std::string format_kb(const std::optional<std::uint64_t>& value) {
    return value ? std::to_string(*value) : "--";
}

}  // namespace

void print_snapshot(const SystemSnapshot& current, const std::vector<ProcessInfo>& rows,
                    SortMode sort_by, std::chrono::milliseconds interval) {
    std::cout << "CPU: " << format_cpu(current.cpu_usage)
              << (current.cpu_usage ? "%" : "")
              << "  Memory (kB): total=" << current.memory.total_kb
              << " used=" << current.memory.used_kb
              << " available=" << current.memory.available_kb << "\n";

    std::cout << "Processes collected: " << current.processes.size()
              << " | Showing: " << rows.size()
              << " | Sort: " << (sort_by == SortMode::Cpu ? "CPU" : "Memory")
              << " | Interval: " << interval.count() << " ms\n";

    std::cout << std::left << std::setw(8) << "PID" << std::setw(17) << "NAME"
              << std::right << std::setw(8) << "CPU%" << std::setw(12) << "RSS (kB)" << "\n";
    for (const ProcessInfo& p : rows) {
        std::cout << std::left << std::setw(8) << p.pid << std::setw(17) << p.name
                  << std::right << std::setw(8) << format_cpu(p.cpu_usage)
                  << std::setw(12) << format_kb(p.memory_kb) << "\n";
    }
    std::cout << "\n";
}
