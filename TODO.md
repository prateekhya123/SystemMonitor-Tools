# System Monitor Tool - Day 1 Tasks

- [x] Create system_monitor.h: Declare functions for gathering system data (processes, CPU, memory)
- [x] Create system_monitor.cpp: Implement data gathering functions using Windows API
- [x] Create main.cpp: Implement console UI layout to display the data
- [x] Compile the project (need to install MinGW or use Visual Studio)
- [x] Test the basic functionality by running the executable

# Day 2: Display process list with CPU and memory usage

- [x] Implement proper CPU usage calculation for each process in GetProcessList()
- [x] Ensure memory usage is correctly retrieved (already partially done)

# Day 3: Implement process sorting by CPU and memory usage

- [x] Add sorting functions (by CPU, by memory)
- [x] Add UI options to sort processes (e.g., press 'c' for CPU sort, 'm' for memory sort)

# Day 4: Add functionality to kill processes

- [x] Add KillProcess function in system_monitor.h and .cpp
- [x] Add input handling in main.cpp to select and kill a process (e.g., enter PID to kill)
- [x] Add confirmation prompt before killing

# Day 5: Implement real-time update feature

- [x] Real-time loop already implemented in main.cpp (updates every 5 seconds)
