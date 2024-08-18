///
/// The basic definition of string
///

#pragma once

#include <stdexcept>

#include "../thirdparty/OpenString-CMake/include/format.h"
#include "../thirdparty/OpenString-CMake/include/text.h"

namespace std {
/// The template specialization for std::hash in order to provide for unordered_map key
template <> struct hash<ostr::text> {
	_CXX17_DEPRECATE_ADAPTOR_TYPEDEFS typedef ostr::text argument_type;
	_CXX17_DEPRECATE_ADAPTOR_TYPEDEFS typedef size_t	 result_type;
	_NODISCARD size_t									 operator()(const ostr::text _Keyval) const noexcept {
		   return hash<string>().operator()(_Keyval.c_str());
	}
};
} // namespace std

// The alias of open string library
using PString	  = ostr::text;
using PStringView = ostr::text_view;

/// Convert the string into specified type C style string
/// \tparam Type The type of the specified C character
/// \param String The string to be converted
/// \return The pointer to the specified C style string
template <class Type> const Type *PStringToCStr(const PString &String) {
	throw std::invalid_argument("Unsupported PStringToCStr type");
}

template <> const char	  *PStringToCStr(const PString &String);
template <> const wchar_t *PStringToCStr(const PString &String);