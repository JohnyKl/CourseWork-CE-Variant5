#pragma once

#include "stdafx.h"

using namespace System;
using namespace System::Drawing;

namespace Zaveryshka_Kursova {
    const int POS_LEFT      = 0;
    const int POS_RIGHT     = 1;

    const float POINT_RADIUS  = 5.0f;

    ref class CustomPoint
    {
    public:
        Point^ point;
        Brush^  brush;
        int position;

        CustomPoint(void);
        CustomPoint(Point^ point);
        void drawPoint(Windows::Forms::PaintEventArgs^ e);
        std::string toString(int index);
    };
}

