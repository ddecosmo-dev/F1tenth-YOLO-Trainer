import torch
import torch.onnx
from model import F110_YOLO
#import tensorrt as trt

#intialize pytorch and model class
device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')
model = F110_YOLO()

#load the pt file
modelPath = "model_54.pt"
model.load_state_dict(torch.load(modelPath, map_location = device))

model.to(device)
model.eval()

#define dummy input (check resolution!)
dummy_input = torch.randn(1, 3, 180, 320).to(device)

# convert to ONNX
onnx_file_path = "f110_model.onnx"

torch.onnx.export(
    model,                      # The model to be converted
    dummy_input,                # A sample input
    onnx_file_path,             # Where to save the result
    export_params=True,         # Store the trained parameter weights inside the file
    opset_version=12,           # Use a stable ONNX version
    do_constant_folding=True,   # Optimization: fold constant nodes
    input_names=['input'],      # Name the input node for later use in ONNX Runtime
    output_names=['output'],    # Name the output node
    dynamic_axes={              # Allow for flexible batch sizes
        'input': {0: 'batch_size'},
        'output': {0: 'batch_size'}
    }
)

print(f"Model successfully converted to {onnx_file_path}")

#Save TensorRT file to config dir
# needs to be version 8.5

# def build_engine(onnx_path, engine_path, use_fp16=False):
#     logger = trt.Logger(trt.Logger.ERROR)
#     builder = trt.Builder(logger)
#     network = builder.create_network(1 << int(trt.NetworkDefinitionCreationFlag.EXPLICIT_BATCH))
#     parser = trt.OnnxParser(network, logger)
#     config = builder.create_builder_config()
    
#     config.set_memory_pool_limit(trt.MemoryPoolType.WORKSPACE, 1 << 30)

#     if use_fp16:
#         if builder.platform_has_fast_fp16:
#             config.set_flag(trt.BuilderFlag.FP16)
#         else:
#             print("FP16 not supported on this device. Falling back to FP32.")

#     with open(onnx_path, 'rb') as model:
#         if not parser.parse(model.read()):
#             for error in range(parser.num_errors):
#                 print(parser.get_error(error))
#             return None

#     serialized_engine = builder.build_serialized_network(network, config)
#     with open(engine_path, 'wb') as f:
#         f.write(serialized_engine)

# # Set use_fp16=True for FP16, or False for FP32
# build_engine("f110_model.onnx", "f110_model.engine", use_fp16=False)