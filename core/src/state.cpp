#include <assert.h>
#include <string_view>

#include <core/state.h>

namespace cheeky::core {
    State::State(std::string_view name) : BaseLogger(name) {}

    // NZCV flags

    void State::set_n_flag(bool flag) {
        _nzcv.set(0, flag);
    }
    
    bool State::get_n_flag() const {
        return _nzcv.test(0); 
    }
    
    void State::set_z_flag(bool flag) {
        _nzcv.set(1, flag);
    }
    
    bool State::get_z_flag() const {
        return _nzcv.test(1); 
    }
    
    void State::set_c_flag(bool flag) {
        _nzcv.set(2, flag);
    }
    
    bool State::get_c_flag() const {
        return _nzcv.test(2); 
    }
    
    void State::set_v_flag(bool flag) {
        _nzcv.set(3, flag);
    }
    
    bool State::get_v_flag() const {
        return _nzcv.test(3); 
    }

    // PC register

    void State::advance_pc() {
        _logger.debug("advance pc");
        _pc++;
    }

    uint64_t State::get_pc() const {
        return _pc;
    }

    void State::update_pc(uint64_t new_pc) {
        _logger.debug("update pc; old: {}, new: {}", _pc, new_pc);
        _pc = new_pc;
    }

    // General-purpose registers

    uint32_t& State::get_r_ref_32(size_t idx) {
        assert(0 <= idx && idx <= 31);
        return *reinterpret_cast<uint32_t*>(&_r[idx]);
    }

    uint64_t& State::get_r_ref_64(size_t idx) {
        assert(0 <= idx && idx <= 31);
        return _r[idx];
    }

    // Virtual memory accessors 

    uint32_t& State::get_vm_with_offset_32(int32_t offset) {
        if (offset < 0 || offset > STACK_SIZE) 
            throw std::runtime_error("fatal error: stack overflow");

        return *reinterpret_cast<uint32_t*>(&_stack[offset]);
    }

    uint64_t& State::get_vm_with_offset_64(int64_t offset) {
        if (offset < 0 || offset > STACK_SIZE)
            throw std::runtime_error("fatal error: stack overflow");
        
        return *reinterpret_cast<uint64_t*>(&_stack[offset]);
    }
}