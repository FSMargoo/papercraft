#include "../include/String.h"

template<>
const char* PStringToCStr(const PString& String)
{
    // Tips:
    //      It is okay to reinterpret_cast the path's C style string here,
    //      since the ostr::text will maintain a UTF16 string chain which
    //      is always available for this cause's string using
    return reinterpret_cast<const char*>(String.raw().c_str());
}

template<>
const wchar_t* PStringToCStr(const PString& String)
{
    const size_t size = String.size() + 1;
    wchar_t* wideChar = new wchar_t[size];
    mbstowcs_s(nullptr, wideChar, size, String.c_str(), size - 1);

    return wideChar;
}