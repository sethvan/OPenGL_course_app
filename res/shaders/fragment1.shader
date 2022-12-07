#version 330                                                                  
                                                                              
in vec4 vCol;    
in vec2 texCoord;
in vec3 Normal;                                                              
                                                                              
out vec4 colour;    

struct DirectionalLight {
    vec3 colour;
    float ambientIntensity;
    vec3 direction;
    float diffuseIntensity;
};

uniform sampler2D theTexture;

// usually directional lights represent something like the sun
uniform DirectionalLight directionalLight;
                                                                              
void main()                                                                   
{
    vec4 ambientColour = vec4(directionalLight.colour, 1.0f) * directionalLight.ambientIntensity;                                                                      
    
    // This somehow gets the angle at which norm is to the light, 0 degrees or 
    // less should not reflect and 90 degrees should most reflect. Uses dot product.
    float diffuseFactor = max(dot(normalize(Normal), normalize(directionalLight.direction)), 0);
    vec4 diffuseColour = vec4(directionalLight.colour, 1.0f) * directionalLight.diffuseIntensity * diffuseFactor;

    colour = texture(theTexture, texCoord) * (ambientColour + diffuseColour);                                         
}

