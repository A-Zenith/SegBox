//
//
#include "torch/torch.h"
#include "torch/script.h"
#include <string>

using namespace std;

#define PATH "cpp_model.pt"

at::Tensor inference_with_nn(std::vector<torch::jit::IValue> inputs, string path)
{
    torch::jit::script::Module module;
    try
    {
        // Deserialize the ScriptModule from a file using torch::jit::load().
        module = torch::jit::load(path, torch::kCPU);
    }
    catch (const c10::Error& e)
    {
        std::cerr << "error loading the model\n";
    }
    at::Tensor output = module.forward(inputs).toTensor();
    return output;
}
