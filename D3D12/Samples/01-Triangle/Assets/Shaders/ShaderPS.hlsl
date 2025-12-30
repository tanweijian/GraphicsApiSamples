struct Varyings
{
    float4 position : SV_Position;
    float4 color : COLOR;
};

float4 main(Varyings input) : SV_Target
{
    return input.color;
}
