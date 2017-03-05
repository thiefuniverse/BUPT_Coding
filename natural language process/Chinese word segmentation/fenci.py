#!/usr/bin/python3

import argparse
import sys


class FenCi:

    def __init__(self):
        "a Chinese segmentation program. You can choose different ways to segment your words."
        self._dict = {}
        self._maxLen = 6
        # 1 mean forward match   2 mean backward match  3 mean best for forward
        # and backward
        self._mode = 1
        self.outFile = ""
        self.splitFlag = '/'
        self.defaultDict="dict.txt"
        self.envFlag="\n"

    def setEnvFlag(self,flag):
        if flag=='u':
            self.envFlag='\n'
        else:
            self.envFlag='\r\n'
    def setSplitFlag(self,flag):
        self.splitFlag=flag
    def setMode(self, mode):
        self._mode = mode

    def setOutput(self, outfile):
        self.outFile = outfile

    def loadDict(self, dict):
        """
        Keyword Arguments:
        dict -- a dict file which has format like this:
        (Chinese)  (...)
        北京图书馆
        北邮教务处
        长江七号
        长江八号
        长江三号
        电视机
        名字
        电脑
        ...

        """
        readDict = open(dict)

        # get max len of Chinese phrase
        # self._maxLen=int(readDict.readline())

        # create maxLen list for Chinese phrase
#        for m in range(self._maxLen,0,-1):
#           self._dict[m]=[]

        # read each line and get phrase to _dict
        for line in readDict.readlines():
            lineLen = len(line) - 1
            if self._maxLen < lineLen:
                continue
            if lineLen in self._dict:
                self._dict[lineLen].append(line[:lineLen])
            else:
                self._dict[lineLen] = []
                self._dict[lineLen].append(line[:lineLen])

    # segmenta phrases and print them
    def segmenta(self, phrases):
        result = []

        if self._mode == 1:
            while(phrases):
                (phrases, res) = self.segmenta_phrase_forward(phrases)
                result.append(res)
        elif self._mode == 2:
            while(phrases):
                (phrases, res) = self.segmenta_phrase_backward(phrases)
                result.insert(0, res)

        # output to file or stdout
        if self.outFile:
            out = open(self.outFile, mode="a")   # need more operation
            for m in result:
                print(m + self.splitFlag, end="", file=out)
            print(self.envFlag, end="", file=out)
            out.close()
        else:
            for m in result:
                print(m + self.splitFlag, end="")
            print(self.envFlag, end="")
        return result

    def segmenta_file(self, inFile):
        '''handle a file for segmentation'''
        allSen = open(inFile, encoding='utf-16 ')
        for line in allSen.readlines():
            tempLine = line[0:len(line) - 1]
            self.segmenta(tempLine)

        allSen.close()

    def segmenta_phrase_forward(self, phrase):
        # set maxlen
        maxlen = self._maxLen if len(phrase) >= self._maxLen else len(phrase)

        # max forward match
        for m in range(maxlen, 0, -1):
            if m in self._dict:
                if phrase[0:m] in self._dict[m]:
                    return (phrase[m:], phrase[0:m])

        return (phrase[1:], phrase[0:1])
#            else:

    def segmenta_phrase_backward(self, phrase):
        # set maxlen
        maxlen = self._maxLen if len(phrase) >= self._maxLen else len(phrase)

        # max backward match
        for m in range(maxlen, 0, -1):
            if m in self._dict:
                if phrase[len(phrase) - m:] in self._dict[m]:
                    return (phrase[0:len(phrase) - m], phrase[len(phrase) - m:])

        return (phrase[0:len(phrase) - 1], phrase[len(phrase) - 1:])


class getCommand():

    def __init__(self):
        '''get command for segmentation'''
        self.parser = argparse.ArgumentParser(
            description="Hi,I'm your Chinese segamentor!"
        )
        self.parser.add_argument(
            "-f", help="a file with Chinese passages or sentances")
        self.parser.add_argument("-o", help="output to a file")
        self.parser.add_argument("-l", help="a Chinese sentances")
        self.parser.add_argument(
            "-m", type=int, help="set mode(three modes  1 for forward match, 2 for backward match")
        self.parser.add_argument("-v", help="version", action="version",
                                 version="current version is 99999.0   [(';')]  welcome to use me! ")
        self.parser.add_argument("-s",help="split sentances, default it's / and you can replace it with a char or other strings.")
        self.parser.add_argument("-d", help="use your dict")
        self.parser.add_argument("-e",help="u for unix-like and w for windows.")
        self.parser.add_argument(
            "-c", help="add your dict and default dict also will be used")
        self.arg = self.parser.parse_args()

    def handle(self):
        segmentor = FenCi()

        # load dict
        if self.arg.c:
            segmentor.loadDict(self.arg.c)
            segmentor.loadDict("dict.txt")
        elif self.arg.d:
            segmentor.loadDict(self.arg.d)
        else:
            segmentor.loadDict(segmentor.defaultDict)

        if self.arg.e:
            segmentor.setEnvFlag(self.arg.e)
        if self.arg.s:
            segmentor.setSplitFlag(self.arg.s)
        if self.arg.m:
            segmentor.setMode(self.arg.m)
        if self.arg.o:
            segmentor.setOutput(self.arg.o)
        if self.arg.l:
            segmentor.segmenta(self.arg.l)
        elif self.arg.f:
            segmentor.segmenta_file(self.arg.f)
if __name__ == '__main__':
    mm = getCommand()
    if len(sys.argv[1:]) == 0:
        mm.parser.print_help()
        mm.parser.exit()
    else:
        mm.handle()


# debug code
# mm=FenCi()
# mm.loadDict("dict.txt")
# mm.setMode(1)
# mm.segmenta("我的电脑和鼠标很厉害北京邮电大学厉害")
