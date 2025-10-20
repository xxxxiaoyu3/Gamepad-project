// Block.c - 块操作相关函数实现
#include "Block.h"
  
/**
 * @brief 初始化 block_t 结构体
 * @param block 指向 block_t 结构体的指针
 * @param length 块的长度
 * @param depth 块的深度（最大容量）
 * @return 0 表示成功，-1 表示失败
 */
int block_init(block_t* block, unsigned int length, unsigned int depth) {
	if (block == NULL) {
		return -1;
	}
	block->pointer = 0;           // 当前指针位置
	block->block_pointer = 0;     // 当前块的起始位置
	if (length > depth) {
		block->length = depth;    // 长度不能超过深度
	}
	else {
		block->length = length;
	}
	block->depth = depth;         // 设置最大深度
	return 0;
}

/**
 * @brief 增加指针位置，向下移动一项
 * @param block 指向 block_t 结构体的指针
 *
 * 如果指针未到达当前块的末尾，则仅移动指针。
 * 如果已到达末尾且还有空间，则块整体向下滑动。
 */
void block_increase(block_t* block) {
	if (block->pointer < block->block_pointer + block->length - 1) {
		block->pointer++;
	}
	else if (block->block_pointer + block->length < block->depth) {
		block->pointer++;
		block->block_pointer++;
	}
}

/**
 * @brief 减小指针位置，向上移动一项
 * @param block 指向 block_t 结构体的指针
 *
 * 如果指针未到达当前块的起始位置，则仅移动指针。
 * 如果已到达起始且还有空间，则块整体向上滑动。
 */
void block_decrease(block_t* block) {
	if (block->pointer > block->block_pointer) {
		block->pointer--;
	}
	else if (block->block_pointer > 0) {
		block->pointer--;
		block->block_pointer--;
	}
}

/**
 * @brief 打印 block_t 结构体的主要成员变量
 * @param block 指向 block_t 结构体的指针
 */
void block_print(block_t* block) {
	printf("\n%d\n%d\n%d\n%d\n", block->pointer, block->block_pointer, block->length, block->depth);
}