#include <assert.h>

#include <core/state.h>

namespace cheeky::core {
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
        _pc += 4;
    }

    uint64_t State::get_pc() const {
        return _pc;
    }

    // General-purpose registers

    uint64_t* State::get_r_ptr(size_t idx) {
        assert(0 <= idx && idx <= 31);
        return &_r[idx];
    }

    uint64_t& State::get_r_ref(size_t idx) {
        assert(0 <= idx && idx <= 31);
        return *get_r_ptr(idx);
    }
}