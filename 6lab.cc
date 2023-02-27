#include <fstream>
#include <iostream>

using namespace std;

template <typename type>
class vector {  // Шаблонный класс вектора
   public:
   int size() {  // получение размера вектора
      return m_size;
   }

   vector() = default;  // конструктор по умолчанию создаст пустой вектор с размером 0

   vector(int size) : m_size{size}, capacity{size} {  // конструктор по размер с занулением
      array = new type[m_size];
      for (int i = 0; i < m_size; i++) {
         array[i] = type{};  // type{} - вызывает конструктор по умолчанию для типа type,
                             // в случае int вызовет это нулевой конструктор, т.е. будет 0, для float 0.f итп
      }
   }

   vector(vector& other) {             // конструктор копирования
      *this = other;                   // вызываем оператор присваивания, чтоб не писать одно и то же два раза
   }                                   // нужен для корректной работы строки vector<char> newVec = oldVec;
                                       // иначе придется бить на Две отдельных команды
   vector& operator=(vector& other) {  // оператор присваивания
      if (this == &other) {            // если присваиваем сами себя, то лучше ничего не делать :)
         return *this;
      }
      m_size = other.m_size;
      capacity = other.capacity;

      delete[] array;                        // удаляем предыдущий массив, если там nullptr то обычно ничего не произойдет
      array = new type[m_size];              // выделяем новую память
      for (int i = 0; i < m_size; i++) {  // копируем поэлементно
         array[i] = other.array[i];
      }
      return *this;
   }

   void push_back(type& value) {                // добавление нового элемента в конец
      if (m_size == capacity) {                 // если мы достигли вместимости (например, в начале 0 == 0)
         type* oldArray = array;                // сохраняем указатель на старый массив
         capacity = 2 * (m_size + 1);           // добавляем вместимости
         array = new type[capacity];            // выделяем новую память
         for (int i = 0; i < m_size; i++) {  // поэлементно копируем из старого
            array[i] = oldArray[i];
         }
         delete[] oldArray;  // не забываем удалить старый массив
      }
      array[m_size] = value;
      m_size++;
   }

   type& operator[](int index) {  // получение элемента вектора по индексу
      if (index >= m_size) {
         throw runtime_error("Плохой индекс вектора");
      }
      return array[index];
   }

   ~vector() {  // Деструктор вектора, по умолчанию не подойдет, потому что надо вручную удалить память
      delete[] array;
   }

   private:               // приватные поля класса вектор
   int m_size{0};      // текущий размер вектора
   int capacity{0};    // вместимость вектора
   protected:             // поля ниже доступны наследникам
   type* array{nullptr};  // указатель на массив данных типа type
};

class my_string : public vector<char> {                        // класс строки, отнаследован от вектора, и специализирует его
   public:                                                     // шаблонный параметр char -ом, то есть строка это вектор чаров :)
                                                               // и помимо всех методом вектора, строка так же будет иметь публичные методы ниже
   friend ostream& operator<<(ostream& out, my_string& str) {  // перегрузка оператора <<
      for (int i = 0; i < str.size(); i++) {                // для вывода в любой поток
         out << str[i];                                        // выводим посимвольно
      }
      return out;
   }

   friend istream& operator>>(istream& in, my_string& str) {  // перегрузка оператора >>
      char temp;                                              // для ввода из любого потока
      while (in.read(&temp, 1)) {
         if (temp == '\n' || temp == ' ' || temp == '\t') {  // если в потоке встретели перенос строки, пробел или таб
            if (str.size() == 0) {                           // но еще ничего не считали, то пропускаем символ переноса
               continue;
            } else {
               break;  // иначе мы считали одну строку, символ в строку мы не добавляем при этом, но и из потока его забрали
            }
         }
         str.push_back(temp);
      }
      return in;
   }

   bool operator<(my_string& other) {  // оператор меньше для строк, т.е. лексикографическое сравнения
      if (size() < other.size()) {     // сначала сравниваем по размерам
         return true;
      }
      if (size() > other.size()) {
         return false;
      }
      int i = 0;                                       // если таки размеры равны
      while (i < size() && array[i] == other.array[i]) {  // пропускаем одинаковые символы в начале
         i++;
      }
      if (i >= size()) {  // если дошли до конца, то строки на самом деле равны
         return false;
      }
      return array[i] < other.array[i];  // иначе, у нас есть пара символов по которым мы можем рассудить кто меньше
   }
};

template <typename DataType>
bool read_numbers(istream& input, vector<DataType>& numbers) {  // функция считывания "чисел" в вектор
   int count{};                                              // спрашиваем сколько будем вводить
   input >> count;
   if (!input) {
      cout << "Количество чисел не число";
      return false;
   }
   if (count % 2 != 0) {  // если количество нечетно то ошибка
      cout << "Количество чисел не чётно";
      return false;
   }
   for (int i = 0; i < count; i++) {
      DataType value{};  // создаем временное значение, по сути строку
      input >> value;    // считываем ее
      if (!input) {
         cout << "Чтение закончилось неудачей";
         return false;
      }
      numbers.push_back(value);
   }
   return true;
}

template <typename DataType>
void swapping(DataType& left, DataType& right) {  // вспомогательная функция
   DataType temp = left;                          // обмена значений двух переменных
   left = right;
   right = temp;
}

template <typename DataType, template <typename> class Container>      // такой длинный шаблон нужен для точного указания типа контейнера
void quickSort(Container<DataType>& array, size_t low, size_t high) {  // иначе нужно было бы указать что работать сортировка будет с каким то конкретным, а так можно отсортровать как вектор строк, так и вектор чисел
   if (low >= high) {                                                  // вызов с невалидным диапозоном, ничего не делаем
      return;
   }
   int i = low;
   int j = high;

   if (j - i == 1) {                    // если надо проверить соседние
      if (array[j] < array[i]) {        // то проверяем их
         swapping(array[i], array[j]);  // и меняем местами если надо
      }
      return;
   }
   int pivotIndex = (i + j) / 2;     // берем индекс опорного элемента
   DataType pivot = array[pivotIndex];  // и само значение элемента

   while (i < j) {                                 // начинаем работать с двух концов двигаясь к центру
      while (array[i] < pivot && i <= pivotIndex)  // если левый элемент должен остаться слева от
         i++;                                      // опорного, то просто сжвигаемся
      while (pivot < array[j] && j >= pivotIndex)  // также делаем и для правого, он уже он больше опорного
         j--;
      if (i < j) {                      // тут либо мы нашли два элемента которые нужно поменять местами
         swapping(array[i], array[j]);  // или уже дошли до опорного элемента и i == j
         i++;
         j--;
      }
   }
   if (j > low)                  // если слева мы не проверили все элементы, то
      quickSort(array, low, j);  // сортируем этот кусок
   if (i < high)                 // также проверяем для правого
      quickSort(array, i, high);
}

template <typename DataType, template <typename> class Container>  // вспомогательная шаблонная функция вложенным шаблонным параметром template <typename> class Container
void quickSort(Container<DataType>& array) {                       // вызова быстрой сортировки для контейнера
   quickSort(array, 0, array.size() - 1);                          // сортирует по умолсанию весь контейнет
}

template <typename type>
bool to_integer(my_string& str, type& integer) {  // вспомогательная функция перевода
   integer = type{};                              // строки в число, если вернем неправду, то перевод не удался
   type prev = integer;
   bool positive = true;
   for (int i = 0; i < str.size(); i++) {
      if (str[i] == '-') {
         positive = false;
         continue;
      } else if (str[i] > '9' || str[i] < '0') {
         return false;
      }
      integer *= 10;
      integer += str[i] - '0';

      if (integer < prev) {
         return false;
      }
      prev = integer;
   }
   if (!positive) {  // не забываем про знак
      integer *= -1;
   }
   return true;
}

template <typename type>
void print_multiplication(type& left, type& right) {  // вспомогательная функция печати пар
   static type checker = left * right;                // ны эту строку мы попадем только при первом вызове функции печати пары
   cout << left << "\t*\t" << right << "\t=\t" << left * right << endl;
   if (checker != left * right) {                     // если мы обнаружили различие между сохраненным произведением и текущим
      cout << "Не все числа образуют пары!" << endl;  // то аварийно завершаемся с ошибкой
      exit(0);
   }
}

int main(int argc, char** argv) {
   vector<my_string> numbers;  // создаем вектор строк

   if (argc == 2) {  // чтение из файла так как один аргумент предоставлен
      ifstream file(argv[1]);
      if (!file.is_open()) {
         cout << ", нужен новый файл." << endl;
         return 0;
      }
      if (!read_numbers(file, numbers)) {
         cout << ", нужен новый файл." << endl;
         return 0;
      }
   } else if (argc == 1) {  // ввод через консоль в случае отсутствия аргументов
      if (!read_numbers(cin, numbers)) {
         cout << ", начни сначала." << endl;
         return 0;
      }
   } else {
      cout << "неправильные аргументы при запуске." << endl;
      return 0;
   }

   cout << "Введенные числа:" << endl;

   for (int i = 0; i < numbers.size(); i++) {
      cout << numbers[i] << endl;
   }

   quickSort(numbers);  // лексикографическая (потому что оператор < для строк следует логике
                        // словаря) быстрая (через опораный элемент и рекурсию) сортировка
   cout << "после лексикографической сортировки:" << endl;
   for (int i = 0; i < numbers.size(); i++) {
      cout << numbers[i] << endl;
   }
   vector<int> actualNumbers;  //  создаем вектор чисел
   for (int i = 0; i < numbers.size(); i++) {
      int temp;
      if (!to_integer(numbers[i], temp)) {          // переводим каждую строку в число
         cout << "Было введено НЕ число!" << endl;  // если ошибка то прекращаем работать
         return 0;
      }
      actualNumbers.push_back(temp);
   }

   quickSort(actualNumbers);  // сортируем быстро массив чисел
   cout << "Отсортированный массив настоящих чисел:" << endl;
   for (int i = 0; i < actualNumbers.size(); i++) {
      cout << actualNumbers[i] << endl;
   }

   cout << "Разбивка по парам:" << endl;

   int maxPositive, minPositive, maxNegative, minNegative;           // найдем минмаксы в массие
   maxPositive = minPositive = actualNumbers[actualNumbers.size() - 1];  // сначала с положительными числами (в конце должно быть)
                                                                         // самое большое положительное число
   int minPositiveIndex = actualNumbers.size(), index{};              // и сохраним индекс минимального положительного числа
   for (int i = 0; i < actualNumbers.size(); i++) {                   // для учета количество положительных и отрицительных чисел
      if (actualNumbers[i] > 0 && actualNumbers[i] > maxPositive) {
         maxPositive = actualNumbers[i];
      }
      if (actualNumbers[i] > 0 && actualNumbers[i] < minPositive) {
         minPositive = actualNumbers[i];
         minPositiveIndex = index;
      }
      index++;
   }

   if (maxPositive < 0 && minPositive < 0) {  // если все положительне числа оказались отрицательными, значит
      int size = actualNumbers.size();     // массив весь отрицательный и нужно вывести произведения крайних пар
      for (int i = 0; i < size / 2; i++) {
         print_multiplication(actualNumbers[i], actualNumbers[size - 1 - i]);
      }
      return 0;
   }

   maxNegative = minNegative = actualNumbers[0];  // ищем отрицательные максимум и минимум
   for (size_t i = 0; i < actualNumbers.size(); i++) {
      if (actualNumbers[i] < 0 && actualNumbers[i] > maxNegative) {
         maxNegative = actualNumbers[i];
      }
      if (actualNumbers[i] < 0 && actualNumbers[i] < minNegative) {
         minNegative = actualNumbers[i];
      }
   }
   if (maxNegative > 0 && minNegative > 0) {  // если все положительные,
      int size = actualNumbers.size();     // то выводим произведения крайних пар
      for (int i = 0; i < size / 2; i++) {
         print_multiplication(actualNumbers[i], actualNumbers[size - 1 - i]);
      }
      return 0;
   }

   if (minPositiveIndex != actualNumbers.size() / 2) {     // если количество положительных != отрицательным
      int size = actualNumbers.size();                  // то нужно вывести произведения симметричных элементов
      for (int i = 0; i < minPositiveIndex / 2; i++) {  // среди отрицательных и положительных отдельно
         print_multiplication(actualNumbers[i], actualNumbers[minPositiveIndex - 1 - i]);
      }
      for (int i = 0; i < (size - minPositiveIndex) / 2; i++) {
         print_multiplication(actualNumbers[i + minPositiveIndex], actualNumbers[size - 1 - i]);
      }
   } else if (minNegative * maxNegative == maxPositive * minPositive) {        // если прозведения крайних элементов двух половин
      int size = actualNumbers.size() / 2;                                  // равны, то нужно вывести произведения крайних пар
      for (int i = 0; i < size / 2; i++) {                                  // этих половин -4 -2 1 8
         print_multiplication(actualNumbers[i], actualNumbers[size - 1 - i]);  //
      }

      for (int i = 0; i < size / 2; i++) {
         print_multiplication(actualNumbers[size + i], actualNumbers[2 * size - 1 - i]);
      }
   } else {                                    // иначе остается случай когда нужно вывести произведения
      int size = actualNumbers.size();      // элементов с расстоянием между ними в половину массива
      for (int i = 0; i < size / 2; i++) {  // -2 -1 4 8
         print_multiplication(actualNumbers[i], actualNumbers[size / 2 + i]);
      }
   }

   return 0;
}
