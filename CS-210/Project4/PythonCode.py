from collections import Counter

#class to automatically read file and collect info
class frequencyCounter:
    def __init__(self, file):
        self.filePath = file
        self.file = open(self.filePath, 'r')
        self.lines = self.file.readlines()
        self.calculateFrequency()

    def calculateFrequency(self):
        self.counts = Counter(self.lines)
        

    def printFrequency(self):
        for count in self.counts:
            print(count.strip(), ": ", self.counts[count])
    

    def printSpecific(self, name):
        print(name)
        for count in self.counts:
            if count.strip() == name:
                print(count.strip(), ": ", self.counts[count])

    
    def printHistogram(self):
        for count in self.counts:
            print (count.strip(), ": ", ''.ljust(self.counts[count], '*'))

            
#public links to class
def GetFrequency():
    freq = frequencyCounter('source.txt')
    freq.printFrequency()


def findSpecific(name):
    print(name);
    freq = frequencyCounter('source.txt')
    freq.printSpecific(name)


def getHistogram():
    freq = frequencyCounter('source.txt')
    freq.printHistogram()

