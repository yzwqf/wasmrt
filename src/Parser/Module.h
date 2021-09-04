#pragma once

#include "Instruction.h"

#include <string>
#include <vector>

namespace wasmrt {
namespace parser {
namespace module {

using TypeIdx   = uint32_t;
using FuncIdx   = uint32_t;
using TableIdx  = uint32_t;
using MemIdx    = uint32_t;
using GlobalIdx = uint32_t;
using LocalIdx  = uint32_t;
using LabelIdx  = uint32_t;
using Expr 		= std::vector<wasmrt::parser::instruction::Instruction>;

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
	std::string  Name;
	uint8_t*	 Bytes;
};

struct ImportDesc {
	uint8_t  Tag;
	union {
		TypeIdx    FuncType;  	// tag = 0
		TableType  Table;		// tag = 1
		MemType    Mem;      	// tag=2
		GlobalType Global;   	// tag=3
	} Idx;
};

struct Import {
	std::string   			 Module;
	std::string   			 Name;
	std::vector<ImportDesc>  Desc;
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
	std::vector<FuncIdx>  Init;
	Expr                  Offset;
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
	std::vector<uint8_t>  Init;
	Expr    			  Offset;
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