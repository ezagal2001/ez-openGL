#ifndef EZ_SHAPE_G
#define EZ_SHAPE_G

#include <ShapeData.hpp>
#include <cstring>
class ShapeGenerator
{
    public:
        // the static qualifier in this context means (I think)
        // that the function belongs to the class 
        // itself and not to an instance of the class
        static ShapeData makeTriangle();
        static ShapeData makeCube();
};

#endif
