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

def solve_one(N2, fr, to):
#	print >> sys.stderr, 'Q', N2, fr, to
	bg, ed = fr, fr
	l = 0
#	print >> sys.stderr, (bg, ed)
	while (to-bg)%N2 > ed-bg:
		bg *= 2
		ed = ed*2 + 1
		if bg > N2:
			bg -= N2
			ed -= N2
		l += 1
#		print >> sys.stderr, (bg, ed)
	d = (to-bg) % N2
	c = ""
	for b in xrange(l-1, -1, -1):
		b1 = ((d>>b) & 1)
		b2 = 1 if fr >= N2/2 else 0
		c += 'L' if (b1^b2)==1 else 'R'
		fr = (fr*2+b1) % N2
	return c
			

def solve(N, A):
	return ''.join( solve_one(N*2, A[i-1]-1, A[i]-1) for i in xrange(1,len(A)))

def main():
	T = read_int()
	for _ in xrange(T):
		read_str()
		N, X, K = read_int_list()
		A = read_int_list()
		ans = solve(N, [X] + A)
		print '%d:%s' % (len(ans), ans)

if __name__ == "__main__":
	main()
