

def gcd(a, b) :
	if b > a : a, b = b, a
	x, y, k, l, u, v = a, b, 0, 1, 1, 0
	while x % y :
		q = x / y
		x, y, u, v, k, l = y, x % y, k, l, u - k * q, v - l * q
		
	print "gcd:", y, "=", k, "*", a, "+", l, "*", b, "=", a * k + l * b

def exp(a,b) :
	x = a
	y = 1
	z = b
	while 1 :
		if z == 0 : return y
		r = z % 2
		z = z / 2
		if r == 1 : y = x * y
		x = x ** 2





	
