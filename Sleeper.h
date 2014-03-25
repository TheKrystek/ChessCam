#ifndef SLEEPER_H
#define SLEEPER_H

class Sleeper
{
public:
    Sleeper();
    static void usleep(unsigned long usecs);
    static void msleep(unsigned long usecs);
    static void leep(unsigned long usecs);
};

#endif // SLEEPER_H
