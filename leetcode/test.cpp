#include <array>
#include <vector>
#include <iostream>
#include <cassert>
#include <cstddef>

using namespace std;

struct A {
	A() {
		int32_t by_type1[9][6];
		int32_t by_type2[9][6];
		int32_t by_type3[9][8];

		constexpr int32_t gr_jumps[3][2] = {{1, 2}, {-1,+1}, {-2, -1}};

		for (int32_t i = 0; i < 3; i++) {
			for (int32_t j = 0; j < 3; j++) {
				for (int32_t k = 0; k < 2; k++) {
					for (int32_t l = 0; l < 3; l++) {
						by_type1[i*3+j][k*3+l] = gr_jumps[i][k]*3+l-j;
						by_type2[i*3+j][k*3+l] = by_type1[i*3+j][k*3+l] * 9;
					}
				}

			}
		}
		for (int32_t i = 0; i < 3; i++) {
			for (int32_t j = 0; j < 3; j++) {
				uint32_t k = 0;
				for (int32_t l = 0; l < 3; l++) {
					for (int32_t m = 0; m < 3; m++) {
						if (i == l && j == m)
							continue;
						by_type3[i*3+j][k++] = (l - i) * 9 + (m - j);
					}
				}
			}
		}
		std::cout << "// Depend on column.\n";
		std::cout << "static constexpr int32_t by_col[9][6] = {\n";
		for (size_t i = 0; i < 9; i++) {
			for (size_t j = 0; j < 6; j++) {
				std::cout << (j ? ", " : "\t{") << by_type1[i][j];
			}
			std::cout << "}" << (i == 8 ? "" : ",") << "\n";
		}
		std::cout << "};\n";
		std::cout << "// Depend on row.\n";
		std::cout << "static constexpr int32_t by_row[9][6] = {\n";
		for (size_t i = 0; i < 9; i++) {
			for (size_t j = 0; j < 6; j++) {
				std::cout << (j ? ", " : "\t{") << by_type2[i][j];
			}
			std::cout << "}" << (i == 8 ? "" : ",") << "\n";
		}
		std::cout << "};\n";
		std::cout << "// Depend on position in group.\n";
		std::cout << "static constexpr int32_t by_pos[9][8] = {\n";
		for (size_t i = 0; i < 9; i++) {
			for (size_t j = 0; j < 8; j++) {
				std::cout << (j ? ", " : "\t{") << by_type3[i][j];
			}
			std::cout << "}" << (i == 8 ? "" : ",") << "\n";
		}
		std::cout << "};\n";

	}
} /*a*/;

struct Jumps {
	// Depend on column.
	static constexpr int32_t by_col[9][6] = {
		{3, 4, 5, 6, 7, 8},
		{2, 3, 4, 5, 6, 7},
		{1, 2, 3, 4, 5, 6},
		{-3, -2, -1, 3, 4, 5},
		{-4, -3, -2, 2, 3, 4},
		{-5, -4, -3, 1, 2, 3},
		{-6, -5, -4, -3, -2, -1},
		{-7, -6, -5, -4, -3, -2},
		{-8, -7, -6, -5, -4, -3}
	};
	// Depend on row.
	static constexpr int32_t by_row[9][6] = {
		{27, 36, 45, 54, 63, 72},
		{18, 27, 36, 45, 54, 63},
		{9, 18, 27, 36, 45, 54},
		{-27, -18, -9, 27, 36, 45},
		{-36, -27, -18, 18, 27, 36},
		{-45, -36, -27, 9, 18, 27},
		{-54, -45, -36, -27, -18, -9},
		{-63, -54, -45, -36, -27, -18},
		{-72, -63, -54, -45, -36, -27}
	};
	// Depend on position in group.
	static constexpr int32_t by_pos[9][8] = {
		{1, 2, 9, 10, 11, 18, 19, 20},
		{-1, 1, 8, 9, 10, 17, 18, 19},
		{-2, -1, 7, 8, 9, 16, 17, 18},
		{-9, -8, -7, 1, 2, 9, 10, 11},
		{-10, -9, -8, -1, 1, 8, 9, 10},
		{-11, -10, -9, -2, -1, 7, 8, 9},
		{-18, -17, -16, -9, -8, -7, 1, 2},
		{-19, -18, -17, -10, -9, -8, -1, 1},
		{-20, -19, -18, -11, -10, -9, -2, -1}
	};
};

template <size_t N>
constexpr size_t popcount_one = (N & 1) + (N ? popcount_one<N / 2> : 0);

template<size_t N, size_t... I>
constexpr std::array<size_t, N> create_popcount_impl(std::index_sequence<I...>) {
	return std::array<size_t, N>{ {popcount_one<I>...} };
}

template<std::size_t N>
constexpr std::array<size_t, N> create_popcount() {
	return create_popcount_impl<N>(std::make_index_sequence<N>{});
}

static constexpr std::array<size_t, 512> popcount = create_popcount<512>();

static constexpr size_t list_by_count[10] = {0, 1, 2, 2, 3, 3, 3, 3, 3, 3};

struct Field {
	uint8_t row;
	uint8_t col;
	uint8_t grp;
	uint8_t pos;
	uint16_t mask = 0;
	uint8_t me;
	uint8_t next;
	uint8_t prev;
	constexpr Field(uint8_t i) :
		row(i / 9), col(i % 9),
		grp(row / 3 * 3 + col / 3),
		pos(row % 3 * 3 + col % 3),
		me(i),
		next(i < 80 ? i + 1 : i == 80 ? 84 : i == 84 ? 0 : i),
		prev(i == 0 ? 84 : i < 81 ? i - 1 : i == 84 ? 80 : i)
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
	std::array<Field, 9 * 9 + 5> fields = create_fields<9 * 9 + 5>();
	uint16_t possible_mask_row[9] = {511,511,511,511,511,511,511,511,511};
	uint16_t possible_mask_col[9] = {511,511,511,511,511,511,511,511,511};
	uint16_t possible_mask_grp[9] = {511,511,511,511,511,511,511,511,511};

	constexpr Board() {
	}

	Field* operator[](size_t i) { return &fields[i * 9]; }

	void assertList(Field &list)
	{
		assert(list.me >= 81 && list.me < 86);
		assert(list.next == list.me || list.next < 81);
		assert(list.prev == list.me || list.prev < 81);
		assert((list.next == list.me) == (list.prev == list.me));
		assert(list.me == (&list - &fields[0]));
		assert(fields[list.next].me == list.next);
		assert(fields[list.prev].me == list.prev);
	}
	void assertItem(Field &item)
	{
		assert(item.me < 81);
		assert(item.next < 86);
		assert(item.prev < 86);
		assert((item.next == item.me) == (item.prev == item.me));
		assert(item.me == (&item - &fields[0]));
		assert(fields[item.next].me == item.next);
		assert(fields[item.prev].me == item.prev);
	}

	void listAdd(Field &list, Field &item)
	{
		assertList(list);
		assertItem(item);
		Field &next = fields[list.next];
		item.prev = list.me;
		item.next = next.me;
		list.next = item.me;
		next.prev = item.me;
		assertList(list);
		assertItem(item);
	}
	void listDel(Field &item)
	{
		assertItem(item);
		Field &prev = fields[item.prev];
		Field &next = fields[item.next];
		prev.next = next.me;
		next.prev = prev.me;
		assertItem(item);
	}
	void listClr(Field &list)
	{
		assertList(list);
		list.next = list.prev = list.me;
		assertList(list);
	}
	bool listIsEmpty(Field &list)
	{
		assertList(list);
		return list.next == list.me;
	}
	void check(Field &f)
	{
		Field *lists = &fields[81];
		if (f.mask)
			return;
		listDel(f);
		auto possible_mask = possible_mask_row[f.row] &
				     possible_mask_col[f.col] &
				     possible_mask_grp[f.grp];
		listAdd(lists[list_by_count[popcount[possible_mask]]], f);
	}
	void choose(Field &f, uint16_t mask)
	{
		Field& stack = fields[85];
		assert(possible_mask_row[f.row] &
		       possible_mask_col[f.col] &
		       possible_mask_grp[f.grp] & mask);
		if (f.mask) {
			assert(!(possible_mask_row[f.row] & f.mask));
			assert(!(possible_mask_col[f.col] & f.mask));
			assert(!(possible_mask_grp[f.grp] & f.mask));
		}

		possible_mask_row[f.row] ^= mask ^ f.mask;
		possible_mask_col[f.col] ^= mask ^ f.mask;
		possible_mask_grp[f.grp] ^= mask ^ f.mask;
		f.mask = mask;
		listDel(f);
		listAdd(stack, f);
		for (int32_t shift : Jumps::by_col[f.col])
			check(fields[f.me + shift]);
		for (int32_t shift : Jumps::by_row[f.row])
			check(fields[f.me + shift]);
		for (int32_t shift : Jumps::by_pos[f.pos])
			check(fields[f.me + shift]);
	}
	void prechoose(Field &f, uint16_t mask)
	{
		choose(f, mask);
	}
	void choose(Field &f)
	{
		auto possible_mask = possible_mask_row[f.row] &
				     possible_mask_col[f.col] &
				     possible_mask_grp[f.grp];
		assert(possible_mask != 0);
		assert(f.mask == 0);
		uint16_t mask = ((possible_mask ^ (possible_mask - 1)) + 1) / 2;
		choose(f, mask);
	}
	bool rechoose(Field &f)
	{
		auto possible_mask = possible_mask_row[f.row] &
				     possible_mask_col[f.col] &
				     possible_mask_grp[f.grp];
		assert(f.mask != 0);

		possible_mask &= ~(f.mask | (f.mask - 1));
		if (!possible_mask) {
			assert(!(possible_mask_row[f.row] & f.mask));
			assert(!(possible_mask_col[f.col] & f.mask));
			assert(!(possible_mask_grp[f.grp] & f.mask));
			possible_mask_row[f.row] ^= f.mask;
			possible_mask_col[f.col] ^= f.mask;
			possible_mask_grp[f.grp] ^= f.mask;
			f.mask = 0;
			check(f);
			return false;
		}
		uint16_t mask = ((possible_mask ^ (possible_mask - 1)) + 1) / 2;
		choose(f, mask);
		return true;
	}
	void find()
	{
		Field *lists = &fields[81];
		Field& stack = fields[85];
		listClr(stack);
		while (true) {
			prn();
			if (!listIsEmpty(lists[0])) {
				while (true) {
					assert(!listIsEmpty(stack));
					Field &chosen = fields[stack.next];
					std::cout << "rechoose " << int(chosen.row) << " " << int(chosen.col) << std::endl;
					if (rechoose(chosen))
						break;
					else
						prn();
				}
				continue;
			}
			if (!listIsEmpty(lists[1])) {
				std::cout << "choose " << int(fields[lists[1].next].row) << " " << int(fields[lists[1].next].col) << std::endl;
				choose(fields[lists[1].next]);
				continue;
			}
			if (!listIsEmpty(lists[2])) {
				std::cout << "choose " << int(fields[lists[2].next].row) << " " << int(fields[lists[2].next].col) << std::endl;
				choose(fields[lists[2].next]);
				continue;
			}
			if (!listIsEmpty(lists[3])) {
				std::cout << "choose " << int(fields[lists[3].next].row) << " " << int(fields[lists[3].next].col) << std::endl;
				choose(fields[lists[3].next]);
				continue;
			}
			return;
		}
	}
	void prn() const
	{
		for (size_t i = 0; i < 81; i++) {
			if (fields[i].mask == 0)
				std::cout << ".";
			else
				std::cout << __builtin_ctz(fields[i].mask) + 1;
			if (i % 9 == 8)
				std::cout << "\n";
			else
				std::cout << " ";
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
				b.prechoose(b[i][j], 1 << (in[i][j] - '1'));
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
	{
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
	{
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
	{
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
