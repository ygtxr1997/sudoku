#pragma once

#include <stdint.h>
#include <time.h>
#include <vector>
#include <stack>
#include <set>
#include <unordered_set>

#define TRACE_CMH_2(fmt,...) \
	printf("%s(%d)-<%s>: "##fmt, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)

// common
typedef int8_t		INT8;
typedef int16_t		INT16;
typedef int32_t		INT32;
typedef int64_t		INT64;
typedef uint8_t		UINT8;
typedef uint16_t	UINT16;
typedef uint32_t	UINT32;
typedef uint64_t	UINT64;

// DLX相关
typedef std::vector<UINT32>					Vec;
typedef std::vector<Vec>					VVec;
typedef std::vector<std::vector<bool>>		Matrix;

// Sudoku相关
#define SD_ROW		9
#define SD_COL		9
typedef struct Sudoku
{
	Sudoku() : _ppLayout(),
		_uRow(SD_ROW),
		_uCol(SD_COL)
	{}

	UINT8	_ppLayout[SD_ROW][SD_COL];
	UINT8	_uRow;
	UINT8	_uCol;
} Sudoku, *PSudoku;