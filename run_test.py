import subprocess
import timeit

start = timeit.default_timer()
subprocess.call(['./Test1'],stdout=subprocess.PIPE)
stop = timeit.default_timer()
print "Test1 : ",stop - start

start = timeit.default_timer()
subprocess.call(['./Test2'],stdout=subprocess.PIPE)
stop = timeit.default_timer()
print "Test2 : ",stop - start

start = timeit.default_timer()
subprocess.call(['./Test11'],stdout=subprocess.PIPE)
stop = timeit.default_timer()
print "Test11 : ",stop - start

start = timeit.default_timer()
subprocess.call(['./Test12'],stdout=subprocess.PIPE)
stop = timeit.default_timer()
print "Test12 : ",stop - start

print "\nLancement du test 21 pour 20 threads max\n"
for i in range (21):
    start = timeit.default_timer()
    pow = 10**i
    subprocess.call(['./Test21','pow'],stdout=subprocess.PIPE)
    stop = timeit.default_timer()
    print "Test21 avec 10**",i,"threads :",stop - start

print "\nLancement du test 22 pour 20 threads max\n"
for i in range (21):
    start = timeit.default_timer()
    pow = 10**i
    subprocess.call(['./Test22','pow'],stdout=subprocess.PIPE)
    stop = timeit.default_timer()
    print "Test22 : ",stop - start

print "\nLancement du test 23 pour 20 threads max\n"
for i in range (21):
    start = timeit.default_timer()
    pow = 10**i
    subprocess.call(['./Test23','pow'],stdout=subprocess.PIPE)
    stop = timeit.default_timer()
    print "Test23 : ",stop - start

print "\nLancement du test 31 pour 20 threads max\n"
for i in range (21):
    start = timeit.default_timer()
    pow = 10**i
    subprocess.call(['./Test31','pow'],stdout=subprocess.PIPE)
    stop = timeit.default_timer()
    print "Test31 : ",stop - start

print "\nLancement du test 32 pour 20 threads max\n"
for i in range (21):  
    start = timeit.default_timer()
    pow = 10**i
    subprocess.call(['./Test32','pow'],stdout=subprocess.PIPE)
    stop = timeit.default_timer()
    print "Test32 : ",stop - start

print "\nLancement du test 51 pour 20 threads max\n"
for i in range (21):    
    start = timeit.default_timer()
    pow = 10**i
    subprocess.call(['./Test51','pow'],stdout=subprocess.PIPE)
    stop = timeit.default_timer()
    print "Test51 : ",stop - start

'''
start = timeit.default_timer()
subprocess.call(['./Test61'])
stop = timeit.default_timer()
print "Test61 : ",stop - start,"\n"
'''
