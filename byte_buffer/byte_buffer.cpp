#include <cstring>

#include "byte_buffer.hpp"

namespace byte_buffer
{
ByteBuffer::ByteBuffer() noexcept : data_{}, size_{}, dataSize_{} {}

ByteBuffer::ByteBuffer(const ByteBuffer& obj)
{
	allocate(obj.size());
	copy(obj.data());
}

ByteBuffer::ByteBuffer(ByteBuffer&& obj) : data_{obj.data_}, size_{obj.size_}, dataSize_{obj.dataSize_}
{
	obj.data_ = nullptr;
	obj.dataSize_ = 0;
	obj.size_ = 0;
}

ByteBuffer& ByteBuffer::operator=(const ByteBuffer& obj)
{
	if (this != &obj)
	{
		if (size_ < obj.size())
		{
			allocate(obj.size());
		}

		copy(obj.data());
	}

	return *this;
}

ByteBuffer& ByteBuffer::operator=(ByteBuffer&& obj)
{
	if (this != &obj)
	{
		destroy();

		data_ = obj.data_;
		obj.data_ = nullptr;

		dataSize_ = obj.dataSize_;
		obj.dataSize_ = 0;

		size_ = obj.size_;
		obj.size_ = 0;
	}

	return *this;
}

ByteBuffer::~ByteBuffer()
{
	destroy();
}

void ByteBuffer::reallocate(uint32_t size)
{
	allocate(size);
}

void ByteBuffer::fill(std::span<const uint8_t> data)
{
	if (size_ < data.size())
	{
		allocate(data.size());
	}

	copy(data);
}

void ByteBuffer::fill(std::ifstream& file, uint32_t size)
{
	if (size_ < size)
	{
		allocate(size);
	}

	dataSize_ = file.read(reinterpret_cast<char*>(data_), size).gcount();
}

std::span<const uint8_t> ByteBuffer::data() const noexcept
{
	return {data_, dataSize_};
}

const uint32_t ByteBuffer::size() const noexcept
{
	return dataSize_;
}

void ByteBuffer::allocate(uint32_t size)
{
	if (size_ < size)
	{
		destroy();

		data_ = new uint8_t[size];
		size_ = size;
	}

	dataSize_ = 0;
}

void ByteBuffer::copy(std::span<const uint8_t> data)
{
	std::memcpy(data_, data.data(), data.size());
	dataSize_ = data.size();
}

void ByteBuffer::destroy()
{
	if (data_)
	{
		delete[] data_;
	}
}
} // namespace byte_buffer
