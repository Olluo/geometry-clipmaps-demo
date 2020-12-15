#version 410 core

layout (location = 0) in vec2 inVert;
uniform samplerBuffer tex;
uniform mat4 MVP;
// Used for scaling data to position the vertices in the correct place in world
// scaleFactor.x = x position in world
// scaleFactor.y = y position in world
// scaleFactor.z = the scale of the clipmap level
// scaleFactor.w = 1 / width of clipmap
uniform vec4 scaleFactor;
// Used for texture lookup and blending
// scaleFactor.x = x position on footprint
// scaleFactor.y = y position on footprint
// scaleFactor.z = width of clipmap
// scaleFactor.w = unused
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
  // move the vertex and scale compared to where the clipmap is meant to be
  vec2 worldPos = (inVert + scaleFactor.xy) * scaleFactor.zz;
  // move the vertex to correct position in local clipmap coords then add
  // half width so all values are positive for texture lookup
  vec2 uv = inVert + fineBlockOrigin.xy + (0.5 * fineBlockOrigin.zz);;

  // sample the vertex texture
  vec3 texel = texelFetch(tex, int(uv.y * fineBlockOrigin.z + uv.x)).xyz;

  // unpack to obtain zf and zd = (zc - zf)    
  //  zf is elevation value in current (fine) level    
  //  zc is elevation value in coarser level    
  // float zf = floor(zf_zd);
  // float zd = frac(zf_zd) * 512 - 256; // (zd = zc - zf) 

  // // compute alpha (transition parameter) and blend elevation    
  // float2 alpha = clamp((abs(worldPos - ViewerPos) – AlphaOffset) * OneOverWidth, 0, 1);

  // alpha.x = max(alpha.x, alpha.y);
  // float z = zf + alpha.x * zd;
  float z = texel.r;
  z = z * 256.0f;

  // output.pos = mul(float4(worldPos.x, worldPos.y, z, 1), WorldViewProjMatrix);
  
  // output.uv = uv;
  // output.z = z * ZTexScaleFactor;
  // output.alpha = alpha.x;

  // vec4 worldPosFinal = vec4(worldPos.x, zf_zd, worldPos.y, 1.0f);
  // TODO: negative z so image is correct way round 
  vec4 worldPosFinal = vec4(worldPos.x, worldPos.y, -z, 1.0f);

  // calculate the vertex position
  gl_Position = MVP * worldPosFinal;
  // pass the UV values to the frag shader
  vertColour=texel.rrr;
}