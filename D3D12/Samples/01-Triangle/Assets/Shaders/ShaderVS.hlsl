struct Attributes
{
    float4 position : POSITION;
    float4 color : COLOR;
};

struct Varyings
{
    float4 position : SV_Position;
    float4 color : COLOR;
};

Varyings main(Attributes input)
{
    Varyings output = (Varyings) 0;
    output.position = input.position;
    output.color = input.color;
    return output;
}
