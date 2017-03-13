import indexData
import query


indexData.getStopWords("stopWords.txt")
allIndexs = indexData.createIndex()
allIndexs.loadAllFiles(["./data/"])
queryTool=query.QueryStr(allIndexs.allIndex,allIndexs.allDocs)
queryTool.query("i am good")