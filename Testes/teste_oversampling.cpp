#include "Sampling.hpp"

int main(int argc, char *argv[]){
    DataPointer<double> data = std::make_shared<Data< double > >();
    DataPointer<double> data1 = std::make_shared<Data< double > >();
    
    data->load("iris_mult.csv");
    data1->copy(*data);
    std::cout << *data << std::endl;

    std::shared_ptr<OverSampling<double> > smote = std::make_shared<SMOTE<double> >(1, 0.1);
    (*smote)(data);

    std::cout << *data << std::endl;
    
    return 0;
}