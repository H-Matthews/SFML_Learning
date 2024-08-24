#ifndef COMMAND_QUEUE_H
#define COMMAND_QUEUE_H

#include <queue>

#include "Command.hpp"

// Wrapper around the std::Queue Class for our Command structure
class CommandQueue
{

    public:
        void push(const Command& command);
        Command pop();
        bool isEmpty() const;

    private:
        std::queue<Command> mQueue;

};

#endif