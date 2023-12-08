#include "binary_list.cpp"
#include <iostream>
#include <chrono>
using namespace std;
using namespace std::chrono;

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
    char* a = new char[3] {'a', 'b', '\0'};
    char* e = new char[3] {'b', 'd', '\0'};
    char* c = new char[3] {'e', 'z', '\0'};
    char* d = new char[3] {'z', 'a', '\0'};

    b.push_front(a);
    b.push_front(e);
    b.push_front(c);
    b.push_front(d);
    b.insert_after(1, a);
    b.print();
    b.sort();
    b.print();

    delete[] a;
    delete[] e;
    delete[] c;
    delete[] d;
}

void testbig_data()
{
    int n = 100;
    binary_list<int> b("test.bin");
    for (int i = 0; i < n; i++) b.push_front(i);
    auto start = steady_clock::now();
    b.sort();
    auto finish = steady_clock::now();
    cout << "Duration of sorting " << n << " elements: " << duration_cast<milliseconds>(finish - start).count() << " milliseconds" << endl;
}

int main()
{
    testint();
    testchar();
    testc_string();
    // testbig_data();
    return 0;
}