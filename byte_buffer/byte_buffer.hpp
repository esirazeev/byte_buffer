#ifndef BYTE_BUFFER_HPP
#define BYTE_BUFFER_HPP

#include <fstream>
#include <span>

namespace byte_buffer
{
enum class FillingMode : uint8_t
{
	append,
	truncate
};

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
	 * @brief Fills the buffer.
	 * 
	 * @param data Data
	 * @param mode Filling mode
	 */
	void fill(std::span<const uint8_t> data, FillingMode mode);

	/**
	 * @brief Fills a buffer from a file.
	 * 
	 * @param file File object
	 * @param size File data size
	 * @param mode Filling mode
	 */
	void fill(std::ifstream& file, uint32_t size, FillingMode mode);

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
	 * @brief Removes all data from the buffer.
	 */
	void clear();

	/**
	 * @brief Destroys the buffer and frees all resources.
	 */
	void destroy();

private:
	void allocate(uint32_t size, bool saveExistingData);
	void copy(std::span<const uint8_t> data);
	void prepare_buffer_for_filling(uint32_t size, FillingMode mode);

	uint8_t* data_;
	uint32_t dataSize_;
	uint32_t capacity_;
};
} // namespace byte_buffer

#endif // BYTE_BUFFER_HPP
