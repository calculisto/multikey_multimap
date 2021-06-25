#pragma once
#include <map>
#include <functional>

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
    difference_type =std::ptrdiff_t;
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
            const_by_key_iterator
        operator ++ (int)
        {
                auto
            copy { *this };
            operator ++ ();
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
        template <
              std::size_t I1
            , std::size_t I2
            , class OutputIterator
        >
        OutputIterator
    intersection (
          const_by_key_iterator <I1> first1
        , const_by_key_iterator <I1> last1
        , const_by_key_iterator <I2> first2
        , const_by_key_iterator <I2> last2
        , OutputIterator d_first
    ){
        while (first1 != last1 && first2 != last2) {
            if (first1->second < first2->second) {
                ++first1;
            } else  {
                if (!(first2->second < first1->second)) {
                    *d_first++ = *first1;
                    ++first1;
                }
                ++first2;
            }
        }
        return d_first;
    }
    multikey_multimap_t (std::initializer_list <value_type> list)
    {
        for (auto&& e: list)
        {
            insert (std::move (e));
        }
    }
    multikey_multimap_t () = default;
};
} // namespace isto::multikey_multimap
