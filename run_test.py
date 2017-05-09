import numpy as np
import os
import matplotlib.pyplot as plt

def runtime(name, size1, size2, step, step2):
    l = np.arange(0,size1,step)
    l2 = np.arange(0,size2,step2)
    content = []
    pthread = []
    for j in l2:
        os.system('rm time.txt')
        for i in l:
            a = "(time ./"+name+" "+str(i)+" "+str(j)+") 2> tmp.txt 1>/dev/null && awk \'/real/ {print substr($2,3,5)}\' tmp.txt >> time.txt"
            os.system(a)
        
        with open('time.txt') as f:
            content = f.readlines()
        content = [x.strip() for x in content]
            
        os.system('rm time.txt')
        for i in l:
            a = "(time ./pthread/"+name+" "+str(i)+" "+str(j)+") 2> tmp.txt 1>/dev/null && awk \'/real/ {print substr($2,3,5)}\' tmp.txt >> time.txt"
            print a
            os.system(a)
                
        with open('time.txt') as f:
            pthread = f.readlines()
        pthread = [x.strip() for x in pthread]
        
        plt.figure(name+"_"+str(j))
        plt.title(name+"_"+str(j))
        plt.plot(l,content,'b',label="thread.h")
        plt.plot(l,pthread,'r',label="pthread")
        plt.xlabel('Number of threads')
        plt.ylabel('Execution Time')
        plt.legend()
        plt.show()


print "\nTest1 (thread.h): "
os.system('time ./Test1 1>/dev/null')
print "\nTest1 (pthread): "
os.system('time ./pthread/Test1 1>/dev/null')

print "\nTest2 (thread.h): "
os.system('time ./Test2 1>/dev/null')
print "\nTest2 (pthread): "
os.system('time ./pthread/Test2 1>/dev/null')

print "\nTest11 (thread.h): "
os.system('time ./Test11 1>/dev/null')
print "\nTest11 (pthread): "
os.system('time ./pthread/Test11 1>/dev/null')

print "\nTest12 (thread.h): "
os.system('time ./Test12 1>/dev/null')
print "\nTest12 (pthread): "
os.system('time ./pthread/Test12 1>/dev/null')

print "\nLancement du test 21 pour 10**5 threads max\n"
runtime("Test21",10**4,1,10**2,10**2)

print "\nLancement du test 22 pour 10**3 threads max\n"
runtime("Test22",8*10**2,1,10**2,10**2) #pthread plante pour ./Test22 850

print "\nLancement du test 23 pour 10**4 threads max\n"
runtime("Test23",10**4,10**2,10**2)

print "\nLancement du test 31 pour 20 threads max\n"
runtime("Test31",10**3,10**2,20,20)

print "\nLancement du test 32 pour 20 threads max\n"
runtime("Test32",800,1000000,10,200000)

print "\nLancement du test 51 pour 15 threads max\n"
l = np.arange(0,20,1)
l2 = np.arange(0,16,1)
content = []
pthread = []
os.system('rm time.txt')
for i in l:
    a = "(time ./Test51 "+str(i)+") 2> tmp.txt 1>/dev/null && awk \'/real/ {print substr($2,3,5)}\' tmp.txt >> time.txt"
    os.system(a)

with open('time.txt') as f:
    content = f.readlines()
content = [x.strip() for x in content]
print len(content)
    
os.system('rm time.txt')
for i in l2:
    a = "(time ./pthread/Test51 "+str(i)+") 2> tmp.txt 1>/dev/null && awk \'/real/ {print substr($2,3,5)}\' tmp.txt >> time.txt"
    os.system(a)

with open('time.txt') as f:
    pthread = f.readlines()
pthread = [x.strip() for x in pthread]
print len(pthread)

plt.figure("Test51")
plt.title("Test51")
plt.plot(l,content,'b',label="thread.h")
plt.plot(l2,pthread,'r',label="pthread")
plt.xlabel('Number of threads')
plt.ylabel('Execution Time')
plt.legend()
plt.show()

'''
start = timeit.default_timer()
subprocess.call(['./Test61'])
stop = timeit.default_timer()
print "Test61 : ",stop - start,"\n"
'''
