#include <core/game_container.h>
#include <fstream>
#include <ctime>
#include <cstdlib>

namespace game {

    GameContainer::GameContainer() :box_left_dimension_(glm::vec2(100, 100)), box_right_dimension_(glm::vec2(400, 600)) {
        this->obstacles_ = GenerateRandomObstacles();
    }

    void GameContainer::Display() {
        //Display the player circle. Color is based on the current power-up obtained. Default color is blue.
        if (power_up_.obtained_ && power_up_.name_ == "size decrease") {
            ci::gl::color(ci::Color(power_up_.color_.c_str()));
            ci::gl::drawSolidCircle(game_details_.player_position_, game_details_.player_radius_ * 0.5F);
        } else if (power_up_.obtained_ && !power_up_.name_.empty()) {
            ci::gl::color(ci::Color(power_up_.color_.c_str()));
            ci::gl::drawSolidCircle(game_details_.player_position_, game_details_.player_radius_);
        } else {
            ci::gl::color(ci::Color("blue"));
            ci::gl::drawSolidCircle(game_details_.player_position_, game_details_.player_radius_);
        }

        //Display the power-up circle.
        if (power_up_.active_) {
            ci::gl::color(ci::Color(power_up_.color_.c_str()));
            ci::gl::drawSolidCircle(power_up_.position_, power_up_.radius_);
        }

        //Display the frames of the game box.
        ci::gl::color(ci::Color("white"));
        ci::gl::drawStrokedRect(ci::Rectf(box_left_dimension_, box_right_dimension_));

        //Display obstacles.
        for (unsigned int i = 0; i < obstacles_.size(); i++) {
            ci::gl::color(ci::Color(obstacles_[i].color_));
            obstacles_[i].bottom_left_corner_ = glm::vec2(obstacles_[i].position_.x - (obstacles_[i].width_ / 2), obstacles_[i].position_.y + (obstacles_[i].height_ / 2));
            obstacles_[i].upper_right_corner_ = glm::vec2(obstacles_[i].position_.x + (obstacles_[i].width_ / 2), obstacles_[i].position_.y - (obstacles_[i].height_ / 2));
            ci::gl::drawSolidRect(ci::Rectf(obstacles_[i].bottom_left_corner_, obstacles_[i].upper_right_corner_));
        }

        //Display game features.
        DisplayGameFeatures();
    }

    void GameContainer::AdvanceOneFrame() {
        //Modify the position of each obstacle by one frame based on their velocities.
        for (unsigned int i = 0; i < obstacles_.size(); i++) {
            obstacles_[i].velocity_.y = obstacles_[i].velocity_.y * (1 + float(difficulty_level_ / 20));
            if (power_up_.obtained_ && power_up_.name_ == "slow time") {
                obstacles_[i].position_ += obstacles_[i].velocity_ * 0.5F;
            } else {
                obstacles_[i].position_ += obstacles_[i].velocity_;
            }
        }
        //Modify player position.
        game_details_.player_position_ += game_details_.player_velocity_;

        //Check if player has collided with obstacle before executing the next logic statements.
        if (PlayerCollision()) {
            game_details_.game_over_ = true;
        }
        WallCollision();
        AdvanceFrameHelper();
    }

    std::vector<GameContainer::Obstacle> GameContainer::GenerateRandomObstacles() {
        srand (static_cast <unsigned> (time(0)));
        std::vector<GameContainer::Obstacle> obstacles;
        for (int i = 0; i < 10; i++) {
            GameContainer::Obstacle obstacle;
            obstacle.width_ = 10 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(10)));
            obstacle.height_ = 20 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(30)));
            float position_x = box_left_dimension_.x + (obstacle.width_ / 2) + static_cast <float> (rand()) /
                    ( static_cast <float> (RAND_MAX/(box_right_dimension_.x - (obstacle.width_ / 2)
                    - (box_left_dimension_.x + (obstacle.width_ / 2)))));

            obstacle.position_ = glm::vec2(position_x, 100 - obstacle.height_ / 2);
            obstacle.velocity_ = glm::vec2(0, 1 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(4))));
            obstacles.push_back(obstacle);
        }
        return obstacles;
    }

    bool GameContainer::PlayerCollision() {
        //If player has invincibility, no collision will happen.
        if (power_up_.obtained_ && power_up_.name_ == "invincibility") {
            return false;
        }
        //Check for collision for each obstacle with player.
        for (unsigned int i = 0; i < obstacles_.size(); i++) {
            //https://www.geeksforgeeks.org/check-if-any-point-overlaps-the-given-circle-and-rectangle/
            float x_nearest = std::max(obstacles_[i].bottom_left_corner_.x,
                                       std::min(game_details_.player_position_.x, obstacles_[i].upper_right_corner_.x));
            float y_nearest = std::max(obstacles_[i].upper_right_corner_.y,
                                       std::min(game_details_.player_position_.y, obstacles_[i].bottom_left_corner_.y));
            float x_difference = x_nearest - game_details_.player_position_.x;
            float y_difference = y_nearest - game_details_.player_position_.y;
            if (power_up_.obtained_ && power_up_.name_ == "size decrease") {
                if ((std::pow(x_difference, 2) + std::pow(y_difference, 2)) <=
                    std::pow(game_details_.player_radius_ * 0.5F, 2)) {
                    return true;
                }
            } else {
                if ((std::pow(x_difference, 2) + std::pow(y_difference, 2)) <=
                    std::pow(game_details_.player_radius_, 2)) {
                    return true;
                }
            }
        }
        return false;
    }

    GameContainer::Obstacle::Obstacle() {
        color_ = "red";
        position_ = glm::vec2(50, 50);
        velocity_ = glm::vec2(0, 1);
        height_ = 10;
        width_ = 5;
    }

    void GameContainer::setGameDetails(GameDetails game_details) {
        game_details_ = game_details;
    }


    bool GameContainer::NextLevel() {
        //Move to the next level if all obstacles and power-ups have moved past the game container dimensions.
        if (power_up_.position_.y + power_up_.radius_ < box_right_dimension_.y && power_up_.active_) {
            return false;
        }
        for (unsigned int i = 0; i < obstacles_.size(); i++) {
            if (obstacles_[i].upper_right_corner_.y < box_right_dimension_.y) {
                return false;
            }
        }
        power_up_.active_ = false;
        return true;
    }

    void GameContainer::WallCollision() {
        if (game_details_.player_position_[0] + game_details_.player_velocity_[0]  <= box_left_dimension_[0] && game_details_.player_velocity_[0] < 0 ||
        game_details_.player_position_[0] + game_details_.player_velocity_[0] >= box_right_dimension_[0] && game_details_.player_velocity_[0] > 0) {
            game_details_.player_velocity_[0] = game_details_.player_velocity_[0] * -1;
        }
    }

    void GameContainer::AssignPowerUps() {
        srand (unsigned (time(nullptr)));
        //choose random power up
        int power_up = rand() % 3;
        power_up_.name_ = game_details_.power_ups_[power_up];
        float position_x = box_left_dimension_.x + (power_up_.radius_) + static_cast <float> (rand()) /
                                                                           ( static_cast <float> (RAND_MAX/(box_right_dimension_.x - (power_up_.radius_)
                                                                                                            - (box_left_dimension_.x + (power_up_.radius_)))));

        power_up_.position_ = glm::vec2(position_x, 100 - power_up_.radius_ / 2);
        power_up_.velocity_ = glm::vec2(0, 3 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(8))));
        //Assign colors based on power-up.
        if (power_up == 0) {
            power_up_.color_ = "pink";
        } else if (power_up == 1) {
            power_up_.color_ = "purple";
        } else if (power_up == 2) {
            power_up_.color_ = "green";
        }
        power_up_.active_ = true;
    }

    void GameContainer::ObtainPowerUp() {
        //Calculate the velocity and position difference.
        glm::vec2 velocity_difference = game_details_.player_velocity_ - power_up_.velocity_;
        glm::vec2 position_difference = game_details_.player_position_ - power_up_.position_;
        //Calculate the dot product and distance.
        float dot_product = glm::dot(velocity_difference,position_difference);
        float distance = glm::distance(game_details_.player_position_, power_up_.position_);
        //Check distance is less than radius of particles combined and velocity is opposite directions.
        if (distance <= game_details_.player_radius_ + power_up_.radius_ && dot_product < 0) {
            //Assign power-up.
            power_up_.active_ = false;
            power_up_.obtained_ = true;
        }
    }

    void GameContainer::DisplayGameOver() {
        ci::Font font = ci::Font("GAME OVER", 20);
        ci::TextLayout text_layout;
        text_layout.setFont(font);
        ci::gl::drawStringCentered(font.getFullName(), glm::vec2(250, 150), "orange",ci::Font("GAME OVER", 30));
    }

    void GameContainer::DisplayGameFeatures() {
        //Display current power-up.
        if (power_up_.obtained_) {
            ci::gl::color(ci::Color(power_up_.color_.c_str()));
            ci::gl::drawSolidRect(ci::Rectf(glm::vec2(500, 200), glm::vec2(520, 180)));
            ci::gl::drawString("Power-up obtained: " + power_up_.name_, glm::vec2(530, 185), "white",ci::Font("", 20));
        } else if (!power_up_.obtained_ && power_up_.active_) {
            ci::gl::color(ci::Color(power_up_.color_.c_str()));
            ci::gl::drawStrokedRect(ci::Rectf(glm::vec2(500, 200), glm::vec2(520, 180)));
            ci::gl::drawString("Power-up available: " + power_up_.name_, glm::vec2(530, 185), "white",ci::Font("", 20));
        } else {
            ci::gl::color(ci::Color(power_up_.color_.c_str()));
            ci::gl::drawString("No power-up available", glm::vec2(500, 200), "white",ci::Font("", 20));
        }
        //Display current time limit.
        ci::gl::drawString("Time left until no power-up: " + std::to_string(kTimeLimit - time_), glm::vec2(500, 250), "white",ci::Font("", 20));
        //Display current level.
        ci::gl::drawString("Current level: " + std::to_string(difficulty_level_), glm::vec2(500, 300), "white",ci::Font("", 20));
    }

    void GameContainer::AdvanceFrameHelper() {
        //Check if power-up has expired.
        if (time_ >= kTimeLimit) {
            time_ = 0;
            PowerUp empty;
            power_up_ = empty;
        }

        if (power_up_.obtained_) {
            time_++;
        }

        if (power_up_.active_) {
            power_up_.position_ += power_up_.velocity_;
            ObtainPowerUp();
        }

        //Move the game to the next level.
        if (NextLevel()) {
            obstacles_ = GenerateRandomObstacles();
            if (!power_up_.active_ && time_ == 0) {
                AssignPowerUps();
            }
            difficulty_level_++;
        }
    }

    void GameContainer::AssignObstacleDimensions() {
        for (unsigned int i = 0; i < obstacles_.size(); i++) {
            obstacles_[i].bottom_left_corner_ = glm::vec2(obstacles_[i].position_.x - (obstacles_[i].width_ / 2), obstacles_[i].position_.y + (obstacles_[i].height_ / 2));
            obstacles_[i].upper_right_corner_ = glm::vec2(obstacles_[i].position_.x + (obstacles_[i].width_ / 2), obstacles_[i].position_.y - (obstacles_[i].height_ / 2));
        }
    }

    void GameContainer::setPowerUp(PowerUp power_up) {
        power_up_ = power_up;
    }
}  // namespace game