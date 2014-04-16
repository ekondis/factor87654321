#!/usr/bin/python

import time

start = time.time()

print "Searching for two factors in range [1,15000] whose multiple is 87654321"
for i in range(1, 15001):
	for j in range(i+1, 15001):
		if i*j==87654321:
			print "Found! ",str(i)," ",str(j)
			break

end = time.time()

print "kernel execution time = " + ('%.2f' % ((end-start)*1000.0)) + " msecs";

