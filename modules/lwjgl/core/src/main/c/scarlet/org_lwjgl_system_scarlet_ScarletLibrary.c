/*
 * Copyright LWJGL. All rights reserved.
 * License terms: https://www.lwjgl.org/license
 */
#include "common_tools.h"
#include <dlfcn.h>

EXTERN_C_ENTER

JNIEXPORT jlong JNICALL Java_org_lwjgl_system_scarlet_ScarletLibrary_ndlopen(JNIEnv *env, jclass clazz, jlong name) {
    UNUSED_PARAMS(env, clazz)
    return (jlong)(uintptr_t)dlopen((const char *)(uintptr_t)name, RTLD_NOW | RTLD_GLOBAL);
}

JNIEXPORT jlong JNICALL Java_org_lwjgl_system_scarlet_ScarletLibrary_ndlsym(JNIEnv *env, jclass clazz, jlong handle, jlong name) {
    UNUSED_PARAMS(env, clazz)
    return (jlong)(uintptr_t)dlsym((void *)(uintptr_t)handle, (const char *)(uintptr_t)name);
}

JNIEXPORT jint JNICALL Java_org_lwjgl_system_scarlet_ScarletLibrary_ndlclose(JNIEnv *env, jclass clazz, jlong handle) {
    UNUSED_PARAMS(env, clazz)
    return dlclose((void *)(uintptr_t)handle);
}

JNIEXPORT jstring JNICALL Java_org_lwjgl_system_scarlet_ScarletLibrary_ndlerror(JNIEnv *env, jclass clazz) {
    UNUSED_PARAM(clazz)
    const char *error = dlerror();
    return error == NULL ? NULL : (*env)->NewStringUTF(env, error);
}

EXTERN_C_EXIT
