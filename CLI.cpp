#include "CLI.h"

CLI::CLI(DefaultIO* dio) {
    this->dio = dio;
}

void CLI::insertCommands(commandsSharedInfo* csi) {
    Command* first = new uploadCsv(dio, csi);
   Command* second = new algorithmSetting(dio,csi);
    Command* third = new detectAnomalies(dio,csi);
    Command* four = new displayResults(dio,csi);
    Command* five = new uploadAnomalies(dio,csi);
    Command* six = new Exit(dio);

    menu.push_back(first);
    menu.push_back(second);
    menu.push_back(third);
    menu.push_back(four);
    menu.push_back(five);
    menu.push_back(six);


}
void CLI::start(){
    string s = "Welcome to the Anomaly Detection Server.\nPlease choose an option:\n1.upload a time series csv file\n2.algorithm settings\n"
    "3.detect anomalies\n4.display results\n5.upload anomalies and analyze results\n6.exit\n";
    commandsSharedInfo* csi = new commandsSharedInfo();
    int option;
    std::string str;
    insertCommands(csi);
    while(str != "6") {
        dio->write(s);
        str = dio->read();
        option = stoi(str) - 1;
        menu[option]->execute();
    }
}


CLI::~CLI() {
}

