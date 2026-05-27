enum class datatypes{
    VARIANT,
    INT,
    FLOAT,
    STRING,
    FUNCTION,
    OTHER
};

struct Variable{
    datatypes type;
    void* ptr;
    size_t size;
};