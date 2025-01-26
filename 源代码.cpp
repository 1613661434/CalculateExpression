#include <iostream>
#include <string>
using namespace std;

#define operandType double

bool ERROR = false; //定义全局变量ERROR，用于判断表达式是否有错误

// 树存储结构
typedef struct BiTNode {
	bool isOperator = true; //是否是运算符
	char Operator = '#'; //运算符
	int OperatorLevel = 0; //符号等级
	operandType Operand = 0; //运算数
	struct BiTNode* lchild = nullptr, * rchild = nullptr; // 左右孩子指针
} BiTNode, * BiTree;

// 销毁二叉树
void DestroyBiTree(BiTree tree)
{
	if (!tree)return; //空结点返回

	DestroyBiTree(tree->lchild); //销毁左子树
	DestroyBiTree(tree->rchild); //销毁右子树

	delete tree; //销毁当前结点
}

// 循环队列算法，辅助完成二叉树层次遍历
int MaxSize = 50;   // 定义队列中元素的最大个数(默认50)
typedef struct SqQueue {
	BiTree* data = new BiTree[MaxSize]; // 存放队列元素
	int front = 0, rear = 0;   // 队头和队尾指针
} SqQueue;

// 销毁队列
void DestroyQueue(SqQueue& Q) {
	delete[] Q.data;  // 释放动态分配的数组
	Q.data = nullptr; // 防止悬空指针
}

// 判断队列是否为空
bool QueueEmpty(SqQueue& Q) {
	return Q.front == Q.rear;
}

// 入队
bool QueueEn(SqQueue& Q, BiTree& e) {
	if ((Q.rear + 1) % MaxSize == Q.front) return false; // 队满则报错
	Q.data[Q.rear] = e;
	Q.rear = (Q.rear + 1) % MaxSize; // 队尾指针加 1 取模
	return true;
}

// 出队
bool QueueDe(SqQueue& Q, BiTree& e) {
	if (QueueEmpty(Q)) return false; // 队空则报错
	e = Q.data[Q.front];
	Q.front = (Q.front + 1) % MaxSize;
	return true;
}

//计算符号优先级
int OperatorLevel(char oper)
{
	switch (oper) {
	case '+': return 1;  // 加法
	case '-': return 1;  // 减法
	case '*': return 2;  // 乘法
	case '/': return 2;  // 除法
	case '(': return 3;  // 左括号
	case ')': return 4;  // 右括号
	case '.': return 7;  // 小数点
	case '0': return 9;  // 数字0
	case '1': return 9;  // 数字1
	case '2': return 9;  // 数字2
	case '3': return 9;  // 数字3
	case '4': return 9;  // 数字4
	case '5': return 9;  // 数字5
	case '6': return 9;  // 数字6
	case '7': return 9;  // 数字7
	case '8': return 9;  // 数字8
	case '9': return 9;  // 数字9
	default: ERROR = true; return -314;     // 错误值
	}
}

// 读取多行数字并支持小数
operandType Number(char nums[], int len, int& i)
{
	operandType num = 0.0;  // 存储数字的变量
	double decimalPlace = 1.0;  // 处理小数部分的权重
	bool isDecimal = false;  // 标志是否进入小数部分

	// 连续读取多位数字
	while (i < len)
	{
		if (OperatorLevel(nums[i]) == 9)  // 如果是数字字符
		{
			if (isDecimal)
			{
				decimalPlace *= 0.1;  // 更新小数权重
				num += (nums[i] - '0') * decimalPlace;  // 处理小数部分
			}
			else
			{
				num = num * 10 + (nums[i] - '0');  // 处理整数部分
			}
		}

		if (i < len - 1)
		{
			if (nums[i + 1] == '.')
			{
				if (isDecimal)  // 如果已经有小数点，则返回错误
				{
					ERROR = true; //表达式错误
					break;
				}
				isDecimal = true;  // 标记已经进入小数部分
				++i;  // 继续读取下一个字符
			}
			else if (OperatorLevel(nums[i + 1]) == 9)
			{
				++i;  // 继续读取下一个字符
			}
			else break;
		}
		else break;
	}
	return num;  // 返回最终的数字
}

// 通过给定数组创建二叉树
// nums 为数组、len 为数组长度、i 为当前数组下标
BiTree BiTreeCreatetemp(char nums[], int len, int& i)
{
	if (i >= len || ERROR) return nullptr;
	BiTree node = new BiTNode; //根树
	BiTree lastnode = node; //之前新增的根树开始时等于根树

	for (; i < len; ++i)
	{
		if (ERROR) { return node; } //表达式错误，直接返回根树
		if (OperatorLevel(nums[i]) == 9) //为运算数
		{
			if (node->OperatorLevel == 0) //开始的情况
			{
				node->isOperator = false; //标识为运算数
				node->OperatorLevel = 9; //符号等级为9
				node->Operand = Number(nums, len, i); //连续多位读取数字
			}
			else
			{
				BiTree nodeTemp = new BiTNode; //新建节点
				nodeTemp->isOperator = false; //将新建节点标识为运算数
				nodeTemp->OperatorLevel = 9; //符号等级为9
				nodeTemp->Operand = Number(nums, len, i); //连续多位读取数字
				if (!node->lchild) //根节点的左孩子为空
				{
					node->lchild = nodeTemp; //根节点的左孩子为新建节点
				}
				else if (!node->rchild) //根节点的右孩子为空
				{
					node->rchild = nodeTemp; //根节点的右孩子为新建节点
				}
				else if (!lastnode->lchild) //之前新增的根树的左孩子为空
				{
					lastnode->lchild = nodeTemp; //之前新增的根树的左孩子为新建节点
				}
				else //之前新增的根树的右孩子为空
				{
					lastnode->rchild = nodeTemp; //之前新增的根树的右孩子为新建节点
				}
			}
		}
		else //为运算符
		{
			int currentLevel = OperatorLevel(nums[i]); //计算运算符优先级

			if (currentLevel == 4) //遇到右括号
			{
				if (node->isOperator) //回溯的时候，如果根节点为运算符，则让它的运算数为8，标记为回溯
				{
					node->Operand = 8; //根树的运算数为8，标记为回溯
				}
				return node;
			}
			else if (currentLevel == 3) //遇到左括号
			{
				if (nums[i + 1] == ')') { ERROR = true; } //括号里为空，则表达式错误

				BiTree nodeTemp = BiTreeCreatetemp(nums, len, ++i);

				if (lastnode->OperatorLevel == 0) //之前新增的根树为开始的情况，例如((1)) 无限叠加括号
				{
					delete lastnode;
					node = lastnode = nodeTemp;
				}
				else if (lastnode->isOperator) //之前新增的根树为运算符
				{
					if (!lastnode->lchild) //之前新增的根树的左孩子为空
					{
						lastnode->lchild = nodeTemp;
					}
					else if (!lastnode->rchild) //之前新增的根树的右孩子为空
					{
						lastnode->rchild = nodeTemp;
					}
				}
				else
				{
					ERROR = true; //之前新增的根树为运算数，表达式错误，例如:123(5)
				}
				continue;
			}

			//根节点优先级大于等于当前符号优先级,以及根节点为数，根节点为回溯的情况
			if (node->OperatorLevel >= currentLevel || node->Operand == 8)
			{
				BiTree nodeTemp = new BiTNode; //新建节点
				lastnode = nodeTemp; //将最近一次编辑的节点指向新建节点
				nodeTemp->isOperator = true; //新建节点标识为运算符
				nodeTemp->Operator = nums[i]; //新建节点的符号位为当前遇到的运算符
				nodeTemp->OperatorLevel = currentLevel; //新建节点的符号等级为当前遇到的
				nodeTemp->lchild = node; //将新建节点的左孩子指向根节点
				node = nodeTemp; //根节点指向新建节点
			}
			else //根节点比当前符号优先级低
			{
				BiTree nodeTemp = new BiTNode; //新建节点
				lastnode = nodeTemp; //将最近一次编辑的节点指向新建节点
				nodeTemp->isOperator = true; //新建节点标识为运算符
				nodeTemp->Operator = nums[i]; //新建节点的符号位为当前遇到的运算符
				nodeTemp->OperatorLevel = currentLevel; //新建节点的符号等级为当前遇到的
				if (!node->rchild) //根树的右孩子为空
				{
					node->rchild = nodeTemp; //根树的右孩子为新建节点
				}
				else if (node->rchild) //根树的右孩子不为空
				{
					nodeTemp->lchild = node->rchild; //新建节点的左孩子为根树的右孩子
					node->rchild = nodeTemp; //根树的右孩子为新建节点
				}
				else if (!node->lchild) //根树的左孩子为空
				{
					node->lchild = nodeTemp; //根树的左孩子为新建节点
				}
				else //根树的左孩子不为空
				{
					nodeTemp->lchild = node->lchild; //新建节点的左孩子为根树的左孩子
					node->lchild = nodeTemp; //根树的左孩子为新建节点
				}
			}
		}
	}
	return node; //返回根树
}

//多使用一个函数，这样就不需要对i进行传参，默认从0开始
BiTree BiTreeCreate(char nums[], int len, int i = 0)
{
	BiTree tree = BiTreeCreatetemp(nums, len, i);
	if (i != len) { ERROR = true; } //没有遍历完,例如情况：((2) 左右括号不相等
	return tree;
}

//计算表达式
operandType CalculateExpression(BiTree tree)
{
	if (tree == nullptr) { ERROR = true; return -314; }

	if (tree->isOperator) //为运算符
	{
		operandType a = CalculateExpression(tree->lchild);
		operandType b = CalculateExpression(tree->rchild);
		switch (tree->Operator)
		{
		case '+': return a + b;   // 加法
		case '-': return a - b;   // 减法
		case '*': return a * b;   // 乘法
		case '/': if (b == 0) { ERROR = true; return -314; } return a / b;   // 除法
		default: ERROR = true; return -314;     // 错误值
		}
	}
	else //为运算数
	{
		return tree->Operand;
	}
}

//计算二叉树的叶子结点数
int CalculateLeafNode(BiTree tree)
{
	if (!tree)return 0; //结点为空返回0

	int count = 0;
	if (!tree->lchild && !tree->rchild) //叶子结点
	{
		return 1;
	}
	else
	{
		count += CalculateLeafNode(tree->lchild); //左孩子
		count += CalculateLeafNode(tree->rchild); //右孩子
	}
	return count;
}

//计算二叉树的结点数
int CalculateNode(BiTree tree)
{
	if (!tree)return 0;
	return CalculateNode(tree->lchild) + CalculateNode(tree->rchild) + 1;
}

//计算二叉树的深度
int BiTreeDepth(BiTree tree)
{
	if (!tree)return 0;
	return max(BiTreeDepth(tree->lchild), BiTreeDepth(tree->rchild)) + 1;
}

// 先序遍历: 根左右(NLR)
void OrderPre(BiTree tree, bool head = false)
{
	if (tree == nullptr) return;
	if (head)cout << ", "; //用于末尾不输出", "
	if (tree->isOperator)
	{
		cout << tree->Operator; // 访问根树内容
	}
	else
	{
		cout << tree->Operand; // 访问根树内容
	}
	OrderPre(tree->lchild, true); // 递归遍历左子树
	OrderPre(tree->rchild, true); // 递归遍历右子树
}

// 中序遍历: 左根右(LNR)
void OrderInTemp(BiTree tree, bool& head)
{
	if (tree == nullptr) return;
	OrderInTemp(tree->lchild, head); // 递归遍历左子树
	if (head)cout << ", "; //用于末尾不输出", "
	else head = true;
	if (tree->isOperator)
	{
		cout << tree->Operator; // 访问根树内容
	}
	else
	{
		cout << tree->Operand; // 访问根树内容
	}
	OrderInTemp(tree->rchild, head); // 递归遍历右子树
}

//辅助函数来实现中序遍历
void OrderIn(BiTree tree)
{
	bool head = false;
	OrderInTemp(tree, head);
}

// 后序遍历: 左右根(LRN)
void OrderPost(BiTree tree, bool head = false)
{
	if (tree == nullptr) return;
	OrderPost(tree->lchild, true); // 递归遍历左子树
	OrderPost(tree->rchild, true); // 递归遍历右子树
	if (tree->isOperator)
	{
		cout << tree->Operator; // 访问根树内容
	}
	else
	{
		cout << tree->Operand; // 访问根树内容
	}
	if (head)cout << ", "; //用于末尾不输出", "
}

// 层次遍历
void OrderLevel(BiTree tree) {
	if (tree == nullptr) return;
	::MaxSize = (int)(pow(2, (BiTreeDepth(tree) - 1)) + 1); //队列的最大个数为最后一层的最大结点数
	SqQueue Q; // 初始化队列
	BiTree p = nullptr;
	QueueEn(Q, tree); // 根节点入队
	bool head = false; //用于末尾不输出", "
	while (!QueueEmpty(Q))
	{ // 队列不空则继续遍历
		QueueDe(Q, p); // 队头结点出队
		if (head)cout << ", ";
		if (p->isOperator)
		{
			cout << p->Operator; // 访问根树内容
		}
		else
		{
			cout << p->Operand; // 访问根树内容
		}
		head = true;
		if (p->lchild != nullptr)
			QueueEn(Q, p->lchild); // 左子树不为空，则左子树根入队
		if (p->rchild != nullptr)
			QueueEn(Q, p->rchild); // 右子树不为空，则右子树根入队
	}
	DestroyQueue(Q); // 销毁队列，释放内存
}

//补充0，用于计算负数
string AddZeroMinus(const string& expression)
{
	string result;

	for (size_t i = 0; i < expression.length(); ++i)
	{
		if (expression[i] == '-')
		{
			// 检查前面的字符  
			if (i == 0)// 情况1: 在开头的负号前加0 
			{
				result += '0';
			}
			else if (i > 0 && expression[i - 1] == '(')// 情况2: 在 '(' 和 '-' 之间插入0
			{
				result += '0';
			}
		}
		result += expression[i];  // 添加当前字符  
	}
	return result;
}

//封装成一个函数，传入string字符串即可得出计算结果
operandType calculate(string& expression)
{
	ERROR = false; //开始时默认表达式没有错误
	expression.erase(remove(expression.begin(), expression.end(), ' '), expression.end()); //去掉空格
	string addZeroExpression = AddZeroMinus(expression);

	// 将用户输入的字符转换为字符数组
	int len = addZeroExpression.size();
	char* modifiableStr = new char[len + 1]; // +1容量填充空字符
	copy(addZeroExpression.begin(), addZeroExpression.end(), modifiableStr);
	modifiableStr[len] = '\0'; // 添加字符串结束符

	BiTree tree = BiTreeCreate(modifiableStr, len); //创建二叉树表达式
	cout << "先序遍历：";
	OrderPre(tree);
	cout << endl;

	cout << "中序遍历：";
	OrderIn(tree);
	cout << endl;

	cout << "后序遍历：";
	OrderPost(tree);
	cout << endl;

	// 层次遍历需要借助队列实现
	cout << "层次遍历: ";
	OrderLevel(tree);
	cout << endl;

	operandType result = CalculateExpression(tree); //计算二叉树表达式

	cout << "二叉树的结点数: " << CalculateNode(tree) << endl;
	cout << "二叉树的叶子结点数: " << CalculateLeafNode(tree) << endl;
	cout << "二叉树的深度: " << BiTreeDepth(tree) << endl;
	cout << "转换后的表达式:" << addZeroExpression << endl;
	cout << expression << " = " << result << endl;

	if (ERROR)
	{
		cout << "表达式错误!!!" << endl;
	}

	DestroyBiTree(tree); // 销毁二叉树
	delete[] modifiableStr; // 释放动态分配的内存

	return result; //返回计算结果，如果不需要那些二叉树遍历以及统计只需要表达式计算功能，请自行修改
}

int main(void)
{
	string expression;

	while (true)
	{
		cout << "输入表达式(空行则退出)：";
		getline(cin, expression);
		// 如果输入为空是退出条件
		if (expression.empty()) { break; }

		calculate(expression); //封装成一个函数，传入string字符串即可得出计算结果
	}

	return 0;
}