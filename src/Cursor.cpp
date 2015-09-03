// -*- tab-width: 4 -*-
/*!
   @file Cursor.cpp

   Copyright (c) 2015 pegacorn
*/
#include "clang-cpp/Cursor.hpp"

#include <memory>
#include <utility>
#include "clang-c/Index.h"
#include "clang-cpp/Exception.hpp"
#include "clang-cpp/TranslationUnit.hpp"
#include "clang-cpp/UniqueCXObject.hpp"


namespace clangxx {

Cursor Cursor::from_result(std::shared_ptr<const TranslationUnit> translation_unit)
{
	CXCursor cx_cursor(clang_getTranslationUnitCursor(
		translation_unit->native_handle()));
	if ( is_null(cx_cursor) ) {
		CLANGXX_THROW_LogicError("Error retrieving the cursor that represents the given translation unit.");
	}

	return Cursor(std::move(cx_cursor), translation_unit);
}

std::string Cursor::spelling() const
{
	UniqueCXString cx_string(clang_getCursorSpelling(m_cx_cursor));
	if ( !cx_string ) {
		CLANGXX_THROW_LogicError("Error retrieving a name for the entity referenced by this cursor.");
	}

	return clang_getCString(cx_string.get());
}

} // namespace clangxx
