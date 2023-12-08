#include "binary_list.cpp"
#include <iostream>
using namespace std;

int main()
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
    return 0;
}