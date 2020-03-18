#include <iostream>
#include <fcntl.h>

#include "rpigpio.h"

#ifdef __unix__

#define IsUnixOs

#include <sys/types.h>
#include <sys/mman.h>

#endif

void initialize_bcm_board(int* mem_fd)
{
    if (open_dev_mem_file(mem_fd) >= 0)
    {
        map_mem(mem_fd);
    }
    else if (open_dev_gpiomem_file(mem_fd) >= 0)
    {
        map_mem(mem_fd);
    }
    else 
    {
        std::cout << "Error occurred while opening file..." << std::endl;
    }
}

int open_dev_mem_file(int* ptr_mem_fd)
{
    return open_dev_mem_file("/dev/mem", ptr_mem_fd);
}

int open_dev_gpiomem_file(int* ptr_mem_fd)
{
    return open_dev_mem_file("/dev/gpiomem", ptr_mem_fd);
}

int open_dev_mem_file(const char* dev_mem_file_name, int* ptr_mem_fd)
{
    int fd = open_dev_mem_file(dev_mem_file_name);
    ptr_mem_fd = &fd;
    return fd;
}

int open_dev_mem_file(const char* dev_mem_file_name)
{
    std::cout << std::endl;
    std::cout << "Try to open file: " << dev_mem_file_name << "..." << std::endl;
    
    int fd = open(dev_mem_file_name, O_RDWR|O_SYNC);
    
    if (fd < 0)
    {
        std::cout << "Error while mapping physical gpio-register in virtual memory. File descriptor: " << fd << std::endl;
    }
    else
    {
        std::cout << "Successfully open file: " << dev_mem_file_name << " Descriptor: " << fd << std::endl;
    }
    
    return fd;
}

void map_mem(int* mem_fd)
{
    std::cout << std::endl;
    std::cout << "Map memory.." << std::endl;
    std::cout << "File descriptor pointer: " << *mem_fd << " value: " << &mem_fd;

    void* map = mmap(0, BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, *mem_fd, GPIO_BASE);
    
    if (map == (__caddr_t)-1)
    {
        std::cout << "Error mapping memory" << std::endl;
    }
    else
    {
        std::cout << "Successfully mapped memory" << std::endl;
    }
}

void unmap_memory(int* mem_fd)
{
    munmap(mem_fd, BLOCK_SIZE);
}