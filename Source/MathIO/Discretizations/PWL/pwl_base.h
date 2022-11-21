#ifndef _MATHIO_PWL_BASE_
#define _MATHIO_PWL_BASE_


class CellFEView
{
public:
    int degrees_of_freedom;

public:
    CellFEView(int number_of_freedoms)
    {
        degrees_of_freedom = number_of_freedoms;
    }
};

#endif // PWL_BASE_H