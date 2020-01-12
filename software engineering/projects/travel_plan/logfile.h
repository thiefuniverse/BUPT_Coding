#ifndef LOGFILE_H
#define LOGFILE_H

#include<fstream>
#include"traveller.h"


/*生成记录文档*/
class LogFile
{
public:
    /*文档记录对象构造函数*/
    LogFile();

    /*文档记录对象析构函数*/
    ~LogFile();

    /*每到达一个城市，在旅行途中，将旅客状态写入日志文档中*/
    bool writeLogFile(Traveller Customer);
    //return value :表示写入文档是否成功

    /*旅客更改计划时立即记录一次计划变更信息*/
    bool updatePlanLogFile(Traveller Customer);
    //return value ：表示是否写入更新计划成功

private:

};

#endif // LOGFILE_H
