#include <boost/lexical_cast.hpp>
#include <boost/test/unit_test.hpp>

#include "meridian/network/ip_address.hpp"

using meridian::network::ip_address_family;
using meridian::network::ip_address;

BOOST_AUTO_TEST_SUITE(ip_address_tests)

BOOST_AUTO_TEST_CASE(test_wildcard_constructor)
{
    ip_address wildcard4(ip_address_family::IPv4);

    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(wildcard4), "0.0.0.0");
    BOOST_CHECK(wildcard4.addr() != 0);
    BOOST_CHECK(wildcard4.addr4() != 0);
    BOOST_CHECK(wildcard4.addr6() == nullptr);
    BOOST_CHECK(wildcard4.length() > 0);
    BOOST_CHECK_EQUAL(wildcard4.family(), ip_address_family::IPv4);
    BOOST_CHECK_EQUAL(wildcard4.scope(), 0);
    BOOST_CHECK_EQUAL(wildcard4.af(), AF_INET);

    ip_address wildcard6(ip_address_family::IPv6);
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(wildcard6), "::");
    BOOST_CHECK(wildcard6.addr() != 0);
    BOOST_CHECK(wildcard6.addr4() == nullptr);
    BOOST_CHECK(wildcard6.addr6() != 0);
    BOOST_CHECK(wildcard6.length() > 0);
    BOOST_CHECK_EQUAL(wildcard6.family(), ip_address_family::IPv6);
    BOOST_CHECK_EQUAL(wildcard6.scope(), 0);
    BOOST_CHECK_EQUAL(wildcard6.af(), AF_INET6);
}


BOOST_AUTO_TEST_CASE(test_ip_address_family_stream_operators)
{
    std::ostringstream os;
    os << ip_address_family::IPv4 << ' ' << ip_address_family::IPv6;

    std::istringstream is(os.str());
    ip_address_family f;
    is >> f;
    BOOST_CHECK_EQUAL(f, ip_address_family::IPv4);
    is >> f;
    BOOST_CHECK_EQUAL(f, ip_address_family::IPv6);
}


BOOST_AUTO_TEST_CASE(test_loopback_addresses)
{
    ip_address loop4("127.0.0.1");
    ip_address loop6("::1");

    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(loop4), "127.0.0.1");
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(loop6), "::1");
    BOOST_CHECK(loop4.is_loopback());
    BOOST_CHECK(loop6.is_loopback());
}


BOOST_AUTO_TEST_CASE(test_is_IPv4_mapped)
{
    ip_address a("::ffff:192.0.2.128");

    BOOST_CHECK(!a.is_wildcard());
    BOOST_CHECK(!a.is_broadcast());
    BOOST_CHECK(!a.is_loopback());
    BOOST_CHECK(!a.is_multicast());
    BOOST_CHECK( a.is_unicast());
    BOOST_CHECK(!a.is_link_local());
    BOOST_CHECK(!a.is_site_local());
    BOOST_CHECK(!a.is_IPv4_compatible());
    BOOST_CHECK( a.is_IPv4_mapped());
    BOOST_CHECK(!a.is_well_known_mc());
    BOOST_CHECK(!a.is_node_local_mc());
    BOOST_CHECK(!a.is_link_local_mc());
    BOOST_CHECK(!a.is_site_local_mc());
    BOOST_CHECK(!a.is_org_local_mc());
    BOOST_CHECK(!a.is_global_mc());
}


BOOST_AUTO_TEST_CASE(test_is_IPv4_compatible)
{
    ip_address a("::192.0.2.128");

    BOOST_CHECK(!a.is_wildcard());
    BOOST_CHECK(!a.is_broadcast());
    BOOST_CHECK(!a.is_loopback());
    BOOST_CHECK(!a.is_multicast());
    BOOST_CHECK( a.is_unicast());
    BOOST_CHECK(!a.is_link_local());
    BOOST_CHECK(!a.is_site_local());
    BOOST_CHECK( a.is_IPv4_compatible());
    BOOST_CHECK(!a.is_IPv4_mapped());
    BOOST_CHECK(!a.is_well_known_mc());
    BOOST_CHECK(!a.is_node_local_mc());
    BOOST_CHECK(!a.is_link_local_mc());
    BOOST_CHECK(!a.is_site_local_mc());
    BOOST_CHECK(!a.is_org_local_mc());
    BOOST_CHECK(!a.is_global_mc());
}



BOOST_AUTO_TEST_SUITE_END()
