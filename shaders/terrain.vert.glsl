#version 410 core

layout (location = 0) in vec2 inVert;

uniform mat4 MVP;
uniform vec4 scaleFactor;
uniform vec4 fineBlockOrigin;

out vec3 vertColour;

// uniform vec3 camPos;
// struct OUTPUT {    
//   vector pos   : POSITION;
//   float2 uv    : TEXCOORD0; // coordinates for normal-map lookup    
//   float  z     : TEXCOORD1; // coordinates for elevation-map lookup      
//   float alpha  : TEXCOORD2; // transition blend on normal map  
// };

// uniform float4 ScaleFactor, FineBlockOrig;
// uniform float2 ViewerPos, AlphaOffset, OneOverWidth;
// uniform float  ZScaleFactor, ZTexScaleFactor;
// uniform matrix WorldViewProjMatrix;

void main()
{
  // convert from grid xy to world xy coordinates    
  //   scaleFactor.xy: grid spacing of current level      
  //   scaleFactor.zw: origin of current block within world    
  vec2 worldPos = (inVert + scaleFactor.zw) * scaleFactor.xy;
  // compute coordinates for vertex texture    
  //   fineBlockOrig.xy: 1/(w, h) of texture    
  //   fineBlockOrig.zw: origin of block in texture    
  vec2 uv = inVert * fineBlockOrigin.xy + fineBlockOrigin.zw;

  // sample the vertex texture      
  // float zf_zd = tex2Dlod(ElevationSampler, float4(uv, 0, 1));

  // unpack to obtain zf and zd = (zc - zf)    
  //  zf is elevation value in current (fine) level    
  //  zc is elevation value in coarser level    
  // float zf = floor(zf_zd);
  // float zd = frac(zf_zd) * 512 - 256; // (zd = zc - zf) 

  // // compute alpha (transition parameter) and blend elevation    
  // float2 alpha = clamp((abs(worldPos - ViewerPos) – AlphaOffset) * OneOverWidth, 0, 1);

  // alpha.x = max(alpha.x, alpha.y);
  // float z = zf + alpha.x * zd;
  // z = z * ZScaleFactor;

  // output.pos = mul(float4(worldPos.x, worldPos.y, z, 1), WorldViewProjMatrix);
  
  // output.uv = uv;
  // output.z = z * ZTexScaleFactor;
  // output.alpha = alpha.x;

  vec4 worldPosFinal = vec4(worldPos.x, 0.0f, worldPos.y, 1.0f);

  // calculate the vertex position
  gl_Position = MVP * worldPosFinal;
  // pass the UV values to the frag shader
  vertColour=vec3(fineBlockOrigin.x/6, 0.0f, fineBlockOrigin.y/6);
}