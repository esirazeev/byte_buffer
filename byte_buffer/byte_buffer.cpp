#include <cstring>

#include "byte_buffer.hpp"

namespace byte_buffer
{
ByteBuffer::ByteBuffer() noexcept : data_{}, dataSize_{}, capacity_{} {}

ByteBuffer::ByteBuffer(std::span<const uint8_t> data) : data_{}, dataSize_{}, capacity_{}
{
	copy(data, false);
}

ByteBuffer::ByteBuffer(const ByteBuffer& obj) : data_{}, dataSize_{}, capacity_{}
{
	copy(obj.data(), false);
}

ByteBuffer::ByteBuffer(ByteBuffer&& obj) : data_{}, dataSize_{}, capacity_{}
{
	std::swap(data_, obj.data_);
	std::swap(dataSize_, obj.dataSize_);
	std::swap(capacity_, obj.capacity_);
}

ByteBuffer& ByteBuffer::operator=(const ByteBuffer& obj)
{
	if (this != &obj)
	{
		copy(obj.data(), false);
	}

	return *this;
}

ByteBuffer& ByteBuffer::operator=(ByteBuffer&& obj)
{
	if (this != &obj)
	{
		destroy();

		std::swap(data_, obj.data_);
		std::swap(dataSize_, obj.dataSize_);
		std::swap(capacity_, obj.capacity_);
	}

	return *this;
}

ByteBuffer::~ByteBuffer()
{
	destroy();
}

void ByteBuffer::reserve(uint32_t capacity)
{
	if (capacity_ < capacity)
	{
		reallocate(capacity, true);
	}
}

void ByteBuffer::overwrite(std::span<const uint8_t> data)
{
	copy(data, false);
}

void ByteBuffer::overwrite(std::ifstream& file, uint32_t size)
{
	if (capacity_ < size)
	{
		reallocate(size, false);
	}

	dataSize_ = file.read(reinterpret_cast<char*>(data_), size).gcount();
}

void ByteBuffer::append(std::span<const uint8_t> data)
{
	copy(data, true);
}

void ByteBuffer::append(std::ifstream& file, uint32_t size)
{
	if (capacity_ - dataSize_ < size)
	{
		reallocate(dataSize_ + size, true);
	}

	dataSize_ += file.read(reinterpret_cast<char*>(data_ + dataSize_), size).gcount();
}

std::span<const uint8_t> ByteBuffer::data() const noexcept
{
	return {data_, dataSize_};
}

uint32_t ByteBuffer::size() const noexcept
{
	return dataSize_;
}

uint32_t ByteBuffer::capacity() const noexcept
{
	return capacity_;
}

bool ByteBuffer::empty() const noexcept
{
	return dataSize_ == 0;
}

void ByteBuffer::clear()
{
	dataSize_ = 0;
}

void ByteBuffer::destroy()
{
	if (data_)
	{
		delete[] data_;
	}

	data_ = nullptr;
	capacity_ = 0;
	dataSize_ = 0;
}

void ByteBuffer::reallocate(uint32_t size, bool saveExistingData)
{
	capacity_ = size;
	auto newData = new uint8_t[capacity_];

	if (saveExistingData)
	{
		dataSize_ = std::min(dataSize_, capacity_);
		std::memcpy(newData, data_, dataSize_);
	}
	else
	{
		dataSize_ = 0;
	}

	delete[] data_;
	data_ = newData;
}

void ByteBuffer::copy(std::span<const uint8_t> data, bool saveExistingData)
{
	const auto freeSpace{saveExistingData ? capacity_ - dataSize_ : capacity_};

	if (freeSpace < data.size())
	{
		const auto newCapacity{saveExistingData ? dataSize_ + data.size() : data.size()};
		reallocate(newCapacity, saveExistingData);
	}
	else
	{
		dataSize_ = saveExistingData ? dataSize_ : 0;
	}

	std::memcpy(data_ + dataSize_, data.data(), data.size());
	dataSize_ += data.size();
}
} // namespace byte_buffer
