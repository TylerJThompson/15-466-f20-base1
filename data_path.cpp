//I did not write most of this file! It was provided by Jim McCann through they Assets reading. It comes from 15-466-f19-base1.

#include "data_path.hpp"

#include <iostream>
#include <vector>
#include <sstream>

#if defined(_WIN32)
#include <windows.h>
#include <Knownfolders.h>
#include <Shlobj.h>
#include <direct.h>
#include <io.h>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#elif defined(__linux__)
#include <unistd.h>
#include <sys/stat.h>
#endif //WINDOWS


//This function gets the path to the current executable in various os-specific ways:
static std::string get_exe_path() {
#if defined(_WIN32)
	//See: https://stackoverflow.com/questions/1023306/finding-current-executables-path-without-proc-self-exe
	TCHAR buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	std::string ret = buffer;
	ret = ret.substr(0, ret.rfind('\\'));
	return ret;
#elif defined(__linux__)
	//From: https://stackoverflow.com/questions/933850/how-do-i-find-the-location-of-the-executable-in-c
	std::vector< char > buffer(1000);
	while (1) {
		ssize_t got = readlink("/proc/self/exe", &buffer[0], buffer.size());
		if (got <= 0) {
			return "";
		}
		else if (got < (ssize_t)buffer.size()) {
			std::string ret = std::string(buffer.begin(), buffer.begin() + got);
			return ret.substr(0, ret.rfind('/'));
		}
		buffer.resize(buffer.size() + 4000);
	}
#elif defined(__APPLE__)
	//From: https://stackoverflow.com/questions/799679/programmatically-retrieving-the-absolute-path-of-an-os-x-command-line-app/1024933
	uint32_t bufsize = 0;
	std::vector< char > buffer;
	_NSGetExecutablePath(&buffer[0], &bufsize);
	buffer.resize(bufsize, '\0');
	bufsize = buffer.size();
	if (_NSGetExecutablePath(&buffer[0], &bufsize) != 0) {
		throw std::runtime_error("Call to _NSGetExecutablePath failed for mysterious reasons.");
	}
	std::string ret = std::string(&buffer[0]);
	return ret.substr(0, ret.rfind('/'));
#else
#error "No idea what the OS is."
#endif
}

std::string data_path(std::string const& suffix) {
	static std::string path = get_exe_path(); //cache result of get_exe_path()
	return path + "/" + suffix;
}

//the code from here down is mine; it is for finding the path to the resources folder
std::string resources_path() {
	static std::string path = get_exe_path(); //cache result of get_exe_path()
	size_t dist = path.find("dist"); //this line is an edited version of code available at https://thispointer.com/how-to-remove-substrings-from-a-string-in-c/
	return path.substr(0, dist) + "resources/";
}