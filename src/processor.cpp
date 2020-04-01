#include "linux_parser.h"
#include "processor.h"

float Processor::Utilization() {
    auto active_jiffies = static_cast<float>(LinuxParser::ActiveJiffies());
    auto idle_jiffies = static_cast<float>(LinuxParser::IdleJiffies());

    return (active_jiffies / (active_jiffies + idle_jiffies));
}