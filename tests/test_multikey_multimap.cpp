#include <doctest/doctest.h>
#include "../include/isto/multikey_multimap/multikey_multimap.hpp"
    using namespace isto::multikey_multimap;
#include <fmt/format.h>
#include <set>

TEST_CASE("multikey_multimap.hpp")
{
        using
    map_t = multikey_multimap_t <int, char, char, char>;
        map_t
    map;
    map.insert ({{ 'A', 'Y', 'S' }, 1 });
    map.insert ({{ 'B', 'X', 'T' }, 2 });
    map.insert ({{ 'B', 'Y', 'U' }, 3 });
    map.insert ({{ 'A', 'X', 'R' }, 4 });
    map.insert ({{ 'A', 'Y', 'R' }, 5 });

    fmt::print ("The map:\n  v  k1  k2  k3\n  ----------------\n");
    for (auto const& i: map)
    {
        fmt::print (
              "  {:1} {:>2} {:>3} {:>3}\n"
            , i.second
            , std::get <0> (i.first)
            , std::get <1> (i.first)
            , std::get <2> (i.first)
        );
    }

    fmt::print ("By key 1: A\n");
        auto
    [a0, a1] = map.equal_range <0> ('A');
    for (auto a = a0; a != a1; ++a)
    {
        fmt::print (
              "  {} (A, {}, {})\n"
            , a->second
            , std::get <1> (a->first)
            , std::get <2> (a->first)
        );
    }
    fmt::print ("By key 2: Y\n");
        auto
    [b0, b1] = map.equal_range <1> ('Y');
    for (auto b = b0; b != b1; ++b)
    {
        fmt::print (
              "  {} ({}, Y, {})\n"
            , b->second
            , std::get <0> (b->first)
            , std::get <2> (b->first)
        );
    }
        std::set <map_t::value_type>
    res;
    map.intersection (
          a0
        , a1
        , b0
        , b1
        , std::inserter (res, res.begin ())
    );
    fmt::print ("Intersection:\n");
    for (auto e: res)
    {
        fmt::print ("  {}\n", e.second);
            auto
        c = (e.second == 1) || (e.second == 5);
        CHECK(c);
    }

    fmt::print ("By key 3: R\n");
        auto
    [c0, c1] = map.equal_range <2> ('R');
    for (auto c = c0; c != c1; ++c)
    {
        fmt::print (
              "  {} ({}, Y, {})\n"
            , c->second
            , std::get <0> (c->first)
            , std::get <1> (c->first)
        );
    }

        std::set <map_t::value_type>
    res2;
    map.intersection (
          std::tuple { a0, b0, c0 }
        , std::tuple { a1, b1, c1 }
        , std::inserter (res2, res2.begin ())
    );
    fmt::print ("Intersection:\n");
    for (auto e: res2)
    {
        fmt::print ("  {}\n", e.second);
        CHECK(e.second == 5);
    }

} // TEST_CASE("multikey_multimap.hpp")
