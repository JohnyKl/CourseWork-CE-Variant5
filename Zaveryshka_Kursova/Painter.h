#pragma once

#include "stdafx.h"
#include "CustomPoint.h"

using namespace std;
using namespace System::Drawing;
using namespace System;

namespace Zaveryshka_Kursova {
    ref class Painter
    {
	private:
		Array^  points;
        Point^ startOfSegment;
        Point^ endOfSegment;

        Array^ readCoordsFromFile(string path);
        void writeCoordsIntoFile();
        void writeCoordsWithParamsIntoFile(int filteredPosition, string path, string header);
        void readCoordsOfAllPoints();
        void readCoordsOfSegment();
        void findPositionOfPointsRelativelySegment();
        void findPositionOfPointsRelativelyRectangle();
		void findCoordsInLine(string line, int* coords);
    public:
        Painter(void);

        void preparePainter();

        void drawSegment(Windows::Forms::PaintEventArgs^ e);
        void drawPoints(Windows::Forms::PaintEventArgs^ e);
        void drawRectangle(Windows::Forms::PaintEventArgs^ e);
    };
}