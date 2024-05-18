#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>

struct node
{
	struct node* child;
	struct node* brother;
	struct node* parent;
	int key;
};

struct tree
{
	struct node* root;
	int id;
	int leaves;
	int internal_nodes;
	int* leaf;
	int* internal;
};

void addChild(tree& drzewo)
{
	drzewo.root->child = new node;

	drzewo.root->child->child = NULL;
	drzewo.root->child->brother = NULL;
	drzewo.root->child->parent = drzewo.root;

	drzewo.root->child->key = (-1) * drzewo.internal_nodes;
	drzewo.internal_nodes++;

	drzewo.root = drzewo.root->child;
}

void setKey(tree& drzewo, int key)
{
	drzewo.root->key = key;
	drzewo.leaves++;
	drzewo.internal_nodes--;
}

void addBrother(tree& drzewo)
{
	drzewo.root->brother = new node;

	drzewo.root->brother->child = NULL;
	drzewo.root->brother->brother = NULL;
	drzewo.root->brother->parent = drzewo.root->parent;

	drzewo.root->brother->key = (-1) * drzewo.internal_nodes;
	drzewo.internal_nodes++;

	drzewo.root = drzewo.root->brother;
}

void backToParent(tree& drzewo)
{
	drzewo.root = drzewo.root->parent;
}

void putLeavesToTab(node* root, int* leaves)
{
	static int i = 0;

	if (!root)
		return;

	if (!root->parent) i = 0;

	if (!root->child)
	{
		leaves[i] = root->key;
		i++;
	}

	if (root->child)
		putLeavesToTab(root->child, leaves);

	if (root->brother)
		putLeavesToTab(root->brother, leaves);
}

void putInternalToTab(node* root, int* leaves)
{
	static int i = 0;

	if (!root)
		return;

	if (!root->parent) i = 0;

	if (root->child)
	{
		leaves[i] = root->key;
		i++;
	}

	if (root->child)
		putInternalToTab(root->child, leaves);

	if (root->brother)
		putInternalToTab(root->brother, leaves);
}

struct node* find_node(struct node* current, int key)
{
	if (current->key == key)
	{
		return current;
	}
	if (current->child)
	{
		struct node* result = find_node(current->child, key);
		if (result != NULL)
		{
			return result;
		}
	}
	if (current->brother)
	{
		struct node* result = find_node(current->brother, key);
		if (result != NULL)
		{
			return result;
		}
	}
	return NULL;
}

bool has(struct node* current, int key)
{
	struct node* child = current->child;

	while (child)
	{
		if (child->key == key)
		{
			return true;
		}
		else if (has(child, key))
		{
			return true;
		}
		child = child->brother;
	}
	return false;
}

int policzDzieci(node* root)
{
	if (!root->child)
		return 0;
	else
	{
		int dzieci = 0;
		root = root->child;

		while (root)
		{
			root = root->brother;
			dzieci++;
		}
		return dzieci;
	}
}

bool next_permutation(int* array, int length)
{
	int i = length - 1;
	while (i > 0 && array[i - 1] >= array[i])
	{
		i--;
	}
	if (i == 0)
		return false;

	int j = length - 1;
	while (array[j] <= array[i - 1])
	{
		j--;
	}
	int temp = array[i - 1];
	array[i - 1] = array[j];
	array[j] = temp;

	j = length - 1;
	while (i < j)
	{
		temp = array[i];
		array[i] = array[j];
		array[j] = temp;
		i++;
		j--;
	}
	return true;
}

int find_max_sum(int* tab, int rows, int cols)
{
	int* cols_order = new int[cols];
	int* rows_order = new int[rows];
	for (int col = 0; col < cols; ++col)
	{
		cols_order[col] = col;
	}
	for (int row = 0; row < rows; ++row)
	{
		rows_order[row] = row;
	}
	int max = 0;
	do
	{
		do
		{
			int sum = 0;
			int limit = cols < rows ? cols : rows;
			for (int i = 0; i < limit; ++i)
			{
				sum += tab[rows_order[i] * cols + cols_order[i]];
			}
			if (sum > max)
			{
				max = sum;
			}
		} while (next_permutation(cols_order, cols));
	} while (next_permutation(rows_order, rows));
	delete[] rows_order;
	delete[] cols_order;
	return max;
}

void compareTrees(tree drzewo1, tree drzewo2)
{
	//printf("%d z %d\n", drzewo1.id + 1, drzewo2.id + 1);

	int** tab = new int* [drzewo1.leaves + drzewo1.internal_nodes]();
	for (int i = 0; i < (drzewo1.leaves + drzewo1.internal_nodes); i++)
		tab[i] = new int[drzewo2.leaves + drzewo2.internal_nodes]();

	// pierwsza czesc
	for (int i = 0; i < drzewo1.leaves; i++)
		for (int j = 0; j < drzewo2.leaves; j++)
			if (drzewo1.leaf[i] == drzewo2.leaf[j])
				tab[i][j] = 1;

	// druga czesc - 1
	for (int i = 0; i < drzewo1.leaves; i++)
	{
		for (int j = drzewo2.leaves; j < (drzewo2.leaves + drzewo2.internal_nodes); j++)
		{
			node* tmp = find_node(drzewo2.root, drzewo2.internal[j - drzewo2.leaves]);

			if (has(tmp, drzewo1.leaf[i]))
				tab[i][j] = 1;
		}
	}

	// druga czesc - 2
	for (int i = drzewo1.leaves; i < (drzewo1.leaves + drzewo1.internal_nodes); i++)
	{
		for (int j = 0; j < drzewo2.leaves; j++)
		{
			node* tmp = find_node(drzewo1.root, drzewo1.internal[i - drzewo1.leaves]);

			if (has(tmp, drzewo2.leaf[j]))
				tab[i][j] = 1;
		}
	}

	// trzecia czesc
	for (int i = (drzewo1.leaves + drzewo1.internal_nodes) - 1; i >= drzewo1.leaves; i--)
	{
		for (int j = (drzewo2.leaves + drzewo2.internal_nodes) - 1; j >= drzewo2.leaves; j--)
		{
			node* wezelDrzewa1 = find_node(drzewo1.root, drzewo1.internal[i - drzewo1.leaves]);
			node* wezelDrzewa2 = find_node(drzewo2.root, drzewo2.internal[j - drzewo2.leaves]);

			int wiersze = policzDzieci(wezelDrzewa1);
			int kolumny = policzDzieci(wezelDrzewa2);

			// tworzenie malej tabeli
			int* new_tab = new int[wiersze * kolumny];

			node* wskaznikNaDziecko1 = wezelDrzewa1->child;
			int ile_dodano = 0;

			for (int k = 0; k < wiersze; k++)
			{
				node* wskaznikNaDziecko2 = wezelDrzewa2->child;

				for (int l = 0; l < kolumny; l++)
				{
					int index1 = -1, index2 = -1;

					for (int m = 0; m < drzewo1.leaves; m++)
					{
						if (drzewo1.leaf[m] == wskaznikNaDziecko1->key)
							index1 = m;
					}

					if (index1 < 0)
					{
						for (int m = 0; m < drzewo1.internal_nodes; m++)
						{
							if (drzewo1.internal[m] == wskaznikNaDziecko1->key)
								index1 = m + drzewo1.leaves;
						}
					}

					for (int m = 0; m < drzewo2.leaves; m++)
					{
						if (drzewo2.leaf[m] == wskaznikNaDziecko2->key)
							index2 = m;
					}

					if (index2 < 0)
					{
						for (int m = 0; m < drzewo2.internal_nodes; m++)
						{
							if (drzewo2.internal[m] == wskaznikNaDziecko2->key)
								index2 = m + drzewo2.leaves;
						}
					}

					new_tab[ile_dodano] = tab[index1][index2];
					ile_dodano++;

					wskaznikNaDziecko2 = wskaznikNaDziecko2->brother;
				}
				wskaznikNaDziecko1 = wskaznikNaDziecko1->brother;
			}
			// mala tabelka gotowa
			int suma = find_max_sum(new_tab, wiersze, kolumny);

			// sprawdzenie czy > niz suma
			wskaznikNaDziecko1 = wezelDrzewa1->child;
			for (int k = 0; k < wiersze; k++)
			{
				//printf("Czytam tab[%d][%d]\n", wskaznikNaDziecko1->key, j);
				for (int l = 0; l < (drzewo1.leaves + drzewo1.internal_nodes); l++)
				{
					if (l < drzewo1.leaves)
					{
						//sprawdz liscie
						if (drzewo1.leaf[l] == wskaznikNaDziecko1->key)
						{
							//printf("Czytam tab[%d][%d]\n", l, j);
							if (tab[l][j] > suma)
								suma = tab[l][j];
						}
					}
					else
					{
						//sprawdz wezly
						if (drzewo1.internal[l - drzewo1.leaves] == wskaznikNaDziecko1->key)
						{
							//printf("Czytam tab[%d][%d]\n", l, j);
							if (tab[l][j] > suma)
								suma = tab[l][j];
						}
					}
				}
				wskaznikNaDziecko1 = wskaznikNaDziecko1->brother;
			}

			// sprawdzenie czy > niz suma
			node* wskaznikNaDziecko2 = wezelDrzewa2->child;
			for (int k = 0; k < kolumny; k++)
			{
				//printf("Czytam tab[%d][%d]\n", wezelDrzewa1->key, wskaznikNaDziecko2->key);
				for (int l = 0; l < (drzewo2.leaves + drzewo2.internal_nodes); l++)
				{
					if (l < drzewo2.leaves)
					{
						//sprawdz liscie
						if (drzewo2.leaf[l] == wskaznikNaDziecko2->key)
						{
							//printf("Czytam tab[%d][%d]\n", i, l);
							if (tab[i][l] > suma)
								suma = tab[i][l];
						}
					}
					else
					{
						//sprawdz wezly
						if (drzewo2.internal[l - drzewo2.leaves] == wskaznikNaDziecko2->key)
						{
							//printf("Czytam tab[%d][%d]\n", i, l);
							if (tab[i][l] > suma)
								suma = tab[i][l];
						}
					}
				}
				wskaznikNaDziecko2 = wskaznikNaDziecko2->brother;
			}
			tab[i][j] = suma;
			delete[] new_tab;
		}
	}

	printf("%d\n", drzewo1.leaves - tab[drzewo1.leaves][drzewo2.leaves]);

	for (int i = 0; i < (drzewo1.leaves + drzewo1.internal_nodes); i++)
		delete[] tab[i];
	delete[] tab;
}

void compareOrder(int n, tree* drzewa)
{
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			if (i < j)
				compareTrees(drzewa[i], drzewa[j]);
}

int main()
{
	int n, key;
	char cmd;

	scanf("%d", &n);
	tree* drzewa = new tree[n];

	for (int i = 0; i < n; i++)
	{
		cmd = 0;
		drzewa[i].root = new node;

		drzewa[i].root->child = NULL;
		drzewa[i].root->brother = NULL;
		drzewa[i].root->parent = NULL;
		drzewa[i].root->key = 0;

		drzewa[i].id = i;
		drzewa[i].leaves = 0;
		drzewa[i].internal_nodes = 1;

		while (cmd != ';')
		{
			scanf("%c", &cmd);
			switch (cmd)
			{
			case '(':
				addChild(drzewa[i]);
				if (scanf("%d", &key) > 0)
					setKey(drzewa[i], key);
				break;
			case ',':
				addBrother(drzewa[i]);
				if (scanf("%d", &key) > 0)
					setKey(drzewa[i], key);
				break;
			case ')':
				backToParent(drzewa[i]);
				break;
			}
		}
		drzewa[i].leaf = new int[drzewa[i].leaves];
		drzewa[i].internal = new int[drzewa[i].internal_nodes];

		putLeavesToTab(drzewa[i].root, drzewa[i].leaf);
		putInternalToTab(drzewa[i].root, drzewa[i].internal);
	}

	compareOrder(n, drzewa);

	delete[] drzewa;
	return 0;
}