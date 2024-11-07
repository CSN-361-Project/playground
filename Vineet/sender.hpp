#ifndef SENDER_HPP
#define SENDER_HPP

// A rough Idea of how the sender will work

/*
It will start as soon as server is ready

It will be triggered by 
~ a semaphore or a variable of server 
which indicates if any packet is ready to be sent from  

- any connection OR
    [ We can create a packet sending queues for connections, 
        each connection put's it's name in the box and the scheduler picks 
        in a loop picks one or two packets of that connectin and sends it
        if no more packets for that connection it's removed from loop else left for next iternation
        -----Look in Router Scheduling Algorithms for more ideas-----
    ]

- NewConnection Handler
    [ This role of handshake or First request can be given in "accept()" function 
    of server {which will be used by server developer in his callback handler}
    
    ----___TO PONDER___----

    
    ]





*/








#endif // SENDER_HPP