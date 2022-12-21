from random import randint, shuffle
from PIL import Image
# inputData
# "nOfSeeds" : total number of seeds
# "width" : width of image
# "height" : height of image
# "maxSpeed" : max grow speed of seed

directions = [(0,-1,"up"),(0,1,"down"),(1,0,"right"),(-1,0,"left")] # direction list
dict = {"up":0,"down":1,"right":2,"left":3} # direction dictory
colors = [75, 90, 105, 120] # list of colors

def calculateImage(inputData, filename: str): # function to calculate new image
    img = Image.new('RGB',(inputData["width"],inputData["height"]), "black") # create new PIL image
    pixels = img.load() # load data into pixels matrix
    seeds = [] # list to save all seeds
    for _ in range(inputData["nOfSeeds"]): # create n seeds
        seedData = {} # create dictionary
        seedData["x"] = randint(0,inputData["width"]-1) # random x pos
        seedData["y"] = randint(0,inputData["height"]-1) # random y pos
        seedData["speeds"] = [randint(1,inputData["maxSpeed"]) for _ in range(4)] # random speed
        seedData["enabled"] = False
        seeds.append(seedData) # append seed to list
        pixels[seedData["x"],seedData["y"]] = (1,1,1) # color start of seed black
    pixelCount = inputData["nOfSeeds"]
    c = 5
    while pixelCount < inputData["width"]*inputData["height"]:
        newseeds = [] # create new list of seeds
        for seed in seeds:
            if seed["enabled"]: # seed is enabled
                shuffle(directions)
                for (x,y,direction) in directions:
                    speed = seed["speeds"][dict[direction]] # get speed of direction
                    pixelToAdd = 0
                    breaked = False
                    for j in range(1, speed+1):
                        # Check if field is free and in picture
                        if seed["x"]+(x*j) >= 0 and seed["x"]+(x*j) < inputData["width"] and seed["y"]+(y*j) >= 0 and seed["y"]+(y*j) < inputData["height"] and pixels[seed["x"]+(x*j),seed["y"]+(y*j)] == (0,0,0):
                            pixels[seed["x"]+(x*j),seed["y"]+(y*j)] = tuple([colors[dict[direction]] for _ in range(3)]) # color new pixel
                            newSeed = seed.copy() # copy seed
                            newSeed["x"] += j*x # change x
                            newSeed["y"] += j*y # change y
                            newseeds.append(newSeed.copy()) # append new seed to list
                        else:
                            pixelToAdd += j-1
                            breaked = True
                            break
                    if not(breaked):
                        pixelCount += speed # add full speed to pixelCount
                    else:
                        pixelCount += pixelToAdd # add possible speed to pixelCount
                    # prinout status
                    if ((pixelCount / (inputData["width"]*inputData["height"]))*100) > c:
                        print(int((pixelCount / (inputData["width"]*inputData["height"]))*100), "%", "abgeschlossen")
                        c+=5
            else:
                newSeed = seed.copy()
                if randint(1,(inputData["width"]*inputData["height"])//20000) == 1:
                    newSeed["enabled"] = True
                newseeds.append(newSeed.copy()) 
        seeds = newseeds.copy()

    img.save("../beispielausgaben/beispiel"+filename+".png")

def readFile():
    print("Enter number of example: ",end="")
    filename = input() # get file number
    data = {} # init data dictionary
    try:
        with open("../beispieleingaben/beispiel"+filename+".txt") as f: # open input file
            for line in f.readlines():
                line = line.strip()
                data[line.split(" -> ")[0]] = int(line.split(" -> ")[1]) # save line into data dictionary
    except:
        print("Invalid file")
        exit(1)
    return data, filename

def solve():
    inputData, filename = readFile()
    calculateImage(inputData, filename)

if __name__ == '__main__':
    solve()