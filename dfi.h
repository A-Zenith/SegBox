//
// Created by 周锦凡 on 2020/11/8.
//

#ifndef SEG_BOX_DFI_H
#define SEG_BOX_DFI_H

#endif //SEG_BOX_DFI_H

#include <torch/script.h>
#include <torch/torch.h>
#include <string>

#define PATH "cpp_model.pt"

using namespace std;

torch::Tensor inference_with_nn(std::vector<torch::jit::IValue> inputs, string path);
