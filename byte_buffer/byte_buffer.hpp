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
	ByteBuffer(const ByteBuffer&);
	ByteBuffer(ByteBuffer&&);
	ByteBuffer& operator=(const ByteBuffer&);
	ByteBuffer& operator=(ByteBuffer&&);
	~ByteBuffer();

	/**
	 * @brief Reallocates the buffer. All current data will be lost.
	 * 
	 * @param size New buffer size
	 */
	void reallocate(uint32_t size);

	/**
	 * @brief Fills a buffer. All current data will be lost.
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
	 * @brief Returns buffer data size.
	 * 
	 * @return Buffer data size
	 */
	const uint32_t size() const noexcept;

private:
	void allocate(uint32_t size);
	void copy(std::span<const uint8_t> data);
	void destroy();

	uint8_t* data_;
	uint32_t size_;
	uint32_t dataSize_;
};

} // namespace byte_buffer

#endif // BYTE_BUFFER_HPP
