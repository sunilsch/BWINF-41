from time import time
import re
chars = ["!","?",".","<",">","_","-",";",",","(",")","»","«",":","[Illustration]","'","*","\t"]
def solve():
    found = False
    print("Enter number of example: ", end="")
    filename = input()
    start = time()
    with open('../beispieleingaben/Alice_im_Wunderland.txt', encoding='utf-8') as f:
        buch_text = f.read()
    try:
        with open('../beispieleingaben/stoerung'+filename+'.txt', encoding='utf-8') as f:
            words = f.read().split(" ")
    except:
        print("Invalid number")
        exit(1)
    outputFile = open('../beispielausgaben/stoerung'+filename+'.out', "w", encoding='utf-8')
    print("\nSentence: ","".join((x+" ") for x in words),"\n")
    outputFile.write("Sentence: "+"".join((x+" ") for x in words)+"\n")
    for x in chars:
        buch_text = buch_text.replace(x,"")
    buch_text = re.sub(" +"," ",buch_text)
    buch_text = re.sub(" \n","\n",buch_text)
    buch_word_list = []
    word = ""
    line = 1
    for x in buch_text:
        if x == "\n" or x == " ":
            if word != "":
                buch_word_list.append((word, line))
                word = ""
            if x == "\n":
                line+=1
        else:
            word+=x
    for i, (buch_word, line) in enumerate(buch_word_list):
        if buch_word.lower() == words[0].lower():
            solution = ""
            for j,word in enumerate(words):
                buch_word,line2 = buch_word_list[i+j]
                buch_word = buch_word.lower()
                if word != "_":
                    word = word.lower()
                    if word != buch_word:
                        solution = None
                        break
                solution+=(buch_word+" ")
            if solution != None:
                solution = solution[:-1]
                print("Found sentence at line ",line,"-",line2," : ",solution)
                outputFile.write("Found sentence at line "+str(line)+"-"+str(line2)+" : "+solution+"\n")
                found = True
    stop = time()
    if not found:
        print("No suitable positions found!")
        outputFile.write("No suitable positions found!")
    print("\nTime: ",((stop-start)*1000), " ms")
    outputFile.write("\nTime: "+str(((stop-start)*1000))+" ms")
if __name__ == '__main__':
    solve()