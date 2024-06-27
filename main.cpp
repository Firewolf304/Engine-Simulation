#include <iostream>
#include <filesystem>
#include <fstream>

#include "parse/config_parser.hpp"
#include "engine/engine_simulate.hpp"
#include "engine/tester.hpp"
std::string config_file_name = "input.conf";
config::data data;
int main() {

    std::cout << "Hello, World!" << std::endl;
    std::ifstream conf(config_file_name);
    if(!conf.is_open()) {
        throw std::runtime_error("No file");
    }
    double environment_temperature = 0;
    std::print("Enter environment temperature: ");
    while(!(std::cin >> environment_temperature))
    {
        std::print("Incorrect value");
    }


    try {
        config::config_parser configParser{conf};
        data = configParser.getData();
    } catch (const std::exception & e) {exit(-1);}
    data.environment_temp = environment_temperature;
    engine::engine_sim engi{data};
    engine::tester test{engi};
    long maxTime = 1000000;
    auto time = test.test(1000000);
    std::cout << "Time: " << time << std::endl;
    if(time == 1000000) {
        std::cout << "All test done, passed without overheating" << std::endl;
    } else {
        std::cout << "Test failed, need " << time << " sec. before overheating" << std::endl;
    }
    return 0;
}
