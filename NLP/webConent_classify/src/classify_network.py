from bs4 import BeautifulSoup as bp
import numpy as np
import matplotlib.pyplot as plt

############################################################
# activation functions
############################################################


def tanh(x):
    return np.tanh(x)


def tanh_deriv(x):
    return 1.0 - np.tanh(x) * np.tanh(x)


def logistic(x):
    return 1 / (1 + np.exp(-x))


def logistic_deriv(x):
    return logistic(x) * (1 - logistic(x))

############################################################

############################################################
# neural network for content identification
############################################################


class NeuralNetWork:

    def __init__(self, layers, activation, activation_deriv):
        "init a neural network"

        # config activation
        self.activation = activation
        self.activation_deriv = activation_deriv

        # init weights by layers
        self.weights = []
        for i in range(1, len(layers) - 1):
            self.weights.append(
                (2 * np.random.random((layers[i - 1] + 1, layers[i] + 1)) - 1) * 0.25)
            self.weights.append(
                (2 * np.random.random((layers[i] + 1, layers[i + 1])) - 1) * 0.25)


    def saveWeights(self):
        # saveWe=open("weight.txt","w")
        # for m in self.weights:
        #     for n in m:
        #         saveWe.write(n)
        #         saveWe.write(" ")
        #     saveWe.write("\n")
        # saveWe.close()
        pass

    def train(self, trainData, trainOut, learinRate=0.1, epochs=100000):
        ''' training my net args '''
        trainData = np.atleast_2d(trainData)
        tempData = np.ones([trainData.shape[0], trainData.shape[1] + 1])
        tempData[:, 0:-1] = trainData
        trainData = tempData
        trainOut = np.array(trainOut)

        for times in range(epochs):
            # get a random inputData
            inputFlag = np.random.randint(trainData.shape[0])
            inputData = [trainData[inputFlag]]

            for curLayer in range(len(self.weights)):
                inputData.append(self.activation(
                    np.dot(inputData[curLayer], self.weights[curLayer])))
            error = trainOut[inputFlag] - inputData[-1]
            deltas = [error * self.activation_deriv(inputData[-1])]
            for l in range(len(inputData) - 2, 0, -1):
                deltas.append(deltas[-1].dot(self.weights[l].T)
                              * self.activation_deriv(inputData[l]))
            deltas.reverse()

            for i in range(len(self.weights)):
                layer = np.atleast_2d(inputData[i])
                delta = np.atleast_2d(deltas[i])
                self.weights[i] += learinRate * layer.T.dot(delta)

    def classify(self, inputData):
        inputData = np.array(inputData)
        tempIn = np.ones(inputData.shape[0] + 1)
        tempIn[0:-1] = inputData
        inData = tempIn

        for l in range(0, len(self.weights)):
            inData = self.activation(np.dot(inData, self.weights[l]))

        return inData
