#ifndef ANIMATION_H
#define ANIMATION_H

class Animation
{
public:

    float x_s, y_s; // Start pos.
    float x_e, y_e; // End pos.
    float t_s, t_e; // Start/End time.

    float x, y;
    bool active;


    Animation();


};

#endif // ANIMATION_H
