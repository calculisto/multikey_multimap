#pragma once
#include <map>
#include <functional>
#include <optional>

    namespace isto::multikey_multimap
{
    namespace
detail
{
        template <
              std::size_t I
            , class Function
            , class... Tuples
        >
        constexpr void
    static_for_impl_2 (
          Function&& function
        , Tuples&&... tuples
    ){
        std::invoke (
              std::forward <Function> (function)
            , (std::get <I> (std::forward <Tuples> (tuples)))...
        );
    }

        template <
              class Function
            , class... Tuples
            , std::size_t... Is
        >
        constexpr void
    static_for_impl (
          std::index_sequence <Is...>
        , Function&& function
        , Tuples&&... tuples
    ){
        ((static_for_impl_2 <Is> (function, (std::forward <Tuples> (tuples))...)), ...);
    }
} // namespace detail

    template <
          class Function
        , class FirstTuple
        , class... OtherTuples
    >
    constexpr void
static_for (
      Function&& function
    , FirstTuple&& first_tuple
    , OtherTuples&&... other_tuples
){
    static_assert (
          ((std::tuple_size_v <std::remove_reference_t <FirstTuple>> == std::tuple_size_v <std::remove_reference_t <OtherTuples>>) && ...)
        , "All tuples must have the same size"
    );
    detail::static_for_impl (
          std::make_index_sequence <std::tuple_size_v <std::decay_t <FirstTuple>>> {}
        , std::forward <Function> (function)
        , std::forward <FirstTuple> (first_tuple)
        , std::forward <OtherTuples> (other_tuples)...
    );
    
}

    namespace
detail
{
    // Head & tail
        template <class Tuple>
        using
    head_t = std::tuple_element_t <0, Tuple>;

        template <class Tuple>
        constexpr auto
    head (Tuple&& tuple)
    {
        return std::get <0> (std::forward <Tuple> (tuple));
    }

        namespace
    detail
    {
            constexpr auto
        one = std::size_t { 1 };

            template <class Tuple, std::size_t ...Is>
            constexpr auto
        tail_impl_t (std::index_sequence <Is...>)
            -> std::tuple <std::tuple_element_t <Is + one, Tuple>...>
        ;

            template <class Tuple, std::size_t ...Is>
            constexpr auto
        tail_impl (Tuple&& tuple, std::index_sequence <Is...>)
        {
            return std::tuple { std::get<Is + one> (std::forward <Tuple> (tuple))... };
        }
    } // namespace detail

        template <class Tuple>
        using
    tail_t = decltype (detail::tail_impl_t <Tuple> (
            std::make_index_sequence <std::tuple_size_v <std::remove_cvref_t <Tuple>> - detail::one> {}
    ));

        template <class Tuple>
        constexpr auto
    tail (Tuple&& tuple)
    {
        return detail::tail_impl <Tuple> (
              std::forward <Tuple> (tuple)
            , std::make_index_sequence <std::tuple_size_v <std::remove_cvref_t <Tuple>> - detail::one> {}
        );
    }
} // namespace detail

    namespace
detail 
{
    template <class... Ts, std::size_t... Is>
    constexpr auto
not_at_end_impl (
      std::tuple <Ts...> const& first
    , std::tuple <Ts...> const& last
    , std::index_sequence <Is...>
){
    return (... && (std::get <Is> (first) != std::get <Is> (last)));
}
    template <class... Ts>
    constexpr auto
not_at_end (
      std::tuple <Ts...> const& first
    , std::tuple <Ts...> const& last
){
    return not_at_end_impl (first, last, std::index_sequence_for <Ts...> {});
}
    template <class... Ts, std::size_t... Is>
    constexpr auto
not_at_end_impl (
      std::tuple <Ts...> const& first
    , std::tuple <Ts...> const& last
    , std::tuple <Ts...> const& flag
    , std::index_sequence <Is...>
){
    return (... && (std::get <Is> (flag) && (std::get <Is> (first) != std::get <Is> (last))));
}
    template <class... Ts>
    constexpr auto
not_at_end (
      std::tuple <Ts...> const& first
    , std::tuple <Ts...> const& last
    , std::tuple <Ts...> const& flag
){
    return not_at_end_impl (first, last, std::index_sequence_for <Ts...> {});
}
/*
    template <class T>
    constexpr auto
not_at_end (
      T first_begin
    , T first_end
    , T last_begin
    , T last_end
){
        auto const
    [ a, b ] = std::mismatch (first_begin, first_end, last_begin);
    return a != first_end;
}
*/
    namespace
test
{
        constexpr auto
    f1 = std::tuple { 1, 1., 'a' };
        constexpr auto
    f2 = std::tuple { 1, 2., 'a' };
        constexpr auto
    l = std::tuple { 2, 2., 'b' };
    static_assert (not_at_end (f1, l));
    static_assert (!not_at_end (f2, l));
} // namespace test

    template <class T>
    constexpr auto
get_max (std::tuple <T> const& t)
{
    return (std::get <0> (t))->second;
}

    template <class... Ts>
    constexpr auto
get_max (std::tuple <Ts...> const& t)
{
        using std::max;
    return max (head (t)->second, get_max (tail (t)));
}
    template <class... Ts>
    constexpr auto
get_max (
      std::tuple <Ts...> const& t
    , std::tuple <Ts...> const& flag
){
    /*
        auto&&
    [v, rest] = get_first (t, flags);

        using std::max;
    return max (head (t)->second, get_max (tail (t)));
    */
    // TODO
}
    namespace 
test
{
        constexpr auto
    a1 = std::pair { 0, 1 };
        constexpr auto
    a2 = std::pair { 0, 3 };
        constexpr auto
    a3 = std::pair { 0, 0 };
    static_assert (get_max (std::tuple { &a1, &a2, &a3 }) == 3);
} // namespace test

    template <class T, class... Ts>
    constexpr auto
advance_if_under (std::tuple <Ts...>& first, T const& max)
{
    static_for (
          [max](auto& x){ if (x->second < max) ++x; }
        , first
    );
}
    namespace
test
{
    // ?
} // namespace test

    template <class... Ts>
    constexpr auto
all_same (std::tuple <Ts...> const& t)
{
        auto const
    a = std::get <0> (t)->second;
        auto
    r = true;
    static_for (
          [a,&r](auto const& x){ r  = r && (x->second == a); }
        , t
    );
    return r;
}
    namespace
test
{
    static_assert ( all_same (std::tuple { &a1, &a1, &a1 }));
    static_assert (!all_same (std::tuple { &a1, &a2, &a1 }));
} // namespace test
    template <class... Ts>
    constexpr auto
advance_all (std::tuple <Ts...>& first)
{
    static_for (
          [](auto& x){ ++x; }
        , first
    );
}
} // namespace detail

    template <class T, class... Keys>
    class
multikey_multimap_t
{
public:
        using
    key_type = std::tuple <Keys...>;
        using
    mapped_type = T;
        using
    value_type = std::pair<const key_type, T>;
        using
    size_type = std::size_t;
        using
    difference_type = std::ptrdiff_t;
    /*
        using
    key_compare = ;
        using
    allocator_type = ;
    */
        using
    reference = value_type&;
        using
    const_reference = const value_type&;
    /*
        using
    pointer = ;
        using
    const_pointer = ;
    */
private:
        using
    value_t = T;
        using
    pointer_t = value_t*;

    /** Container fot the values.
     * It's a multimap for direct access to the values when all the keys are 
     * known.
     */
        using
    key_tuple_t = std::tuple <Keys...>;
        using
    values_container_t = std::multimap <key_tuple_t, value_t>;
        values_container_t 
    values_container_m;
        using
    values_iterator_t = typename values_container_t::iterator;

public:
        using
    iterator = typename values_container_t::iterator;
        using
    const_iterator = typename values_container_t::const_iterator;
        using
    reverse_iterator = std::reverse_iterator<iterator>;
        using
    const_reverse_iterator = std::reverse_iterator<const_iterator>;
    /*
        using
    node_type = ;
    */

        iterator
    begin () noexcept
    {
        return values_container_m.begin ();
    }
        iterator
    end () noexcept
    {
        return values_container_m.end ();
    }
        const_iterator
    begin () const noexcept
    {
        return values_container_m.begin ();
    }
        const_iterator
    end () const noexcept
    {
        return values_container_m.end ();
    }
        iterator
    insert (value_type const& value)
    {
            auto
        iterator = values_container_m.insert (value);
        static_for ([&](
                  auto& key_map
                , auto const& key
                , auto& reverse_key_map
            ){
                insert_with_value_order (
                      key_map
                    , key
                    , iterator
                    , reverse_key_map
                );
            }
            , keys_m
            , value.first
            , reverse_keys_m
        );
        return iterator;
    }
        const_iterator
    find (key_type const& key) const
    {
        return values_container_m.find (key);
    }

    multikey_multimap_t (std::initializer_list <value_type> list)
    {
        for (auto&& e: list)
        {
            insert (std::move (e));
        }
    }
    multikey_multimap_t () = default;
private:
    /** Keys container, i.e. keys -> value mappings.
     * It's a tuple of multimaps from keys to iterators of the values container.
     */
        template <class K>
        using
    key_map_t = std::multimap <K, values_iterator_t>;
        using
    keys_container_t = std::tuple <key_map_t <Keys>...>;
        keys_container_t
    keys_m;

    /** Reverse key container, i.e. value -> keys mappings.
     * It's a tuple of maps from pointers to values, to iterators of the previous 
     * containers.
     */
        template <class K>
        using
    key_iterator_t = typename key_map_t <K>::iterator;
        template <class K>
        using
    const_key_iterator_t = typename key_map_t <K>::const_iterator;
        template <class K>
        using
    reverse_key_map_t = std::map <pointer_t, key_iterator_t <K>>;
        using
    reverse_keys_container_t = std::tuple <reverse_key_map_t <Keys>...>;
        reverse_keys_container_t 
    reverse_keys_m;
    /** Value insertion.
     * Inserts a value in the values container, sets-up the keys and reverse 
     * keys containers. The important thing is that all { key, iterator }s are 
     * ordered by key AND by value in their container.
     */
        template <
              class KeyMap
            , class Key
            , class Iterator
            , class ReverseKeyMap
        >
        void
    insert_with_value_order (
          KeyMap& key_map
        , Key const& key
        , Iterator const& iterator
        , ReverseKeyMap& reverse_key_map
    ){
            auto
        [i1, i2] = key_map.equal_range (key);
        // Order by value
        for (auto i = i1; i != i2; ++i)
        {
            if (i->second->second > iterator->second)
            {
                    auto
                it = key_map.insert (i, {key, iterator});
                reverse_key_map.insert ({&(iterator->second), it});
                return;
            }
        }
            auto
        it = key_map.insert ({key, iterator});
        reverse_key_map.insert ({&(iterator->second), it});
    }
public:
        template <std::size_t I>
        class
    const_by_key_iterator
    {
    public:
            using 
        difference_type = std::ptrdiff_t;
            using 
        value_type = multikey_multimap_t::value_type;
            using 
        pointer    = value_type*;
            using 
        reference  = value_type&;
            using 
        iterator_category = typename const_key_iterator_t <key_type>::iterator_category;

            using
        key_t = std::tuple_element_t <I, key_tuple_t>;

        const_by_key_iterator (const_key_iterator_t <key_t> it)
            : it_m { it }
        {}
            const_by_key_iterator&
        operator ++ ()
        {
            ++it_m;
            return *this;
        }
            const_by_key_iterator&
        operator -- ()
        {
            --it_m;
            return *this;
        }
            const_by_key_iterator
        operator ++ (int)
        {
                auto
            copy { *this };
            operator ++ ();
            return copy;
        }
            const_by_key_iterator
        operator -- (int)
        {
                auto
            copy { *this };
            operator -- ();
            return copy;
        }
            value_type const&
        operator * () const
        {
            return *(it_m->second);
        }
            value_type const *
        operator -> () const
        {
            return &(*(it_m->second));
        }
            friend bool
        operator == (
              const_by_key_iterator <I> const& a
            , const_by_key_iterator <I> const& b
        ){
            return a.it_m == b.it_m;
        }
            friend bool
        operator != (
              const_by_key_iterator <I> const& a
            , const_by_key_iterator <I> const& b
        ){
            return a.it_m != b.it_m;
        }
        operator const_iterator () const
        {
            return it_m->second;
        }
            auto
        is_begin () const
        {
            return it_m == std::get <I> (keys_m).begin ();
        }
            auto
        is_end () const
        {
            return it_m == std::get <I> (keys_m).end ();
        }
    private:
            const_key_iterator_t <key_t> 
        it_m;
    };

        template <std::size_t I>
        auto
    key_begin () const
        -> const_by_key_iterator <I>
    {
        return std::get <I> (keys_m).begin ();
    }
        template <std::size_t I>
        auto
    key_end () const
        -> const_by_key_iterator <I>
    {
        return std::get <I> (keys_m).end ();
    }
        template <
              std::size_t I
            , class Key = std::tuple_element_t <I, key_tuple_t>
        >
        bool
    contains (Key const& key) const
    {
        return std::get <I> (keys_m).contains (key);
    }
        template <
              std::size_t I
            , class Key = std::tuple_element_t <I, key_tuple_t>
        >
        std::pair <const_by_key_iterator <I>, const_by_key_iterator <I>>
    equal_range (Key const& key) const
    {
        return std::get <I> (keys_m).equal_range (key);
    }

    /** Retrieve values by keys intersections.
     * This is std::set_intersection with the propper indirections.
     */
        /*
private:
        template <class Iterator>
        auto
    get_impl (Iterator const& i) const
    {
        return (*i);
    }
        template <std::size_t I>
        auto
    get_impl (const_by_key_iterator <I> i) const
    {
        return i;
    }
public:
        template <
              class Iterator1
            , class Iterator2
            , class OutputIterator
        >
        OutputIterator
    intersection (
          Iterator1 first1
        , Iterator1 last1
        , Iterator2 first2
        , Iterator2 last2
        , OutputIterator d_first
    ) const {
        while (first1 != last1 && first2 != last2) 
        {
            if (get_impl(first1)->second < get_impl(first2)->second) 
            {
                ++first1;
            } 
            else  
            {
                if (!(get_impl(first2)->second < get_impl(first1)->second)) 
                {
                    *d_first++ = get_impl (first1);
                    ++first1;
                }
                ++first2;
            }
        }
        return d_first;
    }
    */
        template <
              class OutputIterator
            , class... Iterators
        >
        auto
    intersect (
          std::tuple <Iterators...> first
        , std::tuple <Iterators...> last
        , OutputIterator d_first
    ){
        while (detail::not_at_end (first, last))
        {
                auto const
            max = detail::get_max (first);
            detail::advance_if_under (first, max);
            if (detail::all_same (first))
            {
                *d_first++ = std::get <0> (first);
                detail::advance_all (first);
            } 
        }
    }
        template <
              class OutputIterator
            , class... Iterators
        >
        auto
    intersect2 (
          OutputIterator d_first
        , std::pair <Iterators, Iterators>... pairs
    ){
            auto const
        first = std::tuple { std::get <0> (pairs)... };
            auto const
        last = std::tuple { std::get <1> (pairs)... };
        return intersect (first, last, d_first);
    }

private:
    // Build tuple of optional iterators
        /*
        template <class OutputIterator, class Tuple, std::size_t... Is>
        constexpr auto
    select (
          OutputIterator d_first
        , Tuple&& tuple_of_opt_keys
        , std::index_sequence <Is...>
    ){
            auto
        first = std::tuple {  };
        static_for ([](auto opt_k, auto i){

              }
            , std::tuple { opt_keys... }
            , std::tuple { Is... }
        );
    }
    */
public:
        /*
        template <
              class OutputIterator
            , std::size_t... Is
        >
        constexpr auto
    select (
          OutputIterator d_first
        , std::optional <Keys> const&... opt_keys
        , std::index_sequence <Is...> = std::index_sequence_for <Keys...> {}
    ){
            auto
        first = std::tuple { (opt_keys ? equal_range <Is> (*opt_keys).first : key_begin <Is>)... };
            auto
        flag = std::tuple { (opt_keys ? true : false)... };
            auto
        last = std::tuple { (opt_keys ? equal_range <Is> (*opt_keys).second : key_begin <Is>)... };
        while (detail::not_at_end (first, last, flag))
        {
                auto const
            max = detail::get_max (first, flag);
            detail::advance_if_under (first, max, flag);
            if (detail::all_same (first, flag))
            {
                *d_first++ = detail::get (first, flag);
                detail::advance_all (first, flag);
            } 
        }
    }
    */
        /*
private:
    // We need a type-erased const_by_key_iterator
        template <std::size_t... Is>
        class
    any_const_by_key_iterator
    {};
public:
        template <std::size_t... Is>
        class
    selector_t
    {
    public:
        selector_t (multikey_multimap_t const& map)
            : map_m { map }
        {}
            template <std::size_t I>
            constexpr auto
        refine (const_by_key_iterator <I> first, const_by_key_iterator <I> last)
        {
        }
    private:
            multikey_multimap_t const&
        map_m;
    };

        template <std::size_t... Is>
        constexpr auto
    make_selector_t (std::index_sequence <Is...>)
    {
        return selector_t <Is...> { *this };
    }

        constexpr auto
    select_all () const
    {
        return make_selector (std::index_sequence_for <Keys...> {});
    }
    */
private:
        template <
              class OutputIterator
            , std::size_t... Is
        >
        constexpr auto
    select_impl (
          OutputIterator d_first
        , std::index_sequence <Is...> = std::index_sequence_for <Keys...> {}
        , std::optional <Keys> const&... opt_keys
    ){
        for (auto i = values_container_m.begin (); i != values_container_m.end (); ++i)
        {
            if ((... && (!opt_keys || *opt_keys == std::get <Is> (i->first))))
            {
                *d_first++ = i;
            }
        }
        return d_first;
    }
public:
        template <
              class OutputIterator
        >
        constexpr auto
    select (
          OutputIterator d_first
        , std::optional <Keys> const&... opt_keys
    ){
        return select_impl (
              d_first
            , std::index_sequence_for <Keys...> {}
            , opt_keys...
        );
    }
};
} // namespace isto::multikey_multimap
