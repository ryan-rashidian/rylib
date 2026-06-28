/*
 * timepx.h - Precision timer and wait function for POSIX systems.
 *
 * Requires _POSIX_C_SOURCE >= 199309L macro for glibc. It's recommended to
 * define it youself with the preprocessor, or as a compiler option, otherwise
 * timepx must be implemented before including any C standard library headers.
 *
 * Adapted from raylib's WaitTime function: https://github.com/raysan5/raylib
 * Original: Copyright (c) 2013-2026 Ramon Santamaria (@raysan5)
 * Modified to use POSIX clock_gettime instead of platform specific timers.
 */

#ifndef TIMEPX_H
#define TIMEPX_H

double get_time(void);
void wait_time(double seconds);

#ifdef TIMEPX_IMPLEMENTATION
#if defined(__linux__) || defined(PLATFORM_WEB)
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 199309L
#endif
#include <time.h>

double get_time(void)
{
    struct timespec ts = { 0 };
    double time;

    clock_gettime(CLOCK_MONOTONIC, &ts);
    // timespec members are time_t - an integer type of at least 64 bits
    time = (double)ts.tv_sec + (double)ts.tv_nsec / 1000000000.0;

    return time;
}

void wait_time(double seconds)
{
    if (seconds < 0.0) return;

    double target_time = get_time() + seconds;
    // reserve time for busy loop
    double sleep_seconds = seconds - seconds * 0.05;

    // convert double to struct timespec for nanosleep
    struct timespec ts = { 0 };
    time_t sec = sleep_seconds;
    time_t nsec = (sleep_seconds - sec) * 1000000000L;
    ts.tv_sec = sec;
    ts.tv_nsec = nsec;

    while (nanosleep(&ts, &ts) == -1) continue;

    // busy loop spins CPU for remaining wait time
    while (get_time() < target_time) { ; }
}

#endif
#endif
#endif

