#include <fstream>
#include <iostream>

using namespace std;

template <typename type>
class vector {  // Шаблонный класс вектора
   public:
   size_t size() const {  // получение размера вектора
      return m_size;
   }

   vector() = default;  // конструктор по умолчанию создаст пустой вектор с размером 0

   vector(size_t m_size) : m_size{m_size}, capacity{m_size} {  // конструктор по размер с занулением
      array = new type[m_size];
      for (size_t i = 0; i < m_size; i++) {
         array[i] = type{};  // type{} - вызывает конструктор по умолчанию для типа type,
                             // в случае int вызовет это нулевой конструктор, т.е. будет 0, для float 0.f итп
      }
   }

   vector &operator=(const vector &other) {  // оператор присваивания
      m_size = other.m_size;
      capacity = other.capacity;

      delete[] array;                       // удаляем предыдущий массив, если там nullptr то обычно ничего не произойдет
      array = new type[m_size];              // выделяем новую память
      for (size_t i = 0; i < m_size; i++) {  // копируем поэлементно
         array[i] = other.array[i];
      }
      return *this;
   }

   void push_back(const type &value) {         // добавление нового элемента в конец
      if (m_size == capacity) {                 // если мы достигли вместимости (например, в начале 0 == 0)
         type *oldArray = array;               // сохраняем указатель на старый массив
         capacity = 2 * (m_size + 1);           // добавляем вместимости
         array = new type[capacity];           // выделяем новую память
         for (size_t i = 0; i < m_size; i++) {  // поэлементно копируем из старого
            array[i] = oldArray[i];
         }
         delete[] oldArray;  // не забываем удалить старый массив
      }
      array[m_size] = value;
      m_size++;
   }

   type operator[](const size_t index) const {  // получение элемента вектора по индексу
      if (index >= m_size) {
         // throw std::runtime_error("Плохой индекс вектора");
         return 0;
      }
      return array[index];
   }

   ~vector() {  // Деструктор вектора, по умолчанию не подойдет, потому что надо вручную удалить память
      delete[] array;
   }

   private:
   size_t m_size{0};       // текущий размер вектора
   size_t capacity{0};    // вместимость вектора
   type *array{nullptr};  // указатель на массив данных типа type
};

struct n_leaf {               // структура для хранения листа дерева
   int id{};                  // хранит уникальный идентификатор листа
   int weight{};              // его вес
   vector<n_leaf *> leafs{};  // и указатели на всех детей
};

struct density {       // структура для хранения плотности поддерева
   n_leaf *subTree{};  // указатель на начало поддерева (лист в дереве)
   int totalWeight{};  // вес всего поддерева
   size_t size{};      // размер поддерева
   int getdensity() const {
      return float(totalWeight / size);
   }
};

class n_tree {  // класс N дерева
   private:
   n_leaf *head{};  // указатель на корень дерева

   bool add_helper(n_leaf *leaf, n_leaf *newLeaf) {  // рекурсивная вспомогательная функция
      if (leaf->id == newLeaf->id) {                 // добавления листа дерева по идентефикатору
         *leaf = *newLeaf;
         return true;
      }
      for (size_t index = 0; index < leaf->leafs.size(); index++) {  // проходимся по всем поддеревьям
         if (add_helper(leaf->leafs[index], newLeaf)) {              // если добавление случилось, заканчиваем работать
            return true;
         }
      }
      return false;  // если идентификатор не был найден, то возвращаем ложь
   }

   void print_helper(const n_leaf *leaf, const size_t currentDepth) const {  // вспомогательная рекурсивная функция
      if (leaf != nullptr) {                                                 // печати дерева на консоль
         for (size_t depth = 0; depth < currentDepth; depth++) {             // печатает листья учитывая их удаленность
            cout << "---- ";                                                 // от корня
         }
         cout << leaf->id << ": " << leaf->weight << endl;
         for (size_t index = 0; index < leaf->leafs.size(); index++) {
            print_helper(leaf->leafs[index], currentDepth + 1);
         }
      }
   }
   void remove_tree(n_leaf *leaf) {                                     // вспомогательная рекурсивная фунукция удаления поддерева
      if (leaf != nullptr) {                                            // если текущий лист не пуст
         for (size_t index = 0; index < leaf->leafs.size(); index++) {  // удаляем все поддеревья
            remove_tree(leaf->leafs[index]);
         }
         delete leaf;  // и потом удаляем указатель на себя
      }
   }
   density getdensity_helper(n_leaf *leaf) {  // вспомогательная функция получения плотности поддерева
      if (leaf != nullptr) {
         density selfdensity{leaf, leaf->weight, 1};  // добавляем текущий лист
         for (size_t index = 0; index < leaf->leafs.size(); index++) {
            density temp = getdensity_helper(leaf->leafs[index]);  // и от каждого листа получам плотность
            selfdensity.totalWeight += temp.totalWeight;           // добавляя его вес
            selfdensity.size += temp.size;                         // и размер к итогу
         }
         return selfdensity;
      }
      return {};
   }

   density get_max_density_helper(n_leaf *leaf) {  // шаблонная вспомогательная функция получения (макси\мини)мально плотного поддерева
      if (leaf != nullptr) {
         density curr = getdensity_helper(leaf);  // получаем плотность для текущего поддерева
         for (size_t index = 0; index < leaf->leafs.size(); index++) {
            density temp = get_max_density_helper(leaf->leafs[index]);  // получаем макс плотности для поддеревьев
            if (temp.getdensity() > curr.getdensity()) {
               curr = temp;
            }
         }
         return curr;  // возвращаем максимальную плотность из проверенных
      }
      return {};
   }

   density get_min_density_helper(n_leaf *leaf) {  // шаблонная вспомогательная функция получения (макси\мини)мально плотного поддерева
      if (leaf != nullptr) {
         density curr = getdensity_helper(leaf);  // получаем плотность для текущего поддерева
         for (size_t index = 0; index < leaf->leafs.size(); index++) {
            density temp = get_min_density_helper(leaf->leafs[index]);  // получаем макс плотности для поддеревьев
            if (temp.getdensity() < curr.getdensity()) {
               curr = temp;
            }
         }
         return curr;  // возвращаем максимальную плотность из проверенных
      }
      return {};
   }

   n_leaf *copy_from_leaf_helper(n_leaf *otherLeaf) {  // вспомогательная функция копирования поддерева
      n_leaf *newLeaf{};
      if (otherLeaf != nullptr) {
         newLeaf = new n_leaf{otherLeaf->id, otherLeaf->weight};                       // создаем новый лист с таким же ид и весом
         for (size_t index = 0; index < otherLeaf->leafs.size(); index++) {            // и наполняем его вектор детей копиями
            newLeaf->leafs.push_back(copy_from_leaf_helper(otherLeaf->leafs[index]));  // поддеревьев оригинального поддерева
         }
      }
      return newLeaf;
   }

   public:
   n_tree() = default;     // Констуктор по умоланию, корень дерева nullptr == 0
   n_tree(n_leaf *leaf) {  // Конструктор по поддереву, создаст копию дерева начиная с leaf
      head = copy_from_leaf_helper(leaf);
   }
   void print() const {       // функция печати дерева
      print_helper(head, 0);  // вызывает вспомогательную функцию начиная с корня
   }

   bool add(n_leaf *newn_leaf) {  // функция добавления нового листа
      if (head == nullptr) {      // если корня дерева нет
         head = new n_leaf{};
         *head = *newn_leaf;
         return true;
      }
      return add_helper(head, newn_leaf);
   }
   density get_max_density() {
      return get_max_density_helper(head);
   }
   density get_min_density() {
      return get_min_density_helper(head);
   }

   ~n_tree() {            // деструктор N дерева
      remove_tree(head);  // вызывает вспомогательную функцию начиная с корня
   }
};

bool read_tree(istream &input, n_tree &tree) {  // функция чтения дерева из любого
   size_t size{};                               // входого потока (cin или ifstream в частности)
   input >> size;
   vector<int> leafs{};
   int id{}, weight{};
   size_t leafSize{};
   for (size_t index = 0; index < size; index++) {  // для каждого листа
      input >> id >> weight >> leafSize;            // вводится ид, вес и количество детей
      n_leaf *newn_leaf = new n_leaf{id, weight};
      for (size_t indexLeaf = 0; indexLeaf < leafSize; indexLeaf++) {
         input >> id;
         newn_leaf->leafs.push_back(new n_leaf{id});
      }
      if (!input) {  // считвыем числа пока это возможно
         cout << "Обнаружен недопустимый символ" << endl;
         delete newn_leaf;
         return false;
      }
      if (!tree.add(newn_leaf)) {  // проверка на идентификаторы уже внутри add
         cout << "Введён не существующий id:" << id << endl;
         delete newn_leaf;
         return false;
      };
      delete newn_leaf;
   }

   return true;  // возвращаем флаг того, дошли ли мы до конца ввода
}  // если нет, это значит было введено не число

int main(int argc, char **argv) {
   n_tree tree;  // создаем  N дерево

   if (argc == 2) {  // считываем из файла
      ifstream file(argv[1]);
      file.open(argv[1]);
      if (!file.is_open()) {
         cout << "Плохой файл." << endl;
         return 0;
      }
      if (!read_tree(file, tree)) {
         return 0;
      };
   } else if (argc == 1) {  // или с консоли
      if (!read_tree(cin, tree)) {
         return 0;
      }
   } else {
      cout << "Неверные аргументы." << endl;
      return 0;
   }

   cout << "Оригинальое N Дерево:" << endl;
   tree.print();
   {
      cout << "Максимально плотное (max веса/количество) поддерево:" << endl;
      density density = tree.get_max_density();
      n_tree subtree{density.subTree};  // создаем дерево из полученного поддерева
      subtree.print();                  // что бы распечатать его
      cout << "Плотность равна: " << density.totalWeight / density.size << endl;
   }
   {
      cout << "Минимально плотное (max веса/количество) поддерево:" << endl;
      density density = tree.get_min_density();
      n_tree subtree{density.subTree};  // создаем дерево из полученного поддерева
      subtree.print();                  // что бы распечатать его
      cout << "Плотность равна: " << density.totalWeight / density.size << endl;
   }
   return 0;
}
