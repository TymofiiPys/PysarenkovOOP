#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

#include <iostream>
#include <thread>
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

class ComMatrix {
public:
	int N;
	Complex** matr;

	void init_matrix(int N) {
		this->matr = new Complex * [N];
		int i;
		for (i = 0; i < N; i++) {
			this->matr[i] = new Complex[N];
		}
	}
	void copyto(int idest, int jdest, ComMatrix source, int is, int js) {
		this->matr[idest][jdest] = source.matr[is][js];
	}
	ComMatrix(int N) {
		this->N = N;
		this->init_matrix(N);
	}
	ComMatrix(Complex** matr, int N) {
		this->matr = matr;
		this->N = N;
	}
	ComMatrix() {
		this->N = 0;
		this->matr = nullptr;
	}
};

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

ComMatrix add_matrix(ComMatrix a, ComMatrix b) {
	ComMatrix c(a.N);
	for (int i = 0; i < a.N; i++)
		for (int j = 0; j < a.N; j++) {
			c.matr[i][j] = a.matr[i][j] + b.matr[i][j];
		}
	return c;
}

ComMatrix subtr_matrix(ComMatrix a, ComMatrix b) {
	ComMatrix c(a.N);
	for (int i = 0; i < a.N; i++)
		for (int j = 0; j < a.N; j++) {
			c.matr[i][j] = a.matr[i][j] - b.matr[i][j];
		}
	return c;
}

void multiply_m(ComMatrix a, ComMatrix b, ComMatrix* res) {
	int N = a.N;
	res->init_matrix(N);
	if (N == 1)
		res->matr[0][0] = a.matr[0][0] * b.matr[0][0];
	else {
		int Nh = N / 2;
		ComMatrix result_00(Nh);
		ComMatrix result_01(Nh);
		ComMatrix result_10(Nh);
		ComMatrix result_11(Nh);

		ComMatrix a00(Nh);
		ComMatrix a01(Nh);
		ComMatrix a10(Nh);
		ComMatrix a11(Nh);
		ComMatrix b00(Nh);
		ComMatrix b01(Nh);
		ComMatrix b10(Nh);
		ComMatrix b11(Nh);

		ComMatrix m1(Nh);
		ComMatrix m2(Nh);
		ComMatrix m3(Nh);
		ComMatrix m4(Nh);
		ComMatrix m5(Nh);
		ComMatrix m6(Nh);
		ComMatrix m7(Nh);
		int i, j;
		for (i = 0; i < Nh; i++)
			for (j = 0; j < Nh; j++) {
				a00.copyto(i, j, a, i, j);
				a01.copyto(i, j, a, i, j + Nh);
				a10.copyto(i, j, a, i + Nh, j);
				a11.copyto(i, j, a, i + Nh, j + Nh);
				b00.copyto(i, j, b, i, j);
				b01.copyto(i, j, b, i, j + Nh);
				b10.copyto(i, j, b, i + Nh, j);
				b11.copyto(i, j, b, i + Nh, j + Nh);
			}
		std::thread tm1(multiply_m, add_matrix(a00, a11), add_matrix(b00, b11), &m1);
		std::thread tm2(multiply_m, add_matrix(a10, a11), b00, &m2);
		std::thread tm3(multiply_m, a00, subtr_matrix(b01, b11), &m3);
		std::thread tm4(multiply_m, a11, subtr_matrix(b10, b00), &m4);
		std::thread tm5(multiply_m, add_matrix(a00, a01), b11, &m5);
		std::thread tm6(multiply_m, subtr_matrix(a10, a00), add_matrix(b00, b01), &m6);
		std::thread tm7(multiply_m, subtr_matrix(a01, a11), add_matrix(b10, b11), &m7);

		tm1.join();
		tm2.join();
		tm3.join();
		tm4.join();
		tm5.join();
		tm6.join();
		tm7.join();

		result_00 = add_matrix(subtr_matrix(add_matrix(m1, m4), m5), m7);
		result_01 = add_matrix(m3, m5);
		result_10 = add_matrix(m2, m4);
		result_11 = add_matrix(add_matrix(subtr_matrix(m1, m2), m3), m6);

		for (i = 0; i < Nh; i++)
			for (j = 0; j < Nh; j++) {
				res->copyto(i, j, result_00, i, j);
				res->copyto(i, j + Nh, result_01, i, j);
				res->copyto(i + Nh, j, result_10, i, j);
				res->copyto(i + Nh, j + Nh, result_11, i, j);
			}
	}
}

int main(int argc, char** argv)
{
	//doctest::Context context;
	//context.applyCommandLine(argc, argv);

/*	int res = context.run();*/ // run doctest

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
		ComMatrix cma(as, size);
		ComMatrix cmb(bs, size);
		ComMatrix cmc(c, size);
		multiply_m(cma, cmb, &cmc);
		//c = multiply_m(as, bs, size, &c);
		std::cout << "\nƒобуток:\n{ ";
		for (i = 0; i < size; i++) {
			std::cout << "{";
			for (j = 0; j < size; j++) {
				std::cout << cmc.matr[i][j].p << " + " << cmc.matr[i][j].q << "*i, ";
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
	ComMatrix cma(a, 2);
	ComMatrix cmb(b, 2);
	ComMatrix cmc(prod, 2);
	multiply_m(cma, cmb, &cmc);

	CHECK(is_equal(cmc.matr, c, 2));
	a = nullptr;
	b = nullptr;
	c = nullptr;
	prod = nullptr;
	a = init_matrix(4);
	b = init_matrix(4);
	c = init_matrix(4);
	prod = init_matrix(4);

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
	cma = ComMatrix(a, 4);
	cmb = ComMatrix(b, 4);
	cmc = ComMatrix(prod, 4);
	multiply_m(cma, cmb, &cmc);
	CHECK(is_equal(cmc.matr, c, 4));
}