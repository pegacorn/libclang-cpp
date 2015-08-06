#include "clang-cpp/Index.hpp"
#include <cassert>
#include <iostream>
#include <typeinfo>

using namespace clangxx;


int main()
{
	auto index = Index::create();
	std::cout << typeid(index).name() << "\n";
	assert(index.get() != nullptr);
	assert(typeid(index) == typeid(std::shared_ptr<Index>));

	auto tu = index->parse("../vendor/clang/bindings/python/tests/cindex/INPUTS/hello.cpp");
	std::cout << typeid(tu).name() << "\n";
	assert(tu.get() != nullptr);
	assert(typeid(tu) == typeid(std::unique_ptr<TranslationUnit>));
}
