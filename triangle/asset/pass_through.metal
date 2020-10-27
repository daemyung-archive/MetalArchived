#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

struct Input {
    float3 position [[attribute(0)]];
    float3 color    [[attribute(1)]]; 
};

struct Output
{
    float4 clipSpacePosition [[position]];
    float3 color;
};

vertex Output VSMain(Input input [[stage_in]]) {
    Output output;
    output.clipSpacePosition = float4(input.position, 1.0);
    output.color = input.color;
    return output;
}

fragment float4 FSMain(Output input [[stage_in]])
{
    return float4(input.color, 1.0);
}
