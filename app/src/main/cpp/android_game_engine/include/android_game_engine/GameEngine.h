#pragma once

#include <memory>
#include <string>

#include <jni.h>

/**
 * Macros for declaring and defining JNI functions to be called from Java
 */
#define CONCAT(a,b,c) a ## _ ## b ## _ ## c
#define EVAL_CONCAT(a,b,c) CONCAT(a,b,c)

#define JNI_METHOD_DEFINITION(return_type, method_name) \
    EVAL_CONCAT(return_type JNICALL Java, JNI_CLASS_PATH, method_name)

#define JNI_METHOD_DECLARATION(return_type, method_name) \
    JNIEXPORT JNI_METHOD_DEFINITION(return_type, method_name)

namespace age {

class Game;

/**
* Singleton game manager and C++ program entry point. Game callbacks should be run solely on the
 * rendering thread.
*/
namespace GameEngine {

Game *getGame();

/**
 * Main entry point for creating and initializing the game.
 *
 * This should be invoked by the onSurfaceCreated JNI C++ call which in turn should be invoked by
 * GLSurfaceView.Renderer.onSurfaceCreated method.
 *
 * Upon passing in a specialized instance of Game to the parameter 'g', the GameEngine will take
 * over managing the game as well as invoking the following callbacks for the Game via JNI:
 *      - Game::onCreate
 *      - Game::onStart
 *      - Game::onResume
 *      - Game::onPause
 *      - Game::onStop
 *      - Game::onDestroy
 *      - Game::onWindowChanged
 *      - Game::update
 *      - Game::render
 *      - Game::onTouchDownEvent
 *      - Game::onTouchMoveEvent
 *      - Game::onTouchUpEvent
 *
 * To create further callbacks via JNI, use the provided JNI_METHOD_DECLARATION and
 * JNI_METHOD_DEFINITION macros along with the getGame() function.
 *
 * @param width Surface width
 * @param height Surface height
 * @param displayRotation Surface rotation configuration
 * @param g Subclassed instance of Game
 */
void onSurfaceCreated(int width, int height, int displayRotation, std::unique_ptr<Game> &&g);

/**
 * Makes calls to the owning Java Activity through the JNI
 * @param methodName Java Activity method name to invoke
 * @param signature Java Activity method JNI signature
 */
void callJavaActivityVoidMethod(const std::string &methodName, const std::string &signature);

} // namespace GameEngine
} // namespace age