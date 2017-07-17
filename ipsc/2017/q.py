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

def solve(N, M):
	cnt = N
	vst = [False]*(N+1)
	for i in xrange(len(M)-1, -1, -1):
		if not vst[M[i]]:
			vst[M[i]] = True
			cnt -= 1
		if cnt == 0:
			return M[i]
	for i in xrange(1, N+1):
		if not vst[i]:
			vst[i] = True
			cnt -= 1
		if cnt == 0:
			return i

def main():
	T = read_int()
	for _ in xrange(T):
		read_str()
		N, E = read_int_list()
		M = [ read_int() for _ in xrange(E) ]
		ans = solve(N, M)
		print '%s' % (ans)

if __name__ == "__main__":
	main()
