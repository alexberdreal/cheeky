#include <session/execution_unit.h>

namespace cheeky::session {
    ExecutionUnit::ExecutionUnit(std::shared_ptr<State> state) : _state(state) {
        using namespace ops;

        _ops.emplace(AddImm{}.base_fixed_bits(), std::make_shared<AddImm>());
        _ops.emplace(AddsImm{}.base_fixed_bits(), std::make_shared<AddsImm>());
        _ops.emplace(SubImm{}.base_fixed_bits(), std::make_shared<SubImm>());
        _ops.emplace(SubsImm{}.base_fixed_bits(), std::make_shared<SubImm>());
        _ops.emplace(OrrSh{}.base_fixed_bits(), std::make_shared<OrrSh>());
        _ops.emplace(Movz{}.base_fixed_bits(), std::make_shared<Movz>());
        _ops.emplace(StrImm{}.base_fixed_bits(), std::make_shared<StrImm>());
        _ops.emplace(LdrImm{}.base_fixed_bits(), std::make_shared<LdrImm>());
        _ops.emplace(Ret{}.base_fixed_bits(), std::make_shared<Ret>());
        _ops.emplace(Bl{}.base_fixed_bits(), std::make_shared<Bl>());
        _ops.emplace(Stur{}.base_fixed_bits(), std::make_shared<Stur>());
        _ops.emplace(Stp{}.base_fixed_bits(), std::make_shared<Stp>());
        _ops.emplace(Ldur{}.base_fixed_bits(), std::make_shared<Ldur>());
        _ops.emplace(Ldp{}.base_fixed_bits(), std::make_shared<Ldp>());

        _state = std::move(state);
    }


    std::shared_ptr<ops::BaseOperation> ExecutionUnit::find_operation(uint32_t bytes) {
        auto base_bits = ops::get_base_fixed_bits(bytes);
        auto it = _ops.find(base_bits);
        while (it != _ops.end() && !it->second->is_match(bytes)) {
            it++;
        }
        return (it == _ops.end()) ? nullptr : it->second;
    }

    bool ExecutionUnit::execute(uint32_t bytes) {
        auto op = find_operation(bytes);

        if (op == nullptr) {
            std::cerr << "Instruction is not supported: " << std::hex << bytes << std::endl;
            return false;
        }

        assert(_state != nullptr);

        return op->process(bytes, *_state);
    }
}