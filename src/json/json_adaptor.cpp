#include <cppual/json/json_adaptor.h>

#include <cppual/json/filereadstream.h>
#include <cppual/json/filewritestream.h>
#include <cppual/json/istreamwrapper.h>
#include <cppual/json/ostreamwrapper.h>
#include <cppual/json/prettywriter.h>

#include <iostream>
#include <cassert>

namespace cppual { namespace Json {

// ======================================================================

Parser::Parser(string_type const& file_path, Type type)
{
    std::ifstream             file(file_path.c_str());
    rapidjson::IStreamWrapper stream(file);

    ParseStream(stream);

    if (IsNull())
    {
        if (type == Type::Object) SetObject();
        else SetArray();
    }
}

bool Parser::createDocument(string_type const& file_path)
{
    std::ifstream file(file_path.c_str());

    if(!file.is_open()) return false;

    rapidjson::IStreamWrapper stream(file);

    ParseStream(stream);

    changed();

    return true;
}

bool Parser::save(std::ofstream& stream)
{
    if(stream.is_open())
    {
        aboutToSave();

        rapidjson::OStreamWrapper wrapper(stream);
        rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(wrapper);

        base_type::Accept(writer);

        return true;
    }

    return false;
}

Parser::Type Parser::type() const
{
    return GetType() == rapidjson::kObjectType ?
                Type::Object : GetType() == rapidjson::kArrayType ?
                    Type::Array : Type::Null;
}

Parser::size_type Parser::size() const
{
    return Size();
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
  _M_ownerfn(),
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
  _M_ownerfn(),
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
  _M_ownerfn(),
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
  _M_ownerfn(),
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
  _M_ownerfn(obj._M_ownerfn),
  _M_category(obj._M_category),
  _M_index(obj._M_index)
{
    connections();
}

TemplateObject::TemplateObject(TemplateObject&& obj)
: _M_owner(obj._M_owner),
  _M_parser(obj._M_parser),
  _M_ref(obj._M_ref),
  _M_ownerfn(obj._M_ownerfn),
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
    _M_ownerfn  = obj._M_ownerfn;
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
    _M_ownerfn  = obj._M_ownerfn;
    _M_category = obj._M_category;
    _M_index    = obj._M_index;
    _M_owner    = obj._M_owner;

    connections();
    obj.invalidate();

    changed();

    return *this;
}

TemplateObject::~TemplateObject()
{
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
        disconnect(_M_parser->changed, function_type(this, &TemplateObject::onParserDataChanged));
        disconnect(_M_parser->aboutToSave, function_type(this, &TemplateObject::onAboutToSave));
    }
    else if (_M_owner.type() == Parser::Type::Object)
    {
        disconnect(_M_owner.object()->changed, function_type(this, &TemplateObject::onObjectChanged));
        disconnect(_M_owner.object()->aboutToSave, function_type(this, &TemplateObject::onAboutToSave));
    }
    else if (_M_owner.type() == Parser::Type::Array)
    {
        disconnect(_M_owner.array()->changed, function_type(this, &TemplateObject::onArrayChanged));
        disconnect(_M_owner.array()->removed, function_p_type(this, &TemplateObject::onRemoved));
        disconnect(_M_owner.array()->aboutToSave, function_type(this, &TemplateObject::onAboutToSave));
    }
}

void TemplateObject::invalidate()
{
    if (!(*this)) return;

    disconnections();

    _M_parser   = nullptr;
    _M_ref      = nullptr;
    _M_ownerfn  = function_type();
    _M_category = string_type();
    _M_index    = npos;
    _M_owner    = TemplateOwner();

    changed();
}

void TemplateObject::assignFromParserObject()
{
    assert(_M_category.size() > 0);

    if (_M_parser->HasMember(_M_category.c_str()))
    {
        _M_ref = &(*_M_parser)[_M_category];
    }
    else
    {
        std::cout << "Json Object category " << _M_category << " doesn't "
                  << "exist. Creating a new one." << std::endl;

        _M_ref = &_M_parser->AddMember(Parser::StringRefType(_M_category.c_str()),
                                       value_type(rapidjson::kObjectType),
                                       _M_parser->GetAllocator());
    }
}

void TemplateObject::assignFromParserArray()
{
    if (_M_parser->size() > _M_index)
    {
        _M_ref = &(*_M_parser)[_M_index];

        if (_M_ref->IsNull())
        {
            _M_ref = &_M_ref->SetObject();
        }
    }
    else
    {
        std::cout << "Json Array index " << _M_index << " doesn't "
                  << "exist. Appending new Object with index " << _M_parser->size()
                  << "." << std::endl;

        _M_index =  _M_parser->size();
        _M_ref   = &_M_parser->PushBack(value_type(rapidjson::kObjectType), _M_parser->GetAllocator());
    }
}

void TemplateObject::assignFromObject()
{
    assert(_M_category.size() > 0);

    if (parentObjectRef()->HasMember(_M_category.c_str()))
    {
        _M_ref = &(*_M_owner.object())[_M_category];
    }
    else
    {
        std::cout << "Json Object category " << _M_category << " doesn't "
                  << "exist. Creating a new one." << std::endl;

        _M_ref = &parentObjectRef()->AddMember(Parser::StringRefType(_M_category.c_str()),
                                               value_type(rapidjson::kObjectType),
                                               _M_parser->GetAllocator());
    }
}

void TemplateObject::assignFromArray()
{
    if (parentArrayRef()->Size() > _M_index)
    {
        _M_ref = &(*_M_owner.array())[_M_index];

        if (_M_ref->IsNull())
        {
            _M_ref = &_M_ref->SetObject();
        }
    }
    else
    {
        std::cout << "Json Array index " << _M_index << " doesn't "
                  << "exist. Appending new Object with index " << parentArrayRef()->Size()
                  << "." << std::endl;

        _M_index =  parentArrayRef()->Size();
        _M_ref   = &parentArrayRef()->PushBack(value_type(rapidjson::kObjectType),
                                               _M_parser->GetAllocator());
    }
}

TemplateObject::size_type TemplateObject::size() const
{
    assert(_M_ref != nullptr);
    return _M_ref->Size();
}

bool TemplateObject::empty() const
{
    assert(_M_ref != nullptr);
    return _M_ref->Empty();
}

TemplateObject::pointer TemplateObject::parentObjectRef() const
{
    return _M_owner.object()->_M_ref;
}

TemplateObject::pointer TemplateObject::parentArrayRef() const
{
    return _M_owner.array()->_M_ref;
}

void TemplateObject::onParserDataChanged()
{

}

void TemplateObject::onAboutToSave()
{
    aboutToSave();
}

void TemplateObject::onObjectChanged()
{
    if (!(*_M_owner.object()))
    {
        invalidate();
        changed();
    }
}

void TemplateObject::onArrayChanged()
{
    if (!(*_M_owner.array()))
    {
        invalidate();
        changed();
    }
}

void TemplateObject::onRemoved(size_type idx)
{
    if (_M_index == idx) invalidate();

    changed();
}

// ======================================================================

TemplateArray::TemplateArray(Parser& _parser, string_type const& _category)
: _M_owner(),
  _M_parser(&_parser),
  _M_ref(),
  _M_ownerfn(),
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
  _M_ownerfn(),
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
  _M_ownerfn(),
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
  _M_ownerfn(),
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
  _M_ownerfn(obj._M_ownerfn),
  _M_category(obj._M_category),
  _M_index(obj._M_index)

{
    connections();
}

TemplateArray::TemplateArray(TemplateArray&& obj)
: _M_owner(obj._M_owner),
  _M_parser(obj._M_parser),
  _M_ref(obj._M_ref),
  _M_ownerfn(obj._M_ownerfn),
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
    _M_ownerfn  = obj._M_ownerfn;
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
    _M_ownerfn  = obj._M_ownerfn;
    _M_category = obj._M_category;
    _M_index    = obj._M_index;
    _M_owner    = obj._M_owner;

    connections();
    obj.invalidate();

    changed();

    return *this;
}

TemplateArray::~TemplateArray()
{
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
        disconnect(_M_parser->changed, function_type(this, &TemplateArray::onParserDataChanged));
        disconnect(_M_parser->aboutToSave, function_type(this, &TemplateArray::onAboutToSave));
    }
    else if (_M_owner.type() == Parser::Type::Object)
    {
        disconnect(_M_owner.object()->changed, function_type(this, &TemplateArray::onObjectChanged));
        disconnect(_M_owner.object()->aboutToSave, function_type(this, &TemplateArray::onAboutToSave));
    }
    else if (_M_owner.type() == Parser::Type::Array)
    {
        disconnect(_M_owner.array()->changed, function_type(this, &TemplateArray::onArrayChanged));
        disconnect(_M_owner.array()->removed, function_p_type(this, &TemplateArray::onRemoved));
        disconnect(_M_owner.array()->aboutToSave, function_type(this, &TemplateArray::onAboutToSave));
    }
}

void TemplateArray::invalidate()
{
    if (!(*this)) return;

    disconnections();

    _M_parser   = nullptr;
    _M_ref      = nullptr;
    _M_ownerfn  = function_type();
    _M_category = string_type();
    _M_index    = npos;
    _M_owner    = TemplateOwner();

    changed();
}

void TemplateArray::assignFromParserObject()
{
    assert(_M_category.size() > 0);

    if (_M_parser->HasMember(_M_category.c_str()))
    {
        _M_ref = &(*_M_parser)[_M_category];
    }
    else
    {
        std::cout << "Json Array category " << _M_category << " doesn't "
                  << "exist. Creating a new one." << std::endl;

        _M_ref = &_M_parser->AddMember(Parser::StringRefType(_M_category.c_str()),
                                       value_type(rapidjson::kArrayType),
                                       _M_parser->GetAllocator());
    }
}

void TemplateArray::assignFromParserArray()
{
    if (_M_parser->size() > _M_index)
    {
        _M_ref = &(*_M_parser)[_M_index];

        if (_M_ref->IsNull())
        {
            _M_ref = &_M_ref->SetArray();
        }
    }
    else
    {
        std::cout << "Json Array index " << _M_index << " doesn't "
                  << "exist. Appending new Array with index " << _M_parser->size()
                  << "." << std::endl;

        _M_index =  _M_parser->size();
        _M_ref   = &_M_parser->PushBack(value_type(rapidjson::kArrayType), _M_parser->GetAllocator());
    }
}

void TemplateArray::assignFromObject()
{
    assert(_M_category.size() > 0);

    if (parentObjectRef()->HasMember(_M_category.c_str()))
    {
        _M_ref = &(*_M_owner.object())[_M_category];
    }
    else
    {
        std::cout << "Json Array category " << _M_category << " doesn't "
                  << "exist. Creating a new one." << std::endl;

        _M_ref = &parentObjectRef()->AddMember(Parser::StringRefType(_M_category.c_str()),
                                               value_type(rapidjson::kArrayType),
                                               _M_parser->GetAllocator());
    }
}

void TemplateArray::assignFromArray()
{
    if (parentArrayRef()->Size() > _M_index)
    {
        _M_ref = &(*_M_owner.array())[_M_index];

        if (_M_ref->IsNull())
        {
            _M_ref = &_M_ref->SetArray();
        }
    }
    else
    {
        std::cout << "Json Array index " << _M_index << " doesn't "
                  << "exist. Appending new Array with index " << parentArrayRef()->Size()
                  << "." << std::endl;

        _M_index =  parentArrayRef()->Size();
        _M_ref   = &parentArrayRef()->PushBack(value_type(rapidjson::kArrayType),
                                               _M_parser->GetAllocator());
    }
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

TemplateArray::pointer TemplateArray::parentObjectRef() const
{
    return _M_owner.object()->_M_ref;
}

TemplateArray::pointer TemplateArray::parentArrayRef() const
{
    return _M_owner.array()->_M_ref;
}

void TemplateArray::onParserDataChanged()
{

}

void TemplateArray::onAboutToSave()
{
    aboutToSave();
}

void TemplateArray::onObjectChanged()
{
    if (!(*_M_owner.object()))
    {
        invalidate();
        changed();
    }
}

void TemplateArray::onArrayChanged()
{
    if (!(*_M_owner.array()))
    {
        invalidate();
        changed();
    }
}

void TemplateArray::onRemoved(size_type idx)
{
    if (_M_index == idx) invalidate();

    changed();
}

// ======================================================================

Factory::Factory(std::initializer_list<file_pair> json_files)
{
    _M_jsonDocs.reserve(json_files.size());

    for (const auto& item : json_files)
    {
        // create if doesn't exit
        {
            std::fstream file;

            file.open(item.second.c_str(), std::ios::out | std::ios::app);
        }

        _M_jsonDocs.emplace(item.first, std::make_pair(item.second,
                                                       json_ptr(new Parser(item.second))));
    }
}

void Factory::reset()
{
    _M_objects.clear();
    _M_arrays.clear();

    for (auto& item : _M_jsonDocs)
    {
        item.second.second.reset(new Parser(item.second.first));
    }
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

} } // namespace Json
