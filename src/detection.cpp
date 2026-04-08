#include "lab_08.h"
#include <onnxruntime_cxx_api.h>

class YoloModel{
    private:


    public:
        YoloModel() {

        }

        ~YoloModel() {

        }

        bool YoloInference() {

        }
}

Ort::Env env(ORT_LOGGING_LEVEL_WARNING, "F110_Inference");
Ort::SessionOptions session_options;

//make a function to initalize the model
/*

*/


//make a model inference function
//do we need a helped first

//detector function
//take the model and frame 
//if there is a detection, pass to the bounder to read the box
//best way to avoid doing infernece twice!
//global pointers? or something else

/*
Returns, 2D vector!
Basically, take the model and frame as an input (reference), maybe scale?

scale down image to desired resolution
original 960 x 540
scaled: 640 x 360

perform inference, output bounding box values

if nothing detected, return (check how to do this)

when detected,
    get the bottom row of the matrix
    get the index of the center
    return the pixel coordinates (in inference frame)

    expand to original resolution
    return vector
*/