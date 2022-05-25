#include <iostream>
#include <cassert>
#include <thread>
#include <windows.h>
#include <ringbuffer.h>

int main()
{
	RingBuffer rb("MIRROR", 20);

	size_t size = 1LL << 30;
	char* src = new char[size];
	char* dst = new char[size];

	//
	std::cout << "initialize src buffer" << std::endl;
	for (size_t i = 0; i < size; i++)
	{
		src[i] = (i & 0xff);
	}
	std::cout << "initialize src buffer finished" << std::endl;

	//
	std::cout << "transmit" << std::endl;
	std::thread t1([&]() {
		rb.put(src, size);
	});

	std::thread t2([&]() {
		rb.get(dst, size);
	});

	t1.join();
	t2.join();
	std::cout << "transmit done!" << std::endl;

	std::cout << "check dst buffer" << std::endl;
	for (size_t i = 0; i < size; i++)
	{
		// printf("%08llx %02x %02x\n", i, (unsigned char)src[i], (unsigned char)dst[i]);
		
		if ((unsigned char)dst[i] != (i & 0xff))
		{
			std::cout << "check failed" << std::endl;

			printf("%08llx %02x %02x\n", i, (unsigned char)src[i], (unsigned char)dst[i]);
			break;
		}
	}
	std::cout << "check dst buffer finished" << std::endl;

	delete[] src;
	delete[] dst;

	system("pause");
	return 0;
}
