#include "stdafx.h"
#include "CustomPoint.h"

using namespace Zaveryshka_Kursova;
using namespace System;
using namespace System::Drawing;

CustomPoint::CustomPoint(void)//����������� ����� ��� ���������
{
    CustomPoint(gcnew System::Drawing::Point());
}

CustomPoint::CustomPoint(Point^ point)//����������� �����
{
    this->point = point;
    brush       = gcnew SolidBrush(Color::Black);
    position    = 0;
}

void CustomPoint::drawPoint(Windows::Forms::PaintEventArgs^ e)
{
    e->Graphics->FillEllipse(brush, point->X - POINT_RADIUS, point->Y - POINT_RADIUS, POINT_RADIUS*2, POINT_RADIUS*2);//���������� �������� ���� �������� 4 ������� �����
}

std::string CustomPoint::toString(int index)//������������ ������� ��'���� � ����������� ������
{
    std::ostringstream os;
	std::string coordText = "Point";

	os<<index << ": x"<<index<<"=";
	os<<point->X << ", y" << index << "=";
    os << point->Y;
    os << "\n";
    coordText += os.str();

    os.eof();
    
    return coordText;
}