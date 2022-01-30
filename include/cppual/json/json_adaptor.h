#ifndef CPPUAL_JSON_ADAPTOR_H
#define CPPUAL_JSON_ADAPTOR_H

#define RAPIDJSON_HAS_STDSTRING 1

#include <cppual/memory/allocator.h>
#include <cppual/functional.h>
#include <cppual/signal.h>
#include <cppual/meta.h>
#include <cppual/string.h>
#include <cppual/containers.h>

#include <cppual/json/document.h>

#include <iostream>
#include <fstream>
#include <utility>
#include <cassert>
#include <vector>
#include <deque>
#include <tuple>
#include <unordered_map>

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
class Reference;

class TemplateObject;
class TemplateArray ;

template <typename T,
          typename A = memory::allocator<Reference<T>>,
          typename   = typename std::enable_if<is_json_value<T>::value>::type
          >
class ValuesArray;

template <typename T,
          typename A = memory::allocator<T>,
          typename   = typename std::enable_if<std::is_base_of<TemplateObject, T>::value ||
                                               std::is_base_of<TemplateArray , T>::value>::type
          >
class ObjectsArray;

//======================================================

class SHARED_API Parser : protected Document
{
public:
    typedef Document                           base_type      ;
    typedef SizeType                           size_type      ;
    typedef std::string                        string_type    ;
    typedef Value                              value_type     ;
    typedef value_type*                        pointer        ;
    typedef value_type&                        reference      ;
    typedef value_type const&                  const_reference;
    typedef base_type::ValueIterator           iterator       ;
    typedef base_type::ConstValueIterator      const_iterator ;
    typedef base_type::Object                  object_type    ;
    typedef base_type::Array                   array_type     ;
    typedef std::pair<string_type, value_type> pair_type      ;

    enum class Type : u8
    {
        Null,
        Object,
        Array
    };

    //! construct from file
    Parser(string_type const& file, Type type = Type::Object);

    //! construct from memory
    Parser(std::istream& data, Type type = Type::Object);

    //! recreate document from file
    bool createDocument(string_type const& file, Type type = Type::Object);

    bool save(std::ostream& stream);

    Type      type() const;
    size_type size() const;

    const_reference operator [] (string_type const& key) const;
    reference       operator [] (string_type const& key);
    const_reference operator [] (size_type idx) const;
    reference       operator [] (size_type idx);

    signal<void()> changed;
    signal<void()> aboutToSave;

    friend class TemplateObject;
    friend class TemplateArray ;

    template <typename T>
    friend class ReferenceBase;

    template <typename T, typename>
    friend class Reference;

private:
    Type _M_type;
};

// ======================================================================

class SHARED_API TemplateOwner
{
public:
    constexpr TemplateOwner() noexcept
    : _M_owner(),
      _M_type ()
    { }

    constexpr TemplateOwner(TemplateObject& obj)
    : _M_owner(&obj), _M_type(Parser::Type::Object)
    {
        assert(_M_owner != nullptr);
    }

    constexpr TemplateOwner(TemplateArray& array)
    : _M_owner(&array), _M_type(Parser::Type::Array)
    {
        assert(_M_owner != nullptr);
    }

    constexpr TemplateObject* object() const
    {
        assert(_M_owner != nullptr);
        assert(_M_type  == Parser::Type::Object);
        return static_cast<TemplateObject*>(_M_owner);
    }

    constexpr TemplateArray* array() const
    {
        assert(_M_owner != nullptr);
        assert(_M_type  == Parser::Type::Array);
        return static_cast<TemplateArray*>(_M_owner);
    }

    constexpr explicit operator bool () const noexcept
    {
        return _M_owner != nullptr && (_M_type == Parser::Type::Object || _M_type == Parser::Type::Array);
    }

    constexpr bool operator == (std::nullptr_t) const noexcept
    {
        return _M_owner == nullptr;
    }

    constexpr bool operator != (std::nullptr_t) const noexcept
    {
        return _M_owner != nullptr;
    }

    constexpr Parser::Type type () const noexcept
    {
        return _M_type;
    }

private:
    void*        _M_owner;
    Parser::Type _M_type ;
};

// ======================================================================

class SHARED_API TemplateObject
{
public:
    typedef Parser::size_type                          size_type      ;
    typedef Parser::value_type                         value_type     ;
    typedef Parser::reference                          reference      ;
    typedef Parser::pointer                            pointer        ;
    typedef Parser::const_reference                    const_reference;
    typedef Parser::ValueIterator                      iterator       ;
    typedef Parser::ConstValueIterator                 const_iterator ;
    typedef Parser::string_type                        string_type    ;
    typedef std::pair<string_type, Parser::value_type> pair_type      ;

    static constexpr auto npos = static_cast<size_type>(-1);

    TemplateObject () = delete;
    TemplateObject (TemplateObject&&);
    TemplateObject (TemplateObject const&);
    TemplateObject (Parser& _parser, string_type const& _category);
    TemplateObject (Parser& _parser, size_type _index);
    TemplateObject (TemplateObject* _owner, string_type const& _category);
    TemplateObject (TemplateArray* _owner, size_type _idx);

    TemplateObject& operator = (TemplateObject const&);
    TemplateObject& operator = (TemplateObject&&);

    virtual ~TemplateObject();

    const_reference operator [] (string_type const& key) const;
    reference       operator [] (string_type const& key);

    size_type size () const;
    bool      empty() const;

    constexpr pointer operator -> () const
    {
        assert(_M_ref != nullptr && _M_ref->IsObject());
        return _M_ref;
    }

    constexpr pointer ref () const noexcept
    {
        return _M_ref;
    }

    inline Parser* parser () const noexcept
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

    constexpr static Parser::Type type () noexcept
    {
        return Parser::Type::Object;
    }

    constexpr explicit operator bool () const noexcept
    {
        return _M_ref != nullptr;
    }

    void onParserDataChanged();
    void onAboutToSave();
    void onObjectChanged();
    void onArrayChanged();
    void onRemoved(size_type idx);

    signal<void()> aboutToSave;
    signal<void()> changed;
    signal<void()> invalidated;

private:
    pointer parentObjectRef() const;
    pointer parentArrayRef () const;
    void    connections();
    void    disconnections();
    void    invalidate();
    void    assignFromParserObject();
    void    assignFromParserArray();
    void    assignFromObject();
    void    assignFromArray();

private:
    TemplateOwner _M_owner   ;
    Parser*       _M_parser  ;
    pointer       _M_ref     ;
    string_type   _M_category;
    size_type     _M_index   ;

    template <typename>
    friend class ReferenceBase;

    template <typename, typename, typename>
    friend class ValuesArray;

    template <typename, typename, typename>
    friend class ObjectsArray;

    friend class TemplateArray;
};

//======================================================

class SHARED_API TemplateArray
{
public:
    typedef Parser::size_type                          size_type      ;
    typedef Parser::value_type                         value_type     ;
    typedef Parser::reference                          reference      ;
    typedef Parser::pointer                            pointer        ;
    typedef Parser::const_reference                    const_reference;
    typedef Parser::iterator                           iterator       ;
    typedef Parser::const_iterator                     const_iterator ;
    typedef Parser::string_type                        string_type    ;
    typedef std::pair<string_type, Parser::value_type> pair_type      ;

    static constexpr auto npos = static_cast<size_type>(-1);

    TemplateArray () = delete;
    TemplateArray (TemplateArray&&);
    TemplateArray (TemplateArray const&);
    TemplateArray (Parser& _parser, string_type const& _category);
    TemplateArray (Parser& _parser, size_type _index);
    TemplateArray (TemplateObject* _owner, string_type const& _category);
    TemplateArray (TemplateArray* _owner, size_type _idx);

    TemplateArray& operator = (TemplateArray const&);
    TemplateArray& operator = (TemplateArray&&);

    virtual ~TemplateArray ();

    const_reference operator [] (size_type idx) const;
    reference       operator [] (size_type idx);

    size_type size   () const;
    bool      empty  () const;
    void      append ();
    void      remove (size_type idx);
    void      clear  ();

    constexpr pointer operator -> () const
    {
        assert(_M_ref != nullptr && _M_ref->IsArray());
        return _M_ref;
    }

    constexpr pointer ref () const noexcept
    {
        return _M_ref;
    }

    constexpr Parser* parser () const noexcept
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

    constexpr static Parser::Type type () noexcept
    {
        return Parser::Type::Array;
    }

    constexpr explicit operator bool () const noexcept
    {
        return _M_ref != nullptr;
    }

    void onParserDataChanged();
    void onAboutToSave();
    void onObjectChanged();
    void onArrayChanged();
    void onRemoved(size_type idx);

    signal<void()>          aboutToSave;
    signal<void()>          aboutToAppend;
    signal<void()>          appended;
    signal<void(size_type)> aboutToRemove;
    signal<void(size_type)> removed;
    signal<void()>          aboutToClear;
    signal<void()>          cleared;
    signal<void()>          changed;
    signal<void()>          invalidated;

private:
    pointer parentObjectRef() const;
    pointer parentArrayRef () const;
    void    connections();
    void    disconnections();
    void    invalidate();
    void    assignFromParserObject();
    void    assignFromParserArray();
    void    assignFromObject();
    void    assignFromArray();

private:
    TemplateOwner _M_owner   ;
    Parser*       _M_parser  ;
    pointer       _M_ref     ;
    string_type   _M_category;
    size_type     _M_index   ;

    template <typename>
    friend class ReferenceBase;

    template <typename, typename, typename>
    friend class ValuesArray;

    template <typename, typename, typename>
    friend class ObjectsArray;

    friend class TemplateObject;
};

// ======================================================================

template <typename T>
class ReferenceBase
{
public:
    static_assert (is_json_value<T>::value, "invalid type!");

    typedef T                          base_type;
    typedef ReferenceBase<T>           self_type;
    typedef function<void(self_type*)> func_type;

    typedef typename std::conditional<std::is_same<i16, T>::value ||
                                      std::is_enum<T>::value,
            int,
            typename std::conditional<std::is_same<u16, T>::value, uint, T>::type
            >::type
            default_type;

    typedef base_type           value_type ;
    typedef Parser::pointer     pointer    ;
    typedef Parser::size_type   size_type  ;
    typedef Parser::value_type  json_type  ;
    typedef Parser::string_type string_type;

    static constexpr auto npos = static_cast<size_type>(-1);

    inline explicit operator bool () const noexcept
    {
        return _M_owner.type() != Parser::Type::Null &&
               _M_ref != nullptr && !_M_ref->IsNull();
    }

    inline Type type() const
    {
        return ref()->GetType();
    }

    ReferenceBase () = delete;
    ReferenceBase (ReferenceBase&&) = delete;
    ReferenceBase& operator = (ReferenceBase&&) = delete;

    inline ReferenceBase (ReferenceBase const& obj) noexcept
    : _M_ref(obj._M_ref),
      _M_owner(obj._M_owner),
      _M_fn(obj._M_fn)
    {
        connections();
    }

    virtual ~ReferenceBase()
    {
        disconnections();
    }

    inline ReferenceBase& operator = (ReferenceBase const& obj) noexcept
    {
        if (this != &obj && _M_ref != obj._M_ref)
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

    constexpr TemplateOwner owner() const noexcept
    {
        return _M_owner;
    }

    signal<void()> changed;

protected:
    inline ReferenceBase (TemplateObject*     owner,
                          string_type  const& key,
                          default_type const& default_val = default_type ())
    : _M_ref(),
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

    inline ReferenceBase (TemplateArray*      owner,
                          size_type           idx,
                          default_type const& default_val = default_type ())
    : _M_ref(),
      _M_owner(*owner)
    {
        _M_fn = func_type([idx, def = default_val](self_type* ptr)
        {
            auto new_idx = idx;

            if (ptr->_M_owner.array()->size() <= idx)
            {
                std::cout << "Json::" << __func__ << ": invalid reference index "
                          << idx      << " with default value: "
                          << def      << ". Trying to create a new one." << std::endl;

                //! try to obtain a valid reference
                (*ptr->_M_owner.array())->PushBack(json_type(def).Move(),
                                                   ptr->_M_owner.array()->parser()->GetAllocator());

                new_idx = (*ptr->_M_owner.array()).size() - 1;
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

    inline pointer parentObjectRef() const
    {
        return _M_owner.object()->_M_ref;
    }

    inline pointer parentArrayRef() const
    {
        return _M_owner.array()->_M_ref;
    }

private:
    void connections()
    {
        if (_M_owner.type() == Parser::Type::Object)
        {
            connect(_M_owner.object()->changed, *this, &ReferenceBase::assign);
        }
        else if (_M_owner.type() == Parser::Type::Array)
        {
            connect(_M_owner.array()->changed, *this, &ReferenceBase::assign);
        }
    }

    void disconnections()
    {
        if (_M_owner.type() == Parser::Type::Object)
        {
            disconnect(_M_owner.object()->changed, *this, &ReferenceBase::assign);
        }
        else if (_M_owner.type() == Parser::Type::Array)
        {
            disconnect(_M_owner.array()->changed, *this, &ReferenceBase::assign);
        }
    }

    void assign()
    {
        if (_M_fn != nullptr) _M_fn(this);
        changed();
    }

private:
    pointer       _M_ref   ;
    TemplateOwner _M_owner ;
    func_type     _M_fn    ;

    friend class Reference<T>;

    template <typename, typename, typename>
    friend class ValuesArray;
};

// ======================================================================

template <>
class SHARED_API ReferenceBase<Parser::string_type>
{
public:
    typedef ReferenceBase<Parser::string_type> self_type  ;
    typedef Parser::pointer                    pointer    ;
    typedef Parser::size_type                  size_type  ;
    typedef Parser::string_type                value_type ;
    typedef Parser::value_type                 json_type  ;
    typedef Parser::string_type                string_type;
    typedef function<void(self_type*)>         func_type  ;

    static constexpr auto npos = static_cast<size_type>(-1);

    inline explicit operator bool () const noexcept
    {
        return _M_owner.type() != Parser::Type::Null &&
               _M_ref != nullptr && !_M_ref->IsNull();
    }

    inline Type type() const
    {
        return ref()->GetType();
    }

    ReferenceBase () = delete;
    ReferenceBase (ReferenceBase&&) = delete;
    ReferenceBase& operator = (ReferenceBase&&) = delete;

    inline ReferenceBase (ReferenceBase const& obj) noexcept
    : _M_ref(obj._M_ref),
      _M_owner(obj._M_owner),
      _M_fn(obj._M_fn)
    {
        connections();
    }

    virtual ~ReferenceBase()
    {
        disconnections();
    }

    inline ReferenceBase& operator = (ReferenceBase const& obj) noexcept
    {
        if (this != &obj && _M_ref != obj._M_ref)
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

    constexpr TemplateOwner owner() const noexcept
    {
        return _M_owner;
    }

    signal<void()> changed;

protected:
    ReferenceBase (TemplateObject*    owner,
                   string_type const& key,
                   value_type  const& default_val = value_type ());

    ReferenceBase (TemplateArray*    owner,
                   size_type         idx,
                   value_type const& default_val = value_type ());

    inline pointer parentObjectRef() const
    {
        return _M_owner.object()->_M_ref;
    }

    inline pointer parentArrayRef() const
    {
        return _M_owner.array()->_M_ref;
    }

private:
    void connections();
    void disconnections();
    void assign();

private:
    pointer       _M_ref   ;
    TemplateOwner _M_owner ;
    func_type     _M_fn    ;

    friend class Reference<value_type>;

    template <typename, typename, typename>
    friend class ValuesArray;
};

//======================================================

template <typename T>
class Reference<T> : public ReferenceBase<T>
{
public:
    static_assert (std::is_enum_v<T>, "T is NOT an enum!");

    typedef ReferenceBase<T>                      reference_base;
    typedef Parser::size_type                     size_type     ;
    typedef typename reference_base::json_type    json_type     ;
    typedef typename reference_base::string_type  string_type   ;
    typedef typename reference_base::value_type   value_type    ;
    typedef typename reference_base::default_type default_type  ;

    inline Reference (TemplateObject*     owner,
                      string_type  const& name,
                      default_type const& default_val = default_type())
    : reference_base(owner, name, default_val)
    { }

    inline Reference (TemplateArray*      owner,
                      size_type           idx,
                      default_type const& default_val = default_type())
    : reference_base(owner, idx, default_val)
    { }

    inline Reference (Reference const& obj)
    : reference_base(obj)
    { }

    inline operator value_type () const noexcept
    {
        return static_cast<value_type>(this->ref()->GetInt());
    }

    inline auto value () const noexcept
    {
        return static_cast<std::underlying_type_t<value_type>>(this->ref()->GetInt());
    }

    inline Reference& operator = (Reference const& ref)
    {
        reference_base::operator = (ref);
        return *this;
    }

    inline Reference& operator = (value_type value)
    {
        this->ref()->SetInt(static_cast<int>(value));

        return *this;
    }
};

//======================================================

template <>
class SHARED_API Reference<bool> : public ReferenceBase<bool>
{
public:
    typedef ReferenceBase<bool>                  reference_base;
    typedef typename reference_base::json_type   json_type     ;
    typedef typename reference_base::string_type string_type   ;
    typedef typename reference_base::value_type  value_type    ;

    Reference (TemplateObject*    owner,
               string_type const& name,
               value_type  const& default_val = value_type());

    Reference (TemplateArray*    owner,
               size_type         idx,
               value_type const& default_val = value_type());

    inline Reference (Reference const& obj)
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

    inline Reference& operator = (Reference const& ref)
    {
        reference_base::operator = (ref);
        return *this;
    }

    inline Reference& operator = (value_type value)
    {
        ref()->SetBool(value);

        return *this;
    }
};

//======================================================

template <>
class SHARED_API Reference<u16> : public ReferenceBase<u16>
{
public:
    typedef ReferenceBase<u16>                    reference_base;
    typedef typename reference_base::json_type    json_type     ;
    typedef typename reference_base::string_type  string_type   ;
    typedef typename reference_base::value_type   value_type    ;
    typedef typename reference_base::default_type default_type  ;

    Reference (TemplateObject*      owner,
               string_type   const& name,
               value_type           default_val = value_type());

    Reference (TemplateArray* owner,
               size_type      idx,
               value_type     default_val = value_type());

    inline Reference (Reference const& obj)
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

    inline Reference& operator = (Reference const& ref) noexcept
    {
        reference_base::operator = (ref);
        return *this;
    }

    inline Reference& operator = (value_type value)
    {
        ref()->SetUint(static_cast<uint>(value));

        return *this;
    }
};

//======================================================

template <>
class SHARED_API Reference<i16> : public ReferenceBase<i16>
{
public:
    typedef ReferenceBase<i16>                  reference_base;
    typedef typename reference_base::json_type    json_type     ;
    typedef typename reference_base::string_type  string_type   ;
    typedef typename reference_base::value_type   value_type    ;
    typedef typename reference_base::default_type default_type  ;

    Reference (TemplateObject*     owner,
               string_type  const& name,
               value_type          default_val = value_type());

    Reference (TemplateArray* owner,
               size_type      idx,
               value_type     default_val = value_type());

    inline Reference (Reference const& obj)
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

    inline Reference& operator = (Reference const& ref) noexcept
    {
        reference_base::operator = (ref);
        return *this;
    }

    inline Reference& operator = (value_type value)
    {
        ref()->SetInt(static_cast<int>(value));

        return *this;
    }
};

//======================================================

template <>
class SHARED_API Reference<uint> : public ReferenceBase<uint>
{
public:
    typedef ReferenceBase<uint>                  reference_base;
    typedef typename reference_base::json_type   json_type     ;
    typedef typename reference_base::string_type string_type   ;
    typedef typename reference_base::value_type  value_type    ;

    Reference (TemplateObject*    owner,
               string_type const& name,
               value_type         default_val = value_type());

    Reference (TemplateArray* owner,
               size_type      idx,
               value_type     default_val = value_type());

    inline Reference (Reference const& obj)
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

    inline Reference& operator = (Reference const& ref) noexcept
    {
        reference_base::operator = (ref);
        return *this;
    }

    inline Reference& operator = (value_type value)
    {
        ref()->SetUint(value);

        return *this;
    }
};

//======================================================

template <>
class SHARED_API Reference<int> : public ReferenceBase<int>
{
public:
    typedef ReferenceBase<int>                   reference_base;
    typedef typename reference_base::json_type   json_type     ;
    typedef typename reference_base::string_type string_type   ;
    typedef typename reference_base::value_type  value_type    ;

    Reference (TemplateObject*    owner,
               string_type const& name,
               value_type  const& default_val = value_type());

    Reference (TemplateArray* owner,
               size_type idx,
               value_type const& default_val = value_type());

    inline Reference (Reference const& obj)
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

    inline Reference& operator = (Reference const& ref) noexcept
    {
        reference_base::operator = (ref);
        return *this;
    }

    inline Reference& operator = (value_type value) noexcept
    {
        ref()->SetInt(value);

        return *this;
    }
};

//======================================================

template <>
class SHARED_API Reference<u64> : public ReferenceBase<u64>
{
public:
    typedef ReferenceBase<u64>                   reference_base;
    typedef typename reference_base::json_type   json_type     ;
    typedef typename reference_base::string_type string_type   ;
    typedef typename reference_base::value_type  value_type    ;

    Reference (TemplateObject*    owner,
               string_type const& name,
               value_type  const& default_val = value_type());

    Reference (TemplateArray*    owner,
               size_type         idx,
               value_type const& default_val = value_type());

    inline Reference (Reference const& obj)
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

    inline Reference& operator = (Reference const& ref) noexcept
    {
        reference_base::operator = (ref);
        return *this;
    }

    inline Reference& operator = (value_type const& value)
    {
        ref()->SetUint64(value);

        return *this;
    }
};

//======================================================

template <>
class SHARED_API Reference<i64> : public ReferenceBase<i64>
{
public:
    typedef ReferenceBase<i64>                 reference_base;
    typedef typename reference_base::json_type   json_type     ;
    typedef typename reference_base::string_type string_type   ;
    typedef typename reference_base::value_type  value_type    ;

    Reference (TemplateObject*    owner,
               string_type const& name,
               value_type  const& default_val = value_type());

    Reference (TemplateArray*    owner,
               size_type         idx,
               value_type const& default_val = value_type());

    inline Reference (Reference const& obj)
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

    inline Reference& operator = (Reference const& ref) noexcept
    {
        reference_base::operator = (ref);
        return *this;
    }

    inline Reference& operator = (value_type const& value)
    {
        ref()->SetInt64(value);

        return *this;
    }
};

//======================================================

template <>
class SHARED_API Reference<float> : public ReferenceBase<float>
{
public:
    typedef ReferenceBase<float>                 reference_base;
    typedef typename reference_base::json_type   json_type     ;
    typedef typename reference_base::string_type string_type   ;
    typedef typename reference_base::value_type  value_type    ;

    Reference (TemplateObject*    owner,
               string_type const& name,
               value_type  const& default_val = value_type());

    Reference (TemplateArray*    owner,
               size_type         idx,
               value_type const& default_val = value_type());

    inline Reference (Reference const& obj)
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

    inline Reference& operator = (Reference const& ref) noexcept
    {
        reference_base::operator = (ref);
        return *this;
    }

    inline Reference& operator = (value_type const& value)
    {
        ref()->SetFloat(value);

        return *this;
    }
};

//======================================================

template <>
class SHARED_API Reference<double> : public ReferenceBase<double>
{
public:
    typedef ReferenceBase<double>                reference_base;
    typedef typename reference_base::json_type   json_type     ;
    typedef typename reference_base::string_type string_type   ;
    typedef typename reference_base::value_type  value_type    ;

    Reference (TemplateObject*    owner,
               string_type const& name,
               value_type  const& default_val = value_type());

    Reference (TemplateArray*    owner,
               size_type         idx,
               value_type const& default_val = value_type());

    inline Reference (Reference const& obj)
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

    inline Reference& operator = (Reference const& ref) noexcept
    {
        reference_base::operator = (ref);
        return *this;
    }

    inline Reference& operator = (value_type const& value)
    {
        ref()->SetDouble(value);

        return *this;
    }
};

//======================================================

template <>
class SHARED_API Reference<Parser::string_type> : public ReferenceBase<Parser::string_type>
{
public:
    typedef ReferenceBase<Parser::string_type>   reference_base;
    typedef typename reference_base::json_type   json_type     ;
    typedef reference_base::string_type          string_type   ;
    typedef reference_base::value_type           value_type    ;

    Reference (TemplateObject*    owner,
               string_type const& name,
               value_type  const& default_val = value_type());

    Reference (TemplateArray*    owner,
               size_type         idx,
               value_type const& default_val = value_type());

    template <typename Processor,
              typename = typename std::enable_if<
                  std::is_same<typename std::result_of<Processor()>::type, value_type>::value,
                  typename std::result_of<Processor()>::type
                  >::type
              >
    inline Reference (TemplateObject*    owner,
                      string_type const& name,
                      Processor&&        fn,
                      value_type  const& default_val = value_type())
    : reference_base(owner, name, default_val)
    {
        if (!_M_ref->IsString()) throw std::runtime_error("Reference is NOT String");

        connect(jsonStringInvoked, [func = fn](string_type& val)
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
    inline Reference (TemplateObject*    owner,
                      string_type const& name,
                      Processor&&        fn,
                      value_type  const& default_val = value_type(),
                      E = E())
    : reference_base(owner, name, default_val)
    {
        if (!_M_ref->IsString()) throw std::runtime_error("Reference is NOT String");

        connect(jsonEnumInvoked, [func = fn](int& val)
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
    inline Reference (TemplateArray*    owner,
                      size_type         idx,
                      Processor&&       fn,
                      value_type const& default_val = value_type())
    : reference_base(owner, idx, default_val)
    {
        if (!_M_ref->IsString()) throw std::runtime_error("Reference is NOT String");

        connect(jsonStringInvoked, [func = fn](string_type& val)
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
    inline Reference (TemplateArray*    owner,
                      size_type         idx,
                      Processor&&       fn,
                      value_type const& default_val = value_type(),
                      E = E())
    : reference_base(owner, idx, default_val)
    {
        if (!_M_ref->IsString()) throw std::runtime_error("Reference is NOT String");

        connect(jsonEnumInvoked, [func = fn](int& val)
        {
            val = func ();
        });
    }

    inline Reference (Reference const& obj)
    : reference_base(obj),
      jsonEnumInvoked(obj.jsonEnumInvoked),
      jsonStringInvoked(obj.jsonStringInvoked)
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

        jsonStringInvoked(value);
        return value;
    }

    template <typename U,
              typename = typename std::enable_if<std::is_enum<U>::value>::type
              >
    inline U toEnum () const
    {
        int value = 0;

        jsonEnumInvoked(value);
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

    inline Reference& operator = (Reference const& rh) noexcept
    {
        if (this != &rh && ref() != rh.ref())
        {
            reference_base::operator = (rh);

            jsonEnumInvoked   = rh.jsonEnumInvoked;
            jsonStringInvoked = rh.jsonStringInvoked;
        }

        return *this;
    }

    inline Reference& operator = (value_type const& value)
    {
        switch (owner().type())
        {
        case Parser::Type::Object:
            ref()->SetString(value.c_str(),
                             static_cast<size_type>(value.size()),
                             owner().object()->parser()->GetAllocator());
            break;
        case Parser::Type::Array:
            ref()->SetString(value.c_str(),
                             static_cast<size_type>(value.size()),
                             owner().array()->parser()->GetAllocator());
            break;
        default:
            break;
        }

        return *this;
    }

    inline Reference& operator = (value_type::value_type const* value)
    {
        switch (owner().type())
        {
        case Parser::Type::Object:
            ref()->SetString(value, owner().object()->parser()->GetAllocator());
            break;
        case Parser::Type::Array:
            ref()->SetString(value, owner().array()->parser()->GetAllocator());
            break;
        default:
            break;
        }

        return *this;
    }

private:
    signal<void(int&)>         jsonEnumInvoked  ;
    signal<void(string_type&)> jsonStringInvoked;
};

//======================================================

inline bool operator == (Reference<Parser::string_type> const& str1,
                         Reference<Parser::string_type> const& str2)
{
   return str1.value() == str2.value();
}

inline bool operator != (Reference<Parser::string_type> const& str1,
                         Reference<Parser::string_type> const& str2)
{
   return !(str1 == str2);
}

inline bool operator == (Reference<Parser::string_type> const& str1,
                         Reference<Parser::string_type>::value_type const& str2)
{
   return str1.value() == str2;
}

inline bool operator != (Reference<Parser::string_type> const& str1,
                         Reference<Parser::string_type>::value_type const& str2)
{
   return !(str1 == str2);
}

inline bool operator == (Reference<Parser::string_type>::value_type const& str1,
                         Reference<Parser::string_type> const& str2)
{
   return str1 == str2.value();
}

inline bool operator != (Reference<Parser::string_type>::value_type const& str1,
                         Reference<Parser::string_type> const& str2)
{
   return !(str1 == str2);
}

inline bool operator == (cchar* str1, Reference<Parser::string_type> const& str2) noexcept
{
   return str1 == str2.value();
}

inline bool operator != (cchar* str1, Reference<Parser::string_type> const& str2) noexcept
{
   return !(str1 == str2);
}

inline bool operator == (Reference<Parser::string_type> const& str1, cchar* str2) noexcept
{
   return str1.value() == str2;
}

inline bool operator != (Reference<Parser::string_type> const& str1, cchar* str2) noexcept
{
   return !(str1 == str2);
}

//======================================================

inline
Reference<Parser::string_type>::value_type operator + (Reference<Parser::string_type> const& ref,
                                           Reference<Parser::string_type>::value_type const& str)
{
    typedef Reference<Parser::string_type>::value_type value_type;

    return value_type(ref.value() + str);
}

inline
Reference<Parser::string_type>::value_type operator + (Reference<Parser::string_type> const& ref,
                                                       Reference<Parser::string_type>::value_type::
                                                       value_type str)
{
    typedef Reference<Parser::string_type>::value_type value_type;

    return value_type(ref.value() + str);
}

inline
Reference<Parser::string_type>::value_type operator + (Reference<Parser::string_type> const& ref,
                                                       Reference<Parser::string_type>::value_type::
                                                       value_type const* str)
{
    typedef Reference<Parser::string_type>::value_type value_type;

    return value_type(ref.value() + str);
}

inline
Reference<Parser::string_type>::value_type operator + (Reference<Parser::string_type>::value_type const& str,
                                                       Reference<Parser::string_type> const& ref)
{
    typedef Reference<Parser::string_type>::value_type value_type;

    return value_type(str + ref.c_str());
}

inline
Reference<Parser::string_type>::value_type operator + (Reference<Parser::string_type>::value_type::
                                                       value_type str,
                                                       Reference<Parser::string_type> const& ref)
{
    typedef Reference<Parser::string_type>::value_type value_type;

    return value_type(str + ref.value());
}

inline
Reference<Parser::string_type>::value_type operator + (Reference<Parser::string_type>::value_type::
                                                       value_type const* str,
                                                       Reference<Parser::string_type> const& ref)
{
    typedef Reference<Parser::string_type>::value_type value_type;

    return value_type(str + ref.value());
}

// ======================================================================

template <typename T, typename Allocator>
class ObjectsArray<T, Allocator> : public TemplateArray, public Allocator
{
public:
    typedef T                                           value_type            ;
    typedef std::allocator_traits<Allocator>            allocator_traits      ;
    typedef typename allocator_traits::allocator_type   allocator_type        ;
    typedef std::deque<value_type, allocator_type>      array_type            ;
    typedef Parser::size_type                           size_type             ;
    typedef Parser::size_type const                     const_size            ;
    typedef typename array_type::difference_type        difference_type       ;
    typedef typename array_type::reference              reference             ;
    typedef typename array_type::const_reference        const_reference       ;
    typedef typename array_type::iterator               iterator              ;
    typedef typename array_type::const_iterator         const_iterator        ;
    typedef typename array_type::reverse_iterator       reverse_iterator      ;
    typedef typename array_type::const_reverse_iterator const_reverse_iterator;

    ObjectsArray (Parser&               prsr,
                  string_type const&    category,
                  allocator_type const& ator = allocator_type())
    : TemplateArray (prsr, category),
      allocator_type(ator)
    {
        for (size_type i = 0; i < size(); ++i) _M_values.emplace_back(this, i);

        connections ();
    }

    ObjectsArray (TemplateObject*       owner,
                  string_type const&    category,
                  allocator_type const& ator = allocator_type())
    : TemplateArray (owner, category),
      allocator_type(ator)
    {
        for (size_type i = 0; i < size(); ++i) _M_values.emplace_back(this, i);

        connections ();
    }

    ObjectsArray (TemplateArray*        owner,
                  size_type             id,
                  allocator_type const& ator = allocator_type())
    : TemplateArray (owner, id),
      allocator_type(ator)
    {
        for (size_type i = 0; i < size(); ++i) _M_values.emplace_back(this, i);

        connections ();
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

private:
    void onIndexAppended()
    {
        _M_values.emplace_back(this, size() - 1);
    }

    void onIndexRemoved(size_type idx)
    {
        auto it = std::find_if (_M_values.cbegin(), _M_values.cend(), [this, idx](const_reference elem)
        {
            return elem._M_ref == (*this)[idx]._M_ref;
        });

        _M_values.erase(it);
    }

    void onChanged()
    {
        if(!(*this))
        {
            disconnections();
            _M_values.clear();
        }
        else
        {
            _M_values.clear();
            for (size_type i = 0; i < size(); ++i) _M_values.emplace_back(this, i);
        }
    }

    void connections()
    {
        connect(appended, *this, &ObjectsArray::onIndexAppended);
        connect(aboutToRemove, *this, &ObjectsArray::onIndexRemoved);
        connect(aboutToClear, _M_values, &array_type::clear);
        connect(changed, *this, &ObjectsArray::onChanged);
    }

    void disconnections()
    {
        disconnect(appended, *this, &ObjectsArray::onIndexAppended);
        disconnect(aboutToRemove, *this, &ObjectsArray::onIndexRemoved);
        disconnect(aboutToClear, _M_values, &array_type::clear);
        disconnect(changed, *this, &ObjectsArray::onChanged);
    }

private:
    array_type _M_values;
};

// ======================================================================

template <typename T, typename Allocator>
class ValuesArray<T, Allocator> : public TemplateArray, public Allocator
{
public:
    typedef Reference<T>                                value_type            ;
    typedef std::allocator_traits<Allocator>            allocator_traits      ;
    typedef typename allocator_traits::allocator_type   allocator_type        ;
    typedef std::deque<value_type, allocator_type>      array_type            ;
    typedef Parser::size_type                           size_type             ;
    typedef Parser::size_type const                     const_size            ;
    typedef typename array_type::difference_type        difference_type       ;
    typedef typename array_type::reference              reference             ;
    typedef typename array_type::const_reference        const_reference       ;
    typedef typename array_type::iterator               iterator              ;
    typedef typename array_type::const_iterator         const_iterator        ;
    typedef typename array_type::reverse_iterator       reverse_iterator      ;
    typedef typename array_type::const_reverse_iterator const_reverse_iterator;

    ValuesArray (Parser&               prsr,
                 string_type const&    category,
                 allocator_type const& ator = allocator_type())
    : TemplateArray (prsr, category),
      allocator_type(ator)
    {
        for (size_type i = 0; i < size(); ++i) _M_values.emplace_back(this, i);

        connections ();
    }

    ValuesArray (TemplateObject*       owner,
                 string_type const&    category,
                 allocator_type const& ator = allocator_type())
    : TemplateArray (owner, category),
      allocator_type(ator)
    {
        for (size_type i = 0; i < size(); ++i) _M_values.emplace_back(this, i);

        connections ();
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

private:
    void onIndexAppended()
    {
        _M_values.emplace_back(this, size() - 1);
    }

    void onIndexRemoved(size_type idx)
    {
        auto it = std::find_if (_M_values.cbegin(), _M_values.cend(), [this, idx](const_reference elem)
        {
            return elem._M_ref == (*this)[idx]._M_ref;
        });

        _M_values.erase(it);
    }

    void onChanged()
    {
        if(!(*this))
        {
            disconnections();
            _M_values.clear();
        }
        else
        {
            _M_values.clear();
            for (size_type i = 0; i < size(); ++i) _M_values.emplace_back(this, i);
        }
    }

    void connections()
    {
        connect(appended, *this, &ValuesArray::onIndexAppended);
        connect(aboutToRemove , *this, &ValuesArray::onIndexRemoved);
        connect(aboutToClear, _M_values, &array_type::clear);
        connect(changed , *this, &ValuesArray::onChanged);
    }

    void disconnections()
    {
        disconnect(appended, *this, &ValuesArray::onIndexAppended);
        disconnect(aboutToRemove , *this, &ValuesArray::onIndexRemoved);
        disconnect(aboutToClear, _M_values, &array_type::clear);
        disconnect(changed , *this, &ValuesArray::onChanged);
    }

private:
    array_type _M_values;
};

// ======================================================================

class SHARED_API Factory
{
public:
    template<typename T>
    using shared_ptr = std::shared_ptr<T>;

    typedef Parser::string_type                              string_type        ;
    typedef Parser::size_type                                size_type          ;
    typedef shared_ptr<Parser>                               json_ptr           ;
    typedef std::pair<string_type, json_ptr>                 json_pair          ;
    typedef std::tuple<size_type, string_type, Parser::Type> file_tuple         ;
    typedef shared_ptr<TemplateObject>                       generic_object_ptr ;
    typedef shared_ptr<TemplateArray>                        generic_array_ptr  ;
    typedef std::pair<size_type, generic_object_ptr>         generic_object_pair;
    typedef std::pair<size_type, generic_array_ptr>          generic_array_pair ;
    typedef unordered_map<size_type, json_pair>              json_map           ;
    typedef unordered_map<size_type, generic_object_ptr>     objects_container  ;
    typedef unordered_map<size_type, generic_array_ptr>      arrays_container   ;
    typedef unordered_map<size_type, objects_container>      objects_map        ;
    typedef unordered_map<size_type, arrays_container>       arrays_map         ;
    typedef json_map::iterator                               iterator           ;
    typedef json_map::const_iterator                         const_iterator     ;
    typedef json_map::mapped_type                            value_type         ;
    typedef json_map::mapped_type&                           reference          ;
    typedef json_map::mapped_type const&                     const_reference    ;

    // ======================================================================

    //! example doc key
    enum Type
    {
        General,
        Common,
        Settings,
        Devices
    };

    // ======================================================================

    template<typename T, typename... Args>
    shared_ptr<T> createObject(size_type type, size_type key, Args&&... args)
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
    shared_ptr<T> createArray(size_type type, size_type key, Args&&... args)
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

    Factory() = delete;
    Factory(std::initializer_list<file_tuple> json_files);

    void reset();
    void reset(size_type key);
    bool save (size_type key);
    void save ();

    generic_object_ptr getObject(size_type type, size_type key) const;
    generic_array_ptr  getArray (size_type type, size_type key) const;

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

    string_type fileNameOfDocument(size_type key) const
    {
        return (*this)[key].first;
    }

    signal<void()> aboutToReset;
    signal<void()> afterReset  ;

    signal<void(size_type)> aboutToResetDoc;
    signal<void(size_type)> afterResetDoc  ;

private:
    mutable json_map    _M_jsonDocs;
    mutable objects_map _M_objects ;
    mutable arrays_map  _M_arrays  ;
};

// ======================================================================

inline std::ostream& operator << (std::ostream& stream, Reference<Parser::string_type> const& ref)
{
   return stream << ref.value();
}

} } // namespace Json


#endif // CPPUAL_JSON_ADAPTOR_H
