#ifndef ERROR_H
#define ERROR_H


#include <vector>

class Error{

public:
    std::vector<int> bitsTrans;
    std::vector<int> bitsReceive;
    
    Error(){};
    ~Error(){};

    void BitsErorr();
    void BytesErorr();

private:


}

#endif
