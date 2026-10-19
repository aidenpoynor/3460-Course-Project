#include "CpuCollector.h"
#include "MemoryCollector.h"
#include "Metrics.h"
#include "ProcessCollector.h"
#include "Reporter.h"
#include "SystemSnapshot.h"
#include <atomic>
#include <chrono>
#include <csignal>
#include <exception>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

namespace {

std::atomic<bool> running{true};

void handle_sigint(int) {
    running = false;
}

struct Options {
    std::chrono::milliseconds interval{1000};
    std::size_t top_n{10};
    SortMode sort_by{SortMode::Cpu};
};

void print_usage(std::ostream& out) {
    out << "Usage: SystemMonitor [--interval <ms>] [--top <N>] [--sort cpu|mem]\n"
        << "  --interval <ms>  Refresh interval in milliseconds (default 1000)\n"
        << "  --top <N>        Number of process rows to show (default 10)\n"
        << "  --sort cpu|mem   Sort processes by CPU% or memory (default cpu)\n";
}

bool parse_positive(const std::string& text, long long& value) {
    try {
        std::size_t pos{};
        value = std::stoll(text, &pos);
        return pos == text.size() && value > 0;
    } catch (const std::exception&) {
        return false;
    }
}

// Returns false on invalid input. Sets show_help for -h/--help.
bool parse_args(int argc, char* argv[], Options& opts, bool& show_help) {
    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            show_help = true;
            return true;
        }
        if (arg != "--interval" && arg != "--top" && arg != "--sort") return false;
        if (i + 1 >= argc) return false;

        const std::string value = argv[++i];
        if (arg == "--sort") {
            if (value == "cpu") opts.sort_by = SortMode::Cpu;
            else if (value == "mem") opts.sort_by = SortMode::Memory;
            else return false;
            continue;
        }

        long long n{};
        if (!parse_positive(value, n)) return false;
        if (arg == "--interval") opts.interval = std::chrono::milliseconds(n);
        else opts.top_n = static_cast<std::size_t>(n);
    }
    return true;
}

SystemSnapshot collect_snapshot() {
    SystemSnapshot snapshot;
    snapshot.time = std::chrono::steady_clock::now();
    snapshot.memory = collectMemoryInfo();
    snapshot.processes = collectProcessInfo();
    snapshot.cpu = collectCpuCounters();
    return snapshot;
}

}

int main(int argc, char* argv[]) {
    Options opts;
    bool show_help = false;
    if (!parse_args(argc, argv, opts, show_help)) {
        print_usage(std::cerr);
        return 1;
    }
    if (show_help) {
        print_usage(std::cout);
        return 0;
    }

    std::signal(SIGINT, handle_sigint);

    try {
        SystemSnapshot previous = collect_snapshot();
        std::cout << "Baseline collected. First report in " << opts.interval.count()
                  << " ms.\n";

        while (running) {
            std::this_thread::sleep_for(opts.interval);
            if (!running) break;

            SystemSnapshot current = collect_snapshot();
            calculate_metrics(previous, current);

            std::vector<ProcessInfo> rows = current.processes;  // Display copy only
            sort_and_limit(rows, opts.sort_by, opts.top_n);
            print_snapshot(current, rows, opts.sort_by, opts.interval);

            previous = std::move(current);  // Keep the full snapshot as the next baseline
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
