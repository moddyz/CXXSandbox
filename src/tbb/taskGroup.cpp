#include <tbb/task_group.h>

// Computes the N'th fibonacci number.
static int Fibonacci(int n)
{
    if (n >= 2) {
        return Fibonacci(n - 2) + Fibonacci(n - 1);
    } else {
        return n;
    }
}

class FibonacciFunctor
{
public:
    FibonacciFunctor(int n, int& result)
      : m_n(n)
      , m_result(result)
    {}

    void operator()() const { m_result = Fibonacci(m_n); }

private:
    int m_n{ 0 };
    int& m_result;
};

int main(int argc, char** argv)
{
    int fibA, fibB;

    // Create a task group.
    tbb::task_group taskGroup;

    // Schedule tasks for execution.
    taskGroup.run(FibonacciFunctor(10, fibA));
    taskGroup.run(FibonacciFunctor(15, fibB));

    // Wait for tasks to finish.
    taskGroup.wait();

    printf("The answer is %i!\n", fibA);
    printf("The answer is %i!\n", fibB);

    return EXIT_SUCCESS;
}
