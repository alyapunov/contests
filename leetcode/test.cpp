#include <array>
#include <vector>
#include <iostream>
#include <cassert>
#include <cstddef>

using namespace std;

struct Field {
	uint8_t row;
	uint8_t col;
	uint8_t grp;
	uint8_t pos;
	uint16_t mask = 0;
	constexpr Field(uint8_t i) :
		row(i / 9), col(i % 9),
		grp(row / 3 * 3 + col / 3),
		pos(row % 3 * 3 + col % 3)
	{
	}
};

template<size_t N, size_t... I>
constexpr std::array<Field, N> create_fields_impl(std::index_sequence<I...>) {
	return std::array<Field, N>{ {I...} };
}

template<std::size_t N>
constexpr std::array<Field, N> create_fields() {
	return create_fields_impl<N>(std::make_index_sequence<N>{});
}

struct Board {
	std::array<Field, 9 * 9> fields = create_fields<9 * 9>();
	std::array<uint16_t, 9> possible_mask_row = {511,511,511,511,511,511,511,511,511};
	std::array<uint16_t, 9> possible_mask_col = {511,511,511,511,511,511,511,511,511};
	std::array<uint16_t, 9> possible_mask_grp = {511,511,511,511,511,511,511,511,511};

	constexpr Board() = default;

	Field* operator[](size_t i) { return &fields[i * 9]; }

	void choose(Field &f, uint16_t mask)
	{
		possible_mask_row[f.row] ^= mask ^ f.mask;
		possible_mask_col[f.col] ^= mask ^ f.mask;
		possible_mask_grp[f.grp] ^= mask ^ f.mask;
		f.mask = mask;
	}
	bool choose(Field &f)
	{
		auto possible_mask = possible_mask_row[f.row] &
				     possible_mask_col[f.col] &
				     possible_mask_grp[f.grp];
		if (f.mask != 0) {
			assert((possible_mask & f.mask) == 0);
			possible_mask &= ~(f.mask | (f.mask - 1));
			if (possible_mask == 0) {
				choose(f, 0);
				return false;
			}
		} else {
			if (possible_mask == 0)
				return false;
		}

		uint16_t mask = possible_mask & (possible_mask ^ (possible_mask - 1));
		choose(f, mask);
		return true;
	}
	void find()
	{
		uint32_t arr[81];
		uint32_t size = 0;
		for (uint32_t i = 0; i < 81; i++)
			if (fields[i].mask == 0)
				arr[size++] = i;
		uint32_t pos = 0;
		while (true) {
			if (choose(fields[arr[pos]])) {
				pos++;
				if (pos == size)
					return;
			} else {
				assert(pos > 0);
				pos--;
			}
		}
	}
};

static constexpr Board default_board;

class Solution {
public:
	void solveSudoku(vector<vector<char>>& in) {
		Board b = default_board;
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				if (in[i][j] == '.')
					continue;
				b.choose(b[i][j], 1 << (in[i][j] - '1'));
			}
		}
		b.find();
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				in[i][j] = '1' + __builtin_ctz(b[i][j].mask);
			}
		}
	}
};

static void
pnr(const vector<vector<char>>& data)
{
	for (int i = 0; i < 9; i++) {
		std::cout << (i ? " {" : "{{");
		for (int j = 0; j < 9; j++) {
			std::cout << (j ? ", " : "");
			std::cout << "'" << data[i][j] << "'";
		}
		std::cout << (i != 8 ? "},\n" : "}}\n");
	}
}

static void
check(vector<vector<char>> in, const vector<vector<char>>& out)
{
	Solution s;
	s.solveSudoku(in);
	//pnr(in);

	if (in != out) {
		std::cout << "Expected:\n";
		pnr(out);
		std::cout << "Got:\n";
		pnr(in);
		abort();
	}
}

int main()
{
	if (0) {
		vector <vector<char>> in =
			{{'5', '3', '.', '.', '7', '.', '.', '.', '.'},
			 {'6', '.', '.', '1', '9', '5', '.', '.', '.'},
			 {'.', '9', '8', '.', '.', '.', '.', '6', '.'},
			 {'8', '.', '.', '.', '6', '.', '.', '.', '3'},
			 {'4', '.', '.', '8', '.', '3', '.', '.', '1'},
			 {'7', '.', '.', '.', '2', '.', '.', '.', '6'},
			 {'.', '6', '.', '.', '.', '.', '2', '8', '.'},
			 {'.', '.', '.', '4', '1', '9', '.', '.', '5'},
			 {'.', '.', '.', '.', '8', '.', '.', '7', '9'}};
		vector <vector<char>> out =
			{{'5', '3', '4', '6', '7', '8', '9', '1', '2'},
			 {'6', '7', '2', '1', '9', '5', '3', '4', '8'},
			 {'1', '9', '8', '3', '4', '2', '5', '6', '7'},
			 {'8', '5', '9', '7', '6', '1', '4', '2', '3'},
			 {'4', '2', '6', '8', '5', '3', '7', '9', '1'},
			 {'7', '1', '3', '9', '2', '4', '8', '5', '6'},
			 {'9', '6', '1', '5', '3', '7', '2', '8', '4'},
			 {'2', '8', '7', '4', '1', '9', '6', '3', '5'},
			 {'3', '4', '5', '2', '8', '6', '1', '7', '9'}};
		check(in, out);
	}
	if (1) {
		vector<vector<char>> in =
			{{'.', '.', '9', '7', '4', '8', '.', '.', '.'},
			 {'7', '.', '.', '.', '.', '.', '.', '.', '.'},
			 {'.', '2', '.', '1', '.', '9', '.', '.', '.'},
			 {'.', '.', '7', '.', '.', '.', '2', '4', '.'},
			 {'.', '6', '4', '.', '1', '.', '5', '9', '.'},
			 {'.', '9', '8', '.', '.', '.', '3', '.', '.'},
			 {'.', '.', '.', '8', '.', '3', '.', '2', '.'},
			 {'.', '.', '.', '.', '.', '.', '.', '.', '6'},
			 {'.', '.', '.', '2', '7', '5', '9', '.', '.'}};

		vector <vector<char>> out =
			{{'1', '9', '8', '2', '7', '3', '4', '6', '5'},
			 {'7', '6', '5', '8', '4', '9', '1', '3', '2'},
			 {'4', '3', '2', '5', '1', '6', '8', '7', '9'},
			 {'8', '7', '9', '6', '5', '4', '3', '2', '1'},
			 {'5', '4', '6', '3', '2', '1', '9', '8', '7'},
			 {'3', '2', '1', '9', '8', '7', '6', '5', '4'},
			 {'9', '8', '7', '1', '3', '2', '5', '4', '6'},
			 {'6', '5', '4', '7', '9', '8', '2', '1', '3'},
			 {'2', '1', '3', '4', '6', '5', '7', '9', '8'}};
		check(in, out);
	}
	if (0) {
		vector <vector<char>> in =
			{{'.', '.', '.', '.', '.', '.', '.', '.', '.'},
			 {'.', '.', '.', '.', '.', '.', '.', '.', '.'},
			 {'.', '.', '.', '.', '.', '.', '.', '.', '.'},
			 {'.', '.', '.', '.', '.', '.', '.', '.', '.'},
			 {'.', '.', '.', '.', '.', '.', '.', '.', '.'},
			 {'.', '.', '.', '.', '.', '.', '.', '.', '.'},
			 {'.', '.', '.', '.', '.', '.', '.', '.', '.'},
			 {'.', '.', '.', '.', '.', '.', '.', '.', '.'},
			 {'.', '.', '.', '.', '.', '.', '.', '.', '.'}};
		vector <vector<char>> out =
			{{'1', '9', '8', '2', '7', '3', '4', '6', '5'},
			 {'7', '6', '5', '8', '4', '9', '1', '3', '2'},
			 {'4', '3', '2', '5', '1', '6', '8', '7', '9'},
			 {'8', '7', '9', '6', '5', '4', '3', '2', '1'},
			 {'5', '4', '6', '3', '2', '1', '9', '8', '7'},
			 {'3', '2', '1', '9', '8', '7', '6', '5', '4'},
			 {'9', '8', '7', '1', '3', '2', '5', '4', '6'},
			 {'6', '5', '4', '7', '9', '8', '2', '1', '3'},
			 {'2', '1', '3', '4', '6', '5', '7', '9', '8'}};
		check(in, out);
	}


}
