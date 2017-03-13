import re
import os
from collections import defaultdict

stopWords = []


def getStopWords(file):
    stopRead = open(file)
    global stopWords
    for line in stopRead.readlines():
        stopWords.append(line)


class oneDoc:

    def __init__(self, fileName):
        "transfer a file to words list "

        self.fileName = fileName
        self.sentanOffset=5
        self.statics = {}
        self.weight=0
        self.allWords=[]
        self.statics = defaultdict(lambda: [], self.statics)
        self.readWords(fileName)


    def updateWeight(self,queryWords):
        for word in queryWords:
            if word in self.statics:
                self.weight+=len(self.statics[word])

    def readWords(self, curFileName):
        global stopWords

        # need judge stopWords is none or not
        curFile = open(curFileName)
        try:
            readData = curFile.read()
        except UnicodeDecodeError:
            self.success = False
        else:
            self.success=True
            notAlpha = re.compile("[\W]+")
            for line in readData.split("\n"):
                line = re.sub(notAlpha, " ", line)
                wordList = line.split()
                for w in wordList:
                    if w not in stopWords:
                        self.allWords.append(w)

            for index, word in enumerate(self.allWords):
                self.statics[word].append(index)
        curFile.close()


    def getBlurSetance(self,word):
        '''handle one word for generate sentance'''
        if word in self.statics:
            wordIndex=self.statics[word]
            shortWords=self.allWords[wordIndex[0]-self.sentanOffset:wordIndex[0]+self.sentanOffset]
            shortSentance=" ".join(shortWords)
            return shortSentance
        else:
            return ""

    def getLongSentance(self,words):
        '''handle multiple words (word list) for generate sentance '''
        resultStr=""
        for word in words:
            newSen=self.getBlurSetance(word)
            if newSen:
                resultStr += "..."+newSen + "..."

        return resultStr

    def getStatics(self):
        return self.statics


class createIndex:

    def __init__(self):
        ''' create index for raw data,get a complete inverted index'''
        self.allIndex = {}
        self.allIndex = defaultdict(lambda: {}, self.allIndex)
        self.allDocs={}
    def loadAllFiles(self, fileDirs):
        for fileDir in fileDirs:
            files = os.listdir(fileDir)
            for file in files:
                thisFile = fileDir + file
                oneDocum = oneDoc(thisFile)
                self.addOneDoc(oneDocum)
                self.allDocs[thisFile]=oneDocum
    def addOneDoc(self, aDoc):
        statics = aDoc.getStatics()
        for word in statics:
            self.allIndex[word][aDoc.fileName] = statics[word]
