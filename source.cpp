/* 
Тесты:
Простые типы:
push - int
pop - int
sort - char*
insert - char
erase - char
checkfile - int
сложные типы:

 */

#include "binary_list.h"
#include <iostream>
#include <chrono>
using namespace std;
using namespace std::chrono;

void push_test();
void pop_test();
void sort_test();
void insert_test();
void erase_test();
void checkfile_test();
void bigdata_test();
void test_menu();
void test_menu_hard();

void first_menu()
{
    cout << "................................" << endl;
    cout << "Добро пожаловать в меню тестирования!" << endl;
    cout << "Выберите с какими типами данных провести тесты:" << endl;
    cout << "1.\tПростые типы данных\n2.\tСложные типы данных\n3.\tВыход" << endl;
    cout << "................................" << endl;
    cout << "Введите номмер: ";
    int n;
    cin >> n;
    system("clear");
    switch (n)
    {
        case 1:
            test_menu();
            break;
        case 2:
            test_menu_hard();
            break;
        default:
            return;
    }
}

int main()
{
    first_menu();
    return 0;
}

void test_menu()
{
    cout << "................................" << endl;
    cout << "Меню тестов" << endl;
    cout << "Выберите тест:\n1.\tДобавление в начало\n2.\tУдаление из начала\n3.\tСортировка" << endl;
    cout << "4.\tВставка по номеру\n5.\tУдаление по номеру\n6.\tЧтение сохраненного списка из файла" << endl;
    cout << "7.\tПроверка времени тестирования при больших объемах данных\n8.\tВернуться в начало" << endl;
    cout << "................................" << endl;
    int n;
    cout << "Введите номер теста: ";
    cin >> n;
    switch (n)
    {
        case 1:
            push_test();
            break;
        case 2:
            pop_test();
            break;
        case 3:
            sort_test();
            break;
        case 4:
            insert_test();
            break;
        case 5:
            erase_test();
            break;
        case 6:
            checkfile_test();
            break;
        case 7:
            bigdata_test();
            break;
        case 8:
            first_menu();
            break;
        default:
        {
            cout << "Введено недопусимое значение!" << endl;
            test_menu();
            break;
        }
    }
}

void test_menu_hard()
{
    cout << "Тесты со сдожными типами данных еще не готовы" << endl;
    return first_menu();
}

void push_test()
{
    system("clear");
    binary_list<int> b("test.bin");
    b.push_front(1);
    b.push_front(4);
    cout << "Текущий список:" << endl;
    b.print();
    int k = 94;
    cout << "Добавление элемента " << k << " в начало: " << endl;
    b.push_front(k);
    b.print();
    return test_menu();
}

void pop_test()
{
    system("clear");
    binary_list<int> b("test.bin");
    b.push_front(1);
    b.push_front(4);
    b.push_front(54);
    b.push_front(47);
    cout << "Текущий список:" << endl;
    b.print();
    cout << "Удаление элемента из начала:" << endl;
    b.pop_front();
    b.print();
    return test_menu();
}

void sort_test()
{
    system("clear");
    binary_list<char*> b("test.bin");
    char* a = new char[3] {'a', 'b', '\0'};
    char* e = new char[3] {'b', 'd', '\0'};
    char* c = new char[3] {'e', 'z', '\0'};
    char* d = new char[3] {'z', 'a', '\0'};

    b.push_front(a);
    b.push_front(e);
    b.push_front(c);
    b.push_front(d);
    cout << "Текущий список:" << endl;
    b.print();
    cout << "Отсортированный список:" << endl;
    b.sort();
    b.print();

    delete[] a;
    delete[] e;
    delete[] c;
    delete[] d;
    return test_menu();
}

void insert_test()
{
    system("clear");
    binary_list<char> b("test.bin");
    b.push_front('a');
    b.push_front('b');
    b.push_front('c');
    b.push_front('d');
    cout << "Текущий список:" << endl;
    b.print();
    cout << "Вставка элемента Z после второго элемента: " << endl;
    b.insert_after(1, 'Z');
    b.print();
    return test_menu();
}

void erase_test()
{
    system("clear");
    binary_list<char> b("test.bin");
    b.push_front('a');
    b.push_front('b');
    b.push_front('c');
    b.push_front('d');
    b.insert_after(1, 'Z');
    cout << "Текущий список:" << endl;
    b.print();
    cout << "Удаление четвертого элемента:" << endl;
    b.erase_after(2);
    b.print();
    return test_menu();

}

void checkfile_test()
{
    system("clear");
    binary_list<int> b("test.bin");
    b.push_front(1);
    b.push_front(4);
    b.push_front(54);
    b.push_front(47);
    cout << "Список, находящийся в файле:" << endl;
    b.print();
    b.~binary_list();
    cout << "Новый список пуст. Чтение списка из файла с прошлым списком: " << endl;
    binary_list<int> bnew("test.bin");
    bnew.check_file();
    bnew.print();
    return test_menu();
}

void bigdata_test()
{
    system("clear");
    int n;
    cout << "Введите количество элементов списка: ";
    cin >> n;
    binary_list<int> b("test.bin");
    for (int i = 0; i < n; i++) b.push_front(i);
    auto start = steady_clock::now();
    b.sort();
    auto finish = steady_clock::now();
    cout << "Duration of sorting " << n << " elements: " << duration_cast<milliseconds>(finish - start).count() << " milliseconds" << endl;
    return test_menu();
}