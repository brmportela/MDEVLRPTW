#ifndef FILEHELPER_H_INCLUDED
#define FILEHELPER_H_INCLUDED

#include "heuristic/Instance.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <memory>

// Constantes para a leitura das seções do arquivo da instância. Windows e Linux
#ifdef _WIN32

#define NODE_COUNT_SECTION                  "NODE_COUNT_SECTION"
#define DEP_COUNT_SECTION                   "DEP_COUNT_SECTION"
#define DEP_SECTION                         "DEP_SECTION"
#define RECHARGING_STATION_COUNT_SECTION    "RECHARGING_STATION_COUNT_SECTION"
#define RECHARGING_STATION_SECTION          "RECHARGING_STATION_SECTION"
#define CLIENT_COUNT_SECTION                "CLIENT_COUNT_SECTION"
#define CLIENT_SECTION                      "CLIENT_SECTION"
#define VEHICLE_BATTERY_SECTION             "VEHICLE_BATTERY_SECTION"
#define VEHICLE_CAPACITY_SECTION            "VEHICLE_CAPACITY_SECTION"
#define BATTERY_CONSUM_RATE_SECTION         "BATTERY_CONSUM_RATE_SECTION"
#define INVERSE_REFUELING_RATE_SECTION      "INVERSE_REFUELING_RATE_SECTION"
#define AVERAGE_VELOCITY_SECTION            "AVERAGE_VELOCITY_SECTION"
#define GREAT_RESULT                        "GREAT_RESULT"
#define END_SECTION                         "END_SECTION"
#define END_FILE                            "END_FILE"


#else

#define NODE_COUNT_SECTION                  "NODE_COUNT_SECTION\r"
#define DEP_COUNT_SECTION                   "DEP_COUNT_SECTION\r"
#define DEP_SECTION                         "DEP_SECTION\r"
#define RECHARGING_STATION_COUNT_SECTION    "RECHARGING_STATION_COUNT_SECTION\r"
#define RECHARGING_STATION_SECTION          "RECHARGING_STATION_SECTION\r"
#define CLIENT_COUNT_SECTION                "CLIENT_COUNT_SECTION\r"
#define CLIENT_SECTION                      "CLIENT_SECTION\r"
#define VEHICLE_BATTERY_SECTION             "VEHICLE_BATTERY_SECTION\r"
#define VEHICLE_CAPACITY_SECTION            "VEHICLE_CAPACITY_SECTION\r"
#define BATTERY_CONSUM_RATE_SECTION         "BATTERY_CONSUM_RATE_SECTION\r"
#define INVERSE_REFUELING_RATE_SECTION      "INVERSE_REFUELING_RATE_SECTION\r"
#define AVERAGE_VELOCITY_SECTION            "AVERAGE_VELOCITY_SECTION\r"
#define GREAT_RESULT                        "GREAT_RESULT\r"
#define END_SECTION                         "END_SECTION\r"
#define END_FILE                            "END_FILE\r"

#endif

using namespace std;

/**
*   Classe para a leitura de arquivos
**/
class FileHelper
{
    public:
        // Faz a leitura do arquivo de uma instância do problema CVRP
        static std::shared_ptr<Instance> readInstance(string fileName);
};

#endif // FILEHELPER_H_INCLUDED
