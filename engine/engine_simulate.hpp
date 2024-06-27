//
// Created by firewolf on 27.06.24.
//

#pragma once
#include "../parse/data.h"

namespace engine {

    class torgue_from_speed_points {
        std::vector<double> torque;
        std::vector<double> rotation_speed;
    public:
        torgue_from_speed_points() = default;
        torgue_from_speed_points(std::vector<double> & torque, std::vector<double> & rotation_speed) : torque(torque), rotation_speed(rotation_speed) {}
        /*double get_graph_value(double torque) {
            if (torque <= this->torque.front()) {
                return this->rotation_speed;
            }
            for(int i = 0; i < this->torque.size(); i++) {
                if(torque < this->torque[i]) {

                }
            }
        }*/
        double get_graph_value(double speed) {
            if (speed <= this->rotation_speed.front()) {
                return this->torque.front();
            }
            double ret = -1;
            for(int i = 1; i < this->torque.size(); i++) {
                if(speed < this->rotation_speed[i]) {
                    const double k = (torque[i] - torque[i - 1]) / (rotation_speed[i] - rotation_speed[i - 1]);
                    const double b = torque[i] - k * rotation_speed[i];
                    ret = k * speed + b;
                    break;
                    //return torque[i-1] + (torque[i] - torque[i-1]) * ((rotation_speed[i-1] - rotation_speed[i]) / (rotation_speed[i-1] - rotation_speed[i]));
                }
            }
            return ret;
        }
    };

    class engine_sim {
        // input
        double inertion_ = 0;                   // I
        double overheating_ = 0;                // Tпер

        double heating_from_torque_ = 0;        // Hm
        double heating_from_crankshaft_ = 0;    // Hv
        double cooling_from_temp_ = 0;          // C

        double environment_temp_ = 0;
        //local state
        double acceleration_ = 0;
        double torque_ = 0;
        double rotation_speed = 0;
        double engine_temp = 0;
        double heatingRate = 0;
        double coolingRate = 0;
        double engine_power = 0;

        torgue_from_speed_points vector_data;
    public:
        engine_sim() = default;
        engine_sim(config::data data) : vector_data(data.torque, data.rotation_speed) {
            this->inertion_ = data.inertion;
            this->cooling_from_temp_ = data.cooling_from_temp;
            this->environment_temp_ = data.environment_temp;
            this->heating_from_crankshaft_ = data.heating_from_crankshaft;
            this->heating_from_torque_ = data.heating_from_torque;
            this->overheating_ = data.overheating;
            this->engine_temp = this->environment_temp_ = data.environment_temp;
        }
        void update() {
            this->torque_ = vector_data.get_graph_value(rotation_speed);
            this->acceleration_ = torque_ / inertion_;
            this->heatingRate = torque_ * heating_from_torque_ + rotation_speed * rotation_speed * heating_from_crankshaft_;
            this->coolingRate = cooling_from_temp_ * (environment_temp_ - engine_temp);
            this->rotation_speed += acceleration_;
            engine_temp += heatingRate + coolingRate;
            engine_power = torque_ * rotation_speed / 1000;
        }

        double get_engine_power () {
            return engine_power;
        }

        bool is_critical() {
            return this->engine_temp >= this->overheating_;
        }
    };
}
