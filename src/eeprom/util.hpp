#include <cstdint>
#include <optional>

namespace eeprom {
    template<typename T, std::uint32_t base_addr>
    std::optional<T> load();

    template<typename T, std::uint32_t base_addr>
    void store(const T&);

    //template<typename T, std::uint32_t base_addr>
    //class Storeable : public T {
    //    std::optional<Storeable<T>> load();
    //    void store() const;
    //}

    template<std::uint32_t base_addr>
    std::uint32_t crc();
}