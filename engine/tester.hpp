//
// Created by firewolf on 27.06.24.
//

#pragma once
#include "engine_simulate.hpp"

namespace engine {
    class tester {
        engine::engine_sim engi;
    public:
        tester() = default;
        tester(engine::engine_sim & engi) : engi(engi) {
        }
        long test (unsigned long time_limit) {
            unsigned long timer = 0;
            double maxN = -1;
            while(!engi.is_critical() && timer < time_limit) {
                engi.update();
                if(maxN < engi.get_engine_power()) {
                    maxN = engi.get_engine_power();
                }
                timer++;
            }
            return timer;
        }
    };
}
