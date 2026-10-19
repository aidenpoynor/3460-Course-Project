#pragma once
#include <chrono>
#include <vector>
#include "Metrics.h"
#include "SystemSnapshot.h"

void print_snapshot(const SystemSnapshot& current, const std::vector<ProcessInfo>& rows,
                    SortMode sort_by, std::chrono::milliseconds interval);
