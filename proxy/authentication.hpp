#pragma once
#include <string>

namespace authentication {
	bool login(std::string username);
	bool createaccount(std::string username, std::string license);
}