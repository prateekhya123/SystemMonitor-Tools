#include "system_monitor.h"
#include <psapi.h>
#include <tlhelp32.h>
#include <iostream>
#include <map>
#include <algorithm>

// Static map to store previous CPU times for CPU usage calculation
static std::map<DWORD, ULARGE_INTEGER> prevKernelTimes;
static std::map<DWORD, ULARGE_INTEGER> prevUserTimes;
static ULARGE_INTEGER prevIdleTime;
static ULARGE_INTEGER prevKernelTime;
static ULARGE_INTEGER prevUserTime;
static bool firstRun = true;

// Function to get list of processes
std::vector<ProcessInfo> GetProcessList() {
    std::vector<ProcessInfo> processes;
    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        return processes;
    }

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hProcessSnap, &pe32)) {
        CloseHandle(hProcessSnap);
        return processes;
    }

    do {
        ProcessInfo info;
        info.pid = pe32.th32ProcessID;
        info.name = pe32.szExeFile;
        info.cpuUsage = 0.0;
        info.memoryUsage = 0;

        // Get memory usage and CPU times
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pe32.th32ProcessID);
        if (hProcess != NULL) {
            PROCESS_MEMORY_COUNTERS pmc;
            if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) {
                info.memoryUsage = pmc.WorkingSetSize;
            }

            FILETIME ftCreation, ftExit, ftKernel, ftUser;
            if (GetProcessTimes(hProcess, &ftCreation, &ftExit, &ftKernel, &ftUser)) {
                info.kernelTime.LowPart = ftKernel.dwLowDateTime;
                info.kernelTime.HighPart = ftKernel.dwHighDateTime;
                info.userTime.LowPart = ftUser.dwLowDateTime;
                info.userTime.HighPart = ftUser.dwHighDateTime;

                // Calculate CPU usage
                if (!firstRun && prevKernelTimes.find(info.pid) != prevKernelTimes.end() &&
                    prevUserTimes.find(info.pid) != prevUserTimes.end()) {
                    ULARGE_INTEGER currKernel = info.kernelTime;
                    ULARGE_INTEGER currUser = info.userTime;
                    ULARGE_INTEGER prevKernel = prevKernelTimes[info.pid];
                    ULARGE_INTEGER prevUser = prevUserTimes[info.pid];

                    ULARGE_INTEGER kernelDiff, userDiff;
                    kernelDiff.QuadPart = currKernel.QuadPart - prevKernel.QuadPart;
                    userDiff.QuadPart = currUser.QuadPart - prevUser.QuadPart;

                    ULARGE_INTEGER totalDiff;
                    totalDiff.QuadPart = kernelDiff.QuadPart + userDiff.QuadPart;

                    if (totalDiff.QuadPart > 0) {
                        info.cpuUsage = (double)totalDiff.QuadPart / 10000000.0; // Convert to seconds
                        // For percentage, we need system total time, but for simplicity, use raw time
                        // Real CPU % would require system idle time
                    }
                }

                prevKernelTimes[info.pid] = info.kernelTime;
                prevUserTimes[info.pid] = info.userTime;
            }

            CloseHandle(hProcess);
        }

        processes.push_back(info);
    } while (Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);
    firstRun = false;
    return processes;
}

// Function to get overall CPU usage
double GetCPUUsage() {
    // Placeholder implementation
    // For simplicity, return a dummy value
    // Real implementation would require tracking over time
    return 50.0; // Dummy value
}

// Function to get overall memory usage
SIZE_T GetMemoryUsage() {
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    return memInfo.ullTotalPhys - memInfo.ullAvailPhys;
}

// Function to sort processes by CPU usage
void SortByCPU(std::vector<ProcessInfo>& processes) {
    std::sort(processes.begin(), processes.end(), [](const ProcessInfo& a, const ProcessInfo& b) {
        return a.cpuUsage > b.cpuUsage;
    });
}

// Function to sort processes by memory usage
void SortByMemory(std::vector<ProcessInfo>& processes) {
    std::sort(processes.begin(), processes.end(), [](const ProcessInfo& a, const ProcessInfo& b) {
        return a.memoryUsage > b.memoryUsage;
    });
}

// Function to kill a process by PID
bool KillProcess(DWORD pid) {
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
    if (hProcess == NULL) {
        return false;
    }
    bool result = TerminateProcess(hProcess, 0);
    CloseHandle(hProcess);
    return result;
}
