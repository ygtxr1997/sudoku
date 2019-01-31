#pragma once

#include <stdint.h>
#include <vector>
#include <stack>
#include <set>


#define TRACE_CMH_2(fmt,...) \
	printf("%s(%d)-<%s>: "##fmt, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)

typedef int8_t		INT8;
typedef int16_t		INT16;
typedef int32_t		INT32;
typedef int64_t		INT64;
typedef uint8_t		UINT8;
typedef uint16_t	UINT16;
typedef uint32_t	UINT32;
typedef uint64_t	UINT64;

typedef std::vector<UINT32>					Vec;
typedef std::vector<std::vector<bool>>		Matrix;