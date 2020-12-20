cpp_executable(sharedMemoryProducer

    CPPFILES
        producer.cpp

    LIBRARIES
        rt
)

cpp_executable(sharedMemoryConsumer

    CPPFILES
        consumer.cpp

    LIBRARIES
        rt
)
