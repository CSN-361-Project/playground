#ifndef FRAME_HPP
#define FRAME_HPP


// Frame Types
const frameType PADDING = 0x00;
const frameType PING = 0x01;
const frameType ACK = 0x02;
const frameType ACK_ECN = 0x03;

const frameType CRYPTO = 0x06;

class frame{
    public:
        char *data; // pointer to data buffer
        int size; // size of data
        frameType type; // type of frame
};


#endif // FRAME_HPP