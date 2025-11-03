
#version 410 core

layout(location=0) in vec3 position;
layout(location=1) in vec3 vertexColors;

uniform mat4 u_ModelMatrix;
uniform mat4 u_Projection; 
uniform mat4 u_ViewMatrix;

out vec3 v_vertexColors;

void main()
{
        v_vertexColors = vertexColors;

        // here we create a vec4 with the postion vector values(rhs)
        // to be able to multiply with the u_ModelMatrix
        vec4 newPosition = u_Projection * u_ViewMatrix * u_ModelMatrix * vec4(position, 1.0f);

                                                                       // don't forget w here 
        gl_Position = vec4(newPosition.x, newPosition.y, newPosition.z, newPosition.w); 
}
