#pragma once

#include <array>
#include <string_view>
#include <cstdio>
#include <bitset>

namespace NetworkingConcepts
{

namespace __impl
{

constexpr const size_t IP4_OCTECTS_COUNT{ 4 };
constexpr const size_t IP4_OCTET_MAX_LEN{ 3 };
constexpr const char IP4_SEPARATOR{ '.' };
constexpr const size_t IP4_STRING_SIZE{ 16 };

using ip4_int_t = std::array<uint8_t, IP4_OCTECTS_COUNT>;
using ip4_bitset_t = std::bitset<IP4_OCTECTS_COUNT * 8>;

// Convert a part of the string_view to an integer
constexpr auto from_string_chars(std::string_view sv, size_t start, size_t end) noexcept -> int {
    int value{ 0 };
    for (size_t j = start; (j < end) && (j < sv.size()); ++j) {
        value = value * 10 + (sv[j] - '0');
    }
    return value;
}

// Tests for from_string_chars
static_assert(from_string_chars("255", 0, 3) == 255);
static_assert(from_string_chars("001", 0, 3) == 1);
static_assert(from_string_chars("1", 0, 1) == 1);
static_assert(from_string_chars("16", 0, 2) == 16);

// Check if the string could be an IP4 address
constexpr auto could_be_ip4_address(std::string_view sv) noexcept -> bool {
    return std::count(std::begin(sv), std::end(sv), '.') == 3 &&
           std::count(std::begin(sv), std::end(sv), ':') <= 1 &&
           std::all_of(std::begin(sv), std::end(sv), [](char c) { return std::isdigit(c) || c == '.'; });
}

// Check if the string could be an IP6 address
constexpr auto could_be_ip6_address(std::string_view sv) noexcept -> bool {
    return std::count(sv.begin(), sv.end(), ':') >= 2 &&
           std::all_of(sv.begin(), sv.end(), [](char c) { return std::isxdigit(c) || c == ':' || c == '.'; });
}

// Convert a string_view (IPv4 address) to an integer array
constexpr auto parse_ip_v4(std::string_view sv) noexcept -> ip4_int_t {
    ip4_int_t result = {};
    size_t start = 0;

    for (int i = 0; i < IP4_OCTECTS_COUNT; ++i) {
        auto end{ sv.find(IP4_SEPARATOR, start) };
        if (end == std::string_view::npos) {
            end = sv.size();
        }

        // check the octet is less than 3 characters
        if (end - start > IP4_OCTET_MAX_LEN) {
            return {};
        }

        result[i] = static_cast<uint8_t>(std::clamp(from_string_chars(sv, start, end), 0, 255));
        start = end + 1;
    }
    return result;
}

// Tests for parse_ip_v4
static_assert(parse_ip_v4("127.0.0.1") == ip4_int_t{ 0x7f, 0x00, 0x00, 0x01 });
static_assert(parse_ip_v4("255.255.255.255") == ip4_int_t{ 0xff, 0xff, 0xff, 0xff });
static_assert(parse_ip_v4("0.0.0.0") == ip4_int_t{ 0x00, 0x00, 0x00, 0x00 });
static_assert(parse_ip_v4("256.256.256.256") == ip4_int_t{ 0xff, 0xff, 0xff, 0xff });
static_assert(parse_ip_v4("192.168.0.1") == ip4_int_t{192, 168, 0, 1});
static_assert(parse_ip_v4("1.2.3.4") == ip4_int_t{1, 2, 3, 4});

// Mask to bitset
constexpr auto ip4_mask_to_bitset(int cidrMask) noexcept -> ip4_bitset_t {
    ip4_bitset_t mask{ 0 };
    for (int i = 0; i < cidrMask; ++i) {
        mask.set(31 - i);
    }
    return mask;
}

// IPv4 octets to bitset
constexpr auto ip4_octets_to_bitset(ip4_int_t ip4_octets) noexcept -> ip4_bitset_t {
    ip4_bitset_t result{ 0 };

    for (int i = 0; i < IP4_OCTECTS_COUNT; ++i) {
        for (int j = 0; j < 8; ++j) {
            result <<= 1;
            result |= (ip4_octets[i] >> (7 - j)) & 1;
        }
    }
    return result;
}

// Bitset to IPv4 octets
auto bitset_to_ip4_octets(ip4_bitset_t bitset) noexcept -> ip4_int_t {
    return {
        static_cast<uint8_t>((bitset.to_ulong() >> 24) & 0xFF),
        static_cast<uint8_t>((bitset.to_ulong() >> 16) & 0xFF),
        static_cast<uint8_t>((bitset.to_ulong() >> 8) & 0xFF),
        static_cast<uint8_t>(bitset.to_ulong() & 0xFF)
    };
}

} // namespace __impl

/**
 * @brief Get the broadcast address for the network ID + mask combination
 * 
 * @param ip_addr Network ID
 * @param mask Mask
 * @param output_buffer pointer to the output buffer
 */
inline auto get_broadcast_address(const char *ip_addr, char mask, char* output_buffer) noexcept -> void {
    if(!__impl::could_be_ip4_address(std::string_view{ip_addr})) { 
        return;
    }

    // convert ip_addr to array of bytes
    const auto ip_parts{ __impl::parse_ip_v4(ip_addr) };
    const auto mask_bits{ __impl::ip4_mask_to_bitset(mask) };
    const auto ip4_bitset{ __impl::ip4_octets_to_bitset(ip_parts) };
    const auto broadcast_bitset{ ip4_bitset | (~mask_bits) };
    const auto broadcast_octets{ __impl::bitset_to_ip4_octets(broadcast_bitset) };
    std::snprintf(output_buffer, __impl::IP4_STRING_SIZE, "%d.%d.%d.%d"
                                    , broadcast_octets[0]
                                    , broadcast_octets[1]
                                    , broadcast_octets[2]
                                    , broadcast_octets[3]);
}

/**
 * @brief Get the ip4 integral equivalent
 * 
 * @param ip4_addr 
 * @return ip4 bitset
 */
constexpr inline auto get_ip_integral_equivalent(const char* ip4_addr) noexcept -> uint32_t {
    
    std::string_view ip4{ ip4_addr };

    if (ip4.empty() || !__impl::could_be_ip4_address(ip4)) {
        return 0;
    }

    const auto octets{ __impl::parse_ip_v4(ip4) };
    uint32_t ipAddress{ 0 };

    for (int i = 0; i < octets.size(); ++i) {
        if (octets[i] > 255 || octets[i] < 0) {
            return 0;
        }
        ipAddress |= (octets[i] << (24 - i * 8));
    }

    return ipAddress;
}

/**
 * @brief Translate IPv4 address from integral to formatted representation
 * 
 * @param ip4_addr integral representation of IPv4 address
 * @param output_buffer output buffer to store the formatted representation
 */
constexpr inline auto get_abcd_ip_format(uint32_t ip4_addr, char* output_buffer) noexcept -> void {
}

}