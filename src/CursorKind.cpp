// -*- tab-width: 4 -*-
/*!
   @file CursorKind.cpp

   Copyright (c) 2015 pegacorn
*/
#include "clang-cpp/CursorKind.hpp"

#include <cctype>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>


namespace {

std::string name_to_upper(const std::string &name)
{
	std::string upper_name;
	upper_name.reserve(name.size());
	bool is_prev_lower{false};
	for ( std::size_t i{0}; i < name.size(); ++i ) {
		using CharTraits = std::string::traits_type;
		auto ic = CharTraits::to_int_type(name[i]);
		if ( std::isupper(ic) ) {
			if ( (i != 0) &&
				 (is_prev_lower ||
				  (i != (name.size() - 1) &&
				   std::islower(CharTraits::to_int_type(name[i + 1])))) )
			{
				upper_name += "_";
			}
			is_prev_lower = false;
		}
		else {
			ic = std::toupper(ic);
			is_prev_lower = true;
		}
		upper_name += CharTraits::to_char_type(ic);
	}

	return upper_name;
}

} // namespace

namespace clangxx {

bool				CursorKind::s_initialized{false};
CursorKind::Kinds	CursorKind::s_kinds;
CursorKind::NameMap	CursorKind::s_name_map;

void CursorKind::s_initialize()
{
	if ( s_initialized ) {
		return;
	}

#define clangxx_DEFINE_CursorKind(d_value) \
	const CursorKind CursorKind_ ## d_value(CXCursor_ ## d_value, #d_value)

// Declarations
clangxx_DEFINE_CursorKind(UnexposedDecl);
clangxx_DEFINE_CursorKind(StructDecl);
clangxx_DEFINE_CursorKind(UnionDecl);
clangxx_DEFINE_CursorKind(ClassDecl);
clangxx_DEFINE_CursorKind(EnumDecl);
clangxx_DEFINE_CursorKind(FieldDecl);
clangxx_DEFINE_CursorKind(EnumConstantDecl);
clangxx_DEFINE_CursorKind(FunctionDecl);
clangxx_DEFINE_CursorKind(VarDecl);
clangxx_DEFINE_CursorKind(ParmDecl);
clangxx_DEFINE_CursorKind(ObjCInterfaceDecl);
clangxx_DEFINE_CursorKind(ObjCCategoryDecl);
clangxx_DEFINE_CursorKind(ObjCProtocolDecl);
clangxx_DEFINE_CursorKind(ObjCPropertyDecl);
clangxx_DEFINE_CursorKind(ObjCIvarDecl);
clangxx_DEFINE_CursorKind(ObjCInstanceMethodDecl);
clangxx_DEFINE_CursorKind(ObjCClassMethodDecl);
clangxx_DEFINE_CursorKind(ObjCImplementationDecl);
clangxx_DEFINE_CursorKind(ObjCCategoryImplDecl);
clangxx_DEFINE_CursorKind(TypedefDecl);
clangxx_DEFINE_CursorKind(CXXMethod);
clangxx_DEFINE_CursorKind(Namespace);
clangxx_DEFINE_CursorKind(LinkageSpec);
clangxx_DEFINE_CursorKind(Constructor);
clangxx_DEFINE_CursorKind(Destructor);
clangxx_DEFINE_CursorKind(ConversionFunction);
clangxx_DEFINE_CursorKind(TemplateTypeParameter);
clangxx_DEFINE_CursorKind(NonTypeTemplateParameter);
clangxx_DEFINE_CursorKind(TemplateTemplateParameter);
clangxx_DEFINE_CursorKind(FunctionTemplate);
clangxx_DEFINE_CursorKind(ClassTemplate);
clangxx_DEFINE_CursorKind(ClassTemplatePartialSpecialization);
clangxx_DEFINE_CursorKind(NamespaceAlias);
clangxx_DEFINE_CursorKind(UsingDirective);
clangxx_DEFINE_CursorKind(UsingDeclaration);
clangxx_DEFINE_CursorKind(TypeAliasDecl);
clangxx_DEFINE_CursorKind(ObjCSynthesizeDecl);
clangxx_DEFINE_CursorKind(ObjCDynamicDecl);
clangxx_DEFINE_CursorKind(CXXAccessSpecifier);
// References
clangxx_DEFINE_CursorKind(ObjCSuperClassRef);
clangxx_DEFINE_CursorKind(ObjCProtocolRef);
clangxx_DEFINE_CursorKind(ObjCClassRef);
clangxx_DEFINE_CursorKind(TypeRef);
clangxx_DEFINE_CursorKind(CXXBaseSpecifier);
clangxx_DEFINE_CursorKind(TemplateRef);
clangxx_DEFINE_CursorKind(NamespaceRef);
clangxx_DEFINE_CursorKind(MemberRef);
clangxx_DEFINE_CursorKind(LabelRef);
clangxx_DEFINE_CursorKind(OverloadedDeclRef);
clangxx_DEFINE_CursorKind(VariableRef);
// Error conditions
clangxx_DEFINE_CursorKind(InvalidFile);
clangxx_DEFINE_CursorKind(NoDeclFound);
clangxx_DEFINE_CursorKind(NotImplemented);
clangxx_DEFINE_CursorKind(InvalidCode);
// Expressions
clangxx_DEFINE_CursorKind(UnexposedExpr);
clangxx_DEFINE_CursorKind(DeclRefExpr);
clangxx_DEFINE_CursorKind(MemberRefExpr);
clangxx_DEFINE_CursorKind(CallExpr);
clangxx_DEFINE_CursorKind(ObjCMessageExpr);
clangxx_DEFINE_CursorKind(BlockExpr);
clangxx_DEFINE_CursorKind(IntegerLiteral);
clangxx_DEFINE_CursorKind(FloatingLiteral);
clangxx_DEFINE_CursorKind(ImaginaryLiteral);
clangxx_DEFINE_CursorKind(StringLiteral);
clangxx_DEFINE_CursorKind(CharacterLiteral);
clangxx_DEFINE_CursorKind(ParenExpr);
clangxx_DEFINE_CursorKind(UnaryOperator);
clangxx_DEFINE_CursorKind(ArraySubscriptExpr);
clangxx_DEFINE_CursorKind(BinaryOperator);
clangxx_DEFINE_CursorKind(CompoundAssignOperator);
clangxx_DEFINE_CursorKind(ConditionalOperator);
clangxx_DEFINE_CursorKind(CStyleCastExpr);
clangxx_DEFINE_CursorKind(CompoundLiteralExpr);
clangxx_DEFINE_CursorKind(InitListExpr);
clangxx_DEFINE_CursorKind(AddrLabelExpr);
clangxx_DEFINE_CursorKind(StmtExpr);
clangxx_DEFINE_CursorKind(GenericSelectionExpr);
clangxx_DEFINE_CursorKind(GNUNullExpr);
clangxx_DEFINE_CursorKind(CXXStaticCastExpr);
clangxx_DEFINE_CursorKind(CXXDynamicCastExpr);
clangxx_DEFINE_CursorKind(CXXReinterpretCastExpr);
clangxx_DEFINE_CursorKind(CXXConstCastExpr);
clangxx_DEFINE_CursorKind(CXXFunctionalCastExpr);
clangxx_DEFINE_CursorKind(CXXTypeidExpr);
clangxx_DEFINE_CursorKind(CXXBoolLiteralExpr);
clangxx_DEFINE_CursorKind(CXXNullPtrLiteralExpr);
clangxx_DEFINE_CursorKind(CXXThisExpr);
clangxx_DEFINE_CursorKind(CXXThrowExpr);
clangxx_DEFINE_CursorKind(CXXNewExpr);
clangxx_DEFINE_CursorKind(CXXDeleteExpr);
clangxx_DEFINE_CursorKind(UnaryExpr);
clangxx_DEFINE_CursorKind(ObjCStringLiteral);
clangxx_DEFINE_CursorKind(ObjCEncodeExpr);
clangxx_DEFINE_CursorKind(ObjCSelectorExpr);
clangxx_DEFINE_CursorKind(ObjCProtocolExpr);
clangxx_DEFINE_CursorKind(ObjCBridgedCastExpr);
clangxx_DEFINE_CursorKind(PackExpansionExpr);
clangxx_DEFINE_CursorKind(SizeOfPackExpr);
clangxx_DEFINE_CursorKind(LambdaExpr);
clangxx_DEFINE_CursorKind(ObjCBoolLiteralExpr);
clangxx_DEFINE_CursorKind(ObjCSelfExpr);
// Statements
clangxx_DEFINE_CursorKind(UnexposedStmt);
clangxx_DEFINE_CursorKind(LabelStmt);
clangxx_DEFINE_CursorKind(CompoundStmt);
clangxx_DEFINE_CursorKind(CaseStmt);
clangxx_DEFINE_CursorKind(DefaultStmt);
clangxx_DEFINE_CursorKind(IfStmt);
clangxx_DEFINE_CursorKind(SwitchStmt);
clangxx_DEFINE_CursorKind(WhileStmt);
clangxx_DEFINE_CursorKind(DoStmt);
clangxx_DEFINE_CursorKind(ForStmt);
clangxx_DEFINE_CursorKind(GotoStmt);
clangxx_DEFINE_CursorKind(IndirectGotoStmt);
clangxx_DEFINE_CursorKind(ContinueStmt);
clangxx_DEFINE_CursorKind(BreakStmt);
clangxx_DEFINE_CursorKind(ReturnStmt);
clangxx_DEFINE_CursorKind(AsmStmt);
clangxx_DEFINE_CursorKind(ObjCAtTryStmt);
clangxx_DEFINE_CursorKind(ObjCAtCatchStmt);
clangxx_DEFINE_CursorKind(ObjCAtFinallyStmt);
clangxx_DEFINE_CursorKind(ObjCAtThrowStmt);
clangxx_DEFINE_CursorKind(ObjCAtSynchronizedStmt);
clangxx_DEFINE_CursorKind(ObjCAutoreleasePoolStmt);
clangxx_DEFINE_CursorKind(ObjCForCollectionStmt);
clangxx_DEFINE_CursorKind(CXXCatchStmt);
clangxx_DEFINE_CursorKind(CXXTryStmt);
clangxx_DEFINE_CursorKind(CXXForRangeStmt);
clangxx_DEFINE_CursorKind(SEHTryStmt);
clangxx_DEFINE_CursorKind(SEHExceptStmt);
clangxx_DEFINE_CursorKind(SEHFinallyStmt);
clangxx_DEFINE_CursorKind(MSAsmStmt);
clangxx_DEFINE_CursorKind(NullStmt);
clangxx_DEFINE_CursorKind(DeclStmt);
clangxx_DEFINE_CursorKind(OMPParallelDirective);
clangxx_DEFINE_CursorKind(OMPSimdDirective);
clangxx_DEFINE_CursorKind(OMPForDirective);
clangxx_DEFINE_CursorKind(OMPSectionsDirective);
clangxx_DEFINE_CursorKind(OMPSectionDirective);
clangxx_DEFINE_CursorKind(OMPSingleDirective);
clangxx_DEFINE_CursorKind(OMPParallelForDirective);
clangxx_DEFINE_CursorKind(OMPParallelSectionsDirective);
clangxx_DEFINE_CursorKind(OMPTaskDirective);
clangxx_DEFINE_CursorKind(OMPMasterDirective);
clangxx_DEFINE_CursorKind(OMPCriticalDirective);
clangxx_DEFINE_CursorKind(OMPTaskyieldDirective);
clangxx_DEFINE_CursorKind(OMPBarrierDirective);
clangxx_DEFINE_CursorKind(OMPTaskwaitDirective);
clangxx_DEFINE_CursorKind(OMPFlushDirective);
clangxx_DEFINE_CursorKind(SEHLeaveStmt);
// Other
clangxx_DEFINE_CursorKind(TranslationUnit);
// Attributes
clangxx_DEFINE_CursorKind(UnexposedAttr);
clangxx_DEFINE_CursorKind(IBActionAttr);
clangxx_DEFINE_CursorKind(IBOutletAttr);
clangxx_DEFINE_CursorKind(IBOutletCollectionAttr);
clangxx_DEFINE_CursorKind(CXXFinalAttr);
clangxx_DEFINE_CursorKind(CXXOverrideAttr);
clangxx_DEFINE_CursorKind(AnnotateAttr);
clangxx_DEFINE_CursorKind(AsmLabelAttr);
clangxx_DEFINE_CursorKind(PackedAttr);
clangxx_DEFINE_CursorKind(PureAttr);
clangxx_DEFINE_CursorKind(ConstAttr);
clangxx_DEFINE_CursorKind(NoDuplicateAttr);
clangxx_DEFINE_CursorKind(CUDAConstantAttr);
clangxx_DEFINE_CursorKind(CUDADeviceAttr);
clangxx_DEFINE_CursorKind(CUDAGlobalAttr);
clangxx_DEFINE_CursorKind(CUDAHostAttr);
// Preprocessing
clangxx_DEFINE_CursorKind(PreprocessingDirective);
clangxx_DEFINE_CursorKind(MacroDefinition);
clangxx_DEFINE_CursorKind(MacroInstantiation);
clangxx_DEFINE_CursorKind(InclusionDirective);
// Extra Declarations
clangxx_DEFINE_CursorKind(ModuleImportDecl);

#undef clangxx_DEFINE_CursorKind

}

CursorKind CursorKind::from_id(CXCursorKind cx_cursor_kind)
{
	const decltype(s_kinds)::const_iterator iter = s_kinds.find(cx_cursor_kind);
	if ( iter == s_kinds.end() ) {
// TODO: throw
	}
	return iter->second;
}

CursorKind::CursorKind(CXCursorKind value, const std::string &name)
{
	const decltype(s_kinds)::const_iterator iter = s_kinds.find(value);
	if ( iter != s_kinds.end() ) {
// TODO: throw
	}
	m_cx_cursor_kind = value;
	s_kinds[value] = *this;

	s_name_map[*this] = name_to_upper(name);
}

std::string CursorKind::name() const
{
	const decltype(s_name_map)::const_iterator iter = s_name_map.find(*this);
	if ( iter == s_name_map.end() ) {
// TODO: throw
	}
	return iter->second;
}

std::string CursorKind::repr() const
{
	std::ostringstream ostream;
	ostream << "CursorKind." << name();
	return ostream.str();
}

} // namespace clangxx
