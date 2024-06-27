#include "main.hpp"

/* TODO: Please design your data structure carefully so that you can work with the given dataset
 *       in this assignment. The below structures are just some suggestions.
 */

template <typename T>
class List
{
public:
    virtual ~List() = default;
    virtual void push_back(T value) = 0;
    virtual void push_front(T value) = 0;
    virtual void insert(int index, T value) = 0;
    virtual void remove(int index) = 0;
    virtual T &get(int index) const = 0;
    virtual int length() const = 0;
    virtual void clear() = 0;
    virtual void print() const = 0;
    virtual void reverse() = 0;
    virtual void ensureCapacity(int capacity) = 0;
};

template <typename T>
class Vector : public List<T>
{
protected:
    T *data; // dynamic array to store the list's items
    int size;
    int capacity;

public:
    Vector();
    Vector(int capacity);
    ~Vector();
    void push_back(T value);
    void push_front(T value);
    void insert(int index, T value);
    void remove(int index);
    T &get(int index) const;
    int length() const;
    void clear();
    void print() const;
    void reverse();
    int indexOf(T value);
    void ensureCapacity(int capacity);
    void mergeSort();
    void merge(Vector<T> &left, Vector<T> &right);
};

class Dataset
{
private:
    Vector<Vector<int> *> *data;
    Vector<string> *header;
    int nRows;
    int nCols;

public:
    Dataset();
    ~Dataset();
    Dataset(const Dataset &other);
    Dataset &operator=(const Dataset &other);
    bool loadFromCSV(const char *fileName);
    void printHead(int nRows = 5, int nCols = 5) const;
    void printTail(int nRows = 5, int nCols = 5) const;
    void getShape(int &nRows, int &nCols) const;
    int getRows() const;
    int getCols() const;
    void columns() const;
    void setValues(int nRows, int nCols, Vector<string> *header, Vector<Vector<int> *> *data);
    bool drop(int axis = 0, int index = 0, std::string columns = "");
    Dataset extract(int startRow = 0, int endRow = -1, int startCol = 0, int endCol = -1) const;
    Vector<Vector<int> *> *getData() const;
    Vector<string> *getHeader() const;
    void print() const;
};

class kNN
{
private:
    int k;
    Dataset X_train;
    Dataset y_train;
    Dataset y_pred;

public:
    kNN(int k = 5);
    void fit(const Dataset &X_train, const Dataset &y_train);
    Dataset predict(const Dataset &X_test);
    double score(const Dataset &y_test, const Dataset &y_pred);
};

void train_test_split(Dataset &X, Dataset &y, double test_size,
                      Dataset &X_train, Dataset &X_test, Dataset &y_train, Dataset &y_test);

// Please add more or modify as needed