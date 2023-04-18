#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

#include <iostream>
#include <Windows.h>
#include <string>
#include <fstream>
#include <math.h>

class Complex {
public:
	//≈лемент матриц≥ маЇ вид p + i*q
	double p; //д≥йсна частина
	double q; //у€вна частина
	Complex operator*(Complex const& b)
	{
		//a = p, b = q, c = b.p, d = b.q
		//(a + i*b)(c + i*d) = ac - b*d + i*(b*c + a*d)
		return Complex(p * b.p - q * b.q, q * b.p + p * b.q);
	}
	Complex operator+(Complex const& b)
	{
		//a = p, b = q, c = b.p, d = b.q
		//(a + i*b) + (c + i*d) = (a + c) + i* (b + d)
		return Complex(p + b.p, q + b.q);
	}
	Complex operator-(Complex const& b)
	{
		//a = p, b = q, c = b.p, d = b.q
		//(a + i*b) + (c + i*d) = (a + c) + i* (b + d)
		return Complex(p - b.p, q - b.q);
	}
	Complex(double p, double q) {
		this->p = p;
		this->q = q;
	}
	Complex() {
		this->p = 0;
		this->q = 0;
	}
};

void copy(Complex &a, Complex &b) {
	b.p = a.p;
	b.q = a.q;
}

//void init_matrix(int N, Complex*** matr) {
//	*matr = new Complex * [N];
//	int i;
//	for (i = 0; i < N; i++) {
//		(*matr)[i] = new Complex[N];
//	}
//}

Complex** init_matrix(int N) {
	Complex** c = new Complex * [N];
	int i;
	for (i = 0; i < N; i++) {
		c[i] = new Complex[N];
	}
	return c;
}

Complex** init_rect_matrix(int M, int N) {
	Complex** c = new Complex * [M];
	int i;
	for (i = 0; i < M; i++) {
		c[i] = new Complex[N];
	}
	return c;
}

//void add_matrix(Complex** a, Complex** b, int N, Complex*** c) {
//	init_matrix(N, c);
//	Complex sum;
//	for (int i = 0; i < N; i++)
//		for (int j = 0; j < N; j++) {
//			sum = a[i][j] + b[i][j];
//			copy(&sum, &(*c)[i][j]);
//		}
//}

Complex** add_matrix(Complex** a, Complex** b, int N) {
	Complex** c = init_matrix(N);
	Complex sum;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++) {
			sum = a[i][j] + b[i][j];
			copy(sum, c[i][j]);
		}
	return c;
}

//void subtr_matrix(Complex** a, Complex** b, int N, Complex*** c) {
//	init_matrix(N, c);
//	Complex sum;
//	for (int i = 0; i < N; i++)
//		for (int j = 0; j < N; j++) {
//			sum = a[i][j] - b[i][j];
//			copy(&sum, &*c[i][j]);
//		}
//}

Complex** subtr_matrix(Complex** a, Complex** b, int N) {
	Complex** c = init_matrix(N);
	Complex sum;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++) {
			sum = a[i][j] - b[i][j];
			copy(sum, c[i][j]);
		}
	return c;
}

/*Complex** multiply_m(Complex** a, Complex** b, int N) */
void multiply_m(Complex** a, Complex** b, int N, Complex*** res) {
	//Complex** result = init_matrix(N);
	*res = init_matrix(N);
	if (N == 1)
		(*res)/*ult*/[0][0] = a[0][0] * b[0][0];
	else {
		int Nh = N / 2;
		Complex** result_00 = init_matrix(Nh);
		Complex** result_01 = init_matrix(Nh);
		Complex** result_10 = init_matrix(Nh);
		Complex** result_11 = init_matrix(Nh);

		Complex** a00 = init_matrix(Nh);
		Complex** a01 = init_matrix(Nh);
		Complex** a10 = init_matrix(Nh);
		Complex** a11 = init_matrix(Nh);
		Complex** b00 = init_matrix(Nh);
		Complex** b01 = init_matrix(Nh);
		Complex** b10 = init_matrix(Nh);
		Complex** b11 = init_matrix(Nh);

		Complex** m1 = init_matrix(Nh);
		Complex** m2 = init_matrix(Nh);
		Complex** m3 = init_matrix(Nh);
		Complex** m4 = init_matrix(Nh);
		Complex** m5 = init_matrix(Nh);
		Complex** m6 = init_matrix(Nh);
		Complex** m7 = init_matrix(Nh);
		int i, j;
		for (i = 0; i < Nh; i++)
			for (j = 0; j < Nh; j++) {
				copy(a[i][j], a00[i][j]);
				copy(a[i][j + Nh], a01[i][j]);
				copy(a[i + Nh][j], a10[i][j]);
				copy(a[i + Nh][j + Nh], a11[i][j]);
				copy(b[i][j], b00[i][j]);
				copy(b[i][j + Nh], b01[i][j]);
				copy(b[i + Nh][j], b10[i][j]);
				copy(b[i + Nh][j + Nh], b11[i][j]);
			}
		std::thread tm1(multiply_m, add_matrix(a00, a11, Nh), add_matrix(b00, b11, Nh), Nh, &m1);
		std::thread tm2(multiply_m, add_matrix(a10, a11, Nh), b00, Nh, &m2);
		std::thread tm3(multiply_m, a00, subtr_matrix(b01, b11, Nh), Nh, &m3);
		std::thread tm4(multiply_m, a11, subtr_matrix(b10, b00, Nh), Nh, &m4);
		std::thread tm5(multiply_m, add_matrix(a00, a01, Nh), b11, Nh, &m5);
		std::thread tm6(multiply_m, subtr_matrix(a10, a00, Nh), add_matrix(b00, b01, Nh), Nh, &m6);
		std::thread tm7(multiply_m, subtr_matrix(a01, a11, Nh), add_matrix(b10, b11, Nh), Nh, &m7);
		tm1.join();
		tm2.join();
		tm3.join();
		tm4.join();
		tm5.join();
		tm6.join();
		tm7.join();
		//m1 = multiply_m(add_matrix(a00, a11, Nh), add_matrix(b00, b11, Nh), Nh);
		//m2 = multiply_m(add_matrix(a10, a11, Nh), b00, Nh);
		//m3 = multiply_m(a00, subtr_matrix(b01, b11, Nh), Nh);
		//m4 = multiply_m(a11, subtr_matrix(b10, b00, Nh), Nh);
		//m5 = multiply_m(add_matrix(a00, a01, Nh), b11, Nh);
		//m6 = multiply_m(subtr_matrix(a10, a00, Nh), add_matrix(b00, b01, Nh), Nh);
		//m7 = multiply_m(subtr_matrix(a01, a11, Nh), add_matrix(b10, b11, Nh), Nh);

		result_00 = add_matrix(subtr_matrix(add_matrix(m1, m4, Nh), m5, Nh), m7, Nh);
		result_01 = add_matrix(m3, m5, Nh);
		result_10 = add_matrix(m2, m4, Nh);
		result_11 = add_matrix(add_matrix(subtr_matrix(m1, m2, Nh), m3, Nh), m6, Nh);

		for (i = 0; i < Nh; i++)
			for (j = 0; j < Nh; j++) {
				copy(result_00[i][j], (*res)[i][j]);
				copy(result_01[i][j], (*res)[i][j + Nh]);
				copy(result_10[i][j], (*res)[i + Nh][j]);
				copy(result_11[i][j], (*res)[i + Nh][j + Nh]);
			}
		delete result_00, result_01, result_10, result_11,
			a00, a01, a10, a11, b00, b01, b10, b11;
		{
			//Complex** result_00;
			//Complex** result_01;
			//Complex** result_10;
			//Complex** result_11;

			//Complex** a00;
			//Complex** a01;
			//Complex** a10;
			//Complex** a11;
			//Complex** b00;
			//Complex** b01;
			//Complex** b10;
			//Complex** b11;

			//Complex** m1;
			//Complex** m2;
			//Complex** m3;
			//Complex** m4;
			//Complex** m5;
			//Complex** m6;
			//Complex** m7;

			//std::thread inr00(init_matrix, Nh, &result_00);
			//std::thread inr01(init_matrix, Nh, &result_01);
			//std::thread inr10(init_matrix, Nh, &result_10);
			//std::thread inr11(init_matrix, Nh, &result_11);

			//std::thread ina00(init_matrix, Nh, &a00);
			//std::thread ina01(init_matrix, Nh, &a01);
			//std::thread ina10(init_matrix, Nh, &a10);
			//std::thread ina11(init_matrix, Nh, &a11);
			//std::thread inb00(init_matrix, Nh, &b00);
			//std::thread inb01(init_matrix, Nh, &b01);
			//std::thread inb10(init_matrix, Nh, &b10);
			//std::thread inb11(init_matrix, Nh, &b11);

			//std::thread inm1(init_matrix, Nh, &m1);
			//std::thread inm2(init_matrix, Nh, &m2);
			//std::thread inm3(init_matrix, Nh, &m3);
			//std::thread inm4(init_matrix, Nh, &m4);
			//std::thread inm5(init_matrix, Nh, &m5);
			//std::thread inm6(init_matrix, Nh, &m6);
			//std::thread inm7(init_matrix, Nh, &m7);

			//inr00.join();
			//inr01.join();
			//inr10.join();
			//inr11.join();

			//ina00.join();
			//
			//ina01.join();
			//ina10.join();
			//ina11.join();
			//inb00.join();
			//inb01.join();
			//inb10.join();
			//inb11.join();

			//inm1.join();
			//inm2.join();
			//inm3.join();
			//inm4.join();
			//inm5.join();
			//inm6.join();
			//inm7.join();
			//int i, j;
			//for (i = 0; i < Nh; i++)
			//	for (j = 0; j < Nh; j++) {
			//		std::thread ca00(copy, &a[i][j], &a00[i][j]);
			//		std::thread ca01(copy, &a[i][j + Nh], &a01[i][j]);
			//		std::thread ca10(copy, &a[i + Nh][j], &a10[i][j]);
			//		std::thread ca11(copy, &a[i + Nh][j + Nh], &a11[i][j]);
			//		std::thread cb00(copy, &b[i][j], &b00[i][j]);
			//		std::thread cb01(copy, &b[i][j + Nh], &b01[i][j]);
			//		std::thread cb10(copy, &b[i + Nh][j], &b10[i][j]);
			//		std::thread cb11(copy, &b[i + Nh][j + Nh], &b11[i][j]);

			//		ca00.join();
			//		ca01.join();
			//		ca10.join();
			//		ca11.join();
			//		cb00.join();
			//		cb01.join();
			//		cb10.join();
			//		cb11.join();
			//	}

			//Complex** a00pa11;
			//Complex** b00pb11;
			//Complex** a10pa11;
			//Complex** b01sb11;
			//Complex** b10sb00;
			//Complex** a00pb01;
			//Complex** a10sa00;
			//Complex** b00pb01;
			//Complex** a01sa11;
			//Complex** b10pb11;

			//std::thread ta00pa11(add_matrix, a00, a11, Nh, &a00pa11);
			//std::thread tb00pb11(add_matrix, b00, b11, Nh, &b00pb11);
			//std::thread ta10pa11(add_matrix, a10, a11, Nh, &a10pa11);
			//std::thread tb01sb11(subtr_matrix, b01, b11, Nh, &b01sb11);
			//std::thread tb10sb00(subtr_matrix, b10, b00, Nh, &b10sb00);
			//std::thread ta00pb01(add_matrix, a00, a01, Nh, &a00pb01);
			//std::thread ta10sa00(subtr_matrix, a10, a00, Nh, &a10sa00);
			//std::thread tb00pb01(add_matrix, b00, b01, Nh, &b00pb01);
			//std::thread ta01sa11(subtr_matrix, a01, a11, Nh, &a01sa11);
			//std::thread tb10pb11(add_matrix, b10, b11, Nh, &b10pb11);

			//ta00pa11.join();
			//tb00pb11.join();
			//m1 = multiply_m(a00pa11, b00pb11, Nh);

			//ta10pa11.join();
			//m2 = multiply_m(a10pa11, b00, Nh);

			//tb01sb11.join();
			//m3 = multiply_m(a00, b01sb11, Nh);

			//tb10sb00.join();
			//m4 = multiply_m(a11, b10sb00, Nh);

			//ta00pb01.join();
			//m5 = multiply_m(a00pb01, b11, Nh);

			//ta10sa00.join();
			//tb00pb01.join();
			//m6 = multiply_m(a10sa00, b00pb01, Nh);

			//ta01sa11.join();
			//tb10pb11.join();
			//m7 = multiply_m(a01sa11, b10pb11, Nh);

			///*m1 = multiply_m(add_matrix(a00, a11, Nh), add_matrix(b00, b11, Nh), Nh);
			//m2 = multiply_m(add_matrix(a10, a11, Nh), b00, Nh);
			//m3 = multiply_m(a00, subtr_matrix(b01, b11, Nh), Nh);
			//m4 = multiply_m(a11, subtr_matrix(b10, b00, Nh), Nh);
			//m5 = multiply_m(add_matrix(a00, a01, Nh), b11, Nh);
			//m6 = multiply_m(subtr_matrix(a10, a00, Nh), add_matrix(b00, b01, Nh), Nh);
			//m7 = multiply_m(subtr_matrix(a01, a11, Nh), add_matrix(b10, b11, Nh), Nh);*/

			//Complex** m1pm4;
			//Complex** m1pm4sm5;
			//Complex** m1sm2;
			//Complex** m1sm2pm3;
			//
			//std::thread tm1pm4(add_matrix, m1, m4, Nh, &m1pm4);
			//std::thread tm1pm4sm5(subtr_matrix, m1pm4, m5, Nh,&m1pm4sm5);
			//std::thread tm1pm4sm5pm7(add_matrix, m1pm4sm5, m7, Nh, &result_00);
			//std::thread tm3pm5(add_matrix, m3, m5, Nh, &result_01);
			//std::thread tm2pm4(add_matrix, m2, m4, Nh, &result_10);
			//std::thread tm1sm2(subtr_matrix, m1, m2, Nh, &m1sm2);
			//std::thread tm1sm2pm3(add_matrix, m1sm2, m3, Nh, &m1sm2pm3);
			//std::thread tm1sm2pm3m6(add_matrix, m1sm2pm3, m6, Nh, &result_11);

			//tm1pm4.join();
			//tm1pm4sm5.join();
			//tm1pm4sm5pm7.join();
			//tm3pm5.join();
			//tm2pm4.join();
			//tm1sm2.join();
			//tm1sm2pm3.join();
			//tm1sm2pm3m6.join();

			////result_00 = add_matrix(subtr_matrix(add_matrix(m1, m4, Nh), m5, Nh), m7, Nh);
			////result_01 = add_matrix(m3, m5, Nh);
			////result_10 = add_matrix(m2, m4, Nh);
			////result_11 = add_matrix(add_matrix(subtr_matrix(m1, m2, Nh), m3, Nh), m6, Nh);

			//for (i = 0; i < Nh; i++)
			//	for (j = 0; j < Nh; j++) {
			//		std::thread c00(copy, &result_00[i][j], &result[i][j]);
			//		std::thread c01(copy, &result_01[i][j], &result[i][j + Nh]);
			//		std::thread c10(copy, &result_10[i][j], &result[i + Nh][j]);
			//		std::thread c11(copy, &result_11[i][j], &result[i + Nh][j + Nh]);
			//		c00.join();
			//		c01.join();
			//		c10.join();
			//		c11.join();
			//	}
			//delete result_00, result_01, result_10, result_11,	a00, a01, a10, a11, b00, b01, b10, b11;
		}
	}
	//return result;
}

int main(int argc, char** argv)
{
	doctest::Context context;
	context.applyCommandLine(argc, argv);

	int res = context.run(); // run doctest

	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	std::cout << "јлгоритм Ўтрассена\n\n";
	while (true) {
		std::cout << "ќбер≥ть спос≥б введенн€ матриць:\n"
			<< "0 - випадков≥ числа (в≥д 0 до 20 - д≥йсна частина, в≥д 0 до 20i - у€вна)\n"
			<< "1 - введенн€ вручну\n"
			<< "2 - введенн€ через файл matrices.txt\n"
			<< "3 - використати приклад 1\n"
			<< "4 - використати приклад 2\n"
			<< "9 - вих≥д\n";
		int op, M, N, K, i, j, k;
		Complex** a, ** b, ** c;
		a = init_matrix(1);
		b = init_matrix(1);
		c = init_matrix(1);
		//init_matrix(1, &a);
		//init_matrix(1, &b);
		//init_matrix(1, &c);
		std::cin >> op;
		switch (op) {
		case 0:
			std::cout << "¬вед≥ть розм≥рн≥сть матриць A (m x n) ≥ B (n x k)\n";
			std::cout << "M: ";
			std::cin >> M;
			std::cout << "N: ";
			std::cin >> N;
			std::cout << "K: ";
			std::cin >> K;

			a = init_rect_matrix(M, N);
			b = init_rect_matrix(N, K);
			c = init_rect_matrix(M, K);
			for (i = 0; i < M; i++)
				for (j = 0; j < N; j++) {
					a[i][j].p = rand() % 21;
					a[i][j].q = rand() % 21;
				}
			for (i = 0; i < N; i++)
				for (j = 0; j < K; j++) {
					b[i][j].p = rand() % 21;
					b[i][j].q = rand() % 21;
				}
			break;
		case 1:
			std::cout << "¬вед≥ть розм≥рн≥сть матриць A (m x n) ≥ B (n x k)\n";
			std::cout << "M: ";
			std::cin >> M;
			std::cout << "N: ";
			std::cin >> N;
			std::cout << "K: ";
			std::cin >> K;
			a = init_rect_matrix(M, N);
			b = init_rect_matrix(N, K);
			c = init_rect_matrix(M, K);
			std::cout << "¬вед≥ть числа у формат≥ a+b*i: \n";
			std::cout << "ћатриц€ ј: \n";
			for (i = 0; i < M; i++)
				for (j = 0; j < N; j++) {
					std::cout << "≤ндекс (" << i << ", " << j << "): ";
					std::string r, ps, qs;
					std::cin >> r;
					bool q = false;
					for (int k = 0; k < r.length(); k++) {
						if (r[k] == '+')
						{
							a[i][j].p = std::stoi(ps);
							q = true;
						}
						else if (r[k] == '*') {
							a[i][j].q = std::stoi(qs);
							break;
						}
						if (!q) {
							ps += r[k];
						}
						else {
							qs += r[k];
						}

					}
				}
			std::cout << "ћатриц€ B: \n";
			for (i = 0; i < N; i++)
				for (j = 0; j < K; j++) {
					std::cout << "≤ндекс (" << i << ", " << j << "): ";
					std::string r, ps, qs;
					std::cin >> r;
					bool q = false;
					for (int k = 0; k < r.length(); k++) {
						if (r[k] == '+')
						{
							b[i][j].p = std::stoi(ps);
							q = true;
						}
						else if (r[k] == '*') {
							b[i][j].q = std::stoi(qs);
							break;
						}
						if (!q) {
							ps += r[k];
						}
						else {
							qs += r[k];
						}

					}
				}
			break;
		case 2:
		{
			std::ifstream fm("mA.txt");
			std::string Ns;
			std::getline(fm, Ns);
			M = std::stoi(Ns);
			std::getline(fm, Ns);
			N = std::stoi(Ns);
			a = init_rect_matrix(M, N);
			//init_matrix(N, &a);
			//init_matrix(N, &b);
			i = 0, j = 0;
			while (!fm.eof()) {
				std::string r, ps, qs;
				std::getline(fm, r);
				if (r == "")
				{
					i++;
					j = 0;
				}
				else {
					bool q = false;
					for (int k = 0; k < r.length(); k++) {
						if (r[k] == '+')
						{
							a[i][j].p = std::stoi(ps);
							q = true;
							k++;
						}
						else if (r[k] == '*') {
							a[i][j].q = std::stoi(qs);
							break;
						}
						if (!q) {
							ps += r[k];
						}
						else {
							qs += r[k];
						}
					}
					j++;
				}
			}
			fm.close();
			fm.open("mB.txt");
			std::getline(fm, Ns);
			N = std::stoi(Ns);
			std::getline(fm, Ns);
			K = std::stoi(Ns);
			b = init_rect_matrix(N, K);
			i = 0, j = 0;
			while (!fm.eof()) {
				std::string r, ps, qs;
				std::getline(fm, r);
				if (r == "")
				{
					i++;
					j = 0;
				}
				else {
					bool q = false;
					for (int k = 0; k < r.length(); k++) {
						if (r[k] == '+')
						{
							b[i][j].p = std::stoi(ps);
							q = true;
							k++;
						}
						else if (r[k] == '*') {
							b[i][j].q = std::stoi(qs);
							break;
						}
						if (!q) {
							ps += r[k];
						}
						else {
							qs += r[k];
						}
					}
					j++;
				}
			}
		}
		break;
		case 3:
			a = init_matrix(4);
			b = init_matrix(4);
			c = init_matrix(4);
			//init_matrix(4, &a);
			//init_matrix(4, &b);
			//init_matrix(4, &c);
			M = 4;
			N = 4;
			K = 4;
			{int ap[4][4] = { {14,15,-3,-5},
			{15,6,10,5},
			{7,13,18,12},
			{5,15,9,11} };
			int aq[4][4] = { {5,-5,1,18},
				{9,12,-3,-8},
				{-10,20,20,5},
				{8,-6,1,9} };
			int bp[4][4] = { {19,14,-3,14},
				{15,12,6,8},
				{8,8,12,15},
				{17,-7,8,-3} };
			int bq[4][4] = { {18,14,-4,13},
				{-6,10,14,-5},
				{12,18,-5,18},
				{4,-2,-3,0} };
			for (i = 0; i < N; i++)
				for (j = 0; j < N; j++) {
					a[i][j].p = ap[i][j];
					a[i][j].q = aq[i][j];
				}
			for (i = 0; i < N; i++)
				for (j = 0; j < N; j++) {
					b[i][j].p = bp[i][j];
					b[i][j].q = bq[i][j];
				}
			}
			break;
		case 4:
			a = init_matrix(2);
			b = init_matrix(2);
			c = init_matrix(2);
			//init_matrix(2, &a);
			//init_matrix(2, &b);
			//init_matrix(2, &c);
			M = 2;
			N = 2;
			K = 2;
			{int ap[2][2] = { {14,15},
			{15,6} };
			int aq[2][2] = { {5,-5},
				{9,12} };
			int bp[2][2] = { {19,14},
				{15,12} };
			int bq[2][2] = { {18,14},
				{-6,10} };
			for (i = 0; i < N; i++)
				for (j = 0; j < N; j++) {
					a[i][j].p = ap[i][j];
					a[i][j].q = aq[i][j];
				}
			for (i = 0; i < N; i++)
				for (j = 0; j < N; j++) {
					b[i][j].p = bp[i][j];
					b[i][j].q = bq[i][j];
				}
			}
			break;
		case 9:
			delete a, b, c;
			return 0;
		default:
			std::cout << "ѕќћ»Ћ ј!!\n";
			break;
		}
		std::cout << "\nћатриц€ ј\n{ ";
		for (i = 0; i < M; i++) {
			std::cout << "{";
			for (j = 0; j < N; j++) {
				std::cout << a[i][j].p << " + " << a[i][j].q << "*i, ";
			}
			std::cout << "},\n";
		}
		std::cout << " }\n";
		std::cout << "\nћатриц€ B\n{ ";
		for (i = 0; i < N; i++) {
			std::cout << "{";
			for (j = 0; j < K; j++) {
				std::cout << b[i][j].p << " + " << b[i][j].q << "*i, ";
			}
			std::cout << "},\n";
		}
		int max = M > (N > K ? N : K) ? M : (N > K ? N : K);
		Complex** as, ** bs;
		Complex zero;
		zero.p = 0;
		zero.q = 0;
		int size = (int)pow(2, ceil(log2(max)));
		if ((log2(M) != (double)((int)log2(M)) || log2(N) != (double)((int)log2(N))) || M != N)
			//якщо розм≥рн≥сть матриц≥ (в даному випадку ј) не Ї степ≥нню дв≥йки або m i n не Ї р≥вними
		{
			as = new Complex * [size];
			for (i = 0; i < size; i++) {
				as[i] = new Complex[size];
			}
			for (i = 0; i < M; i++) {
				for (j = 0; j < N; j++) {
					as[i][j] = a[i][j];
				}
			}
			for (i = 0; i < size; i++) {
				for (j = N; j < size; j++) {
					as[i][j] = zero;
				}
			}
			for (i = M; i < size; i++) {
				for (j = 0; j < N; j++) {
					as[i][j] = zero;
				}
			}
		}
		else //якщо матриц€ Ї квадратною ≥ розм≥рн≥сть Ї степ≥нню дв≥йки
		{
			as = new Complex * [size];
			for (i = 0; i < size; i++) {
				as[i] = new Complex[size];
			}
			for (i = 0; i < size; i++) {
				for (j = 0; j < size; j++) {
					as[i][j] = a[i][j];
				}
			}
		}
		if (log2(N) != (double)((int)log2(N)) || log2(K) != (double)((int)log2(K)) || N != K)
		{
			bs = new Complex * [size];
			for (i = 0; i < size; i++) {
				bs[i] = new Complex[size];
			}
			for (j = 0; j < N; j++) {
				for (k = 0; k < K; k++) {
					bs[j][k] = b[j][k];
				}
			}
			for (j = 0; j < size; j++) {
				for (k = K; k < size; k++) {
					bs[j][k] = zero;
				}
			}
			for (j = N; j < size; j++) {
				for (k = 0; k < K; k++) {
					bs[j][k] = zero;
				}
			}
		}
		else
		{
			bs = new Complex * [size];
			for (i = 0; i < size; i++) {
				bs[i] = new Complex[size];
			}
			for (i = 0; i < size; i++) {
				for (j = 0; j < size; j++) {
					bs[i][j] = b[i][j];
				}
			}
		}
		std::cout << " }\n";
		std::clock_t start = clock(), end;
		/*c = */multiply_m(as, bs, size, &c);
		std::cout << "\nƒобуток:\n{ ";
		for (i = 0; i < size; i++) {
			std::cout << "{";
			for (j = 0; j < size; j++) {
				std::cout << c[i][j].p << " + " << c[i][j].q << "*i, ";
			}
			std::cout << "},\n";
		}
		std::cout << " }\n";
		end = clock();
		std::cout << "\nћноженн€ виконане за " << (double)(end - start) / CLOCKS_PER_SEC << " с\n\n";
	}
}

bool is_equal(Complex** a, Complex** b, int N) {
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++) {
			if (a[i][j].p != b[i][j].p)
				return false;
			if (a[i][j].q != b[i][j].q)
				return false;
		}
	return true;
}

TEST_CASE("testing multiplication") {
	Complex** a, ** b, ** c, ** prod;
	a = init_matrix(2);
	b = init_matrix(2);
	c = init_matrix(2);
	prod = init_matrix(2);
	//init_matrix(2, &a);
	//init_matrix(2, &b);
	//init_matrix(2, &c);
	int ap[2][2] = { {14,15},
	{15,6} };
	int aq[2][2] = { {5,-5},
		{9,12} };
	int bp[2][2] = { {19,14},
		{15,12} };
	int bq[2][2] = { {18,14},
		{-6,10} };
	int cp[2][2] = { {371,356},
		{285,36} };
	int cq[2][2] = { {182, 356},
		{585, 540} };
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++) {
			a[i][j].p = ap[i][j];
			a[i][j].q = aq[i][j];
			b[i][j].p = bp[i][j];
			b[i][j].q = bq[i][j];
			c[i][j].p = cp[i][j];
			c[i][j].q = cq[i][j];
		}
	multiply_m(a, b, 2, &prod);
	CHECK(is_equal(prod, c, 2));
	a = nullptr;
	b = nullptr;
	c = nullptr;
	prod = nullptr;
	a = init_matrix(4);
	b = init_matrix(4);
	c = init_matrix(4);
	prod = init_matrix(4);
	//init_matrix(4, &a);
	//init_matrix(4, &b);
	//init_matrix(4, &c);
	int ap1[4][4] = { {14,15,-3,-5},
			{15,6,10,5},
			{7,13,18,12},
			{5,15,9,11} };
	int aq1[4][4] = { {5,-5,1,18},
		{9,12,-3,-8},
		{-10,20,20,5},
		{8,-6,1,9} };
	int bp1[4][4] = { {19,14,-3,14},
		{15,12,6,8},
		{8,8,12,15},
		{17,-7,8,-3} };
	int bq1[4][4] = { {18,14,-4,13},
		{-6,10,14,-5},
		{12,18,-5,18},
		{4,-2,-3,0} };
	int cp1[4][4] = { {178,385,121,178},
		{518,119,-20,390},
		{716,-96,164,306},
		{351,193,419,140} };
	int cq1[4][4] = { {440,194,295,44},
		{565,742,-96,546},
		{667,753,458,655},
		{375,345,136,204} };
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++) {
			a[i][j].p = ap1[i][j];
			a[i][j].q = aq1[i][j];
			b[i][j].p = bp1[i][j];
			b[i][j].q = bq1[i][j];
			c[i][j].p = cp1[i][j];
			c[i][j].q = cq1[i][j];
		}
	multiply_m(a, b, 4, &prod);
	CHECK(is_equal(prod, c, 4));
}