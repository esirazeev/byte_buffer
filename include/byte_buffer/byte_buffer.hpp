#ifndef INCLUDE_BYTE_BUFFER_HPP
#define INCLUDE_BYTE_BUFFER_HPP

#include <cstdint>
#include <fstream>
#include <span>

namespace byte_buffer
{
class Buffer final
{
public:
	Buffer() noexcept;
	Buffer(std::span<const std::byte> bytes);
	Buffer(const Buffer&);
	Buffer(Buffer&&);
	Buffer& operator=(const Buffer&);
	Buffer& operator=(Buffer&&);
	~Buffer();

	/**
	 * @brief Increases the capacity of the buffer.
	 * 
	 * @param capacity New buffer capacity
	 */
	void reserve(uint32_t capacity);

	/**
	 * @brief Overwrites the buffer data.
	 * 
	 * @param bytes Bytes
	 */
	void overwrite(std::span<const std::byte> bytes);

	/**
	 * @brief Overwrites the buffer data from file.
	 * 
	 * @param file File object
	 * @param size File data size
	 */
	void overwrite(std::ifstream& file, uint32_t size);

	/**
	 * @brief Appends data to the buffer.
	 * 
	 * @param bytes Bytes
	 */
	void append(std::span<const std::byte> bytes);

	/**
	 * @brief Appends data to the buffer from file.
	 * 
	 * @param file File object
	 * @param size File data size
	 */
	void append(std::ifstream& file, uint32_t size);

	/**
	 * @brief Returns buffer data.
	 * 
	 * @return Buffer data
	 */
	[[nodiscard]] std::span<const std::byte> data() const noexcept;

	/**
	 * @brief Returns the size of the buffer data.
	 * 
	 * @return Buffer data size
	 */
	[[nodiscard]] uint32_t size() const noexcept;

	/**
	 * @brief Returns the size of memory currently allocated for the buffer.
	 * 
	 * @return Size of space allocated in buffer
	 */
	[[nodiscard]] uint32_t capacity() const noexcept;

	/**
	 * @brief Checks if the buffer is empty.
	 * 
	 * @return `True` if the buffer is empty, otherwise `false`
	 */
	[[nodiscard]] bool empty() const noexcept;

	/**
	 * @brief Removes all data from the buffer.
	 */
	void clear();

private:
	void destroy();
	void reallocate(uint32_t size, bool saveExistingData);
	void copy(std::span<const std::byte>, bool saveExistingData);

	std::byte* data_;
	uint32_t dataSize_;
	uint32_t capacity_;
};
} // namespace byte_buffer

#endif // INCLUDE_BYTE_BUFFER_HPP
