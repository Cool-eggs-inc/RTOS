#include <vector>
#include <functional>
#include <chrono>

class RTOS
{
public:

    RTOS() : vstop(false), timer(0) {}

    void start()
    {
        vstop = false;
    }

    void stop()
    {
        vstop = true;
    }

    void addTask(std::function<void()> task, std::chrono::milliseconds interval, bool state)
    {
        tasks.push_back({task, interval, state});
    }

    //std::vector<std::pair<std::function<void()>, std::chrono::milliseconds>> tasks;
    std::vector<std::tuple<std::function<void()>, std::chrono::milliseconds, bool>> tasks;
    std::chrono::milliseconds timer; bool vstop;

    void run()
    {
        while (!vstop)
        {
            for (auto& task : tasks)
            {
                if (timer >= task.second)
                {
                    task.first();
                    timer -= task.second; 
                }
            }
            timer += std::chrono::milliseconds(1);
        }
    }
    private:
};

RTOS os;

void setup()
{
    os.start();
    os.addTask([]() { Serial.println("Task 1"); }, std::chrono::seconds(5000), true);
    os.addTask([]() { Serial.println("Task 2"); }, std::chrono::seconds(2000), true);
}

void loop()
{
    os.run();
}
