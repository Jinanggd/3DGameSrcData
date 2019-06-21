
varying vec3 v_position;
varying vec2 v_uv;
varying vec4 v_color;
varying float visibility;

uniform vec4 u_color;
uniform float u_time;
uniform bool is3D;



void main()
{
	vec2 uv = v_uv;
	
	vec4 color = v_color;
	if(is3D)
		color = mix(vec4(0.2,0.2,0.2,1.0),color,visibility);
		
	gl_FragColor = u_color * color;
}
