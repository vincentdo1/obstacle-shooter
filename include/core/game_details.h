#pragma once
#include <string>
#include <vector>
#include <array>
#include "cinder/gl/gl.h"

namespace game {

class GameDetails {
public:
    float player_radius_ = 10;
    glm::vec2 player_position_;
    glm::vec2 player_velocity_;
    bool game_over_ = false;
    std::vector<std::string> power_ups_ = {"invincibility", "size decrease", "slow time"};

    GameDetails();

    GameDetails(float player_radius, glm::vec2 player_position, glm::vec2 player_velocity, bool game_over);

};

}  // namespace game
