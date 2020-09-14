//I did not write most of this file! It was provided by Jim McCann through they Assets reading. It comes from 15-466-f19-base1.

#pragma once

#include <string>

//construct a path based on the location of the currently-running executable:
// (e.g. if running /home/ix/game0/game.exe will return '/home/ix/game0/' + suffix)
std::string data_path(std::string const& suffix);

//the code from here down is mine; it is for finding the path to the resources folder
std::string resources_path();