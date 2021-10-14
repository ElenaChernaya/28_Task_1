#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

std::mutex m;
void result(std::string& name, int& sp, std::vector<std::string>& tableName, std::vector<int>& tableTime)
{

    int distance = 0;
    int time = 1;

    while (distance < 100)
    {
        distance += sp;

        //сохраняем время и имя пловца, по достижении отметки 100
        if (distance > 100)
        {
            distance = 100;
        }

        m.lock();
        if (distance == 100)
        {
            tableTime.push_back(time);
            tableName.push_back(name);
            std::cout << name << " at the finish line!" << std::endl;
        }

        else
        {
            std::cout << name << " swam " << distance << " meters in " << time << " seconds" << std::endl;
            std::cout << "Thread ID: " << std::this_thread::get_id() << std::endl;
            std::cout << std::endl;
        }
        m.unlock();

        ++time;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

int main() {
    int speed;
    std::string name;
    std::vector<int> speedList; //вектор для сохранения скорости пловцов
    std::vector<std::string> nameList; //вектор для сохранения имен плавцов
    std::vector<std::string> tableName; //вектор для таблицы результатов с именами пловцов
    std::vector<int> tableTime; //вектор для таблицы результатов со скоростями пловцов

    std::cout << "Enter swimmer name and speed:\n";

    for (int i = 0; i < 4; ++i)
    {
        std::cin >> name >> speed;
        nameList.push_back(name);
        speedList.push_back(speed);
    }


    std::vector<std::thread> t; //вектор потоков

    for (int i = 0; i < 4; ++i)
    {
        m.lock();
        t.push_back(std::thread(result, std::ref(nameList[i]), std::ref(speedList[i]), std::ref(tableName), std::ref(tableTime)));
        m.unlock();

        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }

    for (int i = 0; i < t.size(); ++i)
    {
        t[i].join();
    }

    //вывод результатов заплыва
    std::cout << "*** Results table ***" << std::endl;
    std::cout << "-------------------------------" << std::endl;
    for (int i = 0; i < tableTime.size(); ++i)
    {
        std::cout << tableName[i] << " swam 100 meters in " << tableTime[i] << " seconds" << std::endl;
    }
    std::cout << "-------------------------------" << std::endl;
}
