#include <Arduino.h>

class Queue {
public:
    Queue(int max_size, int max_len);
    ~Queue();

    void enqueue(char *str);
    char * dequeue();
    bool isEmpty() const;
    bool isFull() const;
    void traverse() const;

private:
    char** array;
    int front;
    int rear;
    int max_size;
    int max_len;
};


