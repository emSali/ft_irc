#include "../lib/ircserver.hpp"

std::vector<std::string> split_string(const std::string& input_string, char delimiter) {
	std::vector<std::string> tokens;
	std::stringstream ss(input_string);
	std::string token;
	
	while (std::getline(ss, token, delimiter)) {
		tokens.push_back(token);
	}
	return tokens;
}

std::string to_string(int value) {
	std::ostringstream os;
	os << value;
	return os.str();
}