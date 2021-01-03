#include <tbb/task.h>

class MyTask : public tbb::task
{
public:
    MyTask(int& result)
      : m_result(result)
    {}

    tbb::task* execute()
    {
        m_result = 5;
        return nullptr;
    }

private:
    int& m_result;
};

int
main(int argc, char** argv)
{
    int result;

    // Allocate a root level task.
    MyTask& myTask = *(new (tbb::task::allocate_root()) MyTask(result));

    // Schedule task for execution, and wait for it to finish.
    tbb::task::spawn_root_and_wait(myTask);

    printf("The answer is %i!\n", result);

    return EXIT_SUCCESS;
}
