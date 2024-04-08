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

void CommandInfo(Client &c, std::vector<std::string> args, std::string err_nb, std::string err_msg)
{

	std::cout << CMD_ERR(c.getFd(), args[0], err_msg);
	
	std::string msg = GEN_MSG(err_nb, err_msg, c.getNickname());
		if (send(c.getFd(), msg.c_str(), msg.size(), 0) == -1)
			std::cerr << "Error: send to " << c.getFd() << std::endl;
}