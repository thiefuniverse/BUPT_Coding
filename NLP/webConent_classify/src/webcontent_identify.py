from classify_network import NeuralNetWork
import classify_network
from handleWebpage import DataFormal, DataCollect
import numpy as np

def judgeRight(outAnswer,realAnswer):
    for m in range(len(outAnswer)):
        if abs(abs(outAnswer[m])-abs(realAnswer[m]))>0.84:
            return False
    return True

nn = NeuralNetWork([500,35, 7], classify_network.tanh,
                   classify_network.tanh_deriv)
dirControlor = DataCollect()
dataControlor = DataFormal(dirControlor.allWords,
    dirControlor.getTrainData(), dirControlor.getTestData())
inputData, answerData = dataControlor.getRandomTrains(500)
testData,testAnswer=dataControlor.getRandomTest(500)

nn.train(np.array(inputData), np.array(answerData), 0.1, 10000)
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