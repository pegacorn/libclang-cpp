// -*- tab-width: 4 -*-
/*!
   @file Cursor.cpp

   Copyright (c) 2015 pegacorn
*/
#include "clang-cpp/Cursor.hpp"

#include <memory>
#include <string>
#include <utility>
#include <vector>
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

Cursor::Cursor()
	: m_cx_cursor()
{}

Cursor::Cursor(CXCursor &&cx_cursor, std::shared_ptr<const TranslationUnit> &translation_unit)
	: m_translation_unit(translation_unit)
	, m_cx_cursor(std::move(cx_cursor))
{}

Cursor::~Cursor() = default;

Cursor::Cursor(const Cursor &/*other*/) = default;
Cursor::Cursor(Cursor &&/*other*/) noexcept = default;

Cursor &Cursor::operator=(const Cursor &/*other*/) = default;
Cursor &Cursor::operator=(Cursor &&/*other*/) /*noexcept*/ = default;

bool Cursor::operator==(const Cursor &other) const
{
	return clang_equalCursors(m_cx_cursor, other.m_cx_cursor) == 0;
}

bool Cursor::is_definition() const
{
	return clang_isCursorDefinition(m_cx_cursor) == 0;
}

bool Cursor::is_static_method() const
{
	return clang_CXXMethod_isStatic(m_cx_cursor) == 0;
}

Cursor Cursor::get_definition() const
{
	CXCursor cx_cursor(clang_getCursorDefinition(m_cx_cursor));
	if ( is_null(cx_cursor) ) {
		if ( is_null(m_cx_cursor) ) {
			CLANGXX_THROW_LogicError("Error retrieving a cursor that points to the definition of that entity.");
		}
		return Cursor();
	}

	return Cursor(std::move(cx_cursor), m_translation_unit);
}

std::string Cursor::get_usr() const
{
	UniqueCXString cx_string(clang_getCursorUSR(m_cx_cursor));
	if ( !cx_string ) {
		CLANGXX_THROW_LogicError("Error retrieving a Unified Symbol Resolution (USR) for the entity referenced by this cursor.");
	}

	return clang_getCString(cx_string.get());
}

std::string Cursor::spelling() const
{
	if ( m_spelling.empty() ) {
		UniqueCXString cx_string(clang_getCursorSpelling(m_cx_cursor));
		if ( !cx_string ) {
			CLANGXX_THROW_LogicError("Error retrieving a name for the entity referenced by this cursor.");
		}
		m_spelling = clang_getCString(cx_string.get());
	}
	return m_spelling;
}

std::string Cursor::displayname() const
{
	if ( m_displayname.empty() ) {
		UniqueCXString cx_string(clang_getCursorDisplayName(m_cx_cursor));
		if ( !cx_string ) {
			CLANGXX_THROW_LogicError("Error retrieving the display name for the entity referenced by this cursor.");
		}
		m_displayname = clang_getCString(cx_string.get());
	}
	return m_displayname;
}

Cursor Cursor::canonical() const
{
	if ( !m_canonical ) {
		CXCursor cx_cursor(clang_getCanonicalCursor(m_cx_cursor));
		if ( is_null(cx_cursor) ) {
			CLANGXX_THROW_LogicError("Error retrieving the canonical cursor corresponding to this cursor.");
		}
		m_canonical.reset(new Cursor(std::move(cx_cursor), m_translation_unit));
	}
	return *m_canonical;
}

unsigned int Cursor::hash() const
{
	if ( !m_hash ) {
		m_hash = clang_hashCursor(m_cx_cursor);
	}
	return m_hash;
}

Cursor Cursor::semantic_parent() const
{
	if ( !m_semantic_parent ) {
		CXCursor cx_cursor(clang_getCursorSemanticParent(m_cx_cursor));
		if ( is_null(cx_cursor) ) {
			CLANGXX_THROW_LogicError("Error determining the semantic parent of this cursor.");
		}
		m_semantic_parent.reset(new Cursor(std::move(cx_cursor), m_translation_unit));
	}
	return *m_semantic_parent;
}

Cursor Cursor::lexical_parent() const
{
	if ( !m_lexical_parent ) {
		CXCursor cx_cursor(clang_getCursorLexicalParent(m_cx_cursor));
		if ( is_null(cx_cursor) ) {
			CLANGXX_THROW_LogicError("Error determining the lexical parent of this cursor.");
		}
		m_lexical_parent.reset(new Cursor(std::move(cx_cursor), m_translation_unit));
	}
	return *m_lexical_parent;
}

Cursor Cursor::referenced() const
{
	if ( !m_referenced ) {
		CXCursor cx_cursor(clang_getCursorReferenced(m_cx_cursor));
		if ( is_null(cx_cursor) ) {
			if ( is_null(m_cx_cursor) ) {
				CLANGXX_THROW_LogicError("Error retrieving a cursor representing the entity that this cursor references.");
			}
			m_referenced.reset(new Cursor());
		}
		else {
			m_referenced.reset(new Cursor(std::move(cx_cursor), m_translation_unit));
		}
	}
	return *m_referenced;
}

std::string Cursor::brief_comment() const
{
	UniqueCXString cx_string(clang_Cursor_getBriefCommentText(m_cx_cursor));
	if ( !cx_string ) {
		CLANGXX_THROW_LogicError("Error retrieving the brief comment text associated with this cursor.");
	}
	return clang_getCString(cx_string.get());
}

std::string Cursor::raw_comment() const
{
	UniqueCXString cx_string(clang_Cursor_getRawCommentText(m_cx_cursor));
	if ( !cx_string ) {
		CLANGXX_THROW_LogicError("Error retrieving the raw comment text associated with this cursor.");
	}
	return clang_getCString(cx_string.get());
}

Cursor::Arguments Cursor::get_arguments() const
{
	auto generator = [this](unsigned int index) {
		CXCursor cx_cursor(clang_Cursor_getArgument(this->m_cx_cursor, index));
		if ( is_null(cx_cursor) ) {
			CLANGXX_THROW_LogicError("Error retrieving the argument cursor of a function or method.");
		}

		return Cursor(std::move(cx_cursor), this->m_translation_unit);
	};

	int num_args{clang_Cursor_getNumArguments(m_cx_cursor)};
	if ( num_args < 0 ) {
		if ( is_null(m_cx_cursor) ) {
			CLANGXX_THROW_LogicError("Error retrieving the number of non-variadic arguments associated with this cursor.");
		}
		num_args = 0;
	}

	return Arguments(generator, num_args);
}

std::vector<Cursor> Cursor::get_children() const
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

bool Cursor::is_bitfield() const
{
	return clang_Cursor_isBitField(m_cx_cursor) != 0;
}

std::size_t Cursor::get_bitfield_width() const
{
	int width{clang_getFieldDeclBitWidth(m_cx_cursor)};
	if ( width < 0 ) {
		if ( is_null(m_cx_cursor) ) {
			CLANGXX_THROW_LogicError("Error retrieving the bit width of a bit field declaration as an integer.");
		}
		width = 0;
	}
	return static_cast<std::size_t>(width);
}

} // namespace clangxx
