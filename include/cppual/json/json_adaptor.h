#ifndef CPPUAL_JSON_ADAPTOR_H
#define CPPUAL_JSON_ADAPTOR_H

#define RAPIDJSON_HAS_STDSTRING 1

#include <cppual/functional.h>
#include <cppual/signal.h>
#include <cppual/meta.h>
#include <cppual/string.h>

#include <cppual/json/document.h>

#include <iostream>
#include <fstream>
#include <utility>
#include <cassert>
#include <vector>
#include <deque>

class QString;

namespace cppual { namespace Json {

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
struct is_json_value<int16> : std::true_type
{ };

template <>
struct is_json_value<uint> : std::true_type
{ };

template <>
struct is_json_value<int> : std::true_type
{ };

template <>
struct is_json_value<u64> : std::true_type
{ };

template <>
struct is_json_value<int64> : std::true_type
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
struct is_json_int<uint> : std::true_type
{ };

template <>
struct is_json_int<int> : std::true_type
{ };

template <>
struct is_json_int<u16> : std::true_type
{ };

template <>
struct is_json_int<int16> : std::true_type
{ };

template <>
struct is_json_int<u64> : std::true_type
{ };

template <>
struct is_json_int<int64> : std::true_type
{ };

template <typename>
struct is_json_float : std:: false_type
{ };

template <>
struct is_json_float<float> : std::true_type
{ };

template <>
struct is_json_float<double> : std::true_type
{ };

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
class TemplateArray;

template <typename T, bool = std::is_base_of<TemplateObject, T>::value>
class ValueArray;

//======================================================

class Parser : protected rapidjson::Document
{
public:
    typedef rapidjson::Document                   base_type      ;
    typedef rapidjson::SizeType                   size_type      ;
    typedef std::string                           string_type    ;
    typedef rapidjson::Value                      value_type     ;
    typedef value_type*                           pointer        ;
    typedef value_type&                           reference      ;
    typedef value_type const&                     const_reference;
    typedef base_type::ValueIterator              iterator       ;
    typedef base_type::ConstValueIterator         const_iterator ;
    typedef base_type::Object                     object_type    ;
    typedef base_type::Array                      array_type     ;
    typedef std::pair<string_type, value_type>    pair_type      ;

    enum class Type
    {
        Null,
        Object,
        Array
    };

    //! construct from file
    Parser(string_type const& file, Type type = Type::Object);

    //! recreate document from file
    bool createDocument(string_type const& file, Type type = Type::Object);

    bool save(std::ofstream& stream);

    Type      type() const;
    size_type size() const;

    const_reference operator [] (string_type const& key) const;
    reference       operator [] (string_type const& key);
    const_reference operator [] (size_type idx) const;
    reference       operator [] (size_type idx);

    Signal<void()> changed;
    Signal<void()> aboutToSave;

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

class TemplateOwner
{
public:
    inline TemplateOwner() noexcept = default;

    inline TemplateOwner(TemplateObject& obj)
    : _M_owner(&obj), _M_type(Parser::Type::Object)
    {
        assert(_M_owner != nullptr);
    }

    inline TemplateOwner(TemplateArray& array)
    : _M_owner(&array), _M_type(Parser::Type::Array)
    {
        assert(_M_owner != nullptr);
    }

    inline TemplateObject* object() const
    {
        assert(_M_type == Parser::Type::Object);
        return static_cast<TemplateObject*>(_M_owner);
    }

    inline TemplateArray* array() const
    {
        assert(_M_type == Parser::Type::Array);
        return static_cast<TemplateArray*>(_M_owner);
    }

    constexpr explicit operator bool () const noexcept
    {
        return _M_owner != nullptr;
    }

    constexpr Parser::Type type () const noexcept
    {
        return _M_type;
    }

private:
    void*        _M_owner { };
    Parser::Type _M_type  { };
};

// ======================================================================

class TemplateObject
{
public:
    typedef Parser::size_type                               size_type      ;
    typedef Parser::value_type                              value_type     ;
    typedef Parser::reference                               reference      ;
    typedef Parser::pointer                                 pointer        ;
    typedef Parser::const_reference                         const_reference;
    typedef Parser::ValueIterator                           iterator       ;
    typedef Parser::ConstValueIterator                      const_iterator ;
    typedef Parser::string_type                             string_type    ;
    typedef std::pair<string_type, Parser::value_type>      pair_type      ;
    typedef Function<void()>                                function_type  ;
    typedef Function<void(size_type)>                       function_p_type;

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
        return _M_ref != nullptr && _M_parser != nullptr;
    }

    void onParserDataChanged();
    void onAboutToSave();
    void onObjectChanged();
    void onArrayChanged();
    void onRemoved(size_type idx);

    Signal<void()> aboutToSave;
    Signal<void()> changed;

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
    function_type _M_ownerfn ;
    string_type   _M_category;
    size_type     _M_index   ;

    template <typename>
    friend class ReferenceBase;

    template <typename, bool>
    friend class ValueArray;

    friend class TemplateArray;
};

//======================================================

class TemplateArray
{
public:
    typedef Parser::size_type                               size_type      ;
    typedef Parser::value_type                              value_type     ;
    typedef Parser::reference                               reference      ;
    typedef Parser::pointer                                 pointer        ;
    typedef Parser::const_reference                         const_reference;
    typedef Parser::iterator                                iterator       ;
    typedef Parser::const_iterator                          const_iterator ;
    typedef Parser::string_type                             string_type    ;
    typedef std::pair<string_type, Parser::value_type>      pair_type      ;
    typedef Function<void()>                                function_type  ;
    typedef Function<void(size_type)>                       function_p_type;

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

    size_type size () const;
    bool      empty() const;

    constexpr pointer operator -> () const
    {
        assert(_M_ref != nullptr && _M_ref->IsArray());
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
        return Parser::Type::Array;
    }

    constexpr explicit operator bool () const noexcept
    {
        return _M_ref != nullptr && _M_parser != nullptr;
    }

    inline void append ()
    {
        assert(_M_ref != nullptr);

        aboutToAppend();
        _M_ref->PushBack(value_type(rapidjson::kNullType), _M_parser->GetAllocator());
        appended();
    }

    inline void remove (size_type idx)
    {
        assert(_M_ref != nullptr);
        assert(_M_ref->Size() > idx);

        aboutToRemove(idx);
        _M_ref->Erase(&(*this)[idx]);
        removed(idx);
    }

    void onParserDataChanged();
    void onAboutToSave();
    void onObjectChanged();
    void onArrayChanged();
    void onRemoved(size_type idx);

    Signal<void()>          aboutToSave;
    Signal<void()>          aboutToAppend;
    Signal<void()>          appended;
    Signal<void(size_type)> aboutToRemove;
    Signal<void(size_type)> removed;
    Signal<void()>          changed;

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
    function_type _M_ownerfn ;
    string_type   _M_category;
    size_type     _M_index   ;

    template <typename>
    friend class ReferenceBase;

    template <typename, bool>
    friend class ValueArray;

    friend class TemplateObject;
};

// ======================================================================

template <typename T>
class ReferenceBase
{
public:
    static_assert (!std::is_void<T>::value && !std::is_pointer<T>::value, "invalid type!");

    typedef typename std::remove_const<T>::type base_type;

    typedef typename std::conditional<std::is_same<int16, base_type>::value ||
                                      std::is_enum<base_type>::value,
            int,
            typename std::conditional<std::is_same<u16, base_type>::value, uint, base_type>::type
            >::type
            default_type;

    typedef base_type           value_type ;
    typedef Parser::pointer     pointer    ;
    typedef Parser::size_type   size_type  ;
    typedef Parser::value_type  json_type  ;
    typedef Parser::string_type string_type;

    inline explicit operator bool () const noexcept
    {
        return _M_owner.type() != Parser::Type::Null &&
               _M_ref != nullptr && !_M_ref->IsNull();
    }

    inline rapidjson::Type type() const
    {
        return ref()->GetType();
    }

    constexpr ReferenceBase () noexcept
    : _M_ref(),
      _M_owner()
    { }

    constexpr ReferenceBase (ReferenceBase const& obj) noexcept
    : _M_ref(obj._M_ref),
      _M_owner(obj._M_owner)
    { }

    virtual ~ReferenceBase()
    { }

    inline ReferenceBase& operator = (ReferenceBase const& obj) noexcept
    {
        if (this != &obj)
        {
            _M_ref   = obj._M_ref;
            _M_owner = obj._M_owner;
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

protected:
    inline ReferenceBase (TemplateObject*     owner,
                          string_type  const& key,
                          default_type const& default_val = default_type ())
    : _M_ref(),
      _M_owner(*owner)
    {
        if(!(*owner)->HasMember(key.c_str()))
        {
            std::cout << "Json::"    << __func__ << ": invalid reference "
                      << key         << " with default value: "
                      << default_val << ". Trying to create a new one." << std::endl;

            //! try to obtain a valid reference
            (*owner)->AddMember(json_type(key.c_str(),
                                          static_cast<size_type>(key.size()),
                                          owner->parser()->GetAllocator()).Move(),
                                json_type(default_type(default_val)).Move(),
                                owner->parser()->GetAllocator());


        }

        _M_ref = &owner->operator[](key);

        if(!_M_ref)
        {
            throw std::runtime_error("Json::Reference nullptr!");
        }
    }

    inline ReferenceBase (TemplateArray*      owner,
                          size_type           idx,
                          default_type const& default_val = default_type ())
    : _M_ref(),
      _M_owner(*owner)
    {
        if (owner->size() <= idx)
        {
            std::cout << "Json::"    << __func__ << ": invalid reference index "
                      << idx         << " with default value: "
                      << default_val << ". Trying to create a new one." << std::endl;

            //! try to obtain a valid reference
            (*owner)->PushBack(json_type(default_type(default_val)).Move(),
                               owner->parser()->GetAllocator());

            idx = owner->size() - 1;
        }

        _M_ref = &owner->operator[](idx);

        if(!_M_ref) throw std::runtime_error("Json::Reference nullptr!");
    }

    inline pointer parentObjectRef() const
    {
        assert(_M_owner.type() != Parser::Type::Null);
        return _M_owner.object()->_M_ref;
    }

    inline pointer parentArrayRef() const
    {
        assert(_M_owner.type() != Parser::Type::Null);
        return _M_owner.array()->_M_ref;
    }

private:
    pointer       _M_ref  ;
    TemplateOwner _M_owner;

    friend class Reference<T>;

    template <typename, bool>
    friend class ValueArray;
};

// ======================================================================

template <>
class ReferenceBase<Parser::string_type>
{
public:
    typedef Parser::pointer     pointer    ;
    typedef Parser::size_type   size_type  ;
    typedef Parser::string_type value_type ;
    typedef Parser::value_type  json_type  ;
    typedef Parser::string_type string_type;

    inline explicit operator bool () const noexcept
    {
        return _M_owner.type() != Parser::Type::Null &&
               _M_ref != nullptr && !_M_ref->IsNull();
    }

    constexpr rapidjson::Type type() const noexcept
    {
        assert(_M_ref != nullptr);
        return rapidjson::kStringType;
    }

    constexpr ReferenceBase () noexcept
    : _M_ref(),
      _M_owner()
    { }

    constexpr ReferenceBase (ReferenceBase const& obj) noexcept
    : _M_ref(obj._M_ref),
      _M_owner(obj._M_owner)
    { }

    virtual ~ReferenceBase()
    { }

    inline ReferenceBase& operator = (ReferenceBase const& obj) noexcept
    {
        if (this != &obj)
        {
            _M_ref   = obj._M_ref;
            _M_owner = obj._M_owner;
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

protected:
    ReferenceBase (TemplateObject*    owner,
                   string_type const& key,
                   value_type  const& default_val = value_type ());

    ReferenceBase (TemplateArray*    owner,
                   size_type         idx,
                   value_type const& default_val = value_type ());

    inline pointer parentObjectRef() const
    {
        assert(_M_owner.type() != Parser::Type::Null);
        return _M_owner.object()->_M_ref;
    }

    inline pointer parentArrayRef() const
    {
        assert(_M_owner.type() != Parser::Type::Null);
        return _M_owner.array()->_M_ref;
    }

private:
    pointer       _M_ref  ;
    TemplateOwner _M_owner;

    friend class Reference<value_type>;

    template <typename, bool>
    friend class ValueArray;
};

//======================================================

template <typename T>
class Reference<T> : public ReferenceBase<T>
{
public:
    static_assert (std::is_enum<T>::value, "T is NOT an enum!");

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
    {
        if (!this->_M_ref->IsNumber()) throw std::runtime_error("Reference is NOT Enum");
    }

    inline Reference (TemplateArray*      owner,
                      size_type           idx,
                      default_type const& default_val = default_type())
    : reference_base(owner, idx, default_val)
    {
        if (!this->_M_ref->IsNumber()) throw std::runtime_error("Reference is NOT Enum");
    }

    inline Reference (Reference const& obj)
    : reference_base(obj)
    { }

    inline operator value_type () const noexcept
    {
        return static_cast<value_type>(this->ref()->GetInt());
    }

    inline int toInt () const noexcept
    {
        return this->ref()->GetInt();
    }

    inline Reference& operator = (Reference const& ref)
    {
        if (this != &ref)
        {
            reference_base::operator = (ref);
        }

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
class Reference<bool> : public ReferenceBase<bool>
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

    inline Reference& operator = (Reference const& ref)
    {
        if (this != &ref)
        {
            reference_base::operator = (ref);
        }

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
class Reference<u16> : public ReferenceBase<u16>
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

    inline Reference& operator = (Reference const& ref) noexcept
    {
        if (this != &ref)
        {
            reference_base::operator = (ref);
        }

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
class Reference<int16> : public ReferenceBase<int16>
{
public:
    typedef ReferenceBase<int16>                  reference_base;
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

    inline Reference& operator = (Reference const& ref) noexcept
    {
        if (this != &ref)
        {
            reference_base::operator = (ref);
        }

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
class Reference<uint> : public ReferenceBase<uint>
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

    inline Reference& operator = (Reference const& ref) noexcept
    {
        if (this != &ref)
        {
            reference_base::operator = (ref);
        }

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
class Reference<int> : public ReferenceBase<int>
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

    inline Reference& operator = (Reference const& ref) noexcept
    {
        if (this != &ref)
        {
            reference_base::operator = (ref);
        }

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
class Reference<u64> : public ReferenceBase<u64>
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

    inline Reference& operator = (Reference const& ref) noexcept
    {
        if (this != &ref)
        {
            reference_base::operator = (ref);
        }

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
class Reference<int64> : public ReferenceBase<int64>
{
public:
    typedef ReferenceBase<int64>                 reference_base;
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

    inline Reference& operator = (Reference const& ref) noexcept
    {
        if (this != &ref)
        {
            reference_base::operator = (ref);
        }

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
class Reference<float> : public ReferenceBase<float>
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

    inline Reference& operator = (Reference const& ref) noexcept
    {
        if (this != &ref)
        {
            reference_base::operator = (ref);
        }

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
class Reference<double> : public ReferenceBase<double>
{
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

    inline Reference& operator = (Reference const& ref) noexcept
    {
        if (this != &ref)
        {
            reference_base::operator = (ref);
        }

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
class Reference<Parser::string_type> : public ReferenceBase<Parser::string_type>
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

        connect(jsonStringInvoked, [fn](string_type& val)
        {
            val = fn ();
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

        connect(jsonEnumInvoked, [fn](int& val)
        {
            val = fn ();
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

        connect(jsonStringInvoked, [fn](string_type& val)
        {
            val = fn ();
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

        connect(jsonEnumInvoked, [fn](int& val)
        {
            val = fn ();
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

    inline value_type toString() const
    {
        return value_type(ref()->GetString(), ref()->GetStringLength());
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

    inline Reference& operator = (Reference const& ref) noexcept
    {
        if (this != &ref)
        {
            reference_base::operator = (ref);

            jsonEnumInvoked   = ref.jsonEnumInvoked;
            jsonStringInvoked = ref.jsonStringInvoked;
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
    Signal<void(int&)>         jsonEnumInvoked  ;
    Signal<void(string_type&)> jsonStringInvoked;
};

//======================================================

inline bool operator == (Reference<Parser::string_type> const& str1,
                         Reference<Parser::string_type> const& str2)
{
   return str1.toString() == str2.toString();
}

inline bool operator != (Reference<Parser::string_type> const& str1,
                         Reference<Parser::string_type> const& str2)
{
   return !(str1 == str2);
}

inline bool operator == (Reference<Parser::string_type> const& str1,
                         Reference<Parser::string_type>::value_type const& str2)
{
   return str1.toString() == str2;
}

inline bool operator != (Reference<Parser::string_type> const& str1,
                         Reference<Parser::string_type>::value_type const& str2)
{
   return !(str1 == str2);
}

inline bool operator == (Reference<Parser::string_type>::value_type const& str1,
                         Reference<Parser::string_type> const& str2)
{
   return str1 == str2.toString();
}

inline bool operator != (Reference<Parser::string_type>::value_type const& str1,
                         Reference<Parser::string_type> const& str2)
{
   return !(str1 == str2);
}

inline bool operator == (cchar* str1, Reference<Parser::string_type> const& str2) noexcept
{
   return str1 == str2.toString();
}

inline bool operator != (cchar* str1, Reference<Parser::string_type> const& str2) noexcept
{
   return !(str1 == str2);
}

inline bool operator == (Reference<Parser::string_type> const& str1, cchar* str2) noexcept
{
   return str1.toString() == str2;
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

    return value_type(ref.toString() + str);
}

inline
Reference<Parser::string_type>::value_type operator + (Reference<Parser::string_type> const& ref,
                                                       Reference<Parser::string_type>::value_type::
                                                       value_type str)
{
    typedef Reference<Parser::string_type>::value_type value_type;

    return value_type(ref.toString() + str);
}

inline
Reference<Parser::string_type>::value_type operator + (Reference<Parser::string_type> const& ref,
                                                       Reference<Parser::string_type>::value_type::
                                                       value_type const* str)
{
    typedef Reference<Parser::string_type>::value_type value_type;

    return value_type(ref.toString() + str);
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

    return value_type(str + ref.toString());
}

inline
Reference<Parser::string_type>::value_type operator + (Reference<Parser::string_type>::value_type::
                                                       value_type const* str,
                                                       Reference<Parser::string_type> const& ref)
{
    typedef Reference<Parser::string_type>::value_type value_type;

    return value_type(str + ref.toString());
}

// ======================================================================

template <typename T>
class ValueArray<T, true> : public TemplateArray
{
public:
    typedef T                                    value_type     ;
    typedef std::deque<value_type>               array_type     ;
    typedef typename array_type::reference       reference      ;
    typedef typename array_type::const_reference const_reference;
    typedef typename array_type::iterator        iterator       ;
    typedef typename array_type::const_iterator  const_iterator ;

    ValueArray (Parser& prsr, string_type const& category)
    : TemplateArray (prsr, category)
    {
        for (size_type i = 0; i < size(); ++i) _M_values.emplace_back(this, i);

        connections();
    }

    ValueArray (TemplateObject* owner, string_type const& category)
    : TemplateArray (owner, category)
    {
        for (size_type i = 0; i < size(); ++i) _M_values.emplace_back(this, i);

        connections();
    }

    inline iterator begin ()
    {
        return _M_values.begin();
    }

    inline const_iterator cbegin () const
    {
        return _M_values.cbegin();
    }

    inline iterator end ()
    {
        return _M_values.end();
    }

    inline const_iterator cend () const
    {
        return _M_values.cend();
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
        connect(appended, *this, &ValueArray::onIndexAppended);
        connect(aboutToRemove, *this, &ValueArray::onIndexRemoved);
        connect(changed, *this, &ValueArray::onChanged);
    }

    void disconnections()
    {
        disconnect(appended, function_type(this, &ValueArray::onIndexAppended));
        disconnect(aboutToRemove, function_p_type(this, &ValueArray::onIndexRemoved));
        disconnect(changed, function_type(this, &ValueArray::onChanged));
    }

private:
    array_type _M_values;
};

// ======================================================================

template <typename T>
class ValueArray<T, false> : public TemplateArray
{
public:
    typedef Reference<T>                         value_type     ;
    typedef std::deque<value_type>               array_type     ;
    typedef typename array_type::reference       reference      ;
    typedef typename array_type::const_reference const_reference;
    typedef typename array_type::iterator        iterator       ;
    typedef typename array_type::const_iterator  const_iterator ;

    ValueArray (Parser& prsr, string_type const& category)
    : TemplateArray (prsr, category)
    {
        for (size_type i = 0; i < size(); ++i) _M_values.emplace_back(this, i);

        connections();
    }

    ValueArray (TemplateObject* owner, string_type const& category)
    : TemplateArray (owner, category)
    {
        for (size_type i = 0; i < size(); ++i) _M_values.emplace_back(this, i);

        connections();
    }

    inline iterator begin ()
    {
        return _M_values.begin();
    }

    inline const_iterator cbegin () const
    {
        return _M_values.cbegin();
    }

    inline iterator end ()
    {
        return _M_values.end();
    }

    inline const_iterator cend () const
    {
        return _M_values.cend();
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
        connect(appended, *this, &ValueArray::onIndexAppended);
        connect(aboutToRemove , *this, &ValueArray::onIndexRemoved);
        connect(changed , *this, &ValueArray::onChanged);
    }

    void disconnections()
    {
        disconnect(appended, function_type(this, &ValueArray::onIndexAppended));
        disconnect(aboutToRemove , function_p_type(this, &ValueArray::onIndexRemoved));
        disconnect(changed , function_type(this, &ValueArray::onChanged));
    }

private:
    array_type _M_values;
};

// ======================================================================

class Factory
{
public:
    template<typename T>
    using shared_ptr = std::shared_ptr<T>;

    typedef Parser::string_type                               string_type       ;
    typedef Parser::size_type                                 size_type         ;
    typedef shared_ptr<Parser>                                json_ptr          ;
    typedef std::pair<string_type, json_ptr>                  json_pair         ;
    typedef std::pair<size_type, string_type>                 file_pair         ;
    typedef shared_ptr<TemplateObject>                        generic_object_ptr;
    typedef shared_ptr<TemplateArray>                         generic_array_ptr ;
    typedef std::unordered_map<size_type, json_pair>          json_map          ;
    typedef std::unordered_map<size_type, generic_object_ptr> objects_map       ;
    typedef std::unordered_map<size_type, generic_array_ptr>  arrays_map        ;
    typedef json_map::iterator                                iterator          ;
    typedef json_map::const_iterator                          const_iterator    ;
    typedef json_map::mapped_type                             value_type        ;
    typedef json_map::mapped_type&                            reference         ;
    typedef json_map::mapped_type const&                      const_reference   ;

    // ======================================================================

    //! example doc key
    enum Type
    {
        General,
        Common,
        Settings,
        Devices
    };

    template<typename T, typename... Args>
    shared_ptr<T> createObject(size_type type, size_type key, Args&&... args)
    {
        auto json = _M_jsonDocs.find(type);

        if(json != _M_jsonDocs.end())
        {
            auto it = _M_objects.find(key);

            if (it == _M_objects.end())
            {
                _M_objects.emplace(key, shared_ptr<T>(new T(*json->second.second.get(),
                                                            std::forward<Args>(args)...)));
            }

            return std::dynamic_pointer_cast<T>(_M_objects[key]);
        }

        return shared_ptr<T>();
    }

    template<typename T, typename... Args>
    shared_ptr<T> createArray(size_type type, size_type key, Args&&... args)
    {
        auto json = _M_jsonDocs.find(type);

        if (json != _M_jsonDocs.end())
        {
            auto it = _M_arrays.find(key);

            if (it == _M_arrays.end())
            {
                _M_arrays.emplace(key, shared_ptr<T>(new T(*json->second.second.get(),
                                                           std::forward<Args>(args)...)));
            }

            return std::dynamic_pointer_cast<T>(_M_arrays[key]);
        }

        return shared_ptr<T>();
    }

    // ======================================================================

    Factory() = delete;
    Factory(std::initializer_list<file_pair> json_files);

    void reset();
    bool save (size_type key);
    void save ();

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

    Signal<void()> aboutToReset;
    Signal<void()> afterReset  ;

private:
    mutable json_map    _M_jsonDocs;
    mutable objects_map _M_objects ;
    mutable arrays_map  _M_arrays  ;
};

// ======================================================================

template <typename T>
inline std::ostream& operator << (std::ostream& stream, Reference<T> const& ref)
{
   return stream << ref;
}

inline std::ostream& operator << (std::ostream& stream, Reference<Parser::string_type> const& ref)
{
   return stream << ref.toString();
}

} } // namespace Json


#endif // CPPUAL_JSON_ADAPTOR_H
