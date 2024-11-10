#ifndef LINKEDELIST_HPP
#define LINKEDELIST_HPP

#include "includes.hpp"

template <class T>

// create a node of type T
class Node
{
public:
    T* data; // Just a pointer to packet or frame [don't have to worry about memory management]
    Node *next;
    Node(T* data) // will be provided with a pointer to the data
    {
        this->data = data;
        this->next = NULL;
    }
    ~Node()
    {
        // delete data;
    }
};

template <class T>
class LinkedList
{
public:
    Node<T> *head;
    Node<T> *tail;
    int size; // will be used for Producer-Consumer Approach
    LinkedList()
    {
        head = NULL;
        tail = NULL;
        size = 0;
    }

    // Declarations of functions -------------------
    void addNode(T* data); // at tail
    void addNodeAtTail(T* data);
    void addNodeAtHead(T* data);
    T* peekHead(); // don't remove node
    T* extractHead(); // also remove ndoe
    T* peekTail();
    T* extractTail();
    ~LinkedList(); // called using 'delete' keyword


    // Implementations of functions -------------------
    void addNode(T* data)
    {
        Node<T> *newNode = new Node<T>(data);
        if (head == NULL)
        {
            head = newNode;
            tail = newNode;
        }
        else
        {
            tail->next = newNode;
            tail = newNode;
        }
        size++;
    }

    void addNodeAtTail(T* data)
    {
        Node<T> *newNode = new Node<T>(data);
        if (head == NULL)
        {
            head = newNode;
            tail = newNode;
        }
        else
        {
            tail->next = newNode;
            tail = newNode;
        }
        size++;
    }

    void addNodeAtHead(T* data)
    {
        Node<T> *newNode = new Node<T>(data);
        if (head == NULL)
        {
            head = newNode;
            tail = newNode;
        }
        else
        {
            newNode->next = head;
            head = newNode;
        }
        size++;
    }

    T* peekHead()
    {
        if (head == NULL)
        {
            return NULL;
        }
        return head->data;
    }

    T* extractHead()
    {
        if (head == NULL)
        {
            return NULL;
        }
        Node<T> *temp = head;
        head = head->next;
        T* data = temp->data;
        delete temp; // we are not deleting the data, just the node
        size--;
        return data;
    }

    T* peekTail()
    {
        if (tail == NULL)
        {
            return NULL;
        }
        return tail->data;
    }

    T* extractTail()
    {
        if (tail == NULL)
        {
            return NULL;
        }
        Node<T> *temp = head;
        while (temp->next != tail)
        {
            temp = temp->next;
        }
        Node<T> *temp2 = tail;
        tail = temp;
        T* data = temp2->data;
        delete temp2;
        size--;
        return data;
    }

    ~LinkedList()
    {
        Node<T> *temp = head;
        while (temp != NULL)
        {
            Node<T> *temp2 = temp;
            temp = temp->next;
            delete temp2;
        }
    }



    
};

// ---TODO--- Try to make it Thread Safe







#endif // LINKEDELIST_HPP