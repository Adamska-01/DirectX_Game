struct VOut
{
	float4 position : SV_POSITION;
	float4 color : COLOR; 
    float2 texcoord : TEXCOORD;
};
struct VOutSky
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float3 texcoord : TEXCOORD;
};
struct VOutTX
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
};

cbuffer CBuffer0
{
    matrix worldViewProjection; //64 bytes
    matrix worldView; //64 bytes
    float4 colourModifier; //16 bytes
    float4 directionalLightVector; //16 bytes
    float4 directionalLightColour; //16 bytes
    float4 ambientLightColour; //16 bytes
    float4 pointLightPos; //16 bytes
    float4 pointLightColour; //16 bytes
    float3 pointLightAttenuation; //12 bytes
    float range; //4 bytes 
};// Total: 224 bytes;

VOut main( float4 position : POSITION, float4 color : COLOR, float2 texcoord : TEXCOORD, float3 normal : NORMAL ) 
{
	VOut output; 
    
    output.position = mul(worldViewProjection, position); //mul multiplies a vertex model space position by a matrix
    
    //Point light
    float4 lightVector = pointLightPos - position;
    float pointAmount = dot(normalize(lightVector), normal);
    pointAmount = saturate(pointAmount);    //clamp
    //Attenuation
    float dist = distance(position, pointLightPos);
    float4 pointLightAtt = float4(0.0f, 0.0f, 0.0f, 0.0f);
    if (dist < range)
        pointLightAtt = (pointLightColour * pointAmount) / (pointLightAttenuation[0] + (pointLightAttenuation[1] * dist) + (pointLightAttenuation[2] * dist * dist));

    //Directional light
    float diffuseAmount = dot(directionalLightVector, normal);
    diffuseAmount = saturate(diffuseAmount); //clamp
    
    output.color = ambientLightColour + colourModifier + (directionalLightColour * diffuseAmount) + pointLightAtt;
    output.texcoord = texcoord.xy;
	
    return output;
}
 
VOutTX TextVS(float4 position : POSITION, float2 texcoord : TEXCOORD)
{
    VOutTX output = (VOutTX)0;

    output.position = position;

    output.texcoord = texcoord;

    return output;
}

VOut ModelVS(float4 position : POSITION, float2 texcoord : TEXCOORD, float3 normal : NORMAL)
{
    VOut output = (VOut)0;
    
    float4 defaultColour = { 1.0f, 1.0f, 1.0f, 1.0f };

    output.position = mul(worldViewProjection, position);
    
    //Point light
    float4 lightVector = pointLightPos - position;
    float pointAmount = dot(normalize(lightVector), normal);
    pointAmount = saturate(pointAmount); //clamp
    //Attenuation
    float dist = distance(position, pointLightPos);
    float4 pointLightAtt = float4(0.0f, 0.0f, 0.0f, 0.0f);
    if (dist < range)
        pointLightAtt = (pointLightColour * pointAmount) / (pointLightAttenuation[0] + (pointLightAttenuation[1] * dist) + (pointLightAttenuation[2] * dist * dist));

    //Directional light
    float diffuseAmount = dot(directionalLightVector, normal);
    diffuseAmount = saturate(diffuseAmount); //clamp
      
    output.color = defaultColour + colourModifier + ambientLightColour + (directionalLightColour * diffuseAmount) + pointLightAtt;
  
    output.texcoord = texcoord;
    return output;
}
 
VOutSky SkyBoxVS(float4 position : POSITION, float2 texcoord : TEXCOORD, float3 normal : NORMAL)
{
    VOutSky output = (VOutSky)0;

    output.position = mul(worldViewProjection, position);  
    output.texcoord = position.xyz;
    
    return output;  
}

VOutSky ReflectVS(float4 position : POSITION, float2 texcoord : TEXCOORD, float3 normal : NORMAL)
{
    VOutSky output;

    output.position = mul(worldViewProjection, position);
    
    //Position relative to the camera
    float3 wvpos = mul(worldView, position); 
    
    //surface normla relative to the camera 
    float3 wvNormal = mul(worldView, normal);
    wvNormal = normalize(wvNormal);
    
    //Obtain the reverse eye vector 
    float3 eyer = -normalize(wvpos);
    
    //Compute the reflection vector 
    output.texcoord = 2.0 * dot(eyer, wvNormal) * wvNormal - eyer;
    
    return output;
}