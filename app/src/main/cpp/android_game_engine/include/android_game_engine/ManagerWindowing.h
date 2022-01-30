#pragma once

/**
 * Singleton Windowing Manager containing info about the rendering window.
 */

namespace age {
namespace ManagerWindowing {

void init(int width, int height, int displayRotation=0);
void shutdown();

void setWindowDimensions(int width, int height);
void setDisplayRotation(int rotation);

int getWindowWidth();
int getWindowHeight();
int getDisplayRotation();

} // namespace ManagerWindowing
} // namespace age