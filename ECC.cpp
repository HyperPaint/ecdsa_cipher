#include <cmath>
#include <vector>

using namespace std;

struct Curve
{
    // числа, определяющие кривую
    int a;
    int b;
    // определяет длину осей
    int mod;
};

// глобальная переменная определяющая кривую
Curve curve;

struct Point
{
    int x;
    int y;
};

/* работа с кривыми */
template <typename T>
T normalize(T a);
int inverse(int a);
Point add(Point first, Point second);
Point sub(Point first, Point second);
Point multiply(Point point, int multiplier);
bool setCurve(int a, int b, int mod);
bool checkCurve();
vector<Point> getAllPoints();

// mod
template <typename T>
T normalize(T a)
{
    while (a < 0)
    {
        a += curve.mod;
    }
    while (a >= curve.mod)
    {
        a -= curve.mod;
    }
    return a;
}

// возвращает обратный элемент
int inverse(int a)
{
    if (a == 0)
    {
        return a;
    }
    else
    {
        for (int i = 0; i < curve.mod; i++)
        {
            if (((a * i) % curve.mod) == 1)
            {
                return i;
            }
        }
    }
    throw 0;
}

// сложение двух точек
Point add(Point first, Point second)
{
    Point result;
    // числитель, знаменатель, угол наклона прямой
    int top, bottom, tangent;
    // одна и та же точка
    if (first.x == second.x && first.y == second.y)
    {
        top = normalize(3 * (int)pow(first.x, 2) + curve.a);
        bottom = inverse(normalize(2 * first.y));
        tangent = normalize(top * bottom);
        result.x = normalize((int)pow(tangent, 2) - first.x - second.x);
        result.y = normalize(tangent * (first.x - result.x) - first.y);
        return result;
    }
    // одна пара
    else if (first.x == second.x)
    {
        // бесконечно удаленная точка
        result.x = -1;
        result.y = -1;
        return result;
    }
    // первая точка бесконечно удаленная
    else if (first.x == -1 && first.y == -1)
    {
        return second;
    }
    // вторая точка бесконечно удаленная
    else if (second.x == -1 && second.y == -1)
    {
        return first;
    }
    // разные точки
    else
    {
        top = normalize(first.y - second.y);
        bottom = inverse(normalize(first.x - second.x));
        tangent = normalize(top * bottom);
        result.x = normalize((int)pow(tangent, 2) - first.x - second.x);
        result.y = normalize(tangent * (first.x - result.x) - first.y);
        return result;
    }
}

// вычитание
Point sub(Point first, Point second)
{
    second.y = normalize(-second.y);
    return add(first, second);
}

// умножение
Point multiply(Point point, int multiplier)
{
    Point Result = point;
    for (int i = 1; i < multiplier; i++)
    {
        Result = add(Result, point);
    }
    return Result;
}

// задать кривую
bool setCurve(int a, int b, int mod)
{
    curve.a = a;
    curve.b = b;
    curve.mod = mod;
    // проверка кривой на сингулярность
    return checkCurve();
}

// проверка кривой
bool checkCurve()
{
    if (4 * (int)pow(curve.a, 3) + 27 * (int)pow(curve.b, 2) == 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

// нахожу все точки кривой (целочисленные)
vector<Point> getAllPoints()
{
    vector<Point> points;
    Point point;
    int value;
    // перебираю все возможные точки от 0 до максимального значения в остатках
    for (int current = 1; current < curve.mod; current++)
    {
        // по формуле кривой вычисляю y
        value = normalize((int)pow(current, 3) + curve.a * current + curve.b);
        // корень в остатках
        // перебираю все числа от 0 до максимального значения в остатках
        for (int i = 0; i < curve.mod; i++)
        {
            // если число в квадрате == текущему значению, то оно и будет результатом корня
            if (i * i % curve.mod == value)
            {
                // записываю точку
                point.x = current;
                point.y = i;
                points.push_back(point);
                // и обратную ей по y точку
                point.x = current;
                point.y = normalize(curve.mod - i);
                points.push_back(point);
                // выхожу из цикла
                break;
            }
        }
    }
    return points;
}