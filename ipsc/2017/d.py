import collections
import itertools
import re
import sys

read_str = lambda : sys.stdin.readline().strip()
read_str_list = lambda : sys.stdin.readline().strip().split()
read_int = lambda : int(read_str())
read_int_list = lambda : map(int, read_str_list())
read_float = lambda : float(read_str())
read_float_list = lambda : map(float, read_str_list())

def beauty(n):
	s = str(n)
	return len(s) == len(set(s))

def solve_easy(N):
	if beauty(N):
		ans = [N]
	else:
		for i in xrange(1, N/2+1):
			if beauty(i) and beauty(N-i):
				ans = [i, N-i]
				break
	return ' '.join(map(str, [len(ans)]+ans))

def enumbeauty(n):
	s = str(n)
	l = len(s)
	while True:
		ok = True
		for (i,c) in enumerate(s):
			if c in s[:i]:
				n -= 10**(l-i-1)
				s = str(n)
				l = len(s)
				ok = False
				break
		if ok:
			yield n
			n -= 1
			s = str(n)
			l = len(s)

def solve_hard(N):
	if beauty(N):
		ans = [N]
	else:
		t = 0
		it = enumbeauty(N)
		while t < 100000:
			m = it.next()
			if beauty(N-m):
				ans = [m, N-m]
				break
			t += 1
		if t == 10000:
			ans = [-1]
	return ' '.join(map(str, [len(ans)]+ans))


def main():
	T = read_int()
	for _ in xrange(T):
		read_str()
		N = read_int()
		ans = solve_hard(N)
		print '%s' % (ans)

if __name__ == "__main__":
	main()
