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
	 * @brief Reallocates the buffer size. All current data will be lost.
	 * 
	 * @param size New buffer size
	 */
	void reallocate(uint32_t size);

	/**
	 * @brief Fills the buffer. All current data will be lost.
	 * 
	 * @param data Data
	 */
	void fill(std::span<const uint8_t> data);

	/**
	 * @brief Fills a buffer from a file. All current data will be lost.
	 * 
	 * @param file File object
	 * @param size File data size
	 */
	void fill(std::ifstream& file, uint32_t size);

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
	 * @brief Checks if the buffer is empty.
	 * 
	 * @return `True` if the buffer is empty, otherwise `false`
	 */
	bool empty() const noexcept;

	/**
	 * @brief Clears and frees all buffer resources. All current data will be lost.
	 */
	void clear();

private:
	void allocate(uint32_t size);
	void copy(std::span<const uint8_t> data);

	uint8_t* data_;
	uint32_t size_;
	uint32_t dataSize_;
};
} // namespace byte_buffer

#endif // BYTE_BUFFER_HPP
