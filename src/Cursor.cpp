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

Cursor::~Cursor() = default;

std::string Cursor::spelling() const
{
	UniqueCXString cx_string(clang_getCursorSpelling(m_cx_cursor));
	if ( !cx_string ) {
		CLANGXX_THROW_LogicError("Error retrieving a name for the entity referenced by this cursor.");
	}

	return clang_getCString(cx_string.get());
}

std::vector<Cursor> Cursor::get_children()
{
	auto visitor = [](CXCursor cursor, CXCursor /*parent*/, CXClientData client_data) {
		if ( is_null(cursor) ) {
			CLANGXX_THROW_LogicError("cursor is null");
		}
		auto pair = static_cast<std::pair<std::vector<Cursor> *, Cursor *> *>(client_data);
		pair->first->push_back(
		  Cursor(std::move(cursor), pair->second->m_translation_unit));
		return CXChildVisitResult::CXChildVisit_Continue;
	};

	std::vector<Cursor> children;
	auto client_data = std::make_pair(&children, this);
	clang_visitChildren(m_cx_cursor, visitor, &client_data);
	return children;
}

} // namespace clangxx
