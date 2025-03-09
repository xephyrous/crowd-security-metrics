#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>

using namespace boost::interprocess;

/**
 * \brief Creates, maps, and sets default values to a named portion of shared memory
 * \note windows_shared_memory is destroyed when the last attached process dies
 * @param name The name of the shared memory block to create
 * @param size The size, in bytes, of the memory block to be created
 */
mapped_region createSharedMemoryBlock(const std::string& name, long long size, const int init = 0) {
    shared_memory_object block(create_only, name.c_str(), read_write);
    block.truncate(size);
    mapped_region region(block, read_write);
    std::memset(region.get_address(), init, region.get_size());
    return region;
}

/**
 * \brief Opens an existing shared memory block
 * @param name The name of the shared memory block to open
 * @return The mapped region object
 */
mapped_region openSharedMemoryBlock(const std::string& name) {
    shared_memory_object block(open_only, name.c_str(), read_write);
    mapped_region region(block, read_write);
    return region;
}

/**
 * \brief Removes a shared memory block from the system memory
 * @param name The name of the shared memory block to remove
 */
void removeSharedMemoryBlock(const std::string& name) {
    shared_memory_object::remove(name.c_str());
}

/**
 * Reads data from a set shared memory block
 * @param region The region to read from
 * @param offset The memory offset to read from the region
 * @return A void* to the memory address of the offset memory block
 */
void* readMemoryData(const mapped_region& region, const int offset, const size_t size) {
    if (offset < 0 || offset + size > region.get_size()) {
        throw std::out_of_range("Invalid memory read operation, offset out of bounds!");
    }

    return static_cast<void*>(static_cast<char*>(region.get_address()) + offset);
}

/**
 * Writes data to a shared memory block
 * @param region The region to write to
 * @param offset The offset to write at
 * @param data The data, as a void*
 * @param size The size of the data
 */
void writeMemoryData(const mapped_region& region, const int offset, const void* data, const size_t size) {
    if (offset < 0 || offset + size > region.get_size()) {
        throw std::out_of_range("Invalid memory write operation, out of bounds!");
    }

    std::memcpy(static_cast<char*>(region.get_address()) + offset, data, size);
}