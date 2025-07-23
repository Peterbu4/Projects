#include <iostream>
#include <cmath>

const double PI = acos(-1.0);

using namespace std;

class figure // Virtual class
{
public:
    virtual double area() const = 0;
    virtual void print_info() const = 0;
    virtual double circumference() const = 0;
};
template <class T> // Comparing sizes using template
T maxim(T a, T b)
{
    if (a > b)
        return a;
    else
        return b;
}
class Circle : public figure // Derived class of figure
{
public:
    Circle(double r, double x, double y) : R(r), X(x), Y(y) {}
    double area () const override
    {
        return PI * pow(R, 2);
    }
    double circumference() const override
    {
        return 2 * PI * R;
    }
    void print_info() const override
    {
        double tempA = area();
        double tempC = circumference();
        cout << "Okrag o promieniu " << R << " ma srodek w punkcie (" << X << ", " << Y << ")" << " oraz powierzchnie: " << tempA << " i obwod: " << tempC << endl;
    }
    double operator -(Circle &q)
    {
        double wynik;
        wynik = sqrt(pow((X - q.X), 2) + pow((Y - q.Y), 2));
        return wynik;
    }
private:
    double R;
    double X;
    double Y;
};
class Rectangle : public figure // Derived class of figure
{
public:
    Rectangle(double x, double y, double h, double w) : X(x), Y(y), width(w), hight(h) {}
    void print_info() const override
    {
        double tempA = area();
        double tempC = circumference();
        cout << "Lewy dolny rog prostokata ma wspolzedne (" << X << ", " << Y << "), a boki maja dlugosc: "<<hight<<", "<<width <<
            " jego powierzchnia to: " << tempA << ", a obwod to: " << tempC << endl;
    }
    void area() const override
    {
        return width * hight;
    }
    double circumference() const override
    {
        return 2 * width + 2 * hight;
    }
private:
    double X;
    double Y;
    double width;
    double hight;
};


int main()
{
    Circle A(2.9, 4.1, 5.6);
    Circle C(1.3, 5, 6.7);
    Rectangle B(12.3, 4.5, 3.2, 6.1);
    A.print_info();
    B.print_info();
    double area_circle2 = C.area();
    double area_circle1 = A.area();
    double D = maxim(area_circle1, area_circle2);
    cout << "Odleglosc miedzy srodkami okregow wynosi: " << A - C << endl;
    cout << "Powierzchnia wiekszego okregu wynosi: " << D << endl;
    return 0;
}
