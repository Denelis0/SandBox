#include <iostream>
#include <fstream>
#include <string>

using namespace std;

typedef struct sparseMatrix
{
    unsigned int rows, columns,maxSize;
    unsigned int* rowIndexes;
    unsigned int* columnIndexes;
    int* values;
} sparseMatrix;

void matrix_allocation(sparseMatrix* matrix,unsigned int rows,unsigned int columns,unsigned int maxSize ) {

    matrix->rows = rows;
    matrix->columns = columns;
    matrix->maxSize = maxSize;

    matrix->rowIndexes = (unsigned int*)calloc((rows+1), sizeof(unsigned int));
    matrix->columnIndexes = (unsigned int*)calloc(maxSize, sizeof(unsigned int));
    matrix->values = (int*)calloc(maxSize, sizeof(int));

}

void matrix_free(sparseMatrix* matrix){
    free(matrix->rowIndexes);
    free(matrix->columnIndexes);
    free(matrix->values);
}

int set_element(sparseMatrix* matrix, unsigned row, unsigned column, int value){

    unsigned i,j;

    if((row > matrix->rows) || (column > matrix->columns))
    return 1;

    for(i = matrix->columnIndexes[row]; i < matrix->rowIndexes[row + 1]; i++)
        if(matrix->columnIndexes[i] == column)
            {
            if(value)
            {
                matrix->values[i] = value;
                return 0;
            }
            for(j = i; j < (matrix->rowIndexes[matrix->rows] - 1); j++)
            {
                matrix->values[j] = matrix->values[j + 1];
                matrix->columnIndexes[j] = matrix->columnIndexes[j + 1];
            }
            for(j = row + 1; j < (matrix->rows + 1);j++)
                
                matrix->rowIndexes[j]--;
                
            
            return 0;
        }
    

        if(!value)
        return 0;

        if(matrix->rowIndexes[matrix->rows] == matrix->maxSize)
        return 2;

        for(j = matrix->rowIndexes[matrix->rows];j > i; j--){

            matrix->values[j] = matrix->values[j - 1];
            matrix->columnIndexes[j] = matrix->columnIndexes[j - 1];
        }

        matrix->values[i] = value;
        matrix->columnIndexes[j] = column;

        for(j = row + 1; j < (matrix->rows + 1);j++){
            matrix->rowIndexes[j]++;
        }

    return 0;
}


void print_sparseMatrix(sparseMatrix* matrix){

    unsigned i;
    cout<<"Row indexes array:"<<endl;
    for(i = 0; i < (matrix->rows + 1); i++)
    {
        cout<<matrix->rowIndexes[i]<<" ";
    
    }

    cout<<endl<<"Column indexes array:"<<endl;

    for(i = 0; i < matrix->rowIndexes[matrix->rows];i++)
    {
        cout<<matrix->columnIndexes[i]<<" ";
    }

    cout<<endl<<"values array:"<<endl;

    for(i = 0; i < matrix->rowIndexes[matrix->rows];i++)
    {
        cout<<matrix->values[i]<<" ";
    }

    cout<<endl<<endl;
}

void swap(sparseMatrix* matrix, int counter)
{
for(int i = 0; i <= matrix->maxSize; i++){ 

if(matrix->values[i] > counter){ //если значение с начала массива Rows больше заданного значения

int num = 0;

  for(int j = num; j < i;j++) {

 if(matrix->values[(matrix->maxSize) - j] < counter);         //проверяем значение с конца массива Rows,если меньше, то
                                                              //цикл прерывается и происходит обмен значениями.
                                                              //если больше,то ищем следующее значение по индексу меньшему предыдущего;
if(matrix->values[i] < matrix->values[(matrix->maxSize) - j]);

int temp;                                                 //обмен значениями;
temp = matrix->values[i];
matrix->values[i] = matrix->values[(matrix->maxSize) - j];
matrix->values[(matrix->maxSize) - j] = temp;
num = j;                                                   //запоминает индекс для пропуска проверенных элементов с конца массива Rows;
break;                                                     //выход из цикла преждевременно;
}
}
}
}

int main() {

sparseMatrix array;
unsigned int a,b,c;

cout<<"Please,write the number of rows,columns and number of non-zero elements"<<endl;
cin >>a>>b>>c;

if(a*b < c)
{
    cout<<"Error"<<endl;
}

matrix_allocation(&array,a,b,c);

string path = "2lab.data"; 
ifstream fin;
fin.open(path); //проверяем файл;
if(!fin.is_open())
{
cout<<"File error - cannot open to read data"<<endl;
}
else 
{
char buff[3];
int a,b,c;

while(fin.get(buff,sizeof(buff))) //вытаскиваем по три элемента
{

fin>>a>>b>>c;
set_element(&array,a,b,c);
cout<<"("<<a<<","<<b<<","<<c<<")"<<'\t';

}
cout<<endl;
}
print_sparseMatrix(&array);

int counter;
cout<<"Write the number:";
cin>>counter;

swap(&array,counter);
print_sparseMatrix(&array);
cout<<"----";
matrix_free(&array);
return 0;
}