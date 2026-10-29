/* Copyright LWJGL. All rights reserved.
 * License terms: https://www.lwjgl.org/license
 * Native Scarlet has pthread keys, but its loader does not support DSO TLS. */
#pragma once
#include <pthread.h>
#include <stdlib.h>

typedef struct {
    const char *failure;
    int flip, flip_set;
    int unpremultiply, unpremultiply_set;
    int iphone, iphone_set;
} ScarletStbiThread;

static pthread_key_t scarlet_stbi_key;
static pthread_once_t scarlet_stbi_once = PTHREAD_ONCE_INIT;
static void scarlet_stbi_init(void) {
    if (pthread_key_create(&scarlet_stbi_key, free) != 0) abort();
}
static ScarletStbiThread *scarlet_stbi_thread(void) {
    if (pthread_once(&scarlet_stbi_once, scarlet_stbi_init) != 0) abort();
    ScarletStbiThread *state = pthread_getspecific(scarlet_stbi_key);
    if (state == NULL) {
        state = calloc(1, sizeof(*state));
        if (state == NULL) abort();
        if (pthread_setspecific(scarlet_stbi_key, state) != 0) {
            free(state);
            abort();
        }
    }
    return state;
}
#define stbi__g_failure_reason (scarlet_stbi_thread()->failure)
#define stbi__vertically_flip_on_load_local (scarlet_stbi_thread()->flip)
#define stbi__vertically_flip_on_load_set (scarlet_stbi_thread()->flip_set)
#define stbi__unpremultiply_on_load_local (scarlet_stbi_thread()->unpremultiply)
#define stbi__unpremultiply_on_load_set (scarlet_stbi_thread()->unpremultiply_set)
#define stbi__de_iphone_flag_local (scarlet_stbi_thread()->iphone)
#define stbi__de_iphone_flag_set (scarlet_stbi_thread()->iphone_set)
