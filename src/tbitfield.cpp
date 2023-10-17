// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);
// битлен это люди
// мемлен это автобусы
// рмем это коробка с этими автобусами и людьми
// телем - это тип автобусов (как инт или чар)

TBitField::TBitField(int len) // заполняем автобусы и людей и место в коробке для них
{
    if (len < 0) throw " Len < 0 ! ";
    BitLen = len;
    if (len % (sizeof(TELEM) * 8) != 0) {
        MemLen = len/(sizeof(TELEM) * 8) + 1;
    }
    else {
        MemLen = len / (sizeof(TELEM) * 8);
    }
    pMem = new TELEM[MemLen]{};
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования // копируем из одной коробки в другую атвобусы и людей и память
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[bf.MemLen]{};
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = bf.pMem[i];
    }
}

TBitField::~TBitField() // убиваем всех людей и сминает автобус всмятку
{
    delete[]pMem;
    BitLen = 0;
    MemLen = 0;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n // ищем в каком автобусе сидит человек
{
    if (n < 0) throw " N < 0 ! ";
    if (n >= BitLen) throw" N >= BitLen";
    return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n 
{
    if (n < 0) throw " N < 0 ! ";
    if (n >= BitLen) throw" N >= BitLen";
    return 1 << (n - 1) % (sizeof(TELEM) * 8); // маска для пассажира социофоба чтобы с ним (1) никто не сидел(0)
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;// получили кол-во людей всего в автобусах
}

void TBitField::SetBit(const int n) // установить бит // человека сажаем в автобус на n место
{
    if (n < 0) throw " N < 0 ! ";
    if (n >= BitLen) throw" N >= BitLen";
    pMem[GetMemIndex(n)] |= GetMemMask(n);
 
}

void TBitField::ClrBit(const int n) // очистить бит // выбрасываем из автобуса безбилетника 
{
    if (n < 0) throw " N < 0 ! ";
    if (n >= BitLen) throw" N >= BitLen";
    pMem[GetMemIndex(n)] &= ~GetMemMask(n); // инверсия маски
}

int TBitField::GetBit(const int n) const // получить значение бита // проверяем сидит ли кто-то на месте (маской и))
{
    if (n < 0) throw " N < 0 ! ";
    if (n >= BitLen) throw" N >= BitLen";
    if ((pMem[GetMemIndex(n)] & GetMemMask(n)) > 0) {
        return 1;
    }
    else {
        return 0;
    }
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
    if (this == &bf) //проверяем чтобы сам себе не присваивался
        return *this;
    delete[]this->pMem;
    this->BitLen = bf.BitLen;
    this->MemLen = bf.MemLen;
    this->pMem = new TELEM[MemLen]{};
    for (int i = 0; i < MemLen; i++) {
        this->pMem[i] = bf.pMem[i];
    }
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (BitLen != bf.BitLen) {
        return 0;
    }
    else {
        for (int i = 0; i < MemLen; i++) {
            if (pMem[i] != bf.pMem[i]) {
                return 0;
            }
        }
    }
    return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    if (BitLen != bf.BitLen) {
        return 1;
    }
    else {
        for (int i = 0; i < MemLen; i++) {
            if (pMem[i] != bf.pMem[i]) {
                return 1;
            }
        }
    }
    return 0;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    TBitField temp(max(BitLen, bf.BitLen)); // создаем поезд новый где берем максимальную длину из двух исходных
    for (int i = 0; i < min(BitLen, bf.BitLen); i++) { // бегаем по всем вагонам
        temp.pMem[i] = this->pMem[i] | bf.pMem[i]; //если в поезде есть где-то на месте человек, то мы его добавим в новый поезд
    }
    return temp;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    TBitField temp(max(BitLen, bf.BitLen)); // создаем поезд новый где берем максимальную длину из двух исходных
    for (int i = 0; i < min(BitLen, bf.BitLen); i++) { // бегаем по всем вагонам
        temp.pMem[i] = this->pMem[i] & bf.pMem[i]; //если в поезде и там и там есть на месте человек, то мы его добавим в новый поезд
    }
    return temp;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField temp(*this);
    for (int i = 0; i < BitLen; i++) { // проходим по каждому месту и смотрим если там есть кто-то, то мы его оттуда убираем, и наоборот
        if (temp.GetBit(i) == 1) {
            temp.ClrBit(i);
        }
        else {
            temp.SetBit(i);
        }
    }
    return temp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    int len;
    int k; //переменная для просмотра в длине единиц, чтобы устанавливать длину
    istr >> len;
    if (len < 0) throw " Len < 0 ! ";
    delete[]bf.pMem; // удаляем выделенную память чтобы ее переопределить заново
    bf.BitLen= len;
    if (len % (sizeof(TELEM) * 8) != 0) {
        bf.MemLen = len / (sizeof(TELEM) * 8) + 1;
    }
    else {
        bf.MemLen = len / (sizeof(TELEM) * 8);
    }
    bf.pMem = new TELEM[bf.MemLen]{};
    for (int i = 0; i < len; i++) {
        istr >> k; // считываем введенную последовательность людей, если ноль то никого не садим на это место в новом массиве, иначе садим.
        if (k == 1) {
            bf.SetBit(i);
        }
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.BitLen; i++) {
        if (bf.GetBit(i)) { // проверили сидит ли кто-то на месте, если сидит то выводим в поток
            ostr << i;
        }
    }
    return ostr;
}
