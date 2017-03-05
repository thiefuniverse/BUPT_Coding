import threading

import time


start = time.time()
readLock = threading.Lock()
writeLock = threading.Lock()

mm=threading._allocate_lock()

def currentTime():
    end = time.time()
    return end - start


class Person(threading.Thread):

    def __init__(self, num, id, start, still):
        '''present a writer or reader'''
        super(Person,self).__init__()
        self.num = num
        self.startTime = start
        self.id = id
        self.stillTime = still
        self.timer = 0


    def exe(self):
        global  isReading

        if self.id == "R":
            isReading=isReading+1
            print(str(float(currentTime()))+"   reader "+str(self.num)+"   reading "+str(self.stillTime)+"s.....")
        else:
            print(str(float(currentTime()))+"   writer "+str(self.num)+"    writing "+str(self.stillTime)+"s.....")
        time.sleep(float(self.stillTime))

        if self.id == "R":
            isReading=isReading-1
            print(str(float(currentTime())) + "   reader " + str(self.num) + "   reading " + "   completed")
        else:
            print(str(float(currentTime())) + "   writer " + str(self.num) + "    writing " + "   completed")

    def run(self):
        global  config
        global  writerWaiting
        waiting = True
        if self.id == "R":
            while(waiting):
                if currentTime() >= float(self.startTime):
                    if config=="R":    # Reader First: reader
                        if not writeLock.locked():
                            if readLock.locked():
                                self.exe()
                                waiting = False
                            else:
                                readLock.acquire()
                                self.exe()
                                while(isReading>0):
                                    pass
                                readLock.release()
                                waiting = False
                    else:        # Writer First: reader
                        if writeLock.locked():
                            while(writerWaiting>0):
                                pass
                            writeLock.acquire()
                            self.exe()
                            writeLock.release()
                            #readLock.release()
                            waiting = False
                        elif readLock.locked():
                            while (writerWaiting > 0):
                                pass
                            self.exe()
                            waiting = False
                        else:
                            readLock.acquire()
                            self.exe()
                            while(isReading>0):
                                pass
                            readLock.release()
                            waiting = False

        else:
            while(waiting):
                if currentTime() >= float(self.startTime):
                    if config=="R":   # Reader First: reader
                        if readLock.acquire() and writeLock.acquire():
                            self.exe()
                            readLock.release()
                            writeLock.release()

                            waiting = False
                    else:   # Writer First: writer
                        if writeLock.locked():
                            writerWaiting += 1
                            writeLock.acquire()
                            self.exe()
                            writeLock.release()
                            writerWaiting-=1
                            waiting = False
                        else:
                            writerWaiting += 1
                            readLock.acquire()
                            writeLock.acquire()
                            self.exe()
                            writeLock.release()
                            readLock.release()
                            writerWaiting -= 1
                            waiting=False

def loadFile(fileName, allpersons):
    readF = open(fileName)
    for m in readF.readlines():
        data = m.split()
        newPerson = Person(data[0], data[1], data[2], data[3])
        allpersons.append(newPerson)


persons = []


loadFile("configRW.txt", persons)
config = input("Reader First (R) or Writer First (W) ? : ")
while config != 'R' and config != 'W':
    config = input("not valid input,please reinput: ")
isReading=0
writerWaiting=0
for a in persons:
    a.start()
