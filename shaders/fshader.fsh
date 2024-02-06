uniform sampler2D u_texture;
uniform sampler2D u_texture2;
uniform highp float u_blend_percent;
uniform highp vec4 u_lightPosition;
uniform highp float u_lightPower;
varying highp vec4 v_position;
varying highp vec2 v_texcoord;
varying highp vec3 v_normal;

void main(void)
{
    vec4 resultColor = vec4(0.0,0.0,0.0,0.0);
    vec4 eyePosition = vec4(0.0,0.0,0.0,1.0);
    vec4 diffMatColor = texture2D(u_texture, v_texcoord);
    vec4 diffMatColor2 = texture2D(u_texture2, v_texcoord);

    float a = 1.0 - u_blend_percent / 100.0;
    float b = float(u_blend_percent) / 100.0;

    vec4 diffMatColorResult;
    diffMatColorResult = diffMatColor * a + diffMatColor2 * b;// + diffMatColor2 * 0);// + (diffMatColor2 * b);

    vec3 eyeVect = normalize(v_position.xyz - eyePosition.xyz);
    vec3 lightVect = normalize(v_position.xyz - u_lightPosition.xyz);
    vec3 reflectLight = normalize(reflect(lightVect, v_normal));
    float len = float(length(v_position.xyz - eyePosition.xyz));
    len += len == 0.0 ? 1.0 : 0.0;
    float specularFactor = 70.0;
    float ambientFactor = 0.1;

    vec4 diffColor = diffMatColorResult * u_lightPower * max(0.0, dot(v_normal, -lightVect)); /// (1.0 + 0.25 * len * len);
    resultColor += diffColor;
    vec4 ambientColor = ambientFactor * diffMatColorResult;
    resultColor += ambientColor;
    vec4 specularColor = vec4(1.0,1.0,1.0,1.0) * u_lightPower * pow(max(0.0, dot(reflectLight, -eyeVect)), specularFactor);

    resultColor += specularColor;
    gl_FragColor = resultColor;
}
