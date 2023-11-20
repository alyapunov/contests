#include <cstdint>
#include <cassert>
#include <climits>
#include <type_traits>

#include <iostream>

struct Galois {
	uint64_t n;
	static constexpr uint64_t P = 1000000007;

	Galois() : n(0) {}
	template <class T> Galois(T x) : n(check(x)) {}
	template <class T> Galois& operator=(T x) { n = check(x); return *this; }

	operator uint64_t() const { return n; }

	bool operator==(Galois x) const { return n == x.n; }
	bool operator!=(Galois x) const { return n != x.n; }

	Galois& operator+=(Galois x) { n = (n + x.n) % P; return *this; }
	Galois& operator-=(Galois x) { n = (n + P - x.n) % P; return *this; }
	Galois& operator*=(Galois x) { n = (n * x.n) % P; return *this; }

	Galois operator+(Galois x) const { return Galois{(n + x.n) % P}; }
	Galois operator-(Galois x) const { return Galois{(n + P - x.n) % P}; }
	Galois operator*(Galois x) const { return Galois{(n * x.n) % P}; }

	using G = Galois;

	template <class T> friend bool operator==(T x, G y) { return G{x} == y; }
	template <class T> friend bool operator==(G x, T y) { return x == G{y}; }
	template <class T> friend bool operator!=(T x, G y) { return G{x} != y; }
	template <class T> friend bool operator!=(G x, T y) { return x != G{y}; }

	template <class T> G& operator+=(T x) { return *this += G{x}; }
	template <class T> G& operator-=(T x) { return *this -= G{x}; }
	template <class T> G& operator*=(T x) { return *this *= G{x}; }

	template <class T> friend G operator+(T x, G y) { return G{x} + y; }
	template <class T> friend G operator+(G x, T y) { return x + G{y}; }
	template <class T> friend G operator-(T x, G y) { return G{x} - y; }
	template <class T> friend G operator-(G x, T y) { return x - G{y}; }
	template <class T> friend G operator*(T x, G y) { return G{x} * y; }
	template <class T> friend G operator*(G x, T y) { return x * G{y}; }

	template <class T>
	static std::make_unsigned_t<T> check(T x)
	{
		static_assert(std::numeric_limits<T>::is_integer);
		assert(static_cast<std::make_unsigned_t<T>>(x) < P);
		if constexpr (std::is_signed_v<T>)
			assert(x >= 0);
		return static_cast<std::make_unsigned_t<T>>(x);
	}
};

void check(bool expr, const char *str_expr, const char *file, size_t line)
{
	if (!expr) {
		std::cerr << "FAILED " << str_expr
			  << " at " << file << ":" << line << std::endl;
		abort();
	}
}

#define CHECK(expr) check(expr, #expr, __FILE__, __LINE__)

int main()
{
	Galois x1{1};
	CHECK(x1 == 1);
	Galois x2{2u};
	CHECK(x2 == 2u);
	Galois x3{3l};
	CHECK(x3 == 3l);
	Galois x4{4ul};
	CHECK(x4 == 4ul);
	Galois x5{5ll};
	CHECK(x5 == 5ll);
	Galois x6{6ull};
	CHECK(x6 == 6ull);
	Galois x7{short(7)};
	CHECK(x7 == short(7));
	Galois x8{(unsigned short)8};
	CHECK(x8 == (unsigned short)8);
	Galois xa{(char)0xa};
	CHECK(xa == (char)0xa);
	Galois xb{(unsigned char)0xb};
	CHECK(xb == (unsigned char)0xb);
	Galois xc{(signed char)0xc};
	CHECK(xc == (signed char)0xc);

	Galois x;
	CHECK((uint64_t)x == 0);
	x = 42;
	CHECK(x == 42);
	CHECK(42 == x);
	CHECK(x == Galois{42});
	CHECK(x != 43);
	CHECK(43 != x);
	CHECK(x != Galois{43});
	std::cout << x << std::endl;

	x += 1;
	CHECK(x == 43);
	x += Galois{2};
	CHECK(x == 45);
	x -= 5;
	CHECK(x == 40);
	x -= Galois{3};
	CHECK(x == 37);
	x *= 2;
	CHECK(x == 74);
	x *= Galois{3};
	CHECK(x == 222);
	x *= 0;
	CHECK(x == 0);

	x = 100;
	x += Galois::P - 1;
	CHECK(x == 99);
	x += Galois{Galois::P - 9};
	CHECK(x == 90);
	x -= 100;
	CHECK(x == Galois::P - 10);
	x -= Galois{Galois::P - 1};
	CHECK(x == Galois::P - 9);
	x *= 2;
	CHECK(x == Galois::P - 18);
	x *= Galois{3};
	CHECK(x == Galois::P - 54);

	CHECK(Galois{1} + Galois{1} == 2);
	CHECK(Galois{1} + 2 == 3);
	CHECK(3 + Galois{2} == 5);
	CHECK(Galois{3} - Galois{1} == 2);
	CHECK(Galois{5} - 2 == 3);
	CHECK(8 - Galois{3} == 5);
	CHECK(Galois{2} * Galois{2} == 4);
	CHECK(Galois{3} * 4 == 12);
	CHECK(5 * Galois{3} == 15);

	CHECK(Galois{10} + Galois{Galois::P - 5} == 5);
	CHECK(Galois{10} + (Galois::P - 5) == 5);
	CHECK(10 + Galois{Galois::P - 5} == 5);
	CHECK(Galois{5} - Galois{Galois::P - 5} == 10);
	CHECK(Galois{5} - (Galois::P - 5) == 10);
	CHECK(5 - Galois{Galois::P - 5} == 10);
	CHECK(Galois{Galois::P - 5} * Galois{2} == Galois::P - 10);
	CHECK(Galois{Galois::P - 5} * 3 == Galois::P - 15);
	CHECK((Galois::P - 5) * Galois{4} == Galois::P - 20);

	Galois tests[] = {2, 10, 17};
	for (Galois t : tests) {
		Galois r{1};

		Galois sq{t};
		for (uint64_t p = Galois::P; p > 0; p /= 2) {
			if (p % 2)
				r *= sq;
			sq *= sq;
		}
		CHECK(r == t);
	}
}
