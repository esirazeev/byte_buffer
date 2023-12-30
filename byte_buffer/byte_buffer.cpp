#include <cstring>

#include "byte_buffer.hpp"

namespace byte_buffer
{
ByteBuffer::ByteBuffer() noexcept : data_{}, size_{}, dataSize_{} {}

ByteBuffer::ByteBuffer(std::span<const uint8_t> data) : data_{}, size_{}, dataSize_{}
{
	copy(data);
}

ByteBuffer::ByteBuffer(const ByteBuffer& obj) : data_{}, size_{}, dataSize_{}
{
	copy(obj.data());
}

ByteBuffer::ByteBuffer(ByteBuffer&& obj) : data_{}, size_{}, dataSize_{}
{
	std::swap(data_, obj.data_);
	std::swap(size_, obj.size_);
	std::swap(dataSize_, obj.dataSize_);
}

ByteBuffer& ByteBuffer::operator=(const ByteBuffer& obj)
{
	if (this != &obj)
	{
		copy(obj.data());
	}

	return *this;
}

ByteBuffer& ByteBuffer::operator=(ByteBuffer&& obj)
{
	if (this != &obj)
	{
		clear();

		std::swap(data_, obj.data_);
		std::swap(size_, obj.size_);
		std::swap(dataSize_, obj.dataSize_);
	}

	return *this;
}

ByteBuffer::~ByteBuffer()
{
	clear();
}

void ByteBuffer::reallocate(uint32_t size)
{
	allocate(size);
}

void ByteBuffer::fill(std::span<const uint8_t> data)
{
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

uint32_t ByteBuffer::size() const noexcept
{
	return dataSize_;
}

bool ByteBuffer::empty() const noexcept
{
	return dataSize_ == 0;
}

void ByteBuffer::clear()
{
	if (data_)
	{
		delete[] data_;
	}

	data_ = nullptr;
	size_ = 0;
	dataSize_ = 0;
}

void ByteBuffer::allocate(uint32_t size)
{
	if (size_ < size)
	{
		clear();

		data_ = new uint8_t[size];
		size_ = size;
	}

	dataSize_ = 0;
}

void ByteBuffer::copy(std::span<const uint8_t> data)
{
	allocate(data.size());

	std::memcpy(data_, data.data(), data.size());
	dataSize_ = data.size();
}
} // namespace byte_buffer
