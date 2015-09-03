// -*- tab-width: 4 -*-
/*!
   @file Exception.hpp

   Copyright (c) 2015 pegacorn
*/
#ifndef clang_cpp_Exception_hpp
#define clang_cpp_Exception_hpp

#include <cstddef>
#include <stdexcept>
#include <string>
#include "clang-c/Index.h"
#include "clang-cpp/switch_port.hpp"


namespace clangxx {

class CLANGXX_API Exception: public std::exception
{
  public:
	struct Where
	{
		std::string	file;
		std::string	function;
		std::size_t	line{0};

		Where(const std::string &file, const std::string &function, std::size_t line)
			: file(file)
			, function(function)
			, line(line)
		{}
	};

  private:
	Where	m_where;

  protected:
	explicit Exception(const Where &where)
		: m_where(where)
	{}

  public:
	const Where &where() const noexcept {
		return m_where;
	}
}; // class Exception

class CLANGXX_API LogicError: public std::logic_error, public Exception
{
  public:
	LogicError(const std::string &what, const Where &where)
		: std::logic_error(what)
		, Exception(where)
	{}
}; // class LogicError

class CLANGXX_API RuntimeError: public std::runtime_error, public Exception
{
  public:
	RuntimeError(const std::string &what, const Where &where)
		: std::runtime_error(what)
		, Exception(where)
	{}
}; // class RuntimeError

class CLANGXX_API TranslationUnitLoadError: public RuntimeError
{
  private:
	using Base	= RuntimeError;

  public:
	using Base::Base;
}; // class TranslationUnitLoadError

class CLANGXX_API TranslationUnitSaveError: public RuntimeError
{
  private:
	using Base	= RuntimeError;

  private:
	static std::string make_what(CXSaveError kind, const std::string &what);

  private:
	CXSaveError	m_kind;

  public:
	TranslationUnitSaveError(CXSaveError kind, const std::string &what,
							 const Where &where)
		: Base(make_what(kind, what), where)
		, m_kind{kind}
	{}

  public:
	CXSaveError save_error() const noexcept {
		return m_kind;
	}
}; // class TranslationUnitSaveError

#if 0
class CLANGXX_API CompilationDatabaseError: public RuntimeError
{
}; // class CompilationDatabaseError

class CLANGXX_API LibclangError: public Exception
{
}; // class LibclangError
#endif

} // namespace clangxx


#define CLANGXX_CONSTRUCT_Exception_Where \
	Exception::Where(__FILE__, __func__, __LINE__)

#define CLANGXX_THROW_LogicError(d_what) \
	throw clangxx::LogicError((d_what), CLANGXX_CONSTRUCT_Exception_Where)

#define CLANGXX_THROW_RuntimeError(d_what) \
	throw clangxx::RuntimeError((d_what), CLANGXX_CONSTRUCT_Exception_Where)

#define CLANGXX_THROW_TranslationUnitLoadError(d_what) \
	throw clangxx::TranslationUnitLoadError((d_what), CLANGXX_CONSTRUCT_Exception_Where)

#define CLANGXX_THROW_TranslationUnitSaveError(kind, d_what) \
	throw clangxx::TranslationUnitSaveError((kind), (d_what), CLANGXX_CONSTRUCT_Exception_Where)


#endif // clang_cpp_Exception_hpp
