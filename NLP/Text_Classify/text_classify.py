from classify_network import NeuralNetWork
import classify_network
from handleWebpage import DataFormal, DataCollect
import numpy as np

def judgeRight(outAnswer,realAnswer):
    for m in range(len(outAnswer)):
        if abs(abs(outAnswer[m])-abs(realAnswer[m]))>0.50:
            return False
    return True

vectorlen=500
hideLen=45
nn = NeuralNetWork([vectorlen,hideLen,hideLen,5], classify_network.tanh,
                   classify_network.tanh_deriv)
trainControlor = DataCollect("./20_newsgroups/")
testControlor=DataCollect("./mini_newsgroups/")

dataControlor = DataFormal(trainControlor.allWords,trainControlor.getRealData(),testControlor.getRealData())
inputData, answerData = dataControlor.getRandomTrains(vectorlen)
testData,testAnswer=dataControlor.getRandomTest(vectorlen)

nn.train(np.array(inputData), np.array(answerData), 0.01, 1000000)
#gnn.saveWeights()
print(nn.weights)

right=1.0
rightNum=0
for i in range(len(testData)):
    classResult=nn.classify(testData[i])
    print("classify result: ", end="")
    print(classResult,end="")
    print("  answer:  ")
    print(testAnswer[i])
    ri=judgeRight(classResult,testAnswer[i])
    if ri:
        rightNum+=1
    print(ri)

right=rightNum/(len(testData))
print(right)
print(dataControlor.otherData)