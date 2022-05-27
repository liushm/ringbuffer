#include <iostream>
#include <cassert>
#include <chrono>
#include <windows.h>
#include <ringbuffer.h>

int main()
{
	RingBuffer rb("MIRROR", 20);

	const size_t CNT = 1LL << 28;
	float* vertex = new float[CNT];

	for (int i = 0; i < 10; i++)
	{
		rb.get(vertex, CNT * sizeof(float));
	}

	// timestamp before transmit
	double ts;
	rb.get(&ts, sizeof(double));

	// timestamp now
	std::chrono::duration<double> tm = std::chrono::high_resolution_clock::now().time_since_epoch();

	std::cout << tm.count() - ts << std::endl;

	delete[] vertex;

	system("pause");
	return 0;
}