#include <cstring>
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>

#include "byte_buffer.hpp"

TEST(byte_buffer_unit_tests, default_construct)
{
	const byte_buffer::ByteBuffer buffer;

	ASSERT_EQ(buffer.data().data(), nullptr);
	ASSERT_EQ(buffer.data().size(), 0);
	ASSERT_EQ(buffer.size(), 0);
	ASSERT_TRUE(buffer.empty());
}

TEST(byte_buffer_unit_tests, construct_with_input_data)
{
	constexpr uint8_t expectedData[]{0x1, 0x2, 0x3};
	constexpr auto expectedDataSize{std::size(expectedData)};

	const byte_buffer::ByteBuffer buffer({expectedData, expectedDataSize});

	ASSERT_EQ(buffer.size(), expectedDataSize);
	ASSERT_EQ(buffer.data().size(), expectedDataSize);
	ASSERT_EQ(std::memcmp(expectedData, buffer.data().data(), expectedDataSize), 0);
}

TEST(byte_buffer_unit_tests, copy_construct)
{
	constexpr uint8_t expectedData[]{0x1, 0x2, 0x3};
	constexpr auto expectedDataSize{std::size(expectedData)};

	const byte_buffer::ByteBuffer bufferOld({expectedData, expectedDataSize});
	const byte_buffer::ByteBuffer bufferNew(bufferOld);

	ASSERT_EQ(bufferOld.size(), expectedDataSize);
	ASSERT_EQ(bufferOld.data().size(), expectedDataSize);
	ASSERT_EQ(std::memcmp(expectedData, bufferOld.data().data(), expectedDataSize), 0);

	ASSERT_EQ(bufferNew.size(), expectedDataSize);
	ASSERT_EQ(bufferNew.data().size(), expectedDataSize);
	ASSERT_EQ(std::memcmp(expectedData, bufferNew.data().data(), expectedDataSize), 0);
}

TEST(byte_buffer_unit_tests, move_construct)
{
	constexpr uint8_t expectedData[]{0x1, 0x2, 0x3};
	constexpr auto expectedDataSize{std::size(expectedData)};

	byte_buffer::ByteBuffer bufferOld({expectedData, expectedDataSize});
	const byte_buffer::ByteBuffer bufferNew(std::move(bufferOld));

	ASSERT_EQ(bufferOld.size(), 0);
	ASSERT_EQ(bufferOld.data().size(), 0);
	ASSERT_EQ(bufferOld.data().data(), nullptr);

	ASSERT_EQ(bufferNew.size(), expectedDataSize);
	ASSERT_EQ(bufferNew.data().size(), expectedDataSize);
	ASSERT_EQ(std::memcmp(expectedData, bufferNew.data().data(), expectedDataSize), 0);
}

TEST(byte_buffer_unit_tests, copy_assignment_operator)
{
	constexpr uint8_t expectedData[]{0x1, 0x2, 0x3};
	constexpr auto expectedDataSize{std::size(expectedData)};

	const byte_buffer::ByteBuffer bufferOld({expectedData, expectedDataSize});

	constexpr uint8_t bufferNewData[]{0x4, 0x5};
	constexpr auto bufferNewSize{std::size(bufferNewData)};
	byte_buffer::ByteBuffer bufferNew({bufferNewData, bufferNewSize});

	bufferNew = bufferOld;

	ASSERT_EQ(bufferOld.size(), expectedDataSize);
	ASSERT_EQ(bufferOld.data().size(), expectedDataSize);
	ASSERT_EQ(std::memcmp(expectedData, bufferOld.data().data(), expectedDataSize), 0);

	ASSERT_EQ(bufferNew.size(), expectedDataSize);
	ASSERT_EQ(bufferNew.data().size(), expectedDataSize);
	ASSERT_EQ(std::memcmp(expectedData, bufferNew.data().data(), expectedDataSize), 0);
}

TEST(byte_buffer_unit_tests, move_assignment_operator)
{
	constexpr uint8_t expectedData[]{0x1, 0x2, 0x3};
	constexpr auto expectedDataSize{std::size(expectedData)};

	byte_buffer::ByteBuffer bufferOld({expectedData, expectedDataSize});

	constexpr uint8_t bufferNewData[]{0x4, 0x5};
	constexpr auto bufferNewSize{std::size(bufferNewData)};
	byte_buffer::ByteBuffer bufferNew({bufferNewData, bufferNewSize});

	bufferNew = std::move(bufferOld);

	ASSERT_EQ(bufferOld.size(), 0);
	ASSERT_EQ(bufferOld.data().size(), 0);
	ASSERT_EQ(bufferOld.data().data(), nullptr);

	ASSERT_EQ(bufferNew.size(), expectedDataSize);
	ASSERT_EQ(bufferNew.data().size(), expectedDataSize);
	ASSERT_EQ(std::memcmp(expectedData, bufferNew.data().data(), expectedDataSize), 0);
}

TEST(byte_buffer_unit_tests, reserve)
{
	constexpr uint8_t expectedData[]{0x1, 0x2, 0x3};
	constexpr auto expectedDataSize{std::size(expectedData)};

	byte_buffer::ByteBuffer buffer;
	buffer.fill({expectedData, expectedDataSize}, byte_buffer::FillingMode::truncate);
	buffer.reserve(200);

	ASSERT_EQ(buffer.size(), expectedDataSize);
	ASSERT_EQ(buffer.data().size(), expectedDataSize);
	ASSERT_EQ(std::memcmp(expectedData, buffer.data().data(), expectedDataSize), 0);
	ASSERT_FALSE(buffer.empty());
}

TEST(byte_buffer_unit_tests, fill_truncate_empty_buffer)
{
	constexpr uint8_t expectedData[]{0x1, 0x2, 0x3};
	constexpr auto expectedDataSize{std::size(expectedData)};

	byte_buffer::ByteBuffer buffer;
	buffer.fill({expectedData, expectedDataSize}, byte_buffer::FillingMode::truncate);

	ASSERT_EQ(buffer.size(), expectedDataSize);
	ASSERT_EQ(buffer.data().size(), expectedDataSize);
	ASSERT_EQ(std::memcmp(expectedData, buffer.data().data(), expectedDataSize), 0);
	ASSERT_FALSE(buffer.empty());
}

TEST(byte_buffer_unit_tests, fill_truncate_not_empty_buffer_without_reallocation)
{
	constexpr uint8_t oldData[]{0x1, 0x2, 0x3};
	constexpr auto oldDataSize{std::size(oldData)};

	byte_buffer::ByteBuffer buffer;
	buffer.fill({oldData, oldDataSize}, byte_buffer::FillingMode::truncate);

	constexpr uint8_t expectedData[]{0x4, 0x5};
	constexpr auto expectedDataSize{std::size(expectedData)};

	buffer.fill({expectedData, expectedDataSize}, byte_buffer::FillingMode::truncate);

	ASSERT_EQ(buffer.size(), expectedDataSize);
	ASSERT_EQ(buffer.data().size(), expectedDataSize);
	ASSERT_EQ(std::memcmp(expectedData, buffer.data().data(), expectedDataSize), 0);
	ASSERT_FALSE(buffer.empty());
}

TEST(byte_buffer_unit_tests, fill_truncate_not_empty_buffer_with_reallocation)
{
	constexpr uint8_t oldData[]{0x1};
	constexpr auto oldDataSize{std::size(oldData)};

	byte_buffer::ByteBuffer buffer;
	buffer.fill({oldData, oldDataSize}, byte_buffer::FillingMode::truncate);

	constexpr uint8_t expectedData[]{0x4, 0x5, 0x6, 0x7, 0x8};
	constexpr auto expectedDataSize{std::size(expectedData)};

	buffer.fill({expectedData, expectedDataSize}, byte_buffer::FillingMode::truncate);

	ASSERT_EQ(buffer.size(), expectedDataSize);
	ASSERT_EQ(buffer.data().size(), expectedDataSize);
	ASSERT_EQ(std::memcmp(expectedData, buffer.data().data(), expectedDataSize), 0);
	ASSERT_FALSE(buffer.empty());
}

TEST(byte_buffer_unit_tests, fill_append_empty_buffer)
{
	constexpr uint8_t expectedData[]{0x1, 0x2};
	constexpr auto expectedDataSize{std::size(expectedData)};

	byte_buffer::ByteBuffer buffer;
	buffer.fill({expectedData, expectedDataSize}, byte_buffer::FillingMode::append);

	ASSERT_EQ(buffer.size(), expectedDataSize);
	ASSERT_EQ(buffer.data().size(), expectedDataSize);
	ASSERT_EQ(std::memcmp(expectedData, buffer.data().data(), expectedDataSize), 0);
	ASSERT_FALSE(buffer.empty());
}

TEST(byte_buffer_unit_tests, fill_append_not_empty_buffer_without_reallocation)
{
	constexpr uint8_t oldData[]{0x1, 0x2};
	constexpr auto oldDataSize{std::size(oldData)};

	byte_buffer::ByteBuffer buffer;
	buffer.reserve(100);
	buffer.fill({oldData, oldDataSize}, byte_buffer::FillingMode::append);

	constexpr uint8_t newData[]{0x4, 0x5, 0x6};
	constexpr auto newDataSize{std::size(newData)};

	buffer.fill({newData, newDataSize}, byte_buffer::FillingMode::append);

	constexpr uint8_t expectedData[]{0x1, 0x2, 0x4, 0x5, 0x6};
	constexpr auto expectedDataSize{std::size(expectedData)};

	ASSERT_EQ(buffer.size(), expectedDataSize);
	ASSERT_EQ(buffer.data().size(), expectedDataSize);
	ASSERT_EQ(std::memcmp(expectedData, buffer.data().data(), expectedDataSize), 0);
	ASSERT_FALSE(buffer.empty());
}

TEST(byte_buffer_unit_tests, fill_append_not_empty_buffer_with_reallocation)
{
	constexpr uint8_t oldData[]{0x1, 0x2};
	constexpr auto oldDataSize{std::size(oldData)};

	byte_buffer::ByteBuffer buffer;
	buffer.fill({oldData, oldDataSize}, byte_buffer::FillingMode::append);

	constexpr uint8_t newData[]{0x4, 0x5, 0x6};
	constexpr auto newDataSize{std::size(newData)};

	buffer.fill({newData, newDataSize}, byte_buffer::FillingMode::append);

	constexpr uint8_t expectedData[]{0x1, 0x2, 0x4, 0x5, 0x6};
	constexpr auto expectedDataSize{std::size(expectedData)};

	ASSERT_EQ(buffer.size(), expectedDataSize);
	ASSERT_EQ(buffer.data().size(), expectedDataSize);
	ASSERT_EQ(std::memcmp(expectedData, buffer.data().data(), expectedDataSize), 0);
	ASSERT_FALSE(buffer.empty());
}

TEST(byte_buffer_unit_tests, fill_truncate_empty_buffer_from_file)
{
	constexpr auto fileName{"test"};
	constexpr uint8_t expectedData[]{0x1, 0x2, 0x3};
	constexpr auto expectedDataSize{std::size(expectedData)};

	{
		std::ofstream file(fileName);
		file.write(reinterpret_cast<const char*>(expectedData), expectedDataSize);
	}

	byte_buffer::ByteBuffer buffer;

	{
		std::ifstream file(fileName);
		buffer.fill(file, expectedDataSize, byte_buffer::FillingMode::truncate);
	}

	ASSERT_EQ(buffer.size(), expectedDataSize);
	ASSERT_EQ(buffer.data().size(), expectedDataSize);
	ASSERT_EQ(std::memcmp(expectedData, buffer.data().data(), expectedDataSize), 0);
	ASSERT_FALSE(buffer.empty());

	std::filesystem::remove(fileName);
}

TEST(byte_buffer_unit_tests, fill_truncate_not_empty_buffer_without_reallocation_from_file)
{
	constexpr uint8_t oldData[]{0x1, 0x2, 0x3};
	constexpr auto oldDataSize{std::size(oldData)};

	byte_buffer::ByteBuffer buffer;
	buffer.fill({oldData, oldDataSize}, byte_buffer::FillingMode::truncate);

	constexpr uint8_t expectedData[]{0x4, 0x5};
	constexpr auto expectedDataSize{std::size(expectedData)};
	constexpr auto fileName{"test"};

	{
		std::ofstream file(fileName);
		file.write(reinterpret_cast<const char*>(expectedData), expectedDataSize);
	}

	{
		std::ifstream file(fileName);
		buffer.fill(file, expectedDataSize, byte_buffer::FillingMode::truncate);
	}

	ASSERT_EQ(buffer.size(), expectedDataSize);
	ASSERT_EQ(buffer.data().size(), expectedDataSize);
	ASSERT_EQ(std::memcmp(expectedData, buffer.data().data(), expectedDataSize), 0);
	ASSERT_FALSE(buffer.empty());

	std::filesystem::remove(fileName);
}

TEST(byte_buffer_unit_tests, fill_truncate_not_empty_buffer_with_reallocation_from_file)
{
	constexpr uint8_t oldData[]{0x1};
	constexpr auto oldDataSize{std::size(oldData)};

	byte_buffer::ByteBuffer buffer;
	buffer.fill({oldData, oldDataSize}, byte_buffer::FillingMode::truncate);

	constexpr uint8_t expectedData[]{0x4, 0x5, 0x6, 0x7, 0x8};
	constexpr auto expectedDataSize{std::size(expectedData)};
	constexpr auto fileName{"test"};

	{
		std::ofstream file(fileName);
		file.write(reinterpret_cast<const char*>(expectedData), expectedDataSize);
	}

	{
		std::ifstream file(fileName);
		buffer.fill(file, expectedDataSize, byte_buffer::FillingMode::truncate);
	}

	ASSERT_EQ(buffer.size(), expectedDataSize);
	ASSERT_EQ(buffer.data().size(), expectedDataSize);
	ASSERT_EQ(std::memcmp(expectedData, buffer.data().data(), expectedDataSize), 0);
	ASSERT_FALSE(buffer.empty());

	std::filesystem::remove(fileName);
}

TEST(byte_buffer_unit_tests, fill_append_empty_buffer_from_file)
{
	constexpr uint8_t expectedData[]{0x1, 0x2};
	constexpr auto expectedDataSize{std::size(expectedData)};
	constexpr auto fileName{"test"};

	{
		std::ofstream file(fileName);
		file.write(reinterpret_cast<const char*>(expectedData), expectedDataSize);
	}

	byte_buffer::ByteBuffer buffer;

	{
		std::ifstream file(fileName);
		buffer.fill(file, expectedDataSize, byte_buffer::FillingMode::truncate);
	}

	ASSERT_EQ(buffer.size(), expectedDataSize);
	ASSERT_EQ(buffer.data().size(), expectedDataSize);
	ASSERT_EQ(std::memcmp(expectedData, buffer.data().data(), expectedDataSize), 0);
	ASSERT_FALSE(buffer.empty());

	std::filesystem::remove(fileName);
}

TEST(byte_buffer_unit_tests, fill_append_not_empty_buffer_without_reallocation_from_file)
{
	constexpr uint8_t oldData[]{0x1, 0x2};
	constexpr auto oldDataSize{std::size(oldData)};

	byte_buffer::ByteBuffer buffer;
	buffer.reserve(100);
	buffer.fill({oldData, oldDataSize}, byte_buffer::FillingMode::append);

	constexpr uint8_t newData[]{0x4, 0x5, 0x6};
	constexpr auto newDataSize{std::size(newData)};
	constexpr auto fileName{"test"};

	{
		std::ofstream file(fileName);
		file.write(reinterpret_cast<const char*>(newData), newDataSize);
	}

	{
		std::ifstream file(fileName);
		buffer.fill(file, newDataSize, byte_buffer::FillingMode::append);
	}

	constexpr uint8_t expectedData[]{0x1, 0x2, 0x4, 0x5, 0x6};
	constexpr auto expectedDataSize{std::size(expectedData)};

	ASSERT_EQ(buffer.size(), expectedDataSize);
	ASSERT_EQ(buffer.data().size(), expectedDataSize);
	ASSERT_EQ(std::memcmp(expectedData, buffer.data().data(), expectedDataSize), 0);
	ASSERT_FALSE(buffer.empty());

	std::filesystem::remove(fileName);
}

TEST(byte_buffer_unit_tests, fill_append_not_empty_buffer_with_reallocation_from_file)
{
	constexpr uint8_t oldData[]{0x1, 0x2};
	constexpr auto oldDataSize{std::size(oldData)};

	byte_buffer::ByteBuffer buffer;
	buffer.fill({oldData, oldDataSize}, byte_buffer::FillingMode::append);

	constexpr uint8_t newData[]{0x4, 0x5, 0x6};
	constexpr auto newDataSize{std::size(newData)};
	constexpr auto fileName{"test"};

	{
		std::ofstream file(fileName);
		file.write(reinterpret_cast<const char*>(newData), newDataSize);
	}

	{
		std::ifstream file(fileName);
		buffer.fill(file, newDataSize, byte_buffer::FillingMode::append);
	}

	constexpr uint8_t expectedData[]{0x1, 0x2, 0x4, 0x5, 0x6};
	constexpr auto expectedDataSize{std::size(expectedData)};

	ASSERT_EQ(buffer.size(), expectedDataSize);
	ASSERT_EQ(buffer.data().size(), expectedDataSize);
	ASSERT_EQ(std::memcmp(expectedData, buffer.data().data(), expectedDataSize), 0);
	ASSERT_FALSE(buffer.empty());

	std::filesystem::remove(fileName);
}

TEST(byte_buffer_unit_tests, clear)
{
	constexpr uint8_t someData[]{0x1, 0x2};
	constexpr auto someDataSize{std::size(someData)};

	byte_buffer::ByteBuffer buffer;
	buffer.fill({someData, someDataSize}, byte_buffer::FillingMode::truncate);
	buffer.clear();

	ASSERT_EQ(buffer.size(), 0);
	ASSERT_EQ(buffer.data().size(), 0);
	ASSERT_TRUE(buffer.empty());
}

int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
