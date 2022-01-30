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

Parser::Parser(string_type const& file_path, Type type)
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
        if (type == Type::Array)
        {
            SetArray();
            _M_type = Type::Array;
        }
        else
        {
            SetObject();
            _M_type = Type::Object;
        }
    }

    if (!IsArray() && !IsObject()) _M_type = Type::Null;
}

Parser::Parser(std::istream& data, Type type)
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
        if (type == Type::Array)
        {
            SetArray();
            _M_type = Type::Array;
        }
        else
        {
            SetObject();
            _M_type = Type::Object;
        }
    }

    if (!IsArray() && !IsObject()) _M_type = Type::Null;
}


bool Parser::createDocument(string_type const& file_path, Type type)
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
        if (type == Type::Array)
        {
            SetArray();
            _M_type = Type::Array;
        }
        else
        {
            SetObject();
            _M_type = Type::Object;
        }
    }

    if (!IsArray() && !IsObject()) _M_type = Type::Null;

    changed();

    return true;
}

bool Parser::save(std::ostream& stream)
{
    aboutToSave();

    OStreamWrapper wrapper(stream);
    PrettyWriter<OStreamWrapper> writer(wrapper);

    return base_type::Accept(writer);
}

Parser::Type Parser::type() const
{
    return _M_type;
}

Parser::size_type Parser::size() const
{
    return type() == Type::Array ? Size() :
                                   type() == Type::Object ? MemberCount() :
                                                            Parser::size_type();
}

Parser::const_reference Parser::operator [] (string_type const& key) const
{
    assert(type() == Type::Object);
    return base_type::operator[](key.c_str());
}

Parser::reference Parser::operator [] (string_type const& key)
{
    assert(type() == Type::Object);
    return base_type::operator[](key.c_str());
}

Parser::const_reference Parser::operator [] (size_type idx) const
{
    assert(type() == Type::Array);
    return base_type::operator[](idx);
}

Parser::reference Parser::operator [] (size_type idx)
{
    assert(type() == Type::Array);
    return base_type::operator[](idx);
}

// ======================================================================

TemplateObject::TemplateObject(Parser& _parser, string_type const& _category)
: _M_owner(),
  _M_parser(&_parser),
  _M_ref(),
  _M_category(_category),
  _M_index(npos)
{
    assignFromParserObject();
    connections();
}

TemplateObject::TemplateObject(Parser& _parser, size_type _index)
: _M_owner(),
  _M_parser(&_parser),
  _M_ref(),
  _M_category(),
  _M_index(_index)
{
    assignFromParserArray();
    connections();
}

TemplateObject::TemplateObject(TemplateObject* _owner, string_type const& _category)
: _M_owner(*_owner),
  _M_parser(_owner->parser()),
  _M_ref(),
  _M_category(_category),
  _M_index(npos)
{
    assignFromObject();
    connections();
}

TemplateObject::TemplateObject(TemplateArray* _owner, size_type _idx)
: _M_owner(*_owner),
  _M_parser(_owner->parser()),
  _M_ref(),
  _M_category(),
  _M_index(_idx)
{
    assignFromArray();
    connections();
}

TemplateObject::TemplateObject(TemplateObject const& obj)
: _M_owner(obj._M_owner),
  _M_parser(obj._M_parser),
  _M_ref(obj._M_ref),
  _M_category(obj._M_category),
  _M_index(obj._M_index)
{
    connections();
}

TemplateObject::TemplateObject(TemplateObject&& obj)
: _M_owner(obj._M_owner),
  _M_parser(obj._M_parser),
  _M_ref(obj._M_ref),
  _M_category(obj._M_category),
  _M_index(obj._M_index)
{
    obj.invalidate();
    connections();
}

TemplateObject& TemplateObject::operator = (TemplateObject const& obj)
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

TemplateObject& TemplateObject::operator = (TemplateObject&& obj)
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

TemplateObject::~TemplateObject()
{
    disconnections();
}

TemplateObject::const_reference TemplateObject::operator [] (string_type const& key) const
{
    assert(_M_ref != nullptr);
    return _M_ref->operator[](key.c_str());
}

TemplateObject::reference TemplateObject::operator [] (string_type const& key)
{
    assert(_M_ref != nullptr);
    return _M_ref->operator[](key.c_str());
}

void TemplateObject::connections()
{
    if (!_M_owner)
    {
        connect(_M_parser->changed, *this, &TemplateObject::onParserDataChanged);
        connect(_M_parser->aboutToSave, *this, &TemplateObject::onAboutToSave);
    }
    else if (_M_owner.type() == Parser::Type::Object)
    {
        connect(_M_owner.object()->changed, *this, &TemplateObject::onObjectChanged);
        connect(_M_owner.object()->aboutToSave, *this, &TemplateObject::onAboutToSave);
    }
    else if (_M_owner.type() == Parser::Type::Array)
    {
        connect(_M_owner.array()->changed, *this, &TemplateObject::onArrayChanged);
        connect(_M_owner.array()->removed, *this, &TemplateObject::onRemoved);
        connect(_M_owner.array()->aboutToSave, *this, &TemplateObject::onAboutToSave);
    }
}

void TemplateObject::disconnections()
{
    if (!_M_owner)
    {
        disconnect(_M_parser->changed, *this, &TemplateObject::onParserDataChanged);
        disconnect(_M_parser->aboutToSave, *this, &TemplateObject::onAboutToSave);
    }
    else if (_M_owner.type() == Parser::Type::Object)
    {
        disconnect(_M_owner.object()->changed, *this, &TemplateObject::onObjectChanged);
        disconnect(_M_owner.object()->aboutToSave, *this, &TemplateObject::onAboutToSave);
    }
    else if (_M_owner.type() == Parser::Type::Array)
    {
        disconnect(_M_owner.array()->changed, *this, &TemplateObject::onArrayChanged);
        disconnect(_M_owner.array()->removed, *this, &TemplateObject::onRemoved);
        disconnect(_M_owner.array()->aboutToSave, *this, &TemplateObject::onAboutToSave);
    }
}

void TemplateObject::invalidate()
{
    if (!(*this)) return;

    disconnections();

    _M_ref      = nullptr;
    _M_category = string_type();
    _M_index    = npos;
    _M_owner    = TemplateOwner();

    invalidated();
}

void TemplateObject::assignFromParserObject()
{
    assert(_M_category.size() > 0);
    assert(_M_parser->type() == Parser::Type::Object);

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

void TemplateObject::assignFromParserArray()
{
    assert(_M_parser->type() == Parser::Type::Array);

    if (_M_parser->size() <= _M_index)
    {
        std::cout << "Json Array index " << _M_index << " doesn't "
                  << "exist. Appending new Object with index " << _M_parser->size()
                  << "." << std::endl;

        _M_parser->PushBack(value_type(kObjectType), _M_parser->GetAllocator());

        _M_index =  _M_parser->size() - 1;
    }

    _M_ref = &(*_M_parser)[_M_index];

    if (!_M_ref->IsObject()) _M_ref->SetObject();
    assert(_M_ref->IsObject());
}

void TemplateObject::assignFromObject()
{
    assert(_M_category.size() > 0);

    if (!parentObjectRef()->HasMember(_M_category.c_str()))
    {
        std::cout << "Json Object category " << _M_category << " doesn't "
                  << "exist. Creating a new one." << std::endl;

        parentObjectRef()->AddMember(value_type(_M_category.c_str(),
                                                static_cast<size_type>(_M_category.size()),
                                                _M_parser->GetAllocator()),
                                     value_type(kObjectType),
                                     _M_parser->GetAllocator());
    }

    _M_ref = &(*_M_owner.object())[_M_category];

    if (!_M_ref->IsObject()) _M_ref->SetObject();
    assert(_M_ref->IsObject());
}

void TemplateObject::assignFromArray()
{
    if (parentArrayRef()->Size() <= _M_index)
    {
        std::cout << "Json Array index " << _M_index << " doesn't "
                  << "exist. Appending new Object with index " << parentArrayRef()->Size()
                  << "." << std::endl;

        parentArrayRef()->PushBack(value_type(kObjectType),
                                   _M_parser->GetAllocator());

        _M_index = parentArrayRef()->Size() - 1;
    }

    _M_ref = &(*_M_owner.array())[_M_index];

    if (!_M_ref->IsObject()) _M_ref->SetObject();
    assert(_M_ref->IsObject());
}

TemplateObject::size_type TemplateObject::size() const
{
    assert(_M_ref != nullptr);
    return _M_ref->MemberCount();
}

bool TemplateObject::empty() const
{
    assert(_M_ref != nullptr);
    return _M_ref->MemberCount() == 0;
}

TemplateObject::pointer TemplateObject::parentObjectRef() const
{
    assert(_M_owner.object()->_M_ref != nullptr);
    return _M_owner.object()->_M_ref;
}

TemplateObject::pointer TemplateObject::parentArrayRef() const
{
    assert(_M_owner.array()->_M_ref != nullptr);
    return _M_owner.array()->_M_ref;
}

void TemplateObject::onParserDataChanged()
{
    switch (_M_parser->type())
    {
    case Parser::Type::Array:
        assignFromParserArray();
        break;
    case Parser::Type::Object:
        assignFromParserObject();
        break;
    default:
        break;
    }

    changed();
}

void TemplateObject::onAboutToSave()
{
    aboutToSave();
}

void TemplateObject::onObjectChanged()
{
    if (!(*_M_owner.object())) invalidate();
    else
    {
        assignFromObject();
        changed();
    }
}

void TemplateObject::onArrayChanged()
{
    if (!(*_M_owner.array())) invalidate();
    else
    {
        assignFromArray();
        changed();
    }
}

void TemplateObject::onRemoved(size_type idx)
{
    if (_M_index == idx) invalidate();
}

// ======================================================================

TemplateArray::TemplateArray(Parser& _parser, string_type const& _category)
: _M_owner(),
  _M_parser(&_parser),
  _M_ref(),
  _M_category(_category),
  _M_index(npos)
{
    assignFromParserObject();
    connections();
}

TemplateArray::TemplateArray(Parser& _parser, size_type _index)
: _M_owner(),
  _M_parser(&_parser),
  _M_ref(),
  _M_category(),
  _M_index(_index)
{
    assignFromParserArray();
    connections();
}

TemplateArray::TemplateArray(TemplateObject* _owner, string_type const& _category)
: _M_owner(*_owner),
  _M_parser(_owner->parser()),
  _M_ref(),
  _M_category(_category),
  _M_index(npos)
{
    assignFromObject();
    connections();
}

TemplateArray::TemplateArray(TemplateArray* _owner, size_type _idx)
: _M_owner(*_owner),
  _M_parser(_owner->parser()),
  _M_ref(),
  _M_category(),
  _M_index(_idx)
{
    assignFromArray();
    connections();
}

TemplateArray::TemplateArray(TemplateArray const& obj)
: _M_owner(obj._M_owner),
  _M_parser(obj._M_parser),
  _M_ref(obj._M_ref),
  _M_category(obj._M_category),
  _M_index(obj._M_index)

{
    connections();
}

TemplateArray::TemplateArray(TemplateArray&& obj)
: _M_owner(obj._M_owner),
  _M_parser(obj._M_parser),
  _M_ref(obj._M_ref),
  _M_category(obj._M_category),
  _M_index(obj._M_index)
{
    obj.invalidate();
    connections();
}

TemplateArray& TemplateArray::operator = (TemplateArray const& obj)
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

TemplateArray& TemplateArray::operator = (TemplateArray&& obj)
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

TemplateArray::~TemplateArray()
{
    disconnections();
}

TemplateArray::const_reference TemplateArray::operator [] (size_type idx) const
{
    assert(_M_ref != nullptr);
    return _M_ref->operator[](idx);
}

TemplateArray::reference TemplateArray::operator [] (size_type idx)
{
    assert(_M_ref != nullptr);
    return _M_ref->operator[](idx);
}

void TemplateArray::connections()
{
    if (!_M_owner)
    {
        connect(_M_parser->changed, *this, &TemplateArray::onParserDataChanged);
        connect(_M_parser->aboutToSave, *this, &TemplateArray::onAboutToSave);
    }
    else if (_M_owner.type() == Parser::Type::Object)
    {
        connect(_M_owner.object()->changed, *this, &TemplateArray::onObjectChanged);
        connect(_M_owner.object()->aboutToSave, *this, &TemplateArray::onAboutToSave);
    }
    else if (_M_owner.type() == Parser::Type::Array)
    {
        connect(_M_owner.array()->changed, *this, &TemplateArray::onArrayChanged);
        connect(_M_owner.array()->removed, *this, &TemplateArray::onRemoved);
        connect(_M_owner.array()->aboutToSave, *this, &TemplateArray::onAboutToSave);
    }
}

void TemplateArray::disconnections()
{
    if (!_M_owner)
    {
        disconnect(_M_parser->changed, *this, &TemplateArray::onParserDataChanged);
        disconnect(_M_parser->aboutToSave, *this, &TemplateArray::onAboutToSave);
    }
    else if (_M_owner.type() == Parser::Type::Object)
    {
        disconnect(_M_owner.object()->changed, *this, &TemplateArray::onObjectChanged);
        disconnect(_M_owner.object()->aboutToSave, *this, &TemplateArray::onAboutToSave);
    }
    else if (_M_owner.type() == Parser::Type::Array)
    {
        disconnect(_M_owner.array()->changed, *this, &TemplateArray::onArrayChanged);
        disconnect(_M_owner.array()->removed, *this, &TemplateArray::onRemoved);
        disconnect(_M_owner.array()->aboutToSave, *this, &TemplateArray::onAboutToSave);
    }
}

void TemplateArray::invalidate()
{
    if (!(*this)) return;

    disconnections();

    _M_ref      = nullptr;
    _M_category = string_type();
    _M_index    = npos;
    _M_owner    = TemplateOwner();

    invalidated();
}

void TemplateArray::assignFromParserObject()
{
    assert(_M_category.size() > 0);
    assert(_M_parser->type() == Parser::Type::Object);

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

void TemplateArray::assignFromParserArray()
{
    assert(_M_parser->type() == Parser::Type::Array);

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

void TemplateArray::assignFromObject()
{
    assert(_M_category.size() > 0);

    if (!parentObjectRef()->HasMember(_M_category.c_str()))
    {
        std::cout << "Json Array category " << _M_category << " doesn't "
                  << "exist. Creating a new one." << std::endl;

        parentObjectRef()->AddMember(value_type(_M_category.c_str(),
                                                static_cast<size_type>(_M_category.size()),
                                                _M_parser->GetAllocator()),
                                     value_type(kArrayType),
                                     _M_parser->GetAllocator());
    }

    _M_ref = &(*_M_owner.object())[_M_category];

    if (!_M_ref->IsArray()) _M_ref->SetArray();
    assert(_M_ref->IsArray());
}

void TemplateArray::assignFromArray()
{
    if (parentArrayRef()->Size() <= _M_index)
    {
        std::cout << "Json Array index " << _M_index << " doesn't "
                  << "exist. Appending new Array with index " << parentArrayRef()->Size()
                  << "." << std::endl;

        parentArrayRef()->PushBack(value_type(kArrayType),
                                   _M_parser->GetAllocator());

        _M_index =  parentArrayRef()->Size() - 1;
    }

    _M_ref = &(*_M_owner.array())[_M_index];

    if (!_M_ref->IsArray()) _M_ref->SetArray();
    assert(_M_ref->IsArray());
}

TemplateArray::size_type TemplateArray::size() const
{
    assert(_M_ref != nullptr);
    return _M_ref->Size();
}

bool TemplateArray::empty() const
{
    assert(_M_ref != nullptr);
    return _M_ref->Empty();
}

void TemplateArray::append()
{
    assert(_M_ref != nullptr);

    aboutToAppend();
    _M_ref->PushBack(value_type(kNullType), _M_parser->GetAllocator());
    appended();
}

void TemplateArray::remove(size_type idx)
{
    assert(_M_ref != nullptr);
    assert(_M_ref->Size() > idx);

    aboutToRemove(idx);
    _M_ref->Erase(&(*this)[idx]);
    removed(idx);
}

void TemplateArray::clear()
{
    if (empty()) return;

    aboutToClear();
    _M_ref->Clear();
    cleared();
}

TemplateArray::pointer TemplateArray::parentObjectRef() const
{
    assert(_M_owner.object()->_M_ref != nullptr);
    return _M_owner.object()->_M_ref;
}

TemplateArray::pointer TemplateArray::parentArrayRef() const
{
    assert(_M_owner.array()->_M_ref != nullptr);
    return _M_owner.array()->_M_ref;
}

void TemplateArray::onParserDataChanged()
{
    switch (_M_parser->type())
    {
    case Parser::Type::Array:
        assignFromParserArray();
        break;
    case Parser::Type::Object:
        assignFromParserObject();
        break;
    default:
        break;
    }

    changed();
}

void TemplateArray::onAboutToSave()
{
    aboutToSave();
}

void TemplateArray::onObjectChanged()
{
    if (!(*_M_owner.object())) invalidate();
    else
    {
        assignFromObject();
        changed();
    }
}

void TemplateArray::onArrayChanged()
{
    if (!(*_M_owner.array())) invalidate();
    else
    {
        assignFromArray();
        changed();
    }
}

void TemplateArray::onRemoved(size_type idx)
{
    if (_M_index == idx) invalidate();
}

// ======================================================================

ReferenceBase<Parser::string_type>::ReferenceBase (TemplateObject*    owner,
                                                   string_type const& key,
                                                   value_type  const& default_val)
: _M_ref(),
  _M_owner(*owner)
{
    _M_fn = func_type([def_key = key, def = default_val](self_type* ptr)
    {
        if(def_key.empty()) return;

        if (!(*ptr->_M_owner.object())->HasMember(def_key.c_str()))
        {
            std::cout << "Json::" << __func__ << ": invalid reference "
                      << def_key  << " with default value: "
                      << def      << ". Trying to create a new one." << std::endl;

            //! try to obtain a valid reference
            (*ptr->_M_owner.object())->AddMember(json_type(def_key.c_str(),
                                                 static_cast<size_type>(def_key.size()),
                                                 ptr->_M_owner.object()->parser()->GetAllocator()).Move(),
                                                 json_type(def.c_str(),
                                                 static_cast<size_type>(def.size()),
                                                 ptr->_M_owner.object()->parser()->GetAllocator()).Move(),
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

ReferenceBase<Parser::string_type>::ReferenceBase (TemplateArray* owner,
                                                   size_type idx,
                                                   value_type const& default_val)
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
            (*ptr->_M_owner.array())->PushBack(json_type(def.c_str(),
                                               static_cast<size_type>(def.size()),
                                               ptr->_M_owner.array()->parser()->GetAllocator()).Move(),
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

void cppual::json::ReferenceBase<Parser::string_type>::connections()
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

void cppual::json::ReferenceBase<Parser::string_type>::disconnections()
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

void cppual::json::ReferenceBase<Parser::string_type>::assign()
{
    if(_M_fn != nullptr) _M_fn(this);
    changed();
}

// ======================================================================

Reference<bool>::Reference(TemplateObject*    owner,
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

Reference<bool>::Reference(TemplateArray*    owner,
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

Reference<u16>::Reference(TemplateObject*    owner,
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

Reference<u16>::Reference(TemplateArray* owner,
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

Reference<i16>::Reference (TemplateObject*     owner,
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

Reference<i16>::Reference (TemplateArray* owner,
                             size_type      idx,
                             value_type     default_val)
: reference_base(owner, idx, static_cast<default_type>(default_val))
{
    if (!ref()->IsInt())
    {
        std::cerr << "json reference value with index " << idx << " is not a int! type: "
                  << type() << std::endl;
    }
}

// ======================================================================

Reference<uint>::Reference (TemplateObject*    owner,
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

Reference<uint>::Reference (TemplateArray* owner,
                            size_type      idx,
                            value_type     default_val)
: reference_base(owner, idx, default_val)
{
    if (!ref()->IsUint())
    {
        std::cerr << "json reference value with index " << idx << " is not an uint! type: "
                  << type() << std::endl;
    }
}

// ======================================================================

Reference<int>::Reference (TemplateObject*    owner,
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

Reference<int>::Reference (TemplateArray*    owner,
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

Reference<u64>::Reference (TemplateObject*    owner,
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

Reference<u64>::Reference (TemplateArray*    owner,
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

Reference<i64>::Reference (TemplateObject*    owner,
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

Reference<i64>::Reference (TemplateArray*    owner,
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

Reference<float>::Reference (TemplateObject*    owner,
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

Reference<float>::Reference (TemplateArray*    owner,
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

Reference<double>::Reference (TemplateObject*    owner,
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

Reference<double>::Reference (TemplateArray*    owner,
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

Reference<Parser::string_type>::Reference (TemplateObject*    owner,
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

Reference<Parser::string_type>::Reference (TemplateArray*    owner,
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

Factory::Factory(std::initializer_list<file_tuple> json_files)
{
    _M_jsonDocs.reserve(json_files.size());

    for (const auto& item : json_files)
    {
        // create if doesn't exit
        {
            std::fstream file;

            file.open(std::get<1>(item).c_str(), std::ios::out | std::ios::app);
        }

        _M_jsonDocs.emplace(std::get<0>(item), std::make_pair(std::get<1>(item),
                                                              json_ptr(new Parser(std::get<1>(item),
                                                                                  std::get<2>(item)))));
    }
}

void Factory::reset()
{
    aboutToReset();

    /*for (auto& item : _M_objects)
    {
        item.second.clear();
    }

    for (auto& item : _M_arrays)
    {
        item.second.clear();
    }*/

    for (auto& item : _M_jsonDocs)
    {
        item.second.second->createDocument(item.second.first);
    }

    afterReset();
}

void Factory::reset(size_type key)
{
    aboutToResetDoc(key);

    auto it_obj_type   = _M_objects.find(key);
    auto it_array_type = _M_arrays.find(key);

    if (it_obj_type != _M_objects.end())
    {
        it_obj_type->second.clear();
    }

    if (it_array_type != _M_arrays.end())
    {
        it_array_type->second.clear();
    }

    for (auto& item : _M_jsonDocs)
    {
        item.second.second->createDocument(item.second.first);
    }

    afterResetDoc(key);
}

bool Factory::save(size_type key)
{
    auto json = _M_jsonDocs.find(key);

    if (json != _M_jsonDocs.end())
    {
        std::ofstream file;

        file.open(json->second.first.c_str(), std::ios::out | std::ios::trunc);

        return json->second.second->save(file);
    }

    return false;
}

void Factory::save()
{
    for (auto& json : _M_jsonDocs)
    {
        std::ofstream file;

        file.open(json.second.first.c_str(), std::ios::out | std::ios::trunc);

        json.second.second->save(file);
    }
}

Factory::generic_object_ptr Factory::getObject(size_type type, size_type key) const
{
    auto const it_type = _M_objects.find(type);

    if (it_type != _M_objects.end())
    {
        auto const it_key = _M_objects[type].find(key);

        if (it_key != _M_objects[type].end())
        {
            return it_key->second;
        }
    }

    return generic_object_ptr();
}

Factory::generic_array_ptr Factory::getArray(size_type type, size_type key) const
{
    auto const it_type = _M_arrays.find(type);

    if (it_type != _M_arrays.end())
    {
        auto const it_key = _M_arrays[type].find(key);

        if (it_key != _M_arrays[type].end())
        {
            return it_key->second;
        }
    }

    return generic_array_ptr();
}

} } // namespace Json
