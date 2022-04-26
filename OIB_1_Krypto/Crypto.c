// Made by Y. Sendov. February 2022

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <ctype.h>

#define CHANGES_MAX_SIZE 1000
#define ALPHABET_SIZE 33

struct story
{
	int bufsize; // Number of all characters
	int counts; // Counter for the history of changes
	char lastletter1, lastletter2; // For the function of returning the last action
	char changes[CHANGES_MAX_SIZE]; // To save the history of changes
};

struct alphabet
{
	char ralpha[ALPHABET_SIZE]; // Uppercase alphabet
	char ralpha2[ALPHABET_SIZE]; // Lowercase alphabet
	char russ[ALPHABET_SIZE]; // Letters in descending order of frequency
};

int security(char input[3])
{
	int marker = 0;
	int counter = 0;
	for (unsigned int i = 0; i < strlen(input); i++)
	{
		if ((input[i] < 48 || input[i] > 57) && input[i] != 10 && input[i] != 12 && input[i] != 13 && input[i] != 133)
		{
			counter += 1;
		}
		else
		{
			marker = 0;
		}
	}
	if (counter > 0)
	{
		marker += 1;
	}
	return marker;
}

char gethigh(struct alphabet * dict)
{
	int mark = 0;
	char input[3];
	fgets(input, 3, stdin);
	input[strcspn(input, "\n")] = 0;
	fseek(stdin, 0, SEEK_END);
	if (strlen(input) > 1) return 0;
	else
	{
		for (int i = 0; i < 32; i++)
		{
			if (input[0] == dict->ralpha[i])
			{
				mark = 1;
				break;
			}
		}
		if (mark == 1)
		{
			mark = 0;
			return input[0];
		}
		else return 0;
	}
}

char getlow(struct alphabet * dict)
{
	int mark = 0;
	char input[3];
	fgets(input, 3, stdin);
	input[strcspn(input, "\n")] = 0;
	fseek(stdin, 0, SEEK_END);
	if (strlen(input) > 1) return 0;
	else
	{
		for (int i = 0; i < 32; i++)
		{
			if (input[0] == dict->ralpha2[i])
			{
				mark = 1;
				break;
			}
		}
		if (mark == 1)
		{
			mark = 0;
			return input[0];
		}
		else return 0;
	}
}

void error()
{
	printf("�� ����������� ����� ����� ������! ���������� ��� ���.\n");
}

// Frequency analysis of the cryptogram
void freq(struct alphabet * dict)
{
	system("cls");
	printf("��������� ������ ������������:\n\n");
	double symbols[33] = { 0 };
	char symbol;
	int countchar = 0;
	FILE* file = fopen("lab_var_2.txt", "r");
	if (file == NULL)
	{
		system("cls");
		printf("������! �� ��� ������ �������� ���� � �������������. ��������: lab_var_2.txt\n");
		printf("��������� ������� ����� � ��������� ��������� ��� ���.\n");
		exit(-1);
	}
	// Reading each letter from the cryptogram, counting the number of each letter and the number of all letters
	while (!feof(file))
	{
		symbol = fgetc(file);
		for (int i = 0; i < 32; i++)
		{
			if (symbol == dict->ralpha[i])
			{
				countchar++;
				symbols[i]++;
			}
		}
	}
	// Calculating the frequency of occurrence of each letter in a cryptogram
	for (int i = 0; i < 32; i++)
	{
		symbols[i] = symbols[i] / countchar;
	}
	// Sorting the letters of the cryptogram in descending frequency
	for (int i = 0; i < 32; i++)
	{
		for (int j = 0; j < 32 - i; j++)
		{
			if (symbols[j + 1] > symbols[j])
			{
				double temp = symbols[j];
				symbols[j] = symbols[j + 1];
				symbols[j + 1] = temp;
				char tempc = dict->ralpha[j];
				dict->ralpha[j] = dict->ralpha[j + 1];
				dict->ralpha[j + 1] = tempc;
			}
		}
	}
	// Output of frequency analysis to the screen
	for (int i = 0; i < 32; i++)
	{
		if (symbols[i] == 0) break;
		printf("%c - %lf - %c\n", dict->ralpha[i], symbols[i] , dict->russ[i]);
	}
	fclose(file);
}

// Autocorrect letters by frequency analysis
void autochange(char* buf, struct story * name, struct alphabet * dict, int flag)
{
	system("cls");
	if (flag == 0)
	{
		printf("�������������� ������ ���� ����� ��������� ������ ����� ���������� ������� ������������!\n");
		return;
	}
	printf("�������������� ������ ���� \"Cryptanalysis\"\n\n");
	for (int i = 0; i < 32; i++)
	{
		for (int j = 0; j < name->bufsize; j++)
		{
			if (buf[j] == dict->ralpha[i])
			{
				buf[j] = dict->russ[i];
			}
		}
	}
	// Memorizing the history of letter replacement
	for (int i = 0; i < 32; i++)
	{
		name->changes[name->counts] = dict->ralpha[i];
		name->changes[name->counts + 1] = dict->russ[i];
		name->counts = name->counts + 2;
	}
	name->lastletter1 = name->changes[name->counts - 2];
	name->lastletter2 = name->changes[name->counts - 1];
	for (int i = 0; i < name->bufsize; i++)
	{
		printf("%c", buf[i]);
	}
	printf("\n");
}

// Printing a cryptogram
void crypto(char* buf, struct story * name)
{
	system("cls");
	for (int i = 0; i < name->bufsize; i++)
	{
		printf("%c", buf[i]);
	}
	printf("\n");
}

// Function to return the last replaced letter
void backlet(char* buf, struct story * name)
{
	for (int i = 0; i < name->bufsize; i++)
	{
		if (buf[i] == name->lastletter2)
		{
			buf[i] = name->lastletter1;
		}
	}
	if (name->counts - 2 >= 0)
	{
		name->lastletter1 = name->changes[name->counts - 4];
		name->lastletter2 = name->changes[name->counts - 3];
		name->changes[name->counts - 2] = 0;
		name->changes[name->counts - 1] = 0;
		name->counts = name->counts - 2;
	}
	else
	{
		system("cls");
		printf("����� ���� �� �����������! �������� ������ ����� ����.\n");
		return;
	}
}

// History of letter replacements
void history(struct story * name)
{
	system("cls");
	printf("������� ����� ���� \"Cryptanalysis\"\n\n");
	if (name->changes[1] == 0) printf("����� ���� �� �����������.\n");
	for (int i = 0; i < name->counts; i++)
	{
		printf("%c -> %c\n", name->changes[i], name->changes[i + 1]);
		i++;
	}
}

// Decryption
void deshifr(char* buf, struct story * name, struct alphabet * dict, int token)
{
	printf("\n");
	char letter1, letter2;
	if (token == 1)
	{
		printf("����� ����� �� ������ ������������? (������� �����):\n");
		letter1 = getlow(dict);
		while (letter1 == 0)
		{
			printf("�� ����������� ����� �����. ���������� ��� ���:\n");
			letter1 = getlow(dict);
		}
	}
	else
	{
		printf("����� ����� �� ������ ������������? (������� ��������� �����):\n");
		letter1 = gethigh(dict);
		while (letter1 == 0)
		{
			printf("�� ����������� ����� �����. ���������� ��� ���:\n");
			letter1 = gethigh(dict);
		}
	}
	printf("�� �����? (������� �������� �����):\n");
	letter2 = getlow(dict);
	while (letter2 == 0)
	{
		printf("�� ����������� ����� �����. ���������� ��� ���:\n");
		letter2 = getlow(dict);
	}
	for (int i = 0; i < name->bufsize; i++)
	{
		if (buf[i] == letter1)
		{
			buf[i] = letter2;
		}
	}
	name->lastletter1 = letter1;
	name->lastletter2 = letter2;
	name->changes[name->counts] = letter1;
	name->changes[name->counts + 1] = letter2;
	name->counts = name->counts + 2;
}

// Writing a decrypted cryptogram to a file
void record(char* buf, struct story * name)
{
	system("cls");
	FILE* fileout = fopen("result2.txt", "w");
	for (int i = 0; i < name->bufsize; i++)
	{
		fprintf(fileout, "%c", buf[i]);
	}
	printf("�������������� ������������ ������� �������� � %s!\n", "result.txt");
	fclose(fileout);
}

// Output of words sorted by the number of letters
void sortcount(char* buf, struct story * name)
{
	for (int a = 1; a < 20; a++)
	{
		int i = 0;
		int j = 0;
		while (j < name->bufsize)
		{
			while ((buf[j] != ' ' && buf[j] != '-' && buf[j] != '�' && buf[j] != '.' && buf[j] != ',' && buf[j] != '?' && buf[j] != '\n' && buf[j] != ':' && buf[j] != ')' && buf[j] != '(' && buf[j] != '"' && buf[j] != '0' && buf[j] != '1' && buf[j] != '2' && buf[j] != '3' && buf[j] != '4' && buf[j] != '5' && buf[j] != '6' && buf[j] != '7' && buf[j] != '8' && buf[j] != '9' && buf[j] != '�' && buf[j] != '�') && (j < name->bufsize))
			{
				j++;
			}
			if (a == j - i)
			{
				for (int b = i; b < j; b++)
				{
					printf("%c", buf[b]);
				}
				printf("\n");
			}
			j++;
			while (buf[j] == ' ' || buf[j] == '-' || buf[j] == '�' || buf[j] == '.' || buf[j] == ',' || buf[j] == '?' || buf[j] == '\n' || buf[j] == ':' || buf[j] == ')' || buf[j] == '(' || buf[j] == '"' || buf[j] == '0' || buf[j] == '1' || buf[j] == '2' || buf[j] == '3' || buf[j] == '4' || buf[j] == '5' || buf[j] == '6' || buf[j] == '7' || buf[j] == '8' || buf[j] == '9' || buf[j] == '�' || buf[j] == '�')
			{
				j++;
			}
			i = j;
		}
	}
}

// Output of words sorted by the number of unencrypted letters
void sortcountshifr(char* buf, struct story * name, struct alphabet * dict)
{
	for (int a = 1; a < 20; a++)
	{
		int i = 0;
		int j = 0;
		while (j < name->bufsize)
		{
			int kol = 0;
			while ((buf[j] != ' ' && buf[j] != '-' && buf[j] != '�' && buf[j] != '.' && buf[j] != ',' && buf[j] != '?' && buf[j] != '\n' && buf[j] != ':' && buf[j] != ')' && buf[j] != '(' && buf[j] != '"' && buf[j] != '0' && buf[j] != '1' && buf[j] != '2' && buf[j] != '3' && buf[j] != '4' && buf[j] != '5' && buf[j] != '6' && buf[j] != '7' && buf[j] != '8' && buf[j] != '9' && buf[j] != '�' && buf[j] != '�') && (j < name->bufsize))
			{
				for (int c = 0; c < 32; c++)
				{
					if (buf[j] == dict->ralpha[c])
					{
						kol++;
					}
				}
				j++;
			}
			if (a == kol)
			{
				for (int b = i; b < j; b++)
				{
					printf("%c", buf[b]);
				}
				printf("\n");
			}
			j++;
			while (buf[j] == ' ' || buf[j] == '-' || buf[j] == '�' || buf[j] == '.' || buf[j] == ',' || buf[j] == '?' || buf[j] == '\n' || buf[j] == ':' || buf[j] == ')' || buf[j] == '(' || buf[j] == '"' || buf[j] == '0' || buf[j] == '1' || buf[j] == '2' || buf[j] == '3' || buf[j] == '4' || buf[j] == '5' || buf[j] == '6' || buf[j] == '7' || buf[j] == '8' || buf[j] == '9' || buf[j] == '�' || buf[j] == '�')
			{
				j++;
			}
			i = j;
		}
	}
}

int menu()
{
	printf("1. ��������� ������ ������������.\n");
	printf("2. ���������� ����.\n");
	printf("3. ����� �� ����� ���� ����, �������������� �� ���-�� ����.\n");
	printf("4. ����� �� ����� ���� ����, �������������� �� ���-�� ���������������� ����.\n");
	printf("5. ����������� ������������ � �������������� ������� �� ������ ������.\n");
	printf("6. ������� ��������� ��������� ����� �����.\n");
	printf("7. ������� ����� ����.\n");
	printf("8. ������������.\n");
	printf("9. ������ � ���� �������������� ������������.\n");
	printf("0. ����� �� ���������.\n");
	printf("\n�������� ����� ������ ������ ���������: ");

	int value = -1;
	char input[3];
	fgets(input, 3, stdin);
	input[strcspn(input, "\n")] = 0;
	fseek(stdin, 0, SEEK_END);

	if (security(input) == 0 && (atoi(input) < 10))
	{
		value = atoi(input);
		return(value);
	}
	else return -1;
}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	FILE* file = fopen("lab_var_2.txt", "r");
	if (file == NULL)
	{
		system("cls");
		printf("������! �� ��� ������ �������� ���� � �������������. ��������: lab_var_2.txt\n");
		printf("��������� ������� ����� � ��������� ��������� ��� ���.\n");
		return -1;
	}

	struct story mode = { 0, 0, 0, 0, '0' };
	struct alphabet alpha = { "��������������������������������", "��������������������������������", "��������������������������������" };

	printf("���� ���������� \"Cryptanalysis\"\n\n");

	char* buf;
	buf = (char*)malloc(sizeof(char));
	if (buf != NULL)
	{
		char symbol = fgetc(file);
		unsigned int count = 0;
		buf[count] = symbol;
		count++;
		while (!feof(file))
		{
			symbol = fgetc(file);
			buf = (char*)realloc(buf, (count + 1) * sizeof(char));
			buf[count] = symbol;
			count++;
		}
		buf[count - 1] = '\0';
		mode.bufsize = count - 1;
	}
	fclose(file);

	int flag = 0;
	int token = 0;

	int n;
	n = menu();
	while (n != 10)
	{
		switch (n)
		{
		case 1: 
			flag = 1;
			freq(&alpha);
			break;
		case 2: 
			token = 1;
			autochange(buf, &mode, &alpha, flag);
			break;
		case 3:
			sortcount(buf, &mode);
			break;
		case 4:
			sortcountshifr(buf, &mode, &alpha);
			break;
		case 5: 
			crypto(buf, &mode);
			break;
		case 6:
			backlet(buf, &mode);
			break; 
		case 7:
			history(&mode);
			break;
		case 8:
			deshifr(buf, &mode, &alpha, token);
			break;
		case 9:  
			record(buf, &mode);
			break;
		case 0:  
			return 0;
			break;
		default:
			system("cls");
			error();
			break;
		}
		printf("\n");
		n = menu();
	}
	free(buf);
}