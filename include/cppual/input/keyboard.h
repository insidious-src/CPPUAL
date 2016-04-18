/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2016 insidious
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

#ifndef CPPUAL_DEVICES_KEYBOARD_H_
#define CPPUAL_DEVICES_KEYBOARD_H_
#ifdef __cplusplus

#include <cppual/types.h>
#include <cppual/noncopyable.h>

namespace cppual { namespace Input {

struct Keyboard;

// =========================================================

class KeyEvent
{
public:
    typedef u8  size_type;
    typedef u8  key_type;
    typedef u16 mask_type;

    enum Type
    {
        Release,
        Press
    };

    struct Data
    {
        size_type id;
        key_type  key;
        mask_type mask;
    };

    inline KeyEvent () noexcept = default;
    constexpr Data const& data  () const noexcept { return m_gData;  }
    constexpr Type        state () const noexcept { return m_eState; }

    constexpr KeyEvent (size_type uId, key_type uKey, mask_type uMask, Type eType) noexcept
    : m_gData { uId, uKey, uMask },
      m_eState (eType)
    { }

private:
    Data m_gData;
    Type m_eState;
};

// =========================================================

struct KeyPressEvent : public KeyEvent
{
    inline KeyPressEvent () noexcept = default;

    constexpr KeyPressEvent (size_type uId, key_type uKey, mask_type uMask) noexcept
    : KeyEvent (uId, uKey, uMask, Press)
    { }
};

// =========================================================

struct KeyReleaseEvent : public KeyEvent
{
    inline KeyReleaseEvent () noexcept = default;

    constexpr KeyReleaseEvent (size_type uId, key_type uKey, mask_type uMask) noexcept
    : KeyEvent (uId, uKey, uMask, Release)
    { }
};

// =========================================================

class KeyQueue : NonConstructible
{
public:
    typedef KeyEvent event_type;

    static bool pop_front (event_type& next_event, bool wait = false) noexcept;
};

// =========================================================

struct Keyboard
{
    typedef KeyQueue queue_type;

    enum
    {
        MaxCount        = 5,
        KeysAtOnceCount = 3
    };

    enum class Lock : char
    {
        Caps,
        Num,
        Scroll
    };

    enum Code
    {
        UndefinedCode = 0,

        // letters
        CodeA = 4,
        CodeB = 5,
        CodeC = 6,
        CodeD = 7,
        CodeE = 8,
        CodeF = 9,
        CodeG = 10,
        CodeH = 11,
        CodeI = 12,
        CodeJ = 13,
        CodeK = 14,
        CodeL = 15,
        CodeM = 16,
        CodeN = 17,
        CodeO = 18,
        CodeP = 19,
        CodeQ = 20,
        CodeR = 21,
        CodeS = 22,
        CodeT = 23,
        CodeU = 24,
        CodeV = 25,
        CodeW = 26,
        CodeX = 27,
        CodeY = 28,
        CodeZ = 29,

        // digits
        Code1 = 30,
        Code2 = 31,
        Code3 = 32,
        Code4 = 33,
        Code5 = 34,
        Code6 = 35,
        Code7 = 36,
        Code8 = 37,
        Code9 = 38,
        Code0 = 39,

        // navigational keys
        CodeReturn = 40,
        CodeEscape = 41,
        CodeBackSpace = 42,
        CodeTab = 43,
        CodeSpace = 44,

        // code keys
        CodeMinus = 45,
        CodeEqual = 46,
        CodeLBracket = 47,
        CodeRBracket = 48,
        CodeBackSlash = 49,
        CodeNonuSlash = 50,
        CodeSemiColon = 51,
        CodeApostrophe = 52,
        CodeGrave = 53,
        CodeComma = 54,
        CodePeriod = 55,
        CodeSlash = 56,
        CodeCapsLock = 57,

        // F keys
        CodeF1 = 58,
        CodeF2 = 59,
        CodeF3 = 60,
        CodeF4 = 61,
        CodeF5 = 62,
        CodeF6 = 63,
        CodeF7 = 64,
        CodeF8 = 65,
        CodeF9 = 66,
        CodeF10 = 67,
        CodeF11 = 68,
        CodeF12 = 69,

        // screen
        CodePause = 72,
        CodeInsert = 73,
        CodeHome = 74,
        CodePageUp = 75,
        CodeDelete = 76,
        CodeEnd = 77,
        CodePageDown = 78,
        CodeRight = 79,
        CodeLeft = 80,
        CodeDown = 81,
        CodeUp = 82,

        // locks
        CodePrintScr = 70,
        CodeScrollLock = 71,
        CodeNumLockClear = 83,

        // normal numpad
        CodeNumpadDivide = 84,
        CodeNumpadMultiply = 85,
        CodeNumpadMinus = 86,
        CodeNumpadPlus = 87,
        CodeNumpadEnter = 88,
        CodeNumpad1 = 89,
        CodeNumpad2 = 90,
        CodeNumpad3 = 91,
        CodeNumpad4 = 92,
        CodeNumpad5 = 93,
        CodeNumpad6 = 94,
        CodeNumpad7 = 95,
        CodeNumpad8 = 96,
        CodeNumpad9 = 97,
        CodeNumpad0 = 98,
        CodeNumpadDot = 99,

        CodeNonusBackSlash = 100,
        CodeApp = 101,
        CodePower = 102,
        CodeNumpadEqual = 103,

        CodeF13 = 104,
        CodeF14 = 105,
        CodeF15 = 106,
        CodeF16 = 107,
        CodeF17 = 108,
        CodeF18 = 109,
        CodeF19 = 110,
        CodeF20 = 111,
        CodeF21 = 112,
        CodeF22 = 113,
        CodeF23 = 114,
        CodeF24 = 115,

        CodeEexecute = 116,
        CodeHelp = 117,
        CodeMenu = 118,
        CodeSelect = 119,
        CodeStop = 120,
        CodeAgain = 121,
        CodeUndo = 122,
        CodeCut = 123,
        CodeCopy = 124,
        CodePaste = 125,
        CodeFind = 126,
        CodeMute = 127,
        CodeVolumeUp = 128,
        CodeVolumeDown = 129,
    /* not sure whether there's a reason to enable these */
    /*     CodeLOCKINGCAPSLOCK = 130,  */
    /*     CodeLOCKINGNUMLOCK = 131, */
    /*     CodeLOCKINGSCROLLLOCK = 132, */
        CodeNumpadComma = 133,
        CodeNumpadEqualAS400 = 134,

        CodeInt1 = 135,
        CodeInt2 = 136,
        CodeInt3 = 137,
        CodeInt4 = 138,
        CodeInt5 = 139,
        CodeInt6 = 140,
        CodeInt7 = 141,
        CodeInt8 = 142,
        CodeInt9 = 143,
        CodeLang1 = 144,
        CodeLang2 = 145,
        CodeLang3 = 146,
        CodeLang4 = 147,
        CodeLang5 = 148,
        CodeLang6 = 149,
        CodeLang7 = 150,
        CodeLang8 = 151,
        CodeLang9 = 152,

        CodeALTERASE = 153,
        CodeSYSREQ = 154,
        CodeCANCEL = 155,
        CodeCLEAR = 156,
        CodePRIOR = 157,
        CodeRETURN2 = 158,
        CodeSEPARATOR = 159,
        CodeOUT = 160,
        CodeOPER = 161,
        CodeCLEARAGAIN = 162,
        CodeCRSEL = 163,
        CodeEXSEL = 164,

        // numpad extra
        CodeNumpad00 = 176,
        CodeNumpad000 = 177,

        // extra symbols
        CodeTHOUSANDSSEPARATOR = 178,
        CodeDECIMALSEPARATOR = 179,
        CodeCURRENCYUNIT = 180,
        CodeCURRENCYSUBUNIT = 181,

        // extended numpad
        CodeNumpadLEFTPAREN = 182,
        CodeNumpadRIGHTPAREN = 183,
        CodeNumpadLEFTBRACE = 184,
        CodeNumpadRIGHTBRACE = 185,
        CodeNumpadTAB = 186,
        CodeNumpadBACKSPACE = 187,
        CodeNumpadA = 188,
        CodeNumpadB = 189,
        CodeNumpadC = 190,
        CodeNumpadD = 191,
        CodeNumpadE = 192,
        CodeNumpadF = 193,
        CodeNumpadXOR = 194,
        CodeNumpadPOWER = 195,
        CodeNumpadPERCENT = 196,
        CodeNumpadLESS = 197,
        CodeNumpadGREATER = 198,
        CodeNumpadAMPERSAND = 199,
        CodeNumpadDBLAMPERSAND = 200,
        CodeNumpadVERTICALBAR = 201,
        CodeNumpadDBLVERTICALBAR = 202,
        CodeNumpadCOLON = 203,
        CodeNumpadHASH = 204,
        CodeNumpadSPACE = 205,
        CodeNumpadAT = 206,
        CodeNumpadEXCLAM = 207,
        CodeNumpadMEMSTORE = 208,
        CodeNumpadMEMRECALL = 209,
        CodeNumpadMEMCLEAR = 210,
        CodeNumpadMEMADD = 211,
        CodeNumpadMEMSUBTRACT = 212,
        CodeNumpadMEMMULTIPLY = 213,
        CodeNumpadMEMDIVIDE = 214,
        CodeNumpadPLUSMINUS = 215,
        CodeNumpadCLEAR = 216,
        CodeNumpadCLEARENTRY = 217,
        CodeNumpadBINARY = 218,
        CodeNumpadOCTAL = 219,
        CodeNumpadDECIMAL = 220,
        CodeNumpadHEXADECIMAL = 221,

        // modifiers
        CodeLCtrl = 224,
        CodeLShift = 225,
        CodeLAlt = 226,
        CodeLGUI = 227,
        CodeRCtrl = 228,
        CodeRShift = 229,
        CodeRAlt = 230,
        CodeRGUI = 231,
        CodeMODE = 257,

        // media
        CodeAUDIONEXT = 258,
        CodeAUDIOPREV = 259,
        CodeAUDIOSTOP = 260,
        CodeAUDIOPLAY = 261,
        CodeAUDIOMUTE = 262,
        CodeMEDIASELECT = 263,
        CodeWWW = 264,
        CodeMAIL = 265,
        CodeCALCULATOR = 266,
        CodeCOMPUTER = 267,
        CodeAC_SEARCH = 268,
        CodeAC_HOME = 269,
        CodeAC_BACK = 270,
        CodeAC_FORWARD = 271,
        CodeAC_STOP = 272,
        CodeAC_REFRESH = 273,
        CodeAC_BOOKMARKS = 274,

        CodeBRIGHTNESSDOWN = 275,
        CodeBRIGHTNESSUP = 276,
        CodeDISPLAYSWITCH = 277,

        CodeKBDILLUMTOGGLE = 278,
        CodeKBDILLUMDOWN = 279,
        CodeKBDILLUMUP = 280,
        CodeEJECT = 281,
        CodeSLEEP = 282,

        CodeAPP1 = 283,
        CodeAPP2 = 284,

        CodeCount = 512
    };

    enum Key
    {
        UndefinedKey = 0,
        Num0 = '0',
        Num1 = '1',
        Num2 = '2',
        Num3 = '3',
        Num4 = '4',
        Num5 = '5',
        Num6 = '6',
        Num7 = '7',
        Num8 = '8',
        Num9 = '9',
        A = 'A',
        B = 'B',
        C = 'C',
        D = 'D',
        E = 'E',
        F = 'F',
        G = 'G',
        H = 'H',
        I = 'I',
        J = 'J',
        K = 'K',
        L = 'L',
        M = 'M',
        N = 'N',
        O = 'O',
        P = 'P',
        Q = 'Q',
        R = 'R',
        S = 'S',
        T = 'T',
        U = 'U',
        V = 'V',
        W = 'W',
        X = 'X',
        Y = 'Y',
        Z = 'Z',
        Escape = '\033',
        LControl = (CodeLCtrl | 1<<30),
        LShift = (CodeLShift | 1<<30),
        LAlt = (CodeLShift | 1<<30),
        LSystem,
        RControl = (CodeLShift | 1<<30),
        RShift = (CodeLShift | 1<<30),
        RAlt = (CodeLShift | 1<<30),
        RSystem,
        Menu = (CodeMenu | 1<<30),
        LBracket = '[',
        RBracket = ']',
        LParen = '(',
        RParen = ')',
        SemiColon = ';',
        Color = ':',
        Comma = ',',
        Dot = '.',
        Quote = '\'',
        DoubleQuote = '"',
        Slash = '/',
        BackSlash = '\\',
        Tilde = '~',
        Separator = '|',
        Equal = '=',
        Greater = '>',
        Less = '<',
        Dash = '-',
        Space = ' ',
        Return = '\r',
        BackSpace = '\b',
        Tab = '\t',
        PageUp = (CodePageUp | 1<<30),
        PageDown = (CodePageDown | 1<<30),
        End = (CodeEnd | 1<<30),
        Home = (CodeHome | 1<<30),
        Insert = (CodeInsert | 1<<30),
        Delete = '\177',
        Plus = '+',
        Asterisk = '*',
        At = '@',
        Exclamation = '!',
        Question = '?',
        Underscore = '_',
        Caret = '^',
        BackQuote = '`',
        Dollar = '$',
        Hash = '#',
        Ampersand = '&',
        Percent = '%',
        Left = (CodeLeft | 1<<30),
        Right = (CodeRight | 1<<30),
        Up = (CodeUp | 1<<30),
        Down = (CodeDown | 1<<30),
        Numpad0 = (CodeNumpad0 | 1<<30),
        Numpad1 = (CodeNumpad1 | 1<<30),
        Numpad2 = (CodeNumpad2 | 1<<30),
        Numpad3 = (CodeNumpad3 | 1<<30),
        Numpad4 = (CodeNumpad4 | 1<<30),
        Numpad5 = (CodeNumpad5 | 1<<30),
        Numpad6 = (CodeNumpad6 | 1<<30),
        Numpad7 = (CodeNumpad7 | 1<<30),
        Numpad8 = (CodeNumpad8 | 1<<30),
        Numpad9 = (CodeNumpad9 | 1<<30),
        NumpadPlus = (CodeNumpadPlus | 1<<30),
        NumpadMinus = (CodeNumpadMinus | 1<<30),
        NumpadMultiply = (CodeNumpadMultiply | 1<<30),
        NumpadDivide = (CodeNumpadDivide | 1<<30),
        NumpadEnter = (CodeNumpadEnter | 1<<30),
        NumpadDot = (CodeNumpadDot | 1<<30),
        NumpadEqual = (CodeNumpadEqual | 1<<30),
        F1 = (CodeF1 | 1<<30),
        F2 = (CodeF2 | 1<<30),
        F3 = (CodeF3 | 1<<30),
        F4 = (CodeF4 | 1<<30),
        F5 = (CodeF5 | 1<<30),
        F6 = (CodeF6 | 1<<30),
        F7 = (CodeF7 | 1<<30),
        F8 = (CodeF8 | 1<<30),
        F9 = (CodeF9 | 1<<30),
        F10 = (CodeF10 | 1<<30),
        F11 = (CodeF11 | 1<<30),
        F12 = (CodeF12 | 1<<30),
        Pause = (CodePause | 1<<30),
        PrintScr = (CodePrintScr | 1<<30),
        CapsLock = (CodeCapsLock | 1<<30),
        NumLock = (CodeNumLockClear | 1<<30),
        ScrollLock = (CodeScrollLock | 1<<30)
    };

    static bool isKeyPressed (int key_code);
    static bool isLockActive (Lock lock_state);
    static bool isConnected  () noexcept;
};

} } // namespace Input

#endif // __cplusplus
#endif // CPPUAL_DEVICES_KEYBOARD_H_
