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
inline void binary_list<T>::push_front(const T value) //need for char*
{
    if (head == -1)
    {
        head = eof;
        f.seekp(eof);
        int next = -1;
        f.write((char*)&next, sizeof(int));
        f.write((char*)&value, sizeof(T));
        eof += sizeof(int) + sizeof(T);
    }
    else
    {
        f.seekp(eof);
        f.write((char*)&head, sizeof(int));
        f.write((char*)&value, sizeof(T));
        head = eof;
        eof += sizeof(int) + sizeof(T);
    }
    size++;
}

template<>
inline void binary_list<char*>::push_front(char* value)
{
    if (head == -1)
    {
        head = eof;
        f.seekp(eof);
        int next = -1;
        int len = strlen(value);
        f.write((char*)&next, sizeof(int));
        f.write((char*)&len, sizeof(int));
        f.write(value, len);
        eof += sizeof(int) * 2 + len;
    }
    else
    {
        f.seekp(eof);
        int len = strlen(value);
        f.write((char*)&head, sizeof(int));
        f.write((char*)&len, sizeof(int));
        f.write(value, len);
        head = eof;
        eof += sizeof(int) * 2 + len;
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
inline void binary_list<T>::print() //need for char*
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

template<>
inline void binary_list<char*>::print()
{
    if (head == -1) return;
    f.seekg(head);
    int next;
    int len;
    char* value;
    int i = 0;
    while (true)
    {
        f.read((char*)&next, sizeof(int));
        f.read((char*)&len, sizeof(int));
        value = new char[len + 1];
        f.read(value, len);
        value[len] = '\0';
        cout << i++ << ": " << value << endl;
        delete[] value;
        if (next == -1) break;
        f.seekg(next);
    }
}

template <typename T>
inline void binary_list<T>::insert_after(int index, const T value) //need for char*
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
    eof += sizeof(int) + sizeof(T);
}

template<>
inline void binary_list<char*>::insert_after(int index, char* value)
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
    int len = strlen(value);
    f.write((char*)&tempnext, sizeof(int)); //creating new node
    f.write((char*)&len, sizeof(int));
    f.write(value, len);
    f.seekp(next); //comeback to element at index
    next = eof;
    f.write((char*)&next, sizeof(int)); //put next to new element in the end
    size++;
    eof += sizeof(int) * 2 + len;
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
inline T binary_list<T>::front() //need char*
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

template<>
inline char* binary_list<char*>::front()
{
    if (head == -1) throw list_ex("List is empty");
    else 
    {
        f.seekg(head + sizeof(int));
        int len;
        char* value;
        f.read((char*)&len, sizeof(int));
        value = new char[len + 1];
        f.read(value, len);
        value[len] = '\0';
        return value;
    }
}

template <typename T>
inline int binary_list<T>::length()
{
    return size;
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
    T val1, val2;
    f.seekg(prevpos_next + sizeof(int));
    f.read((char*)&val1, sizeof(T));
    f.seekg(curpos_next + sizeof(int));
    f.read((char*)&val2, sizeof(T));
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

template<>
inline void binary_list<char*>::swap_neighbours_if_less(int prevpos)
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
    int len1, len2;
    char* val1;
    char* val2;
    f.seekg(prevpos_next + sizeof(int));
    f.read((char*)&len1, sizeof(int));
    val1 = new char[len1 + 1];
    f.read(val1, len1);
    val1[len1] = '\0';
    f.seekg(curpos_next + sizeof(int));
    f.read((char*)&len2, sizeof(int));
    val2 = new char[len2 + 1];
    f.read(val2, len2);
    val2[len2] = '\0';
    if (val1[0] >= val2[0])
    {
        // начинаем свапать элементы
        f.seekp(prevpos);
        f.write((char*)&curpos_next, sizeof(int));
        f.seekp(prevpos_next);
        f.write((char*)&nextpos_next, sizeof(int));
        f.seekp(curpos_next);
        f.write((char*)&prevpos_next, sizeof(int));
    }
    delete[] val1;
    delete[] val2;
}

template <typename T>
inline void binary_list<T>::sort()
{
    this->push_front(T());
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

template<>
inline void binary_list<char*>::sort()
{
    char* add = new char[4] {'A', 'A', 'A', '\0'};
    this->push_front(add);
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
    delete[] add;
}


