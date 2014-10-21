__author__ = 'Eduard'
from numpy import *

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

def f1(a,b,c,ma,mz):
    return b.dot(c) + a.dot(b) + dot(ma,mz).dot(b).dot(a)

def f2(ma,mb):


a = ones(size,dtype=int32)
b = ones(size,dtype=int32)
c = ones(size,dtype=int32)
m = ones(size,dtype=int32)
ma = ones((size,size),dtype=int32)
mb = ones((size,size),dtype=int32)
mc = ones((size,size),dtype=int32)
mm = ones((size,size),dtype=int32)
mz = ones((size,size),dtype=int32)

ma.transpose()
print f1(a,b,c,ma,mz)