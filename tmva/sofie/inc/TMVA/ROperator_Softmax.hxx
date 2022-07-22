#ifndef TMVA_SOFIE_ROPERATOR_Softmax
 #define TMVA_SOFIE_ROPERATOR_Softmax

 #include "TMVA/SOFIE_common.hxx"
 #include "TMVA/ROperator.hxx"
 #include "TMVA/RModel.hxx"

 #include <sstream>

 namespace TMVA{
 namespace Experimental{
 namespace SOFIE{

 template <typename T>
 class ROperator_Softmax final : public ROperator
 {

 private:

    std::string fNX;
    std::string fNY;
    std::vector<size_t> fShape;

 public:
    ROperator_Softmax(){}
    ROperator_Softmax(std::string nameX, std::string nameY):
       fNX(UTILITY::Clean_name(nameX)), fNY(UTILITY::Clean_name(nameY)){}

    std::vector<ETensorType> TypeInference(std::vector<ETensorType> input){
       return input;
    }

    std::vector<std::vector<size_t>> ShapeInference(std::vector<std::vector<size_t>> input){
       auto ret = input; //suggest copy to compiler
       return ret;
    }

    void Initialize(RModel& model){
       if (model.CheckIfTensorAlreadyExist(fNX) == false){   //input must be a graph input, or already initialized intermediate tensor
          throw std::runtime_error("TMVA SOFIE Softmax Op Input Tensor is not found in model");
       }
       fShape = model.GetTensorShape(fNX);
       model.AddIntermediateTensor(fNY, model.GetTensorType(fNX), fShape);
    }


    std::string Generate(std::string OpName){
       OpName = "op_" + OpName;
       if (fShape.empty()){
          throw std::runtime_error("TMVA SOFIE Transpose Softmax called to Generate without being initialized first");
       }
       std::stringstream out;
       int length = 1;
       for(auto& i: fShape){
          length *= i;
       }
       out << "\n//------ SOFTMAX\n";
       out << SP << "double sum = 0.0;\n";
       out << SP << "for (int id = 0; id < " << length << " ; id++){\n";
       out << SP << SP << "tensor_" << fNY << "[id] = std::exp( - tensor_"  << fNX << "[id]);\n";
       out << SP << SP << "sum += tensor_" << fNY << "[id];\n";
       out << SP << "}\n";
       out << SP << "for (int id = 0; id < " << length << " ; id++){\n";
       out << SP << SP << "tensor_" << fNY << "[id] /= sum;\n";
       out << SP << "}\n";
       return out.str();
    }

 };

 }//SOFIE
 }//Experimental
 }//TMVA


 #endif //TMVA_SOFIE_ROPERATOR_Softmax