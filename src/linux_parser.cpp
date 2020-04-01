#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <string>
#include <vector>

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
  return Internal::ParseFileForNthValueInLthLine<string>(
      kProcDirectory + kVersionFilename, 0, 2);
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
  return 1.0F - Internal::ParseFileForKey<float>(
                    kProcDirectory + kMeminfoFilename, "MemFree:") /
                    Internal::ParseFileForKey<float>(
                        kProcDirectory + kMeminfoFilename, "MemTotal:");
}

long LinuxParser::UpTime() {
  return Internal::ParseFileForNthValueInLthLine<long>(
      kProcDirectory + kUptimeFilename, 0, 0);
}

long LinuxParser::ActiveJiffies() {
  return Internal::ParseFileForNthValueInLthLine<long>(
             kProcDirectory + kStatFilename, 0, 1) +
         Internal::ParseFileForNthValueInLthLine<long>(
             kProcDirectory + kStatFilename, 0, 2) +
         Internal::ParseFileForNthValueInLthLine<long>(
             kProcDirectory + kStatFilename, 0, 3);
}

long LinuxParser::IdleJiffies() {
  return Internal::ParseFileForNthValueInLthLine<long>(
      kProcDirectory + kStatFilename, 0, 4);
}

int LinuxParser::TotalProcesses() {
  return Internal::ParseFileForKey<int>(kProcDirectory + kStatFilename,
                                        "processes");
}

int LinuxParser::RunningProcesses() {
  return Internal::ParseFileForKey<int>(kProcDirectory + kStatFilename,
                                        "procs_running");
}

string LinuxParser::Command(int pid) {
  return Internal::ParseFileForNthValueInLthLine<string>(
      kProcDirectory + std::to_string(pid) + kCmdlineFilename, 0, 0);
}

string LinuxParser::Ram(int pid[[maybe_unused]]) {
  auto ram_in_kb = Internal::ParseFileForKey<long>(
      kProcDirectory + std::to_string(pid) + kStatusFilename, "VmSize:");
  return std::to_string(ram_in_kb / 1024);
}

string LinuxParser::Uid(int pid) {
  return Internal::ParseFileForKey<string>(
      kProcDirectory + std::to_string(pid) + kStatusFilename, "Uid:");
}

string LinuxParser::User(int pid) {
  const auto uid_of_pid = Uid(pid);

  string line;
  string name;
  string password;
  string uid;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> name >> password >> uid && uid == uid_of_pid) {
        return name;
      }
    }
  }
  return name;
}

long LinuxParser::UpTime(int pid[[maybe_unused]]) {
  auto up_time_in_clk_tck = Internal::ParseFileForNthValueInLthLine<int>(
      kProcDirectory + std::to_string(pid) + kStatFilename, 0, 21);
  return up_time_in_clk_tck / sysconf(_SC_CLK_TCK);
}

float LinuxParser::CpuUtilization(int pid[[maybe_unused]]) {
  auto u_time_in_clk_tck = Internal::ParseFileForNthValueInLthLine<long>(
      kProcDirectory + std::to_string(pid) + kStatFilename, 0, 13);
  auto s_time_in_clk_tck = Internal::ParseFileForNthValueInLthLine<long>(
      kProcDirectory + std::to_string(pid) + kStatFilename, 0, 14);
  auto cu_time_in_clk_tck = Internal::ParseFileForNthValueInLthLine<long>(
      kProcDirectory + std::to_string(pid) + kStatFilename, 0, 15);
  auto cs_time_in_clk_tck = Internal::ParseFileForNthValueInLthLine<long>(
      kProcDirectory + std::to_string(pid) + kStatFilename, 0, 16);
  auto total_time_in_s = (u_time_in_clk_tck + s_time_in_clk_tck +
                          cu_time_in_clk_tck + cs_time_in_clk_tck) /
                         sysconf(_SC_CLK_TCK);

  auto start_time_in_s =
      Internal::ParseFileForNthValueInLthLine<long>(
          kProcDirectory + std::to_string(pid) + kStatFilename, 0, 21) /
      sysconf(_SC_CLK_TCK);
  auto up_time_in_s = UpTime() - start_time_in_s;

  return static_cast<float>(total_time_in_s) / static_cast<float>(up_time_in_s);
}