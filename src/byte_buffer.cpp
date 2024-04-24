#include <cstring>

#include "../include/byte_buffer/byte_buffer.hpp"

namespace byte_buffer
{
Buffer::Buffer() noexcept : data_{}, dataSize_{}, capacity_{} {}

Buffer::Buffer(std::span<const std::byte> data) : data_{}, dataSize_{}, capacity_{}
{
	copy(data, false);
}

Buffer::Buffer(const Buffer& obj) : data_{}, dataSize_{}, capacity_{}
{
	copy(obj.data(), false);
}

Buffer::Buffer(Buffer&& obj) : data_{}, dataSize_{}, capacity_{}
{
	std::swap(data_, obj.data_);
	std::swap(dataSize_, obj.dataSize_);
	std::swap(capacity_, obj.capacity_);
}

Buffer& Buffer::operator=(const Buffer& obj)
{
	if (this != &obj)
	{
		copy(obj.data(), false);
	}

	return *this;
}

Buffer& Buffer::operator=(Buffer&& obj)
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

Buffer::~Buffer()
{
	destroy();
}

void Buffer::reserve(uint32_t capacity)
{
	if (capacity_ < capacity)
	{
		reallocate(capacity, true);
	}
}

void Buffer::overwrite(std::span<const std::byte> bytes)
{
	copy(bytes, false);
}

void Buffer::overwrite(std::ifstream& file, uint32_t size)
{
	if (capacity_ < size)
	{
		reallocate(size, false);
	}

	dataSize_ = file.read(reinterpret_cast<char*>(data_), size).gcount();
}

void Buffer::append(std::span<const std::byte> bytes)
{
	copy(bytes, true);
}

void Buffer::append(std::ifstream& file, uint32_t size)
{
	if (capacity_ - dataSize_ < size)
	{
		reallocate(dataSize_ + size, true);
	}

	dataSize_ += file.read(reinterpret_cast<char*>(data_ + dataSize_), size).gcount();
}

std::span<const std::byte> Buffer::data() const noexcept
{
	return {data_, dataSize_};
}

uint32_t Buffer::size() const noexcept
{
	return dataSize_;
}

uint32_t Buffer::capacity() const noexcept
{
	return capacity_;
}

bool Buffer::empty() const noexcept
{
	return dataSize_ == 0;
}

void Buffer::clear()
{
	dataSize_ = 0;
}

void Buffer::destroy()
{
	if (data_)
	{
		delete[] data_;
	}

	data_ = nullptr;
	capacity_ = 0;
	dataSize_ = 0;
}

void Buffer::reallocate(uint32_t size, bool saveExistingData)
{
	capacity_ = size;
	auto newData = new std::byte[capacity_];

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

void Buffer::copy(std::span<const std::byte> bytes, bool saveExistingData)
{
	const auto freeSpace{saveExistingData ? capacity_ - dataSize_ : capacity_};

	if (freeSpace < bytes.size())
	{
		const auto newCapacity{saveExistingData ? dataSize_ + bytes.size() : bytes.size()};
		reallocate(newCapacity, saveExistingData);
	}
	else
	{
		dataSize_ = saveExistingData ? dataSize_ : 0;
	}

	std::memcpy(data_ + dataSize_, bytes.data(), bytes.size());
	dataSize_ += bytes.size();
}
} // namespace byte_buffer
