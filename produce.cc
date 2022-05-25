#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <windows.h>
#include <ringbuffer.h>

int main()
{
	RingBuffer rb("MIRROR", 20);

	char buf[4096] = "hello";

	//
	std::ifstream ifs("C:\\Users\\liushm\\Documents\\Unreal Projects\\test\\Saved\\Logs\\test.log");
	std::string content((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

	// std::cout << content << std::endl;

	rb.put(content.c_str(), content.size());

	system("pause");
	return 0;
}