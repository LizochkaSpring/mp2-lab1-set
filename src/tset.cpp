// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"
//MaxPower - максимальная длина поезда (битлен)
//BitField - поезд с людьми
//TSet - дом для поезда
// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);
static TSet FAKE_SET(1);

TSet::TSet(int mp) : BitField(mp) // получили макс длину и передаем ее в поезд
{
    MaxPower = mp; // присваиваем
}

// конструктор копирования
TSet::TSet(const TSet &s) : BitField(s.BitField)
{
    this->MaxPower = s.MaxPower; // копируем все в поезде нашем и копируем максимальную длину
}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) : BitField(bf)
{
    MaxPower = bf.GetLength(); // передаем наш полностью копированный поезд бф в тбитфилд и присваиваем макс длину
}

TSet::operator TBitField()
{
    return this->BitField; // возвращаем поезд с людьми
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов // возврашяем макс число элементов в доме а в доме поезд с людьми и свободное пространство
{
    return MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
    return this->BitField.GetBit(Elem); // получаем сведения о нахождении кого-то на месте елем
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    this->BitField.SetBit(Elem); // сажаем на место елем человека
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    this->BitField.ClrBit(Elem); // выселяем из места где сидит безбилетник его
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet &s) // присваивание
{
    this->BitField = s.BitField;
    this->MaxPower = s.MaxPower;
    return *this;
}

int TSet::operator==(const TSet &s) const // сравнение
{
    if (this->MaxPower == s.MaxPower && this->BitField == s.BitField)
        return 1;
    else
        return 0;
}

int TSet::operator!=(const TSet &s) const // сравнение
{
    if (this->MaxPower == s.MaxPower && this->BitField == s.BitField)
        return 0;
    else
        return 1;
}

TSet TSet::operator+(const TSet &s) // объединение
{
    return (this->BitField | s.BitField);
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    TSet temp(*this);// скопировали множество, добавили элемент и вернули.
    temp.InsElem(Elem);
    return temp;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    TSet temp(*this);
    temp.DelElem(Elem);
    return temp;
}

TSet TSet::operator*(const TSet &s) // пересечение
{
    return (this->BitField & s.BitField);
}

TSet TSet::operator~(void) // дополнение
{
    return(~this->BitField); 
}

// перегрузка ввода/вывода

istream &operator>>(istream &istr, TSet &s) // ввод
{
    istr >> s.BitField;
    return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s) // вывод
{
    ostr << s.BitField;
    return ostr;
}
