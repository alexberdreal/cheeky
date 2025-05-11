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

            
    const uint32_t* MachObject::load_instructions() {
        const uint32_t* found = nullptr;

        for (auto lc_var : _load_commands) {
            std::visit([&found, this](auto& lc) {
                if constexpr (std::is_same_v<MachObject::lc_entry_point_t, std::decay_t<decltype(lc)>>) {
                    found = reinterpret_cast<const uint32_t*>(_data) + lc.entryoff / sizeof(uint32_t);
                } 
            }, lc_var);
        }

        if (!found) {
            std::cerr << "No text segment found, fatal error" << std::endl;
            std::terminate();
        }

        return found;
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

