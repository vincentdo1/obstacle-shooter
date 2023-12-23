#include <core/game_details.h>
#include <fstream>
#include <iostream>
#include <cassert>

namespace game {

    GameDetails::GameDetails() {
        player_position_ = glm::vec2(250, 600 - player_radius_);
        player_velocity_ = glm::vec2(0, 0);
    }

    GameDetails::GameDetails(float player_radius, glm::vec2 player_position, glm::vec2 player_velocity, bool game_over) {
        player_radius_ = player_radius;
        player_position_ = player_position;
        player_velocity_ = player_velocity;
        game_over_ = game_over;
    }
}  // namespace naivebayes