#include <cassert>
#include <iostream>
#include <typeinfo>
#include "clang-cpp/Index.hpp"

using namespace clangxx;


int main()
{
	std::shared_ptr<Index> index(Index::create());
	std::cout << typeid(index).name() << "\n";
	assert(index.get() != nullptr);
	assert(typeid(index) == typeid(std::shared_ptr<Index>));

	std::shared_ptr<TranslationUnit> tu(
	  index->parse("../vendor/clang/bindings/python/tests/cindex/INPUTS/hello.cpp"));
	std::cout << typeid(tu).name() << "\n";
	assert(tu.get() != nullptr);
	assert(typeid(tu) == typeid(std::shared_ptr<TranslationUnit>));
}
