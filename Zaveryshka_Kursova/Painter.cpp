#include "stdafx.h"
#include "Painter.h"

using namespace std;
using namespace System::Drawing;
using namespace System;
using namespace Zaveryshka_Kursova;

Painter::Painter(void)//конструктор класу, ініціалізує поля класу
{
    startOfSegment = gcnew Point();
    endOfSegment   = gcnew Point();
}

void Painter::preparePainter()//функція, що виконує підготовку об'єктів класу до відображення
{
    readCoordsOfAllPoints();//зчитує координати всіх точок з другого файлу
    readCoordsOfSegment();//зчитує координати відрізку з першого файлу
    findPositionOfPointsRelativelySegment();//визначає позицію точок відносно відрізку (зліва-справа)
    findPositionOfPointsRelativelyRectangle();//визначає позицію точок, що знаходять по одну сторону відрізку, відносно площини квадрату 
    writeCoordsIntoFile();//записує координати у файли
}

void Painter::readCoordsOfAllPoints()//зчитує координати всіх точок з файлу
{
    points = readCoordsFromFile("coordsOfPoints.txt");
}
void Painter::readCoordsOfSegment()//зчитує координати відрізку та формує точки вершин відрізку
{
    Array^ segmentsTop = readCoordsFromFile("coordsOfSegment.txt");//масив вершин відрізку

    try
    {
        if(segmentsTop->Length > 0)//якщо зчитано вершин більше 0
        {
            CustomPoint^ pointA = (CustomPoint^)segmentsTop->GetValue(0);
            CustomPoint^ pointB = (CustomPoint^)segmentsTop->GetValue(1);

            this->startOfSegment = pointA->point;//ініціалізація вершини початку відрізку
            this->endOfSegment = pointB->point;//ініціалізація вершини кінця відрізку
        }
    }
    catch(Exception^ e) { }
}

Array^ Painter::readCoordsFromFile(string path)//функція для зчитування координат з файлу; параметрами функції є шлях до файлу та вказівник на масив, який буде заповнено значеннями координат з файлу
{
    ifstream ifs(path); //створення файлового потоку вводу
    Array^ pointsArray;
    vector<int*> coords;//ініціалізація структури вектору, у яку додаватимуться координати
	
	if(ifs.is_open())//якщо файловий опток відкрито
    {
        while (! ifs.eof() )//доки не досягнуто кінця файлу
        {            
			string line = "";//оголошення та ініціалізація стрічки для зчитування з файлу
			getline(ifs,line);//отримання однієї стрічки з файлу
			
			int* coordsFromLine = new int[2];//оголошення та ініціалізація масиву для збереження зчитаних значень координати
			findCoordsInLine(line, coordsFromLine);//знайти координати в стрічці

			if(coordsFromLine[0] != -1 && coordsFromLine[1] != -1)//якщо координату знайдено
			{
				coords.push_back(coordsFromLine);//додати координату в вектор
			}			
        }
		
        int pointsNumber = coords.size();//кількість координат обчислюється як загальна кількість значень Х та У

		pointsArray = Array::CreateInstance(CustomPoint::typeid, pointsNumber);//ініціалізація масиву з даними типу Point
		
        for (int i = 0; i < pointsNumber; i++)
        {
            pointsArray->SetValue(gcnew CustomPoint(gcnew Point(coords[i][0],coords[i][1])), i);//додати у масив точок нову створену точку з двох послідовних значень осей ординати та абсциси
        }
    }

    ifs.close();//закриття файлового потоку

    return pointsArray;
}

void Painter::findCoordsInLine(string line, int* coords)
{
	size_t posX = line.find_first_of("=") + 1; //знайти позицію першого символа після першого входження символа "="
	size_t posY = line.find_last_of("=") + 1;//знайти позицію першого символа після останнього входження символа "="

	coords[0] = -1;//ініціалізувати значеннями масив точок для ідентифікації відсутності знайдених координат
	coords[1] = -1;

	if(posX > 0 && posY > 0)//якщо символи знайдено
	{
		size_t countX = line.find_first_of(",") - posX;//знайти індекс першого входження символу "," та відняти від нього позицію першої цифри числа координати, щоб знайти довжину числа
		size_t countY = line.length() - posY;//знайти індекс останнього входження символу "," та відняти від нього позицію першої цифри числа координати, щоб знайти довжину числа		

		if(countX > 0 && countY > 0)//якщо символи знайдено
		{
			int x = stoi(line.substr(posX,countX));//перетворити в число потрібну частину стрічки з координатами
			int y = stoi(line.substr(posY,countY));

			coords[0] = x;//заповнити масив значеннями
			coords[1] = y;
		}
	}
}

void Painter::writeCoordsWithParamsIntoFile(int filteredPosition, string path, string header)//функція, що записує координати точок, які фільтровані за заданою позицією, у файл
{
    ofstream ofs(path);//відкрити потік виведення в файл

	ofs << header.c_str();

    for(int i = 0; i < points->Length; i++)//для кожної точки в масиві
    {
        CustomPoint^ point = (CustomPoint^) points->GetValue(i);

        if(point->position == filteredPosition)//якщо позиція відповідає потрібній позиції
        {
            ofs << point->toString(i + 1).c_str();//записати в файл координати точки
        }
    }

    ofs.close();//закрити потік виведення в файл
}

void Painter::writeCoordsIntoFile()//функція, що записує надані у масиві координати у файл за шляхом, переданим у якості параметру
{
    writeCoordsWithParamsIntoFile(POS_LEFT, "thirdFileWithCoords.txt","Points on a left side of the segment:\n");    
    writeCoordsWithParamsIntoFile(POS_RIGHT, "coordsOfPoints.txt","Points on a right side of the segment:\n");  
}

void Painter::drawSegment(Windows::Forms::PaintEventArgs^  e)//функція, що відображує на екрані відрізок, що обмежується двома точками pointА та pointB, переданими у якості параметрів
{
    e->Graphics->DrawLine(gcnew Pen(Color::Black, 3.0f), *startOfSegment, *endOfSegment);//малювання відрізку
}

void Painter::drawPoints(Windows::Forms::PaintEventArgs^  e)//функція, що малює точки
{
    for(int i = 0; i < points->Length; i++)//для кожної переданої в масиві точки
    {
        CustomPoint^ point = (CustomPoint^)points->GetValue(i);

        point->drawPoint(e);//намалювати точку
    }
}

void Painter::findPositionOfPointsRelativelySegment()
{
    //ініціалізація координат кінців відрізку по осям ординати та абсциси
    int aX = Painter::startOfSegment->X;
    int aY = Painter::startOfSegment->Y;
    int bX = Painter::endOfSegment->X;
    int bY = Painter::endOfSegment->Y;

    for(int i = 0; i < Painter::points->Length; i++)//для кожної точки в масиві точок
    {
        CustomPoint^ point = (CustomPoint^)Painter::points->GetValue(i);

        //ініціалізація координат точки
        int cX = point->point->X;
        int cY = point->point->Y;

        //обрахунок значенння, що відображуватиме позицію точки відносно прямої. частиною якого є даний відрізок
        int positionCoefficient = (cX - aX)*(bY - aY) - (cY - aY)*(bX - aX);

        //в залежності від значення
        if(positionCoefficient < 0)//точка зліва від прямої
        {
            point->brush = gcnew SolidBrush(Color::Red);
            point->position = POS_LEFT;
        }
        else //точка зліва чи належить прямій
        {
            point->brush = gcnew SolidBrush(Color::Blue);
            point->position = POS_RIGHT;
        }
    }
}

void Painter::findPositionOfPointsRelativelyRectangle()
{
    //оголошення і ініціалізація нижніх та верхніх меж прямокутника по осям абсциси та ординати
    int lowBoundX = startOfSegment->X;
    int lowBoundY = endOfSegment->Y;
    int highBoundX = endOfSegment->X;
    int highBoundY = startOfSegment->Y;

    //перевірка чи значення нижньої межі менше за значення верхньої
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

    for(int i = 0; i < Painter::points->Length; i++)//для кожної точки в масиві точок
    {
        CustomPoint^ point = (CustomPoint^)Painter::points->GetValue(i);

        if(point->position == POS_RIGHT)//якщо точка знаходиться з однієї сторони відносно прямої
        {
            int cX = point->point->X;
            int cY = point->point->Y;

            if((lowBoundX < cX && highBoundX > cX) && (lowBoundY < cY && highBoundY > cY))//якщо її координати потрапляють в межі прямокутника
            {
                point->brush = gcnew SolidBrush(Color::Green);//змінити колір точки
            }
        }
    }
}

void Painter::drawRectangle(Windows::Forms::PaintEventArgs^ e)//функція, що відображує прямокутник на екрані
{
    Pen^ pen = gcnew Pen(Color::Black, 3.0f);

    int coordX = startOfSegment->X;//початкова координата Х
    int coordY = startOfSegment->Y;//початкова координата У
    int width  = (unsigned) endOfSegment->X - coordX;//ширина прямокутника як різниця між значеннями координати Х кінців відрізка діагоналі
    int height = (unsigned) endOfSegment->Y - coordY;//висота прямокутника як різниця між значеннями координати У кінців відрізка діагоналі

    e->Graphics->DrawRectangle(pen, coordX, coordY, width, height);//намалювати прямокутник
}
