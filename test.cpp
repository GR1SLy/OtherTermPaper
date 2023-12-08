#include <iostream>
#include <fstream>
#include <forward_list>
using namespace std;

struct test
{
    fstream f;
    test() { f.open("test.bin", fstream::in | fstream::out | fstream::binary | fstream::trunc); }
    ~test() { f.close(); }
    void tet(int pos, int next, int val)
    {
        f.seekp(pos);
        f.write((char*)&next, sizeof(int));
        f.write((char*)&val, sizeof(int)); 
    }
    void print(int from)
    {
        f.seekg(from);
        int next, value;
        int i = 0;
        while(true)
        {
            f.read((char*)&next, sizeof(int));
            f.read((char*)&value, sizeof(int));
            cout << i++ << ": " << value << endl;
            if (next == -1) break;
            f.seekg(next);
        }
    }

    void swap_neighbours(int prevpos, bool with_head)
    {
        // сохраняем позиции четырех элементов
        int prevpos_next;
        if (!with_head) // граничное исследование
        {
            f.seekg(prevpos);
            f.read((char*)&prevpos_next, sizeof(int)); //prevpos.next = curpos
        }
        else { int head = prevpos; prevpos_next = head; }
        f.seekg(prevpos_next);
        int curpos_next;
        f.read((char*)&curpos_next, sizeof(int)); //curpos.next = nextpos
        f.seekg(curpos_next);
        int nextpos_next;
        f.read((char*)&nextpos_next, sizeof(int)); //nextpos.next = morepos
        // начинаем свапать элементы
        f.seekp(prevpos);
        f.write((char*)&curpos_next, sizeof(int));
        f.seekp(prevpos_next);
        f.write((char*)&nextpos_next, sizeof(int));
        f.seekp(curpos_next);
        f.write((char*)&prevpos_next, sizeof(int));
    }
};

int main()
{
    /* int n = 5;
    int *arr = new int[n] {4, 3, 2, 1, 0};
    //bubble sort
    int temp;
    for (int i = 0; i < n; i++) for (int j = 0; j < n - 1; j++)
    {
        if (arr[j] > arr[j + 1])
        {
            temp = arr[j];
            arr[j] = arr[j + 1];
            arr[j + 1] = temp;
        }
    }
    for (int i = 0; i < n; i++) cout << arr[i] << endl;
    delete[] arr; */
    /* test t;
    t.tet(0, -1, 2423);
    t.tet(8, 0, 4213);
    t.tet(16, 8, 2381);
    t.tet(24, 16, 65734);
    t.tet(32, 24, 463);
    t.print(32);
    t.swap_neighbours(16, 0);
    cout << "ASDA" << endl;
    t.print(32); */
    return 0;
}