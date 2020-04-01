#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

string LinuxParser::Kernel() {
    return Internal::ParseFileForNthValueInLthLine<string>(kProcDirectory + kVersionFilename, 0, 2);
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() {
    return 1.0F - Internal::ParseFileForKey<float>(kProcDirectory + kMeminfoFilename, "MemFree:") /
            Internal::ParseFileForKey<float>(kProcDirectory + kMeminfoFilename, "MemTotal:");
}

long LinuxParser::UpTime() {
    return Internal::ParseFileForNthValueInLthLine<long>(kProcDirectory + kUptimeFilename, 0, 0);
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

long LinuxParser::ActiveJiffies() {
    return Internal::ParseFileForNthValueInLthLine<long>(kProcDirectory + kStatFilename, 0, 1) +
            Internal::ParseFileForNthValueInLthLine<long>(kProcDirectory + kStatFilename, 0, 2) +
            Internal::ParseFileForNthValueInLthLine<long>(kProcDirectory + kStatFilename, 0, 3);
}

long LinuxParser::IdleJiffies() {
    return Internal::ParseFileForNthValueInLthLine<long>(kProcDirectory + kStatFilename, 0, 4);
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

int LinuxParser::TotalProcesses() {
    return Internal::ParseFileForKey<int>(kProcDirectory + kStatFilename, "processes");
}

int LinuxParser::RunningProcesses() {
    return Internal::ParseFileForKey<int>(kProcDirectory + kStatFilename, "procs_running");
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }