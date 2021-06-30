#include "../header/FileHelper.h"

std::shared_ptr<Instance> FileHelper::readInstance(string fileName)
{
    ifstream streamFile;
    streamFile.open(fileName.c_str());

    // Verifica se o arquivo foi aberto
    if (!streamFile.is_open())
    {
        cout << endl << "[ Erro ]: N�o foi poss�vel abrir o arquivo da inst�ncia" << endl;
    }
    else
    {
        int sectionRead = 0;         // Conta as se��es que foram lidas
        int nodeCount = 0;
        int depCount = 0;
        int stationCount = 0;
        int clientCount = 0;
        std::shared_ptr<Instance> instance = make_shared<Instance>();
        string txtLine;

        // L� linha por linha do arquivo
        while (getline(streamFile, txtLine))
        {
            // Verifica se a linha lida � igual a se��o correspondente
            if(strcmp(txtLine.c_str(), string(NODE_COUNT_SECTION).c_str()) == 0)
            {
                streamFile >> nodeCount;
                instance->setVerticeCount(nodeCount);
                sectionRead++;
            }
            else if(strcmp(txtLine.c_str(), string(DEP_COUNT_SECTION).c_str()) == 0)
            {
                streamFile >> depCount;
                instance->setDepCount(depCount);
                sectionRead++;
            }
            else if(strcmp(txtLine.c_str(), string(DEP_SECTION).c_str()) == 0)
            {
                int id;
                float coordX;
                float coordY;
                float demand;
                float readyTime;
                float dueTime;
                float serviceTime;
                // L� as pr�ximas linhas at� o fim da sess�o
                while(getline(streamFile, txtLine) && (strcmp(txtLine.c_str(), string(END_SECTION).c_str())) != 0)
                {
                    stringstream streamLine(txtLine);
                    // L� as informa��es da linha
                    while(streamLine >> id >> coordX >> coordY >> demand >> readyTime >> dueTime >> serviceTime)
                    {
                        instance->addVerticePoint(id, make_shared<Point>(coordX, coordY));
                        instance->addVerticeAngle(id, make_shared<Point>(coordX, coordY));
                        instance->addVerticeInfos(id, demand, readyTime, dueTime, serviceTime);
                    }
                }
                sectionRead++;
            }
            // Esta��es de recarga
            else if(strcmp(txtLine.c_str(), string(RECHARGING_STATION_COUNT_SECTION).c_str()) == 0)
            {
                streamFile >> stationCount;
                instance->setStationCount(stationCount);
                sectionRead++;
            }
            else if(strcmp(txtLine.c_str(), string(RECHARGING_STATION_SECTION).c_str()) == 0)
            {
                int id;
                float coordX;
                float coordY;
                float demand;
                float readyTime;
                float dueTime;
                float serviceTime;
                // L� as pr�ximas linhas at� o fim da sess�o
                while(getline(streamFile, txtLine) && (strcmp(txtLine.c_str(), string(END_SECTION).c_str())) != 0)
                {
                    stringstream streamLine(txtLine);
                    // L� as informa��es da linha
                    while(streamLine >> id >> coordX >> coordY >> demand >> readyTime >> dueTime >> serviceTime)
                    {
                        id = id + depCount;
                        instance->addVerticePoint(id, make_shared<Point>(coordX, coordY));
                        instance->addVerticeAngle(id, make_shared<Point>(coordX, coordY));
                        instance->addVerticeInfos(id, demand, readyTime, dueTime, serviceTime);
                    }
                }
                sectionRead++;
            }
            else if(strcmp(txtLine.c_str(), string(CLIENT_COUNT_SECTION).c_str()) == 0)
            {
                streamFile >> clientCount;
                instance->setClientCount(clientCount);
                sectionRead++;
            }
            else if(strcmp(txtLine.c_str(), string(CLIENT_SECTION).c_str()) == 0)
            {
                int id;
                float coordX;
                float coordY;
                float demand;
                float readyTime;
                float dueTime;
                float serviceTime;
                // L� as pr�ximas linhas at� o fim da sess�o
                while(getline(streamFile, txtLine) && (strcmp(txtLine.c_str(), string(END_SECTION).c_str())) != 0)
                {
                    stringstream streamLine(txtLine);
                    // L� as informa��es da linha
                    while(streamLine >> id >> coordX >> coordY >> demand >> readyTime >> dueTime >> serviceTime)
                    {
                        id = id + depCount + stationCount;
                        instance->addVerticePoint(id, make_shared<Point>(coordX, coordY));
                        instance->addVerticeAngle(id, make_shared<Point>(coordX, coordY));
                        instance->addVerticeInfos(id, demand, readyTime, dueTime, serviceTime);
                    }
                }
                sectionRead++;
            }
            else if(strcmp(txtLine.c_str(), string(VEHICLE_BATTERY_SECTION).c_str()) == 0)
            {
                float batteryCap;
                streamFile >> batteryCap;
                instance->setBatteryCap(batteryCap);
                sectionRead++;
            }
            else if(strcmp(txtLine.c_str(), string(VEHICLE_CAPACITY_SECTION).c_str()) == 0)
            {
                float vehicleCapacity;
                streamFile >> vehicleCapacity;
                instance->setVehicleCapacity(vehicleCapacity);
                sectionRead++;
            }
            else if(strcmp(txtLine.c_str(), string(BATTERY_CONSUM_RATE_SECTION).c_str()) == 0)
            {
                float vehicleConsum;
                streamFile >> vehicleConsum;
                instance->setVehicleConsum(vehicleConsum);
                sectionRead++;
            }
            else if(strcmp(txtLine.c_str(), string(INVERSE_REFUELING_RATE_SECTION).c_str()) == 0)
            {
                float vehicleRefueling;
                streamFile >> vehicleRefueling;
                instance->setVehicleRefueling(vehicleRefueling);
                sectionRead++;
            }
            else if(strcmp(txtLine.c_str(), string(AVERAGE_VELOCITY_SECTION).c_str()) == 0)
            {
                float vehicleVelocity;
                streamFile >> vehicleVelocity;
                instance->setVehicleVelocity(vehicleVelocity);
                sectionRead++;
            }
            else if(strcmp(txtLine.c_str(), string(GREAT_RESULT).c_str()) == 0)
            {
                float gResult;
                streamFile >> gResult;
                instance->setGResult(gResult);
                sectionRead++;
            }
        }
        // Verifica se todas as se��es foram lidas
        if(sectionRead < 12)
        {
            instance = NULL;
            cout << "[ Erro ]: N�o foi poss�vel ler todas as se��es da inst�ncia!" << endl << endl;
        }
        streamFile.close();
        return instance;
    }
    return NULL;
}
