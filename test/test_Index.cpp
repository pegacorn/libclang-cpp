#include <cassert>
#include <iostream>
#include <typeinfo>
#include "clang-cpp/Index.hpp"

using namespace clangxx;
using namespace std;

const string inputs_dir("../vendor/clang/bindings/python/tests/cindex/INPUTS");


int main()
{
	auto index = Index::create();
	assert(index);
	assert(typeid(index) == typeid(shared_ptr<Index>));

	auto tu = index->parse(inputs_dir + "/hello.cpp");
	assert(tu);
	assert(typeid(tu) == typeid(shared_ptr<TranslationUnit>));
}
