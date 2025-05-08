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
                if constexpr (std::is_same_v<MachObject::SegmentWithSections, std::decay_t<decltype(lc)>>) {
                    const auto seg_name = "__TEXT";
                    const auto sect_name = "__text";
                    if (!memcmp(reinterpret_cast<const void*>(lc.segment.segname), reinterpret_cast<const void*>(seg_name), strlen(seg_name))) {                        
                        for (lc_section_t section : lc.sections) {
                            if (!memcmp(reinterpret_cast<const void*>(section.sectname), reinterpret_cast<const void*>(sect_name), strlen(sect_name))) {
                                found = reinterpret_cast<const uint32_t*>(_data) + section.offset / sizeof(section.offset);
                                break;
                            }
                        }
                    }
                }
            }, lc_var);
        }

        if (!found) {
            std::cerr << "No text segment found, fatal error" << std::endl;
            std::terminate();
        }

        return found;
    }

    std::vector<uint8_t> MachObject::load_rodata() {
        return {};
    }

    uint8_t* MachObject::load_data() {
        return nullptr;
    }

    MachObject::~MachObject() {
        close(_fd);
        munmap(reinterpret_cast<void*>(_data), _data_len);
    }
}

