#include "Rectangle.h"

Rectangle::Rectangle(const Rectangle& object)
{
    this->width = object.width;
    this->height = object.height;
    if (object.color != nullptr)  
    {
        this->color = new char[strlen(object.color) + 1];
        strcpy(this->color, object.color);
    }
    else
    {
        this->color = nullptr;
    }
}
Rectangle::Rectangle(int width, int height, const char* color)
{
    this->width = width;
    this->height = height;
    this->color = new char[strlen(color) + 1];
    strcpy(this->color, color);
}

Rectangle::Rectangle()
{
    this->color = nullptr;
    this->width = 0;
    this->height = 0;
}

Rectangle::~Rectangle()
{
    delete[] color;
}

int Rectangle::PerCalc()
{
    int sum;
    sum = (this->height + this->width) * 2;
    return sum;
}

void Rectangle::print()
{
    cout << "Width: " << width << endl;
    cout << "Height: " << height << endl;
    if (color != nullptr)
    {
        cout << "Fill Color: " << color << endl;
    }
    else
    {
        cout << "Fill Color: Not defined" << endl;
    }
}

Rectangle Rectangle::operator+(const Rectangle& object)
{
    Rectangle temp;
    temp.width = this->width + object.width;
    temp.height = this->height + object.height;
    temp.color = new char[strlen(this->color) + strlen(object.color) + 2];
    int i = 0;
    for (; i < strlen(this->color); i++)
    {
        temp.color[i] = this->color[i];
    }
    temp.color[i++] = '.';
    for (int j = 0; j < strlen(object.color); i++, j++)
    {
        temp.color[i] = object.color[j];
    }
    temp.color[strlen(this->color) + strlen(object.color) + 1] = '\0';
    return temp;
}

Rectangle& Rectangle::operator=(const Rectangle& object)
{
    this->width = object.width;
    this->height = object.height;
    if (this->color != nullptr) delete[] this->color;
    this->color = new char[strlen(object.color) + 1];
    for (int i = 0; i < strlen(object.color); i++)
    {
        this->color[i] = object.color[i];
    }
    this->color[strlen(object.color)] = '\0';
    return *this;
}

Rectangle& Rectangle::operator++()
{
    width++;
    height++;
    return *this;
}

Rectangle& Rectangle::operator++(int plus)
{
    Rectangle temprect(*this);
    ++(*this);
    return temprect;
}

Rectangle operator-(const Rectangle& r1, const Rectangle& r2)
{
    int width = 0;
    int height = 0;
    if (r1.width > r2.width)
    {
        width = r1.width - r2.width;
    }
    else width = 0;

    if (r1.height > r2.height)
    {
        height = r1.height - r2.height;
    }
    else height = 0;

    Rectangle result(width, height, "color");
    return result;
}

ostream& operator<<(ostream& outf, const Rectangle& r)
{
    outf.write((char*)&r.width, sizeof(int));
    outf.write((char*)&r.height, sizeof(int));
    if (r.color == nullptr) return outf;
    int length;
    length = strlen(r.color);
    outf.write((char*)&length, sizeof(int));
    outf.write(r.color, length);
    return outf; 
}

istream& operator>>(istream& inputf, Rectangle& r)
{
    inputf.read((char*)&r.width, sizeof(int));
    inputf.read((char*)&r.height, sizeof(int));
    int length;
    inputf.read((char*)&length, sizeof(int));
    if (r.color != nullptr) delete[] r.color;
    r.color = new char[length + 1];
    inputf.read(r.color, length);
    r.color[length] = '\0';
    return inputf;
}

int Rectangle::get_size()
{
    if (color != nullptr) return sizeof(int) * 3 + strlen(color);
    else return sizeof(int) * 3;
}

bool Rectangle::operator>=(Rectangle &object)
{
    return (this->PerCalc() >= object.PerCalc());
}
