import numpy as np
import os
import matplotlib.pyplot as plt

def runtime(name, size1, size2, step):
    l = np.arange(0,size1,step)
    print l
    l2 = np.arange(1,size2+1,step)
    content = []
    pthread = []
    os.system('rm time.txt')
    for j in l2:
        for i in l:
            a = "(time ./"+name+" "+str(i)+" "+str(j)+") 2> tmp.txt 1>/dev/null && awk \'/real/ {print substr($2,3,5)}\' tmp.txt >> time.txt"
            print a
            os.system(a)
        
        with open('time.txt') as f:
            content = f.readlines()
        content = [x.strip() for x in content]
            
            
        os.system('rm time.txt')
        for i in l:
            a = "(time pthread/"+name+" "+str(i)+" "+str(j)+") 2> tmp.txt 1>/dev/null && awk \'/real/ {print substr($2,3,5)}\' tmp.txt >> time.txt"
            os.system(a)
                
        with open('time.txt') as f:
            pthread = f.readlines()
        pthread = [x.strip() for x in pthread] 
        
        plt.figure(name+str(j))
        plt.title(name+str(j))
        plt.plot(l,content,'b',label="thread.h")
        plt.plot(l,pthread,'r',label="pthread")
        plt.xlabel('Number of threads')
        plt.ylabel('Execution Time')
        plt.legend()
        plt.show()


print "Test1 : "
os.system('time ./Test1')

print "Test2 : "
os.system('time ./Test2')

print "Test11 : "
os.system('time ./Test11')

print "Test12 : "
os.system('time ./Test12')


print "\nLancement du test 21 pour 10**5 threads max\n"
#runtime("Test21",10**4,1,10**2)



print "\nLancement du test 22 pour 10**4 threads max\n"
#runtime("Test22",8*10**2+1,10**2) #pthread plante pour ./Test22 850


print "\nLancement du test 23 pour 10**4 threads max\n"
#runtime("Test23",10**4,10**2)

print "\nLancement du test 31 pour 20 threads max\n"
runtime("Test32",100,10,1)
'''
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
