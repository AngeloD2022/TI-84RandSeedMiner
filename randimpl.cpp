#include <iostream>
#include <vector>
#include <thread>
#include <algorithm>

using namespace std;

long s1, s2;
long mod1 = 2147483563;
long mod2 = 2147483399;
long mult1 = 40014;
long mult2 = 40692;

// I did not implement this...
void SetSeed(int n) {
    if (n < 0)
        n = -n;
    if (n == 0) {
        s1 = 12345;
        s2 = 67890;
    } else {
        s1 = (mult1 * n) % mod1;
        s2 = n % mod2;
    }
}

void SetSeedAtPosition(int seed, int pos = 0){
    SetSeed(seed);

    if(pos != 0){
        for (int i = 0; i < pos; ++i) {
            long k;
            k = s1 / 53668;
            s1 = 40014 * (s1 - k * 53668) - k * 12211;
            if (s1 < 0)
                s1 = s1 + 2147483563;

            k = s2 / 52774;
            s2 = 40692 * (s2 - k * 52774) - k * 3791;
            if (s2 < 0)
                s2 = s2 + 2147483399;
        }
    }
}

// I did not implement this either...
double Uniform() {
    long Z, k;
    k = s1 / 53668;
    s1 = 40014 * (s1 - k * 53668) - k * 12211;
    if (s1 < 0)
        s1 = s1 + 2147483563;

    k = s2 / 52774;
    s2 = 40692 * (s2 - k * 52774) - k * 3791;
    if (s2 < 0)
        s2 = s2 + 2147483399;

    Z = s1 - s2;
    if (Z < 1)
        Z = Z + 2147483562;

    return Z * (4.656613e-10);
}

int RandInt(int min, int max) {
    if (min < max)
        return (min + floor((max - min + 1) * Uniform()));
    else
        return (max + floor((min - max + 1) * Uniform()));
}

int derivedSeed;
int nextPos = -1;

void startSearch(vector<int> known, int min, int max, int aps, int stt, int ss = 0){
    for (int s = ss; s < (ss + stt); ++s) {
        SetSeed(s);
        vector<int> body;
        for (int n = 0; n < aps; ++n) {
            body.push_back(RandInt(min,max));
        }
        auto iter = search(body.begin(), body.end(),known.begin(),known.end());

        if(iter != body.end()){
            // Seed Found
            nextPos = iter - body.begin();
            derivedSeed = s;
            return;
        }else{

        }

    }
}



int main() {

    cout << "–––––––––– TI-84 \"rand\" Seed Reversal Engine ––––––––––"<<endl<<endl;

    string known;
    int seedstart = 0;
    int min = 0;
    int max = 0;
    int attemptsPerSeed = 0;
    int seedsToTry = 100000;

    cout << "Enter the known comma-separated integer sequence..." << endl;
    cin >> known;
    cout << "Enter minimum..." << endl;
    cin >> min;
    cout << "Enter maximum" << endl;
    cin >> max;
    cout << "Enter the amount of data to generate per seed increment..." << endl;
    cin >> attemptsPerSeed;

    cout << "Please be patient, this could take a hot minute."<<endl<<"Creating thread..."<<endl;

    // tokenize input
    vector<int> parsed;
    size_t last = 0;
    size_t next = 0;
    while((next = known.find(",",last)) != string::npos){
        std::string token = known.substr(last,next-last);
        parsed.push_back(stoi(token));
        last = next + 1;
    }
    parsed.push_back(stoi(known.substr(last)));

    // initialize thread...
    thread task(startSearch,parsed,min,max,attemptsPerSeed,seedsToTry,seedstart);
    cout << "Calculating..."<<endl;
    task.join();

    if(nextPos >= 0){
        cout << "DERIVED SEED: "<< derivedSeed<<endl;
        cout << "Position in seed: "<< nextPos<<endl;

        SetSeedAtPosition(derivedSeed,(nextPos+parsed.size()));
        cout << "--- Next 50 values ---"<<endl;
        for (int i = 0; i < 50; ++i) {
            cout<<RandInt(min,max)<<" ";
        }

    }else{
        cout << "Not able to derive this seed. Try increasing the amount of data per seed increment.";
    }
}