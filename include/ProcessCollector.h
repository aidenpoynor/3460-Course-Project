#pragma once
#include <vector>
#include "SystemSnapshot.h"

class ProcessCollector {
public:
    std::vector<ProcessInfo> collect();
};
