#include "system_monitor.h"
#include <iostream>
#include <iomanip>
#include <windows.h>
#include <conio.h>
#include <thread>
#include <atomic>

void DisplayUI(std::vector<ProcessInfo> processes, double cpuUsage, SIZE_T memoryUsage, char sortMode) {
    system("cls"); // Clear console

    std::cout << "System Monitor Tool" << std::endl;
    std::cout << "===================" << std::endl;
    std::cout << "Overall CPU Usage: " << std::fixed << std::setprecision(2) << cpuUsage << "%" << std::endl;
    std::cout << "Overall Memory Usage: " << memoryUsage / (1024 * 1024) << " MB" << std::endl;
    std::cout << std::endl;

    std::cout << "Commands: 'c' - Sort by CPU, 'm' - Sort by Memory, 'k' - Kill process (enter PID), 'q' - Quit" << std::endl;
    std::cout << "Current sort: " << (sortMode == 'c' ? "CPU" : sortMode == 'm' ? "Memory" : "None") << std::endl;
    std::cout << std::endl;

    std::cout << "Process List:" << std::endl;
    std::cout << std::left << std::setw(10) << "PID" << std::setw(30) << "Name" << std::setw(15) << "CPU (%)" << "Memory (MB)" << std::endl;
    std::cout << std::string(65, '-') << std::endl;

    for (const auto& proc : processes) {
        std::cout << std::left << std::setw(10) << proc.pid
                  << std::setw(30) << proc.name
                  << std::setw(15) << std::fixed << std::setprecision(2) << proc.cpuUsage
                  << proc.memoryUsage / (1024 * 1024) << std::endl;
    }
}

int main() {
    char sortMode = 'n'; // 'n' for none, 'c' for CPU, 'm' for memory

    while (true) {
        auto processes = GetProcessList();
        double cpuUsage = GetCPUUsage();
        SIZE_T memoryUsage = GetMemoryUsage();

        // Apply sorting
        if (sortMode == 'c') {
            SortByCPU(processes);
        } else if (sortMode == 'm') {
            SortByMemory(processes);
        }

        DisplayUI(processes, cpuUsage, memoryUsage, sortMode);

        // Check for input without blocking
        if (_kbhit()) {
            char ch = _getch();
            if (ch == 'c') {
                sortMode = 'c';
            } else if (ch == 'm') {
                sortMode = 'm';
            } else if (ch == 'k') {
                std::cout << "\nEnter PID to kill: ";
                DWORD pid;
                std::cin >> pid;
                if (KillProcess(pid)) {
                    std::cout << "Process " << pid << " killed successfully." << std::endl;
                } else {
                    std::cout << "Failed to kill process " << pid << "." << std::endl;
                }
                Sleep(2000);
            } else if (ch == 'q') {
                break;
            }
        }

        Sleep(5000); 
    }

    return 0;
