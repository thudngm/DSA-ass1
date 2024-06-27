#include "kNN.hpp"

/* TODO: You can implement methods, functions that support your data structures here.
 * */

template <typename T>
Vector<T>::Vector()
{
    this->capacity = 5;
    this->size = 0;
    this->data = new T[capacity];
}

template <typename T>
Vector<T>::Vector(int capacity)
{
    this->capacity = capacity;
    this->size = 0;
    this->data = new T[capacity];
}

template <typename T>
Vector<T>::~Vector()
{
    clear();
}

template <typename T>
void Vector<T>::push_back(T value)
{
    this->ensureCapacity(this->size + 1);
    this->data[this->size] = value;
    this->size++;
}

template <typename T>
void Vector<T>::push_front(T value)
{
    for(int i = this->size; i > 0; i--){
        this->data[i] = this->data[i - 1];
    }
    this->data[0] = value;
    this->size++;
}

template <typename T>
void Vector<T>::insert(int index, T value)
{
    if(index < 0 || index > size){
        return;
    }

    this->ensureCapacity(this->size + 1);

    for (int i = size; i > index; i--){
        this->data[i] = this->data[i - 1];
    }
    this->data[index] = value;
    this->size++;
}

template <typename T>
void Vector<T>::remove(int index)
{
    if(index < 0 || index >= size){
        return;
    }

    for (int i = index; i < size - 1; ++i) {
        this->data[i] = this->data[i + 1];
    }
    this->size--;

}

template <typename T>
T &Vector<T>::get(int index) const
{
    if(index < 0 || index >= size){
        throw std::out_of_range("get(): Out of range");
    }
    
    return this->data[index];
}

template <typename T>
int Vector<T>::length() const
{
    return this->size;
}

template <typename T>
void Vector<T>::clear()
{
    delete[] this->data;
    size = 0;
    this->data = new T[this->capacity];
}

template <typename T>
void Vector<T>::print() const
{
    for(int i = 0; i < this->size; i++){
        cout << this->data[i];
        if(i < size - 1){
            cout << " ";
        }
    }
}

template <typename T>
void Vector<T>::reverse()
{
    int start = 0;
    int end = this->size - 1;
    while(start < end){
        T temp = this->data[start];
        this->data[start] = this->data[end];
        this->data[end] = temp;
        start++;
        end--;
    }
}

template <typename T>
int Vector<T>::indexOf(T value)
{
    for(int i = 0; i < size; i++){
        if(this->data[i] == value){
            return i;
        }
    }

    return -1;
}

template <typename T>
void Vector<T>::ensureCapacity(int capacity)
{
    if (capacity == this->capacity) {
        int newCapacity = this->capacity * 3 / 2;
        T *newData = new T[newCapacity];
            
        for (int i = 0; i < size; i++){
            newData[i] = this->data[i];
        }
        
        delete[] this->data;
        this->data = newData;
        this->capacity = newCapacity;
    }
}

template <typename T>
void Vector<T>::mergeSort()
{
    if(size <= 1){
        return;
    }

    int mid = size / 2;
    Vector<T> left(mid);
    Vector<T> right(size - mid);

    for(int i = 0; i < mid; i++){
        left.push_back(this->data[i]);
    }

    for(int i = mid; i < size; i++){
        right.push_back(this->data[i]);
    }

    left.size = mid;
    right.size = size - mid;
    left.mergeSort();
    right.mergeSort();

    merge(left, right);
}

template <typename T>
void Vector<T>::merge(Vector<T> &left, Vector<T> &right)
{
    int leftSize = left.size;
    int rightSize = right.size;
    int i = 0, j = 0, k = 0;

    while(i < leftSize && j < rightSize){
        if(left.data[i] <= right.data[j]){
            data[k++] = left.data[i++];
        } else {
            data[k++] = right.data[j++];
        }
    }

    while (i < leftSize){
        data[k++] = left.data[i++];
    }

    while(j < rightSize){
        data[k++] = right.data[j++];
    }
}

//--------------------------

Dataset::Dataset()
{
    this->nRows = 0;
    this->nCols = 0;
    this->data = new Vector<Vector<int>*>();
    this->header = new Vector<string>();
}

Dataset::~Dataset()
{
    delete this->header;
    delete this->data;
}

Dataset::Dataset(const Dataset &other)
{
    this->nRows = other.nRows;
    this->nCols = other.nCols;

    this->header = new Vector<string>();
    for(int i = 0; i < this->nCols; i++){
        this->header->push_back(other.header->get(i));
    }

    this->data = new Vector<Vector<int>*>();
    for(int i = 0; i < this->nRows; i++){
        Vector<int>* rowData = new Vector<int>(this->nCols);
        for(int j = 0; j < this->nCols; j++){
            rowData->push_back(other.data->get(i)->get(j));
        }
        this->data->push_back(rowData);
    }
}

Dataset &Dataset::operator=(const Dataset &other)
{
    this->nRows = other.nRows;
    this->nCols = other.nCols;

    this->header = new Vector<string>();
    for(int i = 0; i < this->nCols; i++){
        this->header->push_back(other.header->get(i));
    }

    this->data = new Vector<Vector<int>*>();
    for(int i = 0; i < this->nRows; i++){
        Vector<int>* rowData = new Vector<int>(this->nCols);
        for(int j = 0; j < this->nCols; j++){
            rowData->push_back(other.data->get(i)->get(j));
        }
        this->data->push_back(rowData);
    }
    return *this;
}

bool Dataset::loadFromCSV(const char *fileName)
{   
    string line, word;

    //check fileName status
    fstream file(fileName, ios::in); // read and write on file ?
    if(file.is_open()){
        getline(file, line);
        stringstream ss_header(line); //read and write on file ?
        while(getline(ss_header, word, ',')){
            this->header->push_back(word);
            this->nCols++;
        }

        while(getline(file, line)){
            Vector<int>* rowData = new Vector<int>(this->nCols);
            stringstream ss(line);

            while(getline(ss, word, ',')){
                rowData->push_back(stoi(word));
            }
            this->data->push_back(rowData);
            this->nRows++;
        }

        return true;
    }
    else {
        return false;
    }
}

void Dataset::printHead(int nRows, int nCols) const
{
    if(nRows < 0 || nCols < 0){
        return;
    }
    else{
        int nRowsToPrint = min(nRows, this->nRows);
        int nColsToPrint = min(nCols, this->nCols);

        for (int i = -1; i < nRowsToPrint; i++) {
            for (int j = 0; j < nColsToPrint; j++) {
                if (i == -1) {
                    cout << this->header->get(j);
                    if(j < nColsToPrint - 1){
                        cout << " ";
                    }
                }
                else {
                    cout << this->data->get(i)->get(j);
                    if(j < nColsToPrint - 1){
                        cout << " ";
                    }
                }
            }
            if(i < nRowsToPrint - 1){
                cout << endl;
            }
        }
    }
}

void Dataset::printTail(int nRows, int nCols) const
{
    if(nRows < 0 || nCols < 0){
        return;
    }

    int startRow = this->nRows - nRows;
    int startCol = this->nCols - nCols;

    
    if(startRow <= 0){
        for (int i = -1; i < this->nRows; i++) {
            for (int j = startCol; j < this->nCols; j++) {
                if (i == - 1) {
                    cout << this->header->get(j);
                    if(j < this->nCols - 1){
                        cout << " ";
                    }
                }
                else {
                    cout << this->data->get(i)->get(j);
                    if(j < this->nCols - 1){
                        cout << " ";
                    }
                }
            }
            if( i < this->nRows - 1){
                cout << endl;
            }
        }
    }
    else if(startCol <= 0){
        for (int i = 0; i < this->nCols; i++){
            cout << this->header->get(i);
            if(i < this->nCols - 1){
                cout << " ";
            }
        }
        for (int i = startRow; i < this->nRows; i++) {
            for (int j = 0; j < this->nCols; j++) {
                cout << this->data->get(i)->get(j);
                if(j < this->nCols - 1){
                    cout << " ";
                }
            }
            if( i < this->nRows - 1){
                cout << endl;
            }
        }
    }
    else if(startRow <= 0 && startCol <= 0){
        for (int i = -1; i < this->nRows; i++) {
            for (int j = 0; j < this->nCols; j++) {
                if (i == -1) {
                    cout << this->header->get(j);
                    if(j < this->nCols - 1){
                        cout << " ";
                    }
                }
                else {
                    cout << this->data->get(i)->get(j);
                    if(j < this->nCols - 1){
                        cout << " ";
                    }
                }
            }
            if( i < this->nRows - 1){
                cout << endl;
            }
        }
    }
    else{
        for (int i = startRow - 1; i < this->nRows; i++) {
            for (int j = startCol; j < this->nCols; j++) {
                if (i == startRow - 1) {
                    cout << this->header->get(j);
                    if(j < this->nCols - 1){
                        cout << " ";
                    }
                }
                else {
                    cout << this->data->get(i)->get(j);
                    if(j < this->nCols - 1){
                        cout << " ";
                    }
                }
            }
            if( i < this->nRows - 1){
                cout << endl;
            }
        }
    }
}

void Dataset::getShape(int &nRows, int &nCols) const
{
    nRows = this->nRows;
    nCols = this->nCols;
}

int Dataset::getRows() const
{
    return this->nRows;
}

int Dataset::getCols() const
{
    return this->nCols;
}

void Dataset::columns() const
{
    this->header->print();
}

void Dataset::setValues(int nRows, int nCols, Vector<string> *header, Vector<Vector<int> *> *data)
{
    this->nRows = nRows;
    this->nCols = nCols;
    this->header = header;
    this->data = data;
}

bool Dataset::drop(int axis, int index, std::string columns)
{
    if(axis == 0){
        if(index < 0 || index >= this->nRows){
            return false;
        }
        this->data->remove(index);
        this->nRows--; //check 
        return true;
    }
    else if(axis == 1){
        int colIndex = this->header->indexOf(columns);

        if(colIndex == -1){
            return false;
        } 
        else {
            this->header->remove(colIndex);
            for(int i = 0; i < this->nRows; i++){
                this->data->get(i)->remove(colIndex);
            }
            this->nCols--; // check
            return true;
        }
    }
    else {
        return false;
    }
}

Dataset Dataset::extract(int startRow, int endRow, int startCol, int endCol) const
{
    if(endRow == -1){
        endRow = this->nRows - 1;
    }
    if(endCol == -1){
        endCol = this->nCols - 1;
    }

    Dataset newDataset;
    newDataset.nRows = endRow - startRow + 1;
    newDataset.nCols = endCol - startCol + 1;

    //Extract header
    for(int j = startCol; j <= endCol; j++){
        newDataset.header->push_back(this->header->get(j));
    }

    //Extract data
    for(int i = startRow; i <= endRow; i++){
        Vector<int>* newRow = new Vector<int>();
        for(int j = startCol; j <= endCol; j++){
            newRow->push_back(this->data->get(i)->get(j));
        } 
        newDataset.data->push_back(newRow);
    }
    return newDataset;
}

Vector<Vector<int> *> *Dataset::getData() const
{
    return this->data;
}

Vector<string> *Dataset::getHeader() const
{
    return this->header;
}

void Dataset::print() const
{
    cout << this->nRows << " " << this->nCols << endl;
    cout << "Header" << endl;
    for (int j = 0; j < this->nCols; j++) {
        cout << this->header->get(j);
        if(j < this->nCols -1){
            cout << " ";
        }
    }

    cout << endl << "Data" << endl;
    for (int i = 0; i < this->nRows; i++) {
        for (int j = 0; j < this->nCols; j++) {
            cout << this->data->get(i)->get(j);
            if(j < this->nCols -1){
                cout << " ";
            }
        }
        cout << endl;
    }
}

int max_repeat(Vector<int>* data){

    int arr_repeat[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    for(int i = 0; i < data->length(); i++){
        arr_repeat[data->get(i)]++;
    }

    int result = -1;
    int count = -1;
    for(int i = 0; i < 10; i++){
        if(arr_repeat[i] > count){
            result = i;
            count = arr_repeat[i];
        }
    }

    return result;
}

//---------------------------
kNN::kNN(int k)
{
    this->k = k;
}

void kNN::fit(const Dataset &X_train, const Dataset &y_train)
{
    this->X_train = X_train;
    this->y_train = y_train;
}

Dataset kNN::predict(const Dataset &X_test)
{
    int nRows_X_test = X_test.getRows();
    Vector<Vector<int>*>* X_test_data = X_test.getData();

    int nRows_X_train = this->X_train.getRows();
    Vector<Vector<int>*>* X_train_data = this->X_train.getData();
    Vector<Vector<int>*>* y_train_data = y_train.getData();

    int nCols = X_test.getCols();

    //Dataset result
    Vector<string>* header = new Vector<string>(1);
    header->push_back("label");

    Vector<Vector<int>*>* data = new Vector<Vector<int>*>();
    for(int idx = 0; idx < nRows_X_test; idx++){
        Vector<double> distances;
        for(int i = 0; i < nRows_X_train; i++){
            double distance = 0;
            for(int j = 0; j < nCols; j++){
                distance += pow((X_test_data->get(idx)->get(j)) - X_train_data->get(i)->get(j), 2);
            }
            distances.push_back(sqrt(distance));
        }

        //Sort distances
        Vector<double> distancesAfterSort;
        for(int i = 0; i < nRows_X_train; i++){
            distancesAfterSort.push_back(distances.get(i));
        }
        distancesAfterSort.mergeSort();

        //Get k smallest distances from train data
        Vector<int>* knnNeighbors = new Vector<int>();
        for(int k_idx = 0; k_idx < this->k; k_idx++){
            int minIndex = distances.indexOf(distancesAfterSort.get(k_idx));
            knnNeighbors->push_back(y_train_data->get(minIndex)->get(0));
        }

        //Get max value in KNN to get prediction
        int predictValue = max_repeat(knnNeighbors);

        //Append to returned dataset
        Vector<int>* newRow = new Vector<int>(1);
        newRow->push_back(predictValue);
        data->push_back(newRow);
    }

    y_pred.setValues(nRows_X_test, 1, header, data);
    return y_pred;
}

double kNN::score(const Dataset &y_test, const Dataset &y_pred)
{
    Vector<Vector<int>*>* y_test_data = y_test.getData();
    Vector<Vector<int>*>* y_pred_data = y_pred.getData();
    int nRow = y_test.getRows();
    int correctPredict = 0;

    for(int i = 0; i < nRow; i++){
        if(y_test_data->get(i)->get(0) == y_pred_data->get(i)->get(0)){
            correctPredict++;
        }
    }

    double score = (double)correctPredict / (double)nRow;
    return score;
}

void train_test_split(Dataset &X, Dataset &y, double test_size, Dataset &X_train, Dataset &X_test, Dataset &y_train, Dataset &y_test)
{
    int nRows = X.getRows();
    int nRows_train = (int)(nRows * (1 - test_size));
    int nRows_test = (int)(nRows - nRows_train);

    X_test = X.extract(nRows_train, -1, 0, -1);
    X_train = X.extract(0, nRows_train - 1, 0, -1);
    y_test = y.extract(nRows_train, -1, 0, -1);
    y_train = y.extract(0, nRows_train -1, 0, -1);
}