// -*- tab-width: 4 -*-
/*!
   @file Cursor.hpp

   Copyright (c) 2015 pegacorn
*/
#ifndef clang_cpp_Cursor_hpp
#define clang_cpp_Cursor_hpp

#include <memory>
#include <utility>
#include "clang-c/Index.h"
#include "clang-cpp/CursorKind.hpp"
#include "clang-cpp/switch_port.hpp"


namespace clangxx {

class TranslationUnit;

class CLANGXX_API Cursor
{
  public:
	static Cursor from_result(std::shared_ptr<const TranslationUnit> translation_unit);

  private:
	std::shared_ptr<const TranslationUnit>	m_translation_unit;
	// warning: define m_translation_unit before m_cx_cursor
	CXCursor	m_cx_cursor;

  public:
	Cursor(CXCursor &&cx_cursor, std::shared_ptr<const TranslationUnit> &translation_unit) noexcept
		: m_translation_unit(translation_unit)
		, m_cx_cursor(std::move(cx_cursor))
	{}

  public:
	~Cursor() = default;

	Cursor(const Cursor &) = delete;
	Cursor(Cursor &&other) noexcept = default;

	Cursor &operator=(const Cursor &) = delete;
	Cursor &operator=(Cursor &&other) noexcept = default;

  public:
	CursorKind kind() const {
		return CursorKind::from_id(m_cx_cursor.kind);
	}

	std::string spelling() const;
}; // class Cursor

} // namespace clangxx


#endif // clang_cpp_Cursor_hpp
