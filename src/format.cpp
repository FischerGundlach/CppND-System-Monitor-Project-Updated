#include <string>

#include "format.h"
#include "date/date.h"

using std::string;

string Format::ElapsedTime(long times) { return date::format("%T", std::chrono::seconds(times)); }