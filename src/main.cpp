#include "MemoryCollector.h"
#include "ProcessCollector.h"
#include "Reporter.h"
#include "SystemSnapshot.h"

int main() {
    SystemSnapshot snapshot;
    snapshot.memory = MemoryCollector().collect();
    snapshot.processes = ProcessCollector().collect();

    Reporter().print(snapshot);
    return 0;
}
