#include "stdafx.h"
#include "Painter.h"

using namespace std;
using namespace System::Drawing;
using namespace System;
using namespace Zaveryshka_Kursova;

Painter::Painter(void)//����������� �����, �������� ���� �����
{
    startOfSegment = gcnew Point();
    endOfSegment   = gcnew Point();
}

void Painter::preparePainter()//�������, �� ������ ��������� ��'���� ����� �� �����������
{
    readCoordsOfAllPoints();//����� ���������� ��� ����� � ������� �����
    readCoordsOfSegment();//����� ���������� ������ � ������� �����
    findPositionOfPointsRelativelySegment();//������� ������� ����� ������� ������ (����-������)
    findPositionOfPointsRelativelyRectangle();//������� ������� �����, �� ��������� �� ���� ������� ������, ������� ������� �������� 
    writeCoordsIntoFile();//������ ���������� � �����
}

void Painter::readCoordsOfAllPoints()//����� ���������� ��� ����� � �����
{
    points = readCoordsFromFile("coordsOfPoints.txt");
}
void Painter::readCoordsOfSegment()//����� ���������� ������ �� ����� ����� ������ ������
{
    Array^ segmentsTop = readCoordsFromFile("coordsOfSegment.txt");//����� ������ ������

    try
    {
        if(segmentsTop->Length > 0)//���� ������� ������ ����� 0
        {
            CustomPoint^ pointA = (CustomPoint^)segmentsTop->GetValue(0);
            CustomPoint^ pointB = (CustomPoint^)segmentsTop->GetValue(1);

            this->startOfSegment = pointA->point;//����������� ������� ������� ������
            this->endOfSegment = pointB->point;//����������� ������� ���� ������
        }
    }
    catch(Exception^ e) { }
}

Array^ Painter::readCoordsFromFile(string path)//������� ��� ���������� ��������� � �����; ����������� ������� � ���� �� ����� �� �������� �� �����, ���� ���� ��������� ���������� ��������� � �����
{
    ifstream ifs(path); //��������� ��������� ������ �����
    Array^ pointsArray;
    vector<int*> coords;//����������� ��������� �������, � ��� �������������� ����������
	
	if(ifs.is_open())//���� �������� ����� �������
    {
        while (! ifs.eof() )//���� �� ��������� ���� �����
        {            
			string line = "";//���������� �� ����������� ������ ��� ���������� � �����
			getline(ifs,line);//��������� ���� ������ � �����
			
			int* coordsFromLine = new int[2];//���������� �� ����������� ������ ��� ���������� �������� ������� ����������
			findCoordsInLine(line, coordsFromLine);//������ ���������� � ������

			if(coordsFromLine[0] != -1 && coordsFromLine[1] != -1)//���� ���������� ��������
			{
				coords.push_back(coordsFromLine);//������ ���������� � ������
			}			
        }
		
        int pointsNumber = coords.size();//������� ��������� ������������ �� �������� ������� ������� � �� �

		pointsArray = Array::CreateInstance(CustomPoint::typeid, pointsNumber);//����������� ������ � ������ ���� Point
		
        for (int i = 0; i < pointsNumber; i++)
        {
            pointsArray->SetValue(gcnew CustomPoint(gcnew Point(coords[i][0],coords[i][1])), i);//������ � ����� ����� ���� �������� ����� � ���� ���������� ������� ���� �������� �� �������
        }
    }

    ifs.close();//�������� ��������� ������

    return pointsArray;
}

void Painter::findCoordsInLine(string line, int* coords)
{
	size_t posX = line.find_first_of("=") + 1; //������ ������� ������� ������� ���� ������� ��������� ������� "="
	size_t posY = line.find_last_of("=") + 1;//������ ������� ������� ������� ���� ���������� ��������� ������� "="

	coords[0] = -1;//������������ ���������� ����� ����� ��� ������������� ��������� ��������� ���������
	coords[1] = -1;

	if(posX > 0 && posY > 0)//���� ������� ��������
	{
		size_t countX = line.find_first_of(",") - posX;//������ ������ ������� ��������� ������� "," �� ������ �� ����� ������� ����� ����� ����� ����������, ��� ������ ������� �����
		size_t countY = line.length() - posY;//������ ������ ���������� ��������� ������� "," �� ������ �� ����� ������� ����� ����� ����� ����������, ��� ������ ������� �����		

		if(countX > 0 && countY > 0)//���� ������� ��������
		{
			int x = stoi(line.substr(posX,countX));//����������� � ����� ������� ������� ������ � ������������
			int y = stoi(line.substr(posY,countY));

			coords[0] = x;//��������� ����� ����������
			coords[1] = y;
		}
	}
}

void Painter::writeCoordsWithParamsIntoFile(int filteredPosition, string path, string header)//�������, �� ������ ���������� �����, �� ���������� �� ������� ��������, � ����
{
    ofstream ofs(path);//������� ���� ��������� � ����

	ofs << header.c_str();

    for(int i = 0; i < points->Length; i++)//��� ����� ����� � �����
    {
        CustomPoint^ point = (CustomPoint^) points->GetValue(i);

        if(point->position == filteredPosition)//���� ������� ������� ������� �������
        {
            ofs << point->toString(i + 1).c_str();//�������� � ���� ���������� �����
        }
    }

    ofs.close();//������� ���� ��������� � ����
}

void Painter::writeCoordsIntoFile()//�������, �� ������ ����� � ����� ���������� � ���� �� ������, ��������� � ����� ���������
{
    writeCoordsWithParamsIntoFile(POS_LEFT, "thirdFileWithCoords.txt","Points on a left side of the segment:\n");    
    writeCoordsWithParamsIntoFile(POS_RIGHT, "coordsOfPoints.txt","Points on a right side of the segment:\n");  
}

void Painter::drawSegment(Windows::Forms::PaintEventArgs^  e)//�������, �� �������� �� ����� ������, �� ���������� ����� ������� point� �� pointB, ���������� � ����� ���������
{
    e->Graphics->DrawLine(gcnew Pen(Color::Black, 3.0f), *startOfSegment, *endOfSegment);//��������� ������
}

void Painter::drawPoints(Windows::Forms::PaintEventArgs^  e)//�������, �� ����� �����
{
    for(int i = 0; i < points->Length; i++)//��� ����� �������� � ����� �����
    {
        CustomPoint^ point = (CustomPoint^)points->GetValue(i);

        point->drawPoint(e);//���������� �����
    }
}

void Painter::findPositionOfPointsRelativelySegment()
{
    //����������� ��������� ����� ������ �� ���� �������� �� �������
    int aX = Painter::startOfSegment->X;
    int aY = Painter::startOfSegment->Y;
    int bX = Painter::endOfSegment->X;
    int bY = Painter::endOfSegment->Y;

    for(int i = 0; i < Painter::points->Length; i++)//��� ����� ����� � ����� �����
    {
        CustomPoint^ point = (CustomPoint^)Painter::points->GetValue(i);

        //����������� ��������� �����
        int cX = point->point->X;
        int cY = point->point->Y;

        //��������� ���������, �� �������������� ������� ����� ������� �����. �������� ����� � ����� ������
        int positionCoefficient = (cX - aX)*(bY - aY) - (cY - aY)*(bX - aX);

        //� ��������� �� ��������
        if(positionCoefficient < 0)//����� ���� �� �����
        {
            point->brush = gcnew SolidBrush(Color::Red);
            point->position = POS_LEFT;
        }
        else //����� ���� �� �������� �����
        {
            point->brush = gcnew SolidBrush(Color::Blue);
            point->position = POS_RIGHT;
        }
    }
}

void Painter::findPositionOfPointsRelativelyRectangle()
{
    //���������� � ����������� ����� �� ������ ��� ������������ �� ���� ������� �� ��������
    int lowBoundX = startOfSegment->X;
    int lowBoundY = endOfSegment->Y;
    int highBoundX = endOfSegment->X;
    int highBoundY = startOfSegment->Y;

    //�������� �� �������� ������ ��� ����� �� �������� �������
    if(lowBoundX > highBoundX)
    {
        int temp = lowBoundX;
        lowBoundX = highBoundX;
        highBoundX = temp;
    }
    if(lowBoundY > highBoundY)
    {
        int temp = lowBoundY;
        lowBoundY = highBoundY;
        highBoundY = temp;
    }

    for(int i = 0; i < Painter::points->Length; i++)//��� ����� ����� � ����� �����
    {
        CustomPoint^ point = (CustomPoint^)Painter::points->GetValue(i);

        if(point->position == POS_RIGHT)//���� ����� ����������� � ���� ������� ������� �����
        {
            int cX = point->point->X;
            int cY = point->point->Y;

            if((lowBoundX < cX && highBoundX > cX) && (lowBoundY < cY && highBoundY > cY))//���� �� ���������� ����������� � ��� ������������
            {
                point->brush = gcnew SolidBrush(Color::Green);//������ ���� �����
            }
        }
    }
}

void Painter::drawRectangle(Windows::Forms::PaintEventArgs^ e)//�������, �� �������� ����������� �� �����
{
    Pen^ pen = gcnew Pen(Color::Black, 3.0f);

    int coordX = startOfSegment->X;//��������� ���������� �
    int coordY = startOfSegment->Y;//��������� ���������� �
    int width  = (unsigned) endOfSegment->X - coordX;//������ ������������ �� ������ �� ���������� ���������� � ����� ������ �������
    int height = (unsigned) endOfSegment->Y - coordY;//������ ������������ �� ������ �� ���������� ���������� � ����� ������ �������

    e->Graphics->DrawRectangle(pen, coordX, coordY, width, height);//���������� �����������
}
