#include <bits/stdc++.h>

#define pii pair<int,int>
#define tiii tuple<int,int,int>
#define ve vector
#define INF_INT numeric_limits<int>::max()/2

using namespace std;

ifstream inputFile; // input file of example
ofstream outputFile; // output file for text
ofstream outputFileStatistic1; // statistic files
ofstream outputFileStatistic2;
ofstream outputFileStatistic3;

const int dayDuration = 1440; // duration of one full day
const int openDuration = 480; // duration the shop is opened
const int closeDuration = 960; // duration the shop is closed
const int openTime = 540; // time when shop opens
const int closeTime = 1020; // time when shop closes

int n = 3; // best n for all examples is 3
int bestN; // save best n when calculating n
int maxBackMoves; // save max times job can be moved back
int maxScore = INF_INT; // save score to calculate best n

void finishJob(int startTime, int duration, int &time, ve<pii> &waitingTimes, int &maxWaitingTime){  
    int durationSave = duration;
    duration = ((duration / openDuration)*dayDuration) + (duration % openDuration);
    if(startTime > time) time = startTime;
    time += duration;
    int dayTime = time%dayDuration;
    if(dayTime < openTime){
        time += closeDuration;
        dayTime = time%dayDuration;
    }
    if(dayTime > closeTime) time += closeDuration;
    int waitingTime = time - startTime;
    assert((time%dayDuration) >= openTime && (time%dayDuration) <= closeTime);
    waitingTimes.emplace_back(waitingTime, durationSave);
    maxWaitingTime = max(maxWaitingTime, waitingTime);
}

void printTime(int time){
    cout << (time/dayDuration) << " d, " << ((time%dayDuration)/60) << " h, " << ((time%dayDuration)%60) << " m, " << endl;
    outputFile << (time/dayDuration) << " d, " << ((time%dayDuration)/60) << " h, " << ((time%dayDuration)%60) << " m, " << endl;
}

void calculateAndPrintResults(ve<pii> &waitingTimes, int maxWaitingTime, ofstream &outputFileStatistik, bool print){
    int averageWaitingTime = 0;
    for(auto x : waitingTimes){
        averageWaitingTime += x.first;
        if(print) outputFileStatistik << x.second << "," << x.first << endl;
    }
    averageWaitingTime  = averageWaitingTime / waitingTimes.size();
    int score = maxWaitingTime+(averageWaitingTime*5);
    if(maxScore > score){
        maxScore = score;
        bestN = n;
    }
    if(print){
        cout << "Score: " << score << endl;
        cout << "Durchschnittliche Wartezeit: ";
        outputFile << "Durchschnittliche Wartezeit: ";
        printTime(averageWaitingTime);
        cout << "Maximale Wartezeit: ";
        outputFile << "Maximale Wartezeit: ";
        printTime(maxWaitingTime);
    }
}

void firstSolution(queue<pii> data, bool print){
    int time = 0, maxWaitingTime = 0, startTime, duration;
    ve<pii> waitingTimes;
    while(!data.empty()){
        tie(startTime, duration) = data.front();
        data.pop();
        finishJob(startTime, duration, time, waitingTimes, maxWaitingTime);
    }
    calculateAndPrintResults(waitingTimes, maxWaitingTime, outputFileStatistic1, print);
}

void secondSolution(queue<pii> data, bool print){
    int time = 0, maxWaitingTime = 0, nextStartTime = 0, duration, startTime;
    ve<pii> waitingTimes;
    priority_queue<pii, ve<pii>, greater<pii>> pq;
    while(!data.empty() || (data.empty() && !pq.empty())){
        while((nextStartTime <= time || pq.empty()) && !data.empty()){
            tie(startTime, duration) = data.front();
            pq.push(make_pair(duration, startTime));
            data.pop();
            if(!data.empty()) nextStartTime = data.front().first;
        }
        tie(duration, startTime) = pq.top();
        pq.pop();
        finishJob(startTime, duration, time, waitingTimes, maxWaitingTime);
    }
    calculateAndPrintResults(waitingTimes, maxWaitingTime, outputFileStatistic2, print);
}

bool thirdSortMethod(tiii x1, tiii x2){
    if(get<2>(x1) > maxBackMoves && get<2>(x2) > maxBackMoves) return get<2>(x1) > get<2>(x2);
    if(get<2>(x1) > maxBackMoves) return true;
    if(get<2>(x2) > maxBackMoves) return false;
    return x1 < x2;
}

void thirdSolution(queue<pii> data, bool print){
    int time = 0, maxWaitingTime = 0, nextStartTime = 0, duration, startTime, skipped;
    ve<pii> waitingTimes;
    ve<tiii> pq;
    while(!data.empty() || (data.empty() && !pq.empty())){
        while((nextStartTime <= time || pq.empty()) && !data.empty()){
            tie(startTime, duration) = data.front();
            pq.emplace_back(duration, startTime, 0);
            data.pop();
            if(!data.empty()) nextStartTime = data.front().first;
        }
        maxBackMoves = pq.size()*n;
        sort(pq.begin(), pq.end(), thirdSortMethod);
        tie(duration, startTime, skipped) = pq.front();
        pq.erase(pq.begin());
        finishJob(startTime, duration, time, waitingTimes, maxWaitingTime);
        for(int i = 0; i < pq.size(); i++){
            get<2>(pq[i])++;
        }
    }
    calculateAndPrintResults(waitingTimes, maxWaitingTime, outputFileStatistic3, print);
}

void readData(queue<pii> &data){
    string line;
    while(getline(inputFile, line)){
        if(line != " "){
            string startTimeString = (line.substr(0, line.find(" ")));
            line.erase(0, line.find(" ") + 1);
            string durationString = line;
            int startTime = stoi(startTimeString);
            int duration = stoi(durationString);
            data.push(make_pair(startTime, duration));
        }
    }
}

void solve(bool testN){
    queue<pii> data;
    readData(data);
    if(testN){
        for(n = 0; n < 1000; n++){
            thirdSolution(data, false);
        }
        cout << "Best N is: " << bestN << endl;
    } else {
        cout << endl << "Erste Methode von Mark" << endl;
        outputFile << "Erste Methode von Mark" << endl;
        firstSolution(data, true);
        cout << endl << "Zweite Methode von Mark" << endl;
        outputFile << endl << "Zweite Methode von Mark" << endl;
        secondSolution(data, true);
        cout << endl << "Eigene Methode mit n = " << n << endl;
        outputFile << endl << "Eigene Methode" << endl;
        thirdSolution(data, true);
        cout << endl;
    }
}

int main(){
    string filename, istest;
    bool testN;
    cout << "Enter number of example: ";
    cin >> filename;
    cout << "Calculate best n for example or normal run?? (0/1)" << endl;
    cout << "-> ";
    cin >> istest;
    testN = (istest == "0");
    inputFile.open("../beispieleingaben/fahrradwerkstatt"+filename+".txt");
    if(!inputFile.is_open()){
        cout << "Unable to open file" << endl;
        return 0;
    } else {    
        if(!testN){
            outputFile.open("../beispielausgaben/fahrradwerkstatt"+filename+".out");
            outputFileStatistic1.open("../beispielausgaben/fahrradwerkstatt"+filename+"_sta1.csv");
            outputFileStatistic2.open("../beispielausgaben/fahrradwerkstatt"+filename+"_sta2.csv");
            outputFileStatistic3.open("../beispielausgaben/fahrradwerkstatt"+filename+"_sta3.csv");
        }
    }
    solve(testN);

    inputFile.close();
    if(!testN){
        outputFile.close();
        outputFileStatistic1.close();
        outputFileStatistic2.close();
    }
    return 0;
}