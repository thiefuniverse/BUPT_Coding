from bs4 import BeautifulSoup as bs
from collections import defaultdict
import os
import re
import heapq
import numpy as np
from collections import OrderedDict

answerTable = {'student': [1, 0, 0,0,0,0,0], 'other': [0,1,0,0,0,0,0], 'staff': [0, 0,1,0,0,0, 0], 'faculty': [
    0, 0,0,1, 0,0,0],
               'department': [0,0,0,0,1, 0, 0], 'project': [0,0,0,0,0,1,0], 'course': [0,0,0,0,0,0,1]}

class OnePage:
    ''' handle one page'''

    def __init__(self, path):
        self.kind = ""

        self.path = path
        self.getKind()
        self.isFilted = False
        self.statics = {}
        self.addWeight=0

        reader=open(path)
        self.success=True
        try:
            readData=reader.read()
        except UnicodeDecodeError:
            self.success=False
        else:
            self.setAddWeight(readData)
            self.soup = bs(readData)
            self.statics = defaultdict(lambda: 0, self.statics)
            self.geneStatics()

    def setAddWeight(self,page):
        kindPattern=re.compile(self.kind)
        self.addWeight=len(re.findall(kindPattern,page))

    def isEmpty(self):
        return len(self.statics)==0
    def geneStatics(self):
        for str in self.soup.strings:
            words = str.split(" ")
            for word in words:
                self.statics[word] += 1

    def getAnswer(self):
        global  aa
        result = np.array(answerTable[self.kind])
        return result

    def formalData(self,fileVector,isTest):
        """ formalize raw statics to neural network input data
        """
        staticLen = len(self.statics)
        featureVector=[]
        for fileWord in fileVector:
            if fileWord in self.statics:
                if not isTest:
                    featureVector.append(self.statics.get(fileWord)+self.addWeight)
                else:
                    featureVector.append(self.statics.get(fileWord))
            else:
                featureVector.append(0)
        return featureVector
        #
        # if dimen > staticLen:
        #     return self.aplanationStatics(frequenValues, dimen)
        # else:
        #     return heapq.nlargest(dimen,frequenValues)

    def aplanationStatics(self, frequency, dimen):
        """make data meet neural network dimension
        like this,
        transfer  8,6,5,4,3    which is 5 dimensions
        to      8,8,6,6,5,5,4,4,3 which is 9 dimensions
        surely it's awkward, but it's easy :)
        """
        staticLen = len(self.statics)
        aplanRate = int(dimen / staticLen)
        alphaReminder = dimen % staticLen

        realData = []

        for i in range(staticLen):
            if i < alphaReminder:
                realData += [frequency[i]] * (aplanRate + 1)
            else:
                realData += [frequency[i]] * aplanRate

        return realData

    def printStatics(self):
        for word, frequency in self.statics:
            print(word + "  " + str(frequency))

    def getKind(self):
        splits = self.path.split("/")
        self.kind = splits[2]


class DataCollect:

    def __init__(self):
        self.judgeData = "texas"
        self.mainDir = "./webkb/"
        self.thirdDirList = []
        self.judgeDirList = []
        self.allWords={}
        self.allWords=defaultdict(lambda:0,self.allWords)
        self.getTrainings()


    def addToAllWords(self,newPage):
        ''' add a page data to allWords '''
        for word,frequency in newPage.statics.items():
            self.allWords[word]+=frequency

    def getTrainData(self):
        return self.thirdDirList

    def getTestData(self):
        return self.judgeDirList

    def getTrainings(self):
        '''get traings data dirs and judge data dirs'''
        secondDirList = []
        mainDirList = os.listdir(self.mainDir)
        #  ['student', 'other', 'staff', 'faculty', 'department', 'project', 'course']

        # get trainging data pages
        for i in mainDirList:
            oneSecond = os.listdir(self.mainDir + i)
            oneSecond.remove(self.judgeData)
            oneSecond = [self.mainDir + i + "/" + m + "/" for m in oneSecond]
            secondDirList.append(oneSecond)

        for m in secondDirList:
            for i in m:
                third = os.listdir(i)
                newThird = [i + a for a in third]
                newPages = []
                for path in newThird:
                    one=OnePage(path)
                    if one.success:
                        newPages.append(one)
                        self.addToAllWords(one)
                    else:
                        pass
#                self.thirdDirList = self.thirdDirList + newThird
                self.thirdDirList = self.thirdDirList + newPages

        # get judge data pages
        judgeSecDirs = [self.mainDir + a + "/" +
                        self.judgeData + "/" for a in mainDirList]

        for m in judgeSecDirs:
            oneJudge = os.listdir(m)
            newJudge = [m + a for a in oneJudge]
            newPages = [OnePage(path) for path in newJudge]
            self.judgeDirList = self.judgeDirList + newPages


class DataFormal:

    def __init__(self,allWords, trainData, testData, batch=50, elimi=0.02):
        """

        trainData:  allPages for training
        testData:  allPages for testing
        batch:  page numbers of handling in one time
        elimi:  eliminate some meaningless in page by a proportion
        """
        self.allStatics = allWords
        self.elimiRate = elimi
        self.elimiEle = []
        self.fileVector=[]
        self.batch = batch
        self.trainData = trainData
        self.testData = testData
        self.isFileGened=False
        np.random.shuffle(self.trainData)
        np.random.shuffle(self.testData)
        self.otherData=0

    def handleAllWords(self,dimen):
        if not self.isFileGened:
            self.allStatics = OrderedDict(reversed(sorted(self.allStatics.items(), key=lambda a: a[1])))
            elimiNum = int(self.elimiRate * len(self.allStatics))
            allWords = list(self.allStatics.keys())
            self.fileVector = allWords[elimiNum:elimiNum + dimen]
            self.isFileGened=True

    def getRandomTrains(self, dimen):
        """ dimen: dimension of input data
         in fact, it means "n" of nlargest frequency words 
        """

        # get a random flag
      #  randomFlag = np.random.randint(len(self.trainData) - self.batch)
        # get a random batch training data
    #    randomTrains = self.trainData[randomFlag:randomFlag + self.batch]
        self.handleAllWords(dimen)

        randomInputAnswer = []
        # eliminate special words, collect answer
        for training in self.trainData:
            if(not training.isFilted):
                self.filterEliminate(training)
            randomInputAnswer.append(training.getAnswer())
            if training.kind=="other":
                self.otherData+=1

        randomInputData = []
        for training in self.trainData:
            if not training.isEmpty():
                randomInputData.append(np.array(training.formalData(self.fileVector,True)))

        return randomInputData, randomInputAnswer

    def getRandomTest(self,dimen):
        # self.handleAllWords(dimen)
        randomTestAnswer=[]
        for training in self.testData:
            if (not training.isFilted):
                self.filterEliminate(training)
                randomTestAnswer.append(training.getAnswer())


        randomTestData = []
        for training in self.testData:
            if not training.isEmpty():
                randomTestData.append(np.array(training.formalData(self.fileVector,False)))

        return randomTestData,randomTestAnswer


    def setElimiRate(self, elimi):
        self.elimiRate = elimi


    def filterEliminate(self, page):
        for it in self.elimiEle:
            page.statics.pop(it)
        page.isFilted = True
        return page
