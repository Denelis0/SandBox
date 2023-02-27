#include <iostream>
#include <string>
#include <fstream>

using namespace std;

const int Size = 1000;
int counter = 0;
int numbers = 0;
int array[Size][Size];

void Array()
{
    for(int i = 0 ; i < Size ; i++)
    {
        for(int j = 0 ; j < Size ; j++)
        {
            array[i][j] = 0;
        }
    }
     
    }


void check_function(int a,int b)
{

if(array[a][b] != 1)
{
    array[a][b] = 1;
   
}
else
{

   counter++;
}
}

class List
{
public:

void removal(); //удаление всех элементов;
void pop_front(); //удаление первого элемента в списке;
void push_back(int a,int b); //добавление элемента в список;

private:
class Node
{
    public:
Node *pNext;
int a,b;

Node(int a,int b,Node *pNext = nullptr)
{
    this->a = a;
    this->b = b;
    this->pNext = pNext;
}
};

Node *head;

};

void List::pop_front()
{
    Node *temp = head; //вспомогательный указатель берет адрес "головы" на себя;
    head = head->pNext; //"голова" берет адрес следующего элемента;

    delete temp; //освобождение памяти;
    numbers--;

}

void List::removal()
{
    while(numbers)
    {
        pop_front(); //удаление всех элементов,пока numbers не станет 0 (false);
    }
}

void List::push_back(int a,int b)
{
if(head == nullptr) //если нет начала списка;
{
head = new Node(a,b); //создаем начальный элемент;
numbers++;
check_function(a,b); //проверяем на уникальность;
}
else 
{
Node *current = this->head;
while(current->pNext != nullptr) //ищем последний элемент;
{
    current = current->pNext;
} 
check_function(a,b); //проверяем на уникальность;
numbers++;
current->pNext = new Node(a,b);
}
}



int main() {

void Array(); //заполняем массив array нулями;

string path = "1lab.data"; 
ifstream fin;
fin.open(path); //проверяем файл;
List point;
if(!fin.is_open())
{
cout<<"File error - cannot open to read data"<<endl;
}
else 
{
cout<<"Coordinates:"<<endl;
char buff[2];
int a,b;

while(fin.get(buff,sizeof(buff))) //вытаскиваем по два элемента
{

fin>>a>>b;
cout<<a<<b<<'\t';
point.push_back(a,b); //добавляем элемент в односвязный список

}
cout<<endl;
}
cout<<"Repeat moves is: "<<counter<<endl;

point.removal(); //вызов функции "удаление";
fin.clear();
return 0;
};