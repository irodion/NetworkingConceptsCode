#include <gtest/gtest.h>

#include <utils.hpp>

// Test implementation functions

TEST(IsFormattedIP4Address, ValidAddresses) {
    EXPECT_TRUE(NetworkingConcepts::__impl::could_be_ip4_address("0.0.0.0"));
    EXPECT_TRUE(NetworkingConcepts::__impl::could_be_ip4_address("255.255.255.255"));
    EXPECT_TRUE(NetworkingConcepts::__impl::could_be_ip4_address("127.0.0.1"));
    EXPECT_TRUE(NetworkingConcepts::__impl::could_be_ip4_address("192.168.0.1"));
    EXPECT_TRUE(NetworkingConcepts::__impl::could_be_ip4_address("192.168.1.1"));
    EXPECT_TRUE(NetworkingConcepts::__impl::could_be_ip4_address("192.168.127.12"));
    EXPECT_TRUE(NetworkingConcepts::__impl::could_be_ip4_address("192.168.3.11"));
    EXPECT_TRUE(NetworkingConcepts::__impl::could_be_ip4_address("192.168.127.12"));
}

TEST(IsFormattedIP4Address, InValidAddresses) {
    EXPECT_FALSE(NetworkingConcepts::__impl::could_be_ip4_address(""));
    EXPECT_FALSE(NetworkingConcepts::__impl::could_be_ip4_address(" "));
    EXPECT_FALSE(NetworkingConcepts::__impl::could_be_ip4_address("0.0.0"));
    EXPECT_FALSE(NetworkingConcepts::__impl::could_be_ip4_address("0.0.0.0.0"));
    EXPECT_FALSE(NetworkingConcepts::__impl::could_be_ip4_address("0.0.0.0.0.0"));
    EXPECT_FALSE(NetworkingConcepts::__impl::could_be_ip4_address("0.0.0.0.0.0.0"));
    EXPECT_FALSE(NetworkingConcepts::__impl::could_be_ip4_address("0.0.0.0.0.0.0.0"));
    EXPECT_FALSE(NetworkingConcepts::__impl::could_be_ip4_address("0.0.Invalid.0"));
    EXPECT_FALSE(NetworkingConcepts::__impl::could_be_ip4_address("192.168.1"));
    EXPECT_FALSE(NetworkingConcepts::__impl::could_be_ip4_address("192.168.1.1.1"));
    EXPECT_FALSE(NetworkingConcepts::__impl::could_be_ip4_address("192.168.1.1/24"));
    EXPECT_FALSE(NetworkingConcepts::__impl::could_be_ip4_address("192.16&.1.1"));
    EXPECT_FALSE(NetworkingConcepts::__impl::could_be_ip4_address("192.168.a.1"));
}

TEST(IsFormattedIP6Address, ValidAddresses) {
    EXPECT_TRUE(NetworkingConcepts::__impl::could_be_ip6_address("::"));
    EXPECT_TRUE(NetworkingConcepts::__impl::could_be_ip6_address("::1"));
    EXPECT_TRUE(NetworkingConcepts::__impl::could_be_ip6_address("::ffff:127.0.0.1"));
    EXPECT_TRUE(NetworkingConcepts::__impl::could_be_ip6_address("::ffff:192.168.0.1"));
    EXPECT_TRUE(NetworkingConcepts::__impl::could_be_ip6_address("2001:db8::ff00:42:8329"));
    EXPECT_TRUE(NetworkingConcepts::__impl::could_be_ip6_address("fe80::1ff:fe23:4567:890a"));
    EXPECT_TRUE(NetworkingConcepts::__impl::could_be_ip6_address("2001:0db8:85a3:0000:0000:8a2e:0370:7334"));
}

TEST(IsFormattedIP6Address, InValidAddresses) {
    EXPECT_FALSE(NetworkingConcepts::__impl::could_be_ip6_address(""));
    EXPECT_FALSE(NetworkingConcepts::__impl::could_be_ip6_address(" "));
    EXPECT_FALSE(NetworkingConcepts::__impl::could_be_ip6_address("0.0.0"));
    EXPECT_FALSE(NetworkingConcepts::__impl::could_be_ip6_address("0.0.0.0.0"));
    EXPECT_FALSE(NetworkingConcepts::__impl::could_be_ip6_address("0.0.0.0.0.0"));
    EXPECT_FALSE(NetworkingConcepts::__impl::could_be_ip6_address("0.0.0.0.0.0.0"));
    EXPECT_FALSE(NetworkingConcepts::__impl::could_be_ip6_address("2001:q:q:q:0000:8a2e:0370:7334"));
}

TEST(IP4MaskToBitsetTest, ConvertCIDR0) {
    auto result{ NetworkingConcepts::__impl::ip4_mask_to_bitset(0) };
    EXPECT_EQ(result, NetworkingConcepts::__impl::ip4_bitset_t(0x00000000));
}

TEST(IP4MaskToBitsetTest, ConvertCIDR16) {
    auto result{ NetworkingConcepts::__impl::ip4_mask_to_bitset(16) };
    EXPECT_EQ(result, NetworkingConcepts::__impl::ip4_bitset_t(0xFFFF0000));
}

TEST(IP4MaskToBitsetTest, ConvertCIDR24) {
    auto result{ NetworkingConcepts::__impl::ip4_mask_to_bitset(24) };
    EXPECT_EQ(result, NetworkingConcepts::__impl::ip4_bitset_t(0xFFFFFF00));
}

TEST(IP4MaskToBitsetTest, ConvertCIDR32) {
    auto result{ NetworkingConcepts::__impl::ip4_mask_to_bitset(32) };
    EXPECT_EQ(result, NetworkingConcepts::__impl::ip4_bitset_t(0xFFFFFFFF));
}

TEST(IP4OctetsToBitsetTest, ConvertZeroOctets) {
    auto result{ NetworkingConcepts::__impl::ip4_octets_to_bitset({0, 0, 0, 0}) };
    EXPECT_EQ(result, NetworkingConcepts::__impl::ip4_bitset_t(0x00000000));
}

TEST(IP4OctetsToBitsetTest, ConvertFullOctets) {
    auto result{ NetworkingConcepts::__impl::ip4_octets_to_bitset({255, 255, 255, 255}) };
    EXPECT_EQ(result, NetworkingConcepts::__impl::ip4_bitset_t(0xFFFFFFFF));
}

TEST(IP4OctetsToBitsetTest, ConvertDefaultAddress) {
    auto result{ NetworkingConcepts::__impl::ip4_octets_to_bitset({192, 168, 0, 1}) };
    EXPECT_EQ(result, NetworkingConcepts::__impl::ip4_bitset_t(0xC0A80001));
}

TEST(BitsetToOctets, ConvertFullBits) {
  const NetworkingConcepts::__impl::ip4_bitset_t bitset( 0xFFFFFFFF );
  const auto ip4_octets{ NetworkingConcepts::__impl::bitset_to_ip4_octets(bitset) };
  EXPECT_EQ((ip4_octets), (NetworkingConcepts::__impl::ip4_int_t{255, 255, 255, 255}));
}

TEST(BitsetToOctets, ConvertNoBits) {
  const NetworkingConcepts::__impl::ip4_bitset_t bitset( 0x00000000 );
  const auto ip4_octets{ NetworkingConcepts::__impl::bitset_to_ip4_octets(bitset) };
  EXPECT_EQ((ip4_octets), (NetworkingConcepts::__impl::ip4_int_t{0, 0, 0, 0}));
}

TEST(BitsetToOctets, ConvertDefaultAddress) {
  const NetworkingConcepts::__impl::ip4_bitset_t bitset( 0xC0A80001 );
  const auto ip4_octets{ NetworkingConcepts::__impl::bitset_to_ip4_octets(bitset) };
  EXPECT_EQ((ip4_octets), (NetworkingConcepts::__impl::ip4_int_t{192, 168, 0, 1}));
}


// ============================================================================
// Main test body
// ============================================================================
// Test get_broadcast_address() function
TEST(GetBroadcastAddressTest, DefaultAddressAndMask) {
    const char* ip_address{ "192.168.0.1" };
    const char* expected_broadcast_address{ "192.168.0.255" };
    int mask = 24;
    char output_buffer[16];
    NetworkingConcepts::get_broadcast_address(ip_address, mask, output_buffer);
    EXPECT_STREQ(output_buffer, expected_broadcast_address);
}

TEST(GetBroadcastAddressTest, TestCase1) {
    const char* ip_address{ "192.168.2.10" };
    const char* expected_broadcast_address{ "192.168.2.255" };
    int mask = 24;
    char output_buffer[16];
    NetworkingConcepts::get_broadcast_address(ip_address, mask, output_buffer);
    EXPECT_STREQ(output_buffer, expected_broadcast_address);
}

TEST(GetBroadcastAddressTest, TestCase2) {
    const char* ip_address{ "10.1.23.10" };
    const char* expected_broadcast_address{ "10.1.31.255" };
    int mask = 20;
    char output_buffer[16];
    NetworkingConcepts::get_broadcast_address(ip_address, mask, output_buffer);
    EXPECT_STREQ(output_buffer, expected_broadcast_address);
}

TEST(GetBroadcastAddressTest, TestCase3) {
    const char* ip_address{ "10.1.0.0" };
    const char* expected_broadcast_address{ "10.1.255.255" };
    int mask = 16;
    char output_buffer[16];
    NetworkingConcepts::get_broadcast_address(ip_address, mask, output_buffer);
    EXPECT_STREQ(output_buffer, expected_broadcast_address);
}

TEST(GetIntegralEquivalentTest, TestCase1) {
    const char* ip_address{ "192.168.2.10" };
    uint32_t expected_integral_address{ 3232236042 };
    const auto result{ NetworkingConcepts::get_ip_integral_equivalent(ip_address) };
    EXPECT_EQ(result, expected_integral_address);
}

TEST(GetIntegralEquivalentTest, TestCase2) {
    const char* ip_address{ "10.1.23.10" };
    uint32_t expected_integral_address{ 167843594 };
    const auto result{ NetworkingConcepts::get_ip_integral_equivalent(ip_address) };
    EXPECT_EQ(result, expected_integral_address);
}
// Test get_ip_integral_equivalent


