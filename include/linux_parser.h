#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

namespace LinuxParser {
// Paths
const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};

// System
float MemoryUtilization();
long UpTime();
std::vector<int> Pids();
int TotalProcesses();
int RunningProcesses();
std::string OperatingSystem();
std::string Kernel();

// CPU
enum CPUStates {
  kUser_ = 0,
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_,
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_
};
std::vector<std::string> CpuUtilization();
long Jiffies();
long ActiveJiffies();
long ActiveJiffies(int pid);
long IdleJiffies();

// Processes
std::string Command(int pid);
std::string Ram(int pid);
std::string Uid(int pid);
std::string User(int pid);
long int UpTime(int pid);

namespace Internal{
    template <typename T> T ParseFileForNthValueInLthLine(const std::string& filename, const int& lth, const int& nth){
        T nthValue;
        std::string dummy;
        std::string line;
        std::ifstream stream(filename);
        if (stream.is_open()) {
            int l {0};
            while (std::getline(stream, line) && l <= lth){
                if (l == lth){
                    std::istringstream linestream(line);
                    for(auto n=0; n < nth; n++){
                        linestream >> dummy;
                    }
                    linestream >> nthValue;
                    return nthValue;
                } else l++;
            }
        }
        return nthValue;
    }
};  // namespace Internal
};  // namespace LinuxParser

#endif