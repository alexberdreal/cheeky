#pragma once
#include <ops/base_op.h>

/// 

namespace cheeky::ops {
    template <>
    class BaseOperation<OpName::add_shft_ext, 0b100010, 2, 1> {
        static constexpr uint32_t base_mask = 

        bool validate(uint32_t data) {
            if (data & ext_mask != ext_mask) {
                return false;
            }      
        }
    };

    using OpAdd = BaseOperation<OpName::add>;
}