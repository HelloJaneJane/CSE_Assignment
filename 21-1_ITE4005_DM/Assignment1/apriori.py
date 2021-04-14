import sys
from itertools import combinations

class Apriori:
    def __init__(self, minSupport, transactions):
        self._minSupport = minSupport
        self._transactions = transactions
        self._frequentSet = list()
        self._associationRules = list()

    def run(self):
        # 1-itemset 만들기
        k = 1
        C = list() # candidate
        L = list() # frequent itemset
        for t in transactions:
            for i in t:
                if {i} not in C: C.append({i})
        # print("C1=",C)
        
        # frequent한 게 없거나 더이상 candidate 못 만들 때까지 루프 돌림
        while(True):
            # L 만들기: C에서 minSupport 넘는 걸로 추린 거
            L = list()
            for c in C:
                if round(self.getSupport(c),2)>= self._minSupport and c not in L:
                    L.append(c)
            # print("L",k,"=",L)
            self._frequentSet.append(L)

            # L(k)로 join, prun해서 C(k+1) 만들기        
            C = self.generate(L, k)
            # print("C",k+1,"=",C)
            
            if len(C)==0: break
            k+=1
        # print(self._frequentSet)

        # association rule 만들기
        for itemSets in self._frequentSet:
            for itemSet in itemSets:
                if len(itemSet)>1: self.getAssociationRule(itemSet)

    # 주어진 itemset의 support 값 구하기 (반올림은 나중에)
    def getSupport(self, itemSet):
        # item들이 t에 다 들어있으면 supnum++
        supnum=0
        for t in transactions:
            if len(t)<len(itemSet): pass
            flag=True
            for item in itemSet:
                if item not in t:
                    flag=False
                    break
            if flag: supnum += 1
        return supnum/len(transactions)*100

    # 주어진 L에 대하여 다음 candidate 만들기
    def generate(self, L, k):
        C = list()
        for i1 in range(0,len(L)):
            for i2 in range(i1+1, len(L)):
                # joining: k+1개 되도록 합침
                unioned = L[i1]|L[i2]
                if len(unioned)!=k+1: pass
                # pruning: 하나씩 빼보면서 k개짜리가 L에 없으면 탈락
                flag = True
                for i in unioned:
                    tmp = unioned - {i}
                    if tmp not in L:
                        flag=False
                        break
                if flag and unioned not in C: C.append(unioned)
        return C

    # 주어진 itemset에 대한 association rule 구하기
    def getAssociationRule(self, itemSet):
        # print("itemSet=",itemSet)
        # 가능한 모든 조합
        combi=list()
        for l in range(1, len(itemSet)):
            combi = combi + list(combinations(itemSet,l))
        
        # X->Y (조합 하나 = X+Y)
        for subset in combi:
            X = set(subset)
            Y = itemSet-set(subset)
            # print(X,"->",Y)

            support = round(self.getSupport(itemSet),2)
            confidence = round(support/self.getSupport(X)*100,2)

            self._associationRules.append((X,Y,support,confidence))


if __name__ == '__main__':
    minSupport = float(sys.argv[1])
    inputFilename = sys.argv[2]
    outputFilename = sys.argv[3]

    transactions = list()

    # input
    inputFile = open(inputFilename, 'r')
    while True:
        line = inputFile.readline()
        if not line: break
        transactions.append(line.split('\t'))
        # \n 없애기
        transactions[-1][-1].rstrip('\n')
        # id값 int로 변환
        for i in range(0,len(transactions[-1])):
            transactions[-1][i] = int(transactions[-1][i])
    inputFile.close()

    # Apriori
    apriori = Apriori(minSupport, transactions)
    apriori.run()

    # output
    outputFile = open(outputFilename, 'w')
    for rule in apriori._associationRules:
        outstr = '%s\t%s\t%.2f\t%.2f\n' % (rule[0],rule[1],rule[2],rule[3])
        outputFile.write(outstr)
    outputFile.close()