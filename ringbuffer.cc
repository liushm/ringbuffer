#include <iostream>
#include <cassert>
#define NOMINMAX
#include <windows.h>

#include "ringbuffer.h"

SharedMem::SharedMem(std::string name, unsigned long long size)
{
	hMap = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, name.c_str());
	if (!hMap)
	{
		std::cout << "open failed, create it!" << std::endl;

		hMap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, size >> 32, size & 0xFFFFFFFF, name.c_str());
	}
	else
	{
		std::cout << "opened!" << std::endl;
	}

	if (hMap)
	{
		addr = MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, size);
	}

	assert(addr);
}

SharedMem::~SharedMem()
{
	UnmapViewOfFile(addr);
	CloseHandle(hMap);
}


RingBuffer::RingBuffer(std::string name, unsigned char exp) :
	info(name + "_info", 24),
	buff(name + "_data", 1LL << exp),
	buffSize(1LL << exp),
	buffMask((1LL << exp) - 1)
{
}

RingBuffer::~RingBuffer()
{
}

size_t RingBuffer::put(const void* buffer, size_t size)
{
	// check is full
	// while not full, copy data
	// update END OFFSET
	// update COUNT

	PINT64 const offsetCount = (PINT64)info;
	PINT64 const offsetStart = (PINT64)info + 1;
	PINT64 const offsetEnd = (PINT64)info + 2;

	size_t remain = size;
	do
	{
		INT64 count = InterlockedXor64(offsetCount, 0);

		if (count != buffSize)
		{
			size_t bSize = std::min((size_t)buffSize - count, remain);

			// copy date into ring buffer
			if (*offsetEnd + (INT64)bSize > buffSize)
			{
				const size_t part1 = buffSize - *offsetEnd;
				const size_t part2 = *offsetEnd + (INT64)bSize - buffSize;

				memcpy((PUCHAR)buff + *offsetEnd, (PUCHAR)buffer + size - remain, part1);
				memcpy((PUCHAR)buff, (PUCHAR)buffer + size - remain + part1, part2);
			}
			else
			{
				memcpy((PUCHAR)buff + *offsetEnd, (PUCHAR)buffer + size - remain, bSize);
			}

			// atomic is NOT necessary here
			*offsetEnd = (*offsetEnd + bSize) & buffMask;

			InterlockedAdd64(offsetCount, bSize);

			//
			remain -= bSize;
		}
		else
		{
			SwitchToThread();
		}
	}
	while (remain > 0);

	return size;
}

size_t RingBuffer::get(void* buffer, size_t size)
{
	// check is empty
	// while not empty and buffer is not full, copy data
	// update START OFFSET
	// update COUNT

	PINT64 const offsetCount = (PINT64)info;
	PINT64 const offsetStart = (PINT64)info + 1;
	PINT64 const offsetEnd = (PINT64)info + 2;

	size_t remain = size;
	do
	{
		INT64 count = InterlockedXor64(offsetCount, 0);

		if (count != 0)
		{
			size_t bSize = std::min((size_t)count, remain);

			if (*offsetStart + (INT64)bSize > buffSize)
			{
				const size_t part1 = buffSize - *offsetStart;
				const size_t part2 = *offsetStart + (INT64)bSize - buffSize;

				memcpy((PUCHAR)buffer + size - remain, (PUCHAR)buff + *offsetStart, part1);
				memcpy((PUCHAR)buffer + size - remain + part1, (PUCHAR)buff, part2);
			}
			else
			{
				memcpy((PUCHAR)buffer + size - remain, (PUCHAR)buff + *offsetStart, bSize);
			}

			// atomic is NOT necessary here
			*offsetStart = (*offsetStart + bSize) & buffMask;

			InterlockedAdd64(offsetCount, -(INT64)bSize);

			//
			remain -= bSize;
		}
		else
		{
			SwitchToThread();
		}
	}
	while (remain > 0);

	return size;
}

size_t RingBuffer::get_some(void* buffer, size_t size)
{
	PINT64 const offsetCount = (PINT64)info;
	PINT64 const offsetStart = (PINT64)info + 1;
	PINT64 const offsetEnd = (PINT64)info + 2;

	INT64 count = 0;

	// loop till received some data
	while ((count = InterlockedXor64(offsetCount, 0)) == 0)
	{
	}

	size_t bSize = std::min((size_t)count, size);

	if (*offsetStart + (INT64)bSize > buffSize)
	{
		const size_t part1 = buffSize - *offsetStart;
		const size_t part2 = *offsetStart + (INT64)bSize - buffSize;

		memcpy((PUCHAR)buffer, (PUCHAR)buff + *offsetStart, part1);
		memcpy((PUCHAR)buffer + part1, (PUCHAR)buff, part2);
	}
	else
	{
		memcpy((PUCHAR)buffer, (PUCHAR)buff + *offsetStart, bSize);
	}

	// atomic is NOT necessary here
	*offsetStart = (*offsetStart + bSize) & buffMask;

	InterlockedAdd64(offsetCount, -(INT64)bSize);

	return bSize;
}

void RingBuffer::debug()
{
	PINT64 const offsetCount = (PINT64)info;
	PINT64 const offsetStart = (PINT64)info + 1;
	PINT64 const offsetEnd = (PINT64)info + 2;

	std::cout << *offsetCount << " (" << *offsetStart << " -> " << *offsetEnd << ")" << std::endl;
}