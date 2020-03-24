from z3 import *
import time

# Number of Queens
print("N: ")
N = int(input())

start = time.time()

### Variables

# 1~N의 Column에 있는 각 Queen의 row 위치
X = [Int("x_%s" % (i)) for i in range(1,N+1) ]


### Constraints

# i번 column에 있는 퀸은 k번 row에 있다는 뜻
# 가능한 i : 1~N, k : 1~N
domain = [And(1<=X[i], X[i]<=N) for i in range(0,N) ]

# left column의 row번호와 right column의 row번호가 같으면 가로로 공격 가능
# -> left<right, X[left]!=X[right] 이어야 함
rowConst = [Implies(l<r, X[l]!=X[r]) for l in range(0,N) for r in range(0,N)]

# left column의 row번호와 right column의 row번호의 차이가 left~right 차이와 같으면 대각선으로 공격 가능
# -> left<right, (right-left)!=abs(X[left]-X[right]) 이어야 함
digConst = [Implies(l<r, And((r-l)!=X[r]-X[l],(r-l)!=X[l]-X[r])) for l in range(0,N) for r in range(0,N)]

eight_queens_c = domain + rowConst + digConst

s = Solver()
s.add(eight_queens_c)

if s.check() == sat:
    m = s.model()
    r = [m.evaluate(X[i]) for i in range(0,N)]
    print(r)
else:
    print("fail")

print("elapsed time: ", time.time() - start, " sec")
