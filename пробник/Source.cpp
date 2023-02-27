#include<iostream>
#include <cstdlib>				// для функций rand() и srand()
#include <ctime>				// для функции time()
#include"Tree.h"
using namespace std;

/*Функция проверки завершенности игры
Параметры функии:
	1)Указатель на узел дерева
Результат - значние логического типа: истина или ложь
Принцип работы:
	1)Нулевая гипетеза- игра не закончена.
	2)Если в поле встречается "0"-незанятая клетка, то гипотеза опровергается.
*/
bool game_is_over(Tree_node* PNode) {
	bool res = true;
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			if (PNode->field[i][j] == 0) {
				res = false;
			}
		}
	}
	return res;
}
/*Функция проверки клетки
Параметры:
	1)Указатель на узел дерева
	2)Значение строки текущей клетки
	3)Значение столбца текущей клетки
	4)Значение целого типа: 1 или 2- значение человека или компьютера
	5)Значение типа char: s или с - направление смещения: по строке или столбцу
	6)Значение целого типа: 1 или -1  - значение смещения
Принцип работы:
	1)Копируем значение текущего поля
	2)Смотрим значние параметра direction, если он равен s, то смещаемся на значение параметра step по строке, если directon равен
	  c, то смещаеся по столбцу
	3)Если значение рассматриваемой клетки равно переданному и заданная клетка не с ней имееет связь, то добавляем сына текущему узлу
*/
bool check_cell(Tree_node* PNode, int string, int column, const int value, char direction, const int step) {
	bool flag = false;
	int field[10][10];
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			field[i][j] = PNode->field[i][j];
		}
	}
	if (direction == 'c') {
		if (field[string + step][column] == 0) {
			field[string + step][column] = value;
		}
		if (field[string + step][column] == value) {
			bool res = false;
			for (int i = 0; i < 4; i++) {
				if (PNode->connections[string][column][i] == (string + step) * 10 + column) {
					res = true;
				}
			}
			if (res == false) {
				flag = true;
				push(PNode, PNode->number, field);
				for (int i = 0; i < 10; i++) {
					for (int j = 0; j < 10; j++) {
						PNode->child[PNode->count_child - 1]->count_connections[i][j] = PNode->count_connections[i][j];
						PNode->child[PNode->count_child - 1]->connections[i][j][0] = PNode->connections[i][j][0];
						PNode->child[PNode->count_child - 1]->connections[i][j][1] = PNode->connections[i][j][1];
						PNode->child[PNode->count_child - 1]->connections[i][j][2] = PNode->connections[i][j][2];
						PNode->child[PNode->count_child - 1]->connections[i][j][3] = PNode->connections[i][j][3];
					}
				}
				PNode->child[PNode->count_child - 1]->connections[string + step][column][PNode->child[PNode->count_child - 1]->count_connections[string + step][column]] = string * 10 + column;
				PNode->child[PNode->count_child - 1]->connections[string][column][PNode->child[PNode->count_child - 1]->count_connections[string][column]] = (string + step) * 10 + column;
				PNode->child[PNode->count_child - 1]->count_connections[string][column]++;
				PNode->child[PNode->count_child - 1]->count_connections[string + step][column]++;
			}
		}
	}
	if (direction == 's') {
		if (field[string][column + step] == 0) {
			field[string][column + step] = value;
		}
		if (field[string][column + step] == value) {
			bool res = false;
			for (int i = 0; i < 4; i++) {
				if (PNode->connections[string][column][i] == string * 10 + column + step) {
					res = true;
				}
			}
			if (res == false) {
				flag = true;
				push(PNode, PNode->number, field);
				for (int i = 0; i < 10; i++) {
					for (int j = 0; j < 10; j++) {
						PNode->child[PNode->count_child - 1]->count_connections[i][j] = PNode->count_connections[i][j];
						PNode->child[PNode->count_child - 1]->connections[i][j][0] = PNode->connections[i][j][0];
						PNode->child[PNode->count_child - 1]->connections[i][j][1] = PNode->connections[i][j][1];
						PNode->child[PNode->count_child - 1]->connections[i][j][2] = PNode->connections[i][j][2];
						PNode->child[PNode->count_child - 1]->connections[i][j][3] = PNode->connections[i][j][3];
					}
				}
				PNode->child[PNode->count_child - 1]->connections[string][column + step][PNode->child[PNode->count_child - 1]->count_connections[string][column + step]] = string * 10 + column;
				PNode->child[PNode->count_child - 1]->connections[string][column][PNode->child[PNode->count_child - 1]->count_connections[string][column]] = string * 10 + column + step;
				PNode->child[PNode->count_child - 1]->count_connections[string][column]++;
				PNode->child[PNode->count_child - 1]->count_connections[string][column + step]++;
			}
		}
	}
	return flag;
}
/*Функция проверки того, что на текущем ходу был сформирован квадрат из текущих клеток и клеток, находящихся над ними
Параметры функции:
	1)Указатель на узел дерева
	2)Номер строки первой клетки
	3)Номер столбца первой клетки
	4)Номер строки второй клетки
	5)Номер столбца второй клетки
Принцип работы:
	1)Если у первой клетки есть связь с верхней, то переменная flag_first=true
	2)Если у второй клетки есть связь с верхней, то переменная flag_second=true
	3)Если клетки соеденены с верхними, то проверяем связь верхних клеток между собой
*/
bool is_square_up(Tree_node* PNode, int string_first, int column_first, int string_second, int column_second) {
	bool flag_first = false;
	bool flag_second = false;
	bool flag_up = false;
	for (int i = 0; i < 4; i++) {
		if (PNode->connections[string_first][column_first][i] == (string_first - 1) * 10 + column_first) {
			flag_first = true;
		}
		if (PNode->connections[string_second][column_second][i] == (string_second - 1) * 10 + column_second) {
			flag_second = true;
		}
	}
	if (flag_second == true && flag_first == true) {
		for (int i = 0; i < 4; i++) {
			if (PNode->connections[string_first - 1][column_first][i] == (string_second - 1) * 10 + column_second) {
				flag_up = true;
			}
		}
	}
	return flag_up;
}
/*Функция проверки того, что на текущем ходу был сформирован квадрат из текущих клеток и клеток, находящихся под ними
Параметры функции:
	1)Указатель на узел дерева
	2)Номер строки первой клетки
	3)Номер столбца первой клетки
	4)Номер строки второй клетки
	5)Номер столбца второй клетки
Принцип работы:
	1)Если у первой клетки есть связь с нижней, то переменная flag_first=true
	2)Если у второй клетки есть связь с нижней, то переменная flag_second=true
	3)Если клетки соеденены с нижними, то проверяем связь нижних клеток между собой
*/
bool is_square_down(Tree_node* PNode, int string_first, int column_first, int string_second, int column_second) {
	bool flag_first = false;
	bool flag_second = false;
	bool flag_down = false;
	for (int i = 0; i < 4; i++) {
		if (PNode->connections[string_first][column_first][i] == (string_first + 1) * 10 + column_first) {
			flag_first = true;
		}
		if (PNode->connections[string_second][column_second][i] == (string_second + 1) * 10 + column_second) {
			flag_second = true;
		}
	}
	if (flag_second == true && flag_first == true) {
		for (int i = 0; i < 4; i++) {
			if (PNode->connections[string_first + 1][column_first][i] == (string_second + 1) * 10 + column_second) {
				flag_down = true;
			}
		}
	}
	return flag_down;
}
/*Функция проверки того, что на текущем ходу был сформирован квадрат из текущих клеток и клеток, находящихся справа
Параметры функции:
	1)Указатель на узел дерева
	2)Номер строки первой клетки
	3)Номер столбца первой клетки
	4)Номер строки второй клетки
	5)Номер столбца второй клетки
Принцип работы:
	1)Если у первой клетки есть связь с правой, то переменная flag_first=true
	2)Если у второй клетки есть связь с правой, то переменная flag_second=true
	3)Если клетки соеденены с правыми, то проверяем связь правых клеток между собой
*/
bool is_square_right(Tree_node* PNode, int string_first, int column_first, int string_second, int column_second) {
	bool flag_first = false;
	bool flag_second = false;
	bool flag_right = false;
	for (int i = 0; i < 4; i++) {
		if (PNode->connections[string_first][column_first][i] == string_first * 10 + column_first + 1) {
			flag_first = true;
		}
		if (PNode->connections[string_second][column_second][i] == string_second * 10 + column_second + 1) {
			flag_second = true;
		}
	}
	if (flag_second == true && flag_first == true) {
		for (int i = 0; i < 4; i++) {
			if (PNode->connections[string_first][column_first + 1][i] == string_second * 10 + column_second + 1) {
				flag_right = true;
			}
		}
	}
	return flag_right;
}
/*Функция проверки того, что на текущем ходу был сформирован квадрат из текущих клеток и клеток, находящихся слева
Параметры функции:
	1)Указатель на узел дерева
	2)Номер строки первой клетки
	3)Номер столбца первой клетки
	4)Номер строки второй клетки
	5)Номер столбца второй клетки
Принцип работы:
	1)Если у первой клетки есть связь с левой, то переменная flag_first=true
	2)Если у второй клетки есть связь с левой, то переменная flag_second=true
	3)Если клетки соеденены с леваыми, то проверяем связь левых клеток между собой
*/
bool is_square_left(Tree_node* PNode, int string_first, int column_first, int string_second, int column_second) {
	bool flag_first = false;
	bool flag_second = false;
	bool flag_left = false;
	for (int i = 0; i < 4; i++) {
		if (PNode->connections[string_first][column_first][i] == string_first * 10 + column_first - 1) {
			flag_first = true;
		}
		if (PNode->connections[string_second][column_second][i] == string_second * 10 + column_second - 1) {
			flag_second = true;
		}
	}
	if (flag_second == true && flag_first == true) {
		for (int i = 0; i < 4; i++) {
			if (PNode->connections[string_first][column_first - 1][i] == string_second * 10 + column_second - 1) {
				flag_left = true;
			}
		}
	}
	return flag_left;
}
/*Функция проверки того, что на текущем ходу был сформирован квадрат
Параметры функции:
	1)Указатель на узел дерева
	2)Номер строки первой клетки
	3)Номер столбца первой клетки
	4)Номер строки второй клетки
	5)Номер столбца второй клетки
Принцип работы:
	1)Смотрим номер строки.Если клетки расположены на одной строке:
	1.1)Если она последняя,то вызываем функцию is_square_up()
	1.2)Если она первая,то вызываем функцию is_square_down()
	1.3)Если строка не крайняя,то вызывем функции is_square_up() и is_square_down()
	2)Смотрим номер столбца. Если клетки в одном столбце:
	2.1)Если он последний,то вызываем функцию is_square_left()
	2.2)Если он первый,то вызываем функцию is_square_right()
	2.3)Если столбец не крайний, то вызывем функции is_square_left() и is_square_right()
*/
void is_square(Tree_node* PNode, int string_first, int column_first, int string_second, int column_second) {
	bool flag = false;
	if (string_first == string_second) {
		if (string_first == 9) {
			flag = is_square_up(PNode, string_first, column_first, string_second, column_second);
			if (flag == true) { PNode->is_square = true; }
		}
		if (string_first == 0) {
			flag = is_square_down(PNode, string_first, column_first, string_second, column_second);
			if (flag == true) { PNode->is_square = true; }
		}
		if (string_first > 0 && string_first < 9) {
			flag = is_square_up(PNode, string_first, column_first, string_second, column_second);
			if (flag == true) { PNode->is_square = true; }
			flag = is_square_down(PNode, string_first, column_first, string_second, column_second);
			if (flag == true) { PNode->is_square = true; }
		}
	}
	if (column_first == column_second) {
		if (column_first == 9) {
			flag = is_square_left(PNode, string_first, column_first, string_second, column_second);
			if (flag == true) { PNode->is_square = true; }
		}
		if (column_first == 0) {
			flag = is_square_right(PNode, string_first, column_first, string_second, column_second);
			if (flag == true) { PNode->is_square = true; }
		}
		if (column_first > 0 && column_first < 9) {
			flag = is_square_left(PNode, string_first, column_first, string_second, column_second);
			if (flag == true) { PNode->is_square = true; }
			flag = is_square_right(PNode, string_first, column_first, string_second, column_second);
			if (flag == true) { PNode->is_square = true; }
		}
	}
}
/*Функция генерации хода
Параметры функии:
	1)Указатель на узел дерева
	2)Значение целого типа: 1, если нужно сгенерировать ход для компьютера,
	  2 - для человека.
Принцип работы:
	1)Просматриваем все значения клеток текущего игрового поля
	2)Если значение равно переданному, то рассматриваем его позицию.
	3)В зависимости от позиции равссматриваем соседнии клетки
	4)Ели узел был добвлен, проверяем был ли собрана квадрат на данном ходу
*/
void generation_move(Tree_node* PNode, const int value) {
	bool flag;
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			if (PNode->field[i][j] == value) {
				if (i == 0 && j == 0) {
					flag = check_cell(PNode, i, j, value, 's', 1);
					if (flag == true) { is_square(PNode->child[PNode->count_child - 1], i, j, i, j + 1); }
					flag = check_cell(PNode, i, j, value, 'c', 1);
					if (flag == true) { is_square(PNode->child[PNode->count_child - 1], i, j, i + 1, j); }
				}
				if (i == 0 && j == 9) {
					flag = check_cell(PNode, i, j, value, 's', -1);
					if (flag == true) { is_square(PNode->child[PNode->count_child - 1], i, j, i, j - 1); }
					flag = check_cell(PNode, i, j, value, 'c', 1);
					if (flag == true) { is_square(PNode->child[PNode->count_child - 1], i, j, i + 1, j); }
				}
				if (i == 9 && j == 0) {
					flag = check_cell(PNode, i, j, value, 'c', -1);
					if (flag == true) { is_square(PNode->child[PNode->count_child - 1], i, j, i - 1, j); }
					flag = check_cell(PNode, i, j, value, 's', 1);
					if (flag == true) { is_square(PNode->child[PNode->count_child - 1], i, j, i, j + 1); }
				}
				if (i == 9 && j == 9) {
					flag = check_cell(PNode, i, j, value, 'c', -1);
					if (flag == true) { is_square(PNode->child[PNode->count_child - 1], i, j, i - 1, j); }
					check_cell(PNode, i, j, value, 's', -1);
					if (flag == true) { is_square(PNode->child[PNode->count_child - 1], i, j, i, j - 1); }
				}
				if (i == 0 && j > 0 && j < 9) {
					flag = check_cell(PNode, i, j, value, 's', -1);
					if (flag == true) { is_square(PNode->child[PNode->count_child - 1], i, j, i, j - 1); }
					flag = check_cell(PNode, i, j, value, 's', 1);
					if (flag == true) { is_square(PNode->child[PNode->count_child - 1], i, j, i, j + 1); }
					flag = check_cell(PNode, i, j, value, 'c', 1);
					if (flag == true) { is_square(PNode->child[PNode->count_child - 1], i, j, i + 1, j); }
				}
				if (i == 9 && j > 0 && j < 9) {
					flag = check_cell(PNode, i, j, value, 's', -1);
					if (flag == true) { is_square(PNode->child[PNode->count_child - 1], i, j, i, j - 1); }
					flag = check_cell(PNode, i, j, value, 's', 1);
					if (flag == true) { is_square(PNode->child[PNode->count_child - 1], i, j, i, j + 1); }
					flag = check_cell(PNode, i, j, value, 'c', -1);
					if (flag == true) { is_square(PNode->child[PNode->count_child - 1], i, j, i - 1, j); }
				}
				if (j == 0 && i != 0 && i != 9) {
					flag = check_cell(PNode, i, j, value, 'c', -1);
					if (flag == true) { is_square(PNode->child[PNode->count_child - 1], i, j, i - 1, j); }
					flag = check_cell(PNode, i, j, value, 'c', 1);
					if (flag == true) { is_square(PNode->child[PNode->count_child - 1], i, j, i + 1, j); }
					flag = check_cell(PNode, i, j, value, 's', 1);
					if (flag == true) { is_square(PNode->child[PNode->count_child - 1], i, j, i, j + 1); }
				}
				if (j == 9 && i != 0 && i != 9) {
					flag = check_cell(PNode, i, j, value, 'c', -1);
					if (flag == true) { is_square(PNode->child[PNode->count_child - 1], i, j, i - 1, j); }
					flag = check_cell(PNode, i, j, value, 'c', 1);
					if (flag == true) { is_square(PNode->child[PNode->count_child - 1], i, j, i + 1, j); }
					flag = check_cell(PNode, i, j, value, 's', -1);
					if (flag == true) { is_square(PNode->child[PNode->count_child - 1], i, j, i, j - 1); }
				}
				if (i != 0 && i != 9 && j != 0 && j != 9) {
					flag = check_cell(PNode, i, j, value, 's', -1);
					if (flag == true) { is_square(PNode->child[PNode->count_child - 1], i, j, i, j - 1); }
					flag = check_cell(PNode, i, j, value, 's', 1);
					if (flag == true) { is_square(PNode->child[PNode->count_child - 1], i, j, i, j + 1); }
					flag = check_cell(PNode, i, j, value, 'c', -1);
					if (flag == true) { is_square(PNode->child[PNode->count_child - 1], i, j, i - 1, j); }
					flag = check_cell(PNode, i, j, value, 'c', 1);
					if (flag == true) { is_square(PNode->child[PNode->count_child - 1], i, j, i + 1, j); }
				}
			}
		}
	}
}
/*Функция постоения дерева возможных ходов
Параметры:
	1)Указатель на корень дерева
Принцип работы:
	1)Функция повторяется, пока высота дерева не равна 5
	2)Если высота текущего дерева нечетная, то следующий ход компьютера
	2.1)Если на предыдущем ходу был собран квадрат, то добавляем сына с текущем состоянием поля т.к. человек пропускает ход
	2.2)Если на предыдущем ходу не был собран квадрат, то вызываем функцию generation_move с параметром 2.
	3)Если высота текущего дерева четная, то следующий ход человека
	3.1)Если на предыдущем ходу был собран квадрат, то добавляем сына с текущем состоянием поля т.к. компьютер пропускает ход
	3.2)Если на предыдущем ходу не был собран квадрат, то вызываем функцию generation_move с параметром 1.
*/
void make_tree(Tree_node* PNode) {
	if (PNode->height % 2 == 1) {
		if (PNode->is_square == false) {
			generation_move(PNode, 1);
		}
		else {
			push(PNode, PNode->number, PNode->field);
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 10; j++) {
					PNode->child[0]->count_connections[i][j] = PNode->count_connections[i][j];
					PNode->child[0]->connections[i][j][0] = PNode->connections[i][j][0];
					PNode->child[0]->connections[i][j][1] = PNode->connections[i][j][1];
					PNode->child[0]->connections[i][j][2] = PNode->connections[i][j][2];
					PNode->child[0]->connections[i][j][3] = PNode->connections[i][j][3];
				}
			}
		}
	}
	if (PNode->height % 2 == 0) {
		if (PNode->is_square == false) {
			generation_move(PNode, 2);
		}
		else {
			push(PNode, PNode->number, PNode->field);
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 10; j++) {
					PNode->child[0]->count_connections[i][j] = PNode->count_connections[i][j];
					PNode->child[0]->connections[i][j][0] = PNode->connections[i][j][0];
					PNode->child[0]->connections[i][j][1] = PNode->connections[i][j][1];
					PNode->child[0]->connections[i][j][2] = PNode->connections[i][j][2];
					PNode->child[0]->connections[i][j][3] = PNode->connections[i][j][3];
				}
			}
		}
	}
	if (PNode->height <= 3) {
		for (int i = 0; i < PNode->count_child; i++) {
			make_tree(PNode->child[i]);
		}
	}
}
/*Оценочная функция
Параметры:
	1)Указатель на узел дерева
Принцип работы:
	1)Просматриваются значения клеток текущего поля
	2)Если значение клетки равно 1, то она принадлежит компьютеру. К результату прибавляется количество связей этой клетки
	3)Если значение клетки равно 2, то она принадлежит человеку. От результата вычитается количество связей этой клетки
*/
void get_assessment(Tree_node* PNode) {
	int res = 0;
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			if (PNode->field[i][j] == 1) {
				res = res + PNode->count_connections[i][j];
			}
			if (PNode->field[i][j] == 2) {
				res = res - PNode->count_connections[i][j];
			}
		}
	}
	PNode->assessment = res;
}
/*Функция альфа бетта отсечения
Параметры:
	1)Указатель на узел дерева
Принцип работы:
	1)Если узел является листом, то высчитываем его оценку
	2)Если проверены не все сыновья узла, то применяем функция к непроверенным сыновьям
	3)Если узел не является корнем, то копируем значения alpha и betta родителя узла
	4)Вычисялем значение проверенных сыновей
	5)Если проверены все сыновья, то узел проверен
	6)Если узел не корень:
	6.1)Если родитель находится на чётном уровне (минимума), то корректируем значение betta родителя узла
	6.2)Если родитель находится на нечётном уровне (максимума), то корректируем значение alpha родителя узла
	6.3)Если у родителя узла alpha<betta:
	6.3.1)Значение оценки родителя равно оценке текущего узла
	6.3.2)Если у узла есть следующий брат, то переходим к нему
	6.3.3)Переходим к родителю текущего узла
	6.4)Если у родителя узла alpha>betta:
	6.4.1)Переходим к родителю текущего узла
	6.4.2)Количетсво проверенных сыновей = количество сыновей
	6.4.3)Узел проверен
*/
void alpha_betta(Tree_node* PNode) {
	if (PNode->height == 5) {
		get_assessment(PNode);
	}
	if (PNode->count_check_child != PNode->count_child) {
		for (int i = PNode->count_check_child; i < PNode->count_child; i++) {
			alpha_betta(PNode->child[i]);
		}
	}
	if (PNode->height != 1) {
		PNode->alpha = PNode->parent->alpha;
		PNode->betta = PNode->parent->betta;
	}
	for (int i = 0; i < PNode->count_child; i++) {
		if (PNode->child[i]->is_check == true) {
			PNode->count_check_child++;
		}
	}
	if (PNode->count_check_child == PNode->count_check_child) {
		PNode->is_check = true;
	}
	if (PNode->height != 1) {
		if (PNode->parent->height % 2 == 1) {
			if (PNode->parent->betta > PNode->assessment)
				PNode->parent->betta = PNode->assessment;
		}
		if (PNode->parent->height % 2 == 0) {
			if (PNode->parent->alpha < PNode->assessment) {
				PNode->parent->alpha = PNode->assessment;
			}
		}
		if (PNode->parent->alpha < PNode->parent->betta) {
			PNode->parent->assessment = PNode->assessment;
			if (PNode->next_brother != nullptr) {
				PNode = PNode->next_brother;
			}
			else {
				PNode = PNode->parent;
			}
		}
		else {
			PNode = PNode->parent;
			PNode->count_check_child = PNode->count_child;
			PNode->is_check = true;
		}
	}
}
/*Функция поиска лучшего хода
Параметры:
	1)Указатель на корень дерева
Принцип работы:
	1)Проверяем оценку сыновей корня
	2)Если значение оценки сына равно оценке корня, то меняем значение поля корня на значение поля текущего сына
	3)Если встречается ещё один сын с оценкой, равной оценке корня, то с вероятностью 50%
		(проверяем остаток случайно сгенерированного числа от деления на 2) заменяем значение поля корня
*/
void choose_best_move(Tree_node* PNode) {
	int tmp = 0;
	srand(unsigned int(time(0)));
	for (int k = 0; k < PNode->count_child; k++) {
		if (PNode->child[k]->assessment == PNode->assessment) {
			tmp++;
			if (tmp == 1) {
				for (int i = 0; i < 10; i++) {
					for (int j = 0; j < 10; j++) {
						PNode->field[i][j] = PNode->child[k]->field[i][j];
						PNode->count_connections[i][j] = PNode->child[k]->count_connections[i][j];
						for (int l = 0; l < 4; l++) {
							PNode->connections[i][j][l] = PNode->child[k]->connections[i][j][l];
						}
					}
				}
				if (PNode->child[k]->is_square == true) { PNode->is_square = true; }
			}
			else {
				int r = rand() % 2;
				if (r == 0) {
					for (int i = 0; i < 10; i++) {
						for (int j = 0; j < 10; j++) {
							PNode->field[i][j] = PNode->child[k]->field[i][j];
							PNode->count_connections[i][j] = PNode->child[k]->count_connections[i][j];
							for (int l = 0; l < 4; l++) {
								PNode->connections[i][j][l] = PNode->child[k]->connections[i][j][l];
							}
						}
					}
					if (PNode->child[k]->is_square == true) { PNode->is_square = true; }
				}
			}
		}
	}
}
/*Функция вывода теущего поля
Параметры:
	1)Указатель на узел дерева
Принцип работы:
	1)Если значение клетки равно 1, то выводим красную точку,2 - синию, 0-белую
	2)Если у клетки есть связь с правой, то выводим "-" соответствующего цвета, иначе " "
	3)Если у клетки есть связь с нижней, то выводим "|"соответствующего цвета " "
*/
void Print_field(Tree_node* PNode) {
	cout << " \u001B[33m 0   1   2   3   4   5   6   7   8   9\n";
	for (int i = 0; i < 10; i++) {

		if (i > 0) {
			for (int j = 0; j < 10; j++) {
				bool f = false;
				for (int k = 0; k < 4; k++) {
					if (PNode->connections[i][j][k] == (i - 1) * 10 + j) { f = true; }
				}
				if (f == true) {
					if (PNode->field[i][j] == 1) { cout << "\u001B[31m  | "; }
					else { cout << "\u001B[34m  | "; }
				}
				else { cout << "    "; }
			}
			cout << "\n";
		}
		cout << "\u001B[33m" << i << " ";
		for (int j = 0; j < 10; j++) {
			if (PNode->field[i][j] == 1) { cout << "\u001B[31m."; }
			if (PNode->field[i][j] == 2) { cout << "\u001B[34m."; }
			if (PNode->field[i][j] == 0) { cout << "\u001B[32m."; }
			bool f = false;
			for (int k = 0; k < 4; k++) {
				if (PNode->connections[i][j][k] == i * 10 + j + 1) { f = true; }
			}
			if (f == true) {
				if (PNode->field[i][j] == 1) { cout << "\u001B[31m - "; }
				else { cout << "\u001B[34m - "; }
			}
			else { cout << "   "; }
		}
		cout << "\n";
	}
	cout << "\n";
}
/*Функция создания корня дерева
Параметры функции:
	1)Матрица состояния текущего игровго поля
Принцип работы:
	1)Создание новго узла с переданным значением
	2)Высота узла равна 1
*/
Tree_node* creat_root(int start_field[10][10]) {
	Tree_node* root = newNode(start_field);
	root->height = 1;
	return root;
}
/*Функция пересоздания корня
Параметры:
	1)Указатель на корень
Принцип работы:
	1)Копирование состояния поля, связей клеток и их количества во временные переменные
	2)Удаление дерева по указателю на корень
	3)Создание нового корня, по переданному указателю
	4)Сохранение значний переданного корня
*/
Tree_node* re_create_root(Tree_node* root) {
	int field[10][10];
	int count_connections[10][10];
	int connections[10][10][4];
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			field[i][j] = root->field[i][j];
			count_connections[i][j] = root->count_connections[i][j];
			for (int k = 0; k < 4; k++) {
				connections[i][j][k] = root->connections[i][j][k];
			}
		}
	}
	deletion(root);
	root = creat_root(field);
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			root->count_connections[i][j] = count_connections[i][j];
			for (int k = 0; k < 4; k++) {
				root->connections[i][j][k] = connections[i][j][k];
			}
		}
	}
	return root;
}
/*Функция считывания хода человека
Параметры функии:
	1)Указатель на узел дерева
Принцип работы:
	1)Считываются номера строки и столбца начальной клетки.
	2)Если они выходят за диапозон [0;9], то сообщается об ошибке, функция перезапускается.
	3)Считываются номера строки и столбца целевой клетки.
	4)Если они выходят за диапозон [0;9], то сообщается об ошибке, функция перезапускается.
	5)Если целевая и начльная клетка не соседние, то сообщается об ошибке, функция перезапускается.
	6)Значение поля в целевой клетке заполняется значением "2".
	7)К количествам связей начальной о целевой клеток добаляется единица.
	8)Значения строк и столбцов начальной и целевой клеток записываются в соответствующие массивы соединённых клеток.
*/
void read_human_move(Tree_node* PNode) {
	int start_string = 0, start_column = 0, finish_string = 0, finish_column = 0;
	cout << "\u001B[37mВведите строку начальной клетки  : ";
	cin >> start_string;
	cout << "Введите столбец начальной клетки : ";
	cin >> start_column;
	if (start_column < 10 && start_column >= 0 && start_string < 10 && start_string >= 0) {
		if (PNode->field[start_string][start_column] == 2) {
			cout << "Введите строку целевой клетки  : ";
			cin >> finish_string;
			cout << "Введите столбец целевой клетки: ";
			cin >> finish_column;
			if (finish_column < 10 && finish_column >= 0 && finish_string < 10 && finish_string >= 0) {
				if (PNode->field[finish_string][finish_column] != 1) {
					if (abs(start_string - finish_string) == 1 && start_column == finish_column
						|| abs(start_column - finish_column) == 1 && start_string == finish_string) {
						PNode->field[finish_string][finish_column] = 2;
						PNode->count_connections[start_string][start_column]++;
						PNode->count_connections[finish_string][finish_column]++;
						int i = 0;
						while (PNode->connections[start_string][start_column][i] != -1) { i++; }
						PNode->connections[start_string][start_column][i] = finish_string * 10 + finish_column;
						i = 0;
						while (PNode->connections[finish_string][finish_column][i] != -1) { i++; }
						PNode->connections[finish_string][finish_column][i] = start_string * 10 + start_column;
						is_square(PNode, start_string, start_column, finish_string, finish_column);
					}
					else {
						cout << "\u001B[31m\tЭту клетку нельзя соеденить\n"
							<< "\tОт начальной клетки до нее не достать\n\n";
						read_human_move(PNode);
					}
				}
				else {
					cout << "\u001B[31m\tЭту клетку нельзя соеденить\n"
						<< "\tОна принадлежит сопернику\n\n";
					read_human_move(PNode);
				}
			}
			else {
				cout << "\u001B[31m\tЦелевая клетка вне допустимого диапозона\n"
					<< "\tЗначния строки и столбца должны лежать в интервале от 0 до 9\n\n";
				read_human_move(PNode);
			}
		}
		else {
			cout << "\u001B[31m\tЭта клетка не принадлежит вам\n\n";
			read_human_move(PNode);
		}
	}
	else {
		cout << "\u001B[31m\tНачальная клетка вне допустимого диапозона\n"
			<< "\tЗначния строки и столбца должны лежать в интервале от 0 до 9\n\n";
		read_human_move(PNode);
	}
}
/*Функция игры
Параметры:
	1)Указатель на узел дерева
Принцип работы:
	1)Функция повторяется пока игра не окончена
	2)i- показатель того, что компьютер собрал квадрат
	3)Если компьютером был собран квадрат,то выводим  соответствующую надпись
	4)Выводим поле игры
	4)Считываем ход человека
	5)Если он собрал квадрат - повторяем пункты 4)-5)
	6)Строим дерево возможных ходов
	7)Производим алгоритм альфа-бетта отсечения
	8)Выбираем лучший ход
	9)Если был собран квадрат- повторяем пункты 6)-9)
*/
Tree_node* game(Tree_node* root) {
	int i = 0;
	while (game_is_over(root) == false) {
		system("cls");
		if (i > 0) {
			cout << "\u001B[35mÊîìïüþòåð ñîáðàë êâàäðàò \n";
			i = 0;
		}
		Print_field(root);
		read_human_move(root);
		while (root->is_square == true) {
			system("cls");
			Print_field(root);
			cout << "\u001B[35mÂû ñîáðàëè êâàäðàò\n";
			root->is_square = false;
			read_human_move(root);
		}
		make_tree(root);
		alpha_betta(root);
		choose_best_move(root);
		while (root->is_square == true) {
			root = re_create_root(root);
			root->is_square = false;
			make_tree(root);
			alpha_betta(root);
			choose_best_move(root);
			i++;
		}
		root = re_create_root(root);
	}
	return root;
}
/*Функция вывода результата игры
Параметры:
	1)Указатель на корень дерева
Принцип работы:
	1)Подсчёт баллов игрока и компьютера
	2)Вывод результата
*/
void get_result(Tree_node* root) {
	int score_c = 0, score_h = 0;
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			if (root->field[i][j] == 1) { score_c++; }
			if (root->field[i][j] == 2) { score_h++; }
		}
	}
	if (score_h > score_c) { cout << "\u001B[36m\tВы выграли! Ваш счёт " << score_h << " Счёт компьютера " << score_c << "\n"; }
	if (score_h < score_c) { cout << "\u001B[36m\tВы проиграли( Ваш счёт  " << score_h << " Счёт компьютера" << score_c << "\n"; }
	if (score_h == score_c) { cout << "\u001B[36\tmНичья! Ваш счёт " << score_h << " Счёт компьютера " << score_c << "\n"; }
}
int main() {
	setlocale(LC_ALL, "ru");     		///Включение русского языка
	int field[10][10];
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			field[i][j] = 0;
		}
	}
	field[9][0] = 1;
	field[0][9] = 2;
	Tree_node* root;
	root = creat_root(field);
	root = game(root);
	Print_field(root);
	system("cls");
	get_result(root);
	return 0;
}
