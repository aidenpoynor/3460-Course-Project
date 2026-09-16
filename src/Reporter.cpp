#include "Reporter.h"
#include <iostream>

void Reporter::print(const SystemSnapshot& snapshot) {
    std::cout << "Memory (kB)\n";
    std::cout << "  Total:     " << snapshot.memory.total_kb << "\n";
    std::cout << "  Used:      " << snapshot.memory.used_kb << "\n";
    std::cout << "  Available: " << snapshot.memory.available_kb << "\n";

    std::cout << "\nProcesses: " << snapshot.processes.size() << "\n";
    for (const ProcessInfo& p : snapshot.processes) {
        std::cout << "  " << p.pid << "  " << p.name << "\n";
    }
}
