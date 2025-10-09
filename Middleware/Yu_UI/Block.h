#ifndef BLOCK_H
#define BLOCK_H
#include <stdint.h>
#include <stdio.h>
/**
 * @brief 滚动窗口/选择块结构
 *
 * 该结构用于保存一个可滚动列表的显示窗口与当前选择位置。
 * 典型用法见 MenuCore / MenuSelector：
 *  - `depth` 表示列表总条目数（最大深度），范围为 [0, depth-1]
 *  - `length` 表示一次可见的行数（窗口高度）
 *  - `block_pointer` 表示当前窗口在全列表中的起始索引（即可见区第0项对应的全局索引）
 *  - `pointer` 表示当前全局选中项索引
 *
 * 不变量（期望但不由类型强制）：
 *  0 <= block_pointer <= pointer < depth
 *  1 <= length <= depth （初始化时应保证 length 不大于 depth）
 */
typedef struct {
	int16_t pointer;       /**< 当前全局选中项索引（0..depth-1） */
	int16_t length;        /**< 窗口长度（可见行数） */
	int16_t block_pointer; /**< 窗口起始全局索引 */
	int16_t depth;         /**< 列表总长度（最大深度） */
} block_t;


/**
 * @brief 初始化 block_t 结构体
 * @param block 指向要初始化的 block_t
 * @param length 期望的窗口长度（可见行数），若 length>depth 则会被限制为 depth
 * @param depth 列表总长度（最大深度）
 * @return 0 表示成功，-1 表示参数错误（例如 block == NULL）
 *
 * 该函数会把 `pointer` 和 `block_pointer` 置为 0，并设置 `length` 和 `depth`。
 */
int block_init(block_t* block, unsigned int length, unsigned int depth);


/**
 * @brief 将选择向下移动一项（或在窗口末尾时把窗口整体向下滑动）
 * @param block 指向 block_t
 *
 * 行为：
 *  - 如果 pointer < block_pointer + length - 1，则仅 pointer++。
 *  - 否则（pointer 在窗口末尾）且 (block_pointer + length < depth)，则 pointer++ 且 block_pointer++，
 *    相当于窗口和指针同时向下滑动一行，保证 pointer 仍在可见窗口内。
 */
void block_increase(block_t* block);


/**
 * @brief 将选择向上移动一项（或在窗口起始时把窗口整体向上滑动）
 * @param block 指向 block_t
 *
 * 行为：
 *  - 如果 pointer > block_pointer，则仅 pointer--。
 *  - 否则（pointer 在窗口起始）且 block_pointer > 0，则 pointer-- 且 block_pointer--，
 *    窗口整体上移一行。
 */
void block_decrease(block_t * block);


/**
 * @brief 打印 block 的主要成员（仅用于调试）
 * @param block 指向 block_t
 *
 * 该函数使用 printf 输出 `pointer`, `block_pointer`, `length`, `depth` 的值。
 * 在嵌入式平台上，如无标准输出，可能需要替换为串口打印或断点调试。
 */
void block_print(block_t* block);
#endif

