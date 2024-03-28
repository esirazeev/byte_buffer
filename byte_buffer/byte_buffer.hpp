#ifndef BYTE_BUFFER_HPP
#define BYTE_BUFFER_HPP

#include <fstream>
#include <span>

namespace byte_buffer
{
class ByteBuffer final
{
public:
	ByteBuffer() noexcept;
	ByteBuffer(std::span<const uint8_t> data);
	ByteBuffer(const ByteBuffer&);
	ByteBuffer(ByteBuffer&&);
	ByteBuffer& operator=(const ByteBuffer&);
	ByteBuffer& operator=(ByteBuffer&&);
	~ByteBuffer();

	/**
	 * @brief Increases the capacity of the buffer.
	 * 
	 * @param capacity New buffer capacity
	 */
	void reserve(uint32_t capacity);

	/**
	 * @brief Overwrites the buffer data.
	 * 
	 * @param data Data
	 */
	void overwrite(std::span<const uint8_t> data);

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
	 * @param data Data
	 */
	void append(std::span<const uint8_t> data);

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
	std::span<const uint8_t> data() const noexcept;

	/**
	 * @brief Returns the size of the buffer data.
	 * 
	 * @return Buffer data size
	 */
	uint32_t size() const noexcept;

	/**
	 * @brief Returns the size of memory currently allocated for the buffer.
	 * 
	 * @return Size of space allocated in buffer
	 */
	uint32_t capacity() const noexcept;

	/**
	 * @brief Checks if the buffer is empty.
	 * 
	 * @return `True` if the buffer is empty, otherwise `false`
	 */
	bool empty() const noexcept;

	/**
	 * @brief Removes all data from the buffer.
	 */
	void clear();

private:
	void destroy();
	void reallocate(uint32_t size, bool saveExistingData);
	void copy(std::span<const uint8_t> data, bool saveExistingData);

	uint8_t* data_;
	uint32_t dataSize_;
	uint32_t capacity_;
};
} // namespace byte_buffer

#endif // BYTE_BUFFER_HPP
