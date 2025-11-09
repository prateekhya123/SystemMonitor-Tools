#ifndef SYSTEM_MONITOR_H
#define SYSTEM_MONITOR_H

#include <vector>
#include <string>
#include <windows.h>
#include <map>

// Structure to hold process information
struct ProcessInfo {
    DWORD pid;
    std::string name;
    double cpuUsage;
    SIZE_T memoryUsage;
    ULARGE_INTEGER kernelTime;
    ULARGE_INTEGER userTime;
};

// Function to get list of processes
std::vector<ProcessInfo> GetProcessList();

// Function to get overall CPU usage
double GetCPUUsage();

// Function to get overall memory usage
SIZE_T GetMemoryUsage();

// Function to sort processes by CPU usage
void SortByCPU(std::vector<ProcessInfo>& processes);

// Function to sort processes by memory usage
void SortByMemory(std::vector<ProcessInfo>& processes);

// Function to kill a process by PID
bool KillProcess(DWORD pid);

#endif // SYSTEM_MONITOR_H
