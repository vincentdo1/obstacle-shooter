#pragma once
#include <string>
#include <vector>
#include <array>
#include "cinder/gl/gl.h"
#include "game_details.h"

namespace game {

class GameContainer {
 public:
    glm::vec2 box_left_dimension_;
    glm::vec2 box_right_dimension_;

    struct Obstacle {
        public:
            char* color_;
            glm::vec2 position_;
            glm::vec2 bottom_left_corner_;
            glm::vec2 upper_right_corner_;
            glm::vec2 velocity_;
            float height_;
            float width_;

            Obstacle();

            Obstacle(std::string color, glm::vec2 position, glm::vec2 velocity, float height, float width);

        };

    struct PowerUp {
        public:
            glm::vec2 position_;
            glm::vec2 velocity_;
            float radius_ = 10;
            std::string color_;
            std::string name_;
            bool active_ = false;
            bool obtained_ = false;
        };
    GameContainer();

    /**
    * Displays the container walls and the current positions of the obtsacles and the player.
    */
    void Display();

    /**
     * Updates the positions and velocities of all obstacles and the player.
     */
    void AdvanceOneFrame();

    /**
     * This function assigns the dimensions of an obstacle. Mainly used by test cases.
     */
    void AssignObstacleDimensions();

    /**
     * This function determines if there is an obstacle and player collision.
     * @return a boolean
     */
    bool PlayerCollision();

    /**
     * This function displays the phrase "GAME OVER" if the player collided with an obstacle.
     */
    void DisplayGameOver();

    /**
     * This function determines if all obstacles have been avoided in the level.
     * @return a boolean
     */
    bool NextLevel();

    /**
     * This function uses collision algorithm for when the player collides with the wall.
     */
    void WallCollision();

    /**
     * This function assigns power-ups randomly (10% chance for a power to appear).
     */
    void AssignPowerUps();

    /**
     * This function determines if the player has obtained the power-up.
     */
    void ObtainPowerUp();
    /**
     * This function randomly creates an Obstacle.
     * @return std::vector<Obstacle>
     */

    /**
     * This function displays the current power-ups, time, etc.
     */
    void DisplayGameFeatures();

    /**
     * This function exists to simply make the code in AdvanceOneFrame() look cleaner.
     */
    void AdvanceFrameHelper();

    std::vector<Obstacle> GenerateRandomObstacles();

    GameDetails getGameDetails() {
        return game_details_;
    }

    void setGameDetails(GameDetails game_details);

    PowerUp getPowerUp() {
        return power_up_;
    }

    void setPowerUp(PowerUp power_up);

    std::vector<Obstacle> getObstacles() {
        return obstacles_;
    }

private:
    GameDetails game_details_;
    std::vector<Obstacle> obstacles_;
    PowerUp power_up_;
    int difficulty_level_ = 0;
    int time_ = 0;
    int static const kTimeLimit = 500;
};

}  // namespace game

