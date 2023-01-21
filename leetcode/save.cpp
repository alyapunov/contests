#include <cstddef>
#include <cstdint>

struct CircleHasher {
	uint64_t R = 1009;
	uint64_t Q = INT32_MAX;
	size_t L;
	uint64_t K;
	CircleHasher(size_t l) {
		L = l;
		l--;
		K = 1;
		uint64_t p = R;
		while (l) {
			if (l & 1)
				K = K * p % Q;
			p = p * p % Q;
			l >>= 1;
		}
		K = Q - K;
	}

	uint64_t get(const char *str)
	{
		uint64_t h = 0;
		for (size_t i = 0; i < L; i++)
			h = (h * R + (uint8_t)(*str++)) % Q;
		return h;
	}

	uint64_t next(uint64_t h, const char *str)
	{
		return ((h + (uint8_t)str[0] * K) * R + (uint8_t)str[L]) % Q;
	}
};
