// -*- tab-width: 4 -*-
/*!
   @file Index.hpp

   Copyright (c) 2015 pegacorn
*/
#ifndef clang_cpp_Index_hpp
#define clang_cpp_Index_hpp

#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "clang-cpp/switch_port.hpp"
#include "clang-cpp/TranslationUnit.hpp"
#include "clang-cpp/UniqueCXObject.hpp"
#include "clang-cpp/UnsavedFile.hpp"


namespace clangxx {

class CLANGXX_API Index: public std::enable_shared_from_this<Index>
{
  public:
	static std::shared_ptr<Index> create(bool excludeDecls = false);

  private:
	UniqueCXIndex	m_cx_index;

  private:
	Index(UniqueCXIndex &&cx_index) noexcept;

  public:
	~Index();

	Index(const Index &) = delete;
	Index(Index &&other) noexcept;

	Index &operator=(const Index &) = delete;
	Index &operator=(Index &&other) noexcept;

  public:
	CXIndex	native_handle() const noexcept {
		return m_cx_index.get();
	}

	std::shared_ptr<TranslationUnit> read(const std::string &path) {
		return TranslationUnit::from_ast_file(path, shared_from_this());
	}

	std::shared_ptr<TranslationUnit> parse(
	  const std::string &path, const std::vector<std::string> *args = nullptr,
	  const std::vector<UnsavedFile> *unsaved_files = nullptr,
	  CXTranslationUnit_Flags options = CXTranslationUnit_None)
	{
		return TranslationUnit::from_source(path, args, unsaved_files, options,
											shared_from_this());
	}
}; // class Index

} // namespace clangxx


#endif // clang_cpp_Index_hpp
