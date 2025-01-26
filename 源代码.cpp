#include <iostream>
#include <string>
using namespace std;

#define operandType double

bool ERROR = false; //����ȫ�ֱ���ERROR�������жϱ��ʽ�Ƿ��д���

// ���洢�ṹ
typedef struct BiTNode {
	bool isOperator = true; //�Ƿ��������
	char Operator = '#'; //�����
	int OperatorLevel = 0; //���ŵȼ�
	operandType Operand = 0; //������
	struct BiTNode* lchild = nullptr, * rchild = nullptr; // ���Һ���ָ��
} BiTNode, * BiTree;

// ���ٶ�����
void DestroyBiTree(BiTree tree)
{
	if (!tree)return; //�ս�㷵��

	DestroyBiTree(tree->lchild); //����������
	DestroyBiTree(tree->rchild); //����������

	delete tree; //���ٵ�ǰ���
}

// ѭ�������㷨��������ɶ�������α���
int MaxSize = 50;   // ���������Ԫ�ص�������(Ĭ��50)
typedef struct SqQueue {
	BiTree* data = new BiTree[MaxSize]; // ��Ŷ���Ԫ��
	int front = 0, rear = 0;   // ��ͷ�Ͷ�βָ��
} SqQueue;

// ���ٶ���
void DestroyQueue(SqQueue& Q) {
	delete[] Q.data;  // �ͷŶ�̬���������
	Q.data = nullptr; // ��ֹ����ָ��
}

// �ж϶����Ƿ�Ϊ��
bool QueueEmpty(SqQueue& Q) {
	return Q.front == Q.rear;
}

// ���
bool QueueEn(SqQueue& Q, BiTree& e) {
	if ((Q.rear + 1) % MaxSize == Q.front) return false; // �����򱨴�
	Q.data[Q.rear] = e;
	Q.rear = (Q.rear + 1) % MaxSize; // ��βָ��� 1 ȡģ
	return true;
}

// ����
bool QueueDe(SqQueue& Q, BiTree& e) {
	if (QueueEmpty(Q)) return false; // �ӿ��򱨴�
	e = Q.data[Q.front];
	Q.front = (Q.front + 1) % MaxSize;
	return true;
}

//����������ȼ�
int OperatorLevel(char oper)
{
	switch (oper) {
	case '+': return 1;  // �ӷ�
	case '-': return 1;  // ����
	case '*': return 2;  // �˷�
	case '/': return 2;  // ����
	case '(': return 3;  // ������
	case ')': return 4;  // ������
	case '.': return 7;  // С����
	case '0': return 9;  // ����0
	case '1': return 9;  // ����1
	case '2': return 9;  // ����2
	case '3': return 9;  // ����3
	case '4': return 9;  // ����4
	case '5': return 9;  // ����5
	case '6': return 9;  // ����6
	case '7': return 9;  // ����7
	case '8': return 9;  // ����8
	case '9': return 9;  // ����9
	default: ERROR = true; return -314;     // ����ֵ
	}
}

// ��ȡ�������ֲ�֧��С��
operandType Number(char nums[], int len, int& i)
{
	operandType num = 0.0;  // �洢���ֵı���
	double decimalPlace = 1.0;  // ����С�����ֵ�Ȩ��
	bool isDecimal = false;  // ��־�Ƿ����С������

	// ������ȡ��λ����
	while (i < len)
	{
		if (OperatorLevel(nums[i]) == 9)  // ����������ַ�
		{
			if (isDecimal)
			{
				decimalPlace *= 0.1;  // ����С��Ȩ��
				num += (nums[i] - '0') * decimalPlace;  // ����С������
			}
			else
			{
				num = num * 10 + (nums[i] - '0');  // ������������
			}
		}

		if (i < len - 1)
		{
			if (nums[i + 1] == '.')
			{
				if (isDecimal)  // ����Ѿ���С���㣬�򷵻ش���
				{
					ERROR = true; //���ʽ����
					break;
				}
				isDecimal = true;  // ����Ѿ�����С������
				++i;  // ������ȡ��һ���ַ�
			}
			else if (OperatorLevel(nums[i + 1]) == 9)
			{
				++i;  // ������ȡ��һ���ַ�
			}
			else break;
		}
		else break;
	}
	return num;  // �������յ�����
}

// ͨ���������鴴��������
// nums Ϊ���顢len Ϊ���鳤�ȡ�i Ϊ��ǰ�����±�
BiTree BiTreeCreatetemp(char nums[], int len, int& i)
{
	if (i >= len || ERROR) return nullptr;
	BiTree node = new BiTNode; //����
	BiTree lastnode = node; //֮ǰ�����ĸ�����ʼʱ���ڸ���

	for (; i < len; ++i)
	{
		if (ERROR) { return node; } //���ʽ����ֱ�ӷ��ظ���
		if (OperatorLevel(nums[i]) == 9) //Ϊ������
		{
			if (node->OperatorLevel == 0) //��ʼ�����
			{
				node->isOperator = false; //��ʶΪ������
				node->OperatorLevel = 9; //���ŵȼ�Ϊ9
				node->Operand = Number(nums, len, i); //������λ��ȡ����
			}
			else
			{
				BiTree nodeTemp = new BiTNode; //�½��ڵ�
				nodeTemp->isOperator = false; //���½��ڵ��ʶΪ������
				nodeTemp->OperatorLevel = 9; //���ŵȼ�Ϊ9
				nodeTemp->Operand = Number(nums, len, i); //������λ��ȡ����
				if (!node->lchild) //���ڵ������Ϊ��
				{
					node->lchild = nodeTemp; //���ڵ������Ϊ�½��ڵ�
				}
				else if (!node->rchild) //���ڵ���Һ���Ϊ��
				{
					node->rchild = nodeTemp; //���ڵ���Һ���Ϊ�½��ڵ�
				}
				else if (!lastnode->lchild) //֮ǰ�����ĸ���������Ϊ��
				{
					lastnode->lchild = nodeTemp; //֮ǰ�����ĸ���������Ϊ�½��ڵ�
				}
				else //֮ǰ�����ĸ������Һ���Ϊ��
				{
					lastnode->rchild = nodeTemp; //֮ǰ�����ĸ������Һ���Ϊ�½��ڵ�
				}
			}
		}
		else //Ϊ�����
		{
			int currentLevel = OperatorLevel(nums[i]); //������������ȼ�

			if (currentLevel == 4) //����������
			{
				if (node->isOperator) //���ݵ�ʱ��������ڵ�Ϊ���������������������Ϊ8�����Ϊ����
				{
					node->Operand = 8; //������������Ϊ8�����Ϊ����
				}
				return node;
			}
			else if (currentLevel == 3) //����������
			{
				if (nums[i + 1] == ')') { ERROR = true; } //������Ϊ�գ�����ʽ����

				BiTree nodeTemp = BiTreeCreatetemp(nums, len, ++i);

				if (lastnode->OperatorLevel == 0) //֮ǰ�����ĸ���Ϊ��ʼ�����������((1)) ���޵�������
				{
					delete lastnode;
					node = lastnode = nodeTemp;
				}
				else if (lastnode->isOperator) //֮ǰ�����ĸ���Ϊ�����
				{
					if (!lastnode->lchild) //֮ǰ�����ĸ���������Ϊ��
					{
						lastnode->lchild = nodeTemp;
					}
					else if (!lastnode->rchild) //֮ǰ�����ĸ������Һ���Ϊ��
					{
						lastnode->rchild = nodeTemp;
					}
				}
				else
				{
					ERROR = true; //֮ǰ�����ĸ���Ϊ�����������ʽ��������:123(5)
				}
				continue;
			}

			//���ڵ����ȼ����ڵ��ڵ�ǰ�������ȼ�,�Լ����ڵ�Ϊ�������ڵ�Ϊ���ݵ����
			if (node->OperatorLevel >= currentLevel || node->Operand == 8)
			{
				BiTree nodeTemp = new BiTNode; //�½��ڵ�
				lastnode = nodeTemp; //�����һ�α༭�Ľڵ�ָ���½��ڵ�
				nodeTemp->isOperator = true; //�½��ڵ��ʶΪ�����
				nodeTemp->Operator = nums[i]; //�½��ڵ�ķ���λΪ��ǰ�����������
				nodeTemp->OperatorLevel = currentLevel; //�½��ڵ�ķ��ŵȼ�Ϊ��ǰ������
				nodeTemp->lchild = node; //���½��ڵ������ָ����ڵ�
				node = nodeTemp; //���ڵ�ָ���½��ڵ�
			}
			else //���ڵ�ȵ�ǰ�������ȼ���
			{
				BiTree nodeTemp = new BiTNode; //�½��ڵ�
				lastnode = nodeTemp; //�����һ�α༭�Ľڵ�ָ���½��ڵ�
				nodeTemp->isOperator = true; //�½��ڵ��ʶΪ�����
				nodeTemp->Operator = nums[i]; //�½��ڵ�ķ���λΪ��ǰ�����������
				nodeTemp->OperatorLevel = currentLevel; //�½��ڵ�ķ��ŵȼ�Ϊ��ǰ������
				if (!node->rchild) //�������Һ���Ϊ��
				{
					node->rchild = nodeTemp; //�������Һ���Ϊ�½��ڵ�
				}
				else if (node->rchild) //�������Һ��Ӳ�Ϊ��
				{
					nodeTemp->lchild = node->rchild; //�½��ڵ������Ϊ�������Һ���
					node->rchild = nodeTemp; //�������Һ���Ϊ�½��ڵ�
				}
				else if (!node->lchild) //����������Ϊ��
				{
					node->lchild = nodeTemp; //����������Ϊ�½��ڵ�
				}
				else //���������Ӳ�Ϊ��
				{
					nodeTemp->lchild = node->lchild; //�½��ڵ������Ϊ����������
					node->lchild = nodeTemp; //����������Ϊ�½��ڵ�
				}
			}
		}
	}
	return node; //���ظ���
}

//��ʹ��һ�������������Ͳ���Ҫ��i���д��Σ�Ĭ�ϴ�0��ʼ
BiTree BiTreeCreate(char nums[], int len, int i = 0)
{
	BiTree tree = BiTreeCreatetemp(nums, len, i);
	if (i != len) { ERROR = true; } //û�б�����,���������((2) �������Ų����
	return tree;
}

//������ʽ
operandType CalculateExpression(BiTree tree)
{
	if (tree == nullptr) { ERROR = true; return -314; }

	if (tree->isOperator) //Ϊ�����
	{
		operandType a = CalculateExpression(tree->lchild);
		operandType b = CalculateExpression(tree->rchild);
		switch (tree->Operator)
		{
		case '+': return a + b;   // �ӷ�
		case '-': return a - b;   // ����
		case '*': return a * b;   // �˷�
		case '/': if (b == 0) { ERROR = true; return -314; } return a / b;   // ����
		default: ERROR = true; return -314;     // ����ֵ
		}
	}
	else //Ϊ������
	{
		return tree->Operand;
	}
}

//�����������Ҷ�ӽ����
int CalculateLeafNode(BiTree tree)
{
	if (!tree)return 0; //���Ϊ�շ���0

	int count = 0;
	if (!tree->lchild && !tree->rchild) //Ҷ�ӽ��
	{
		return 1;
	}
	else
	{
		count += CalculateLeafNode(tree->lchild); //����
		count += CalculateLeafNode(tree->rchild); //�Һ���
	}
	return count;
}

//����������Ľ����
int CalculateNode(BiTree tree)
{
	if (!tree)return 0;
	return CalculateNode(tree->lchild) + CalculateNode(tree->rchild) + 1;
}

//��������������
int BiTreeDepth(BiTree tree)
{
	if (!tree)return 0;
	return max(BiTreeDepth(tree->lchild), BiTreeDepth(tree->rchild)) + 1;
}

// �������: ������(NLR)
void OrderPre(BiTree tree, bool head = false)
{
	if (tree == nullptr) return;
	if (head)cout << ", "; //����ĩβ�����", "
	if (tree->isOperator)
	{
		cout << tree->Operator; // ���ʸ�������
	}
	else
	{
		cout << tree->Operand; // ���ʸ�������
	}
	OrderPre(tree->lchild, true); // �ݹ����������
	OrderPre(tree->rchild, true); // �ݹ����������
}

// �������: �����(LNR)
void OrderInTemp(BiTree tree, bool& head)
{
	if (tree == nullptr) return;
	OrderInTemp(tree->lchild, head); // �ݹ����������
	if (head)cout << ", "; //����ĩβ�����", "
	else head = true;
	if (tree->isOperator)
	{
		cout << tree->Operator; // ���ʸ�������
	}
	else
	{
		cout << tree->Operand; // ���ʸ�������
	}
	OrderInTemp(tree->rchild, head); // �ݹ����������
}

//����������ʵ���������
void OrderIn(BiTree tree)
{
	bool head = false;
	OrderInTemp(tree, head);
}

// �������: ���Ҹ�(LRN)
void OrderPost(BiTree tree, bool head = false)
{
	if (tree == nullptr) return;
	OrderPost(tree->lchild, true); // �ݹ����������
	OrderPost(tree->rchild, true); // �ݹ����������
	if (tree->isOperator)
	{
		cout << tree->Operator; // ���ʸ�������
	}
	else
	{
		cout << tree->Operand; // ���ʸ�������
	}
	if (head)cout << ", "; //����ĩβ�����", "
}

// ��α���
void OrderLevel(BiTree tree) {
	if (tree == nullptr) return;
	::MaxSize = (int)(pow(2, (BiTreeDepth(tree) - 1)) + 1); //���е�������Ϊ���һ����������
	SqQueue Q; // ��ʼ������
	BiTree p = nullptr;
	QueueEn(Q, tree); // ���ڵ����
	bool head = false; //����ĩβ�����", "
	while (!QueueEmpty(Q))
	{ // ���в������������
		QueueDe(Q, p); // ��ͷ������
		if (head)cout << ", ";
		if (p->isOperator)
		{
			cout << p->Operator; // ���ʸ�������
		}
		else
		{
			cout << p->Operand; // ���ʸ�������
		}
		head = true;
		if (p->lchild != nullptr)
			QueueEn(Q, p->lchild); // ��������Ϊ�գ��������������
		if (p->rchild != nullptr)
			QueueEn(Q, p->rchild); // ��������Ϊ�գ��������������
	}
	DestroyQueue(Q); // ���ٶ��У��ͷ��ڴ�
}

//����0�����ڼ��㸺��
string AddZeroMinus(const string& expression)
{
	string result;

	for (size_t i = 0; i < expression.length(); ++i)
	{
		if (expression[i] == '-')
		{
			// ���ǰ����ַ�  
			if (i == 0)// ���1: �ڿ�ͷ�ĸ���ǰ��0 
			{
				result += '0';
			}
			else if (i > 0 && expression[i - 1] == '(')// ���2: �� '(' �� '-' ֮�����0
			{
				result += '0';
			}
		}
		result += expression[i];  // ��ӵ�ǰ�ַ�  
	}
	return result;
}

//��װ��һ������������string�ַ������ɵó�������
operandType calculate(string& expression)
{
	ERROR = false; //��ʼʱĬ�ϱ��ʽû�д���
	expression.erase(remove(expression.begin(), expression.end(), ' '), expression.end()); //ȥ���ո�
	string addZeroExpression = AddZeroMinus(expression);

	// ���û�������ַ�ת��Ϊ�ַ�����
	int len = addZeroExpression.size();
	char* modifiableStr = new char[len + 1]; // +1���������ַ�
	copy(addZeroExpression.begin(), addZeroExpression.end(), modifiableStr);
	modifiableStr[len] = '\0'; // ����ַ���������

	BiTree tree = BiTreeCreate(modifiableStr, len); //�������������ʽ
	cout << "���������";
	OrderPre(tree);
	cout << endl;

	cout << "���������";
	OrderIn(tree);
	cout << endl;

	cout << "���������";
	OrderPost(tree);
	cout << endl;

	// ��α�����Ҫ��������ʵ��
	cout << "��α���: ";
	OrderLevel(tree);
	cout << endl;

	operandType result = CalculateExpression(tree); //������������ʽ

	cout << "�������Ľ����: " << CalculateNode(tree) << endl;
	cout << "��������Ҷ�ӽ����: " << CalculateLeafNode(tree) << endl;
	cout << "�����������: " << BiTreeDepth(tree) << endl;
	cout << "ת����ı��ʽ:" << addZeroExpression << endl;
	cout << expression << " = " << result << endl;

	if (ERROR)
	{
		cout << "���ʽ����!!!" << endl;
	}

	DestroyBiTree(tree); // ���ٶ�����
	delete[] modifiableStr; // �ͷŶ�̬������ڴ�

	return result; //���ؼ��������������Ҫ��Щ�����������Լ�ͳ��ֻ��Ҫ���ʽ���㹦�ܣ��������޸�
}

int main(void)
{
	string expression;

	while (true)
	{
		cout << "������ʽ(�������˳�)��";
		getline(cin, expression);
		// �������Ϊ�����˳�����
		if (expression.empty()) { break; }

		calculate(expression); //��װ��һ������������string�ַ������ɵó�������
	}

	return 0;
}