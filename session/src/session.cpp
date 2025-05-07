#include <session/session.h>

static const size_t MAX_INSTR_PREFETCH_SIZE = 4 * 1024;  // 4KB 

void prefetch_instructions(void* addr, size_t size) {
    madvise(addr, std::max(size, MAX_INSTR_PREFETCH_SIZE), MADV_WILLNEED | MADV_SEQUENTIAL);
}