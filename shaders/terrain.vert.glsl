#version 450 core
// ==== Buffered Data ====
// Buffered vertex in data
layout (location = 0) in vec2 inVert;

// ==== Texture Buffers ====
// The height data stored in a texture buffer
uniform samplerBuffer heightData;

// ==== Uniforms ====
// The model-view-projection matrix
uniform mat4 MVP;
// The location of the footprint in its clipmap's local coords
uniform vec2 footprintLocalPos;
// The offset of the clipmap level in the world
uniform vec2 clipmapOffsetPos;
// The scale of the clipmap level
uniform float clipmapScale;
// The width of the clipmap
uniform float clipmapD;

// ==== Out Data ====
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
  // Move the vertex and scale compared to where the clipmap is meant to be
  vec2 worldPos = (inVert + footprintLocalPos + clipmapOffsetPos) * vec2(clipmapScale);
  // Move the vertex to correct position in local clipmap coords then 
  // add half width so all values are positive for texture lookup
  vec2 uv = inVert + footprintLocalPos + vec2(0.5 * clipmapD);;

  // sample the vertex texture
  vec3 texel = texelFetch(heightData, int(uv.y * clipmapD + uv.x)).xyz;

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
  // vertColour=texel.rrr;
  vertColour=vec3(clipmapScale / 10, texel.r, 0.8f);
}