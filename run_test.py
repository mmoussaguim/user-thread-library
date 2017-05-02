import subprocess
import timeit
import os

print "Test1 : "
os.system('time ./Test1')

print "Test2 : "
os.system('time ./Test2')

print "Test11 : "
os.system('time ./Test11')

print "Test12 : "
os.system('time ./Test12')


print "\nLancement du test 21 pour 20 threads max\n"
for i in range (21):
    pow = 100*i
    print "Test21 avec ",pow,"threads :"
    os.system('time ./Test21 '+str(pow))
    
'''
print "\nLancement du test 22 pour 20 threads max\n"
for i in range (21):
    start = timeit.default_timer()
    pow = 10**i
    os.system('./Test22 pow')
    #subprocess.call(['./Test22','pow'],stdout=subprocess.PIPE)
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


start = timeit.default_timer()
subprocess.call(['./Test61'])
stop = timeit.default_timer()
print "Test61 : ",stop - start,"\n"
'''
