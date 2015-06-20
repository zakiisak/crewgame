#include "util_funcs.h"
#include <iostream>
#include <stdarg.h>
#include <sstream>
#include <string>

namespace zakix {
	const char* toString(const char* arg0, ...) {
		char		text[256];
		va_list		ap;

		if (arg0 == NULL)
			*text = 0;
		else {
			va_start(ap, arg0);
			vsprintf_s(text, arg0, ap);
			va_end(ap);
		}

		return text;
	}

	std::string toStringSTD(const char* arg0, ...) {
		char		text[256];
		va_list		ap;

		if (arg0 == NULL)
			*text = 0;
		else {
			va_start(ap, arg0);
			vsprintf_s(text, arg0, ap);
			va_end(ap);
		}

		std::string str(text);

		return str;
	}

	std::vector<std::string> split_string(const std::string &s, char delimeter) {
		std::vector<std::string> elems;
		std::stringstream ss(s);
		std::string item;
		while (std::getline(ss, item, delimeter)) {
			elems.push_back(item);
		}
		return elems;
	}
}