#pragma once

#include <jni.h>

namespace age {
namespace ManagerJNI {

void init(JNIEnv *env, jobject activity, jobject context);
void shutdown();

JNIEnv *getJNIEnv();
jobject getActivity();
jobject getContext();

} // namespace ManagerJNI
} // namespace age