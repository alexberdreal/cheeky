#include <loader/mach_object.h>
#include <cstdio> 
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <utils/util.h>

auto static get_fd_with_size(std::string_view path) -> std::pair<int, size_t> {
    int fd = open(path.data(), O_RDONLY);
    struct stat st;
    int res = fstat(fd, &st);
    if (res != 0) {
        throw std::runtime_error("cannot get_fd_with_size");
    }
    return { fd, st.st_size };
}

namespace cheekydbg::loader {
    MachObject::MachObject(mach_header_64 header, std::vector<lc_variant_t> load_commands) 
        : _header(std::move(header)), _load_commands(std::move(load_commands)) {}

    MachObject MachObject::load(std::string_view path) {
        const auto [fd, len] = get_fd_with_size(path);

        auto data = reinterpret_cast<char*>(mmap(nullptr, len, PROT_READ, MAP_PRIVATE, fd, 0));
        
        // Parse headers 
        const auto header = reinterpret_cast<header_t*>(data);
        
        std::vector<lc_variant_t> load_commands;
        load_commands.reserve(header->ncmds);

        data += sizeof(header_t);

        std::cout << "B";

        for (size_t i = 0; i < header->ncmds; ++i) {
            const auto lc_base = reinterpret_cast<lc_base_t*>(data);
        
            auto lc_var_opt = load_lc_from_address_unknown_type(data, lc_base->cmd);

            if (lc_var_opt.has_value()) {
                auto lc_var = *lc_var_opt;
                load_commands.emplace_back(std::move(lc_var));
            }

            data += lc_base->cmdsize;
        }

        close(fd);

        return MachObject(*header, std::move(load_commands));
    }
}

