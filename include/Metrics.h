#pragma once
#include <cstddef>
#include <vector>
#include "SystemSnapshot.h"

enum class SortMode { Cpu, Memory };

// Fills cpu_usage on current (overall) and on each process that has a matching baseline.
void calculate_metrics(const SystemSnapshot& previous, SystemSnapshot& current);

bool compareByCpu(const ProcessInfo& a, const ProcessInfo& b);
bool compareByMemory(const ProcessInfo& a, const ProcessInfo& b);

// Sorts descending with unavailable values last, then keeps the first top_n rows.
void sort_and_limit(std::vector<ProcessInfo>& rows, SortMode mode, std::size_t top_n);
