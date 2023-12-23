#include <visualizer/game_simulation_app.h>

using game::visualizer::GameSimulationApp;

void prepareSettings(GameSimulationApp::Settings* settings) {
  settings->setResizable(false);
}

// This line is a macro that expands into an "int main()" function.
CINDER_APP(GameSimulationApp, ci::app::RendererGl, prepareSettings);
