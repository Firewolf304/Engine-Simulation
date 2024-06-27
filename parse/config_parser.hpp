//
// Created by firewolf on 27.06.24.
//

#pragma once
#include <string>
#include <fstream>
#include <unordered_map>
#include <functional>
#include <print>

#include "data.h"


namespace config {
    class config_parser {
        config::data given;
        std::unordered_map<std::string, std::function<void(std::string &, std::ifstream &)>> symbol_map = {
                {"I", [&](std::string & key, std::ifstream & stream){
                    std::string value;
                    stream >> value;
                    given.inertion = this->parseDouble(value);
#ifdef DEBUG
                    std::print("{0}", given.inertion);
#endif
                }},
                {"M", [&](std::string & key, std::ifstream & stream){

                    given.torque = this->parseArray(stream);
#ifdef DEBUG
                    std::print("[");
                    for(auto d : given.torque) {
                        std::print("{0}, ", d);
                    }
                    std::print("]\n");
#endif
                }},
                {"V", [&](std::string & key, std::ifstream & stream){
                    given.rotation_speed = this->parseArray(stream);
#ifdef DEBUG
                    std::print("[");
                    for(auto d : given.rotation_speed) {
                        std::print("{0}, ", d);
                    }
                    std::print("]\n");
#endif
                }},
                {"T", [&](std::string & key, std::ifstream & stream){
                    std::string value;
                    stream >> value;
                    given.overheating = this->parseDouble(value);
#ifdef DEBUG
                    std::print("{0}", given.overheating);
#endif
                }},
                {"Hm", [&](std::string & key, std::ifstream & stream){
                    std::string value;
                    stream >> value;
                    given.heating_from_torque = this->parseDouble(value);
#ifdef DEBUG
                    std::print("{0}", given.heating_from_torque);
#endif
                }},
                {"Hv", [&](std::string & key, std::ifstream & stream){
                    std::string value;
                    stream >> value;
                    given.heating_from_crankshaft = this->parseDouble(value);
#ifdef DEBUG
                    std::print("{0}", given.heating_from_crankshaft);
#endif
                }},
                {"C", [&](std::string & key, std::ifstream & stream){
                    std::string value;
                    stream >> value;
                    given.cooling_from_temp = this->parseDouble(value);
#ifdef DEBUG
                    std::print("{0}", given.cooling_from_temp);
#endif
                }},
        };
    public:
        config_parser() = default;
        config_parser(std::ifstream & config) {
            std::string line;
            while(std::getline(config, line, '=')) {
#ifdef DEBUG
                std::print("{0} = ", line);
#endif
                line = replaceString(line, " ", "");
                line = replaceString(line, "\n", "");
                if(this->symbol_map.contains(line)) {
                    this->symbol_map[line](line, config);
                }
            }
#ifdef DEBUG
            std::print("\n", line);
#endif
        }
        config::data getData() {
            return this->given;
        }
    private:
        double parseDouble(std::string & line) {
            //line = replaceString(" ", "", replaceString(line, "=", ""));
            return std::stod(line.c_str());
        }
        int parseInt(std::string & line) {
            //line = replaceString(" ", "", replaceString(line, "=", ""));
            return std::atoi(line.c_str());
        }
        std::vector<double> parseArray(std::ifstream & stream) {
            char trash;
            stream >> trash; // {
            std::string value;
            std::vector<double> ret;
            std::streampos prevPos;
            while(std::getline(stream, value, ',')) {
                //stream >> value;

                ret.push_back(this->parseDouble(value));
                if(value == "}" || value.contains('}')|| value.contains('\n') ) {
                    stream.seekg(prevPos);
                    break;
                }
                prevPos = stream.tellg();
            }
            std::getline(stream, value, '\n');
            return ret;
        }
        std::string replaceString(std::string subject, const std::string& search,
                                  const std::string& replace) {
            size_t pos = 0;
            while ((pos = subject.find(search, pos)) != std::string::npos) {
                subject.replace(pos, search.length(), replace);
                pos += replace.length();
            }
            return subject;
        }
    };

}
