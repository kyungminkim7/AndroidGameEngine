#pragma once

#include <jni.h>

#include "GameEngine.h"

/**
 *  Main entry point for native C++ development with JNI.
 */

#define CONCAT(a,b,c) a ## _ ## b ## _ ## c
#define EVAL_CONCAT(a,b,c) CONCAT(a,b,c)

#define JNI_METHOD_DEFINITION(return_type, method_name) \
  EVAL_CONCAT(return_type JNICALL Java_com_example_androidgameengine, JAVA_ACTIVITY, method_name)

#define JNI_METHOD_DECLARATION(return_type, method_name) \
  JNIEXPORT JNI_METHOD_DEFINITION(return_type, method_name)

namespace age {
namespace JNIInterface {

void init(JNIEnv *env, jobject activity, jobject context);
void shutdown();

JNIEnv *getJNIEnv();
jobject getActivity();
jobject getContext();

} // namespace JNIInterface
} // namespace age