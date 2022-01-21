#include <iostream>
#include <Windows.h>
#include <Winuser.h>
#include <string>
#include <iomanip>
#include <fstream>

using namespace std;

int stateM;

static int num; //статическая переменная для учета общего числа элементов в базе данных

struct Students //
{
	string fam;
	char name[30];
	char faculty[30];
	int group;
};

void MainMenu() //вывод меню
{
	cout << "         МЕНЮ\n"
		<< "(1) Добавить студента. \n"
		<< "(2) Поиск студента по фамилии. \n"
		<< "(3) Фильтр по номеру группы. \n"
		<< "(4) Сортировать по алфавиту. \n"
		<< "(5) Распечатать данные. \n"
		<< "(0) Выход из программы. \n";
	cout << " ";
	cin >> stateM;
}
//char* rus(const char* text) 
//{
//	char *bufRus = new char[strlen(text)];
//	CharToOem(text, bufRus);
//	return bufRus;
//}
void output(Students a[],int k) // распечатка данных в табличном виде
{
    cout << setw(26) << "Last name|" << setw(26) << "First name|" << setw(26) << "faculty|" << setw(10) << "Group |" <<endl;
        for (int i = 0; i < k; i++)
        {
            cout << setw(88) << "_______________________________________________________________________________________" << endl;
                cout << setw(25) << a[i].fam << "|" << setw(25) << a[i].name << "|" << setw(25) <<
                a[i].faculty << "|" << setw(9) << a[i].group  << "|";
            cout << endl;
        }
}

void checkFile(string nameFile) //проверка файла и считывание количество записей в ней
{
	ofstream fout(nameFile, ofstream::app);
	ifstream fin(nameFile);

	if (!fout.is_open())
	{
		cout << ("Ошибка открытия файла") << endl;
	}
	else
	{
		fin >> num;

		if (num < 0) {
			fout << "0";
			num = 0;
		}
	}

	fout.close();
	fin.close();
}

void ReadFile(string nameFile, Students a[]) //запись данных из файла в структуру
{
	ifstream fin(nameFile);
	int d;
	cout << "     Записей " << num << "\n";
	fin >> d;
	for (int i = 0; i < num; i++)
	{
		fin >> a[i].fam;
		fin >> a[i].name;
		fin >> a[i].faculty;
		fin >> a[i].group;
	}
	fin.close();
}

void WriteFile(string nameFile, Students a[])  // запись данных из структуры в файл
{
	ofstream fout(nameFile);
	if (!fout.is_open())
	{
		cout << ("Ошибка открытия файла") << endl;
	}
	else
	{
		fout << num << "\n";
		for (int i = 0; i < num; i++)
		{
			fout << a[i].fam << "\n";
			fout << a[i].name << "\n";
			fout << a[i].faculty << "\n";
			fout << a[i].group << "\n";
		}
	}
	fout.close();
}

Students* AddEl(Students a[])
{
	Students* tempTable = new Students[num]; //Создаем временную таблицу
	for (short i = 0; i < num; i++) //Переносим все из основной таблицы в временную
		tempTable[i] = a[i];

	delete[] a; //Удаляем основную таблицу
	num++;
	a = new Students[num]; //Выделяем память для основной таблицы, но уже на +1 эл-м больше 
	for (short i = 0; i < num - 1; i++)
		a[i] = tempTable[i]; //Переносим все из временной таблицы в основну

	delete[] tempTable; //Удаляем временную таблицу
//Заполняем новый элемент:
	cout << " Фамилия: ";
	cin >> a[num - 1].fam;
	cout << " Имя: ";
	cin >> a[num - 1].name;
	cout << " Факультет: ";
	cin >> a[num - 1].faculty;
	cout << " Группа: ";
	cin >> a[num - 1].group;
	return a;
}
void sort(Students a[]) //сортировка данных по алфавиту
{
	Students min; //Создаем временную таблицу
	int n_min;
	for (int i = 0; i < num; i++)
	{
		min = a[i]; n_min = i; //Переносим все из основной таблицы в временную
		for (int j = i; j < num; j++)
			if (strcmp(a[j].fam, min.fam) < 0)
			{
				min = a[j];
				n_min = j;
			}
		//Переносим все из временной таблицы в основну
		a[n_min] = a[i];
		a[i] = min;
	}
}

int IsGroup (Students a[], Students b[], int c) //фильтр по номеру группы
{
	int k = 0;
	for (int i = 0; i < num; i++)
		if (a[i].group  == c) b[k++] = a[i];
	return k; //возвращаем количество записей студентов из группы
}

int SearchByLN(Students a[], Students b[], string c) //поиск студентов по фамилии
{
	int k = 0;
	for (int i = 0; i < num; i++)
		if (a[i].fam == c) b[k++] = a[i];
	return k; //возвращаем количество записей студентов с такой фамилией
}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	MainMenu(); //вывод меню

	checkFile("Students.txt"); //первичная проверка файла

	Students* S = new Students[num]; //создание основной таблицы (структуры)

	ReadFile("Students.txt", S);//запись данных из файла в структуру

	while (stateM != 0) {
		switch (stateM)
		{
		default: 
			MainMenu();
			break;
		case 1:
		{	
			cout << "          Добавить студента" << "\n";
			S=AddEl(S); //вызов функции добавления студента
			WriteFile("Students.txt", S); //запись в файл
		}
		break;
		case 2:
		{
			cout << "          Поиск студента по фамилии" << "\n";
			string LN;
			Students* whisLN = new Students[num]; //создание временной таблицы
			cout << "Введите фамилию студента: ";
			cin >> LN;
			int k = SearchByLN(S, whisLN, LN);//поиск студента по фамилии
			cout << "Студенты с фамилией " << LN << "\n";
			output(whisLN, k);//распечатка найденных значений
			delete[] whisLN;//Удаляем временную таблицу
		}
			break;
		case 3:
		{
			cout << "          Фильтр по номеру группы" << "\n";
			int group;
			Students* isG = new Students[num];//создание временной таблицы
			cout << "Введите номер группы: ";
			cin >> group;
			int k = IsGroup(S, isG, group);//поиск студента по номеру группы
			cout << "Студенты в группе номер " << group << "\n";
			output(isG, k);//распечатка найденных значений
			delete[] isG;//Удаляем временную таблицу
		}
			break;
		case 4:
		{
			cout << "          Сортировать по алфавиту" << "\n";
			sort(S);//сортировка по алфавиту
			cout << "Сортировка по алфавиту выполнена" << "\n";
			output(S, num);//распечатка отсортированной таблицы
			WriteFile("Students.txt", S); //запись отсортированной таблицы
		}
			break;
		case 5:
			cout << "          Распечатать данные" << "\n";
			output(S,num);//распечатка данных
			break;
		}
		MainMenu();
	}
}

