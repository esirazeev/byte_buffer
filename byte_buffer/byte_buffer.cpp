#include <cstring>

#include "byte_buffer.hpp"

namespace byte_buffer
{
ByteBuffer::ByteBuffer() noexcept : data_{}, dataSize_{}, capacity_{} {}

ByteBuffer::ByteBuffer(std::span<const uint8_t> data) : data_{}, dataSize_{}, capacity_{}
{
	copy(data);
}

ByteBuffer::ByteBuffer(const ByteBuffer& obj) : data_{}, dataSize_{}, capacity_{}
{
	copy(obj.data());
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
		copy(obj.data());
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
		allocate(capacity, true);
	}
}

void ByteBuffer::fill(std::span<const uint8_t> data, FillingMode mode)
{
	prepare_buffer_for_filling(data.size(), mode);

	std::memcpy(data_ + dataSize_, data.data(), data.size());
	dataSize_ += data.size();
}

void ByteBuffer::fill(std::ifstream& file, uint32_t size, FillingMode mode)
{
	prepare_buffer_for_filling(size, mode);

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

void ByteBuffer::allocate(uint32_t size, bool saveExistingData)
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

void ByteBuffer::copy(std::span<const uint8_t> data)
{
	if (capacity_ < data.size())
	{
		allocate(data.size(), false);
	}

	std::memcpy(data_, data.data(), data.size());
	dataSize_ = data.size();
}

void ByteBuffer::prepare_buffer_for_filling(uint32_t size, FillingMode mode)
{
	const auto saveExistingData{mode == FillingMode::append};
	const auto newCapacity{saveExistingData ? dataSize_ + size : size};

	if (capacity_ < newCapacity)
	{
		allocate(newCapacity, saveExistingData);
	}
	else
	{
		dataSize_ = saveExistingData ? dataSize_ : 0;
	}
}
} // namespace byte_buffer
