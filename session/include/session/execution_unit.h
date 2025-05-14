#pragma once
#include <unordered_map>
#include <ops/ops.h>

namespace cheeky::session {
    class ExecutionUnit {
        using State = core::State;
    private:
        // 0 < opcode < 16
        // TODO: implement decision tree for efficient lookup
        std::unordered_multimap<uint8_t, std::shared_ptr<ops::BaseOperation>> _ops;
        std::shared_ptr<State> _state;

        std::shared_ptr<ops::BaseOperation> find_operation(uint32_t bytes);
    public:

        ExecutionUnit(std::shared_ptr<State> state);

        // Returns 1 if there is more instructions to execute, 0 - otherwise
        bool execute(uint32_t bytes);
    };
}