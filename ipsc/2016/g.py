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

def solve(S):
	dgt = ''.join(re.findall(r'[0-9]+', S))
	while len(dgt) > 1 and dgt[0] == '0':
		dgt = dgt[1:]
	return dgt

def main():
	T = read_int()
	for _ in xrange(T):
		read_str()
		S = read_str()
		ans = solve(S)
		print '%s' % (ans)

if __name__ == "__main__":
	main()
