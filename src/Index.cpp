// -*- tab-width: 4 -*-
/*!
   @file Index.cpp

   Copyright (c) 2015 pegacorn
*/
#include "clang-cpp/Index.hpp"

#include <memory>
#include <utility>
#include "clang-c/Index.h"
#include "clang-cpp/UniqueCXObject.hpp"


namespace clangxx {

std::shared_ptr<Index> Index::create(bool excludeDecls/* = false*/)
{
	UniqueCXIndex cx_index(clang_createIndex(excludeDecls, 0));
	if ( !cx_index ) {
// TODO: throw
	}

//	return std::make_shared<Index>(std::move(cx_index));
	return std::shared_ptr<Index>(new Index(std::move(cx_index)));
}

} // namespace clangxx
