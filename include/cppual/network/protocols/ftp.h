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

#ifndef CPPUAL_NETWORK_PROTOCOL_FTP_H_
#define CPPUAL_NETWORK_PROTOCOL_FTP_H_
#ifdef __cplusplus

#include <cppual/network/address.h>
#include <cppual/network/transport/tcp.h>
#include <cppual/containers.h>

namespace cppual { namespace network { namespace ftp {

enum class TransferMode : unsigned char
{
    Binary,
    Ascii,
    Ebcdic
};

class Response
{
public:
    enum Status
    {
        // 1xx: the requested action is being initiated,
        // expect another reply before proceeding with a new command
        RestartMarkerReply          = 110,
        ServiceReadySoon            = 120,
        DataConnectionAlreadyOpened = 125,
        OpeningDataConnection       = 150,

        // 2xx: the requested action has been successfully completed
        Ok                    = 200,
        PointlessCommand      = 202,
        SystemStatus          = 211,
        DirectoryStatus       = 212,
        FileStatus            = 213,
        HelpMessage           = 214,
        SystemType            = 215,
        ServiceReady          = 220,
        ClosingConnection     = 221,
        DataConnectionOpened  = 225,
        ClosingDataConnection = 226,
        EnteringPassiveMode   = 227,
        LoggedIn              = 230,
        FileActionOk          = 250,
        DirectoryOk           = 257,

        // 3xx: the command has been accepted, but the requested action
        // is dormant, pending receipt of further information
        NeedPassword       = 331,
        NeedAccountToLogIn = 332,
        NeedInformation    = 350,

        // 4xx: the command was not accepted and the requested action did not take place,
        // but the error condition is temporary and the action may be requested again
        ServiceUnavailable        = 421,
        DataConnectionUnavailable = 425,
        TransferAborted           = 426,
        FileActionAborted         = 450,
        LocalError                = 451,
        InsufficientStorageSpace  = 452,

        // 5xx: the command was not accepted and
        // the requested action did not take place
        CommandUnknown          = 500,
        ParametersUnknown       = 501,
        CommandNotImplemented   = 502,
        BadCommandSequence      = 503,
        ParameterNotImplemented = 504,
        NotLoggedIn             = 530,
        NeedAccountToStore      = 532,
        FileUnavailable         = 550,
        PageTypeUnknown         = 551,
        NotEnoughMemory         = 552,
        FilenameNotAllowed      = 553,

        // 10xx: custom codes
        InvalidResponse  = 1000,
        ConnectionFailed = 1001,
        ConnectionClosed = 1002,
        InvalidFile      = 1003
    };

    bool isValid () const noexcept;

    constexpr string const& message () const noexcept { return _M_gMsg; }
    constexpr u16           status () const noexcept { return _M_uStatus; }

private:
    string _M_gMsg;
    u16    _M_uStatus;
};

class Directory final : public Response
{
public:
    string getDirectory () noexcept;

private:
    string _M_gDir;
};

class Listing final : public Response
{
public:
    dyn_array<string> const& list () noexcept;

private:
    dyn_array<string> _M_gList;
};

// =========================================================

class Client final
{
public:
    Response  connect (address const& server, u16 port = 21) noexcept;
    Response  disconnect () noexcept;
    Response  login () noexcept;
    Response  login (string const& name, string const& password) noexcept;
    Response  keepAlive () noexcept;
    Directory getWorkingDirectory () noexcept;
    Listing   getDirectoryListing (string const& dir = "") noexcept;
    Response  changeDirectory (string const& dir) noexcept;
    Response  parentDirectory () noexcept;
    Response  create (string const& name) noexcept;
    Response  rename (string const& cur_name, string const& new_name) noexcept;
    Response  remove (string const& name) noexcept;

    Response download (string const& file,
                       string const& path,
                       TransferMode  mode = TransferMode::Binary);

    Response upload (string const& file,
                     string const& path,
                     TransferMode  mode = TransferMode::Binary);

private:
    tcp_stream _M_gStream;
};

} } } // Http

#endif // __cplusplus
#endif // CPPUAL_NETWORK_PROTOCOL_FTP_H_
