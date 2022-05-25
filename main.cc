#include <iostream>
#include <cassert>
#include <windows.h>
#include <ringbuffer.h>

int main()
{
	RingBuffer rb("MIRROR", 10);

	char buf[4096] = "hello";

	//
	rb.put(buf, 256);
	rb.debug();

	//
	rb.get(buf, 128);
	rb.debug();

	// system("pause");
	return 0;
}
