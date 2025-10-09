#ifndef MenuCore_H
#define MenuCore_H
#include <stdint.h>
#include "Block.h"

#define Line_Max 4
#define	NodeArr_Num			150
#define	NodeBranchArr_Num	100

extern const char stringEXE_type[];
extern const char stringDIR_type[];

typedef enum{
	EXE_type = 0,		//判断是执行节点
	DIR_type = 1		//还是菜单节点
}NodeType;

#define TypeToStr(t) \
    ((t) == EXE_type ? stringEXE_type : \
    (t) == DIR_type ? stringDIR_type : "UNKNOWN")

typedef enum {
	Zero_Error = -1,
	Node_OutOfRange = 0,
	NodeBranch_OutOfRange = 1
}NodeAllocError;

typedef struct {
	NodeType type;
	const char* name;
	void* pointer;
	void* parentPointer;
	void* lastNode;
	void* nextNode;
}Node_t;

typedef struct {
	Node_t* parentNode;
	Node_t* baseNode;
	int16_t childNum;
	block_t block;
}NodeBranch_t;

extern Node_t NodeArr[];
extern NodeBranch_t NodeBranchArr[];

Node_t* NodeAlloc();
NodeBranch_t* NodeBranchAlloc();
NodeBranch_t* set_branch(NodeBranch_t* nodeBranch, ...);
#define	SetBranch(...) set_branch(NodeBranchAlloc(), __VA_ARGS__, NULL)
Node_t* set_node(Node_t* node, NodeType type, const char* name, void* pointer);
#define	SetNode(type, name, pointer) set_node(NodeAlloc(), type, name, (void*)pointer)

#endif // MenuCore_H
