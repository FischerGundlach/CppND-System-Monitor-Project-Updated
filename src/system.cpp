#include <format.h>
#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

System::System() {
  operatingSystem_ = LinuxParser::OperatingSystem();
  kernel_ = LinuxParser::Kernel();
}

Processor& System::Cpu() { return cpu_; }

vector<Process>& System::Processes() {
  processes_.clear();

  auto pids = LinuxParser::Pids();
  for (const auto& pid : pids) {
    auto user = LinuxParser::User(pid);
    auto command = LinuxParser::Command(pid);
    auto cpu_utilization = LinuxParser::CpuUtilization(pid);
    auto ram = LinuxParser::Ram(pid);
    auto up_time = LinuxParser::UpTime(pid);
    processes_.emplace_back(pid, user, command, cpu_utilization, ram, up_time);
  }
  std::sort(processes_.rbegin(), processes_.rend());

  return processes_;
}

std::string System::Kernel() { return kernel_; }

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() { return operatingSystem_; }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long int System::UpTime() { return LinuxParser::UpTime(); }