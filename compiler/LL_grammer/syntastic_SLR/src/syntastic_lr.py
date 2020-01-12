#!/usr/bin/python3.5
from prettytable import PrettyTable

# allGenerator['N']=['1','2','3','4','5','6','7','8','9','0']


class LL_Grammer:

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
        #self.allGenerator['E'] = ['E+T', 'T']
        self.allGenerator['T'] = ['T*F', 'T/F','F']
        #self.allGenerator['T'] = ['T*F',  'F']
        self.allGenerator['F'] = ['(E)', 'n']  # n means num

        self.extensionSymbol='E^'
        self.firstCollect = {}
        self.followCollect = {}
        self.tempTerminal = {}
        self.multiNum={}
        self.getTerSymbol()

        self.geneList=[]
        self.stateList={}
        self.extenState=[]
        self.stateNum=0
        self.realNum=[]

        self.dfa={}
        # letter n in generator is number
        # so for it working, should add real numebr into terminal
        # then we get a tempTernimal set()
        self.tempTerminal = self.terSymbol.copy()
        self.number = set(["1", "2", "3", "4", "5", "6", "7", "8", "9", "0"])
        self.tempTerminal = self.tempTerminal.union(self.number)

        self.analyzeTable = {}

        self.actionTable=[]

    def addActionTable(self,stack,inputS,analyzeAction):
        m=[stack,inputS,analyzeAction]
        self.actionTable.append(m)

    def printDFA(self):
        print("DFA states as these:")
        for state in self.dfa:
            for nextState in self.dfa[state]:
                print("State ",state," can shift ",nextState," to ",self.dfa[state][nextState])

    def printActionTable(self):
        header=["stack","input","analyze action"]
        table = PrettyTable(header)
        table.align["Noter"] = 1
        table.padding_width = 1

        for tempCloumn in self.actionTable:
            table.add_row(tempCloumn)

        print(table)
    def humanAction(self,action):
        if action[0]=='s':
            return "Shift "+action[1:]
        elif action[0]=='r':
            return "Reduce by " + self.geneList[int(action[1:])]
        else:
            return action

    def addItemToTable(self,frontState,terOrNoter,actionOrGoto):
        if frontState not in self.analyzeTable:
            self.analyzeTable[frontState]={}
            self.analyzeTable[frontState][terOrNoter] = actionOrGoto
        else:
            self.analyzeTable[frontState][terOrNoter] = actionOrGoto

    def printAnalazyTable(self):
        self.initAnalyzeTable()

        print("AnalyzeTable is  here:")
        header = list(self.terSymbol)
        header.append("$")
        otherHeader=list(self.noterSymbol)
        otherHeader.remove(self.extensionSymbol)
        header+=otherHeader

        cloumn = header.copy()
        header.insert(0, "Noter")
        table = PrettyTable(header)
        table.align["Noter"] = 1
        table.padding_width = 1

        for state in self.realNum:
            tempCloumn = []
            tempCloumn.append(state)
            for ter in cloumn:
                if ter not in self.analyzeTable[state]:
                    self.analyzeTable[state][ter]=""
                tempCloumn.append(self.analyzeTable[state][ter])
            table.add_row(tempCloumn)

        print(table)

    def analyze(self,inputStr):
        self.actionTable.clear()
        stack=[0]
        inputS=inputStr+'$'
        action=""
        analyzeStr=""
        topState=''
        tempTarget=""
        while action!="acc":
            topState=stack[len(stack)-1]

            firstTarget=inputS[0]
            tempTarget=firstTarget
            if firstTarget in self.number:
                tempTarget=firstTarget
                firstTarget="n"
            if firstTarget not in self.analyzeTable[topState]:
                print("error for this string")
                return
            action=self.analyzeTable[topState][firstTarget]

            if not action:
                print("error for this string")
                return
            actionRecord=self.humanAction(action)
            # update action table
            self.addActionTable(stack.copy(),inputS,actionRecord)

            if action[0]=='s':
                stack.append(tempTarget)
                stack.append(int(action[1:]))
                inputS=inputS[1:]
            elif action[0]=='r':
               # stack.pop()
                num=int(action[1:])
                genor=self.geneList[num]
                genorFormat=genor.split("->")
                removeLen=len(genorFormat[1])*2
                stack=stack[0:len(stack)-removeLen]

                lastState=stack[len(stack)-1]
                # judge next state
                nextState=self.analyzeTable[lastState][genorFormat[0]]

                stack.append(genorFormat[0])
                stack.append(nextState)

        print("Analyze Action Table for "+inputStr+" is  here:")
        self.printActionTable()

    def initAnalyzeTable(self):
        for state in range(self.stateNum):
            if state not in self.analyzeTable:
                self.analyzeTable[state]={}

        for stateNum in self.analyzeTable:
            tempSymbol=self.terSymbol.copy()
            tempSymbol.add('$')

            for ter in tempSymbol:
                if ter in self.analyzeTable[stateNum]:
                    action = self.analyzeTable[stateNum][ter]
                    if (not action == 'acc') and (action[0]=='s'):
                        num = int(action[1:])
                        if num not in self.realNum:
                            num = self.replaceMultiNum(num)
                            self.analyzeTable[stateNum][ter] = action[0] + str(num)



            for noter in self.noterSymbol:
                if noter in self.analyzeTable[stateNum]:
                    action = self.analyzeTable[stateNum][noter]
                    num = int(action)
                    if num not in self.realNum:
                        num = self.replaceMultiNum(num)
                        self.analyzeTable[stateNum][noter] = num


        for m in self.multiNum:
            for n in self.multiNum[m]:
                self.analyzeTable.pop(n)



    def initGeneList(self):
        for noter in self.allGenerator:
            for m in self.allGenerator[noter]:
                self.geneList.append(noter+'->'+m)
        self.geneList.append(self.extensionSymbol+'->'+self.startSymbol)


    def extensionGrammer(self):
        self.allGenerator['E^'] = ['E']
        self.noterSymbol.add('E^')

    def getTerSymbol(self):
        '''get terminal symbol'''
        for li in self.allGenerator.values():
            for select in li:
                for c in select:
                    if c not in self.noterSymbol:
                        self.terSymbol.add(c)

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
        # replace f with its real first collect
        for no in self.noterSymbol:
            for noter in self.noterSymbol:
                if "f" + no in self.firstCollect[noter]:
                    self.firstCollect[noter].remove("f" + no)
                    self.firstCollect[noter] = self.firstCollect[
                        noter].union(self.firstCollect[no])

        for m in self.noterSymbol:
            if len(m) == 2:
                self.firstCollect.pop(m)

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

        self.followCollect[self.extensionSymbol]=set()
        self.followCollect[self.extensionSymbol].add('$')
    def printFollowcollect(self):
        for noter in self.followCollect:
            print("Follow[" + noter + "] : ", end="")
            for ter in self.followCollect[noter]:
                print(ter + " ", end="")
            print("\n")

    def initState(self):
        initState=self.createState()

        initState.addGenerator(self.extensionSymbol,'.'+self.startSymbol)
        initState.ableNoterminal.add(self.startSymbol)

        addList=set()
        newAddList=set()
        addList.add(self.startSymbol)
        continueAdd=True

        # add . and generator recursively
        while continueAdd:
            continueAdd=False
            for addSymbol in addList:
                if addSymbol not in initState.generator:
                    for m in self.allGenerator[addSymbol]:
                        initState.addGenerator(addSymbol, '.' + m)
                        if (m[0] in self.noterSymbol) and (m[0] not in newAddList )and (m[0] not in addList):
                            newAddList.add(m[0])
                            initState.ableNoterminal.add(m[0])
                            continueAdd=True
                        if m[0] in self.terSymbol:
                            initState.ableTerminal.add(m[0])
            addList=newAddList.copy()
            newAddList.clear()

        self.stateList[initState.number]=initState
        self.extenState.append(initState)
        self.dfa[initState.number]={}
        self.dfa[initState.number][0]='0'


    def constructDFA(self):
        self.initState()

        newExtenState=[]
        continueExten=True
        oldState=[]

        while continueExten:
            continueExten=False

            for extenS in self.extenState:

                if extenS.number not in self.dfa:
                    self.dfa[extenS.number]={}

                oldState.append(extenS.number)
                # init dfa
                for m in extenS.ableTerminal:
                    newState = self.createState()
                    self.dfa[extenS.number][m] = newState.number
                    newExtenState.append(newState)
                for i in extenS.ableNoterminal:
                    newState = self.createState()
                    self.dfa[extenS.number][i] = newState.number
                    newExtenState.append(newState)

                allGeneor = extenS.generator
                for noter in allGeneor:
                    for genor in allGeneor[noter]:
                        pos = genor.find('.')
                        tempSymbol = genor[pos + 1:pos + 2]

                        # for analyzeTable
                        if tempSymbol:
                            nextStateNum = self.dfa[extenS.number][tempSymbol]
                            self.stateList[nextStateNum].addGenerator(noter, self.movePoint(genor))

                            if tempSymbol in self.terSymbol:
                                self.addItemToTable(extenS.number, tempSymbol, 's' + str(nextStateNum))
                            elif tempSymbol in self.noterSymbol:
                                self.addItemToTable(extenS.number, tempSymbol, nextStateNum)

                            newGenor=self.movePoint(genor)
                            pos = newGenor.find('.')
                            tempSymbol = newGenor[pos + 1:pos + 2]
                            if not tempSymbol:
                                for ter in self.followCollect[noter]:
                                    self.addItemToTable(nextStateNum, ter,
                                                        'r' + str(self.geneList.index(noter + '->' + newGenor[0:-1])))

                        else:

                            for ter in self.followCollect[noter]:
                                self.addItemToTable(extenS.number, ter,
                                                    'r' + str(self.geneList.index(noter + '->' + genor[0:-1])))
                            if noter == self.extensionSymbol:
                                self.addItemToTable(extenS.number, '$',
                                                    'acc')



                for state in newExtenState:
                    self.extensionState(state)

            self.extenState.clear()
            for m in newExtenState:
                if not m.isEnd():
                    exten=True
                    for oldNum in oldState:
                        if self.stateList[oldNum].isEqual(m):
                            exten=False
                            break
                    if exten:
                        self.extenState.append(m)
                else:
                    oldState.append(m.number)
            newExtenState.clear()
            if len(self.extenState):
                continueExten=True

    def createState(self):
        a=grammerState(self.stateNum)
        self.stateList[self.stateNum]=a
        self.stateNum+=1
        return a

    def extensionState(self,state):
        continueExten=True
        tempGeneor={}
        while continueExten:
            continueExten=False

            for noter, genorList in state.generator.items():
                for genor in genorList:
                    pos = genor.find('.')
                    nextChar = genor[pos + 1:pos + 2]
                    if nextChar in self.noterSymbol:
                        for item in self.allGenerator[nextChar]:
                            if not state.hasGenor(nextChar,'.'+item):
                                # state.addGenerator(nextChar,'.'+item)
                                if nextChar not in tempGeneor:
                                    tempGeneor[nextChar]=set()
                                tempGeneor[nextChar].add('.' + item)
                                continueExten=True

            for m in tempGeneor:
                if m not in state.generator:
                    state.generator[m]=set()
                state.generator[m] = state.generator[m].union(tempGeneor[m])


            tempGeneor.clear()

        # get next state for terminal and noterminal
        for noter, genorList in state.generator.items():
            for genor in genorList:
                pos = genor.find('.')
                nextChar = genor[pos + 1:pos + 2]
                if nextChar in self.terSymbol:
                    state.ableTerminal.add(nextChar)
                elif nextChar in self.noterSymbol:
                    state.ableNoterminal.add(nextChar)


    def movePoint(self,genor):
        '''move point to next position'''
        pointPos=genor.find('.')
        prim=genor[pointPos:pointPos+2]
        if prim:
            pointPos = genor.replace(prim,prim[1]+'.')
            return pointPos
        else:
            return False

    def removeMultiState(self):
        count=0

        realStateList=[]
        for item in self.stateList:
            shouldAdd=True
            for real in realStateList:
                if self.stateList[item].generator==real.generator:
                    if real.number not in self.multiNum:
                        self.multiNum[real.number]=set()
                    self.multiNum[real.number].add(self.stateList[item].number)
                    shouldAdd=False
            if shouldAdd:
                realStateList.append(self.stateList[item])
                self.realNum.append(self.stateList[item].number)
        count=len(realStateList)
        self.stateList=realStateList


        print("All states as these:")
        for i in self.stateList:
            i.print()
            print("---------------------------------------------")

        print(count)

    def replaceMultiNum(self,num):
        for real in self.multiNum:
            if num in self.multiNum[real]:
                return real
class grammerState:

    def __init__(self,num):
        self.generator = {}
        self.number=num
        self.ableTerminal=set()
        self.ableNoterminal=set()

    def addGenerator(self, noterminal, genor):
        if noterminal in self.generator:
            self.generator[noterminal].add(genor)
        else:
            self.generator[noterminal] = set()
            self.generator[noterminal].add(genor)
    def isEqual(self,another):
        return self.generator==another.generator

    def hasGenor(self,noter,genor):
        return noter in self.generator and genor in self.generator[noter]

    def isEnd(self):
        return len(self.ableNoterminal)==0 and len(self.ableTerminal)==0

    def print(self):
        print("State number "+str(self.number))
        for m in self.generator:
            for n in self.generator[m]:
                print(m+'-->'+n,end=" ")
            print("")


if __name__ == '__main__':
    test = LL_Grammer()
   # test.killLeftCur()  # 消去直接左递归
    test.initGeneList()
  #  test.printGenerator()  # 打印所有产生式
    test.getFirstCollect()  # 获取First集
  #  test.printFirstcollect()  # 打印First集
    test.getFollowCollect()  # 获取Follow集
   # test.printFollowcollect()  # 打印Follow集

    test.extensionGrammer()
    test.constructDFA()
    test.removeMultiState()
    test.printDFA()
    test.printAnalazyTable()

    test.analyze("n+n*n")
    test.analyze("6-3*(2+7)")
    test.analyze("8*(5/3+2)")
    test.analyze("6*5-4/3")


    while True:
        print("")
        testStr = input(
            '''Please input your analyze string:(like 4*6+3 , input 'end' can quit): ''')
        if testStr == "end":
            print("bye")
            break
        test.analyze(testStr)

