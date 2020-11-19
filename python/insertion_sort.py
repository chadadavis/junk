#!/usr/bin/python

def insertion_sort(A):
	for j in range(1, len(A)):
		key = A[j]
		i = j - 1
		while i >= 0 and A[i] > key:
			A[i + 1] = A[i]
			i = i - 1
		A[i + 1] = key
# insertion_sort

# Test
#A = [4,1,3,3,65,1,66,352,6234,2,3,55,62,6,346,622,3]
#insertion_sort(A)
#print A


