from collections import defaultdict
import os
import re
import heapq
import numpy as np
from collections import OrderedDict

answerTable = {}

def SetAnswerTable(mainDir="./20_newsgroups/"):
   # ="./20_newsgroups/"
    mainDirList=os.listdir(mainDir)

    global answerTable
    index=0
    for dir in mainDirList:
        binStr=bin(int(str(index),10))
        binStr=binStr[2:]
        if len(binStr)<5:
            binStr="0"*(5-len(binStr))+binStr
        answerTable[dir]=[int(a) for a in list(binStr)]
        index+=1
    
class OnePage:
    ''' handle one page'''

    def __init__(self, path):
        self.kind = ""

        self.path = path
        self.getKind()
        self.isFilted = False
        self.statics = {}

        reader = open(path)
        self.success = True
        try:
            self.readData = reader.read()
        except UnicodeDecodeError:
            self.success = False
        else:
            self.statics = defaultdict(lambda: 0, self.statics)
            self.geneStatics()

    def isEmpty(self):
        return len(self.statics) == 0

    def geneStatics(self):
        for str in self.readData.split("\n"):
            words = str.split(" ")
            for word in words:
                self.statics[word] += 1

    def getAnswer(self):
        result = np.array(answerTable[self.kind])
        return result

    def formalData(self, fileVector, isTest):
        """ formalize raw statics to neural network input data
        """
        staticLen = len(self.statics)
        featureVector = []
        for fileWord in fileVector:
            if fileWord in self.statics:
                if not isTest:
                    featureVector.append(
                        self.statics.get(fileWord))
                else:
                    featureVector.append(self.statics.get(fileWord))
            else:
                featureVector.append(0)
        return featureVector

    def printStatics(self):
        for word, frequency in self.statics:
            print(word + "  " + str(frequency))

    def getKind(self):
        splits = self.path.split("/")
        self.kind = splits[2]


class DataCollect:

    def __init__(self,mainDir="./20_newsgroups/"):
        self.mainDir = mainDir
        self.thirdDirList = []
        self.allWords = {}
        self.allWords = defaultdict(lambda: 0, self.allWords)
        self.getTrainings()

    def getRealData(self):
        return self.thirdDirList

    def addToAllWords(self, newPage):
        ''' add a page data to allWords '''
        for word, frequency in newPage.statics.items():
            self.allWords[word] += frequency

    def getTrainings(self):
        '''get traings data dirs and judge data dirs'''
        secondDirList = []
        mainDirList = os.listdir(self.mainDir)
        #  ['student', 'other', 'staff', 'faculty', 'department', 'project', 'course']

        # get trainging data pages
        for i in mainDirList:
            oneSecond = os.listdir(self.mainDir + i)

            oneSecond = [self.mainDir + i + "/" + m for m in oneSecond]
            secondDirList.append(oneSecond)

        for m in secondDirList:
            newPages = []
            for path in m:
                one = OnePage(path)
                if one.success:
                    newPages.append(one)
                    self.addToAllWords(one)
                else:
                    pass
                #                self.thirdDirList = self.thirdDirList + newThird
            self.thirdDirList = self.thirdDirList + newPages


class DataFormal:

    def __init__(self, allWords, trainData, testData, batch=50, elimi=0.001):
        """

        trainData:  allPages for training
        testData:  allPages for testing
        batch:  page numbers of handling in one time
        elimi:  eliminate some meaningless in page by a proportion
        """
        SetAnswerTable()
        self.allStatics = allWords
        self.elimiRate = elimi
        self.elimiEle = []
        self.fileVector = []
        self.batch = batch
        self.trainData = trainData
        self.testData = testData
        self.isFileGened = False
        np.random.shuffle(self.trainData)
        np.random.shuffle(self.testData)
        self.otherData = 0

    def handleAllWords(self, dimen):
        if not self.isFileGened:
            self.allStatics = OrderedDict(
                reversed(sorted(self.allStatics.items(), key=lambda a: a[1])))
            elimiNum = int(self.elimiRate * len(self.allStatics))
            allWords = list(self.allStatics.keys())
            self.fileVector = allWords[elimiNum:elimiNum + dimen]
            self.isFileGened = True

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

        randomInputData = []
        for training in self.trainData:
            if not training.isEmpty():
                randomInputData.append(
                    np.array(training.formalData(self.fileVector, True)))

        return randomInputData, randomInputAnswer

    def getRandomTest(self, dimen):
        # self.handleAllWords(dimen)
        randomTestAnswer = []
        for training in self.testData:
            if (not training.isFilted):
                self.filterEliminate(training)
                randomTestAnswer.append(training.getAnswer())

        randomTestData = []
        for training in self.testData:
            if not training.isEmpty():
                randomTestData.append(
                    np.array(training.formalData(self.fileVector, False)))

        return randomTestData, randomTestAnswer

    def setElimiRate(self, elimi):
        self.elimiRate = elimi

    def filterEliminate(self, page):
        for it in self.elimiEle:
            page.statics.pop(it)
        page.isFilted = True
        return page
