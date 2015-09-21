// -*- tab-width: 4 -*-
/*!
   @file Cursor.hpp

   Copyright (c) 2015 pegacorn
*/
#ifndef clang_cpp_Cursor_hpp
#define clang_cpp_Cursor_hpp

#include <cstddef>
#include <functional>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "clang-c/Index.h"
#include "clang-cpp/CursorKind.hpp"
#include "clang-cpp/Reader.hpp"
#include "clang-cpp/switch_port.hpp"
#include "clang-cpp/UniqueCXObject.hpp"


namespace clangxx {

class TranslationUnit;

class CLANGXX_API Cursor
{
  public:
	using Arguments	= RandomAccessReader<Cursor, unsigned int, std::function<Cursor(unsigned int)>>;

  public:
//	static Cursor from_location(std::shared_ptr<const TranslationUnit> translation_unit, const SourceLocation &location);

	static Cursor from_result(std::shared_ptr<const TranslationUnit> translation_unit);

  private:
	mutable std::shared_ptr<const TranslationUnit>	m_translation_unit;
	// warning: define m_translation_unit before m_cx_cursor
	CXCursor	m_cx_cursor;
	mutable std::string				m_spelling;
	mutable std::string				m_displayname;
	mutable std::shared_ptr<Cursor>	m_canonical;
	mutable unsigned int			m_hash{0};
	mutable std::shared_ptr<Cursor>	m_semantic_parent;
	mutable std::shared_ptr<Cursor>	m_lexical_parent;
	mutable std::shared_ptr<Cursor>	m_referenced;

  private:
	Cursor();

	Cursor(CXCursor &&cx_cursor, std::shared_ptr<const TranslationUnit> &translation_unit);

  public:
	~Cursor();

	Cursor(const Cursor &other);
	Cursor(Cursor &&other) noexcept;

	Cursor &operator=(const Cursor &other);
	Cursor &operator=(Cursor &&other) /*noexcept*/;

  public:
	explicit operator bool() const noexcept {
		return !is_null(m_cx_cursor);
	}

	bool operator==(const Cursor &other) const;

	bool operator!=(const Cursor &other) const {
		return !(*this == other);
	}

	bool is_definition() const;

	bool is_static_method() const;

	Cursor get_definition() const;

	std::string get_usr() const;

	CursorKind kind() const {
		return CursorKind::from_id(m_cx_cursor.kind);
	}

	std::string spelling() const;

	std::string displayname() const;

//	SourceLocation location() const;

//	SourceRange extent() const;

//	access_specifier() const;

//	Type type() const;

	Cursor canonical() const;

//	Type result_type() const;

//	Type underlying_typedef_type() const;

//	Type enum_type() const;

//	unsigned long long enum_value() const;

//	std::string objc_type_encoding() const;

	unsigned int hash() const;

	Cursor semantic_parent() const;

	Cursor lexical_parent() const;

	std::shared_ptr<const TranslationUnit> translation_unit() const {
		return m_translation_unit;
	}

	Cursor referenced() const;

	std::string brief_comment() const;

	std::string raw_comment() const;

	Arguments get_arguments() const;

	std::vector<Cursor> get_children() const;

//	walk_preorder() const;

//	get_tokens() const;

	bool is_bitfield() const;

	std::size_t get_bitfield_width() const;
}; // class Cursor

} // namespace clangxx


#endif // clang_cpp_Cursor_hpp
