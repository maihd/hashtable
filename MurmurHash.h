#pragma once

#include <stdint.h>

uint32_t murmurHash32(void* buffer, int length, uint32_t seed);
uint64_t murmurHash64(void* buffer, int length, uint64_t seed);