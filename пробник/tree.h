#pragma once
#ifndef _MyTree_H_
#define _MyTree_H_
#include<iostream>
#define N 40               	      //Определение максимального количества сыновей
using namespace std;
                                  // Класс узла дерева
struct Tree_node {
    Tree_node* child[N];          //Массив указателей на сыновей
    Tree_node* parent;            //Указатель на родителя
    Tree_node* next_brother;      //Указатель на следующего брата
    int field [10][10] ;          //Состояние поля игры
    int connections[10][10][4];   //Массив клеток, соединённых с данной
    int count_connections[10][10];//Количество клеток, соединённых с данной
    int count_child;              //Количество сыновей
    int number;                   //Номер узла
    int height;                   //Высота узла
    int assessment;               //Оценка текущего состояния игры
    int alpha;                    //Параметр для альфа-бета отсечения
    int betta;                    //Параметр для альфа-бета отсечения
    int count_check_child;        //Количество просмотренных сыновей при поиске лучшего хода
    bool is_check;                //Был ли полностью рассмотрен узел т.е. рассмотрены все его сыновья
    bool is_square;               //Показывет был ли образован квадрат на этом ходу
    Tree_node() {
        for (int i = 0; i < 10; i++){
            for (int j = 0; j < 10; j++){
                field[i][j] = 0;
                count_connections[i][j] = 0;
                connections[i][j][0] = -1;
                connections[i][j][1] = -1;
                connections[i][j][2] = -1;
                connections[i][j][3] = -1;
            }
        }
        number = 0;
        height = 0;
        count_child = 0;
        count_check_child = 0;
        alpha = INT16_MIN;
        betta = INT16_MAX;
        assessment = INT16_MIN;
        parent = nullptr;
        next_brother = nullptr;
        is_square = false;
        is_check = false;
    }
};
int num = 0;            		  //Счётчик узлов
/*Функция для создания узла
 Параметры функции:
     1)Состояние поля
 Результат - указатель на узел
*/
Tree_node* newNode(int value[][10] ) {
    num++;
    Tree_node* temp = new Tree_node;
    for (int i = 0; i < 10; i++){
        for (int j = 0; j < 10; j++){
            temp->field[i][j] = value[i][j];
        }
    }
    temp->number = num;
    return temp;
}
/* Функция для добавления узла в дерево по индексу родителя
Параметры функции:
    1)Указатель на корень дерева
    2)Индекс родительского узла
    3)Значение узла
*/
void push(Tree_node* PNode, int index, int key[][10]) {
    if (PNode->number == index) {
        if (PNode->parent == nullptr) {
            PNode->height = 1;
        }
        PNode->count_child++;
        PNode->child[PNode->count_child-1] = newNode(key);
        PNode->child[PNode->count_child-1]->parent = PNode;
        PNode->child[PNode->count_child-1]->height = PNode->height + 1;
        if (PNode->count_child > 1) {
          PNode->child[PNode->count_child - 2]->next_brother = PNode->child[PNode->count_child - 1];
        }
    }
    for (int i = 0; i < PNode->count_child; i++) {
        push(PNode->child[i], index, key);
    }
}
/*Функция удаления дерева
 Параметры функции:
    1)Указатель на корень дерева
*/
void deletion(Tree_node* PNode) {
    for (int i = 0; i < PNode->count_child; i++) {
        deletion(PNode->child[i]);
    }
    delete PNode;
    num = 0;
}
#endif