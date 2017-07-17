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

def solve(R, C):
	flip = False
	if R > C:
		R, C = C, R
		flip = True
	P = [ (i,i) for i in xrange(R) ]
	c = R
	while c+2 <= C:
		P.append((0, c+1))
		c += 2
	if c < C:
		P.append((0, c))
	if not flip:
		return ("%d\n"%len(P)) + '\n'.join( "%d %d"%p for p in P )
	else:
		return ("%d\n"%len(P)) + '\n'.join( "%d %d"%(p[1],p[0]) for p in P )

def main():
	T = read_int()
	for _ in xrange(T):
		read_str()
		R, C = read_int_list()
		ans = solve(R, C)
		print '%s' % (ans)

if __name__ == "__main__":
	main()
