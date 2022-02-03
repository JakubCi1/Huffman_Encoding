#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <string>


using namespace std;

struct node
{
	int value{};
	bool leaf_node{};
	char stored_char{};
	struct node *left, *right;
	void combineNodes(node l, node r);
	bool findEncoding(char a, vector<char>* encoding);
};

node createNewNode(int val, char a);

int main()
{
	string* path = new string;
	cout << "Input text file path:" << endl;
	getline(cin, *path);
	ifstream readData(*path);

	char* textCharacters_temp = new char[128]{};
	int* charAmmount_temp = new int[128]{};
	int* ln = new int{};

	if (readData)
	{
		char sign;
		

		while (readData.get(sign))
		{
			bool in_array = false;
			int i = 0;
			while (textCharacters_temp[i] != '\0')
			{
				if (sign == textCharacters_temp[i])
				{
					in_array = true;
					charAmmount_temp[i] += 1;
				}
				i += 1;
			}

			if (in_array == true) continue;

			textCharacters_temp[*ln] = sign;
			charAmmount_temp[*ln] += 1;
			*ln += 1;
		}
	}
	else
	{
		cout << "FILE NOT FOUND" << endl;
		return -1;
	}
		
	readData.close();

	char* textCharacters = new char[*ln];
	int* charAmmount = new int[*ln];

	for (int i = 0; i < 128; i++)
	{
		if (textCharacters_temp[i] == '\0') break;
		textCharacters[i] = textCharacters_temp[i];
		charAmmount[i] = charAmmount_temp[i];
	}

	delete[] charAmmount_temp;
	delete[] textCharacters_temp;
	charAmmount_temp = nullptr;
	textCharacters_temp = nullptr;

	for (int i = 0; i < *ln; i++)
	{
		for (int j = i + 1; j < *ln; j++)
		{
			if (charAmmount[i] > charAmmount[j])
			{
				swap(charAmmount[i], charAmmount[j]);
				swap(textCharacters[i], textCharacters[j]);
			}
		}
	}

	node* nodeArray = new node[1024];

	for (int i = 0; i < *ln; i++)
	{
		nodeArray[i] = createNewNode(charAmmount[i], textCharacters[i]);
	}

	/*for (int i = 0; i < *ln; i++)
	{
		cout << "znak: " << nodeArray[i].stored_char << " wartosc: " << nodeArray[i].value << " leaf node: " << (bool)nodeArray[i].leaf_node << endl;
	}*/
	int last_in_array = *ln - 1, first_in_array = 0;
	int i{};
	int ln_cpy = *ln;

	while (*ln > 1)
	{
		node n;
		n.combineNodes(nodeArray[i], nodeArray[i + 1]);
		last_in_array++;
		if (nodeArray[last_in_array - 1].value > n.value)
		{
			int j;
			for (j = i+1; j <= last_in_array-1; j++)
			{
				if (nodeArray[j].value > n.value)
					break;
			}

			for (int k = last_in_array - 1; k >= j; k--)
			{
				nodeArray[k + 1] = nodeArray[k];
			}
			nodeArray[j] = n;
		}
		else
		{
			nodeArray[last_in_array] = n;
		}



		i += 2;
		*ln -= 1;
	}

	
	/*vector<char> encodingVector;
	for (int i = 0; i < 9; i++)
	{
		bool found = combinedNodes[i].findEncoding('a', &encodingVector);
		if (found)
		{
			for (int j = 0; j < encodingVector.size(); j++)
			{
				cout << encodingVector[j] << " " << endl;
			}
		}
	}*/

	
	

	ofstream dataWrite("../kodowania.txt");

	if (!dataWrite)
	{
		cout << "ERROR when writing wile, does specified path exists?" << endl;
		return -1;
	}

	for (int i = 0; i < ln_cpy; i++)
	{
		vector<char> encodingVector;
		nodeArray[last_in_array].findEncoding(textCharacters[i], &encodingVector);
		if (textCharacters[i] == 10 || textCharacters[i] == 13)
		{
			dataWrite << "ENTER";
		}
		else
		{
			dataWrite << textCharacters[i];
		}
		dataWrite << ": ";
		for (int j = 0; j < encodingVector.size(); j++)
		{
			dataWrite << encodingVector[j];
		}
		dataWrite << "\n";
	}

	dataWrite.close();
	delete path;
	path = nullptr;

	//for (int i = 0; i <= last_in_array; i++)
	//	cout << nodeArray[i].value << endl;
	///*cout << nodeArray[last_in_array].right->right->left->left->right->stored_char << endl;*/
	/*cout << nodeArray[last_in_array].left->right->right->stored_char << endl;*/


	delete[] charAmmount;
	delete[] textCharacters;
	charAmmount = nullptr;
	textCharacters = nullptr;
	delete[] nodeArray;
	nodeArray = nullptr;
	delete ln;
	ln = nullptr;
	return 0;
}

node createNewNode(int val, char a)
{
	node t;
	t.value = val;
	t.stored_char = a;
	t.leaf_node = true;
	t.left = nullptr;
	t.right = nullptr;
	return t;
}

void node::combineNodes(node l, node r)
{
	 
	node* pl = new node;
	pl->value = l.value;
	pl->stored_char = l.stored_char;
	pl->left = l.left;
	pl->right = l.right;
	pl->leaf_node = l.leaf_node;
	node* pr = new node;
	pr->value = r.value;
	pr->stored_char = r.stored_char;
	pr->left = r.left;
	pr->right = r.right;
	pr->leaf_node = r.leaf_node;
	value = l.value + r.value;
	leaf_node = false;
	stored_char = '\0';
	left = pl;
	right = pr;
}

bool node::findEncoding(char a, vector<char>* encoding)
{
	if (right == nullptr && left == nullptr)
	{
		if (stored_char == a)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	if (!(left == nullptr))
	{
		encoding->push_back('0');
		bool found = left->findEncoding(a, encoding);
		if (found == true)
		{
			return true;
		}
		else encoding->pop_back();
	}

	if (!(right == nullptr))
	{
		encoding->push_back('1');
		bool found = right->findEncoding(a, encoding);
		if (found == true)
		{
			return true;
		}
		else encoding->pop_back();
	}

	return false;
}
