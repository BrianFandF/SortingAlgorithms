#include<iostream>
#include<time.h>
#include<termcolor/termcolor.hpp>
#include<Windows.h>
#include<vector>
#include<chrono>
using namespace std;

bool proverka(int k, int left, int right)
{
	if (k > 0 && left > right)
	{
		return true;
	}

	else
	{
		return false;
	}
}

bool proverka_min(int k, int left, int right)
{
	if (k > 0 && left < right)
	{
		return true;
	}

	else
	{
		return false;
	}
}

void insert_sort(int A[], int size_array)
{
	for (int i = 0; i < size_array; i++)
	{
		int k = i;
		while (k > 0 && A[k - 1] > A[k])
		{
			int tmp = A[k - 1];
			A[k - 1] = A[k];
			A[k] = tmp;
			--k;
		}
	}
}

void insert_sort(int A[], int size_array, bool (*proverka)(int , int , int))
{
	for (int i = 0; i < size_array; i++)
	{
		int k = i;
		while (proverka(k , A[k - 1], A[k]))
		{
			int tmp = A[k - 1];
			A[k - 1] = A[k];
			A[k] = tmp;
			--k;
		}
	}
}

void print(int A[], int size_array)
{
	for (int i = 0; i < size_array; i++)
	{
		cout << termcolor::bright_yellow << A[i] << " ";
	}
	cout << endl << termcolor::reset;
}

void print(vector<int> south)
{
	for (auto tmp : south)
	{
		cout << termcolor::bright_yellow << tmp << " ";
	}
	cout << termcolor::reset << endl;
}

void generate(int A[], int size_array)
{
	for (int i = 0; i < size_array; i++)
	{
		A[i] = rand() % size_array;
	}
}

int number_of_digits(int number)
{
	int count = 0;
	while (number)
	{
		number /= 10;
		++count;
	}

	return count;
}

int get_digit_by_rank(int number, int rank)
{
	if (rank < 0)
	{
		cout << termcolor::bright_red << "Неправильный разряд, разряд должен быть >=0" << termcolor::reset << endl;
		return -1;
	}

	int tmp = pow(10, rank + 1);
	int res = number % tmp / pow(10, rank);
	return res;
}

int find_max_number(vector<int> numbers)
{
	int max_number_of_digits = 0;
	for (auto current_number : numbers)
	{
		if (max_number_of_digits < number_of_digits(current_number))
		{
			max_number_of_digits = number_of_digits(current_number);
		}
	}

	return max_number_of_digits;
}

vector<int> find_max_min_keys(vector<int> numbers, int rank)
{
	if (rank < 0)
	{
		return { -1, -1 };
	}

	int min_key = get_digit_by_rank(numbers[0], rank);
	int max_key = min_key;
	int digit = 0;
	for (auto current_number : numbers)
	{
		digit = get_digit_by_rank(current_number, rank);
		if (digit < min_key)
		{
			min_key = digit;
		}

		if (digit > max_key)
		{
			max_key = digit;
		}
	}

	return { min_key, max_key };
}

vector<int> count_sort(vector<int> numbers, int rank)
{
	vector<int> max_min = find_max_min_keys(numbers, rank);
	int min_key = max_min[0];
	int max_key = max_min[1];
	int n = max_key - min_key + 1;
	vector<int> support;
	for (int i = 0; i < n; i++)
	{
		support.push_back(0);
	}

	for (auto current_number : numbers)
	{
		support[get_digit_by_rank(current_number, rank) - min_key] += 1;
	}

	int size = numbers.size();
	for (int i = support.size() - 1; i >= 0; i--)
	{
		size -= support[i];
		support[i] = size;
	}

	vector<int> res;
	int size_res = numbers.size();
	for (int i = 0; i < size_res; i++)
	{
		res.push_back(0);
	}

	for (auto current_number : numbers)
	{
		res[support[get_digit_by_rank(current_number, rank) - min_key]] = current_number;
		support[get_digit_by_rank(current_number, rank) - min_key] += 1;
	}

	return res;
}

vector<int> radix_sort(vector<int> numbers)
{
	int max_number_of_digits = find_max_number(numbers);
	int rank = 0;
	for (int i = 0; i < max_number_of_digits; i++)
	{
		numbers = count_sort(numbers, rank);
		rank += 1;
	}

	return numbers;
}

typedef bool (*proverka_type)(int, int, int);

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	srand(time(NULL));
	vector<chrono::duration<double>> time;
	vector<proverka_type> functions{proverka, proverka_min};
	const int SIZE = 10;
	int A[SIZE];
	//int j = 100000;
	for (int i = 0; i < SIZE; i++)
	{
		A[i] = rand() % SIZE + 1;
	}
	cout << termcolor::bright_cyan << "Исходный массив: " << endl;
	print(A, SIZE);
	int choice = 0;
	auto start = chrono::high_resolution_clock::now();
	cout << "Введите значение: 0 - по возраснию, 1 - по убыванию" << endl;
	cin >> choice;
	insert_sort(A, SIZE, functions[choice]);
	auto end = chrono::high_resolution_clock::now();
	chrono::duration<double> total = end - start;
	time.push_back(total);

	cout << termcolor::bright_green << "Результат сортировки вставками: " << endl;
	print(A, SIZE);
	cout << termcolor::bright_blue << "Затраченное время: " << termcolor::bright_red << fixed << time[0].count() << termcolor::reset << endl;
	
	vector<int> for_test;
	//int j1 = 100000;
	for (int i = 1; i <= 100000; i++)
	{
		for_test.push_back(rand() % 100000 + 1);
	}

	cout << termcolor::bright_cyan << "Исходный массив: " << endl;
	//print(for_test);
	cout << termcolor::bright_green << "Результат поразрядной сортировки: " << endl;
	start = chrono::high_resolution_clock::now();
	vector<int> res = radix_sort(for_test);
	end = chrono::high_resolution_clock::now();
	total = end - start;
	time.push_back(total);
	//print(res);
	cout << termcolor::bright_blue << "Затраченное время: " << termcolor::bright_red << fixed << time[1].count() << termcolor::reset << endl;
	pair<int, int> test{ 1, 2 };
	cout << test.first << endl;
	cout << test.second << endl;
}
