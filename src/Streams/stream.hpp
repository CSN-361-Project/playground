

#include "../includes.hpp"

class Stream
{
    char *data;
    int length;

    Stream()
    {
        this->data = nullptr;
        this->length = 0;
    }

    void send(char *data, int lenght)
    {
        char *temp = new char[this->length + length];
        for (int i = 0; i < this->length; i++)
        {
            temp[i] = this->data[i];
        }
        for (int i = 0; i < length; i++)
        {
            temp[this->length + i] = data[i];
        }
        delete[] this->data;
        this->data = temp;
        this->length += length;
    }

    int receive(char *input)
    {
        input = (char *)realloc(input, this->length);
        for (int i = 0; i < this->length; i++)
        {
            input[i] = this->data[i];
        }
        return this->length; // return the length of the data
    }

    void closeStream()
    {
        delete[] this->data;
        this->data = nullptr;
        this->length = 0;
    }
};
