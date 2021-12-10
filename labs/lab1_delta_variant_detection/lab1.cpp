#include <iostream>
#include <fstream>
#include <chrono>

using namespace std::chrono;

struct variant {
  std::string label;
  std::string sequence;
};

bool isMatch(std::string &a, int &aStart, std::string &b, int &bStart) {
  std::string probe1, probe2;
  int probeSize = 100;
  int count = 0;
  for (int i = 0; i < probeSize; i++) {
    if (a[aStart+i] != b[bStart+i]) {
      count++;
    }
    if (count > 1) {
      return false;
    }
  }
  return true;
}

int main() {

  auto start = high_resolution_clock::now();

  int lineCount = 0, probeSize = 100, deltaCount = 0;
  std::string label, sequence, bestProbe;
  int deltaMatch = 0, notDeltaMatch = 0;
  double fpr, fnr, errorRate = 0, previousRate = 100;
  int falsePositive, falseNegative, probeStart, probeStop;
  double bestProbefpr, bestProbefnr, bestProbeER;

  std::string inFile = "covid.txt";
  std::ifstream inputFile(inFile);


  if (inputFile.is_open()) {
    while (inputFile >> label >> sequence) {
      lineCount++;
      if (label == "delta_variant") {
        deltaCount++;
      }
    }
    inputFile.clear();
    inputFile.seekg(0, std::ios_base::beg);
  }
  else {
    std::cout << "could not open file: " << inFile << std::endl;
    return -1;
  }

  variant *seqArray = new variant[lineCount];

  for (int i = 0; i < lineCount; i++) {
    inputFile >> seqArray[i].label >> seqArray[i].sequence;
  }
  inputFile.close();

  for (int i = 0; i < lineCount; i++) {
    if (seqArray[i].label == "delta_variant") {
      for (int j = 0; j < seqArray[i].sequence.size() - probeSize + 1; j++) {
        for (int k = 0; k < lineCount; k++) {
          for (int l = 0; l < seqArray[k].sequence.size() - probeSize + 1; l++) {
            if (isMatch(seqArray[i].sequence, j, seqArray[k].sequence, l)) {
              if (seqArray[k].label == "delta_variant") {
                deltaMatch++;
              }
              else {
                notDeltaMatch++;
              }
              break;
            }
          }
        }
        fpr = (double)notDeltaMatch / (lineCount - deltaCount);
        fnr = (double)(deltaCount - deltaMatch) / deltaCount;
        errorRate = 2.0 * fpr + 1.0 * fnr;
        if (errorRate < previousRate) {
          previousRate = errorRate;
          bestProbe = seqArray[i].sequence.substr(j, probeSize);
          falsePositive = notDeltaMatch;
          falseNegative = deltaCount - deltaMatch;
          bestProbefpr = fpr;
          bestProbefnr = fnr;
          bestProbeER = errorRate;
        }
        deltaMatch = 0;
        notDeltaMatch = 0;
      }
    }
  }

  std::cout << "Best Probe Sequence: \n" << bestProbe << std::endl;
  std::cout << "Number of False Positives: " << falsePositive << std::endl;
  std::cout << "False Positive Rate: "  << bestProbefpr << std::endl;
  std::cout << "Number of False Negatives: " << falseNegative << std::endl;
  std::cout << "False Negative Rate: " << bestProbefnr << std::endl;
  std::cout << "Erorr Rate: " << bestProbeER << std::endl;
  std::cout << "The Worst-Case Running Time: O(N^2 * M^2 * K)" << std::endl;
  std::cout << "The Anticipated Running Time: O([N^2 * M^2 * K]/t)";
  std::cout << " where t is the time saved for break statement when there is a match";
  std::cout << " and the time saved for only picking the delta_variant in the first loop\n";

  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<seconds>(stop - start);
  std::cout << "Time Taken: " << duration.count() << " seconds\n";

  delete [] seqArray;

  return 0;
}
