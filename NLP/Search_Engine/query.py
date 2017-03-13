import re
from indexData import oneDoc

def transferWords(str):
    notAlpha = re.compile("[\W]+")
    queryStr = re.sub(notAlpha, " ", str)
    queryWords = queryStr.split()
    return queryWords


class QueryStr:
    def __init__(self,allIndex,allDocs):
        '''query from allIndex files'''
        self.allIndex=allIndex
        self.curResult={}
        self.allDocs=allDocs

    def query(self,str):
        queryWords=transferWords(str)
        fileResultLists=set()

        resultDocs=set()

        for word in queryWords:
            for key in self.allIndex[word]:
                fileResultLists.add(key)
                resultDocs.add(self.allDocs[key])  #get all documents for results


        # sort results by its weights
        resultDocs = list(resultDocs)
        for resDoc in resultDocs:
            resDoc.updateWeight(queryWords)
        resultDocs=sorted(resultDocs, key=lambda oneDoc: oneDoc.weight,reverse=True)

        print("文件及文件内关键字符串提取如下：")
        for doc in resultDocs:
            res=doc.getLongSentance(queryWords)  # get a sub string about these words for this doc
            if res:
                print(doc.fileName+"  "+res)
        #self.showResult(fileResultLists)

    def showResult(self,fileList):
        for file in fileList:
            print(file)


