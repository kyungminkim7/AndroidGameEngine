#include <chrono>
#include <memory>

#include "Game.h"
#include "Log.h"
#include "ManagerAssets.h"
#include "ManagerWindowing.h"

namespace age {
namespace GameEngine {

enum class State {RUNNING, PAUSED, QUIT};

std::unique_ptr<age::Game> game;

State state = State::PAUSED;

std::chrono::system_clock::time_point lastUpdateTime;

template <typename T>
void appCmdCallback(android_app *app, int32_t cmd);

template <typename T>
void run(android_app *app) {
    state = State::PAUSED;
    app->onAppCmd = appCmdCallback<T>;
    
    int pollId, events;
    android_poll_source* source;
    
    Log::info("Starting game loop.");
    while (state != State::QUIT) {
        // Event processing loop.
        while ((pollId = ALooper_pollAll((state == State::PAUSED ? -1 : 0),
                                         nullptr, &events,
                                         reinterpret_cast<void**> (&source))) >= 0) {
            if (source != nullptr) {
                source->process(app, source);
            }
        }
        
        if (state == State::RUNNING) {
            // Calculate update duration
            auto currentUpdateTime = std::chrono::system_clock::now();
            auto updateDuration = currentUpdateTime - lastUpdateTime;
            lastUpdateTime = currentUpdateTime;
            
            game->onUpdate(updateDuration);
            game->render();
            
            ManagerWindowing::swapBuffers();
        }
    }
    Log::info("Exiting game loop.");
}

template <typename T>
void appCmdCallback(android_app *app, int32_t cmd) {
    switch (cmd) {
        case APP_CMD_CONFIG_CHANGED:
            break;
        
        case APP_CMD_INIT_WINDOW:
            age::Log::info("Initializing window.");
            ManagerWindowing::init(app->window);
            ManagerAssets::init(app->activity->assetManager);
            game.reset(new T);
            game->init();
            state = State::RUNNING;
            break;
        
        case APP_CMD_TERM_WINDOW:
            age::Log::info("Terminating window.");
            game.reset();
            ManagerAssets::shutdown();
            ManagerWindowing::shutdown();
            break;
        
        case APP_CMD_GAINED_FOCUS:
            age::Log::info("Gaining focus.");
            break;
        
        case APP_CMD_LOST_FOCUS:
            age::Log::info("Losing focus.");
            break;
        
        case APP_CMD_LOW_MEMORY:
            break;
        
        case APP_CMD_PAUSE:
            age::Log::info("Pausing.");
            state = State::PAUSED;
            break;
        
        case APP_CMD_RESUME:
            age::Log::info("Resuming.");
            break;
        
        case APP_CMD_SAVE_STATE:
            age::Log::info("Saving state.");
            break;
        
        case APP_CMD_START:
            age::Log::info("Starting.");
            break;
        
        case APP_CMD_STOP:
            age::Log::info("Stopping.");
            break;
        
        case APP_CMD_DESTROY:
            age::Log::info("Destroying.");
            state = State::QUIT;
            break;
        
        default:
            break;
    }
}

} // namespace GameEngine
} // namespace age