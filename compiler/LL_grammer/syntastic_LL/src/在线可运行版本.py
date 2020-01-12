#!/usr/bin/python3.5
#from prettytable import PrettyTable

# allGenerator['N']=['1','2','3','4','5','6','7','8','9','0']


class LL_Grammer():

    def __init__(self):
        '''define a LL grammer'''
        self.noterSymbol = set()
        self.noterSymbol.add('E')
        self.noterSymbol.add('T')
        self.noterSymbol.add('F')
        self.terSymbol = set()
        self.allGenerator = {}

        self.startSymbol = 'E'
        self.allGenerator['E'] = ['E+T', 'E-T', 'T']
        self.allGenerator['T'] = ['T*F', 'T/F', 'F']
        self.allGenerator['F'] = ['(E)', 'n']  # n means num

        self.firstCollect = {}
        self.followCollect = {}
        self.tempTerminal = {}
        self.getTerSymbol()

        # letter n in generator is number
        # so for it working, should add real numebr into terminal
        # then we get a tempTernimal set()
        self.tempTerminal = self.terSymbol.copy()
        self.number = set(["1", "2", "3", "4", "5", "6", "7", "8", "9", "0"])
        self.tempTerminal = self.tempTerminal.union(self.number)

        self.analyzeTable = {}

    def initAnalyzeTable(self):
        for noter in self.noterSymbol:
            self.analyzeTable[noter] = {}
            for ter in self.terSymbol:
                self.analyzeTable[noter][ter] = " "
            self.analyzeTable[noter]["$"] = " "

    def addAnalyzeItem(self, noter, ter, generator):
        self.analyzeTable[noter][ter] = noter + "-->" + generator

    def analyzeTableConstructor(self):
        for noter in self.noterSymbol:
            for first in self.firstCollect[noter]:
                # when first is empty, handle noter's follow
                if first == "empty":
                    realGenerator = ""
                    for gerator in self.allGenerator[noter]:
                        if gerator == "empty" or self.isAddFollow(gerator):
                            realGenerator = gerator
                            break
                    for item in self.followCollect[noter]:
                        self.addAnalyzeItem(noter, item, realGenerator)
                # find the generator of the first add add into table
                else:
                    for gerator in self.allGenerator[noter]:
                        if (gerator[0:1] == first) or (
                                gerator[0:2] in self.noterSymbol and first in self.firstCollect[gerator[0:2]]) \
                                or (gerator[0:1] in self.noterSymbol and first in self.firstCollect[gerator[0:1]]):
                            self.addAnalyzeItem(noter, first, gerator)
                            break

    def printAnalyzeTable(self):
        pass
        ############################################################
        # print("AnalyzeTable is  here:")                          #
        # header = list(self.terSymbol)                            #
        # header.append("$")                                       #
        # cloumn = header.copy()                                   #
        # header.insert(0, "Noter")                                #
        # table = PrettyTable(header)                              #
        # table.align["Noter"] = 1                                 #
        # table.padding_width = 1                                  #
        #                                                          #
        # for noter in self.noterSymbol:                           #
        #     tempCloumn = []                                      #
        #     tempCloumn.append(noter)                             #
        #     for ter in cloumn:                                   #
        #         tempCloumn.append(self.analyzeTable[noter][ter]) #
        #     table.add_row(tempCloumn)                            #
        #                                                          #
        # print(table)                                             #
        ############################################################

    def reverse_gerator(self, str):
        start = 0
        end = len(str) - 1
        reverStr = ""
        while start <= end:
            if str[start] in self.tempTerminal or str[start] == '$':
                reverStr = str[start] + reverStr
                start += 1
            elif str[start:start + 2] in self.noterSymbol:
                reverStr = str[start:start + 2] + reverStr
                start += 2
            elif str[start:start + 1] in self.noterSymbol:
                reverStr = str[start:start + 1] + reverStr
                start += 1
        return reverStr

    def analyze(self, inputStr):
        target = inputStr + "$"
        stack = "$" + self.startSymbol  # init the stack
        isCompleted = ""
        leftSentance = self.startSymbol
        leftSequences = []
        leftSequences.append(leftSentance)
        while target != stack or target != '$':
            firstTarget = target[0]   # get first char in target str

            # when top of stack is terminal
            if stack[len(stack) - 1:] == firstTarget or stack[len(stack) - 1:] in self.number:
                isCompleted += firstTarget
                stack = stack[0:len(stack) - 1]
                inStack = self.reverse_gerator(stack)
                leftSentance = isCompleted + inStack[0:len(inStack) - 1]
                target = target[1:]
            elif stack[len(stack) - 1:] in self.tempTerminal and stack[len(stack) - 1:] != firstTarget:
                print("error! this string doesn't belong to  this grammer.")
                return 0
            # when top 2 in stack is noterminal
            elif stack[len(stack) - 2:] in self.noterSymbol:
                # for handling real number  rather 'n'
                if firstTarget in self.number:
                    firstTarget = 'n'

                # get generator from analyzeTable
                if not self.analyzeTable[stack[len(stack) - 2:]][firstTarget]:
                    print("error! this string doesn't belong to  this grammer.")
                    return 0
                temp = self.analyzeTable[stack[len(stack) - 2:]][firstTarget]
                if temp[5:] == "empty":
                    stack = stack[0:len(stack) - 2]
                else:
                    # if it's n, means I should move real number from target[0] to stack
                    # else just other chars
                    if temp[5:] == "n":
                        stack = stack[0:len(stack) - 1] + target[0]
                    else:
                        stack = stack[0:len(stack) - 1] + \
                            self.reverse_gerator(temp[5:])

                # leftSenctance= isCompleted + reverse(stack)
                inStack = self.reverse_gerator(stack)
                leftSentance = isCompleted + inStack[0:len(inStack) - 1]
                leftSequences.append(leftSentance)
            elif stack[len(stack) - 1:] in self.noterSymbol:
                if firstTarget in self.number:
                    firstTarget = 'n'

                if not self.analyzeTable[stack[len(stack) - 1:]][firstTarget]:
                    print("error! this string doesn't belong to  this grammer.")
                    return 0
                temp = self.analyzeTable[stack[len(stack) - 1:]][firstTarget]
                if temp[4:] == "empty":
                    stack = stack[0:len(stack) - 1]
                else:
                    if temp[4:] == "n":
                        stack = stack[0:len(stack) - 1] + target[0]
                    else:
                        stack = stack[0:len(stack) - 1] + \
                            self.reverse_gerator(temp[4:])
                inStack = self.reverse_gerator(stack)
                leftSentance = isCompleted + inStack[0:len(inStack) - 1]
                leftSequences.append(leftSentance)

        print("\nresult for " + inputStr + " :", end="")
        i = 0
        for m in range(len(leftSequences)):
            if i % 5 == 0:
                print("\n", end="")
            if m >= 1:
                print("==>", end="")
            i += 1
            print(leftSequences[m], end="")

    def getTerSymbol(self):
        '''get terminal symbol'''
        for li in self.allGenerator.values():
            for select in li:
                for c in select:
                    if c not in self.noterSymbol:
                        self.terSymbol.add(c)

    def isDirectLCExist(self, ter, line):
        '''judge a direct　left recursion exists or not'''
        for m in line:
            if m.startswith(ter):
                return True
        return False

    def killLeftCur_line(self, ter, line):
        '''eliminate left recursion'''
        result = {}
        newTer = ter + '^'
        result[ter] = []
        result[newTer] = []
        flag = 0

        for m in line:
            if m.startswith(ter):
                result[newTer].append(m[1:] + newTer)
                self.noterSymbol.add(newTer)
            else:
                flag += 1
                result[ter].append(m + newTer)
        if flag:
            result[newTer].append("empty")
        return result

    def killLeftCur(self):
        generator = self.allGenerator.copy()
        for a in self.allGenerator:
            if self.isDirectLCExist(a, self.allGenerator[a]):
                generator.update(self.killLeftCur_line(
                    a, self.allGenerator[a]))
        self.allGenerator = generator

    def printTerSymbol(self):
        '''print terminal symbol'''
        for i in self.terSymbol:
            print(i + ',', end="")
        print('\n')

    def printGenerator(self):
        for noter in self.allGenerator:
            terLine = self.allGenerator[noter]
            for select in terLine:
                print(noter + ' ---> ' + select + ' ,', end="")
            print('\n')

    def printFirstcollect(self):
        for noter in self.firstCollect:
            print("First[" + noter + "] : ", end="")
            for ter in self.firstCollect[noter]:
                print(ter + " ", end="")
            print("\n")

    def getFirstCollect(self):
        '''compute first collection'''
        for noter in self.noterSymbol:
            self.firstCollect[noter] = set()
            self.followCollect[noter] = set()

        for noter in self.noterSymbol:
            for m in self.allGenerator[noter]:
                # judge empty generator
                if m == "empty":
                    self.firstCollect[noter].add("empty")

                    # if first symbol is terminal, add it
                if m[0:1] in self.terSymbol:
                    self.firstCollect[noter].add(m[0:1])
                else:
                    # if it's none terminal, add a symbol with (f + noter)
                    if m[0:2] in self.noterSymbol:
                        self.firstCollect[noter].add("f" + m[0:2])
                    elif m[0:1] in self.noterSymbol:
                        self.firstCollect[noter].add("f" + m[0:1])

        # replace f with its real first collect
        for no in self.noterSymbol:
            for noter in self.noterSymbol:
                if "f" + no in self.firstCollect[noter]:
                    self.firstCollect[noter].remove("f" + no)
                    self.firstCollect[noter] = self.firstCollect[
                        noter].union(self.firstCollect[no])

    def isAddFollow(self, gene):
        '''judge A-> aTM for add follow(A) to follow(T)
        noter for T
        gene for M
        '''
        # gene doesn't exist
        if not gene:
            return True

        if gene[0:1] == '^':
            return False
        l = 0
        up = len(gene)
        while True:
            if gene[l:l + 2] in self.firstCollect and "empty" in self.firstCollect[gene[l:l + 2]]:
                l += 2
            elif gene[l:l + 1] in self.firstCollect and "empty" in self.firstCollect[gene[l:l + 1]]:
                l += 1
            elif gene[l:l + 1] == "":
                return True
            else:
                return False

    def getFollowCollect(self):
        '''get follow collection'''
        self.followCollect[self.startSymbol].add('$')
        for geneLine in self.allGenerator:
            for eachGene in self.allGenerator[geneLine]:
                if eachGene == "empty":
                    pass
                else:
                    m = 0
                    up = len(eachGene)
                    while True:
                        if eachGene[m:m + 2] in self.noterSymbol or eachGene[m:m + 1] in self.noterSymbol:
                            start = m
                            if eachGene[m:m + 1] in self.noterSymbol:
                                end = m + 1
                            if eachGene[m:m + 2] in self.noterSymbol:
                                end = m + 2
                            l = end - 1
                            m = end
                            if self.isAddFollow(eachGene[l + 1:]):
                                self.followCollect[
                                    eachGene[start:end]].add("o" + geneLine)
                            if eachGene[l + 1:l + 2] in self.terSymbol:
                                self.followCollect[eachGene[start:end]].add(
                                    eachGene[l + 1:l + 2])
                            if eachGene[l + 1:l + 3] in self.noterSymbol:
                                self.followCollect[eachGene[start:end]] = self.followCollect[eachGene[start:end]].union(
                                    self.firstCollect[eachGene[l + 1:l + 3]])
                            elif eachGene[l + 1:l + 2] in self.noterSymbol:
                                self.followCollect[eachGene[start:end]] = self.followCollect[eachGene[start:end]].union(
                                    self.firstCollect[eachGene[l + 1:l + 2]])
                        else:
                            m += 1
                            if m >= up:
                                break

        # replace o+ noterminal to real follow(noterminal)
        for no in self.noterSymbol:
            for noter in self.noterSymbol:
                if "o" + no in self.followCollect[noter]:
                    self.followCollect[noter].remove("o" + no)
                    self.followCollect[noter] = self.followCollect[
                        noter].union(self.followCollect[no])

        for no in self.noterSymbol:
            for noter in self.noterSymbol:
                if "o" + no in self.followCollect[noter]:
                    self.followCollect[noter].remove("o" + no)
                    self.followCollect[noter] = self.followCollect[
                        noter].union(self.followCollect[no])

        for noter in self.noterSymbol:
            if "empty" in self.followCollect[noter]:
                self.followCollect[noter].remove("empty")

    def printFollowcollect(self):
        for noter in self.followCollect:
            print("Follow[" + noter + "] : ", end="")
            for ter in self.followCollect[noter]:
                print(ter + " ", end="")
            print("\n")


if __name__ == '__main__':
    test = LL_Grammer()
    test.killLeftCur()  # 消去直接左递归
    test.printGenerator()  # 打印所有产生式
    test.initAnalyzeTable()  # 初始化分析表
    test.getFirstCollect()  # 获取First集
    test.printFirstcollect()  # 打印First集
    test.getFollowCollect()  # 获取Follow集
    test.printFollowcollect()  # 打印Follow集
    test.analyzeTableConstructor()  # 构造分析表
    test.printAnalyzeTable()  # 打印分析表
    test.analyze("(4*5/3)*4")  # 分析并输出分析结果
    test.analyze("2*4/(3/1)")
    test.analyze("(3*7+4-3*2+6/3)")
    # you can add some other expressions and test them
    test.analyze("5*3/(2*4-6+2/1)")
    testStr = ""
    while True:
        print("")
        testStr = input(
            '''Please input your analyze string:(like 4*6+3 , input 'end' can quit): ''')
        if testStr == "end":
            print("bye")
            break
        test.analyze(testStr)
