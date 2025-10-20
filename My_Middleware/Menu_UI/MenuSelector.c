
// MenuSelector.c - 菜单选择器相关功能实现
#include "MenuSelector.h"

// 当前菜单选择器指针，全局变量
NodeBranch_t* Selector;

/**
 * @brief 向上移动菜单选择
 *
 * 调用 block_decrease 使选择项上移。
 */
void SelectUP(void) {
	block_decrease(&(Selector->block));
}

/**
 * @brief 向下移动菜单选择
 *
 * 调用 block_increase 使选择项下移。
 */
void SelectDOWN(void) {
	block_increase(&(Selector->block));
}

/**
 * @brief 进入子菜单或执行菜单项
 *
 * 如果当前项为 EXE_type，执行其函数指针。
 * 如果为 DIR_type，进入子菜单。
 */
void SelectINorRUN(void) {
	Node_t* baseNode = Selector->baseNode;
	int num = Selector->block.pointer;
	Node_t* currentNode = baseNode;
	for (int i = 0; i < num; i++) {
		currentNode = currentNode->nextNode;
	}
	if (currentNode == NULL) return;
	if (currentNode->type == EXE_type) {
		if (currentNode->pointer != NULL) {
			((FuncPtr)(currentNode->pointer))(); // 执行函数
		}
	}
	else if (currentNode->type == DIR_type) {
		if (currentNode->pointer != NULL) {
			Selector = currentNode->pointer; // 进入子菜单
		}
	}
}

/**
 * @brief 返回上一级菜单
 *
 * 如果存在父节点，则切换到父菜单。
 */
void SelectOut(void) {
	if (Selector->parentNode == NULL) return;
	Node_t* nodeParent = Selector->parentNode;
	if (nodeParent->parentPointer == NULL) return;
	Selector = nodeParent->parentPointer;
}

/**
 * @brief OLED 显示一行字符串（模拟）
 * @param line 行号
 * @param str  要显示的字符串
 */
void OLEDPrintStringLine(int line, const char* str) {
	printf("[OLED:%d] [%-16.16s]\n", line, str);
}
/**
 * @brief 打印当前菜单选择器的可见项
 *
 * 功能：根据 `Selector->block` 的窗口信息，找到当前窗口的起始节点，
 *       连续输出 `length` 行或直到链表结束。当前选中项在输出时以 "> " 前缀标识。
 */
void PrintSelector() {
	static uint16_t blk_length = 0;	// 用于记录窗口列表长度变化
	NodeBranch_t* selector = Selector; // 当前活动分支
	if (selector == NULL) return;     // 防护：没有活动分支则直接返回

	block_t* blk = &selector->block;   // 引用本分支的窗口控制块
	/* 窗口长度变化时清屏并更新长度
		窗口长度没有变化时更新的数据显示直接覆盖*/
	if(blk_length != blk->length)		// 如果窗口长度变化，清屏并更新长度
	{
		myOLED_Clear(); // 更新显示
		// 更新当前窗口长度
		blk_length = blk->length;	
	}
	
	Node_t* node = selector->baseNode; // 从链表头开始

	/* 跳过前面 block_pointer 个节点，定位到窗口起始节点 */
	for (int i = 0; i < blk->block_pointer && node != NULL; i++) {
		node = node->nextNode;
	}

	/* 从窗口起始节点开始，输出最多 length 行 */
	for (int i = 0; i < blk->length && node != NULL; i++) {
		int index = blk->block_pointer + i; // 当前行对应的全局索引
		char buf[64];                        // 行字符串缓冲（注意栈大小）

		/* 如果该行是全局选中项，则前面加 "> " 标记 */
		if (index == blk->pointer) {
			snprintf(buf, sizeof(buf), "> %s", node->name);
		}
		else {
			snprintf(buf, sizeof(buf), "  %s", node->name);
		}

		/* 将格式化后的行交给显示层（由 PrintStringLine 实现） */
		PrintStringLine(i, buf);

		/* 移动到下一个节点继续输出 */
		node = node->nextNode;
	}
}