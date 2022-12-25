#include <cppual/json/json_adaptor.h>

#include <cppual/json/filereadstream.h>
#include <cppual/json/filewritestream.h>
#include <cppual/json/istreamwrapper.h>
#include <cppual/json/ostreamwrapper.h>
#include <cppual/json/prettywriter.h>

#include <iostream>
#include <cassert>

namespace cppual { namespace json {

// ======================================================================

doc_parser::doc_parser(string_type const& file_path, doc_type type)
: Document(),
  _M_type (type)
{
    std::ifstream file(file_path.c_str());

    std::cout << "Json File " << file_path << std::endl;

    if (!file.is_open()) return;

    file.seekg(std::ios::beg, std::ios::end);

    auto const size = file.tellg();

    file.seekg(std::ios::beg);

    IStreamWrapper stream(file);

    ParseStream(stream);

    if (HasParseError() || !size)
    {
        if (type == doc_type::array)
        {
            SetArray();
            _M_type = doc_type::array;
        }
        else
        {
            SetObject();
            _M_type = doc_type::object;
        }
    }

    if (!IsArray() && !IsObject()) _M_type = doc_type::null;
}

doc_parser::doc_parser(std::istream& data, doc_type type)
: Document(),
  _M_type (type)
{
    data.seekg(std::ios::beg, std::ios::end);

    auto const size = data.tellg();

    data.seekg(std::ios::beg);

    IStreamWrapper stream(data);

    ParseStream(stream);

    if (HasParseError() || !size)
    {
        if (type == doc_type::array)
        {
            SetArray();
            _M_type = doc_type::array;
        }
        else
        {
            SetObject();
            _M_type = doc_type::object;
        }
    }

    if (!IsArray() && !IsObject()) _M_type = doc_type::null;
}


bool doc_parser::create_document(string_type const& file_path, doc_type type)
{
    std::ifstream file(file_path.c_str());

    if(!file.is_open()) return false;

    file.seekg(std::ios::beg, std::ios::end);

    auto const size = file.tellg();

    file.seekg(std::ios::beg);

    IStreamWrapper stream(file);

    ParseStream(stream);

    if (HasParseError() || !size)
    {
        if (type == doc_type::array)
        {
            SetArray();
            _M_type = doc_type::array;
        }
        else
        {
            SetObject();
            _M_type = doc_type::object;
        }
    }
    else
    {
        switch (GetType ())
        {
        case kObjectType:
            _M_type = doc_type::object;
            break;
        case kArrayType:
            _M_type = doc_type::array;
            break;
        default:
            _M_type = doc_type::null;
            break;
        }
    }

    changed();

    return true;
}

bool doc_parser::save(std::ostream& stream)
{
    about_to_save();

    OStreamWrapper wrapper(stream);
    PrettyWriter<OStreamWrapper> writer(wrapper);

    return base_type::Accept(writer);
}

doc_parser::doc_type doc_parser::type() const
{
    return _M_type;
}

doc_parser::size_type doc_parser::size() const
{
    return type() == doc_type::array ? Size() :
                                       type() == doc_type::object ? MemberCount() :
                                                                    doc_parser::size_type();
}

doc_parser::const_reference doc_parser::operator [] (string_type const& key) const
{
    assert(type() == doc_type::object);
    return base_type::operator[](key.c_str());
}

doc_parser::reference doc_parser::operator [] (string_type const& key)
{
    assert(type() == doc_type::object);
    return base_type::operator[](key.c_str());
}

doc_parser::const_reference doc_parser::operator [] (size_type idx) const
{
    assert(type() == doc_type::array);
    return base_type::operator[](idx);
}

doc_parser::reference doc_parser::operator [] (size_type idx)
{
    assert(type() == doc_type::array);
    return base_type::operator[](idx);
}

// ======================================================================

template_base::template_base(doc_parser& _parser, string_type const& _category)
: _M_owner(),
  _M_parser(&_parser),
  _M_ref(),
  _M_category(_category),
  _M_index(npos)
{
}

template_base::template_base(doc_parser& _parser, size_type _index)
: _M_owner(),
  _M_parser(&_parser),
  _M_ref(),
  _M_category(),
  _M_index(_index)
{
}

template_base::template_base(template_object* _owner, string_type const& _category)
: _M_owner(*_owner),
  _M_parser(_owner->parser()),
  _M_ref(),
  _M_category(_category),
  _M_index(npos)
{
}

template_base::template_base(template_array* _owner, size_type _idx)
: _M_owner(*_owner),
  _M_parser(_owner->parser()),
  _M_ref(),
  _M_category(),
  _M_index(_idx)
{
}

template_base::template_base(template_base const& obj)
: _M_owner(obj._M_owner),
  _M_parser(obj._M_parser),
  _M_ref(obj._M_ref),
  _M_category(obj._M_category),
  _M_index(obj._M_index)
{
}

template_base::template_base(template_base&& obj)
: _M_owner(obj._M_owner),
  _M_parser(obj._M_parser),
  _M_ref(obj._M_ref),
  _M_category(obj._M_category),
  _M_index(obj._M_index)
{
    obj.invalidate();
}

template_base& template_base::operator = (template_base const& obj)
{
    if (this == &obj) return *this;

    disconnections();

    _M_parser   = obj._M_parser;
    _M_ref      = obj._M_ref;
    _M_category = obj._M_category;
    _M_index    = obj._M_index;
    _M_owner    = obj._M_owner;

    connections();

    changed();

    return *this;
}

template_base& template_base::operator = (template_base&& obj)
{
    if (this == &obj) return *this;

    disconnections();

    _M_parser   = obj._M_parser;
    _M_ref      = obj._M_ref;
    _M_category = obj._M_category;
    _M_index    = obj._M_index;
    _M_owner    = obj._M_owner;

    obj.invalidate();
    connections();

    changed();

    return *this;
}

template_base::~template_base()
{
    destroyed ();
}

template_base::size_type template_base::size() const
{
    assert(_M_ref != nullptr);
    return type () == doc_parser::doc_type::object ?
                _M_ref->MemberCount () :
                type () == doc_parser::doc_type::array ?
                    _M_ref->Size () :
                    size_type ();
}

bool template_base::empty() const
{
    assert(_M_ref != nullptr);
    return type () == doc_parser::doc_type::object ?
                _M_ref->MemberCount () == 0 :
                type () == doc_parser::doc_type::array ?
                    _M_ref->Size ()  == 0 :
                    true;
}

void template_base::invalidate()
{
    if (_M_ref == nullptr) return;

    _M_owner    = template_owner ();
    _M_parser   = nullptr;
    _M_ref      = nullptr;
    _M_category = string_type ();
    _M_index    = size_type ();

    invalidated ();
}

template_base::pointer template_base::parent_object_ref() const
{
    assert(_M_owner.object()->_M_ref != nullptr);
    return _M_owner.object()->_M_ref;
}

template_base::pointer template_base::parent_array_ref() const
{
    assert(_M_owner.array()->_M_ref != nullptr);
    return _M_owner.array()->_M_ref;
}

void template_base::connections()
{

}

void template_base::disconnections()
{

}

// ======================================================================

template_object::template_object(doc_parser& _parser, string_type const& _category)
: template_base (_parser, _category)
{
    assign_from_parser_object();
    template_object::connections();
}

template_object::template_object(doc_parser& _parser, size_type _index)
: template_base (_parser, _index)
{
    assign_from_parser_array();
    template_object::connections();
}

template_object::template_object(template_object* _owner, string_type const& _category)
: template_base (_owner, _category)
{
    assign_from_object();
    template_object::connections();
}

template_object::template_object(template_array* _owner, size_type _idx)
: template_base (_owner, _idx)
{
    assign_from_array();
    template_object::connections();
}

template_object::template_object(template_object const& obj)
: template_base (obj)
{
    template_object::connections();
}

template_object::template_object(template_object&& obj)
: template_base (std::move (obj))
{
    template_object::connections();
}

template_object& template_object::operator = (template_object const& obj)
{
    template_base::operator = (obj);
    return *this;
}

template_object& template_object::operator = (template_object&& obj)
{
    template_base::operator = (std::move (obj));
    return *this;
}

template_object::~template_object()
{
    template_object::disconnections ();
}

template_object::const_reference template_object::operator [] (string_type const& key) const
{
    assert(_M_ref != nullptr);
    return _M_ref->operator[](key.c_str());
}

template_object::reference template_object::operator [] (string_type const& key)
{
    assert(_M_ref != nullptr);
    return _M_ref->operator[](key.c_str());
}

void template_object::connections()
{
    if (_M_owner == nullptr && _M_parser)
    {
        connect(_M_parser->changed, *this, &template_object::on_parser_data_changed);
        connect(_M_parser->about_to_save, *this, &template_object::on_about_to_save);
    }
    else if (_M_owner.type() == doc_parser::doc_type::object)
    {
        connect(_M_owner.object()->changed, *this, &template_object::on_object_changed);
        connect(_M_owner.object()->about_to_save, *this, &template_object::on_about_to_save);
        connect(_M_owner.object()->destroyed, *this, &template_object::invalidate);
    }
    else if (_M_owner.type() == doc_parser::doc_type::array)
    {
        connect(_M_owner.array()->changed, *this, &template_object::on_array_changed);
        connect(_M_owner.array()->removed, *this, &template_object::on_removed);
        connect(_M_owner.array()->about_to_save, *this, &template_object::on_about_to_save);
        connect(_M_owner.array()->destroyed, *this, &template_object::invalidate);
    }
}

void template_object::disconnections()
{
    if (!_M_ref) return;

    if (_M_owner == nullptr && _M_parser)
    {
        disconnect(_M_parser->changed, *this, &template_object::on_parser_data_changed);
        disconnect(_M_parser->about_to_save, *this, &template_object::on_about_to_save);
    }
    else if (_M_owner.type() == doc_parser::doc_type::object)
    {
        disconnect(_M_owner.object()->changed, *this, &template_object::on_object_changed);
        disconnect(_M_owner.object()->about_to_save, *this, &template_object::on_about_to_save);
        disconnect(_M_owner.object()->destroyed, *this, &template_object::invalidate);
    }
    else if (_M_owner.type() == doc_parser::doc_type::array)
    {
        disconnect(_M_owner.array()->changed, *this, &template_object::on_array_changed);
        disconnect(_M_owner.array()->removed, *this, &template_object::on_removed);
        disconnect(_M_owner.array()->about_to_save, *this, &template_object::on_about_to_save);
        disconnect(_M_owner.array()->destroyed, *this, &template_object::invalidate);
    }
}

void template_object::assign_from_parser_object()
{
    assert(_M_category.size() > 0);
    assert(_M_parser->type() == doc_parser::doc_type::object);

    if (!_M_parser->HasMember(_M_category.c_str()))
    {
        std::cout << "Json Object category " << _M_category << " doesn't "
                  << "exist. Creating a new one." << std::endl;

        _M_parser->AddMember(value_type(_M_category.c_str(),
                                        static_cast<size_type>(_M_category.size()),
                                        _M_parser->GetAllocator()),
                             value_type(kObjectType),
                             _M_parser->GetAllocator());
    }

    _M_ref = &(*_M_parser)[_M_category];

    if (!_M_ref->IsObject()) _M_ref->SetObject();
    assert(_M_ref->IsObject());
}

void template_object::assign_from_parser_array()
{
    assert(_M_parser->type() == doc_parser::doc_type::array);

    if (_M_parser->size() <= _M_index)
    {
        std::cout << "Json Array index " << _M_index << " doesn't "
                  << "exist. Appending new Object with index " << _M_parser->size()
                  << "." << std::endl;

        _M_parser->PushBack(value_type(kObjectType), _M_parser->GetAllocator());

        _M_index = _M_parser->size() - 1;
    }

    _M_ref = &(*_M_parser)[_M_index];

    if (!_M_ref->IsObject()) _M_ref->SetObject();
    assert(_M_ref->IsObject());
}

void template_object::assign_from_object()
{
    assert(_M_category.size() > 0);

    if (!parent_object_ref()->HasMember(_M_category.c_str()))
    {
        std::cout << "Json Object category " << _M_category << " doesn't "
                  << "exist. Creating a new one." << std::endl;

        parent_object_ref()->AddMember(value_type(_M_category.c_str(),
                                                  static_cast<size_type>(_M_category.size()),
                                                  _M_parser->GetAllocator()),
                                       value_type(kObjectType),
                                       _M_parser->GetAllocator());
    }

    _M_ref = &(*_M_owner.object())[_M_category];

    if (!_M_ref->IsObject()) _M_ref->SetObject();
    assert(_M_ref->IsObject());
}

void template_object::assign_from_array()
{
    if (parent_array_ref()->Size() <= _M_index)
    {
        std::cout << "Json Array index " << _M_index << " doesn't "
                  << "exist. Appending new Object with index " << parent_array_ref()->Size()
                  << "." << std::endl;

        parent_array_ref()->PushBack(value_type(kObjectType),
                                     _M_parser->GetAllocator());

        _M_index = parent_array_ref()->Size() - 1;
    }

    _M_ref = &(*_M_owner.array())[_M_index];

    if (!_M_ref->IsObject()) _M_ref->SetObject();
    assert(_M_ref->IsObject());
}

void template_object::on_parser_data_changed()
{
    switch (_M_parser->type())
    {
    case doc_parser::doc_type::array:
        assign_from_parser_array();
        break;
    case doc_parser::doc_type::object:
        assign_from_parser_object();
        break;
    default:
        break;
    }

    changed();
}

void template_object::on_about_to_save()
{
    about_to_save();
}

void template_object::on_object_changed()
{
    if (_M_owner == nullptr) invalidate();
    else
    {
        assign_from_object();
        changed();
    }
}

void template_object::on_array_changed()
{
    if (_M_owner == nullptr) invalidate();
    else
    {
        assign_from_array();
        changed();
    }
}

void template_object::on_removed(size_type idx)
{
    if (_M_index == idx) invalidate();
}

// ======================================================================

template_array::template_array(doc_parser& _parser, string_type const& _category)
: template_base (_parser, _category)
{
    assign_from_parser_object();
    template_array::connections();
}

template_array::template_array(doc_parser& _parser, size_type _index)
: template_base (_parser, _index)
{
    assign_from_parser_array();
    template_array::connections();
}

template_array::template_array(template_object* _owner, string_type const& _category)
: template_base (_owner, _category)
{
    assign_from_object();
    template_array::connections();
}

template_array::template_array(template_array* _owner, size_type _idx)
: template_base (_owner, _idx)
{
    assign_from_array();
    template_array::connections();
}

template_array::template_array(template_array const& obj)
: template_base (obj)
{
    template_array::connections();
}

template_array::template_array(template_array&& obj)
: template_base (obj)
{
    template_array::connections();
}

template_array& template_array::operator = (template_array const& obj)
{
    template_base::operator = (obj);
    return *this;
}

template_array& template_array::operator = (template_array&& obj)
{
    template_base::operator = (std::move (obj));
    return *this;
}

template_array::~template_array()
{
    template_array::disconnections ();
}

template_array::const_reference template_array::operator [] (size_type idx) const
{
    assert(_M_ref != nullptr);
    return _M_ref->operator[](idx);
}

template_array::reference template_array::operator [] (size_type idx)
{
    assert(_M_ref != nullptr);
    return _M_ref->operator[](idx);
}

void template_array::connections()
{
    if (_M_owner == nullptr && _M_parser)
    {
        connect(_M_parser->changed, *this, &template_array::on_parser_data_changed);
        connect(_M_parser->about_to_save, *this, &template_array::on_about_to_save);
    }
    else if (_M_owner.type() == doc_parser::doc_type::object)
    {
        connect(_M_owner.object()->changed, *this, &template_array::on_object_changed);
        connect(_M_owner.object()->about_to_save, *this, &template_array::on_about_to_save);
        connect(_M_owner.object()->destroyed, *this, &template_array::invalidate);
    }
    else if (_M_owner.type() == doc_parser::doc_type::array)
    {
        connect(_M_owner.array()->changed, *this, &template_array::on_array_changed);
        connect(_M_owner.array()->removed, *this, &template_array::on_removed);
        connect(_M_owner.array()->about_to_save, *this, &template_array::on_about_to_save);
        connect(_M_owner.array()->destroyed, *this, &template_array::invalidate);
    }
}

void template_array::disconnections()
{
    if (!_M_ref) return;

    if (_M_owner == nullptr && _M_parser)
    {
        disconnect(_M_parser->changed, *this, &template_array::on_parser_data_changed);
        disconnect(_M_parser->about_to_save, *this, &template_array::on_about_to_save);
    }
    else if (_M_owner.type() == doc_parser::doc_type::object)
    {
        disconnect(_M_owner.object()->changed, *this, &template_array::on_object_changed);
        disconnect(_M_owner.object()->about_to_save, *this, &template_array::on_about_to_save);
        disconnect(_M_owner.object()->destroyed, *this, &template_array::invalidate);
    }
    else if (_M_owner.type() == doc_parser::doc_type::array)
    {
        disconnect(_M_owner.array()->changed, *this, &template_array::on_array_changed);
        disconnect(_M_owner.array()->removed, *this, &template_array::on_removed);
        disconnect(_M_owner.array()->about_to_save, *this, &template_array::on_about_to_save);
        disconnect(_M_owner.array()->destroyed, *this, &template_array::invalidate);
    }
}

void template_array::assign_from_parser_object()
{
    assert(_M_category.size() > 0);
    assert(_M_parser->type() == doc_parser::doc_type::object);

    if (!_M_parser->HasMember(_M_category.c_str()))
    {
        std::cout << "Json Array category " << _M_category << " doesn't "
                  << "exist. Creating a new one." << std::endl;

        _M_parser->AddMember(value_type(_M_category.c_str(),
                                        static_cast<size_type>(_M_category.size()),
                                        _M_parser->GetAllocator()),
                             value_type(kArrayType),
                             _M_parser->GetAllocator());
    }

    _M_ref = &(*_M_parser)[_M_category];

    if (!_M_ref->IsArray()) _M_ref->SetArray();
    assert(_M_ref->IsArray());
}

void template_array::assign_from_parser_array()
{
    assert(_M_parser->type() == doc_parser::doc_type::array);

    if (_M_parser->size() <= _M_index)
    {
        std::cout << "Json Array index " << _M_index << " doesn't "
                  << "exist. Appending new Array with index " << _M_parser->size()
                  << "." << std::endl;

        _M_parser->PushBack(value_type(kArrayType), _M_parser->GetAllocator());

        _M_index = _M_parser->size() - 1;
    }

    _M_ref = &(*_M_parser)[_M_index];

    if (!_M_ref->IsArray()) _M_ref->SetArray();
    assert(_M_ref->IsArray());
}

void template_array::assign_from_object()
{
    assert(_M_category.size() > 0);

    if (!parent_object_ref()->HasMember(_M_category.c_str()))
    {
        std::cout << "Json Array category " << _M_category << " doesn't "
                  << "exist. Creating a new one." << std::endl;

        parent_object_ref()->AddMember(value_type(_M_category.c_str(),
                                                  static_cast<size_type>(_M_category.size()),
                                                  _M_parser->GetAllocator()),
                                       value_type(kArrayType),
                                       _M_parser->GetAllocator());
    }

    _M_ref = &(*_M_owner.object())[_M_category];

    if (!_M_ref->IsArray()) _M_ref->SetArray();
    assert(_M_ref->IsArray());
}

void template_array::assign_from_array()
{
    if (parent_array_ref()->Size() <= _M_index)
    {
        std::cout << "Json Array index " << _M_index << " doesn't "
                  << "exist. Appending new Array with index " << parent_array_ref()->Size()
                  << "." << std::endl;

        parent_array_ref()->PushBack(value_type(kArrayType),
                                     _M_parser->GetAllocator());

        _M_index = parent_array_ref()->Size() - 1;
    }

    _M_ref = &(*_M_owner.array())[_M_index];

    if (!_M_ref->IsArray()) _M_ref->SetArray();
    assert(_M_ref->IsArray());
}

template_array::size_type template_array::size() const
{
    assert(_M_ref != nullptr);
    return _M_ref->Size();
}

bool template_array::empty() const
{
    assert(_M_ref != nullptr);
    return _M_ref->Empty();
}

void template_array::append()
{
    assert(_M_ref != nullptr && _M_parser != nullptr);

    about_to_append();
    _M_ref->PushBack(value_type(kNullType), _M_parser->GetAllocator());
    appended();
}

void template_array::remove(size_type idx)
{
    assert(_M_ref != nullptr);
    assert(_M_ref->Size() > idx);

    about_to_remove(idx);
    _M_ref->Erase(&(*this)[idx]);
    removed(idx);
}

void template_array::clear()
{
    if (empty()) return;

    about_to_clear();
    _M_ref->Clear();
    cleared();
}

void template_array::on_parser_data_changed()
{
    assert(_M_parser != nullptr);

    switch (_M_parser->type())
    {
    case doc_parser::doc_type::array:
        assign_from_parser_array();
        break;
    case doc_parser::doc_type::object:
        assign_from_parser_object();
        break;
    default:
        break;
    }

    changed();
}

void template_array::on_about_to_save()
{
    about_to_save();
}

void template_array::on_object_changed()
{
    if (_M_owner == nullptr) invalidate();
    else
    {
        assign_from_object();
        changed();
    }
}

void template_array::on_array_changed()
{
    if (_M_owner == nullptr) invalidate();
    else
    {
        assign_from_array();
        changed();
    }
}

void template_array::on_removed(size_type idx)
{
    if (_M_index == idx) invalidate();
}

// ======================================================================

value_reference_base::value_reference_base (template_object* owner)
: _M_ref  (),
  _M_owner(*owner),
  _M_fn   ()
{
    connections ();
}

value_reference_base::value_reference_base(template_array* owner)
: _M_ref  (),
  _M_owner(*owner),
  _M_fn   ()
{
    connections ();
}

value_reference_base::value_reference_base (value_reference_base const& obj)
: _M_ref(obj._M_ref),
  _M_owner(obj._M_owner),
  _M_fn(obj._M_fn)
{
    connections ();
}

value_reference_base::~value_reference_base ()
{
    disconnections ();
}

value_reference_base& value_reference_base::operator = (value_reference_base const& obj)
{
    if (this != &obj && obj._M_ref && _M_ref != obj._M_ref)
    {
        disconnections ();

        _M_ref    = obj._M_ref  ;
        _M_owner  = obj._M_owner;
        _M_fn     = obj._M_fn   ;

        connections ();
        changed ();
    }

    return *this;
}

void value_reference_base::connections()
{
    if (_M_owner.type() == doc_parser::doc_type::object)
    {
        connect(_M_owner.object()->changed, *this, &base_type::assign);
        connect(_M_owner.object()->destroyed, *this, &base_type::invalidate);
    }
    else if (_M_owner.type() == doc_parser::doc_type::array)
    {
        connect(_M_owner.array()->changed, *this, &base_type::assign);
        connect(_M_owner.array()->destroyed, *this, &base_type::invalidate);
    }
}

void value_reference_base::disconnections()
{
    if (_M_owner.type() == doc_parser::doc_type::object)
    {
        disconnect(_M_owner.object()->changed, *this, &base_type::assign);
        disconnect(_M_owner.object()->destroyed, *this, &base_type::invalidate);
    }
    else if (_M_owner.type() == doc_parser::doc_type::array)
    {
        disconnect(_M_owner.array()->changed, *this, &base_type::assign);
        disconnect(_M_owner.array()->destroyed, *this, &base_type::invalidate);
    }
}

void value_reference_base::assign()
{
    if(_M_fn == nullptr) return;

    _M_fn (this);
    changed ();
}

void value_reference_base::invalidate() noexcept
{
    _M_ref   = nullptr;
    _M_owner = template_owner ();
    _M_fn    = func_type ();
}

// ======================================================================

value_reference_template<doc_parser::string_type>::value_reference_template (template_object*   owner,
                                                                             string_type const& key,
                                                                             value_type  const& default_val)
: base_type (owner)
{
    _M_fn = func_type([def_key = key, def = default_val](base_type* ptr)
    {
        if(def_key.empty()) return;

        if (!(*ptr->_M_owner.object())->HasMember(def_key.c_str()))
        {
            std::cout << "json::" << __func__ << ": invalid reference "
                      << def_key  << " with default value: "
                      << def      << ". Trying to create a new one." << std::endl;

            /// try to obtain a valid reference
            (*ptr->_M_owner.object())->AddMember(json_type(def_key.c_str(),
                                                 static_cast<size_type>(def_key.size()),
                                                 ptr->_M_owner.object()->parser()->GetAllocator()),
                                                 json_type(def.c_str(),
                                                 static_cast<size_type>(def.size()),
                                                 ptr->_M_owner.object()->parser()->GetAllocator()),
                                                 ptr->_M_owner.object()->parser()->GetAllocator());
        }

        ptr->_M_ref = &(ptr->_M_owner.object()->operator[](def_key));

        if(!ptr->_M_ref)
        {
            throw std::runtime_error("json reference is nullptr!");
        }
    });

    _M_fn (this);
}

value_reference_template<doc_parser::string_type>::value_reference_template (template_array* owner,
                                                                             size_type idx,
                                                                             value_type const& default_val)
: base_type (owner)
{
    _M_fn = func_type([idx, def = default_val](base_type* ptr)
    {
        auto new_idx = idx;

        if ((*ptr->_M_owner.array())->Size() <= idx)
        {
            std::cout << "json::" << __func__ << ": invalid reference index "
                      << idx      << " with default value: "
                      << def      << ". Trying to create a new one." << std::endl;

            /// try to obtain a valid reference
            (*ptr->_M_owner.array())->PushBack(json_type(def.c_str(),
                                               static_cast<size_type>(def.size()),
                                               ptr->_M_owner.array()->parser()->GetAllocator()),
                                               ptr->_M_owner.array()->parser()->GetAllocator());

            new_idx = (*ptr->_M_owner.array())->Size() - 1;
        }

        ptr->_M_ref = &(ptr->_M_owner.array()->operator[](new_idx));

        if(!ptr->_M_ref)
        {
            throw std::runtime_error("json reference is nullptr!");
        }
    });

    _M_fn (this);
}

// ======================================================================

value_reference<bool>::value_reference(template_object*   owner,
                                       string_type const& name,
                                       value_type const&  default_val)
: reference_base(owner, name, default_val)
{
    if (!ref()->IsBool())
    {
        std::cerr << "json reference " << name << " value is not a bool! type: "
                  << type() << std::endl;
    }
}

value_reference<bool>::value_reference(template_array*   owner,
                                       size_type         idx,
                                       value_type const& default_val)
: reference_base(owner, idx, default_val)
{
    if (!ref()->IsBool())
    {
        std::cerr << "json reference value with index " << idx << " is not a bool! type: "
                  << type() << std::endl;
    }
}

// ======================================================================

value_reference<u16>::value_reference(template_object*   owner,
                                      string_type const& name,
                                      value_type         default_val)
: reference_base(owner, name, static_cast<default_type>(default_val))
{
    if (!ref()->IsUint())
    {
        std::cerr << "json reference " << name << " value is not an uint! type: "
                  << type() << std::endl;
    }
}

value_reference<u16>::value_reference(template_array* owner,
                          size_type      idx,
                          value_type     default_val)
: reference_base(owner, idx, static_cast<default_type>(default_val))
{
    if (!ref()->IsUint())
    {
        std::cerr << "json reference value with index " << idx << " is not an uint! type: "
                  << type() << std::endl;
    }
}

// ======================================================================

value_reference<i16>::value_reference (template_object*    owner,
                                       string_type  const& name,
                                       value_type          default_val)
: reference_base(owner, name, static_cast<default_type>(default_val))
{
    if (!ref()->IsInt())
    {
        std::cerr << "json reference " << name << " value is not a int! type: "
                  << type() << std::endl;
    }
}

value_reference<i16>::value_reference (template_array* owner,
                                       size_type       idx,
                                       value_type      default_val)
: reference_base(owner, idx, static_cast<default_type>(default_val))
{
    if (!ref()->IsInt())
    {
        std::cerr << "json reference value with index " << idx << " is not a int! type: "
                  << type() << std::endl;
    }
}

// ======================================================================

value_reference<uint>::value_reference (template_object*   owner,
                                        string_type const& name,
                                        value_type         default_val)
: reference_base(owner, name, default_val)
{
    if (!ref()->IsUint())
    {
        std::cerr << "json reference " << name << " value is not an uint! type: "
                  << type() << std::endl;
    }
}

value_reference<uint>::value_reference (template_array* owner,
                                        size_type       idx,
                                        value_type      default_val)
: reference_base(owner, idx, default_val)
{
    if (!ref()->IsUint())
    {
        std::cerr << "json reference value with index " << idx << " is not an uint! type: "
                  << type() << std::endl;
    }
}

// ======================================================================

value_reference<int>::value_reference (template_object*   owner,
                                       string_type const& name,
                                       value_type  const& default_val)
: reference_base(owner, name, default_val)
{
    if (!ref()->IsInt())
    {
        std::cerr << "json reference " << name << " value is not a int! type: "
                  << type() << std::endl;
    }
}

value_reference<int>::value_reference (template_array*   owner,
                                       size_type         idx,
                                       value_type const& default_val)
: reference_base(owner, idx, default_val)
{
    if (!ref()->IsInt())
    {
        std::cerr << "json reference value with index " << idx << " is not a int! type: "
                  << type() << std::endl;
    }
}

// ======================================================================

value_reference<u64>::value_reference (template_object*   owner,
                                       string_type const& name,
                                       value_type  const& default_val)
: reference_base(owner, name, default_val)
{
    if (!ref()->IsUint64())
    {
        std::cerr << "json reference " << name << " value is not an u64! type: "
                  << type() << std::endl;
    }
}

value_reference<u64>::value_reference (template_array*   owner,
                                       size_type         idx,
                                       value_type const& default_val)
: reference_base(owner, idx, default_val)
{
    if (!ref()->IsUint64())
    {
        std::cerr << "json reference value with index " << idx << " is not an u64! type: "
                  << type() << std::endl;
    }
}

// ======================================================================

value_reference<i64>::value_reference (template_object*   owner,
                                       string_type const& name,
                                       value_type  const& default_val)
: reference_base(owner, name, default_val)
{
    if (!ref()->IsInt64())
    {
        std::cerr << "json reference " << name << " value is not a int64! type: "
                  << type() << std::endl;
    }
}

value_reference<i64>::value_reference (template_array*   owner,
                                       size_type         idx,
                                       value_type const& default_val)
: reference_base(owner, idx, default_val)
{
    if (!ref()->IsInt64())
    {
        std::cerr << "json reference value with index " << idx << " is not a int64! type: "
                  << type() << std::endl;
    }
}

// ======================================================================

value_reference<float>::value_reference (template_object*   owner,
                                         string_type const& name,
                                         value_type  const& default_val)
: reference_base(owner, name, default_val)
{
    if (!ref()->IsNumber())
    {
        std::cerr << "json reference " << name << " value is not a float! type: "
                  << type() << std::endl;
    }
}

value_reference<float>::value_reference (template_array*   owner,
                                         size_type         idx,
                                         value_type const& default_val)
: reference_base(owner, idx, default_val)
{
    if (!ref()->IsNumber())
    {
        std::cerr << "json reference value with index " << idx << " is not a float! type: "
                  << type() << std::endl;
    }
}

// ======================================================================

value_reference<double>::value_reference (template_object*   owner,
                                          string_type const& name,
                                          value_type  const& default_val)
: reference_base(owner, name, default_val)
{
    if (!ref()->IsNumber())
    {
        std::cerr << "json reference " << name << " value is not a double! type: "
                  << type() << std::endl;
    }
}

value_reference<double>::value_reference (template_array*   owner,
                                          size_type         idx,
                                          value_type const& default_val)
: reference_base(owner, idx, default_val)
{
    if (!ref()->IsNumber())
    {
        std::cerr << "json reference value with index " << idx << " is not a double! type: "
                  << type() << std::endl;
    }
}

// ======================================================================

value_reference<doc_parser::string_type>::value_reference (template_object*   owner,
                                                           string_type const& name,
                                                           value_type  const& default_val)
: reference_base(owner, name, default_val)
{
    if (!ref()->IsString())
    {
        std::cerr << "json reference " << name << " value is not a string! type: "
                  << type() << std::endl;
    }
}

value_reference<doc_parser::string_type>::value_reference (template_array*   owner,
                                                           size_type         idx,
                                                           value_type const& default_val)
: reference_base(owner, idx, default_val)
{
    if (!ref()->IsString())
    {
        std::cerr << "json reference value with index " << idx << " is not a string! type: "
                  << type() << std::endl;
    }
}

// ======================================================================

factory::factory(std::initializer_list<file_tuple> json_files)
{
    typedef memory::allocator<doc_parser> doc_allocator;

    _M_jsonDocs.reserve(json_files.size());

    for (const auto& item : json_files)
    {
        // create if doesn't exit
        {
            std::fstream file;

            file.open(std::get<1>(item).c_str(), std::ios::out | std::ios::app);
        }

        _M_jsonDocs.emplace(std::get<0>(item),
                            std::make_pair(std::get<1>(item),
                                           memory::allocate_shared<doc_parser> (doc_allocator(),
                                                                                std::get<1>(item),
                                                                                std::get<2>(item))));
    }
}

void factory::reset()
{
    about_to_reset();

    for (auto& item : _M_objects)
    {
        item.second.clear();
    }

    for (auto& item : _M_arrays)
    {
        item.second.clear();
    }

    for (auto& item : _M_jsonDocs)
    {
        item.second.second->create_document(item.second.first, item.second.second->type ());
    }

    after_reset();
}

void factory::reset(size_type key)
{
    about_to_reset_doc(key);

    auto it_obj   = _M_objects.find  (key);
    auto it_array = _M_arrays.find   (key);
    auto it_doc   = _M_jsonDocs.find (key);

    if (it_obj != _M_objects.end ())
    {
        it_obj->second.clear ();
    }

    if (it_array != _M_arrays.end ())
    {
        it_array->second.clear ();
    }

    if (it_doc != _M_jsonDocs.end ())
    {
        it_doc->second.second->create_document (it_doc->second.first, it_doc->second.second->type ());
    }

    after_reset_doc (key);
}

bool factory::save (size_type key, string_type const& file_name)
{
    auto json = _M_jsonDocs.find (key);

    if (json != _M_jsonDocs.end ())
    {
        std::ofstream file;

        file.open (file_name.c_str (), std::ios::out | std::ios::trunc);

        return json->second.second->save (file);
    }

    return false;
}

bool factory::save (size_type key)
{
    auto json = _M_jsonDocs.find (key);

    if (json != _M_jsonDocs.end ())
    {
        std::ofstream file;

        file.open (json->second.first.c_str (), std::ios::out | std::ios::trunc);

        return json->second.second->save (file);
    }

    return false;
}

void factory::save ()
{
    for (auto& json : _M_jsonDocs)
    {
        std::ofstream file;

        file.open (json.second.first.c_str (), std::ios::out | std::ios::trunc);

        json.second.second->save (file);
    }
}

factory::generic_object_ptr factory::get_object (size_type type, size_type key) const
{
    auto const it_type = _M_objects.find (type);

    if (it_type != _M_objects.end ())
    {
        auto const it_key = _M_objects[type].find (key);

        if (it_key != _M_objects[type].end ())
        {
            return it_key->second;
        }
    }

    return generic_object_ptr ();
}

factory::generic_array_ptr factory::get_array (size_type type, size_type key) const
{
    auto const it_type = _M_arrays.find (type);

    if (it_type != _M_arrays.end ())
    {
        auto const it_key = _M_arrays[type].find (key);

        if (it_key != _M_arrays[type].end ())
        {
            return it_key->second;
        }
    }

    return generic_array_ptr ();
}

factory::string_type factory::file_name_of_doc_no_ext (size_type key) const
{
    auto const end = (*this)[key].first.find_last_of ('.');

    return end != string_type::npos ? (*this)[key].first.substr (0, end) : (*this)[key].first;
}

factory::string_type factory::file_name_of_doc_ext(size_type key) const
{
    auto const start = (*this)[key].first.find_last_of ('.');

    return start != string_type::npos ? (*this)[key].first.substr (start) : string_type ();
}

bool factory::has_file_name_doc_ext (size_type key) const
{
    auto const end = (*this)[key].first.find_last_of ('.');

    return end != string_type::npos;
}

} } // namespace Json
