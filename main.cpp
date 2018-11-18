#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

using namespace rapidjson;
using namespace std;

typedef map<string, vector<string>> FILES;
typedef vector<Document *> DATA;

void saveToFiles(const FILES *obj) {
  for (auto const &x : (*obj)) {
    ofstream outFile("../emails/" + x.first + ".txt");
    for (const auto &e : x.second)
      outFile << e << "\n";
  }
}

void putFileDataInVector(const string path, DATA *data) {
  std::ifstream file("../person.json");

  if (file.is_open()) {
    string line;
    Document *l = NULL;
    while (getline(file, line)) {
      l = new Document();
      l->Parse(line.c_str());
      data->push_back(l);
    }
    file.close();
  }
}

FILES parseVectorToMap(const DATA *data) {
  FILES obj;
  for (int i = 0; i < data->size(); i++) {
    Document *doc = (*data)[i];
    string currentConf = doc->FindMember("confName")->value.GetString();
    for (rapidjson::SizeType k = 0; k < (*doc)["emails"].Size(); k++) {
      const Value &c = (*doc)["emails"][k];
      obj[currentConf].push_back(c.GetString());
    }
  }

  return obj;
}

int main(int argc, char *argv[]) {
  string path = argv[1];

  DATA data;
  putFileDataInVector(path, &data);

  FILES obj = parseVectorToMap(&data);
  saveToFiles(&obj);

  return 0;
}
