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

def solve(N, A):
	if len(A) != N-1:
		print >> sys.stderr, 'wrong len'
	M = [1]*N
	for i in xrange(1,N):
		M[i] = M[i-1] * A[i-1]
	return sum(M)

def main():
	T = read_int()
	for _ in xrange(T):
		read_str()
		N = read_int()
		A = read_int_list()
		ans = solve(N, A)
		print '%s' % (ans)

if __name__ == "__main__":
	main()
