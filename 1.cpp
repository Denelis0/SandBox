#include <fstream>
#include <iostream>

using namespace std;

template <typename type>
class vector {  // Шаблонный класс вектора
   public:
   size_t size()  const {  // получение размера вектора
      return m_size;
   }

   vector() = default;  // конструктор по умолчанию создаст пустой вектор с размером 0

   vector(size_t size) : m_size{size}, capacity{size} {  // конструктор по размер с занулением
      array = new type[m_size];
      for (size_t i = 0; i < m_size; i++) {
         array[i] = type{};  // type{} - вызывает конструктор по умолчанию для типа type,
                             // в случае int вызовет это нулевой конструктор, т.е. будет 0, для float 0.f итп
      }
   }

   vector(vector& other) {             // конструктор копирования
      *this = other;                   // вызываем оператор присваивания, чтоб не писать одно и то же два раза
   }                                   // нужен для корректной работы строки vector<char> newVec = oldVec;
                                       // иначе придется бить на Две отдельных команды
   vector& operator=(const vector& other) {  // оператор присваивания
      if (this == &other) {            // если присваиваем сами себя, то лучше ничего не делать :)
         return *this;
      }
      m_size = other.m_size;
      capacity = other.capacity;

      delete[] array;                        // удаляем предыдущий массив, если там nullptr то обычно ничего не произойдет
      array = new type[m_size];              // выделяем новую память
      for (size_t i = 0; i < m_size; i++) {  // копируем поэлементно
         array[i] = other.array[i];
      }
      return *this;
   }

   void push_back(type& value) {                // добавление нового элемента в конец
      if (m_size == capacity) {                 // если мы достигли вместимости (например, в начале 0 == 0)
         type* oldArray = array;                // сохраняем указатель на старый массив
         capacity = 2 * (m_size + 1);           // добавляем вместимости
         array = new type[capacity];            // выделяем новую память
         for (size_t i = 0; i < m_size; i++) {  // поэлементно копируем из старого
            array[i] = oldArray[i];
         }
         delete[] oldArray;  // не забываем удалить старый массив
      }
      array[m_size] = value;
      m_size++;
   }

   type& operator[](size_t index) const {  // получение элемента вектора по индексу
      if (index >= m_size) {
         throw runtime_error("Плохой индекс вектора");
      }
      return array[index];
   }

   ~vector() {  // Деструктор вектора, по умолчанию не подойдет, потому что надо вручную удалить память
      delete[] array;
   }

   private:               // приватные поля класса вектор
   size_t m_size{0};      // текущий размер вектора
   size_t capacity{0};    // вместимость вектора
   protected:             // поля ниже доступны наследникам
   type* array{nullptr};  // указатель на массив данных типа type
};

class my_string {  // класс строки, использут вектор чаров внутри
   public:
   my_string() = default;

   my_string(const char* raw) {
      size_t len{};
      while (raw[len] != '\0') {
         len++;
      }
      data = vector<char>(len);

      for (size_t i = 0; i < data.size(); i++) {
         data[i] = raw[i];
      }
   }

   bool operator!=(const my_string& other) const {  // оператор неравенства,
      if (size() != other.size()) {                 // если размеры равны то правда
         return true;
      }
      for (size_t i = 0; i < size(); i++) {
         if (data[i] != other.data[i]) {
            return true;  // если нашли два разных символа, то тоже правда
         }
      }
      return false;
   }

   bool operator==(const my_string& other) const {  // оператор сравнения
      return !(this->operator!=(other));
   }

   friend ostream& operator<<(ostream& out, const my_string& str) {  // перегрузка оператора <<
      for (size_t i = 0; i < str.size(); i++) {                      // для вывода в любой поток
         out << str[i];
      }
      return out;
   }

   friend istream& operator>>(istream& in, my_string& str) {  // перегрузка оператора >>
      char temp;                                              // для ввода из любого потока
      while (in.read(&temp, 1)) {
         if (temp == '\n') {        // если в потоке встретели перенос строки
            if (str.size() == 0) {  // но еще ничего не считали, то пропускаем символ переноса
               continue;
            } else {
               break;  // иначе мы считали одну строку
            }
         }
         str.push_back(temp);
      }
      return in;
   }

   char& operator[](size_t index) const {  // оператор получения символа по индексу
      return data[index];
   }

   void push_back(char c) {  // оператор добавления символа
      data.push_back(c);
   }

   size_t size() const {  // метод получения размера
      return data.size();
   }

   private:
   vector<char> data;  // хранилище всех символов
};

class hash_map {      // класс мапы где ключ и значение это строка
   public:            // расширяется динамически при нехватке места, по умолчанию мест 6
   struct pair {      // это пара :)
      my_string key;  // хранит пару ключ - значение, таких типов которые дали
      my_string value;
   };

   my_string& operator[](const my_string& key) {  // оператора получения элемента мапы по ключу
      size_t index = getIndex(key);               // получаем индекс по хэшу и разрешению коллизий
      if (array[index].key.size() > 0) {          // если после поиска подходящего индекса
         if (array[index].key != key) {           // мы попали на непустой другой ключ
            cout << "Рехэширвание!" << endl;      // значит свободные места закончились и
            rehashing();                          // пришло время для рехэширования
            return this->operator[](key);         // после него опять вызываем доступ по тому же ключу
         }
      } else {
         array[index].key = key;  // если ключ пуст, то заполняем его , новый элемент добавили по сути
      }
      return array[index].value;  // возвращаем  my_string& что бы можно было написать unordered_mapa[key]=value;
   }                              // т.е. запись в value случится по адресу

   bool erase(const my_string& key) {  // удаление пары по ключу
      size_t index = getIndex(key);
      if (array[index].key == key) {
         array[index] = {};
      } else {
         return false;
      }
      return true;
   }

   bool find(const my_string& key) {  // поиск элемента по ключу
      size_t index = getIndex(key);
      if (array[index].key == key) {
         return true;
      }
      return false;
   }

   void print() {  // печать мапы на экран
      for (size_t i = 0; i < N; i++) {
         if (array[i].key.size() > 0) {
            cout << array[i].key << "\t:\t" << array[i].value << endl;
         }
      }
   }

   ~hash_map() {       // дестркуктор класса
      delete[] array;  // удаляем массив, поэтому [] после delete
   }

   private:
   size_t getIndex(const my_string& key) const {
      size_t index = hash(key), index0 = index, attempt{0};                            // вычисляем индекс используя хэш функцию
      while (attempt < N && array[index].key.size() > 0 && array[index].key != key) {  // если остались попытки
         attempt++;                                                                    // и текущий индекс уже занят каким то другим ключом
         index = (index0 + c * attempt + d * attempt * attempt) % N;                   // то ищем новый индекс с помощью квадратичной
      }
      return index;
   }
   size_t hash(const my_string& key) const {  // хэш функция строки
      size_t result{};
      for (size_t i = 0; i < key.size(); i++) {
         result = (127 * result + key[i]) % N;  // код каждого символа аккумулируем с предыдущим результатом
      }                                         // оставаясь в рамках индексов 0 .. N-1
      return result;
   }

   void rehashing() {                                                 // метод рехэширования
      pair* old_array = array;                                        // сохраняем указатель на старый массив
      size_t oldN = N;                                                // и его размер
      N = oldN * 2;                                                   // увеличиваем текущий размер в два раза
      array = new pair[N];                                            // и создаем соответсвующий массив в динамической памяти
      for (size_t i = 0; i < oldN; i++) {                             // проходимся по всем
         if (old_array[i].key.size() > 0) {                           // непустым элементам старой мапы
            this->operator[](old_array[i].key) = old_array[i].value;  // и заносим их в новую, вычисляя новое место
         }
      }
      delete[] old_array;  // не забываем удалить память из под старого массива
   }

   static const size_t c{3}, d{2};  // эмперически подобранные (т.е. проверили что мапа заполняется полностью без пустых мест)
                                    // коэффициенты для квадратичного разрешения коллизий
   size_t N{6};
   pair* array = new pair[N]{};  // статический массив пар Ключ-Значение, N+1 для последнего элемента, чей адрес
};                               // будет использоваться для end()

bool work(istream& input, hash_map& data) {  // функция работы с  мапой

   const my_string add("add"), del("del"), print("print"), find("find"), quit("quit");
   for (;;) {
      my_string command{};
      input >> command;
      cout << "cmd: " << command << endl;
      if (command == quit) {
         cout << "Конец команд." << endl;
         return true;
      } else if (command == add) {
         my_string key{};        // создаем ключ
         my_string value{};      // и значение
         input >> key >> value;  // считываем
         data[key] = value;      // и заносим в мапу
      } else if (command == del) {
         my_string key{};  // создаем ключ
         input >> key;
         if (data.erase(key)) {
            cout << "Успешно удалено" << endl;
         } else {
            cout << "Элемент не был найден" << endl;
         }
      } else if (command == find) {
         my_string key{};  // создаем ключ
         input >> key;
         if (data.find(key)) {
            cout << key << " : " << data[key] << endl;
         } else {
            cout << "Элемент не был найден" << endl;
         }
      } else if (command == print) {
         data.print();
      }
   }
}

int main(int argc, char** argv) {
   hash_map data{};

   if (argc == 2) {  // чтение из файла так как один аргумент предоставлен
      ifstream file(argv[1]);
      if (!file.is_open()) {
         cout << ", нужен новый файл." << endl;
         return 0;
      }
      if (!work(file, data)) {
         cout << ", нужен новый файл." << endl;
         return 0;
      }
   } else if (argc == 1) {  // ввод через консоль в случае отсутствия аргументов
      if (!work(cin, data)) {
         cout << ", начните сначала." << endl;
         return 0;
      }
   } else {
      cout << "Неправильные аргументы запуска!" << endl;
      return 0;
   }
   return 0;
}