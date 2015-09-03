// -*- tab-width: 4 -*-
/*!
   @file CursorKind.hpp

   Copyright (c) 2015 pegacorn
*/
#ifndef clang_cpp_CursorKind_hpp
#define clang_cpp_CursorKind_hpp

#include <functional>
#include <string>
#include <type_traits>
#include <unordered_map>
#include "clang-c/Index.h"
#include "clang-cpp/switch_port.hpp"


namespace clangxx {

class CLANGXX_API CursorKind
{
  private:
	using NameMap	= std::unordered_map<CursorKind, std::string>;

  public:
	using Kinds		= std::unordered_map<CXCursorKind, CursorKind,
										 std::hash<std::underlying_type<CXCursorKind>::type>>;

  private:
	static bool		s_initialized;
	static Kinds	s_kinds;
	static NameMap	s_name_map;

  public:
	static void s_initialize();

	static CursorKind from_id(CXCursorKind cx_cursor_kind);

	static const Kinds &get_all_kinds() {
		return s_kinds;
	}

  private:
	CXCursorKind	m_cx_cursor_kind;

  public:
	constexpr CursorKind() noexcept
		: m_cx_cursor_kind{}
	{}

	CursorKind(CXCursorKind value, const std::string &name);

  public:
	CXCursorKind value() const {
		return m_cx_cursor_kind;
	}

	CXCursorKind from_param() const {
		return value();
	}

	std::string name() const;

	bool is_declaration() const {
		return clang_isDeclaration(m_cx_cursor_kind) != 0;
	}

	bool is_reference() const {
		return clang_isReference(m_cx_cursor_kind) != 0;
	}

	bool is_expression() const {
		return clang_isExpression(m_cx_cursor_kind) != 0;
	}

	bool is_statement() const {
		return clang_isStatement(m_cx_cursor_kind) != 0;
	}

	bool is_attribute() const {
		return clang_isAttribute(m_cx_cursor_kind) != 0;
	}

	bool is_invalid() const {
		return clang_isInvalid(m_cx_cursor_kind) != 0;
	}

	bool is_translation_unit() const {
		return clang_isTranslationUnit(m_cx_cursor_kind) != 0;
	}

	bool is_preprocessing() const {
		return clang_isPreprocessing(m_cx_cursor_kind) != 0;
	}

	bool is_unexposed() const {
		return clang_isUnexposed(m_cx_cursor_kind) != 0;
	}

	std::string repr() const;

#define clangxx_DEFINE_BINARY_OPERATOR(d_op) \
	constexpr bool operator d_op(CursorKind rhs) const noexcept { \
		return m_cx_cursor_kind d_op rhs.m_cx_cursor_kind; \
	}

clangxx_DEFINE_BINARY_OPERATOR(==)
clangxx_DEFINE_BINARY_OPERATOR(!=)
clangxx_DEFINE_BINARY_OPERATOR(<)
clangxx_DEFINE_BINARY_OPERATOR(<=)
clangxx_DEFINE_BINARY_OPERATOR(>)
clangxx_DEFINE_BINARY_OPERATOR(>=)

#undef clangxx_DEFINE_BINARY_OPERATOR

  private:
	std::size_t hash_code() const noexcept {
		return std::hash<std::underlying_type<CXCursorKind>::type>()(m_cx_cursor_kind);
	}

	friend struct std::hash<CursorKind>;
}; // class CursorKind

} // namespace clangxx

namespace std {

template<>
struct hash<clangxx::CursorKind>
{
	size_t operator()(clangxx::CursorKind cursor_kind) const noexcept {
		return cursor_kind.hash_code();
	}
};

} // namespace std


#endif // clang_cpp_CursorKind_hpp
