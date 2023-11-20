#include <iostream>
#include <cmath>
#include <vector>
#include <string>

struct Galois {
	static constexpr uint64_t P = 1000000007;
	uint64_t n;
	explicit Galois(uint64_t x = 0) : n(x) {}
	void operator=(uint64_t x) { n = x; }
	operator uint64_t() const { return n; }

	void operator+=(uint64_t x) { n = (n + x) % P; }
	void operator+=(Galois x) { n = (n + x.n) % P; }
	void operator-=(uint64_t x) { n = (n + P - x) % P; }
	void operator-=(Galois x) { n = (n + P - x.n) % P; }
	void operator*=(uint64_t x) { n = (n * x) % P; }
	void operator*=(Galois x) { n = (n * x.n) % P; }

	Galois operator+(uint64_t x) const { return Galois{(n + x) % P}; }
	Galois operator+(Galois x) const { return Galois{(n + x.n) % P}; }
	Galois operator-(uint64_t x) const { return Galois{(n + P - x) % P}; }
	Galois operator-(Galois x) const { return Galois{(n + P - x.n) % P}; }
	Galois operator*(uint64_t x) const { return Galois{(n * x) % P}; }
	Galois operator*(Galois x) const { return Galois{(n * x.n) % P}; }
};

void decrement(std::string& s)
{
	for (auto itr = s.rbegin(); itr != s.rend(); ++itr) {
		if ((*itr)-- == '0')
			*itr = '0';
		else
			break;
	}
}

std::string_view prepare(const std::string& s)
{
	std::string_view res{s};
	while (!res.empty() && res[0] == '0')
		res.remove_prefix(1);
	return res;
}

static const char *
decrement_and_truncate(std::string_view& s)
{
	const char *decr = &s.back();
	while (*decr == '0')
		--decr;
	while (s.front() == '0')
		s.remove_prefix(1);
	if (decr == &s.front() && *decr == '1')
		s.remove_prefix(1);
	return decr;
}

char rotate_if(char c, bool cond)
{
	static const char rot[2][256] = {
		"................................................0123456789",
		"................................................9012345678"
	};
	return rot[cond][static_cast<unsigned char>(c)];
}

template <class NUMBER>
NUMBER
palicount(std::string_view a, std::string_view b)
{
	const char *decr_a = decrement_and_truncate(a);
	const char *decr_b = decrement_and_truncate(b);
	if (a.empty() || b.empty())
		return NUMBER{1};
	a.remove_suffix(a.size() > b.size() ? a.size() - b.size() : 0);
	b.remove_suffix(b.size() > a.size() ? b.size() - a.size() : 0);
	size_t n = a.size();

	NUMBER num{0}, res{0};
	bool is_rot = false;
	for (size_t i = 0, j = n - 1, k = 0, l = n - 1; i < n; i++, j--) {
		if (k == i) {
			is_rot = false;
			for (k++, l--; k < n; k++, l--) {
				char ca = rotate_if(a[k], &a[k] >= decr_a);
				char cb = rotate_if(b[l], &b[l] >= decr_b);
				if (ca != cb) {
					is_rot = ca < cb;
					break;
				}
			}
		}
		char ca = rotate_if(rotate_if(a[i], &a[i] >= decr_a), is_rot);
		char cb = rotate_if(b[j], &b[j] >= decr_b);

		if (i == 0) {
			res += 1 + (std::min(ca, cb) - '0');
		} else {
			res += (num + 1) * (cb - '0');
			if (ca < cb)
				res += (ca - '0' + 1);
		}

		num *= 10;
		num += (ca - '0');
	}

	return res;
}


uint64_t
palicount(uint64_t l, uint64_t r)
{
	if (l > r)
		std::swap(l, r);
	size_t num_digits = log10(r - 1e-5) + 1;
	uint64_t digits[20] = {};
	uint64_t p[20] = {};
	p[num_digits - 1] = 1;
	for (size_t j = 1; j < num_digits; j++)
		p[num_digits - 1 - j] = p[num_digits - j] * 10;

	uint64_t inv = 0;
	uint64_t res = 1;
	for (uint64_t i = 1; i < l; i++) {
		for (size_t j = 0; ; j++) {
			digits[j]++;
			inv += p[j];
			if (digits[j] < 10)
				break;
			inv -= p[j] * 10;
			digits[j] = 0;
		}
		if (inv < r) {
			res++;
		}
	}
	return res;
}

void
check(std::string_view a, std::string_view  b, uint64_t exp)
{
	uint64_t res = palicount<uint64_t>(a, b);
	if (res != exp) {
		std::cerr << a << ", " << b
			  << ": expected " << exp << ", got " << res << "\n";
		abort();
	}
}

int main(int argn, const char** args)
{
#ifdef MY
	if (argn >= 2)
		freopen(args[1], "r", stdin);
	else if (freopen("./data1.txt", "r", stdin) == NULL)
		freopen("../data1.txt", "r", stdin);
#else
	(void)argn;
	(void)args;
#endif
	std::ios::sync_with_stdio(false);

	check("102", "111", palicount(102, 111));

	check("24", "60", 16);
	check("60", "24", 16);

	for (uint64_t i = 1; i < 1000; i++) {
		for (uint64_t j = 0; j < 1000; j++) {
			std::string sa = std::to_string(i);
			std::string sb = std::to_string(j);
			while (sa.length() < 3)
				sa.insert(sa.begin(), '0');
			while (sb.length() < 3)
				sb.insert(sb.begin(), '0');
			check(sa, sb, palicount(i, j));
		}
	}

	check("4444444", "4444443", palicount(4444444, 4444443));

	check("4289383", "46930886", 2013050);
	check("81692777", "14636915", 11957310);
	check("57747793", "24238335", 13997111);
	check("19885386", "49760492", 9895073);
	check("96516649", "89641421", 86518904);
	check("25202362", "50490027", 12724685);
	check("83368690", "2520059", 2100945);
	check("44897763", "67513926", 30312251);
	check("65180540", "40383426", 26322142);
	check("4089172", "3455736", 1413117);
	check("35005211", "21595368", 7559509);
	check("94702567", "26956429", 25528437);
	check("36465782", "61021530", 22251984);
	check("78722862", "33665123", 26502155);
	check("45174067", "68703135", 31036009);
	check("1513929", "1979802", 299731);
	check("15634022", "35723058", 5584959);
	check("69133069", "25898167", 17904204);
	check("59961393", "89018456", 53376713);
	check("28175011", "56478042", 15912700);

	for (uint64_t i = 1; i < 1000; i++) {
		for (uint64_t j = 0; j < 1000; j++) {
			std::string sa = std::to_string(i);
			std::string sb = std::to_string(j);
			while (sa.length() < 3)
				sa.insert(sa.begin(), '0');
			while (sb.length() < 3)
				sb.insert(sb.begin(), '0');
			check(sa, sb, palicount(i, j));
		}
	}

	for (size_t i = 0; i < 1000; i++) {
		uint64_t a = rand() % 1000000 + 1;
		uint64_t b = rand() % 1000000 + 1;
		std::string sa = std::to_string(a);
		std::string sb = std::to_string(b);
		check(sa, sb, palicount(a, b));
	}

	//std::string str;
	//while (std::getline(std::cin, str)) {
	//}
}