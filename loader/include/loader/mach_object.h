#pragma once

#include <vector>
#include <optional>
#include <assert.h>
#include <string_view>
#include <iostream>

#include <mach-o/loader.h>

namespace cheeky::loader {
    class MachObject {

        // Load Commands:
        using header_t = mach_header_64;
        using lc_base_t = load_command;

        using lc_segment_t              = segment_command_64;
        using lc_section_t              = section_64;
        using lc_symtab_t               = symtab_command;
        using lc_dysymtab_t             = dysymtab_command;
        using lc_load_dylib_t           = dylib_command;
        using lc_id_dylib_t             = dylib_command;
        using lc_load_weak_dylib_t      = dylib_command;
        using lc_reexport_dylib_t       = dylib_command;
        using lc_load_upward_dylib_t    = dylib_command;
        using lc_load_dylinker_t        = dylinker_command;
        using lc_id_dylinker_t          = dylinker_command;
        using lc_rpath_t                = rpath_command;
        using lc_entry_point_t          = entry_point_command;
        using lc_function_starts_t      = linkedit_data_command;
        using lc_data_in_code_t         = linkedit_data_command;
        using lc_dyld_chained_fixups_t  = linkedit_data_command;
        using lc_dyld_exports_t         = linkedit_data_command;

    public:
        struct SegmentWithSections {
            lc_segment_t segment;
            std::vector<lc_section_t> sections;
        };

        ~MachObject();

        /// Loads Mach-O executable internals from a disk
        static MachObject load(std::string_view path);
        // Gets mmaped __TEXT instructions 
        std::pair<const uint32_t*, uint64_t> load_instructions();
        // Loads __TEXT rodata
        std::vector<uint8_t> load_rodata();
        // Loads __DATA segment
        uint8_t* load_data();

    private:

        // TODO: add the rest of load commands;
        using lc_variant_t = std::variant<SegmentWithSections, lc_entry_point_t>;

        /// Header;
        /// Only single-arch Mach-O format is supported atm, there must be no fat headers
        header_t _header;
        /// Load commands;
        std::vector<lc_variant_t> _load_commands;  
        
        int _fd;
        char* _data;
        size_t _data_len;

        ////////////////////////////////////////

        /// Constructor for internal usage 
        MachObject(header_t header, std::vector<lc_variant_t> load_commands, int fd, char* data, size_t len);

        /// @brief Loads load command from `data`, having its `cmd`.
        /// @param data pointer to memory where the load command resides. 
        /// @param cmd constant, can be found inside lc_base_t structure.
        /// @return lc_variant_t if load command is supported, otherwise - nullopt.
        static std::optional<lc_variant_t> load_lc_from_address_unknown_type(char* data, uint32_t cmd);

        /// @brief Loads load command with given type.
        /// @tparam LC_T one of lc_variant_t types.
        /// @param data pointer to memory where the load command resides.
        /// @return `LC_T` object
        template <typename LC_T>
        static inline LC_T load_lc_from_address_known_type(char* data) {
            constexpr size_t size = sizeof(LC_T);
            LC_T lc_out;
            memcpy(reinterpret_cast<void*>(&lc_out), reinterpret_cast<void*>(data), size);
            return lc_out;
        }
    };
}


