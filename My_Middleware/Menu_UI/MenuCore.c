// MenuCore.c - 菜单核心功能实现
#include "MenuCore.h"
#include <stdarg.h>

// EXE 类型字符串常量
const char stringEXE_type[10] = "EXE";
// DIR 类型字符串常量
const char stringDIR_type[10] = "DIR";

// 节点数组和分支数组
// 全局/静态对象在 C 中会自动被零初始化，去掉对包含枚举成员的结构体数组的显式 {0} 初始化
Node_t NodeArr[NodeArr_Num];
static int16_t NodeAllocHead = 0;
NodeBranch_t NodeBranchArr[NodeBranchArr_Num];
static int16_t NodeBranchAllocHead = 0;

// 节点分配错误记录
static NodeAllocError ErrorRecord = Zero_Error;

/**
 * @brief 获取最近一次节点/分支分配错误
 * @return 最近一次分配时记录的错误代码（Zero_Error 表示无错误）
 */
NodeAllocError GetNodeAllocError(void) {
	return ErrorRecord;
}

/**
 * @brief 清除分配错误记录（重置为 Zero_Error）
 */
void ClearNodeAllocError(void) {
	ErrorRecord = Zero_Error;
}

/**
 * @brief 分配一个新的节点（Node_t）
 * @return 成功返回节点指针，失败返回 NULL
 */
Node_t * NodeAlloc(){
	if (NodeAllocHead < NodeArr_Num) {
		NodeAllocHead++;
		return &(NodeArr[NodeAllocHead-1]);
	}
	else {
		ErrorRecord = Node_OutOfRange;
		return NULL;
	}
}

/**
 * @brief 分配一个新的节点分支（NodeBranch_t）
 * @return 成功返回分支指针，失败返回 NULL
 */
NodeBranch_t* NodeBranchAlloc() {
	if (NodeBranchAllocHead < NodeBranchArr_Num) {
		NodeBranchAllocHead++;
		return &(NodeBranchArr[NodeBranchAllocHead-1]);
	}
	else {
		ErrorRecord = NodeBranch_OutOfRange;
		return NULL;
	}
}

/**
 * @brief 设置节点属性
 * @param node 节点指针
 * @param type 节点类型（EXE/DIR）
 * @param name 节点名称
 * @param pointer 节点指向的数据或分支
 * @return 设置后的节点指针
 */
Node_t* set_node(Node_t* node, NodeType type, const char* name, void* pointer) {
	if (node == NULL) {
		return NULL;
	}
	node->type = type;           // 节点类型
	node->name = name;           // 节点名称
	node->pointer = pointer;     // 指向的数据或分支
	node->parentPointer = NULL;  // 父分支指针
	node->lastNode = NULL;       // 上一个兄弟节点
	node->nextNode = NULL;       // 下一个兄弟节点
	if (type == DIR_type && pointer != NULL) {
		((NodeBranch_t*)(pointer))->parentNode = node; // 设置分支的父节点
	}
	return node;
}

/**
 * @brief 设置分支，连接多个节点形成链表
 * @param nodeBranch 分支指针
 * @param ... 可变参数，依次为 Node_t*，以 NULL 结尾
 * @return 设置后的分支指针
 *
 * 该函数会将传入的节点依次连接，形成链表，并设置父分支指针。
 */
NodeBranch_t* set_branch(NodeBranch_t* nodeBranch, ...) {
	if (nodeBranch == NULL) {
		return NULL;
	}
	nodeBranch->baseNode = NULL;     // 分支的第一个节点
	nodeBranch->parentNode = NULL;   // 分支的父节点
	nodeBranch->childNum = 0;        // 子节点数量
	va_list args;
	va_start(args, nodeBranch);
	int16_t count = 0;
	Node_t* currentNode = NULL;
	Node_t* LastNode = NULL;
	while ((currentNode = va_arg(args, Node_t*)) != NULL) {
		if (count == 0) {
			nodeBranch->baseNode = currentNode;
		}
		currentNode->parentPointer = nodeBranch; // 设置父分支指针
		currentNode->lastNode = LastNode;        // 设置上一个兄弟节点
		if (LastNode != NULL) {
			LastNode->nextNode = currentNode;     // 设置下一个兄弟节点
		}
		LastNode = currentNode;
		count++;
	}
	nodeBranch->childNum = count;    // 记录子节点数量
	block_init(&(nodeBranch->block), Line_Max, count); // 初始化分支的块
	va_end(args);
	return nodeBranch;
}
