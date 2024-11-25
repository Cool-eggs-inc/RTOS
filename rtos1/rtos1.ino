#include <vector>
#include <functional>
#include <chrono>

class RTOS
{
public:
bool stop;
    RTOS() : stop(false), timer(0) {}

    void start()
    {
        stop = false;
    }

    void stop()
    {
        stop = true;
    }

    void addTask(std::function<void()> task, std::chrono::milliseconds interval)
    {
        tasks.push_back({task, interval});
    }

private:
    std::vector<std::pair<std::function<void()>, std::chrono::milliseconds>> tasks;
    std::chrono::milliseconds timer;

    void run()
    {
        while (!stop)
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
};

RTOS os;

void setup()
{
    os.start();
}

void loop()
{
    os.addTask([]() { Serial.println("Task 1"); }, std::chrono::seconds(1));
    os.addTask([]() { Serial.println("Task 2"); }, std::chrono::seconds(2));

    while (true)
    {
        os.run();
    }
}
