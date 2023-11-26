#include "io.h"

#include <fstream>
#include <string>
#include <nlohmann/json.hpp>
#include <iostream>

using namespace std;
using json = nlohmann::json;


const string filename = "output.csv";

void io_init(){
   ofstream temp(filename, ios::out | ios::trunc);

   string s = "Timestamp,Pressure,Thrust,Kn\n";

   temp << s;

   temp.close();
}

void io_write(string str){
    ofstream temp(filename, ios::out | std::ofstream::app);

    temp << str;

    temp.close();

}
