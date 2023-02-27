#include <iostream>
#include <string>
#include <fstream>


using namespace std;


typedef struct tree_element { //структура,представляющая элемент дерева;
   int value;
   struct tree_element* left;
   struct tree_element* right;
} tree_element;

void remove_el(tree_element* el) {  // вспомогательная рекурсивная фунукция удаления поддерева
      if (el != nullptr) {         // если текущий лист не пуст
         remove_el(el->left);    // вызываем очистку для левого
         remove_el(el->right);   // и правого поддеревьев
         delete el;                // и потом удаляем указатель на себя
      }
   }

tree_element* create_tree_element(int i){ //создание нового элемента дерева с заданным значением;
   tree_element* elem = new tree_element;
   elem->value = i;
   elem->left == nullptr;
   elem->right == nullptr;
   return elem;
}

tree_element* root = create_tree_element(0); //создание дерева;

void insert_into_tree(tree_element* root,tree_element* elem){ //вставляет элемент в дерево
   if(elem->value < root->value){
      if(root->left == nullptr){ //идем в левое поддерево;
         root->left = elem; //вставляем,если он пуст;
      }
      else 
{
         insert_into_tree(root->left,elem); //рекурсивно вставляем в левое поддерево;
      }
   }
   else{
      if(root->right == nullptr){
         root->right = elem;
      }
         else {
            insert_into_tree(root->right,elem);
         }
      }
   }

void print_tree(tree_element* cur_elem){ //печать дерева от элемента вниз
   if(cur_elem->left != nullptr){
      print_tree(cur_elem->left);
   }
    cout<<"Value = "<<cur_elem->value<<endl;
   if(cur_elem->right != nullptr){
      print_tree(cur_elem->right);
   }
  
}

void swap(int &left, int &right) {  // своя функция обмена значений
      int temp = left;                 // чтобы не использовать стандартнуб библиотеку
      left = right;
      right = temp;
   }


bool swap_els(tree_element* el, tree_element* prev) {    // функция обмена четных листьев
      if (el != nullptr) {                        // если текущий лист не пуст
         if (el->left != nullptr) {               // идем в левое поддерево
            if (swap_els(el->left, el)) {     // выполняем обмен в нем, и если вернулась правда
                                                    // это значит что четное число сейчас есть в
                                                    // текущем листе и в левом, поэтому
               swap(el->value, el->left->value);  // вызываем функцию обмена значений (Не
            }                                       // из стандартной библиотеки, а свою
         }                                          // для вызова стандартной тут надо было бы написать ::swap(a,b)
         if (el->right != nullptr) {              // тоже самое для правого поддерева
            if (swap_els(el->right, el)) {
               swap(el->value, el->right->value);
            }
         }
         // возвращаем правду, если четное число сейчас в текущем и предыдущем листе
         // для того чтобы предыдущий вызов swap_els выполнил обмен значений листьев
         // тем самым мы спустимся до листьев, и будем поднимать четное значение, если
         // пока предки этого листа четные
         return el->value % 2 == 0 && (prev != nullptr) ? prev->value % 2 == 0 : false;
      }
      return false;
   }
   


void swapHelper() {                 // функция обмена четных листьев
      swap_els(root,nullptr);  // вызывает вспомогательную функцию
   }

int main()
{

string path = "3lab.data"; 
ifstream file;
file.open(path);
int N,k,Size;
cout<<"How much numbers will there be in the tree?"<<endl<<"Numbers: ";
cin>>Size;
int array[Size];
if(!file.is_open())
{
cout<<"File error - cannot open to read data"<<endl;
}
else 
{
   k = 0;
   while(file >> N) //переносим данные из файла в статический массив;
   {
      array[k++] = N;
   }

for(int i = 0; i < Size ; i++){
   tree_element* el = create_tree_element(array[i]); //создание элементов;
   insert_into_tree(root,el); //вставка элементов в дерево;
}
print_tree(root); //печать дерева до изменений;
swapHelper(); // вызываем метод обмена четных листьев в дереве
cout<<"-------------------------------------------------------"<<endl;
print_tree(root); //печать дерева после изменений;
remove_el(root);
return 0;
}
}