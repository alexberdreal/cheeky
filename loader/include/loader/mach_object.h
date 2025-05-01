#pragma once

#include <vector>
#include <optional>
#include <string_view>
#include <iostream>

#include <mach-o/loader.h>

namespace cheekydbg::loader {
    class MachObject {
        // Load Commands:
        using header_t = mach_header_64;
        using lc_base_t = load_command;

        using lc_segment_t = segment_command_64;
        using lc_src_version_t = source_version_command;
        using lc_build_version_t = build_version_command;
        // TODO: add the rest of load commands;
        using lc_variant_t = std::variant<lc_segment_t, lc_src_version_t, lc_build_version_t>;

    private:
        /// Header;
        /// Only single-arch Mach-O format is supported atm, there must be no fat headers
        header_t _header;
        /// Load commands;
        std::vector<lc_variant_t> _load_commands;  
        
        ////////////////////////////////////////

        /// Constructor for internal usage 
        MachObject(header_t header, std::vector<lc_variant_t> load_commands);

        template <typename LC_T>
        static LC_T load_lc_from_address_known_type(void* data) {
            constexpr size_t size = sizeof(LC_T);
            LC_T lc_out;
            memcpy(reinterpret_cast<void*>(&lc_out), data, size);
            return lc_out;
        }

        static std::optional<lc_variant_t> load_lc_from_address_unknown_type(void* data, int cmd) {
            switch (cmd) {
                case LC_SEGMENT_64:
                    return { load_lc_from_address_known_type<lc_segment_t>(data) };
                case LC_BUILD_VERSION:
                    return { load_lc_from_address_known_type<lc_build_version_t>(data) };
                case LC_SOURCE_VERSION:
                    return { load_lc_from_address_known_type<lc_src_version_t>(data) };
                default:
                    return std::nullopt;
            }
        }
    public: 
        /// Loads Mach-O executable internals from a disk
        static MachObject load(std::string_view path);

        void print_internals() {
            std::cout << "Header: " << std::endl;
            std::cout << "Magic num: " << _header.magic << std::endl;
            std::cout << "Cmd size: " << _header.sizeofcmds << std::endl;
            for (const auto& lc_var : _load_commands) {
                std::visit([](const auto& lc) {
                    using T = std::decay_t<decltype(lc)>;
                    if constexpr (std::is_same_v<T, lc_segment_t>) {
                        std::cout << "segname: " << lc.segname << std::endl;
                    }
                }, lc_var);
            } 
        }
    };
}


