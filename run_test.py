import numpy as np
import os
import matplotlib.pyplot as plt

def runtime(name, size1, size2, step, step2):
    l = np.arange(0,size1,step)
    l2 = np.arange(0,size2,step2)
    content = []
    content_mem = []
    pthread = []
    pthread_mem = []
    for j in l2:
        os.system('rm time.txt')
        os.system('rm mem_use.txt')
        for i in l:
            a = "(time ./build/"+name+" "+str(i)+" "+str(j)+") 2> tmp.txt 1>/dev/null && awk \'/real/ {print substr($2,3,5)}\' tmp.txt >> time.txt"
            os.system(a)
        
        with open('time.txt') as f:
            content = f.readlines()
        content = [x.strip() for x in content]

        with open('mem_use.txt') as f:
            content_mem = f.readlines()
        content_mem = [x.strip() for x in content_mem]
           
        os.system('rm time.txt')
        os.system('rm mem_use.txt')
        for i in l:
            a = "(time ./pthread/"+name+" "+str(i)+" "+str(j)+") 2> tmp.txt 1>/dev/null && awk \'/real/ {print substr($2,3,5)}\' tmp.txt >> time.txt"
            os.system(a)
                
        with open('time.txt') as f:
            pthread = f.readlines()
        pthread = [x.strip() for x in pthread]

        with open('mem_use.txt') as f:
            pthread_mem = f.readlines()
        pthread_mem = [x.strip() for x in pthread_mem]

        plt.figure(name+"_runtime_"+str(j))
        plt.title(name+"_runtime_"+str(j))
        plt.plot(l,content,'b',label="thread.h")
        plt.plot(l,pthread,'r',label="pthread")
        plt.xlabel('Number of threads')
        plt.ylabel('Execution Time')
        plt.legend()

        plt.figure(name+"_mem_"+str(j))
        plt.title(name+"_mem_"+str(j))
        plt.plot(l,content_mem,'b',label="thread.h")
        plt.plot(l,pthread_mem,'r',label="pthread")
        plt.xlabel('Number of threads')
        plt.ylabel('Memory usage')
        plt.legend()
        plt.show()


print "\nTest1 (thread.h): "
os.system('time ./build/01-main 1>/dev/null')
print "\nTest1 (pthread): "
os.system('time ./pthread/01-main 1>/dev/null')

print "\nTest2 (thread.h): "
os.system('time ./build/02-switch 1>/dev/null')
print "\nTest2 (pthread): "
os.system('time ./pthread/02-switch 1>/dev/null')

print "\nTest11 (thread.h): "
os.system('time ./build/11-join 1>/dev/null')
print "\nTest11 (pthread): "
os.system('time ./pthread/11-join 1>/dev/null')

print "\nTest12 (thread.h): "
os.system('time ./build/12-join-main 1>/dev/null')
print "\nTest12 (pthread): "
os.system('time ./pthread/12-join-main 1>/dev/null')
'''
print "\nLancement du test 21 pour 10**5 threads max\n"
runtime("21-create-many",10**4,1,10**2,10**2)

print "\nLancement du test 22 pour 10**3 threads max\n"
runtime("22-create-many-recursive",8*10**2,1,10**2,10**2) #pthread plante pour ./Test22 850

print "\nLancement du test 23 pour 10**4 threads max\n"
runtime("23-create-many-once",10**4,1,10**2,10**2)

print "\nLancement du test 31 pour 20 threads max\n"
runtime("31-switch-many",10**3,80,20,20)

print "\nLancement du test 32 pour 20 threads max\n"
runtime("32-switch-many-join",800,600000,10,200000)

runtime("61-mutex",801,1,10,1)

print "\nLancement du test 51 pour 15 threads max\n"
l = np.arange(0,20,1)
l2 = np.arange(0,16,1)
content = []
content_mem = []
pthread = []
pthread_mem = []
os.system('rm time.txt')
os.system('rm mem_use.txt')
for i in l:
    a = "(time ./build/51-fibonacci "+str(i)+") 2> tmp.txt 1>/dev/null && awk \'/real/ {print substr($2,3,5)}\' tmp.txt >> time.txt"
    os.system(a)

with open('time.txt') as f:
    content = f.readlines()
content = [x.strip() for x in content]


with open('mem_use.txt') as f:
    content_mem = f.readlines()
content_mem = [x.strip() for x in content_mem]
    
os.system('rm time.txt')
os.system('rm mem_use.txt')
for i in l2:
    a = "(time ./pthread/51-fibonacci "+str(i)+") 2> tmp.txt 1>/dev/null && awk \'/real/ {print substr($2,3,5)}\' tmp.txt >> time.txt"
    os.system(a)

with open('time.txt') as f:
    pthread = f.readlines()
pthread = [x.strip() for x in pthread]

with open('mem_use.txt') as f:
    pthread_mem = f.readlines()
pthread_mem = [x.strip() for x in pthread_mem]


plt.figure("51-fibonacci_mem")
plt.title("51-fibonacci_mem")
plt.plot(l,content,'b',label="thread.h")
plt.plot(l2,pthread,'r',label="pthread")
plt.xlabel('Number of threads')
plt.ylabel('Execution Time')
plt.legend()

plt.figure("51-fibonacci_runtime")
plt.title("51-fibonacci_runtime")
plt.plot(l,content_mem,'b',label="thread.h")
plt.plot(l2,pthread_mem,'r',label="pthread")
plt.xlabel('Number of threads')
plt.ylabel('Memory usage')
plt.legend()

plt.show()

'''
