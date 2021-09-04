#pragma once

#include "Bytecode.h"
#include "Type.h"

#include <string>
#include <utility>
#include <vector>

using namespace wasmrt::parser::type;

namespace wasmrt {
namespace parser {
namespace module {

using Expr = wasmrt::parser::bytecode::Expr;

inline constexpr uint32_t MagicNumber    = 0x6D736100;
inline constexpr uint32_t SupportVersion = 0x00000001;
inline constexpr uint32_t PageSize 		 = 65536;      	// 64KB
inline constexpr uint32_t MaxPageCount 	 = 65536; 	 	// 2^16

enum SectionID {
	SecCustomID = 0,
	SecTypeID,
	SecImportID,
	SecFuncID,
	SecTableID,
	SecMemID,
	SecGlobalID,
	SecExportID,
	SecStartID,
	SecElemID,
	SecCodeID,
	SecDataID
};

enum ImportTag {
	ImportTagFunc   = 0
	ImportTagTable  = 1
	ImportTagMem    = 2
	ImportTagGlobal = 3
};

enum ExportTag {
	ExportTagFunc   = 0
	ExportTagTable  = 1
	ExportTagMem    = 2
	ExportTagGlobal = 3
};

struct CustomSec {
	CustomSec(std::string &&Name, uint8_t *Bytes)
		: Name(std::move(Name)), Bytes(Bytes) {}

	CustomSec(CustomSec &&Other)
		: Name(std::move(Other.Name)), Bytes(Other.Bytes) {
		Other.Bytes = nullptr;
	}

	~CustomSec() {
		if (Bytes != nullptr)
			delete[] Bytes;
	}

	std::string  Name;
	uint8_t*	 Bytes;
};

struct ImportDesc {
	uint8_t  Tag;
	union {
		TypeIdx    FuncType;  	// tag = 0
		TableType  Table;		// tag = 1
		MemType    Mem;      	// tag = 2
		GlobalType Global;   	// tag = 3
	} Idx;
};

struct Import {
	Import(Import &&Other)
		: Module(std::move(Other.Module)),
		  Name(std::move(Other.Name)), Desc(Desc) {}

	std::string  Module;
	std::string  Name;
	ImportDesc   Desc;
};

struct Global {
	GlobalType  Type;
	Expr 		Init;
};

struct Export {
	std::string	Name;
	ExportDesc	Desc;
};

struct ExportDesc {
	uint8_t	  Tag;
	uint32_t  Idx;
};

struct Elem {
	TableIdx              Table;
	Expr                  Offset;
	std::vector<FuncIdx>  Init;
};

struct Locals {
	uint32   Number;
	ValType  Type;
};

struct Code {
	std::vector<Locals>  Locals;
	Expr                 Expr;
};

struct Data {
	MemIdx  			  Mem;
	Expr    			  Offset;
	std::vector<uint8_t>  Init;
};

struct Module {
	uint32_t   				Magic;
	uint32_t   				Version;
	FuncIdx   				StartSec;
	std::vector<CustomSec> 	CustomSecs;
	std::vector<FuncType>  	TypeSec;
	std::vector<Import>  	ImportSec;
	std::vector<TypeIdx>   	FuncSec;
	std::vector<TableType> 	TableSec;
	std::vector<MemType>   	MemSec;
	std::vector<Global>    	GlobalSec;
	std::vector<Export>    	ExportSec;
	std::vector<Elem>      	ElemSec;
	std::vector<Code>    	CodeSec;
	std::vector<Data>		DataSec;
};

} // namespace module
} // namespace parser
} // namespace wasmrt