#ifndef CPPUAL_JSON_ADAPTOR_H
#define CPPUAL_JSON_ADAPTOR_H

#define RAPIDJSON_HAS_STDSTRING 1

#include <cppual/memory/allocator.h>
#include <cppual/functional.h>
#include <cppual/signal.h>
#include <cppual/types.h>
#include <cppual/meta.h>
#include <cppual/string.h>
#include <cppual/containers.h>

#include <cppual/json/document.h>

#include <iostream>
#include <fstream>
#include <utility>
#include <cassert>
#include <tuple>

class QString;

namespace cppual { namespace json {

// ====================================================

template <typename T>
struct is_json_value : std::is_enum<T>
{ };

template <>
struct is_json_value<bool> : std::true_type
{ };

template <>
struct is_json_value<u16> : std::true_type
{ };

template <>
struct is_json_value<i16> : std::true_type
{ };

template <>
struct is_json_value<u32> : std::true_type
{ };

template <>
struct is_json_value<i32> : std::true_type
{ };

template <>
struct is_json_value<u64> : std::true_type
{ };

template <>
struct is_json_value<i64> : std::true_type
{ };

template <>
struct is_json_value<float> : std::true_type
{ };

template <>
struct is_json_value<double> : std::true_type
{ };

template <>
struct is_json_value<string> : std::true_type
{ };

template <>
struct is_json_value<std::string> : std::true_type
{ };

template <typename>
struct is_json_int : std:: false_type
{ };

template <>
struct is_json_int<u32> : std::true_type
{ };

template <>
struct is_json_int<i32> : std::true_type
{ };

template <>
struct is_json_int<u16> : std::true_type
{ };

template <>
struct is_json_int<i16> : std::true_type
{ };

template <>
struct is_json_int<u64> : std::true_type
{ };

template <>
struct is_json_int<i64> : std::true_type
{ };

//======================================================

template <typename>
struct is_json_float : std:: false_type
{ };

template <>
struct is_json_float<float> : std::true_type
{ };

template <>
struct is_json_float<double> : std::true_type
{ };

//======================================================

template <typename>
struct is_json_string : std::false_type
{ };

template <>
struct is_json_string<string> : std::true_type
{ };

template <>
struct is_json_string<std::string> : std::true_type
{ };

//======================================================

template <typename>
struct is_json_qstring : std::false_type
{ };

template <>
struct is_json_qstring<QString> : std::true_type
{ };

//======================================================

template <typename T,
          typename = typename std::enable_if<is_json_value<T>::value>::type>
class value_reference;

class template_object;
class template_array ;

template <typename T,
          typename A = memory::allocator<value_reference<T>>,
          typename   = typename std::enable_if<is_json_value<T>::value>::type
          >
class values_array;

template <typename T,
          typename A = memory::allocator<T>,
          typename   = typename std::enable_if<std::is_base_of<template_object, T>::value ||
                                               std::is_base_of<template_array , T>::value>::type
          >
class objects_array;

//======================================================

class SHARED_API doc_parser : protected Document
{
public:
    typedef Document                           base_type      ;
    typedef SizeType                           size_type      ;
    typedef string                             string_type    ;
    typedef Value                              value_type     ;
    typedef value_type*                        pointer        ;
    typedef value_type&                        reference      ;
    typedef value_type const&                  const_reference;
    typedef base_type::ValueIterator           iterator       ;
    typedef base_type::ConstValueIterator      const_iterator ;
    typedef base_type::Object                  object_type    ;
    typedef base_type::Array                   array_type     ;
    typedef std::pair<string_type, value_type> pair_type      ;

    enum class doc_type : u8
    {
        null,
        object,
        array
    };

    //! construct from file
    doc_parser(string_type const& file, doc_type type = doc_type::object);

    //! construct from memory
    doc_parser(std::istream& data, doc_type type = doc_type::object);

    //! recreate document from file
    bool create_document(string_type const& file, doc_type type = doc_type::object);

    bool save(std::ostream& stream);

    doc_type  type() const;
    size_type size() const;

    const_reference operator [] (string_type const& key) const;
    reference       operator [] (string_type const& key);
    const_reference operator [] (size_type idx) const;
    reference       operator [] (size_type idx);

    signal<void()> changed;
    signal<void()> about_to_save;

    friend class template_object;
    friend class template_array ;

    template <typename, typename, typename>
    friend class values_array;

    template <typename, typename, typename>
    friend class objects_array;

    template <typename T>
    friend class value_reference_base;

    template <typename T, typename>
    friend class value_reference;

private:
    doc_type _M_type;
};

// ======================================================================

union SHARED_API template_pointer
{
    template_object* object;
    template_array*  array ;
    void*            ptr   ;

    constexpr template_pointer () noexcept : ptr () { }
    constexpr template_pointer (template_object* ptr) noexcept : object (ptr) { }
    constexpr template_pointer (template_array * ptr) noexcept : array  (ptr) { }

    friend
    constexpr bool operator == (template_pointer const&, template_pointer const&) noexcept;

    friend
    constexpr bool operator == (template_pointer const&, std::nullptr_t) noexcept;

    friend
    constexpr bool operator == (std::nullptr_t, template_pointer const&) noexcept;
};

// =========================================================

constexpr bool operator == (template_pointer const& lh, template_pointer const& rh) noexcept
{ return lh.ptr == rh.ptr; }

constexpr bool operator == (template_pointer const& lh, std::nullptr_t) noexcept
{ return lh.ptr == nullptr; }

constexpr bool operator == (std::nullptr_t, template_pointer const& rh) noexcept
{ return rh.ptr == nullptr; }

constexpr bool operator != (template_pointer const& lh, template_pointer const& rh) noexcept
{ return !(lh == rh); }

constexpr bool operator != (template_pointer const& lh, std::nullptr_t) noexcept
{ return !(lh == nullptr); }

constexpr bool operator != (std::nullptr_t, template_pointer const& rh) noexcept
{ return !(rh == nullptr); }

// ======================================================================

class SHARED_API template_owner
{
public:
    typedef template_pointer          pointer  ;
    typedef pointer template_owner::* safe_bool;

    constexpr template_owner () noexcept
    : _M_owner (),
      _M_type  ()
    { }

    constexpr template_owner (template_object& obj)
    : _M_owner (&obj), _M_type (doc_parser::doc_type::object)
    {
        assert(_M_owner != nullptr);
    }

    constexpr template_owner (template_array& array)
    : _M_owner (&array), _M_type (doc_parser::doc_type::array)
    {
        assert(_M_owner != nullptr);
    }

    constexpr template_object* object () const
    {
        assert(_M_owner != nullptr);
        assert(_M_type  == doc_parser::doc_type::object);
        return _M_owner.object;
    }

    constexpr template_array* array () const
    {
        assert(_M_owner != nullptr);
        assert(_M_type  == doc_parser::doc_type::array);
        return _M_owner.array;
    }

    constexpr explicit operator safe_bool () const noexcept
    {
        return _M_owner != nullptr &&
              (_M_type  == doc_parser::doc_type::object || _M_type == doc_parser::doc_type::array) ?
               &template_owner::_M_owner : nullptr;
    }

    constexpr bool operator == (std::nullptr_t) const noexcept
    {
        return _M_owner == nullptr;
    }

    constexpr bool operator != (std::nullptr_t) const noexcept
    {
        return _M_owner != nullptr;
    }

    constexpr doc_parser::doc_type type () const noexcept
    {
        return _M_type;
    }

private:
    pointer              _M_owner;
    doc_parser::doc_type _M_type ;
};

// ======================================================================

class SHARED_API template_object
{
public:
    typedef doc_parser::size_type              size_type      ;
    typedef doc_parser::value_type             value_type     ;
    typedef doc_parser::reference              reference      ;
    typedef doc_parser::pointer                pointer        ;
    typedef doc_parser::const_reference        const_reference;
    typedef doc_parser::ValueIterator          iterator       ;
    typedef doc_parser::ConstValueIterator     const_iterator ;
    typedef doc_parser::string_type            string_type    ;
    typedef std::pair<string_type, value_type> pair_type      ;

    static constexpr auto npos = static_cast<size_type> (-1);

    template_object () = delete;
    template_object (template_object&&);
    template_object (template_object const&);
    template_object (doc_parser& _parser, string_type const& _category);
    template_object (doc_parser& _parser, size_type _index);
    template_object (template_object* _owner, string_type const& _category);
    template_object (template_array* _owner, size_type _idx);

    template_object& operator = (template_object const&);
    template_object& operator = (template_object&&);

    virtual ~template_object();

    const_reference operator [] (string_type const& key) const;
    reference       operator [] (string_type const& key);

    virtual size_type size () const;
    virtual bool      empty() const;

    constexpr pointer operator -> () const
    {
        assert(_M_ref != nullptr && _M_ref->IsObject());
        return _M_ref;
    }

    constexpr pointer ref () const noexcept
    {
        return _M_ref;
    }

    constexpr doc_parser* parser () const noexcept
    {
        return _M_parser;
    }

    inline string_type category () const noexcept
    {
        return _M_category;
    }

    constexpr size_type index () const noexcept
    {
        return _M_index;
    }

    constexpr static doc_parser::doc_type type () noexcept
    {
        return doc_parser::doc_type::object;
    }

    constexpr explicit operator bool () const noexcept
    {
        return _M_ref != nullptr;
    }

    void on_parser_data_changed();
    void on_about_to_save();
    void on_object_changed();
    void on_array_changed();
    void on_removed(size_type idx);

    signal<void()> about_to_save;
    signal<void()> changed;
    signal<void()> invalidated;
    signal<void()> destroyed;

private:
    pointer parent_object_ref() const;
    pointer parent_array_ref () const;
    void    connections();
    void    disconnections();
    void    invalidate();
    void    assign_from_parser_object();
    void    assign_from_parser_array();
    void    assign_from_object();
    void    assign_from_array();

private:
    template_owner _M_owner   ;
    doc_parser*    _M_parser  ;
    pointer        _M_ref     ;
    string_type    _M_category;
    size_type      _M_index   ;

    template <typename>
    friend class value_reference_base;

    template <typename, typename, typename>
    friend class values_array;

    template <typename, typename, typename>
    friend class objects_array;

    friend class template_array;
};

//======================================================

class SHARED_API template_array
{
public:
    typedef doc_parser::size_type              size_type      ;
    typedef doc_parser::value_type             value_type     ;
    typedef doc_parser::reference              reference      ;
    typedef doc_parser::pointer                pointer        ;
    typedef doc_parser::const_reference        const_reference;
    typedef doc_parser::iterator               iterator       ;
    typedef doc_parser::const_iterator         const_iterator ;
    typedef doc_parser::string_type            string_type    ;
    typedef std::pair<string_type, value_type> pair_type      ;

    static constexpr auto npos = static_cast<size_type> (-1);

    template_array () = delete;
    template_array (template_array&&);
    template_array (template_array const&);
    template_array (doc_parser& _parser, string_type const& _category);
    template_array (doc_parser& _parser, size_type _index);
    template_array (template_object* _owner, string_type const& _category);
    template_array (template_array* _owner, size_type _idx);

    template_array& operator = (template_array const&);
    template_array& operator = (template_array&&);

    virtual ~template_array ();

    const_reference operator [] (size_type idx) const;
    reference       operator [] (size_type idx);

    virtual size_type size   () const;
    virtual bool      empty  () const;
    virtual void      append ();
    virtual void      remove (size_type idx);
    virtual void      clear  ();

    constexpr pointer operator -> () const
    {
        assert(_M_ref != nullptr && _M_ref->IsArray());
        return _M_ref;
    }

    constexpr pointer ref () const noexcept
    {
        return _M_ref;
    }

    constexpr doc_parser* parser () const noexcept
    {
        return _M_parser;
    }

    inline string_type category () const noexcept
    {
        return _M_category;
    }

    constexpr size_type index () const noexcept
    {
        return _M_index;
    }

    constexpr static doc_parser::doc_type type () noexcept
    {
        return doc_parser::doc_type::array;
    }

    constexpr explicit operator bool () const noexcept
    {
        return _M_ref != nullptr;
    }

    void on_parser_data_changed();
    void on_about_to_save();
    void on_object_changed();
    void on_array_changed();
    void on_removed(size_type idx);

    signal<void()>          about_to_save;
    signal<void()>          about_to_append;
    signal<void()>          appended;
    signal<void(size_type)> about_to_remove;
    signal<void(size_type)> removed;
    signal<void()>          about_to_clear;
    signal<void()>          cleared;
    signal<void()>          changed;
    signal<void()>          invalidated;
    signal<void()>          destroyed;

private:
    pointer parent_object_ref() const;
    pointer parent_array_ref () const;
    void    connections();
    void    disconnections();
    void    invalidate();
    void    assign_from_parser_object();
    void    assign_from_parser_array();
    void    assign_from_object();
    void    assign_from_array();

private:
    template_owner _M_owner   ;
    doc_parser*    _M_parser  ;
    pointer        _M_ref     ;
    string_type    _M_category;
    size_type      _M_index   ;

    template <typename>
    friend class value_reference_base;

    template <typename, typename, typename>
    friend class values_array;

    template <typename, typename, typename>
    friend class objects_array;

    friend class template_object;
};

// ======================================================================

template <typename T>
class SHARED_API value_reference_base
{
public:
    static_assert (is_json_value<T>::value, "invalid type!");

    typedef typename std::remove_cv_t<T> base_type;
    typedef value_reference_base<T>      self_type;
    typedef function<void(self_type*)>   func_type;

    typedef typename std::conditional<std::is_same<i16, T>::value ||
                                      std::is_enum<T>::value,
            typename std::conditional<sizeof (T) <= sizeof (i32), i32, i64>::type,
            typename std::conditional<std::is_same<u16, T>::value, uint, T>::type
            >::type
            default_type;

    typedef base_type                       value_type ;
    typedef doc_parser::pointer             pointer    ;
    typedef doc_parser::size_type           size_type  ;
    typedef doc_parser::value_type          json_type  ;
    typedef doc_parser::string_type         string_type;

    static constexpr auto npos = static_cast<size_type> (-1);

    inline explicit operator bool () const noexcept
    {
        return _M_owner.type() != doc_parser::doc_type::null &&
               _M_ref != nullptr && !_M_ref->IsNull();
    }

    inline Type type() const
    {
        return ref()->GetType();
    }

    value_reference_base () = delete;
    value_reference_base (value_reference_base&&) = delete;
    value_reference_base& operator = (value_reference_base&&) = delete;

    inline value_reference_base (value_reference_base const& obj) noexcept
    : _M_ref(obj._M_ref),
      _M_owner(obj._M_owner),
      _M_fn(obj._M_fn)
    {
        connections();
    }

    virtual ~value_reference_base()
    {
        disconnections();
    }

    inline value_reference_base& operator = (value_reference_base const& obj) noexcept
    {
        if (this != &obj && obj._M_ref && _M_ref != obj._M_ref)
        {
            disconnections();

            _M_ref    = obj._M_ref  ;
            _M_owner  = obj._M_owner;
            _M_fn     = obj._M_fn   ;

            connections();
            changed();
        }

        return *this;
    }

    constexpr pointer ref() const
    {
        assert(_M_ref != nullptr);
        return _M_ref;
    }

    constexpr template_owner owner() const noexcept
    {
        return _M_owner;
    }

    signal<void()> changed;

protected:
    inline value_reference_base (template_object*    owner,
                                 string_type  const& key,
                                 default_type const& default_val = default_type ())
    : _M_ref  (),
      _M_owner(*owner)
    {
        _M_fn = func_type([def_key = key, def = default_val](self_type* ptr)
        {
            if(def_key.empty()) return;

            if(!(*ptr->_M_owner.object())->HasMember(def_key.c_str()))
            {
                std::cout << "Json::" << __func__ << ": invalid reference "
                          << def_key  << " with default value: "
                          << def      << ". Trying to create a new one." << std::endl;

                //! try to obtain a valid reference
                (*ptr->_M_owner.object())->AddMember(json_type(def_key.c_str(),
                                                     static_cast<size_type>(def_key.size()),
                                                     ptr->_M_owner.object()->parser()->GetAllocator()).Move(),
                                                     json_type(def).Move(),
                                                     ptr->_M_owner.object()->parser()->GetAllocator());
            }

            ptr->_M_ref = &(ptr->_M_owner.object()->operator[](def_key));

            if(!ptr->_M_ref)
            {
                throw std::runtime_error("json reference is nullptr!");
            }
        });

        _M_fn(this);
        connections();
    }

    inline value_reference_base (template_array*     owner,
                                 size_type           idx,
                                 default_type const& default_val = default_type ())
    : _M_ref  (),
      _M_owner(*owner)
    {
        _M_fn = func_type([idx, def = default_val](self_type* ptr)
        {
            auto new_idx = idx;

            if ((*ptr->_M_owner.array())->Size() <= idx)
            {
                std::cout << "Json::" << __func__ << ": invalid reference index "
                          << idx      << " with default value: "
                          << def      << ". Trying to create a new one." << std::endl;

                //! try to obtain a valid reference
                (*ptr->_M_owner.array())->PushBack(json_type(def).Move(),
                                                   ptr->_M_owner.array()->parser()->GetAllocator());

                new_idx = (*ptr->_M_owner.array())->Size() - 1;
            }

            ptr->_M_ref = &(ptr->_M_owner.array()->operator[](new_idx));

            if(!ptr->_M_ref)
            {
                throw std::runtime_error("json reference is nullptr!");
            }
        });

        _M_fn(this);
        connections();
    }

    constexpr pointer parent_object_ref() const
    {
        return _M_owner.object()->_M_ref;
    }

    constexpr pointer parent_array_ref() const
    {
        return _M_owner.array()->_M_ref;
    }

private:
    void connections()
    {
        if (_M_owner.type() == doc_parser::doc_type::object)
        {
            connect(_M_owner.object()->changed, *this, &value_reference_base::assign);
            connect(_M_owner.object()->destroyed, *this, &value_reference_base::invalidate);
        }
        else if (_M_owner.type() == doc_parser::doc_type::array)
        {
            connect(_M_owner.array()->changed, *this, &value_reference_base::assign);
            connect(_M_owner.array()->destroyed, *this, &value_reference_base::invalidate);
        }
    }

    void disconnections()
    {
        if (_M_owner.type() == doc_parser::doc_type::object)
        {
            disconnect(_M_owner.object()->changed, *this, &value_reference_base::assign);
            disconnect(_M_owner.object()->destroyed, *this, &value_reference_base::invalidate);
        }
        else if (_M_owner.type() == doc_parser::doc_type::array)
        {
            disconnect(_M_owner.array()->changed, *this, &value_reference_base::assign);
            disconnect(_M_owner.array()->destroyed, *this, &value_reference_base::invalidate);
        }
    }

    void assign()
    {
        if(_M_fn == nullptr) return;

        _M_fn(this);
        changed();
    }

    void invalidate()
    {
        disconnections();

        _M_ref   = nullptr;
        _M_owner = template_owner ();
        _M_fn    = func_type ();
    }

private:
    pointer        _M_ref   ;
    template_owner _M_owner ;
    func_type      _M_fn    ;

    friend class value_reference<T>;

    template <typename, typename, typename>
    friend class values_array;

    template <typename, typename, typename>
    friend class objects_array;
};

// ======================================================================

template <>
class SHARED_API value_reference_base<doc_parser::string_type>
{
public:
    typedef value_reference_base<doc_parser::string_type> self_type  ;
    typedef doc_parser::pointer                           pointer    ;
    typedef doc_parser::size_type                         size_type  ;
    typedef doc_parser::string_type                       value_type ;
    typedef doc_parser::value_type                        json_type  ;
    typedef doc_parser::string_type                       string_type;
    typedef function<void(self_type*)>                    func_type  ;

    static constexpr auto npos = static_cast<size_type> (-1);

    inline explicit operator bool () const noexcept
    {
        return _M_owner.type() != doc_parser::doc_type::null &&
               _M_ref != nullptr && !_M_ref->IsNull();
    }

    inline Type type() const
    {
        return ref()->GetType();
    }

    value_reference_base () = delete;
    value_reference_base (value_reference_base&&) = delete;
    value_reference_base& operator = (value_reference_base&&) = delete;

    inline value_reference_base (value_reference_base const& obj) noexcept
    : _M_ref(obj._M_ref),
      _M_owner(obj._M_owner),
      _M_fn(obj._M_fn)
    {
        connections();
    }

    virtual ~value_reference_base()
    {
        disconnections();
    }

    inline value_reference_base& operator = (value_reference_base const& obj) noexcept
    {
        if (this != &obj && obj._M_ref && _M_ref != obj._M_ref)
        {
            disconnections();

            _M_ref    = obj._M_ref  ;
            _M_owner  = obj._M_owner;
            _M_fn     = obj._M_fn   ;

            connections();
            changed();
        }

        return *this;
    }

    inline pointer ref() const
    {
        assert(_M_ref != nullptr);
        return _M_ref;
    }

    constexpr template_owner owner() const noexcept
    {
        return _M_owner;
    }

    signal<void()> changed;

protected:
    value_reference_base (template_object*   owner,
                          string_type const& key,
                          value_type  const& default_val = value_type ());

    value_reference_base (template_array*   owner,
                          size_type         idx,
                          value_type const& default_val = value_type ());

    constexpr pointer parent_object_ref() const
    {
        return _M_owner.object()->_M_ref;
    }

    constexpr pointer parent_array_ref() const
    {
        return _M_owner.array()->_M_ref;
    }

private:
    void connections();
    void disconnections();
    void assign();
    void invalidate();

private:
    pointer        _M_ref   ;
    template_owner _M_owner ;
    func_type      _M_fn    ;

    friend class value_reference<value_type>;

    template <typename, typename, typename>
    friend class values_array;

    template <typename, typename, typename>
    friend class objects_array;
};

//======================================================

template <typename T>
class SHARED_API value_reference<T> : public value_reference_base<T>
{
public:
    static_assert (std::is_enum_v<T>, "T is NOT an enum!");

    typedef value_reference_base<T>               reference_base;
    typedef doc_parser::size_type                 size_type     ;
    typedef typename reference_base::json_type    json_type     ;
    typedef typename reference_base::string_type  string_type   ;
    typedef typename reference_base::value_type   value_type    ;
    typedef typename reference_base::default_type default_type  ;

    inline value_reference (template_object*    owner,
                            string_type  const& name,
                            default_type const& default_val = default_type())
    : reference_base(owner, name, default_val)
    { }

    inline value_reference (template_array*     owner,
                            size_type           idx,
                            default_type const& default_val = default_type())
    : reference_base(owner, idx, default_val)
    { }

    inline value_reference (value_reference const& obj)
    : reference_base(obj)
    { }

    inline operator value_type () const noexcept
    {
        return static_cast<value_type>(sizeof (default_type) <= sizeof (i32) ? this->ref()->GetInt   () :
                                                                               this->ref()->GetInt64 ());
    }

    inline auto value () const noexcept
    {
        return static_cast<std::underlying_type_t<value_type>>
                          (sizeof (default_type) <= sizeof (i32) ? this->ref()->GetInt   () :
                                                                   this->ref()->GetInt64 ());
    }

    inline value_reference& operator = (value_reference const& ref)
    {
        reference_base::operator = (ref);
        return *this;
    }

    inline value_reference& operator = (value_type value)
    {
        if (sizeof (default_type) <= sizeof (i32))
            this->ref()->SetInt (static_cast<i32> (value));
        else
            this->ref()->SetInt64 (static_cast<i64>(value));

        return *this;
    }
};

//======================================================

template <>
class SHARED_API value_reference<bool> : public value_reference_base<bool>
{
public:
    typedef value_reference_base<bool>           reference_base;
    typedef typename reference_base::json_type   json_type     ;
    typedef typename reference_base::string_type string_type   ;
    typedef typename reference_base::value_type  value_type    ;

    value_reference (template_object*   owner,
                     string_type const& name,
                     value_type  const& default_val = value_type());

    value_reference (template_array*   owner,
                     size_type         idx,
                     value_type const& default_val = value_type());

    inline value_reference (value_reference const& obj)
    : reference_base(obj)
    { }

    inline operator value_type () const noexcept
    {
        return ref()->GetBool();
    }

    inline value_type value () const noexcept
    {
        return this->ref()->GetBool();
    }

    inline value_reference& operator = (value_reference const& ref)
    {
        reference_base::operator = (ref);
        return *this;
    }

    inline value_reference& operator = (value_type value)
    {
        ref()->SetBool(value);

        return *this;
    }
};

//======================================================

template <>
class SHARED_API value_reference<u16> : public value_reference_base<u16>
{
public:
    typedef value_reference_base<u16>             reference_base;
    typedef typename reference_base::json_type    json_type     ;
    typedef typename reference_base::string_type  string_type   ;
    typedef typename reference_base::value_type   value_type    ;
    typedef typename reference_base::default_type default_type  ;

    value_reference (template_object*   owner,
                     string_type const& name,
                     value_type         default_val = value_type());

    value_reference (template_array* owner,
                     size_type       idx,
                     value_type      default_val = value_type());

    inline value_reference (value_reference const& obj)
    : reference_base(obj)
    { }

    inline operator value_type () const noexcept
    {
        return static_cast<value_type>(ref()->GetUint());
    }

    inline value_type value () const noexcept
    {
        return static_cast<value_type>(ref()->GetUint());
    }

    inline value_reference& operator = (value_reference const& ref) noexcept
    {
        reference_base::operator = (ref);
        return *this;
    }

    inline value_reference& operator = (value_type value)
    {
        ref()->SetUint(static_cast<uint>(value));

        return *this;
    }
};

//======================================================

template <>
class SHARED_API value_reference<i16> : public value_reference_base<i16>
{
public:
    typedef value_reference_base<i16>             reference_base;
    typedef typename reference_base::json_type    json_type     ;
    typedef typename reference_base::string_type  string_type   ;
    typedef typename reference_base::value_type   value_type    ;
    typedef typename reference_base::default_type default_type  ;

    value_reference (template_object*   owner,
                     string_type const& name,
                     value_type         default_val = value_type());

    value_reference (template_array* owner,
                     size_type       idx,
                     value_type      default_val = value_type());

    inline value_reference (value_reference const& obj)
    : reference_base(obj)
    { }

    inline operator value_type () const noexcept
    {
        return static_cast<value_type>(ref()->GetInt());
    }

    inline value_type value () const noexcept
    {
        return static_cast<value_type>(ref()->GetInt());
    }

    inline value_reference& operator = (value_reference const& ref) noexcept
    {
        reference_base::operator = (ref);
        return *this;
    }

    inline value_reference& operator = (value_type value)
    {
        ref()->SetInt(static_cast<int>(value));

        return *this;
    }
};

//======================================================

template <>
class SHARED_API value_reference<u32> : public value_reference_base<u32>
{
public:
    typedef value_reference_base<u32>            reference_base;
    typedef typename reference_base::json_type   json_type     ;
    typedef typename reference_base::string_type string_type   ;
    typedef typename reference_base::value_type  value_type    ;

    value_reference (template_object*   owner,
                     string_type const& name,
                     value_type         default_val = value_type());

    value_reference (template_array* owner,
                     size_type       idx,
                     value_type      default_val = value_type());

    inline value_reference (value_reference const& obj)
    : reference_base(obj)
    { }

    inline operator value_type () const noexcept
    {
        return ref()->GetUint();
    }

    inline value_type value () const noexcept
    {
        return ref()->GetUint();
    }

    inline value_reference& operator = (value_reference const& ref) noexcept
    {
        reference_base::operator = (ref);
        return *this;
    }

    inline value_reference& operator = (value_type value)
    {
        ref()->SetUint(value);

        return *this;
    }
};

//======================================================

template <>
class SHARED_API value_reference<i32> : public value_reference_base<i32>
{
public:
    typedef value_reference_base<i32>            reference_base;
    typedef typename reference_base::json_type   json_type     ;
    typedef typename reference_base::string_type string_type   ;
    typedef typename reference_base::value_type  value_type    ;

    value_reference (template_object*   owner,
                     string_type const& name,
                     value_type  const& default_val = value_type());

    value_reference (template_array*   owner,
                     size_type         idx,
                     value_type const& default_val = value_type());

    inline value_reference (value_reference const& obj)
    : reference_base(obj)
    { }

    inline operator value_type () const noexcept
    {
        return ref()->GetInt();
    }

    inline value_type value () const noexcept
    {
        return ref()->GetInt();
    }

    inline value_reference& operator = (value_reference const& ref) noexcept
    {
        reference_base::operator = (ref);
        return *this;
    }

    inline value_reference& operator = (value_type value) noexcept
    {
        ref()->SetInt(value);

        return *this;
    }
};

//======================================================

template <>
class SHARED_API value_reference<u64> : public value_reference_base<u64>
{
public:
    typedef value_reference_base<u64>            reference_base;
    typedef typename reference_base::json_type   json_type     ;
    typedef typename reference_base::string_type string_type   ;
    typedef typename reference_base::value_type  value_type    ;

    value_reference (template_object*   owner,
                     string_type const& name,
                     value_type  const& default_val = value_type());

    value_reference (template_array*   owner,
                     size_type         idx,
                     value_type const& default_val = value_type());

    inline value_reference (value_reference const& obj)
    : reference_base(obj)
    { }

    inline operator value_type () const noexcept
    {
        return ref()->GetUint64();
    }

    inline value_type value () const noexcept
    {
        return ref()->GetUint64();
    }

    inline value_reference& operator = (value_reference const& ref) noexcept
    {
        reference_base::operator = (ref);
        return *this;
    }

    inline value_reference& operator = (value_type const& value)
    {
        ref()->SetUint64(value);

        return *this;
    }
};

//======================================================

template <>
class SHARED_API value_reference<i64> : public value_reference_base<i64>
{
public:
    typedef value_reference_base<i64>            reference_base;
    typedef typename reference_base::json_type   json_type     ;
    typedef typename reference_base::string_type string_type   ;
    typedef typename reference_base::value_type  value_type    ;

    value_reference (template_object*   owner,
                     string_type const& name,
                     value_type  const& default_val = value_type());

    value_reference (template_array*   owner,
                     size_type         idx,
                     value_type const& default_val = value_type());

    inline value_reference (value_reference const& obj)
    : reference_base(obj)
    { }

    inline operator value_type () const noexcept
    {
        return ref()->GetInt64();
    }

    inline value_type value () const noexcept
    {
        return ref()->GetInt64();
    }

    inline value_reference& operator = (value_reference const& ref) noexcept
    {
        reference_base::operator = (ref);
        return *this;
    }

    inline value_reference& operator = (value_type const& value)
    {
        ref()->SetInt64(value);

        return *this;
    }
};

//======================================================

template <>
class SHARED_API value_reference<float> : public value_reference_base<float>
{
public:
    typedef value_reference_base<float>          reference_base;
    typedef typename reference_base::json_type   json_type     ;
    typedef typename reference_base::string_type string_type   ;
    typedef typename reference_base::value_type  value_type    ;

    value_reference (template_object*   owner,
                     string_type const& name,
                     value_type  const& default_val = value_type());

    value_reference (template_array*   owner,
                     size_type         idx,
                     value_type const& default_val = value_type());

    inline value_reference (value_reference const& obj)
    : reference_base(obj)
    { }

    inline operator value_type () const noexcept
    {
        return ref()->GetFloat();
    }

    inline value_type value () const noexcept
    {
        return ref()->GetFloat();
    }

    inline value_reference& operator = (value_reference const& ref) noexcept
    {
        reference_base::operator = (ref);
        return *this;
    }

    inline value_reference& operator = (value_type const& value)
    {
        ref()->SetFloat(value);

        return *this;
    }
};

//======================================================

template <>
class SHARED_API value_reference<double> : public value_reference_base<double>
{
public:
    typedef value_reference_base<double>         reference_base;
    typedef typename reference_base::json_type   json_type     ;
    typedef typename reference_base::string_type string_type   ;
    typedef typename reference_base::value_type  value_type    ;

    value_reference (template_object*   owner,
                     string_type const& name,
                     value_type  const& default_val = value_type());

    value_reference (template_array*   owner,
                     size_type         idx,
                     value_type const& default_val = value_type());

    inline value_reference (value_reference const& obj)
    : reference_base(obj)
    { }

    inline operator value_type () const noexcept
    {
        return ref()->GetDouble();
    }

    inline value_type value () const noexcept
    {
        return ref()->GetDouble();
    }

    inline value_reference& operator = (value_reference const& ref) noexcept
    {
        reference_base::operator = (ref);
        return *this;
    }

    inline value_reference& operator = (value_type const& value)
    {
        ref()->SetDouble(value);

        return *this;
    }
};

//======================================================

template <>
class SHARED_API value_reference<doc_parser::string_type> : public
                 value_reference_base<doc_parser::string_type>
{
public:
    typedef value_reference_base<doc_parser::string_type> reference_base;
    typedef typename reference_base::json_type            json_type     ;
    typedef reference_base::string_type                   string_type   ;
    typedef reference_base::value_type                    value_type    ;

    value_reference (template_object*   owner,
                     string_type const& name,
                     value_type  const& default_val = value_type());

    value_reference (template_array*   owner,
                     size_type         idx,
                     value_type const& default_val = value_type());

    template <typename Processor,
              typename = typename std::enable_if<
                  std::is_same<typename std::result_of<Processor()>::type, value_type>::value,
                  typename std::result_of<Processor()>::type
                  >::type
              >
    inline value_reference (template_object*   owner,
                            string_type const& name,
                            Processor&&        fn,
                            value_type  const& default_val = value_type())
    : reference_base(owner, name, default_val)
    {
        if (!_M_ref->IsString()) throw std::runtime_error("Reference is NOT String");

        connect(json_string_invoked, [func = fn](string_type& val)
        {
            val = func ();
        });
    }

    template <typename Processor,
              typename E = typename std::enable_if<
                  std::is_enum<typename std::result_of<Processor()>::type>::value,
                  typename std::result_of<Processor()>::type
                  >::type
              >
    inline value_reference (template_object*   owner,
                            string_type const& name,
                            Processor&&        fn,
                            value_type  const& default_val = value_type(),
                            E = E())
    : reference_base(owner, name, default_val)
    {
        if (!_M_ref->IsString()) throw std::runtime_error("Reference is NOT String");

        connect(json_enum_invoked, [func = fn](int& val)
        {
            val = func ();
        });
    }

    template <typename Processor,
              typename = typename std::enable_if<
                  std::is_same<typename std::result_of<Processor()>::type, value_type>::value,
                  typename std::result_of<Processor()>::type
                  >::type
              >
    inline value_reference (template_array*   owner,
                            size_type         idx,
                            Processor&&       fn,
                            value_type const& default_val = value_type())
    : reference_base(owner, idx, default_val)
    {
        if (!_M_ref->IsString()) throw std::runtime_error("Reference is NOT String");

        connect(json_string_invoked, [func = fn](string_type& val)
        {
            val = func ();
        });
    }

    template <typename Processor,
              typename E = typename std::enable_if<
                  std::is_enum<typename std::result_of<Processor()>::type>::value,
                  typename std::result_of<Processor()>::type
                  >::type
              >
    inline value_reference (template_array*   owner,
                            size_type         idx,
                            Processor&&       fn,
                            value_type const& default_val = value_type(),
                            E = E())
    : reference_base(owner, idx, default_val)
    {
        if (!_M_ref->IsString()) throw std::runtime_error("Reference is NOT String");

        connect(json_enum_invoked, [func = fn](int& val)
        {
            val = func ();
        });
    }

    inline value_reference (value_reference const& obj)
    : reference_base(obj),
      json_enum_invoked(obj.json_enum_invoked),
      json_string_invoked(obj.json_string_invoked)
    { }

    inline operator value_type () const
    {
        return value_type(ref()->GetString(), ref()->GetStringLength());
    }

    inline value_type value () const noexcept
    {
        return value_type(ref()->GetString(), ref()->GetStringLength());
    }

    inline value_type operator () () const
    {
        value_type value;

        json_string_invoked(value);
        return value;
    }

    template <typename U,
              typename = typename std::enable_if<std::is_enum<U>::value>::type
              >
    inline U toEnum () const
    {
        int value = 0;

        json_enum_invoked(value);
        return static_cast<U>(value);
    }

    inline cchar* c_str() const
    {
        return ref()->GetString();
    }

    //! string length
    inline size_type length() const
    {
        return ref()->GetStringLength();
    }

    //! string length
    inline size_type size() const
    {
        return length();
    }

    inline value_reference& operator = (value_reference const& rh) noexcept
    {
        if (this != &rh)
        {
            reference_base::operator = (rh);

            json_enum_invoked   = rh.json_enum_invoked  ;
            json_string_invoked = rh.json_string_invoked;
        }

        return *this;
    }

    inline value_reference& operator = (value_type const& value)
    {
        switch (owner().type())
        {
        case doc_parser::doc_type::object:
            ref()->SetString(value.c_str(),
                             static_cast<size_type>(value.size()),
                             owner().object()->parser()->GetAllocator());
            break;
        case doc_parser::doc_type::array:
            ref()->SetString(value.c_str(),
                             static_cast<size_type>(value.size()),
                             owner().array()->parser()->GetAllocator());
            break;
        default:
            break;
        }

        return *this;
    }

    inline value_reference& operator = (value_type::value_type const* value)
    {
        switch (owner().type())
        {
        case doc_parser::doc_type::object:
            ref()->SetString(value, owner().object()->parser()->GetAllocator());
            break;
        case doc_parser::doc_type::array:
            ref()->SetString(value, owner().array()->parser()->GetAllocator());
            break;
        default:
            break;
        }

        return *this;
    }

private:
    signal<void(int&)>         json_enum_invoked  ;
    signal<void(string_type&)> json_string_invoked;
};

//======================================================

inline bool operator == (value_reference<doc_parser::string_type> const& str1,
                         value_reference<doc_parser::string_type> const& str2)
{
   return str1.value() == str2.value();
}

inline bool operator != (value_reference<doc_parser::string_type> const& str1,
                         value_reference<doc_parser::string_type> const& str2)
{
   return !(str1 == str2);
}

inline bool operator == (value_reference<doc_parser::string_type> const& str1,
                         value_reference<doc_parser::string_type>::value_type const& str2)
{
   return str1.value() == str2;
}

inline bool operator != (value_reference<doc_parser::string_type> const& str1,
                         value_reference<doc_parser::string_type>::value_type const& str2)
{
   return !(str1 == str2);
}

inline bool operator == (value_reference<doc_parser::string_type>::value_type const& str1,
                         value_reference<doc_parser::string_type> const& str2)
{
   return str1 == str2.value();
}

inline bool operator != (value_reference<doc_parser::string_type>::value_type const& str1,
                         value_reference<doc_parser::string_type> const& str2)
{
   return !(str1 == str2);
}

inline bool operator == (cchar* str1, value_reference<doc_parser::string_type> const& str2) noexcept
{
   return str1 == str2.value();
}

inline bool operator != (cchar* str1, value_reference<doc_parser::string_type> const& str2) noexcept
{
   return !(str1 == str2);
}

inline bool operator == (value_reference<doc_parser::string_type> const& str1, cchar* str2) noexcept
{
   return str1.value() == str2;
}

inline bool operator != (value_reference<doc_parser::string_type> const& str1, cchar* str2) noexcept
{
   return !(str1 == str2);
}

//======================================================

inline
value_reference<doc_parser::string_type>::value_type
operator + (value_reference<doc_parser::string_type> const& ref,
            value_reference<doc_parser::string_type>::value_type const& str)
{
    typedef value_reference<doc_parser::string_type>::value_type value_type;

    return value_type(ref.value() + str);
}

inline
value_reference<doc_parser::string_type>::value_type
operator + (value_reference<doc_parser::string_type> const& ref,
            value_reference<doc_parser::string_type>::value_type::value_type str)
{
    typedef value_reference<doc_parser::string_type>::value_type value_type;

    return value_type(ref.value() + str);
}

inline
value_reference<doc_parser::string_type>::value_type
operator + (value_reference<doc_parser::string_type> const& ref,
            value_reference<doc_parser::string_type>::value_type::value_type const* str)
{
    typedef value_reference<doc_parser::string_type>::value_type value_type;

    return value_type(ref.value() + str);
}

inline
value_reference<doc_parser::string_type>::value_type
operator + (value_reference<doc_parser::string_type>::value_type const& str,
            value_reference<doc_parser::string_type> const& ref)
{
    typedef value_reference<doc_parser::string_type>::value_type value_type;

    return value_type(str + ref.c_str());
}

inline
value_reference<doc_parser::string_type>::value_type
operator + (value_reference<doc_parser::string_type>::value_type::value_type str,
            value_reference<doc_parser::string_type> const& ref)
{
    typedef value_reference<doc_parser::string_type>::value_type value_type;

    return value_type(str + ref.value());
}

inline
value_reference<doc_parser::string_type>::value_type
operator + (value_reference<doc_parser::string_type>::value_type::value_type const* str,
            value_reference<doc_parser::string_type> const& ref)
{
    typedef value_reference<doc_parser::string_type>::value_type value_type;

    return value_type(str + ref.value());
}

// ======================================================================

template <typename T, typename Allocator>
class SHARED_API objects_array<T, Allocator> : public template_array, public Allocator
{
public:
    typedef T                                           value_type            ;
    typedef std::allocator_traits<Allocator>            allocator_traits      ;
    typedef typename allocator_traits::allocator_type   allocator_type        ;
    typedef std::deque<value_type, allocator_type>      array_type            ;
    typedef doc_parser::size_type                       size_type             ;
    typedef doc_parser::size_type const                 const_size            ;
    typedef typename array_type::difference_type        difference_type       ;
    typedef typename array_type::reference              reference             ;
    typedef typename array_type::const_reference        const_reference       ;
    typedef typename array_type::iterator               iterator              ;
    typedef typename array_type::const_iterator         const_iterator        ;
    typedef typename array_type::reverse_iterator       reverse_iterator      ;
    typedef typename array_type::const_reverse_iterator const_reverse_iterator;

    objects_array (doc_parser&           prsr,
                   string_type    const& category,
                   allocator_type const& ator = allocator_type())
    : template_array (prsr, category),
      allocator_type (ator)
    {
        for (size_type i = 0; i < _M_ref->Size (); ++i) _M_values.emplace_back(this, i);

        connections ();
    }

    objects_array (template_object*      owner,
                   string_type    const& category,
                   allocator_type const& ator = allocator_type())
    : template_array (owner, category),
      allocator_type (ator)
    {
        for (size_type i = 0; i < _M_ref->Size (); ++i) _M_values.emplace_back (this, i);

        connections ();
    }

    objects_array (template_array*       owner,
                   size_type             id,
                   allocator_type const& ator = allocator_type())
    : template_array (owner, id),
      allocator_type (ator)
    {
        for (size_type i = 0; i < _M_ref->Size (); ++i) _M_values.emplace_back (this, i);

        connections ();
    }

    inline ~objects_array ()
    {
        disconnections ();
        destroyed ();
        destroyed.clear ();
    }

    inline iterator begin ()
    {
        return _M_values.begin();
    }

    inline const_iterator begin () const
    {
        return _M_values.begin();
    }

    inline const_iterator cbegin () const
    {
        return _M_values.cbegin();
    }

    inline reverse_iterator rbegin ()
    {
        return _M_values.rbegin();
    }

    inline const_reverse_iterator rbegin () const
    {
        return _M_values.rbegin();
    }

    inline const_reverse_iterator crbegin () const
    {
        return _M_values.crbegin();
    }

    inline iterator end ()
    {
        return _M_values.end();
    }

    inline const_iterator end () const
    {
        return _M_values.end();
    }

    inline const_iterator cend () const
    {
        return _M_values.cend();
    }

    inline reverse_iterator rend ()
    {
        return _M_values.rend();
    }

    inline const_reverse_iterator rend () const
    {
        return _M_values.rend();
    }

    inline const_reverse_iterator crend () const
    {
        return _M_values.crend();
    }

    constexpr allocator_type get_allocator () const noexcept
    {
        return *this;
    }

    inline const_reference operator [] (size_type i) const
    {
        assert(_M_values.size() > i);
        return _M_values[i];
    }

    inline reference operator [] (size_type i)
    {
        assert(_M_values.size() > i);
        return _M_values[i];
    }

    bool empty () const
    {
        return _M_values.empty ();
    }

    size_type size () const
    {
        return static_cast<size_type> (_M_values.size ());
    }

    void append ()
    {
        assert(_M_ref != nullptr && _M_parser != nullptr);

        about_to_append ();
        _M_ref->PushBack (template_array::value_type(kNullType), _M_parser->GetAllocator());
        _M_values.emplace_back (this, _M_ref->Size () - 1);
        appended ();
    }

    void remove (size_type idx)
    {
        assert(_M_ref != nullptr);
        assert(_M_ref->Size() > idx);

        auto it = std::find_if (_M_values.cbegin (), _M_values.cend (), [this, idx](const_reference elem)
        {
            return elem._M_ref == (*this)[idx]._M_ref;
        });

        if (it == _M_values.cend ()) return;

        about_to_remove (idx);
        _M_values.erase (it);
        _M_ref->Erase (&_M_ref->operator [] (idx));
        removed (idx);
    }

    void clear ()
    {
        if (empty ()) return;

        assert(_M_ref != nullptr);

        about_to_clear ();
        _M_values.clear ();
        _M_ref->Clear ();
        cleared ();
    }

private:
    void on_changed ()
    {
        if(_M_ref == nullptr)
        {
            disconnections();
            _M_values.clear();
        }
        else
        {
            _M_values.clear();
            for (size_type i = 0; i < _M_ref->Size (); ++i) _M_values.emplace_back (this, i);
        }
    }

    void connections ()
    {
        connect(changed, *this, &objects_array::on_changed);
        connect(destroyed, *this, &objects_array::clear);
    }

    void disconnections ()
    {
        disconnect(changed, *this, &objects_array::on_changed);
        disconnect(destroyed, *this, &objects_array::clear);
    }

private:
    array_type _M_values;
};

// ======================================================================

template <typename T, typename Allocator>
class SHARED_API values_array<T, Allocator> : public template_array, public Allocator
{
public:
    typedef value_reference<T>                          value_type            ;
    typedef std::allocator_traits<Allocator>            allocator_traits      ;
    typedef typename allocator_traits::allocator_type   allocator_type        ;
    typedef std::deque<value_type, allocator_type>      array_type            ;
    typedef doc_parser::size_type                       size_type             ;
    typedef doc_parser::size_type const                 const_size            ;
    typedef typename array_type::difference_type        difference_type       ;
    typedef typename array_type::reference              reference             ;
    typedef typename array_type::const_reference        const_reference       ;
    typedef typename array_type::iterator               iterator              ;
    typedef typename array_type::const_iterator         const_iterator        ;
    typedef typename array_type::reverse_iterator       reverse_iterator      ;
    typedef typename array_type::const_reverse_iterator const_reverse_iterator;

    values_array (doc_parser&           prsr,
                  string_type    const& category,
                  allocator_type const& ator = allocator_type())
    : template_array (prsr, category),
      allocator_type (ator)
    {
        for (size_type i = 0; i < _M_ref->Size (); ++i) _M_values.emplace_back (this, i);

        connections ();
    }

    values_array (template_object*      owner,
                  string_type    const& category,
                  allocator_type const& ator = allocator_type())
    : template_array (owner, category),
      allocator_type (ator)
    {
        for (size_type i = 0; i < _M_ref->Size (); ++i) _M_values.emplace_back (this, i);

        connections ();
    }

    inline ~values_array ()
    {
        disconnections ();
        destroyed ();
        destroyed.clear ();
    }

    inline iterator begin ()
    {
        return _M_values.begin();
    }

    inline const_iterator begin () const
    {
        return _M_values.begin();
    }

    inline const_iterator cbegin () const
    {
        return _M_values.cbegin();
    }

    inline reverse_iterator rbegin ()
    {
        return _M_values.rbegin();
    }

    inline const_reverse_iterator rbegin () const
    {
        return _M_values.rbegin();
    }

    inline const_reverse_iterator crbegin () const
    {
        return _M_values.crbegin();
    }

    inline iterator end ()
    {
        return _M_values.end();
    }

    inline const_iterator end () const
    {
        return _M_values.end();
    }

    inline const_iterator cend () const
    {
        return _M_values.cend();
    }

    inline reverse_iterator rend ()
    {
        return _M_values.rend();
    }

    inline const_reverse_iterator rend () const
    {
        return _M_values.rend();
    }

    inline const_reverse_iterator crend () const
    {
        return _M_values.crend();
    }

    constexpr allocator_type get_allocator () const noexcept
    {
        return *this;
    }

    inline const_reference operator [] (size_type i) const
    {
        assert(size() > i);
        return _M_values[i];
    }

    inline reference operator [] (size_type i)
    {
        assert(size() > i);
        return _M_values[i];
    }

    bool empty () const
    {
        return _M_values.empty ();
    }

    size_type size () const
    {
        return static_cast<size_type> (_M_values.size ());
    }

    void append ()
    {
        assert(_M_ref != nullptr && _M_parser != nullptr);

        about_to_append ();
        _M_ref->PushBack (template_array::value_type(kNullType), _M_parser->GetAllocator());
        _M_values.emplace_back (this, _M_ref->Size () - 1);
        appended ();
    }

    void remove (size_type idx)
    {
        assert(_M_ref != nullptr);
        assert(_M_ref->Size() > idx);

        auto it = std::find_if (_M_values.cbegin (), _M_values.cend (), [this, idx](const_reference elem)
        {
            return elem._M_ref == (*this)[idx]._M_ref;
        });

        if (it == _M_values.cend ()) return;

        about_to_remove (idx);
        _M_values.erase (it);
        _M_ref->Erase (&_M_ref->operator [] (idx));
        removed (idx);
    }

    void clear ()
    {
        if (empty ()) return;

        assert(_M_ref != nullptr);

        about_to_clear ();
        _M_values.clear ();
        _M_ref->Clear ();
        cleared ();
    }

private:
    void on_changed()
    {
        if(_M_ref == nullptr)
        {
            disconnections();
            _M_values.clear();
        }
        else
        {
            _M_values.clear();
            for (size_type i = 0; i < _M_ref->Size (); ++i) _M_values.emplace_back (this, i);
        }
    }

    void connections ()
    {
        connect(changed, *this, &values_array::on_changed);
        connect(destroyed, *this, &values_array::clear);
    }

    void disconnections ()
    {
        disconnect(changed, *this, &values_array::on_changed);
        disconnect(destroyed, *this, &values_array::clear);
    }

private:
    array_type _M_values;
};

// ======================================================================

class SHARED_API factory
{
public:
    template<typename T>
    using shared_ptr = std::shared_ptr<T>;

    typedef doc_parser::string_type                                  string_type        ;
    typedef doc_parser::size_type                                    size_type          ;
    typedef shared_ptr<doc_parser>                                   json_ptr           ;
    typedef std::pair<string_type, json_ptr>                         json_pair          ;
    typedef std::tuple<size_type, string_type, doc_parser::doc_type> file_tuple         ;
    typedef shared_ptr<template_object>                              generic_object_ptr ;
    typedef shared_ptr<template_array>                               generic_array_ptr  ;
    typedef std::pair<size_type, generic_object_ptr>                 generic_object_pair;
    typedef std::pair<size_type, generic_array_ptr>                  generic_array_pair ;
    typedef unordered_map<size_type, json_pair>                      json_map           ;
    typedef unordered_map<size_type, generic_object_ptr>             objects_container  ;
    typedef unordered_map<size_type, generic_array_ptr>              arrays_container   ;
    typedef unordered_map<size_type, objects_container>              objects_map        ;
    typedef unordered_map<size_type, arrays_container>               arrays_map         ;
    typedef json_map::iterator                                       iterator           ;
    typedef json_map::const_iterator                                 const_iterator     ;
    typedef json_map::mapped_type                                    value_type         ;
    typedef json_map::mapped_type&                                   reference          ;
    typedef json_map::mapped_type const&                             const_reference    ;

    // ======================================================================

    //! example doc key
    enum doc_key
    {
        general,
        common,
        settings,
        devices
    };

    // ======================================================================

    template<typename T, typename... Args>
    shared_ptr<T> make_object(size_type type, size_type key, Args&&... args)
    {
        auto json = _M_jsonDocs.find(type);

        if (json != _M_jsonDocs.end())
        {
            auto it_type = _M_objects.find(type);

            if (it_type == _M_objects.end())
            {
                _M_objects.emplace(type, objects_container());
            }

            auto it_key = _M_objects[type].find(key);

            if (it_key == _M_objects[type].end())
            {
                _M_objects[type].emplace(key,
                                         memory::allocate_shared<T>(memory::allocator<T>(),
                                                                    *json->second.second.get(),
                                                                    std::forward<Args>(args)...));
            }

            return std::dynamic_pointer_cast<T>(_M_objects[type][key]);
        }

        return shared_ptr<T>();
    }

    template<typename T, typename... Args>
    shared_ptr<T> make_array(size_type type, size_type key, Args&&... args)
    {
        auto json = _M_jsonDocs.find(type);

        if (json != _M_jsonDocs.end())
        {
            auto it_type = _M_arrays.find(type);

            if (it_type == _M_arrays.end())
            {
                _M_arrays.emplace(type, arrays_container());
            }

            auto it_key = _M_arrays[type].find(key);

            if (it_key == _M_arrays[type].end())
            {
                _M_arrays[type].emplace(key,
                                        memory::allocate_shared<T>(memory::allocator<T>(),
                                                                   *json->second.second.get(),
                                                                   std::forward<Args>(args)...));
            }

            return std::dynamic_pointer_cast<T>(_M_arrays[type][key]);
        }

        return shared_ptr<T>();
    }

    // ======================================================================

    factory() = delete;
    factory(std::initializer_list<file_tuple> json_files);

    void reset();
    void reset(size_type key);
    bool save (size_type key);
    void save ();

    generic_object_ptr get_object(size_type type, size_type key) const;
    generic_array_ptr  get_array (size_type type, size_type key) const;

    reference operator [] (size_type key) const
    {
        return _M_jsonDocs.at(key);
    }

    const_reference operator [] (size_type key)
    {
        return _M_jsonDocs.at(key);
    }

    const_iterator cbegin() const
    {
        return _M_jsonDocs.cbegin();
    }

    iterator begin()
    {
        return _M_jsonDocs.begin();
    }

    const_iterator cend() const
    {
        return _M_jsonDocs.cend();
    }

    iterator end()
    {
        return _M_jsonDocs.end();
    }

    string_type file_name_of_doc(size_type key) const
    {
        return (*this)[key].first;
    }

    signal<void()> about_to_reset;
    signal<void()> after_reset   ;

    signal<void(size_type)> about_to_reset_doc;
    signal<void(size_type)> after_reset_doc   ;

private:
    mutable json_map    _M_jsonDocs;
    mutable objects_map _M_objects ;
    mutable arrays_map  _M_arrays  ;
};

// ======================================================================

inline std::ostream& operator << (std::ostream& stream, value_reference<doc_parser::string_type> const& ref)
{
   return stream << ref.value();
}

} } // namespace Json


#endif // CPPUAL_JSON_ADAPTOR_H
