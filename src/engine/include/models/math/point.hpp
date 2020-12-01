/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#ifndef _TYRA_POINT_
#define _TYRA_POINT_

/** https://en.wikipedia.org/wiki/Point_(geometry) */
class Point
{

public:
    union
    {
        struct
        {
            float x;
            float y;
        };
        float xy[2] __attribute__((__aligned__(16)));
    };

    Point(const float &t_x, const float &t_y);
    Point(const Point &v);
    Point();
    ~Point();

    void set(const float &t_x, const float &t_y);
    void rotate(const float &t_angle, const float &t_x, const float &t_y);
    void set(const Point &v);
    const void print() const;
};

#endif
