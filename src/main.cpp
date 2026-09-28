#include "MemoryCollector.h"
#include "ProcessCollector.h"
#include "Reporter.h"
#include "SystemSnapshot.h"

int main() {
    SystemSnapshot snapshot;
    snapshot.memory = collectMemoryInfo();
    snapshot.processes = collectProcessInfo();

    printSystemSnapshot(snapshot);
    return 0;
}
