#include <visualizer/game_simulation_app.h>

namespace game {

namespace visualizer {

GameSimulationApp::GameSimulationApp() {
    ci::app::setWindowSize(kWindowSize, kWindowSize);
}

void GameSimulationApp::draw() {
    ci::Color background_color("black");
    ci::gl::clear(background_color);

    game_container_.Display();
    if (game_container_.getGameDetails().game_over_) {
        game_container_.DisplayGameOver();
    }
}

void GameSimulationApp::update() {
    if (start_game_ && !game_container_.getGameDetails().game_over_) {
        game_container_.AdvanceOneFrame();
    }
    if (game_container_.getGameDetails().game_over_) {
        start_game_ = false;
        game_container_.DisplayGameOver();
    }
}

void GameSimulationApp::keyDown(ci::app::KeyEvent event) {
    GameDetails game_details = game_container_.getGameDetails();
  switch (event.getCode()) {
    case ci::app::KeyEvent::KEY_SPACE:
      start_game_ = !start_game_;
      break;

    case ci::app::KeyEvent::KEY_DELETE:
      game_container_ = GameContainer();
      break;
    case ci::app::KeyEvent::KEY_LEFT:
        game_details.player_velocity_ = glm::vec2(-2, 0);
        game_container_.setGameDetails(game_details);
      break;
    case ci::app::KeyEvent::KEY_RIGHT:
      game_details.player_velocity_ = glm::vec2(2, 0);
      game_container_.setGameDetails(game_details);
      break;
  }
}

}  // namespace GameSimulation

}  // namespace naivebayes
