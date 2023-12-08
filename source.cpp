#include "binary_list.cpp"
#include <iostream>
using namespace std;

void testint()
{
    binary_list<int> b("test.bin");
    try { b.pop_front(); } catch (const binary_list<int>::list_ex& ex) { cerr << ex.what() << endl; }
    cout << "Empty: " << b.empty() << endl;
    b.push_front(1);
    b.push_front(2);
    b.push_front(3);
    b.push_front(4);
    // b.insert_after(1, 24);
    b.push_front(5);
    // b.erase_after(3);
    b.print();
    cout << "Empty: " << b.empty() << endl;
    b.sort();
    cout << "Front: " << b.front() << "; Size = " << b.length() << endl;
    b.print();
}

void testchar()
{
    binary_list<char> b("test.bin");
    b.push_front('a');
    b.push_front('b');
    b.push_front('c');
    b.push_front('d');
    b.insert_after(1, 'Z');
    b.print();
    b.erase_after(2);
    cout << endl;
    b.sort();
    b.print();
}

void testc_string()
{
    binary_list<char*> b("test.bin");
    char* a = "asd\0";
    char* e = "rte\0";
    char* c = "wes\0";
    char* d = "qwer\0";

    b.push_front(a);
    b.push_front(e);
    b.push_front(c);
    b.push_front(d);
    b.insert_after(1, a);
    b.print();
    b.sort();
    // b.print();

    // delete[] a;
    // delete[] e;
    // delete[] c;
    // delete[] d;
}

int main()
{
    testc_string();
    return 0;
}