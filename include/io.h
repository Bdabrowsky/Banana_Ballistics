#pragma once

#include <string>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

using namespace std;

void io_write(string str);

json read_propellant();

void io_init();