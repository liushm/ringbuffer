#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <chrono>
#include <windows.h>
#include <ringbuffer.h>

int main()
{
	RingBuffer rb("MIRROR", 20);

	const size_t CNT = 1LL << 28;
	float* vertex = new float[CNT];

	//
	// std::ifstream ifs("C:\\Users\\liushm\\Documents\\Unreal Projects\\test\\Saved\\Logs\\test.log");
	// std::string content((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

	// timestamp before transmit
	std::chrono::duration<double> tm = std::chrono::high_resolution_clock::now().time_since_epoch();
	double ts = tm.count();

	for (int i = 0; i < 10; i++)
	{
		rb.put(vertex, CNT * sizeof(float));
	}

	// send timestamp
	rb.put(&ts, sizeof(double));

	delete[] vertex;

	system("pause");
	return 0;
}