#pragma once
#include <string>
#include <vector>

namespace zakix {
	const char* toString(const char* arg0, ...);
	std::string toStringSTD(const char* arg0, ...);

	std::vector<std::string> split_string(const std::string &s, char delimeter);
}