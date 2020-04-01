#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid, std::string user, std::string command, float cpu_utilization, std::string ram,
                 long int up_time) : pid_(pid), user_(user), command_(command), cpu_utilization_(cpu_utilization), ram_(ram),
                                     up_time_(up_time) {}

int Process::Pid() { return pid_; }

float Process::CpuUtilization() { return cpu_utilization_; }

float Process::CpuUtilization() const { return cpu_utilization_; }

string Process::Command() { return command_; }

string Process::Ram() { return ram_; }

string Process::User() { return user_; }

long int Process::UpTime() { return up_time_; }

bool Process::operator<(Process const& a) const {
    return this->cpu_utilization_< a.CpuUtilization();
}



