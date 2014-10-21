__author__ = 'Eduard'
from numpy import *
from multiprocessing import Process, Lock

# ---------------------------------------------------------------------------------
# --                   Laboratory Work #8
# --         Lab7.Threads in Python programming language
# --      Name      Pashchuk Eduard Fedorovich
# --      Group     IP-22
# --      Date      19.10.2014
# --      F1        d = (B*C) + (A*B) +(C*(B*(MA*MZ)))
# --      F2        MC = SORT(TRANS(MA)*MB)
# --      F3        D = SORT(A + M)*TRANS(MC*MM)
# ---------------------------------------------------------------------------------
size = 10

def f1(a,b,c,ma,mz,l):
    d = b.dot(c) + a.dot(b) + dot(ma,mz).dot(b).dot(a)
    l.acquire()
    print d
    l.release()

def f2(ma,mb,l):
    mc = sort(dot(mb,ma.transpose()))[:,::-1]
    l.acquire()
    print mc
    l.release()

def f3(a,m,mc,mm,l):
    d = sort(a + m).dot(dot(mc,mm).transpose())
    l.acquire()
    print d
    l.release()

if __name__ == '__main__':
    a = ones(size,dtype=int32)
    b = ones(size,dtype=int32)
    c = ones(size,dtype=int32)
    m = ones(size,dtype=int32)
    ma = ones((size,size),dtype=int32)
    mb = ones((size,size),dtype=int32)
    mc = ones((size,size),dtype=int32)
    mm = ones((size,size),dtype=int32)
    mz = ones((size,size),dtype=int32)
    lock = Lock()
    