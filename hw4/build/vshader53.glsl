#version 150

in  vec4 vPosition;
in  vec3 vNormal;
in  vec3 vColor;
in  vec2 vTexCoord;
out vec4 color;
out vec3 pointPos;
out vec2 fTexCoord;
out float fSphereTexCoord;
out vec2 fSphereTexCoord2;
out vec2 latticeSphereTexCoord2;
out float  fSphereLattice;

uniform vec4 GlobalAmbientProduct, AmbientProduct, DiffuseProduct, SpecularProduct;
uniform vec4 PAmbientProduct, PDiffuseProduct, PSpecularProduct;
uniform mat4 ModelView;
uniform mat4 Projection;
uniform mat3 Normal_Matrix;
uniform vec4 LightPosition;  // In Eye Frame 
uniform vec3 lightdirection;
uniform float Shininess;

uniform float ConstAtt;  // Constant Attenuation
uniform float LinearAtt; // Linear Attenuation
uniform float QuadAtt;   // Quadratic Attenuation

uniform float cosangle;
uniform vec4  SpotlightPoint;
uniform float expo;

uniform float hasGlobalLight;
uniform float hasDirectionalLight;
uniform float hasPointLight;
uniform float hasSpotLight;

uniform int texIsVertical;
uniform int texIsEyeFrame;
uniform float sphereLattice;

void main()
{   
    color = vec4(0);
    vec3 pos = (ModelView * vPosition).xyz;

    if(hasGlobalLight>0) color += GlobalAmbientProduct;
    if(hasDirectionalLight>0){
        vec3 L = normalize(-lightdirection);
        vec3 E = normalize(-pos);
        vec3 H = normalize(L+E);

        vec3 N = normalize(Normal_Matrix * vNormal);
        if(dot(N,E)<0) N = -N;

        float attenuation = 1.0;
        vec4 ambient = AmbientProduct;
        float d = max(dot(L,N), 0.0);
        vec4 diffuse = d * DiffuseProduct;
        float s = pow(max(dot(N,H),0.0), Shininess);
        vec4 specular = s * SpecularProduct;
        if (dot(L,N)<0.0) specular = vec4(0.0, 0.0, 0.0, 1.0);
        gl_Position = Projection * ModelView * vPosition;   
        color += attenuation * (ambient + diffuse + specular);
    }
    if(hasPointLight>0){
        vec3 L = normalize(LightPosition.xyz - pos);
        vec3 E = normalize(-pos);
        vec3 H = normalize(L+E);
        vec3 N = normalize(Normal_Matrix * vNormal);
        if(dot(N,E)<0) N = -N;
        float dist = length(LightPosition.xyz-pos);
        float attenuation = 1.0/(ConstAtt+LinearAtt*dist+QuadAtt*dist*dist);

        vec4 ambient = PAmbientProduct;
        float d = max(dot(L,N), 0.0);
        vec4 diffuse = d * PDiffuseProduct;
        float s = pow(max(dot(N,H),0.0), Shininess);
        vec4 specular = s * PSpecularProduct;
        if (dot(L,N)<0.0) specular = vec4(0.0, 0.0, 0.0, 1.0);
        gl_Position = Projection * ModelView * vPosition;
        color += attenuation * (ambient + diffuse + specular);
    }
    if(hasSpotLight>0){
        vec3 L = normalize(LightPosition.xyz - pos);
        vec3 E = normalize(-pos);
        vec3 H = normalize(L+E);
        vec3 N = normalize(Normal_Matrix * vNormal);
        if(dot(N,E)<0) N = -N;
        float dist = length(LightPosition.xyz-pos);
        float attenuation = 1.0/(ConstAtt+LinearAtt*dist+QuadAtt*dist*dist);

        vec4 ambient = PAmbientProduct;
        float d = max(dot(L,N), 0.0);
        vec4 diffuse = d * PDiffuseProduct;
        float s = pow(max(dot(N,H),0.0), Shininess);
        vec4 specular = s * PSpecularProduct;
        if (dot(L,N)<0.0) specular = vec4(0.0, 0.0, 0.0, 1.0);
        gl_Position = Projection * ModelView * vPosition;
        vec3 Lf = normalize(SpotlightPoint.xyz - LightPosition.xyz);
        float spoti = 0.0;
        if(dot(Lf,-L)>=cosangle) spoti = pow(dot(Lf,-L),expo);
        color += attenuation * (ambient + diffuse + specular) * spoti;
    }
    if(hasSpotLight+hasPointLight+hasGlobalLight+hasDirectionalLight == 0.0){
        gl_Position = Projection * ModelView * vPosition;
        vec4 vColor4 = vec4(vColor.r, vColor.g, vColor.b, 1.0); 
        color = vColor4;
    }
    pointPos = pos;
    fTexCoord = vTexCoord;
    if(texIsEyeFrame==0){ //sphere Frame
        if(texIsVertical==1){
            fSphereTexCoord = vPosition[0]*2.5;
            fSphereTexCoord2 = vec2(0.75*(vPosition[0]+1),0.75*(vPosition[1]+1));
            }
        else{
            fSphereTexCoord = (vPosition[0]+vPosition[1]+vPosition[2])*1.5;
            fSphereTexCoord2 = vec2(0.45*(vPosition[0]+vPosition[1]+vPosition[2]),0.45*(vPosition[0]-vPosition[1]+vPosition[2]));
            }
        
        if(sphereLattice == 1){
            latticeSphereTexCoord2 = vec2(0.5*(vPosition[0]+1),0.5*(vPosition[1]+1));
        }else{
            latticeSphereTexCoord2 = vec2(0.3*(vPosition[0]+vPosition[1]+vPosition[2]),0.3*(vPosition[0]-vPosition[1]+vPosition[2]));
        }
    }else{
        if(texIsVertical==1){
            fSphereTexCoord = pos[0]*2.5;
            fSphereTexCoord2 = vec2(0.75*(pos[0]+1),0.75*(pos[1]+1));
            }
        else{
            fSphereTexCoord = (pos[0]+pos[1]+pos[2])*1.5;
            fSphereTexCoord2 = vec2(0.45*(pos[0]+pos[1]+pos[2]),0.45*(pos[0]-pos[1]+pos[2]));
            }
        
        if(sphereLattice == 1){
            latticeSphereTexCoord2 = vec2(0.5*(pos[0]+1),0.5*(pos[1]+1));
        }else{
            latticeSphereTexCoord2 = vec2(0.3*(vPosition[0]+vPosition[1]+vPosition[2]),0.3*(vPosition[0]-vPosition[1]+vPosition[2]));
        }
    }
    
    
    

    fSphereLattice = sphereLattice;


}