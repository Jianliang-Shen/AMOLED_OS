#include "../include/queue.h"

Queue::Queue(int max_size, int max_len) {
    if (max_size <= 0) {
        printf("Queue size must be greater than zero.\n");
    }

    this->max_size = max_size;
    this->max_len = max_len;
    this->array = new char*[max_size];

    for (int i = 0; i < max_size; ++i) {
            array[i] = new char[max_len + 1];  // +1 for null terminator
            memset(array[i], ' ', max_len);
            array[i][max_len] = '\0';  // Null terminate the string
    }
    this->front = -1;
    this->rear = -1;

    printf("Queue is inited.\n");
}

Queue::~Queue() {
    delete[] array;
}

void Queue::enqueue(char* str) {
    if (isFull()) {
        printf("Queue is full. Cannot enqueue.\n");
        return;
    }

    if (isEmpty()) {
        front = rear = 0;
    } else {
        rear = (rear + 1) % max_size;
    }

    array[rear] = str;
    printf("enqueue successfully, array[%d] = %s\n", rear, array[rear]);
}

char *Queue::dequeue() {
    if (isEmpty()) {
        printf("Queue is empty. Cannot dequeue.\n");
        return NULL; // You may choose a different way to handle this situation
    }

    char *str = array[front];

    if (front == rear) {
        // Last element in the queue
        front = rear = -1;
    } else {
        front = (front + 1) % max_size;
    }

    return str;
}

bool Queue::isEmpty() const {
    return front == -1 && rear == -1;
}

bool Queue::isFull() const {
    return (rear + 1) % max_size == front;
}

void Queue::traverse() const {
    if (isEmpty()) {
        printf("Queue is empty.\n");
        return;
    }

    printf("front is %d, rear is %d\n", front, rear);
    int current = front;
    while (current != rear) {
        printf("front is %d, rear is %d, current is %d\n", front, rear, current);

        printf("%s\n", array[current]);
        current = (current + 1) % max_size;
    }

    // Print the last element
    printf("%s\n", array[rear]);
}