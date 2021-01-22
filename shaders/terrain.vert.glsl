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
// The position of the camera (only x, y)
// uniform vec2 viewerPos;
// The highest point in the clipmap - used for colour
uniform float highestPoint;

// ==== Out Data ====
out vec3 vertColour;

void main()
{
  // Calculate world coordinates by translating then scaling based on the position of the clipmap level
  vec2 worldPos = (inVert + footprintLocalPos + clipmapOffsetPos) * vec2(clipmapScale);
  // Calculate uv coordinates for height map lookup
  vec2 uv = inVert + footprintLocalPos;
  // sample the height map texture at the uv coordinates
  vec2 height = texelFetch(heightData, int(uv.y * clipmapD + uv.x)).rg;

  // Unpack the fine and coarse values into their own floats
  float zf = height.r;
  // ==============================================================================
  // This was the code that was supposed to blend the outer regions of each clipmap
  // but unfortuantely I couldn't get it working.

  // float zc = height.g;

  // // Computation for blending heights at the edges of clipmap levels
  // // The transition width where blending will take place at the edges of the clipmap levels
  // float w =  clipmapD / 10.0f;

  // // The amount to offset when calculating the alpha = (((x,y) max - (x,y) min) / 2) - w - 1
  // vec2 xyMin = clipmapOffsetPos * vec2(clipmapScale);
  // vec2 xyMax = (clipmapOffsetPos + vec2(clipmapD) - vec2(1)) * vec2(clipmapScale);
  // vec2 alphaOffset = ((xyMax - xyMin) / 2) - vec2(w - 1);
  // // compute alpha (transition parameter) and blend elevation    
  // vec2 alpha = clamp(((abs(worldPos - viewerPos) - (alphaOffset)) / w), 0, 1);
  // // float2 alpha = clamp((abs(worldPos - ViewerPos) – AlphaOffset) * OneOverWidth, 0, 1);

  // alpha.x = max(alpha.x, alpha.y);
  // float z = zf + alpha.x * zc;
  // ==============================================================================
  float z = zf;
  z = z * 50.0f;

  // vec4 worldPosFinal = vec4(worldPos.x, zf_zd, worldPos.y, 1.0f);
  vec4 worldPosFinal = vec4(worldPos.x, worldPos.y, -z, 1.0f);

  // calculate the vertex position
  gl_Position = MVP * worldPosFinal;
  
  vertColour=vec3(0.0f, (height.r / highestPoint), 0.0f);
}