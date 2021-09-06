#include <iostream>
#include <iomanip>
#include <vector>

template<class T>
void fillData(std::vector<T>& p, std::vector<T>& q, std::vector<T>& r, std::vector<T>& d)
{
	p = { 1, 1, 1, 1 };
	q = { -3, -2, -2, -2, -3 };
	r = { 1, 1, 1, 1 };
	d = { 0, -4.67, -4.67, -4.67, 0 };
}


template<class T>
bool checkData(const std::vector<T>& p, const std::vector<T>& q, const std::vector<T>& r, const std::vector<T>& d)
{
	if (d.empty())
	{
		std::wcout << L"Вектор свободных членов пуст" << std::endl;
		return false;
	}
	if (p.size() != r.size() || p.size() + 1 != q.size() || q.size() != d.size())
	{
		std::wcout << L"Несоответствие размеров векторов" << std::endl;
		return false;
	}

	bool correct = true;
	bool strictEquation = false;
	for (size_t i = 1; i < p.size(); i++)
	{
		if (abs(q[i]) < abs(p[i - 1]) + abs(r[i]))
		{
			correct = false;
			break;
		}

		if (abs(q[i]) > abs(p[i - 1]) + abs(r[i]))
			strictEquation = true;
	}

	if (!correct || !strictEquation)
	{
		std::wcout << L"Данные неустойчевы относительно погрешностей уровнения" << std::endl;
		//return false;
	}

	return true;
}

template<class T>
std::vector<T> calculateX(const std::vector<T> &p, const std::vector<T> &q, const std::vector<T> &r, const std::vector<T> &d)
{
	if (!checkData(p, q, r, d))
		return std::vector<T>();

	//if mat 1*1 then solution is easy to solve, but common algo can't solve this
	if (d.size() == 1)
		return std::vector<T>({ d[0] / r[0] });
	
	T z;
	std::vector<T> e( p.size() );
	std::vector<T> n( p.size() );
	std::vector<T> x( d.size() );

	e[0] = -r[0] / q[0];
	n[0] = d[0] / q[0];

	for (size_t i = 1; i < p.size(); i++)
	{
		z = q[i] + p[i - 1] * e[i - 1];
		e[i] = -r[i] / z;
		n[i] = (d[i] - p[i - 1] * n[i - 1]) / z;
	}

	x.back() =	(d.back() - p.back() * n.back()) /
				(q.back() + p.back() * e.back());

	for (size_t i = p.size(); i >= 1; i--)
		x[i - 1] = e[i - 1] * x[i] + n[i - 1];

	return x;
}

template<class T>
T calculateDelta(const std::vector<T> &p, const std::vector<T> &q, const std::vector<T> &r, const std::vector<T> &d, const std::vector<T> &x)
{
	T deltaMax = -1;
	T delta;

	auto getA = [=](size_t i, size_t j) -> T
	{
		if (i == j)
			return q[i];

		if (i == j + 1)
			return p[j];

		if (i + 1 == j)
			return r[i];

		return 0;
	};

	for (size_t i = 0; i < x.size(); i++)
	{
		delta = d[i];
		for (size_t j = 0; j < x.size(); j++)
		{
			delta -= getA(i, j) * x[j];
		}
		delta = abs(delta);

		if (delta > deltaMax)
			deltaMax = delta;
	}

	return deltaMax;
}

template<class T>
void showResult(const std::vector<T>& x, T delta)
{
	std::wcout << L"Вектор x:" << std::endl;
	for (size_t i = 0; i < x.size(); i++)
	{
		std::wcout << std::setprecision(5) << std::fixed << x[i] << " ";
	}
	std::wcout << std::endl;
	std::wcout << L"Невязка: " << delta << std::endl;
}

int main()
{
	setlocale(LC_ALL, "");
	double delta;
	std::vector<double> p, q, r, d, x;

	fillData(p, q, r, d);
	x = calculateX(p, q, r, d);
	delta = calculateDelta(p, q, r, d, x);
	showResult(x, delta);
}
