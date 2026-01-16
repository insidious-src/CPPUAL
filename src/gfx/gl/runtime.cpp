/*
 * Product: C++ Unified Abstraction Library
 * Author: K. Petrov
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2024 K. Petrov
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <cppual/gfx/gl/runtime.h>
#include <cppual/gfx/gl/gldef.h>
#include <cppual/containers>
#include <cppual/gfx/draw.h>


#include <algorithm>
#include <stdexcept>
//#include <cmath>

namespace cppual::gfx::gl {

namespace { /// optimize for internal usage

typedef unordered_map<resource_version, resource_version> version_map  ;
typedef dyn_array<string>                                 string_vector;
typedef std::size_t                                       size_type    ;

constexpr GLenum query_to_gl_enum (string_query query) noexcept
{
    return query == string_query::renderer ? InfoRenderer :
           query == string_query::vendor   ? InfoVendor   :
           query == string_query::version  ? InfoVersion  : InfoSLVersion;
}

constexpr resource_version get_gl_version ()
{
    auto major = ::GLint ();
    auto minor = ::GLint ();

    ::glGetIntegerv (InfoMajorVersion, &major);
    ::glGetIntegerv (InfoMinorVersion, &minor);

    return resource_version { major, minor };
}

constexpr version_map get_sl_versions ()
{
    version_map slVersions;

    slVersions.reserve(12);

    slVersions.emplace(resource_version(2, 0), resource_version(1, 10));
    slVersions.emplace(resource_version(2, 1), resource_version(1, 20));
    slVersions.emplace(resource_version(3, 0), resource_version(1, 30));
    slVersions.emplace(resource_version(3, 1), resource_version(1, 40));
    slVersions.emplace(resource_version(3, 2), resource_version(1, 50));
    slVersions.emplace(resource_version(3, 3), resource_version(3, 30));
    slVersions.emplace(resource_version(4, 0), resource_version(4, 00));
    slVersions.emplace(resource_version(4, 1), resource_version(4, 10));
    slVersions.emplace(resource_version(4, 2), resource_version(4, 20));
    slVersions.emplace(resource_version(4, 3), resource_version(4, 30));
    slVersions.emplace(resource_version(4, 4), resource_version(4, 40));
    slVersions.emplace(resource_version(4, 5), resource_version(4, 50));

    return slVersions;
}

constexpr string_vector get_gl_extensions ()
{
    GLint n = GLint ();

    ::glGetIntegerv (GL_NUM_EXTENSIONS, &n);

    string_vector extensions;

    if (n <= 0) return extensions;

    extensions.reserve(static_cast<size_type>(n));

    for (auto i = 0U; i < static_cast<GLuint> (n); ++i)
        extensions.push_back (reinterpret_cast<cchar*> (::glGetStringi (InfoExtensions, i)));

    return extensions;
}

constexpr string_vector get_gl_labels ()
{
    string_vector labels;

    labels.reserve(4);

    labels.push_back (reinterpret_cast<cchar*> (::glGetString(query_to_gl_enum(string_query::renderer))));
    labels.push_back (reinterpret_cast<cchar*> (::glGetString(query_to_gl_enum(string_query::vendor))));
    labels.push_back (reinterpret_cast<cchar*> (::glGetString(query_to_gl_enum(string_query::version))));
    labels.push_back (reinterpret_cast<cchar*> (::glGetString(query_to_gl_enum(string_query::sl_version))));

    return labels;
}

} // anonymous namespace

// ====================================================

platform::version_type platform::version ()
{
    if(!context_interface::current() || context_interface::current()->device() != device_backend::gl)
        throw std::runtime_error("NO OpenGL context is assigned to the current thread!");

    static const auto version = get_gl_version ();
    return version;
}

platform::version_type platform::sl_version ()
{
    if(!context_interface::current() || context_interface::current()->device() != device_backend::gl)
        throw std::runtime_error("NO OpenGL context is assigned to the current thread!");

    static auto versions = get_sl_versions ();
    return versions[version()];
}

platform::string_type platform::label (string_query query)
{
    if(!context_interface::current() || context_interface::current()->device() != device_backend::gl)
        throw std::runtime_error("NO OpenGL context is assigned to the current thread!");

    static const auto labels = get_gl_labels ();

    return labels[static_cast<size_type>(query)];
}

bool platform::is_extension_supported (string_type const& name)
{
    if(!context_interface::current() || context_interface::current()->device() != device_backend::gl)
        throw std::runtime_error("NO OpenGL context is assigned to the current thread!");

    static auto extensions = get_gl_extensions ();
    auto        it         = std::find (extensions.begin(), extensions.end(), name);

    return it != extensions.end();
}

void platform::draw_test_triagle(float fAxis)
{
    ::glClearColor (255.0f, 255.0f, 255.0f, 255.0f);
    ::glClear (GL_COLOR_BUFFER_BIT);

    ::glPushMatrix ();
    ::glRotatef (fAxis, 0.0f, 0.0f, 1.0f);
    ::glBegin (GL_TRIANGLES);

    ::glColor3f (1.0f, 0.0f, 0.0f);
    ::glVertex2f (0.0f, 1.0f);

    ::glColor3f (0.0f, 1.0f, 0.0f);
    ::glVertex2f (0.87f, -0.5f);

    ::glColor3f (0.0f, 0.0f, 1.0f);
    ::glVertex2f (-0.87f, -0.5f);

    ::glEnd ();
    ::glPopMatrix ();
}

} // namespace GL
