#include <loader/mach_object.h>
#include <cstdio> 
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <core/util.h>

auto static get_fd_with_size(std::string_view path) -> std::pair<int, size_t> {
    int fd = open(path.data(), O_RDONLY);
    struct stat st;
    int res = fstat(fd, &st);
    if (res != 0) {
        throw std::runtime_error("cannot get_fd_with_size");
    }
    return { fd, st.st_size };
}

namespace cheeky::loader {
    MachObject::MachObject(mach_header_64 header, std::vector<lc_variant_t> load_commands, int fd, char* data, size_t len) 
        : _header(std::move(header)), _load_commands(std::move(load_commands)), _fd(fd), _data(data), _data_len(len) {}

    MachObject MachObject::load(std::string_view path) {
        const auto [fd, len] = get_fd_with_size(path);

        auto data = reinterpret_cast<char*>(mmap(nullptr, len, PROT_READ, MAP_PRIVATE, fd, 0));
        auto file_start = data;
        
        // Parse headers 
        const auto header = reinterpret_cast<header_t*>(data);
        
        std::vector<lc_variant_t> load_commands;
        load_commands.reserve(header->ncmds);

        data += sizeof(header_t);

        for (size_t i = 0; i < header->ncmds; ++i) {
            const auto lc_base = reinterpret_cast<lc_base_t*>(data);
        
            auto lc_var_opt = load_lc_from_address_unknown_type(data, lc_base->cmd);

            if (lc_var_opt.has_value()) {
                auto lc_var = *lc_var_opt;
                load_commands.emplace_back(std::move(lc_var));
            }

            data += lc_base->cmdsize;
        }

        return MachObject(*header, std::move(load_commands), fd, file_start, len);
    }
        
    // Returns instructions 4-KB alligned ptr with PC offset from it
    std::pair<const uint32_t*, uint64_t> MachObject::load_instructions() {
        // pointer to 4KB-alligned TEXT segment 
        const uint32_t* found = nullptr;
        // offset of alligned memory inside a file
        std::optional<uint64_t> alligned_text_seg_off;
        // offset of initial PC relative to alligned text segment 
        std::optional<uint64_t> pc_off;

        for (const auto& lc_var : _load_commands) {
            std::visit([&, this](const auto& lc) {
                if constexpr (std::is_same_v<SegmentWithSections, std::decay_t<decltype(lc)>>) {
                    const auto seg_name = "__TEXT";
                    const auto sect_name = "__text";
                    if (!memcmp(reinterpret_cast<const void*>(lc.segment.segname), reinterpret_cast<const void*>(seg_name), strlen(seg_name))) {
                        for (lc_section_t section : lc.sections) {
                            if (!memcmp(reinterpret_cast<const void*>(section.sectname), reinterpret_cast<const void*>(sect_name), strlen(sect_name))) {
                                // data must be 4KB alligned in order to do PC-relative calculations
                                int off = 4096 - (section.offset % 4096);
                                found = reinterpret_cast<const uint32_t*>(_data) + (section.offset - off) / sizeof(uint32_t);
                                alligned_text_seg_off.emplace(section.offset - off);
                                break;
                            }
                        }
                    }
            }}, lc_var);
        }

        if (!alligned_text_seg_off.has_value() || !found) {
            std::cerr << "cannot find text segment, fatal error" << std::endl;
            std::terminate();
        }

        for (const auto& lc_var : _load_commands) {
            std::visit([&, this](const auto& lc) {
                if constexpr (std::is_same_v<MachObject::lc_entry_point_t, std::decay_t<decltype(lc)>>) {
                    pc_off = (lc.entryoff - *alligned_text_seg_off) / sizeof(uint32_t);
                } 
            }, lc_var);
        }

        if (!pc_off.has_value()) {
            std::cerr << "No entry point load command found, fatal error" << std::endl;
            std::terminate();
        }

        return { found, *pc_off };
    }

    std::optional<MachObject::lc_variant_t> MachObject::load_lc_from_address_unknown_type(char* data, uint32_t cmd) {
        switch (cmd) {
            case LC_SEGMENT_64: {
                SegmentWithSections sws;
                sws.segment = load_lc_from_address_known_type<lc_segment_t>(data);
                sws.sections.resize(sws.segment.nsects);
                assert((sws.segment.nsects * sizeof(lc_section_t) + sizeof(lc_segment_t)) == sws.segment.cmdsize);
                memcpy(reinterpret_cast<void*>(sws.sections.data()), data + sizeof(lc_segment_t), sws.segment.nsects * sizeof(lc_section_t));
                return { sws };
            }
            case LC_MAIN: {
                return load_lc_from_address_known_type<lc_entry_point_t>(data);
            }
            default:
                return std::nullopt;
        }
    }

    MachObject::~MachObject() {
        // unmap binary's memory
        munmap(reinterpret_cast<void*>(_data), _data_len);
        // free  descriptor
        close(_fd);
    }

    std::vector<uint8_t> MachObject::load_rodata() {
        return {};
    }

    uint8_t* MachObject::load_data() {
        return nullptr;
    }
}

