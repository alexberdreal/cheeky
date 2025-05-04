#include "utils/util.h"

namespace cheeky::utils {

constexpr uint32_t log2(uint32_t data) {
    unsigned res = -1;   
    while (data != 0) {
        data >>= 1;
        res++;
    }
    return res;
}

}