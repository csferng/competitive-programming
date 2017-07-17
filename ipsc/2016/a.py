import collections
import itertools
import sys

read_str = lambda : sys.stdin.readline().strip()
read_str_list = lambda : sys.stdin.readline().strip().split()
read_int = lambda : int(read_str())
read_int_list = lambda : map(int, read_str_list())
read_float = lambda : float(read_str())
read_float_list = lambda : map(float, read_str_list())

def solve(A, L):
	for (i,a1) in enumerate(A):
		for j in xrange(len(A)):
			if j == i:
				continue
			l = len(A[j])
			for k in xrange(l):
				if A[j][k:] == A[i][:l-k]:
					A[j] += A[i][l-k:]
					A[i] = ""
					break
			if A[i] == "":
				break
	S = ''.join(A)
	if L >= len(S):
		return S + 'A'*(L-len(S))
	else:
		print >> sys.stderr, "ERROR", L, len(S), S

def main():
	L = 42 if len(sys.argv) < 2 else int(sys.argv[1])
	T = read_int()
	for _ in xrange(T):
		print >> sys.stderr, _
		read_str()
		A = read_str_list()
		ans = solve(A, L)
		print ans

if __name__ == "__main__":
	main()
