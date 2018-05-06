#include "Uniforms.glsl"
#include "Samplers.glsl"
#include "Transform.glsl"
#include "ScreenPos.glsl"
#include "Fog.glsl"

varying vec2 vTexCoord;
varying vec4 vWorldPos;
#ifdef VERTEXCOLOR
    varying vec4 vColor;
#endif
#ifdef INTERSECTION
    varying vec4 vScreenPos;
#endif

#ifdef COMPILEPS
    uniform float cHighlightThresholdMax;
    uniform vec4 cHighlightColor;   
#endif

#line 21

void VS()
{
    mat4 modelMatrix = iModelMatrix;
    vec3 worldPos = GetWorldPos(modelMatrix);
    gl_Position = GetClipPos(worldPos);
    vTexCoord = GetTexCoord(iTexCoord);
    vWorldPos = vec4(worldPos, GetDepth(gl_Position));

    #ifdef INTERSECTION
        vScreenPos = GetScreenPos(gl_Position);
    #endif

    #ifdef VERTEXCOLOR
        vColor = iColor;
    #endif

}

void PS()
{
    // Get material diffuse albedo
    #ifdef DIFFMAP
        vec4 diffColor = cMatDiffColor;//* texture2D(sDiffMap, vTexCoord);
        #ifdef ALPHAMASK
            if (diffColor.a < 0.5)
                discard;
        #endif
    #else
        vec4 diffColor = cMatDiffColor;
    #endif

    #ifdef VERTEXCOLOR
        diffColor *= vColor;
    #endif

    // Get fog factor
    #ifdef HEIGHTFOG
        float fogFactor = GetHeightFogFactor(vWorldPos.w, vWorldPos.y);
    #else
        float fogFactor = GetFogFactor(vWorldPos.w);
    #endif

    #ifdef INTERSECTION
        #ifdef HWDEPTH
            float BackgroundDepth = ReconstructDepth(texture2DProj(sDepthBuffer, vScreenPos).r);
        #else
            float BackgroundDepth = DecodeDepth(texture2DProj(sDepthBuffer, vScreenPos).rgb);
        #endif

        float fragmentDepth = vWorldPos.w;
        float diff = abs((BackgroundDepth - fragmentDepth) * (cFarClipPS - cNearClipPS));
        diff = diff / cHighlightThresholdMax;
        
        if(diff <= 1.0)
        {
          diffColor = mix(cHighlightColor, diffColor, vec4(diff, diff, diff, diff));
        }
            
    #endif

    gl_FragColor = vec4(GetFog(diffColor.rgb, fogFactor), diffColor.a);
}
