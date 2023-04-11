#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

#include <iostream>
#include <Windows.h>
#include <string>
#include <fstream>
#include <math.h>

class Complex {
public:
	//������� ������� �� ��� p + i*q
	double p; //����� �������
	double q; //����� �������
	Complex operator*(Complex const& b)
	{
		//a = p, b = q, c = b.p, d = b.q
		//(a + i*b)(c + i*d) = ac - b*d + i*(b*c + a*d)
		/*Complex r;
		r.p = p * b.p - q * b.q;
		r.q = q * b.p + p * b.q;*/
		return Complex(p * b.p - q * b.q, q * b.p + p * b.q);
	}
	Complex operator+(Complex const& b)
	{
		//a = p, b = q, c = b.p, d = b.q
		//(a + i*b) + (c + i*d) = (a + c) + i* (b + d)
		/*Complex r;
		r.p = p + b.p;
		r.q = q + b.q;*/
		return Complex(p + b.p, q + b.q);
	}
	Complex operator-(Complex const& b)
	{
		//a = p, b = q, c = b.p, d = b.q
		//(a + i*b) + (c + i*d) = (a + c) + i* (b + d)
		/*Complex r;
		r.p = p - b.p;
		r.q = q - b.q;*/
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

void copy(Complex& a, Complex& b) {
	b.p = a.p;
	b.q = a.q;
}

void init_matrix(int N, Complex*** matr) {
	*matr = new Complex * [N];
	int i;
	for (i = 0; i < N; i++) {
		*matr[i] = new Complex[N];
	}
}

//Complex** init_matrix(int N) {
//	Complex** c = new Complex * [N];
//	int i;
//	for (i = 0; i < N; i++) {
//		c[i] = new Complex[N];
//	}
//	return c;
//}

Complex** init_rect_matrix(int M, int N) {
	Complex** c = new Complex * [M];
	int i;
	for (i = 0; i < M; i++) {
		c[i] = new Complex[N];
	}
	return c;
}

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

Complex** multiply_m(Complex** a, Complex** b, int N) {
	Complex** result;
	init_matrix(N, &result);
	if (N == 1)
		result[0][0] = a[0][0] * b[0][0];
	else {
		int Nh = N / 2;

		Complex** result_00;
		Complex** result_01;
		Complex** result_10;
		Complex** result_11;

		Complex** a00;
		Complex** a01;
		Complex** a10;
		Complex** a11;
		Complex** b00;
		Complex** b01;
		Complex** b10;
		Complex** b11;

		Complex** m1;
		Complex** m2;
		Complex** m3;
		Complex** m4;
		Complex** m5;
		Complex** m6;
		Complex** m7;

		std::thread inr00(init_matrix, Nh, result_00);
		std::thread inr01(init_matrix, Nh, result_01);
		std::thread inr10(init_matrix, Nh, result_10);
		std::thread inr11(init_matrix, Nh, result_11);

		std::thread ina00(init_matrix, Nh, a00);
		std::thread ina01(init_matrix, Nh, a01);
		std::thread ina10(init_matrix, Nh, a10);
		std::thread ina11(init_matrix, Nh, a11);
		std::thread inb00(init_matrix, Nh, b00);
		std::thread inb01(init_matrix, Nh, b01);
		std::thread inb10(init_matrix, Nh, b10);
		std::thread inb11(init_matrix, Nh, b11);

		std::thread inm1(init_matrix, Nh, m1);
		std::thread inm2(init_matrix, Nh, m2);
		std::thread inm3(init_matrix, Nh, m3);
		std::thread inm4(init_matrix, Nh, m4);
		std::thread inm5(init_matrix, Nh, m5);
		std::thread inm6(init_matrix, Nh, m6);
		std::thread inm7(init_matrix, Nh, m7);

		inr00.join();
		inr01.join();
		inr10.join();
		inr11.join();

		ina00.join();
		ina01.join();
		ina10.join();
		ina11.join();
		inb00.join();
		inb01.join();
		inb10.join();
		inb11.join();

		inm1.join();
		inm2.join();
		inm3.join();
		inm4.join();
		inm5.join();
		inm6.join();
		inm7.join();
		int i, j;
		for (i = 0; i < Nh; i++)
			for (j = 0; j < Nh; j++) {
				std::thread ca00(copy, a[i][j], a00[i][j]);
				std::thread ca01(copy, a[i][j + Nh], a01[i][j]);
				std::thread ca10(copy, a[i + Nh][j], a10[i][j]);
				std::thread ca11(copy, a[i + Nh][j + Nh], a11[i][j]);
				std::thread cb00(copy, b[i][j], b00[i][j]);
				std::thread cb01(copy, b[i][j + Nh], b01[i][j]);
				std::thread cb10(copy, b[i + Nh][j], b10[i][j]);
				std::thread cb11(copy, b[i + Nh][j + Nh], b11[i][j]);

				ca00.join();
				ca01.join();
				ca10.join();
				ca11.join();
				cb00.join();
				cb01.join();
				cb10.join();
				cb11.join();
			}

		m1 = multiply_m(add_matrix(a00, a11, Nh), add_matrix(b00, b11, Nh), Nh), m1;
		m2 = multiply_m(add_matrix(a10, a11, Nh), b00, Nh);
		m3 = multiply_m(a00, subtr_matrix(b01, b11, Nh), Nh);
		m4 = multiply_m(a11, subtr_matrix(b10, b00, Nh), Nh);
		m5 = multiply_m(add_matrix(a00, a01, Nh), b11, Nh);
		m6 = multiply_m(subtr_matrix(a10, a00, Nh), add_matrix(b00, b01, Nh), Nh);
		m7 = multiply_m(subtr_matrix(a01, a11, Nh), add_matrix(b10, b11, Nh), Nh);

		result_00 = add_matrix(subtr_matrix(add_matrix(m1, m4, Nh), m5, Nh), m7, Nh);
		result_01 = add_matrix(m3, m5, Nh);
		result_10 = add_matrix(m2, m4, Nh);
		result_11 = add_matrix(add_matrix(subtr_matrix(m1, m2, Nh), m3, Nh), m6, Nh);

		for (i = 0; i < Nh; i++)
			for (j = 0; j < Nh; j++) {
				copy(result_00[i][j], result[i][j]);
				copy(result_01[i][j], result[i][j + Nh]);
				copy(result_10[i][j], result[i + Nh][j]);
				copy(result_11[i][j], result[i + Nh][j + Nh]);
			}
		delete result_00, result_01, result_10, result_11,
			a00, a01, a10, a11, b00, b01, b10, b11;
	}
	return result;
}

int main(int argc, char** argv)
{
	doctest::Context context;
	context.applyCommandLine(argc, argv);

	int res = context.run(); // run doctest

	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	std::cout << "�������� ���������\n\n";
	while (true) {
		std::cout << "������ ����� �������� �������:\n"
			<< "0 - �������� ����� (�� 0 �� 20 - ����� �������, �� 0 �� 20i - �����)\n"
			<< "1 - �������� ������\n"
			<< "2 - �������� ����� ���� matrices.txt\n"
			<< "3 - ����������� ������� 1\n"
			<< "4 - ����������� ������� 2\n"
			<< "9 - �����\n";
		int op, M, N, K, i, j, k;
		Complex** a, ** b, ** c;
		a = init_matrix(1);
		b = init_matrix(1);
		c = init_matrix(1);
		std::cin >> op;
		switch (op) {
		case 0:
			std::cout << "������ ���������� ������� A (m x n) � B (n x k)\n";
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
			std::cout << "������ ���������� ������� A (m x n) � B (n x k)\n";
			std::cout << "M: ";
			std::cin >> M;
			std::cout << "N: ";
			std::cin >> N;
			std::cout << "K: ";
			std::cin >> K;
			a = init_rect_matrix(M, N);
			b = init_rect_matrix(N, K);
			c = init_rect_matrix(M, K);
			std::cout << "������ ����� � ������ a+b*i: \n";
			std::cout << "������� �: \n";
			for (i = 0; i < M; i++)
				for (j = 0; j < N; j++) {
					std::cout << "������ (" << i << ", " << j << "): ";
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
			std::cout << "������� B: \n";
			for (i = 0; i < N; i++)
				for (j = 0; j < K; j++) {
					std::cout << "������ (" << i << ", " << j << "): ";
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
			N = std::stoi(Ns);
			a = init_matrix(N);
			b = init_matrix(N);
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
			N = 4;
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
			N = 2;
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
			std::cout << "�������!!\n";
			break;
		}
		std::cout << "\n������� �\n{ ";
		for (i = 0; i < M; i++) {
			std::cout << "{";
			for (j = 0; j < N; j++) {
				std::cout << a[i][j].p << " + " << a[i][j].q << "*i, ";
			}
			std::cout << "},\n";
		}
		std::cout << " }\n";
		std::cout << "\n������� B\n{ ";
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
			//���� ���������� ������� (� ������ ������� �) �� � ������� ����� ��� m i n �� � ������
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
		else //���� ������� � ���������� � ���������� � ������� �����
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
		c = multiply_m(as, bs, size);
		std::cout << "\n�������:\n{ ";
		for (i = 0; i < size; i++) {
			std::cout << "{";
			for (j = 0; j < size; j++) {
				std::cout << c[i][j].p << " + " << c[i][j].q << "*i, ";
			}
			std::cout << "},\n";
		}
		std::cout << " }\n";
		end = clock();
		std::cout << "\n�������� �������� �� " << (double)(end - start) / CLOCKS_PER_SEC << " �\n\n";
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
	Complex** a, ** b, ** c;
	a = init_matrix(2);
	b = init_matrix(2);
	c = init_matrix(2);
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
	CHECK(is_equal(multiply_m(a, b, 2), c, 2));
	a = nullptr;
	b = nullptr;
	c = nullptr;
	a = init_matrix(4);
	b = init_matrix(4);
	c = init_matrix(4);
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
	CHECK(is_equal(multiply_m(a, b, 4), c, 4));
}