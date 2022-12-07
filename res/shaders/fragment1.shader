#version 330                                                                  
                                                                              
in vec4 vCol;    
in vec2 texCoord;
in vec3 Normal;   
in vec3 fragPos;                                                           
                                                                              
out vec4 colour;    

struct DirectionalLight {
    vec3 colour;
    float ambientIntensity;
    vec3 direction;
    float diffuseIntensity;
};

struct Material {
    float specularIntensity;
    float shininess;
};

uniform sampler2D theTexture;

// Usually directional lights represent something like the sun
uniform DirectionalLight directionalLight;
uniform Material material;
// Basically the same as the camera position
uniform vec3 eyePosition;
                                                                              
void main()                                                                   
{
    vec4 ambientColour = vec4(directionalLight.colour, 1.0f) * directionalLight.ambientIntensity;                                                                      
    
    // This somehow gets the angle at which norm is to the light, 0 degrees or 
    // less should not reflect and 90 degrees should most reflect. Uses dot product.
    float diffuseFactor = max(dot(normalize(Normal), normalize(directionalLight.direction)), 0);
    vec4 diffuseColour = vec4(directionalLight.colour, 1.0f) * directionalLight.diffuseIntensity * diffuseFactor;
    vec4 specularColour = vec4(0, 0, 0, 0);

    if(diffuseFactor > 0.0f) {
        vec3 fragToEye = normalize(eyePosition - fragPos);
        vec3 reflectedVertex = normalize(reflect(directionalLight.direction, normalize(Normal)));

        float specularFactor = dot(fragToEye, reflectedVertex);
        if(specularFactor > 0.0f) {
            specularFactor = pow(specularFactor, material.shininess);
            specularColour = vec4(directionalLight.colour * material.specularIntensity * specularFactor, 1.0f);
        }
    }

    colour = texture(theTexture, texCoord) * (ambientColour + diffuseColour + specularColour);                                         
}

