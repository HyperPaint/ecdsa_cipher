#include <iostream>
#include <cstdlib>

#include "ECC.cpp" // кривые
#include "SHA1.cpp" // хэширование

using namespace std;

string getTextFromFile(string path);
int inverse(int a, int base);

int main()
{
    setlocale(LC_ALL, "ru");
    // рандомизация рандома
    srand(time(0));

    // вот это буду подписывать
    string message = getTextFromFile("file.txt");

    // вот такая кривая
    int a = 1;
    int b = 6;
    int mod = 11;
    cout << "Кривая с параметрами a: " << a << " b: " << b << " mod: " << mod << endl;
    cout << "Подходит: " << boolalpha << setCurve(1, 6, 11) << endl;
    
    // все точки кривой
    vector<Point> allPoints = getAllPoints();
    // первообразный корень
    Point pointP = allPoints[rand() % allPoints.size()];
    cout << "Первообразный корень: " << "(" << pointP.x << " " << pointP.y << ")" << endl;
    // вычисляю порядок подгруппы
    cout << "Точки циклической группы:" << endl;
    Point pointPcopy = pointP;
    int q = 0; // порядок циклической подгруппы - большое простое число - количество точек в группе
    while (true)
    {
        q++;
        pointPcopy = add(pointPcopy, pointP);
        if (pointP.x == pointPcopy.x && pointP.y == pointPcopy.y)
        {
            break;
        }
        cout << "(" << pointPcopy.x << " " << pointPcopy.y << ")" << endl;
    }
    cout << "Порядок группы: " << q << endl; // если число не простое, не будет обратного элемента

    // генерация ключей
    int x = (rand() % (q - 1)) + 1; // закрытый ключ
    cout << "Закрытый ключ: " << x << endl;
    Point pointQ = multiply(pointP, x); // открытый ключ
    cout << "Открытый ключ: " << "(" << pointQ.x << " " << pointQ.y << ")" << endl;

    // подпись
    start:
    int k = (rand() % (q - 1)) + 1; // случайное целое число
    int r = multiply(pointP, k).x % q;
    if (r == 0)
    {
        goto start; // уравнение подписи не зависит от секретного ключа
    }
    int s = (inverse(k, q) * (stoll(myHash(message), 0, 16) + x * r)) % q;
    if (s == 0)
    {
        goto start; // обратного значения нет, невозможно проверить
    }
    cout << "Реальная подпись: " << "(" << r << " " << s << ")" << endl; // это подпись

    // проверка
    int u1 = (inverse(s, q) * stoll(myHash(message), 0, 16)) % q;
    int u2 = (inverse(s, q) * r) % q;
    Point checkPoint = add(multiply(pointP, u1), multiply(pointQ, u2));
    checkPoint.x = checkPoint.x % q;
    cout << "Вычисленная подпись: " << "(" << checkPoint.x << " " << "X" << ")" << endl; // это вычисленная подпись
    cout << "Подходит: " << ((checkPoint.x % q) == r) << endl;

    system("pause");
    return 0;
}

string getTextFromFile(string path)
{
    // считывание файла
    ifstream file;
    // открытие файла в бинарном виде на чтение
    string text = "";
    file.open(path, ios::in | ios::binary);
    if (!file.is_open())
    {
        file.close();
        return "";
    }
    // считываю файл
    const size_t readCount = 100;
    char buff[readCount] = "\0";
    while (!file.eof())
    {
        file.read(buff, readCount);
        text += buff;
    }
    // закрываю его
    file.close();

    return text;
}

// возвращает обратный элемент
int inverse(int a, int base)
{
    if (a == 0)
    {
        return a;
    }
    else
    {
        for (int i = 0; i < base; i++)
        {
            if (((a * i) % base) == 1)
            {
                return i;
            }
        }
    }
    throw 0;
}