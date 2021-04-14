import sys
import pandas as pd
import math
import time

class Dtree:
    def __init__(self, dataDF, classLabel):
        self._dataDF = dataDF
        self._classLabel = classLabel

        self._isLeaf = True
        self._LchildDF = None
        self._RchildDF = None
        self._LchildTree = None
        self._RchildTree = None

        self._splitAttr = None # tuple (attr name, attr value)
        self._maxGain = 0.0
        self._threshold = 0.08
    
    # training data를 partition해서 트리 만드는 재귀함수
    def build(self):
        # maximum gain ratio 되는 attribute 선택 (얘가 splitting attr)
        for key in self._dataDF.keys()[:-1]:
            nowAttrCol = self._dataDF[key]
            values = nowAttrCol.unique() # attr에 해당 값 종류

            # 현재 attr 기준으로 binary partition
            # 그종류인거=Left, 아닌거=Right 로 나눴을때마다 각각 gain ratio
            parentE = self.getEntrophy(self._dataDF)
            for value in values:
                leftDF = self._dataDF.loc[self._dataDF[key]==value]
                rightDF = self._dataDF.loc[self._dataDF[key]!=value]

                # print("value=",value)
                leftE = self.getEntrophy(leftDF)
                leftP = len(leftDF)/len(self._dataDF)
                rightE = self.getEntrophy(rightDF)
                rightP = len(rightDF)/len(self._dataDF)
                
                gain = parentE - (leftE*leftP + rightE*rightP)
                # print("gain=",gain)
                
                # gain값 최대 되도록 갱신
                if gain > self._maxGain:
                    self._isLeaf = False
                    self._splitAttr = (key, value)
                    self._maxGain = gain
                    self._LchildDF = leftDF
                    self._RchildDF = rightDF
                    self._LchildTree = Dtree(leftDF, self._classLabel)
                    self._RchildTree = Dtree(rightDF, self._classLabel)

        # 재귀 멈춤
        if self._isLeaf or self._maxGain<self._threshold: 
            # print("isLeaf",self._dataDF)
            pass
        # attr 기준으로 split
        else:            
            # print(self._splitAttr)
            # print("Left", self._LchildDF)
            # print("Right", self._RchildDF)
            self._LchildTree.build()
            self._RchildTree.build()

    # 주어지는 dataframe의 엔트로피
    def getEntrophy(self, df):
        # {라벨:개수} 딕셔너리 만듦
        labelNum = dict()
        for row in df[self._classLabel]:
            if row not in labelNum: labelNum[row] = 1
            else: labelNum[row] += 1
        # 비율 가중치
        e = 0.0
        for key in labelNum:
            p = float(labelNum[key]) / len(df)
            e -= p * math.log(p, 2)
        return e
    
    # 주어지는 dataframe의 majority voting
    def getDecision(self, df, classLabel):
        return df[classLabel].value_counts().idxmax()

    # test data의 한 row로 트리 탐색
    def predict(self, row):
        # 맨 밑이면 decision 내리기
        if self._isLeaf:
            return self.getDecision(self._dataDF, self._classLabel)
        # 밑에 더 있으면 child tree에서 재귀
        else:
            key = self._splitAttr[0]
            value = self._splitAttr[1]
            # 그종류인거=Left, 아닌거=Right
            if row[key]==value: return self._LchildTree.predict(row)
            else: return self._RchildTree.predict(row)


if __name__ == '__main__':
    startTime = time.time()

    trainingFilename = sys.argv[1]
    testFilename = sys.argv[2]
    outputFilename = sys.argv[3]

    # input
    trainingDF = pd.read_csv(trainingFilename, sep='\t')
    testDF = pd.read_csv(testFilename, sep='\t')    
    
    classLabel = trainingDF.keys()[-1]

    # build decision tree
    dt = Dtree(trainingDF, classLabel)
    dt.build()

    # classify
    decisions = list()
    for i in range(len(testDF)):
        decisions.append(dt.predict(testDF.iloc[i]))
    testDF[classLabel] = decisions

    # output
    testDF.to_csv(outputFilename, index=False, sep='\t')

    # test
    print("threshold: ",dt._threshold)
    print("time: ",time.time()-startTime)
    if len(sys.argv)>4:
        answerDF = pd.read_csv(sys.argv[4], sep='\t')
        result = (answerDF[classLabel]==decisions)
        print('Accuracy:', result.value_counts()[True], "/", len(result))
    