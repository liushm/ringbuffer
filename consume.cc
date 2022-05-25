#include <iostream>
#include <cassert>
#include <windows.h>
#include <ringbuffer.h>

char buf[1024 * 1024] = "hello";

int main()
{
	RingBuffer rb("MIRROR", 20);

	rb.get_some(buf, sizeof(buf));

	std::cout << buf << std::endl;

	system("pause");
	return 0;
}