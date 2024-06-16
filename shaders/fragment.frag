out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D drawTexture;
void main()
{
    FragColor = texture(drawTexture, texCoord);
}
