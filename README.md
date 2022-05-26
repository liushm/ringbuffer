# RingBuffer

用于进程间单向通信的缓存，基于共享内存，

> Ring Buffer rb("test", 20)
>
> rb.put("hello", 5)
>
> rb.get(buffer, 2)
>
> rb.debug()

### RingBuffer::RingBuffer(const std::string name, unsigned char exp)

* name：共享内存的名字前缀

* exp：缓存的大小，RingBuffer预留大小必须是2的整数幂，exp=20即使用1M缓存

### size_t RingBuffer::put(const void* buffer, size_t size)

* 向RingBuffer写入数据，size为写入的数量，如果RingBuffer剩余空间不足，当前线程会阻塞，直到所有内容都所写入后才会返回，返回值为总共写入的数量（即size）

### size_t RingBuffer::get(void* buffer, size_t size)

* 从RingBuffer读取数据，size为读取的数量，如果RingBuffer内已写入的数据不足以填满buffer，则当前线程会阻塞，直到buffer被填满后才会返回，返回值为读取的数量（即size）

### size_t RingBuffer::get_some(void* buffer, size_t size)

* 从RingBuffer读取数据，size为最大读取的数量，如果RingBuffer为空，则当前线程会阻塞，直到有数据被写入，当读取到数据后立即返回，返回值为读取到的数量（不超过size）