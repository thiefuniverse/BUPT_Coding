# this is a simple library which implement basic graphics algorithm

import PIL.ImageDraw as ImageDraw
import PIL.Image as Image
import PIL.ImageShow as ImageShow

import matplotlib.pyplot as plt

# Bresenham draw line algorithm
def DrawLine(allList,x1, y1, x2, y2):
    dx = x2 - x1
    dy = y2 - y1
    error = 0
    derror = float(dy) / float(dx)

    xList = []
    yList = []
    y = min(y1,y2)
    x = min(x1,x2)
    maxX = max(x1,x2)
    maxY = max(y1,y2)
    counter = 0
    while x < maxX and y <maxY:
        xList.append(x)
        counter +=1
        error += derror
        if abs(error) >= 0.5:
            error -= 1.0
        x += 0.1
    
    pie = (float(y2)-float(y1))/counter;
    for i in range(counter):
        yList.append(y+i*pie)

    for i in range(counter):
        allList.append(xList[i])
        allList.append(yList[i])
    

def DrawMatrix(allList,x1,y1,width,height):
    x ,y =x1, y1
    while x<width+x1:
        allList.append(x)
        allList.append(y)
        allList.append(x)
        allList.append(y+height)
        x += 0.1
    while y<height+y1:
        allList.append(x1)
        allList.append(y)
        allList.append(x1+width)
        allList.append(y)
        y += 0.1
    
    # plt.scatter(xList,yList)
    # plt.show()


def draw_circle8(allList,xc,yc,x,y):
    
    allList.append(xc+x);
    allList.append(yc+y);
    allList.append(xc-x);
    allList.append(yc+y);
    allList.append(xc+x);
    allList.append(yc-y);
    allList.append(xc-x);
    allList.append(yc-y);
    
    allList.append(xc+y);
    allList.append(yc+x);
    allList.append(xc-y);
    allList.append(yc+x);
    allList.append(xc+y);
    allList.append(yc-x);
    allList.append(xc-y);
    allList.append(yc-x);

# Bresenham draw circle algorithm
def DrawCircle(allList,x1,x2,width):
    d = 3 - 2 * width;
    x = 0
    y = width
    while (x <= y):
        draw_circle8(allList,x1,x2,x,y)
        if (d<0):
            d += (4*x +6)
        else:
            d+= (4*(x-y)+10)
            y-=1
        x+=1
                
    pass
    
def DrawThief(allList):
    DrawMatrix(allList,9,10,300,300)
    DrawLine(allList,30,30,31,60)
    DrawLine(allList,10,50,30,60)
    DrawLine(allList,31,60,33,120)
    DrawLine(allList,31,120,50,100)


    DrawLine(allList,50,30,60,70)
    DrawLine(allList,60,70,61,120)

    DrawLine(allList,61,70,80,73)
    DrawLine(allList,80,73,81,170)
    DrawLine(allList,80,170,75,160)
    DrawLine(allList,61,90,80,92)
    DrawLine(allList,60,110,80,112)
    DrawLine(allList,50,130,80,132)
    DrawLine(allList,90,120,60,140)

    DrawLine(allList,86,110,120,112)
    DrawLine(allList,95,50,94,170)
    DrawLine(allList,85,110,89,120)




    DrawLine(allList,30,200,31,250)
    DrawLine(allList,12,210,30,211)
    DrawLine(allList,12,230,30,231)
    DrawLine(allList,12,250,30,251)

    DrawLine(allList,60,200,61,250)
    DrawLine(allList,70,210,90,211)
    DrawLine(allList,70,230,90,231)
    DrawLine(allList,70,250,90,251)
    # DrawLine(allList,60,30,60,70)
    # DrawLine(allList,60,30,60,70)
    # DrawLine(allList,60,30,60,70)
    # DrawLine(allList,60,30,60,70)
    # DrawLine(allList,30,30,30,60);
    # DrawLine(allList,30,30,30,60);
    # DrawLine(allList,30,30,30,60);
    # DrawLine(allList,30,30,30,60);
    # DrawLine(allList,30,30,30,60);
    # DrawLine(allList,30,30,30,60);

def clearDraw(allList):
    allList[:] = []
    
        


allList = []
im = Image.new("RGB", (400, 400))
draw = ImageDraw.Draw(im)

# # DrawLine
# DrawLine(allList,1, 1, 250, 160)
# DrawLine(allList,250,160,330,290)
# DrawLine(allList,1,1,330,290)

# clearDraw(allList)

# DrawMatrix
#DrawMatrix(allList,40,40,50,70)

#clearDraw(allList)


# DrawCircle
#DrawCircle(allList,90,90,30);

# clearDraw(allList)

# Draw author name
DrawThief(allList)


draw.point(allList, fill=125)
im.show()
# DrawLine(4,5,8,9)
# xps = [0, 7, 8, 9, 2]
# yps = [0, 5, 7, 3, 1]
# plt.scatter(xps, yps)
# plt.show()


# xps = [4, 89, 8, 9, 2]
# yps = [2, 5, 7, 3, 1]
# plt.scatter(xps, yps)
# plt.show()
