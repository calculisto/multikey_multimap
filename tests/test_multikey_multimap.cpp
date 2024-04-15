#include <doctest/doctest.h>
#include "../include/calculisto/multikey_multimap/multikey_multimap.hpp"
    using namespace calculisto::multikey_multimap;
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
    map.insert ({{ 'A', 'Y', 'R' }, 2 });

    fmt::print ("The map:\n  v  k1  k2  k3\n  ----------------\n");
    for (auto&& [k, v]: map)
    {
        fmt::print (
              "  {:1} {:>2} {:>3} {:>3}\n"
            , v
            , std::get <0> (k)
            , std::get <1> (k)
            , std::get <2> (k)
        );
    }

    fmt::print ("\nBy key 1: \n");
    for (auto i = map.key_begin <0> (); i != map.key_end <0> (); ++i)
    {
            auto const
        [k, v] = *i;
        fmt::print (
              "  {} ({}, {}, {})\n"
            , v
            , std::get <0> (k)
            , std::get <1> (k)
            , std::get <2> (k)
        );
    }
    fmt::print ("\nBy key 2: \n");
    for (auto i = map.key_begin <1> (); i != map.key_end <1> (); ++i)
    {
        fmt::print (
              "  {} ({}, {}, {})\n"
            , i->second
            , std::get <0> (i->first)
            , std::get <1> (i->first)
            , std::get <2> (i->first)
        );
    }
    fmt::print ("\nBy key 3: \n");
    for (auto i = map.key_begin <2> (); i != map.key_end <2> (); ++i)
    {
        fmt::print (
              "  {} ({}, {}, {})\n"
            , i->second
            , std::get <0> (i->first)
            , std::get <1> (i->first)
            , std::get <2> (i->first)
        );
    }

    fmt::print ("\nEqual range, key 1 = A\n");
        auto
    [a0, a1] = map.equal_range <0> ('A');
    for (auto a = a0; a != a1; ++a)
    {
        fmt::print (
              "  {} ({}, {}, {})\n"
            , a->second
            , std::get <0> (a->first)
            , std::get <1> (a->first)
            , std::get <2> (a->first)
        );
    }
    fmt::print ("\nEqual range, key 2 = Y\n");
        auto
    [b0, b1] = map.equal_range <1> ('Y');
    for (auto b = b0; b != b1; ++b)
    {
        fmt::print (
              "  {} ({}, {}, {})\n"
            , b->second
            , std::get <0> (b->first)
            , std::get <1> (b->first)
            , std::get <2> (b->first)
        );
    }
    fmt::print ("\nEqual range, key 3 = R\n");
        auto
    [c0, c1] = map.equal_range <2> ('R');
    for (auto c = c0; c != c1; ++c)
    {
        fmt::print (
              "  {} ({}, {}, {})\n"
            , c->second
            , std::get <0> (c->first)
            , std::get <1> (c->first)
            , std::get <2> (c->first)
        );
    }
    /*
    {
            std::vector <map_t::const_iterator>
        res;
        map.intersection (
              a0
            , a1
            , b0
            , b1
            , std::back_inserter (res)
        );
        fmt::print ("Intersection A x Y:\n");
        CHECK(res.size () == 3);
        for (auto e: res)
        {
            fmt::print (
                  "  {} ({}, {}, {})\n"
                , e->second
                , std::get <0> (e->first)
                , std::get <1> (e->first)
                , std::get <2> (e->first)
            );
                auto
            c = (e->second == 1) || (e->second == 5) || (e->second == 2);
            CHECK(c);
        }

            std::vector <map_t::const_iterator>
        res2;
        map.intersection (
              res.begin ()
            , res.end ()
            , c0
            , c1
            , std::back_inserter (res2)
        );
        fmt::print ("Intersection A x Y x R:\n");
        for (auto e: res2)
        {
            fmt::print (
                  "  {} ({}, {}, {})\n"
                , e->second
                , std::get <0> (e->first)
                , std::get <1> (e->first)
                , std::get <2> (e->first)
            );
        }
    }
    {
            std::vector <map_t::const_iterator>
        res;
        map.intersection (
              a0
            , a1
            , b0
            , b1
            , std::back_inserter (res)
        );
        fmt::print ("Intersection A x Y:\n");
        CHECK(res.size () == 3);
        for (auto e: res)
        {
            fmt::print (
                  "  {} ({}, {}, {})\n"
                , e->second
                , std::get <0> (e->first)
                , std::get <1> (e->first)
                , std::get <2> (e->first)
            );
                auto
            c = (e->second == 1) || (e->second == 5) || (e->second == 2);
            CHECK(c);
        }

            std::vector <map_t::const_iterator>
        res2;
        map.intersection (
              res.begin ()
            , res.end ()
            , c0
            , c1
            , std::back_inserter (res2)
        );
        fmt::print ("Intersection A x Y x R:\n");
        for (auto e: res2)
        {
            fmt::print (
                  "  {} ({}, {}, {})\n"
                , e->second
                , std::get <0> (e->first)
                , std::get <1> (e->first)
                , std::get <2> (e->first)
            );
                auto
            c = (e->second == 5) || (e->second == 2);
            CHECK(c);
        }

            std::vector <map_t::const_iterator>
        res3;
        map.intersection (
              c0
            , c1
            , res.begin ()
            , res.end ()
            , std::back_inserter (res3)
        );
        fmt::print ("Intersection A x Y x R:\n");
        for (auto e: res3)
        {
            fmt::print (
                  "  {} ({}, {}, {})\n"
                , e->second
                , std::get <0> (e->first)
                , std::get <1> (e->first)
                , std::get <2> (e->first)
            );
                auto
            c = (e->second == 5) || (e->second == 2);
            CHECK(c);
        }
    }
*/
    {
            auto
        r = std::vector <map_t::const_iterator> {};
        map.intersect (
              std::tuple { a0, b0, c0 }
            , std::tuple { a1, b1, c1 }
            , std::back_inserter (r)
        );
        CHECK(r.size () == 2);
        for (auto&& e: r)
        {
                auto
            c = (e->second == 5) || (e->second == 2);
            CHECK(c);
        }
    }
    {
            auto
        r = std::vector <map_t::const_iterator> {};
        map.intersect (
              std::tuple { a0, b0 }
            , std::tuple { a1, b1 }
            , std::back_inserter (r)
        );
        CHECK(r.size () == 3);
        for (auto&& e: r)
        {
                auto
            c = (e->second == 5) || (e->second == 2) || (e->second == 1);
            CHECK(c);
        }
    }
    {
            auto
        r = std::vector <map_t::const_iterator> {};
        map.intersect (
              std::tuple { b0, c0 }
            , std::tuple { b1, c1 }
            , std::back_inserter (r)
        );
        CHECK(r.size () == 2);
        for (auto&& e: r)
        {
                auto
            c = (e->second == 5) || (e->second == 2);
            CHECK(c);
        }
    }
    {
            auto
        r = std::vector <map_t::const_iterator> {};
        map.intersect (
              std::tuple { c0 }
            , std::tuple { c1 }
            , std::back_inserter (r)
        );
        CHECK(r.size () == 3);
        for (auto&& e: r)
        {
                auto
            c = (e->second == 5) || (e->second == 2) || (e->second == 4);
            CHECK(c);
        }
    }
    {
            auto
        r = std::vector <map_t::const_iterator> {};
        map.intersect2 (
              std::back_inserter (r)
            , std::pair { a0, a1 }
            , std::pair { b0, b1 }
            , std::pair { c0, c1 }
        );
        CHECK(r.size () == 2);
        for (auto&& e: r)
        {
                auto
            c = (e->second == 5) || (e->second == 2);
            CHECK(c);
        }
    }
    {
            auto
        r = std::vector <map_t::const_iterator> {};
        map.intersect2 (
              std::back_inserter (r)
            , std::pair { b0, b1 }
            , std::pair { c0, c1 }
        );
        CHECK(r.size () == 2);
        for (auto&& e: r)
        {
                auto
            c = (e->second == 5) || (e->second == 2);
            CHECK(c);
        }
    }
    {
            auto
        r = std::vector <map_t::const_iterator> {};
        map.intersect2 (
              std::back_inserter (r)
            , std::pair { c0, c1 }
        );
        CHECK(r.size () == 3);
        for (auto&& e: r)
        {
                auto
            c = (e->second == 5) || (e->second == 2) || (e->second == 4);
            CHECK(c);
        }
    }
    /*
    {
        fmt::print ("\nSelect A, Y, R\n");
            auto
        r = std::vector <map_t::const_iterator> {};
        map.select (
              std::back_inserter (r)
            , 'A'
            , 'Y'
            , 'R'
        );
        CHECK(r.size () == 2);
        for (auto&& e: r)
        {
            fmt::print (
                  "  {} ({}, {}, {})\n"
                , e->second
                , std::get <0> (e->first)
                , std::get <1> (e->first)
                , std::get <2> (e->first)
            );
                auto
            c = (e->second == 5) || (e->second == 2);
            CHECK(c);
        }
    }
    {
        fmt::print ("\nSelect *, Y, R\n");
            auto
        r = std::vector <map_t::const_iterator> {};
        map.select (
              std::back_inserter (r)
            , {}
            , 'Y'
            , 'R'
        );
        CHECK(r.size () == 2);
        for (auto&& e: r)
        {
            fmt::print (
                  "  {} ({}, {}, {})\n"
                , e->second
                , std::get <0> (e->first)
                , std::get <1> (e->first)
                , std::get <2> (e->first)
            );
                auto
            c = (e->second == 5) || (e->second == 2);
            CHECK(c);
        }
    }
    {
        fmt::print ("\nSelect *, *, R\n");
            auto
        r = std::vector <map_t::const_iterator> {};
        map.select (
              std::back_inserter (r)
            , {}
            , {}
            , 'R'
        );
        CHECK(r.size () == 3);
        for (auto&& e: r)
        {
            fmt::print (
                  "  {} ({}, {}, {})\n"
                , e->second
                , std::get <0> (e->first)
                , std::get <1> (e->first)
                , std::get <2> (e->first)
            );
                auto
            c = (e->second == 5) || (e->second == 2) || (e->second == 4);
            CHECK(c);
        }
    }
    */
    /*
    {
            auto
        r = map.select_all ();
        r.refine (a0, a1);
    }
    */
    {
            auto
        r = std::vector <map_t::const_iterator> {};
        map.select (std::back_inserter (r), 'A', 'Y', 'R');
        CHECK(r.size () == 2);
        for (auto&& e: r)
        {
                auto
            c = (e->second == 5) || (e->second == 2);
            CHECK(c);
        }
    }
    {
            auto
        r = std::vector <map_t::const_iterator> {};
        map.select (std::back_inserter (r), 'A', 'Y', {});
        CHECK(r.size () == 3);
        for (auto&& e: r)
        {
                auto
            c = (e->second == 5) || (e->second == 2) || (e->second == 1);
            CHECK(c);
        }
    }
    {
            auto
        r = std::vector <map_t::const_iterator> {};
        map.select (std::back_inserter (r), {}, 'Y', 'R');
        CHECK(r.size () == 2);
        for (auto&& e: r)
        {
                auto
            c = (e->second == 5) || (e->second == 2);
            CHECK(c);
        }
    }
    {
            auto
        r = std::vector <map_t::const_iterator> {};
        map.select (std::back_inserter (r), {}, {}, 'R');
        CHECK(r.size () == 3);
        for (auto&& e: r)
        {
                auto
            c = (e->second == 5) || (e->second == 2) || (e->second == 4);
            CHECK(c);
        }
    }
} // TEST_CASE("multikey_multimap.hpp")
