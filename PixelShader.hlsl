TextureCube cube0;
Texture2D pTexture0;    
SamplerState sampler0; 

float4 main(float4 position : SV_POSITION, float4 color : COLOR, float2 texcoord : TEXCOORD) : SV_TARGET //Rendere target
{
    return color * (pTexture0.Sample(sampler0, texcoord)); // + pTexture0.Sample(sampler0, texcoord));
}

float4 TextPS(float4 position : SV_POSITION, float2 texcoord : TEXCOORD) : SV_TARGET
{
    return pTexture0.Sample(sampler0, texcoord);
}

float4 ModelPS(float4 position : SV_POSITION, float4 color : COLOR,  float2 texcoord : TEXCOORD) : SV_TARGET
{ 
    return color * (pTexture0.Sample(sampler0, texcoord));
}

float4 SkyBoxPS(float4 position : SV_POSITION, float4 color : COLOR, float3 texcoord : TEXCOORD) : SV_TARGET
{ 
    return cube0.Sample(sampler0, texcoord);
}

float4 ReflectPS(float4 position : SV_POSITION, float4 color : COLOR, float3 texcoord : TEXCOORD) : SV_TARGET
{
    return color * cube0.Sample(sampler0, texcoord);
}