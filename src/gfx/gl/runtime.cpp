/*
 * Product: C++ Unified Abstraction Library
 * Author: K. Petrov
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2018 insidious
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

#include "gldef.h"

#include <algorithm>
#include <vector>
#include <cmath>
#include <unordered_map>
#include <stdexcept>

namespace cppual { namespace Graphics { namespace GL {

namespace {

typedef std::unordered_map<Version, Version> version_map  ;
typedef std::vector<string>                  string_vector;
typedef std::size_t                          size_type    ;

constexpr GLenum queryToGLEnum (StringQuery query) noexcept
{
    return query == StringQuery::Renderer ? InfoRenderer :
           query == StringQuery::Vendor   ? InfoVendor   :
           query == StringQuery::Version  ? InfoVersion  : InfoSLVersion;
}

Version getGLVersion ()
{
    GLint major = GLint ();
    GLint minor = GLint ();

    ::glGetIntegerv (InfoMajorVersion, &major);
    ::glGetIntegerv (InfoMinorVersion, &minor);

    return Version { major, minor };
}

version_map getSLVersions ()
{
    version_map slVersions;

    slVersions.reserve(12);
    slVersions.emplace(Version(2, 0), Version(1, 10));
    slVersions.emplace(Version(2, 1), Version(1, 20));
    slVersions.emplace(Version(3, 0), Version(1, 30));
    slVersions.emplace(Version(3, 1), Version(1, 40));
    slVersions.emplace(Version(3, 2), Version(1, 50));
    slVersions.emplace(Version(3, 3), Version(3, 30));
    slVersions.emplace(Version(4, 0), Version(4, 00));
    slVersions.emplace(Version(4, 1), Version(4, 10));
    slVersions.emplace(Version(4, 2), Version(4, 20));
    slVersions.emplace(Version(4, 3), Version(4, 30));
    slVersions.emplace(Version(4, 4), Version(4, 40));
    slVersions.emplace(Version(4, 5), Version(4, 50));

    return slVersions;
}

string_vector getGLExtensions ()
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

string_vector getGLLabels ()
{
    string_vector labels;

    labels.reserve(4);

    labels.push_back (reinterpret_cast<cchar*> (::glGetString(queryToGLEnum(StringQuery::Renderer))));
    labels.push_back (reinterpret_cast<cchar*> (::glGetString(queryToGLEnum(StringQuery::Vendor))));
    labels.push_back (reinterpret_cast<cchar*> (::glGetString(queryToGLEnum(StringQuery::Version))));
    labels.push_back (reinterpret_cast<cchar*> (::glGetString(queryToGLEnum(StringQuery::SLVersion))));

    return labels;
}

} // anonymous namespace

// ====================================================

Version Platform::version ()
{
    if(!IDeviceContext::current() || IDeviceContext::current()->device() != DeviceType::GL)
        throw std::runtime_error("NO OpenGL context is assigned to the current thread!");

    static const auto version = getGLVersion ();
    return version;
}

Version Platform::slVersion ()
{
    if(!IDeviceContext::current() || IDeviceContext::current()->device() != DeviceType::GL)
        throw std::runtime_error("NO OpenGL context is assigned to the current thread!");

    static auto versions = getSLVersions();
    return versions[version()];
}

string Platform::label (StringQuery query)
{
    if(!IDeviceContext::current() || IDeviceContext::current()->device() != DeviceType::GL)
        throw std::runtime_error("NO OpenGL context is assigned to the current thread!");

    static const auto labels = getGLLabels ();

    return labels[static_cast<size_type>(query)];
}

bool Platform::isExtensionSupported (string const& name)
{
    if(!IDeviceContext::current() || IDeviceContext::current()->device() != DeviceType::GL)
        throw std::runtime_error("NO OpenGL context is assigned to the current thread!");

    static auto extensions = getGLExtensions ();
    auto        it         = std::find (extensions.begin(), extensions.end(), name);

    return it != extensions.end();
}

void Platform::drawTestTriagle(float fAxis)
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

} } } // namespace GL
