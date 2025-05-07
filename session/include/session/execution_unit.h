#pragma once
#include <unordered_map>
#include <ops/ops.h>

namespace cheeky::session {
    class ExecutionUnit {
    private:
        // 0 < opcode < 63
        // TODO: change to array of priority list (the most used instructions should be first)
        std::unordered_multimap<uint8_t, std::shared_ptr<ops::BaseOperation>> _ops;
        std::shared_ptr<ops::State> _state;

        std::shared_ptr<ops::BaseOperation> find_operation(uint32_t bytes) {
            std::cout << "finding operation " << std::endl;
            auto base_bits = ops::get_base_fixed_bits(bytes);
            auto it = _ops.find(base_bits);
            while (it != _ops.end() && !it->second->is_match(bytes)) {
                it++;
            }
            return (it == _ops.end()) ? nullptr : it->second;
        }
    public:

        ExecutionUnit(std::shared_ptr<ops::State> state) : _state(state) {
            using namespace ops;

            _ops.emplace(AddImm{}.base_fixed_bits(), std::make_shared<AddImm>());
            _ops.emplace(AddsImm{}.base_fixed_bits(), std::make_shared<AddsImm>());
            _ops.emplace(SubImm{}.base_fixed_bits(), std::make_shared<SubImm>());

            _state = std::make_shared<ops::State>();
        }

        void execute(uint32_t bytes) {
            auto op = find_operation(bytes);

            if (op == nullptr) {
                std::cerr << "Instruction is not supported: " << std::hex << bytes << std::endl;
                std::terminate();
            }

            std::cout << "executing" << std::endl;

            assert(_state != nullptr);

            op->process(bytes, *_state);
        }
    };
}