#include <iostream>
#include <fstream>
using namespace std;

/*
Состав файла:
Первые 16 байт - дежурные (4 - head, 4 - size, 8 - eof)
Остальной фалй - элементы (каждый по 8 байт)
Состав элемента: 4 байта - next, 4 - value
*/

struct blist
{
    long int eof; //long int чтобы сохранить кратность 8 байтам в шаге файла
    int head;
    int size;
    fstream f;

    blist(const char filename[])
    {
        f.open(filename, fstream::in | fstream::out | fstream::binary);
        if (!f.is_open()) cout << "Error: " << filename << " couldn't be open" << endl;
        head = -1;
        size = 0;
        eof = 16;
    }
    ~blist()
    {
        f.seekp(0);
        f.write((char*)&head, sizeof(int));
        f.write((char*)&size, sizeof(int));
        f.write((char*)&eof, sizeof(long int));
        f.close();
    }

    void check_file() //если надо работать со списком из файла
    {
        f.read((char*)&head, sizeof(int));
        f.read((char*)&size, sizeof(int));
        f.read((char*)&eof, sizeof(long int));
    }

    void clear() { head = -1; size = 0; eof = 0; }

    void push_front(int value)
    {
        if (head == -1)
        {
            head = eof;
            f.seekp(eof);
            int next = -1;
            f.write((char*)&next, sizeof(int));
            f.write((char*)&value, sizeof(int));
            eof += 2 * sizeof(int);
        }
        else
        {
            f.seekp(eof);
            f.write((char*)&head, sizeof(int));
            f.write((char*)&value, sizeof(int));
            head = eof;
            eof += 2 * sizeof(int);
        }
        size++;
    }

    void pop_front()
    {
        if (head == -1) return; //need exception
        int next, value;
        f.seekg(head);
        f.read((char*)&next, sizeof(int));
        head = next;
        size--;
    }

    void print()
    {
        if (head == -1) return;
        f.seekg(head);
        int next, value;
        int i = 0;
        while (true)
        {
            f.read((char*)&next, sizeof(int));
            f.read((char*)&value, sizeof(int));
            cout << i++ << ": " << value << endl;
            if (next == -1) break;
            f.seekg(next);
        }
    }

    void insert_after(int index, int value)
    {
        if (index >= size || index < 0) return; //need exception
        //find element at index (insert to next)
        f.seekg(head);
        int next;
        f.read((char*)&next, sizeof(int));
        f.seekg(next);
        for (int i = 0; i < index - 1; i++)
        {
            f.read((char*)&next, sizeof(int));
            f.seekg(next);
        }
        //next - element at index
        int tempnext; //next element
        f.read((char*)&tempnext, sizeof(int));
        f.seekp(eof); //jump to end of file
        f.write((char*)&tempnext, sizeof(int)); //creating new node
        f.write((char*)&value, sizeof(int));
        f.seekp(next); //comeback to element at index
        next = eof;
        f.write((char*)&next, sizeof(int)); //put next to new element in the end
        size++;
        eof += 2 * sizeof(int);
    }

    void erase_after(int index)
    {
        if (index >= size || index < 0) return; //need exception
        //find element at index (insert to next)
        f.seekg(head);
        int next;
        f.read((char*)&next, sizeof(int));
        f.seekg(next);
        for (int i = 0; i < index - 1; i++)
        {
            f.read((char*)&next, sizeof(int));
            f.seekg(next);
        }
        //next - element at index
        int tempnext;
        f.read((char*)&tempnext, sizeof(int));
        f.seekg(tempnext); //erasing element
        f.read((char*)&tempnext, sizeof(int)); //next after erasing element
        f.seekp(next);
        f.write((char*)&tempnext, sizeof(int));
        size--;
    }

    int operator[](int index)
    {
        if (index >= size || index < 0) return -1; //need exception
        //find element at index (insert to next)
        f.seekg(head);
        int next;
        f.read((char*)&next, sizeof(int));
        f.seekg(next);
        for (int i = 0; i < index - 1; i++)
        {
            f.read((char*)&next, sizeof(int));
            f.seekg(next);
        }
        //next - element at index
        int value;
        f.seekg(next + sizeof(int));
        f.read((char*)&value, sizeof(int));
        return value;
    }

    bool empty()
    {
        return (head == -1);
    }

    int front()
    {
        f.seekg(head + sizeof(int));
        int value;
        f.read((char*)&value, sizeof(int));
        return value;
    }

    int get_size() { return size; }

    void swap_neighbours_if_less(int prevpos)
    {
        // сохраняем позиции четырех элементов
        f.seekg(prevpos);
        int prevpos_next;
        f.read((char*)&prevpos_next, sizeof(int)); //prevpos.next = curpos
        f.seekg(prevpos_next);
        int curpos_next;
        f.read((char*)&curpos_next, sizeof(int)); //curpos.next = nextpos
        f.seekg(curpos_next);
        int nextpos_next;
        f.read((char*)&nextpos_next, sizeof(int)); //nextpos.next = morepos
        int val1, val2;
        f.seekg(prevpos_next + sizeof(int));
        f.read((char*)&val1, sizeof(int));
        f.seekg(curpos_next + sizeof(int));
        f.read((char*)&val2, sizeof(int));
        if (val1 > val2)
        {
            // начинаем свапать элементы
            f.seekp(prevpos);
            f.write((char*)&curpos_next, sizeof(int));
            f.seekp(prevpos_next);
            f.write((char*)&nextpos_next, sizeof(int));
            f.seekp(curpos_next);
            f.write((char*)&prevpos_next, sizeof(int));
        }
    }

    void sort()
    {
        this->push_front(9999999);
        int next = head;
        for (int i = 0; i < size - 2; i++)
        {
            next = head;
            for (int j = 0; j < size - 2; j++)
            {
                f.seekg(next);
                this->swap_neighbours_if_less(next);
                f.seekg(next);
                f.read((char*)&next, sizeof(int));
            }
        }
        this->pop_front();
    }
};

int main()
{

    blist b("test.bin");
    // b.check_file();
    cout << "Empty: " << b.empty() << endl;
    b.push_front(1);
    b.push_front(2);
    b.push_front(3);
    b.push_front(4);
    // b.insert_after(1, 24);
    b.push_front(5);
    // b.erase_after(3);
    b.print();
    cout << "b[4] = " << b[4] << "; Empty: " << b.empty() << endl;
    b.sort();
    cout << "Front: " << b.front() << "; Size = " << b.get_size() << "; Head = " << b.head << endl;
    b.print();
    return 0;
}