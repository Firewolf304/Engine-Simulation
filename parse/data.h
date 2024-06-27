//
// Created by firewolf on 27.06.24.
//

#pragma once

#include <vector>

namespace config {
    struct data final {
        double inertion = 0;
        double overheating = 0;

        double heating_from_torque = 0;
        double heating_from_crankshaft = 0;
        double cooling_from_temp = 0;

        double environment_temp = 0;
        double critical_temp = 0;

        std::vector<double> torque;
        std::vector<double> rotation_speed;

    };
}
