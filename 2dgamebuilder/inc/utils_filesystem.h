#ifndef UTILS_FILESYSTEM_HPP
#define UTILS_FILESYSTEM_HPP

#include "utils.h"
#include <string>
#include <vector>

namespace utils
{
	typedef std::vector<std::string> string_vector;

	bool file_exists(const std::string& file);

	bool make_directory(const std::string& sPath);
	string_vector get_directory_list(const std::string& sRelPath);
	string_vector get_file_list(const std::string& sRelPath, bool bWithPath = false);
	string_vector get_file_list(const std::string& sRelPath, bool bWithPath, const std::string& sExtensions);
	void copy_file(std::string source, std::string dest);
	std::string SplitFilename(const std::string str);
	std::string SplitPath(const std::string str);
	std::string RemoveExtension(const std::string str);
}

#endif