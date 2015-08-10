// -*- tab-width: 4 -*-
/*!
   @file Cursor.cpp

   Copyright (c) 2015 pegacorn
*/
#include "clang-cpp/Cursor.hpp"

#include <memory>
#include <utility>
#include "clang-c/Index.h"
#include "clang-cpp/TranslationUnit.hpp"
#include "clang-cpp/UniqueCXObject.hpp"


namespace clangxx {

Cursor Cursor::from_result(std::shared_ptr<const TranslationUnit> translation_unit)
{
	CXCursor cx_cursor(clang_getTranslationUnitCursor(
		translation_unit->native_handle()));
	if ( is_null(cx_cursor) ) {
// TODO: throw
	}

	return Cursor(std::move(cx_cursor), translation_unit);
}

std::string Cursor::spelling() const
{
	UniqueCXString cx_string(clang_getCursorSpelling(m_cx_cursor));
	if ( !cx_string ) {
// TODO: throw
	}

	return clang_getCString(cx_string.get());
}

} // namespace clangxx
