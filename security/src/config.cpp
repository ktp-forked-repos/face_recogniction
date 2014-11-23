#include "config.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <boost/optional.hpp>

namespace constant {
	const std::string separator("::");
}
config::config(std::string file_path):
 path(file_path)
{ }

bool config::write(db_type db)
{
	std::ofstream config_file(path.c_str());
	if (!config_file) {
		std::cout << "could not open file to write config";
		return false;
	}

	db_type::const_iterator it;
	for(it = db.begin(); it != db.end(); ++it) {
		config_file << constant::separator << it->first << constant::separator << it->second <<
		  constant::separator <<"\n";
	}
	return true;
}

boost::optional<config::db_type> config::read()
{
	std::ifstream config_file(path.c_str());
	if (!config_file) {
		std::cout << path << "could not be open\n";
		return boost::none;
	}

	config_file_content.clear();
	while (config_file) {
		std::string line;
		std::getline(config_file, line);
		config_file_content.push_back(line);
	}
	return parse();
}

//bool config::create_default_db()
//{
		//db_type default_db;
		//default_db.insert(std::make_pair("root", "password"));
		//if (!write(default_db)) {
		//}
//}

boost::optional<config::db_type> config::parse()
{
	boost::optional<db_type> db = db_type();
	std::vector<std::string>::const_iterator it;
	for(it = config_file_content.begin(); it != config_file_content.end() -1 ; ++it) {
		if (it->size() < 2) {
			return boost::none;
		}
		std::size_t user_begin = it->find(constant::separator) + constant::separator.length();
		std::size_t user_end = it->find(constant::separator, user_begin);
		std::string user = it->substr(user_begin, user_end - user_begin);
		std::cout << "user:" << user <<":\n";

		std::size_t password_begin = it->find(constant::separator, user_end)
		 + constant::separator.length();
		std::size_t password_end = it->find(constant::separator, password_begin);
		std::string password = it->substr(password_begin, password_end - password_begin);
		std::cout << "password:" << password <<":\n";

		db->insert(std::make_pair(user, password));

	}
	return db;
}