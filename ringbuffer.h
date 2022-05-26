#pragma once

class SharedMem
{
public:
	SharedMem(const std::string& name, unsigned long long size);
	virtual ~SharedMem();

	SharedMem() = delete;
	SharedMem(const SharedMem& shm) = delete;

	inline operator LPVOID() { return addr; }
	inline operator PUCHAR() { return (PUCHAR)addr; };
	inline operator PINT64() { return (PINT64)addr; }

protected:
	HANDLE hMap = INVALID_HANDLE_VALUE;
	LPVOID addr = nullptr;
};

class RingBuffer
{
public:
	RingBuffer(const std::string& name, unsigned char exp);
	virtual ~RingBuffer();

	RingBuffer() = delete;
	RingBuffer(const RingBuffer& rb) = delete;

	//
	size_t put(const void* buffer, size_t size);
	size_t get(void* buffer, size_t size);

	//
	size_t get_some(void* buffer, size_t size);
	
	void debug();

protected:
	SharedMem info;
	SharedMem buff;
	INT64 buffSize;
	INT64 buffMask;
};