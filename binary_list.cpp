#include "binary_list.h"

template <typename T>
inline binary_list<T>::binary_list()
{
    head = -1;
    size = 0;
    eof = 16;
}

template <typename T>
inline binary_list<T>::binary_list(const char filename[])
{
    f.open(filename, fstream::in | fstream::out | fstream::binary);
    if (!f.is_open()) cout << "Error: " << filename << " couldn't be open" << endl;
    head = -1;
    size = 0;
    eof = 16;
}

template <typename T>
inline binary_list<T>::~binary_list()
{
    f.seekp(0);
    f.write((char*)&head, sizeof(int));
    f.write((char*)&size, sizeof(int));
    f.write((char*)&eof, sizeof(long int));
    f.close();
}

template <typename T>
inline void binary_list<T>::open_file(const char filename[])
{
    // try { f.close(); } catch(const exception &ex) { cout << ex.what() << endl; }
    f.close();
    f.open(filename, fstream::in | fstream::out | fstream::binary);
    if (!f.is_open()) cout << "Error: " << filename << " couldn't be open" << endl;
}

template <typename T>
inline void binary_list<T>::clear()
{
    head = -1;
    size = 0;
    eof = 16;
}

template <typename T>
inline void binary_list<T>::check_file()
{
    f.read((char*)&head, sizeof(int));
    f.read((char*)&size, sizeof(int));
    f.read((char*)&eof, sizeof(long int));
}

template <typename T>
inline void binary_list<T>::push_front(const T value)
{
    if (head == -1)
    {
        head = eof;
        f.seekp(eof);
        int next = -1;
        f.write((char*)&next, sizeof(int));
        f.write((char*)&value, sizeof(T));
        eof += 2 * sizeof(T);
    }
    else
    {
        f.seekp(eof);
        f.write((char*)&head, sizeof(int));
        f.write((char*)&value, sizeof(T));
        head = eof;
        eof += 2 * sizeof(T);
    }
    size++;
}

template <typename T>
inline void binary_list<T>::pop_front()
{
    if (head == -1) throw list_ex("List is empty. Nothing to pop_front.");
    int next;
    f.seekg(head);
    f.read((char*)&next, sizeof(int));
    head = next;
    size--;
}

template <typename T>
inline void binary_list<T>::print()
{
    if (head == -1) return;
    f.seekg(head);
    int next;
    T value;
    int i = 0;
    while (true)
    {
        f.read((char*)&next, sizeof(int));
        f.read((char*)&value, sizeof(T));
        cout << i++ << ": " << value << endl;
        if (next == -1) break;
        f.seekg(next);
    }
}

template <typename T>
inline void binary_list<T>::insert_after(int index, const T value)
{
    if (index >= size || index < 0) throw list_ex("Can not insert_after. Index out of bounds");
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
    f.write((char*)&value, sizeof(T));
    f.seekp(next); //comeback to element at index
    next = eof;
    f.write((char*)&next, sizeof(int)); //put next to new element in the end
    size++;
    eof += 2 * sizeof(T);
}

template <typename T>
inline void binary_list<T>::erase_after(int index)
{
    if (index >= size || index < 0) throw list_ex("Can not erase_after. Index out of bounds");
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

template <typename T>
inline bool binary_list<T>::empty()
{
    return (head == -1);
}

template <typename T>
inline T binary_list<T>::front()
{
    if (head == -1) throw list_ex("List is empty");
    else 
    {
        f.seekg(head + sizeof(int));
        T value;
        f.read((char*)&value, sizeof(T));
        return value;
    }
}

template <typename T>
inline int binary_list<T>::length()
{
    return size;
}

template <typename T>
inline void binary_list<T>::sort()
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

template <typename T>
inline void binary_list<T>::swap_neighbours_if_less(int prevpos)
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
    if (val1 >= val2)
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
